/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2011 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
 
/*
 * Contributed to rasdaman by Alexander Herzig, Landcare Research New Zealand
 */

// general includes
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cmath>
#include "dirent.h"
#include <vector>
#include "errno.h"
#include "sys/types.h" 
#include "unistd.h"
#include <limits>
#include "nmlog.h"
#include <algorithm>



#include "rimport.h"
#include "include/globals.hh"

using namespace std;

#ifdef DEBUG
int nmlog::nmindent = -1;
#endif

// ---------------------------------------------------- HELPERs ----------

int
getDirContent(string path, string suffix, vector<string>& names)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(path.c_str())) == NULL)
	{
		NMErr(ctxRimport, << "could not read directory '" << path << "'!" << std::endl);
		return 0;
	}

	string filename;
	while ((dirp = readdir(dp)) != NULL)
	{
		//skip directories
		if (dirp->d_type == DT_DIR)
			continue;

		if (hasSuffix(dirp->d_name, suffix))
		{
			filename = path + "/";
			filename += dirp->d_name;
			if (access(filename.c_str(), R_OK) == 0)
			{
				names.push_back(filename);
			}
			else
			{
				NMErr(ctxRimport, << "file access failure - skip image '" <<
						filename << "'!" << std::endl);
				continue;
			}
		}
	}
	closedir(dp);
	return 1;
}

bool
hasSuffix(std::string name, std::string suffix)
{
	string::size_type dpos = name.find_last_of(".", name.length() - 1);
	if (dpos == string::npos)
		return false;

	if (suffix.compare("*") == 0 || suffix.empty())
		return true;

	std::string test = name.substr(dpos + 1, name.length() - dpos + 1);
	std::string given = suffix;

	std::transform(test.begin(), test.end(), test.begin(), ::tolower);
	std::transform(given.begin(), given.end(), given.begin(), ::tolower);

	return given.compare(test) == 0 ? true : false;
}

void
readImageInformation(vector<string>& vnames, Header& header, vector<double>& bnd,
		vector<string>& vvalidtiles, bool b3D, double cellsizez)
{
	Header tileHeader;
	vector<string>::const_iterator iter;

	int numfiles = 0;
	for (iter = vnames.begin(); iter != vnames.end(); iter++)
	{
		resetHeader(tileHeader);
		if (!readTileInformation(*iter, tileHeader))
		{
			NMErr(ctxRimport, << "read tile error: skipped file '" << (*iter)
					<< "'!" << endl);
			continue;
		}

		// filter tiles
		if (bnd.size() >= 4)
		{
			// align the user specified boundary with the underlying pixel borders
			// snapping to the closest pixel border
			bnd[0] = tileHeader.cellsize.x * (int)((bnd[0] / tileHeader.cellsize.x) + 0.5);
			bnd[1] = tileHeader.cellsize.x * (int)((bnd[1] / tileHeader.cellsize.x) + 0.5);
			bnd[2] = tileHeader.cellsize.y * (int)((bnd[2] / tileHeader.cellsize.y) + 0.5);
			bnd[3] = tileHeader.cellsize.y * (int)((bnd[3] / tileHeader.cellsize.y) + 0.5);

			if (!tileOverlaps(tileHeader, bnd))
				continue;
		}
		vvalidtiles.push_back(*iter);

		// if we get a set of files, we assume that they share the
		// same data type, cell size, number of bands, and spatial
		// reference system, so we only read this info from the
		// first of all given files
		if (numfiles == 0)
		{
			if (!tileHeader.crs_name.empty())
				header.crs_name = tileHeader.crs_name;
			header.nbands = tileHeader.nbands;

			header.cellsize.x = tileHeader.cellsize.x;
			header.cellsize.y = tileHeader.cellsize.y;
			header.rmantype = tileHeader.rmantype;
			header.gdaltype = tileHeader.gdaltype;
		}

		unionRegions2D(header, tileHeader);

		// statistics: we can only take over min and max
		if (tileHeader.stats_min < header.stats_min)
			header.stats_min = tileHeader.stats_min;
		if (tileHeader.stats_max > header.stats_max)
			header.stats_max = tileHeader.stats_max;

		numfiles++;
	}

	if (numfiles > 0)
	{

		// update info we only use, when we import a single file
		if (numfiles == 1)
		{
			header.stats_mean = tileHeader.stats_mean;
			header.stats_stddev = tileHeader.stats_stddev;
		}

		// intersect joint file header with given import region (boundary)
		if (bnd.size() >= 4)
		{
			header.xmin = header.xmin > bnd[0] ? header.xmin : bnd[0];
			header.xmax = header.xmax < bnd[1] ? header.xmax : bnd[1];
			header.ymin = header.ymin > bnd[2] ? header.ymin : bnd[2];
			header.ymax = header.ymax < bnd[3] ? header.ymax : bnd[3];
		}

		// update the origin
		header.origin.x = header.xmin;
		header.origin.y = header.ymax;
		header.ncols = ((header.xmax - header.xmin) / header.cellsize.x) + 0.5;
		header.nrows = ((header.ymax - header.ymin) / header.cellsize.y) + 0.5;

		if (b3D)
		{
			if (cellsizez > 0)
				header.cellsize.z = cellsizez;
			else
				header.cellsize.z = header.cellsize.x;

			// snap the z-boundary to the nearest pixel border
			if (bnd.size() == 6)
			{
				header.zmin = header.cellsize.z * (int)((bnd[4] / header.cellsize.z) + 0.5);
				header.zmax = header.cellsize.z * (int)((bnd[5] / header.cellsize.z) + 0.5);
			}
			header.origin.z = header.zmin;
		}

	}

	NMDebug(<< "rimport successfully analysed " << numfiles << " files." << endl << endl);

}

void
printImageInformation(Header& header, RasdamanHelper2& helper)
{
	cout.precision(15);
	cout << "Update Region:" << endl;
	cout << "-------------" << endl << endl;
	cout << "coordinate reference system: " << header.crs_name << endl;
	cout << "EPSG-code: " << header.epsg_code << endl;
	cout << "bands:     " << header.nbands << endl;
	//cout << "origin:    " << header.origin.x << ", " << header.origin.y << endl;
	cout << "data type: " << helper.getDataTypeString(header.rmantype) << endl;
	cout << "cellsize:  " << header.cellsize.x << ", " << header.cellsize.y
			<< ", " << header.cellsize.z << endl;
	cout << "columns:   " << header.ncols << endl;
	cout << "rows:      " << header.nrows << endl;
	cout << "layers:    " << header.nlayers << endl;
	cout << "xmin:      " << header.xmin << endl;
	cout << "xmax:      " << header.xmax << endl;
	cout << "ymin:      " << header.ymin << endl;
	cout << "ymax:      " << header.ymax << endl;
	cout << "zmin:      " << header.zmin << endl;
	cout << "zmax:      " << header.zmax << endl;
	cout << endl;
}

void
resetHeader(Header& header)
{
	header.crs_name = "CRS:1";
	header.epsg_code = -1;
	header.origin.x = numeric_limits<double>::max();
	header.origin.y = numeric_limits<double>::max() * -1;
	header.origin.z = 0;
	header.xmin = numeric_limits<double>::max();
	header.xmax = numeric_limits<double>::max() * -1;
	header.ymin = numeric_limits<double>::max();
	header.ymax = numeric_limits<double>::max() * -1;
	header.zmin = numeric_limits<double>::max() * -1;
	header.zmax = numeric_limits<double>::max();
	header.cellsize.x = 1;
	header.cellsize.y = 1;
	header.cellsize.z = 0;
	header.ncols = -1;
	header.nrows = -1;
	header.nlayers = 1;
	header.stats_max = numeric_limits<double>::max() * -1;
	header.stats_min = numeric_limits<double>::max() * -1;
	header.stats_mean = numeric_limits<double>::max() * -1;
	header.stats_stddev = numeric_limits<double>::max() * -1;
	header.rmantype = r_Type::UNKNOWNTYPE;
	header.gdaltype = GDT_Unknown;
	header.nbands = 1;
	header.rat_avail = false;
}

r_Type::r_Type_Id
getRmanDataType(GDALDataType type)
{
	r_Type::r_Type_Id rtype;
	switch (type)
	{
	case GDT_Byte: // 	Eight bit unsigned integer
		rtype = r_Type::CHAR;
		break;
	case GDT_UInt16: // 	Sixteen bit unsigned integer
		rtype = r_Type::USHORT;
		break;
	case GDT_Int16: // 	Sixteen bit signed integer
		rtype = r_Type::SHORT;
		break;
	case GDT_UInt32: // 	Thirty two bit unsigned integer
		rtype = r_Type::ULONG;
		break;
	case GDT_Int32: // 	Thirty two bit signed integer
		rtype = r_Type::LONG;
		break;
	case GDT_Float32: // 	Thirty two bit floating point
		rtype = r_Type::FLOAT;
		break;
	case GDT_Float64: // 	Sixty four bit floating point
		rtype = r_Type::DOUBLE;
		break;
	case GDT_CInt16: // 	Complex Int16
	case GDT_CInt32: //	Complex Int32
	case GDT_CFloat32: // 	Complex Float32
	case GDT_CFloat64: // 	Complex Float64
	case GDT_Unknown: //  Unknown or unspecified type
	default:
		rtype = r_Type::UNKNOWNTYPE;
		break;
	}

	return rtype;
}

bool readTileInformation(string filename, Header& header)
{
	GDALDataset* pDs = (GDALDataset*)GDALOpen(filename.c_str(), GA_ReadOnly);

	if (pDs == 0)
		return false;

	NMDebugInd(1, << "analysing file '" << filename << "' ("
			<< pDs->GetDriver()->GetDescription() << "/" <<
	           pDs->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME) << ")" << endl);

	readTileInformation(pDs, header);

	GDALClose(pDs);
	return true;
}

bool
readTileInformation(GDALDataset* pDS, Header& header)
{
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double czx;
	double czy;

	// get image information
	double affine[6];
	pDS->GetGeoTransform(affine);
	double pixelwidth = affine[1] < 0 ? affine[1] * -1 : affine[1];
	double pixelheight = affine[5] < 0 ? affine[5] * -1 : affine[5];

	header.crs_name = pDS->GetProjectionRef();
	header.nbands = pDS->GetRasterCount();
	header.ncols = pDS->GetRasterXSize();
	header.nrows = pDS->GetRasterYSize();

	// derive tile information
	xmin = affine[0];
	xmax = affine[0] + (pixelwidth * header.ncols);
	ymin = affine[3] - (pixelheight * header.nrows);
	ymax = affine[3];
	czx = pixelwidth;//(xmax - xmin) / (float) header.ncols;
	czy = pixelheight;//(ymax - ymin) / (float) header.nrows;

	// get some statistics information
	pDS->GetRasterBand(1)->GetStatistics(0, 0,
			&header.stats_min, &header.stats_max, &header.stats_mean,
			&header.stats_stddev);

	// update header information
	header.cellsize.x = czx;
	header.cellsize.y = czy;
	header.xmin = xmin;
	header.xmax = xmax;
	header.ymin = ymin;
	header.ymax = ymax;
	header.gdaltype = pDS->GetRasterBand(1)->GetRasterDataType();
	header.rmantype = getRmanDataType(header.gdaltype);
	header.origin.x = header.xmin;
	header.origin.y = header.ymax;

	return true;
}

void intersectRegions2D(Header& inoutRegion, Header& intersectRegion)
{
	Header& io = inoutRegion;
	Header& s = intersectRegion;

	io.xmin = io.xmin > s.xmin ? io.xmin : s.xmin;
	io.xmax = io.xmax < s.xmax ? io.xmax : s.xmax;
	io.ymin = io.ymin > s.ymin ? io.ymin : s.ymin;
	io.ymax = io.ymax < s.ymax ? io.ymax : s.ymax;

	io.ncols = ((io.xmax - io.xmin) / intersectRegion.cellsize.x) + 0.5;
	io.nrows = ((io.ymax - io.ymin) / intersectRegion.cellsize.y) + 0.5;
}

void intersectRegions2D(Header& inoutRegion, std::vector<double>& intersectRegion)
{
	Header& io = inoutRegion;
	std::vector<double>& s = intersectRegion;

	io.xmin = io.xmin > s[1] ? io.xmin : s[1];
	io.xmax = io.xmax < s[2] ? io.xmax : s[2];
	io.ymin = io.ymin > s[3] ? io.ymin : s[3];
	io.ymax = io.ymax < s[4] ? io.ymax : s[4];

}

void unionRegions2D(Header& inoutRegion, Header& unionRegion)
{
	Header& io = inoutRegion;
	Header& u = unionRegion;

	io.xmin = io.xmin < u.xmin ? io.xmin : u.xmin;
	io.xmax = io.xmax > u.xmax ? io.xmax : u.xmax;
	io.ymin = io.ymin < u.ymin ? io.ymin : u.ymin;
	io.ymax = io.ymax > u.ymax ? io.ymax : u.ymax;

	io.ncols = ((io.xmax - io.xmin) / unionRegion.cellsize.x) + 0.5;
	io.nrows = ((io.ymax - io.ymin) / unionRegion.cellsize.y) + 0.5;
}

void unionRegions2D(Header& inoutRegion, std::vector<double>& unionRegion)
{
	Header& io = inoutRegion;
	std::vector<double>& u = unionRegion;

	io.xmin = io.xmin < u[0] ? io.xmin : u[0];
	io.xmax = io.xmax > u[1] ? io.xmax : u[1];
	io.ymin = io.ymin < u[2] ? io.ymin : u[2];
	io.ymax = io.ymax > u[3] ? io.ymax : u[3];
}

void copyRegion2D(Header& outRegion, Header& inRegion)
{
	outRegion.xmin = inRegion.xmin;
	outRegion.xmax = inRegion.xmax;
	outRegion.ymin = inRegion.ymin;
	outRegion.ymax = inRegion.ymax;
}

void copyRegion2D(Header& outRegion, std::vector<double>& inRegion)
{
	outRegion.xmin = inRegion[0];
	outRegion.xmax = inRegion[1];
	outRegion.ymin = inRegion[2];
	outRegion.ymax = inRegion[3];
}

int
processImageFiles(vector<string>& filenames, string collname,
		vector<double>& oids, Header& processRegion,
		string mode3D, r_Point& shiftPt, RasdamanHelper2& helper)
{
	/* PROCEDURE
	 * - read source geospatial region (srcGeoRegion)
	 * - intersect with user specified boundary (i.e. processRegion) -> insertGeoRegion
	 * - determine the source region in image (pixel) space to be read -> readGDALImgDOM
	 * - shift the insertGeoRegion by the user specified shift vector (shiftPt)
	 * - determine the appropriate shift vector (writeShift) for writing the source region into the
	 *   rasdaman data base (only applies when in update mode)
	 * - in 3D mode: determine the new z-value depending on the specified parameter
	 *   (i.e. top or bottom), the user specified shift vector, and whether we're in updating
	 *   mode or not
	 * - import the image (importImage deals with sequential and band processing)
	 * - write RATs
	 * - write petascope meta data
	 */


	NMDebugCtx(ctxRimport, << "...");

	bool b3D = (!mode3D.empty() || shiftPt.dimension() == 3) ? true : false;

	Header newGeoRegion;

	int tilecounter = 1;
	vector<string>::const_iterator iter;
	for (iter = filenames.begin(); iter != filenames.end(); ++iter, ++tilecounter)
	{
			bool bUpdate = oids.size() == 0 ? false : true;

			GDALDataset* pDs = (GDALDataset*)GDALOpen((*iter).c_str(), GA_ReadOnly);
			if (pDs == 0)
			{
				NMErr(ctxRimport, << "failed opening data set '" << *iter << "'!");
				return 0;
			}

			Header srcGeoRegion;
			resetHeader(srcGeoRegion);
			readTileInformation(pDs, srcGeoRegion);

			printRegion(srcGeoRegion, "srcGeoRegion");
			printRegion(processRegion, "processRegion");

			Header insertGeoRegion;
			resetHeader(insertGeoRegion);
			copyRegion2D(insertGeoRegion, srcGeoRegion);

			intersectRegions2D(insertGeoRegion, processRegion);

			printRegion(insertGeoRegion, "insertGeoRegion");

			// determine the image region to be read from the src in pixel space
			r_Long read_scol = ((insertGeoRegion.xmin - srcGeoRegion.xmin) / srcGeoRegion.cellsize.x) + 0.5;
			r_Long read_srow = ((srcGeoRegion.ymax - insertGeoRegion.ymax) / srcGeoRegion.cellsize.y) + 0.5;
			r_Minterval readGDALImgDOM(2);
			readGDALImgDOM	<< r_Sinterval(read_scol, (r_Long)(read_scol + insertGeoRegion.ncols - 1))
						    << r_Sinterval(read_srow, (r_Long)(read_srow + insertGeoRegion.nrows - 1));

			NMDebugAI(<< "src img size:     " << srcGeoRegion.ncols << " x " << srcGeoRegion.nrows << endl);
			NMDebugAI(<< "readGDALImgDOM:   " << readGDALImgDOM.get_string_representation() << endl << endl);

			// shift the insertGeoRegion by the user specified shiftVector; account for (geo) spatial
			// image domains with negative values: positive shift (e.g. 150:200) shifts to the north east
			// and negative shift values, e.g. -150:-200, shift to the south west (applies to most
			// projected coordinate reference systems)
			if (shiftPt.dimension() >= 2)
			{
				double xfactor = fabs(insertGeoRegion.xmin) > fabs(insertGeoRegion.xmax) ? -1 : 1;
				double yfactor = fabs(insertGeoRegion.ymin) > fabs(insertGeoRegion.ymax) ? -1 : 1;

				insertGeoRegion.xmin += (shiftPt[0] * xfactor * processRegion.cellsize.x);
				insertGeoRegion.xmax += (shiftPt[0] * xfactor * processRegion.cellsize.x);
				insertGeoRegion.ymin += (shiftPt[1] * yfactor * processRegion.cellsize.y);
				insertGeoRegion.ymax += (shiftPt[1] * yfactor * processRegion.cellsize.y);
			}

			printRegion(insertGeoRegion, "shifted insertGeoRegion");

			// determine the shift vector (relative to the current (present) image domain)
			// to apply while writing the src image into the rasdaman data base
			vector<double> isdom = helper.getNMMetaGeoDomain(oids.size() >= 1 ? oids[0] : -1);

			resetHeader(newGeoRegion);
			copyRegion2D(newGeoRegion, insertGeoRegion);
			unionRegions2D(newGeoRegion, isdom);
			newGeoRegion.zmin = processRegion.zmin;
			newGeoRegion.zmax = processRegion.zmax;

			r_Point writeShift;
			int zshift = 0;
			double newZ = 0;
			if (bUpdate)
			{
				r_Minterval aint = helper.getImageSdom(collname, oids[0]);
				if (b3D)
				{
					// if we've got a z-shift given, we always shift relative to
					// z = 0
					if (shiftPt.dimension() == 3)
					{
						if (mode3D == "top")
							zshift = (shiftPt[2] + tilecounter - 1) + 0.5;
						else if (mode3D == "bottom")
							zshift = (shiftPt[2] - tilecounter - 1) + 0.5;
					}
					else
					{
						if (mode3D == "top")
							zshift = aint[2].high() + 1;
						else if (mode3D == "bottom")
							zshift = aint[2].low() - 1;
					}

					writeShift = r_Point(3)
							<< aint[0].low() + ((insertGeoRegion.xmin - isdom[0]) / processRegion.cellsize.x) + 0.5
							<< aint[1].low() + ((isdom[3] - insertGeoRegion.ymax) / processRegion.cellsize.y) + 0.5
							<< zshift;
				}
				else
				{
					writeShift = r_Point(2)
							<< aint[0].low() + ((insertGeoRegion.xmin - isdom[0]) / processRegion.cellsize.x) + 0.5
							<< aint[1].low() + ((isdom[3] - insertGeoRegion.ymax) / processRegion.cellsize.y) + 0.5;
				}
			}
			else
			{
				if (b3D)
				{
					// if we've got a z-shift given, we always shift relative to
					// z = 0
					if (shiftPt.dimension() == 3)
					{
						if (mode3D == "top")
							zshift = shiftPt[2] + tilecounter - 1 + 0.5;
						else if (mode3D == "bottom")
							zshift = shiftPt[2] - tilecounter - 1 + 0.5;
					}
					else
					{
						if (mode3D == "top")
							zshift = tilecounter -1;
						else if (mode3D == "bottom")
							zshift = -1 * (tilecounter -1);
					}

					writeShift = r_Point(3) << 0 << 0 << zshift;
				}
				else
				{
					writeShift = r_Point(2) << 0 << 0;
				}
			}
			newZ = zshift * processRegion.cellsize.z;
			NMDebugAI(<< "writeShift: " << writeShift.get_string_representation() << endl << endl);

			// determine the new image region (valid after the src has been written into
			// the data base) by union the present image region with the region to
			// be inserted; furthermore copy all other info from the processRegion into
			// the newGeoRegion

			// determine z min and max
			if (b3D || shiftPt.dimension() == 3)
			{
				if ( newZ < newGeoRegion.zmin || newZ > newGeoRegion.zmax )
				{
					NMDebugAI(<< "import layer outside z boundary, abortion!" << std::endl);
					continue;
				}

				if (!bUpdate)
				{
					newGeoRegion.zmin = newGeoRegion.zmax = newZ;
					NMDebugAI( << "inserting 1st 3D slice at zcoord: " << newZ << endl);
				}
				else
				{
					NMDebugAI( << "inserting 2nd+ 3D slice | newZ: " << newZ << endl);
					newGeoRegion.zmin = (newZ < isdom[4]) ? newZ : isdom[4];
					NMDebugAI( << "new zmin: " << newGeoRegion.zmin << endl);

					newGeoRegion.zmax = (newZ > isdom[5]) ? newZ : isdom[5];
					NMDebugAI( << "new zmax: " << newGeoRegion.zmax << endl);
				}
			}

			newGeoRegion.cellsize.x = processRegion.cellsize.x;
			newGeoRegion.cellsize.y = processRegion.cellsize.y;
			newGeoRegion.cellsize.z = processRegion.cellsize.z;
			newGeoRegion.ncols = ((newGeoRegion.xmax - newGeoRegion.xmin) / newGeoRegion.cellsize.x) + 0.5;
			newGeoRegion.nrows = ((newGeoRegion.ymax - newGeoRegion.ymin) / newGeoRegion.cellsize.y) + 0.5;
			newGeoRegion.nlayers = ((newGeoRegion.zmax - newGeoRegion.zmin) / newGeoRegion.cellsize.z) + 0.5;
			newGeoRegion.stats_max = processRegion.stats_max;
			newGeoRegion.stats_min = processRegion.stats_min;
			newGeoRegion.stats_mean = processRegion.stats_mean;
			newGeoRegion.stats_stddev = processRegion.stats_stddev;
			newGeoRegion.epsg_code = processRegion.epsg_code;
			newGeoRegion.crs_name = processRegion.crs_name;
			newGeoRegion.rmantype = processRegion.rmantype;
			newGeoRegion.gdaltype = srcGeoRegion.gdaltype;
			newGeoRegion.nbands = srcGeoRegion.nbands;

			printRegion(newGeoRegion, "newGeoRegion");

			importImage(helper, pDs, collname, oids, readGDALImgDOM, writeShift,
					newGeoRegion, b3D);

			// release data set
			GDALClose(pDs);

			// since we don't support merging of RATs, we stay on the safe side and
			// don't import RAT at all in mosaicing mode
			if (filenames.size() == 1 && !bUpdate)
			{
				for (int v=0; v < oids.size(); ++v)
				{
					for (int b=1; b <= processRegion.nbands; ++b)
					{
						helper.writeNMRAT(*iter, oids[v], b);
					}
				}
			}
	}

	// update the petascope meta data with the newGeoRegion
	helper.writePSMetadata(collname, newGeoRegion.crs_name, newGeoRegion.rmantype,
			newGeoRegion.xmin, newGeoRegion.xmax, newGeoRegion.ymin, newGeoRegion.ymax,
			newGeoRegion.zmin, newGeoRegion.zmax, newGeoRegion.ncols, newGeoRegion.nrows, newGeoRegion.nlayers);


	NMDebugCtx(ctxRimport, << "done!");
	return 1;
}

int importImage(RasdamanHelper2& helper, GDALDataset* pDs, string& collname, vector<double>& oids,
		r_Minterval& readGDALImgDOM, r_Point& writeShift, Header& newGeoRegion,
		bool asCube)
{
	NMDebugCtx(ctxRimport, << "...");

	// get the pixel's data type length (in bytes)
	unsigned int pixelsize = helper.getTypeSize(newGeoRegion.rmantype);

	// determine parameters for sequential processing
	unsigned long chunksize = helper.getMaxImgSize() /
			(readGDALImgDOM[0].get_extent() * pixelsize);
	if (chunksize > readGDALImgDOM[1].get_extent())
		chunksize = readGDALImgDOM[1].get_extent();

	// prepare sequential processing variables
	unsigned long niter = chunksize == 0 ? readGDALImgDOM[1].get_extent() : readGDALImgDOM[1].get_extent() / chunksize;
	unsigned long rest = readGDALImgDOM[1].get_extent() - (niter * chunksize);
	unsigned long imgsize_bytes = pixelsize * readGDALImgDOM[0].get_extent() * readGDALImgDOM[1].get_extent();
	double imgsize_mib = (imgsize_bytes /  (1024.0 * 1024.0));

	// some debug output
	NMDebugInd(2, << "...size of import region: " << readGDALImgDOM[0].get_extent() << "x" << readGDALImgDOM[1].get_extent()
		 << "x" << pixelsize << " = " << imgsize_mib << " MiB" << std::endl);
	NMDebugInd(2, << "...processing scheme: " << niter << " x " << chunksize << " + " << rest
		 << " rows" << std::endl);

	// process individual bands
	for (int b=1; b <= newGeoRegion.nbands; b++)
	{
		// prepare a sequentially updated writeShift vector accounting for row-wise input
		r_Point seqWriteShift = r_Point(writeShift);

		// calc the sequential read variables
		int startcolumn = readGDALImgDOM[0].low();
		int startrow = readGDALImgDOM[1].low();
		int endrow = startrow + chunksize -1;
		int rowstoread = chunksize;

		for (int iter=0; iter <= niter; iter++)
		{
			NMDebugAI(<< "importing chunk " << iter+1 << " of " << (rest > 0 ? niter+1 : niter) <<
					": row " << seqWriteShift[1] << " to " << seqWriteShift[1] + rowstoread-1 << endl << endl);

			// create the interval object for writing the image buffer to rasdaman
			r_Minterval rint;
			if (asCube)
			{
				rint = r_Minterval(3) << r_Sinterval(0, (r_Long)readGDALImgDOM[0].get_extent()-1)
									  << r_Sinterval(0, (r_Long)rowstoread-1)
									  << r_Sinterval(0, 0);
			}
			else
			{
				rint = r_Minterval(2) << r_Sinterval(0, (r_Long)readGDALImgDOM[0].get_extent()-1)
									  << r_Sinterval(0, (r_Long)rowstoread-1);
			}

			// allocate the memory for the 'transfer buffer'
			void* gdalbuf = CPLMalloc(pixelsize * readGDALImgDOM[0].get_extent() * rowstoread);
			if (gdalbuf == NULL)
			{
				NMErr(ctxRimport, << "memory allocation failed!");
				GDALClose(pDs);
				return 0;
			}

			// read image buffer from file
			GDALRasterBand* pBand = pDs->GetRasterBand(b);
			pBand->RasterIO(GF_Read, startcolumn, startrow, readGDALImgDOM[0].get_extent(), rowstoread,
					gdalbuf, readGDALImgDOM[0].get_extent(), rowstoread, newGeoRegion.gdaltype, 0, 0);

			if (iter == 0 && oids.size() < b)
				oids.push_back(helper.insertImage(collname, (char*)gdalbuf, seqWriteShift, rint, true));
			else
				helper.updateImage(collname, oids[b-1], (char*)gdalbuf, seqWriteShift, rint, true);

			// release memory of the reading buffer
			free(gdalbuf);

			// adjust the shift vector by the just written number of rows;
			// (will be only effective while sequential processing
			seqWriteShift[1] += rowstoread;

			// calc new iteration vars for sequential buffer reading and writing
			startrow = endrow + 1;
			if (iter == niter-1)
			{
				if (rest > 0)
				{
					endrow += rest;
					rowstoread = rest;
				}
				else
					break;
			}
			else
				endrow += chunksize;

		} // end sequential processing

		helper.writeNMMetadata(collname, oids[b-1], newGeoRegion.epsg_code,
			newGeoRegion.crs_name, newGeoRegion.xmin, newGeoRegion.xmax,
			newGeoRegion.ymin, newGeoRegion.ymax, newGeoRegion.zmin,
			newGeoRegion.zmax, newGeoRegion.cellsize.x,
			newGeoRegion.cellsize.y, newGeoRegion.cellsize.z,
			helper.getDataTypeString(newGeoRegion.rmantype),
			newGeoRegion.stats_min, newGeoRegion.stats_max,
			newGeoRegion.stats_mean, newGeoRegion.stats_stddev,
			"");

	} // end band processing

	NMDebugCtx(ctxRimport, << "done!");
	return 1;
}


bool
tileOverlaps(Header& header, vector<double>& bnd)
{
	// we only consider the 2D x/y case because GDAL only supports
	// 2D xy coordinates and all given regions are axis-parallel of course

	// layout of bnd
	// bnd[0] = xmin; bnd[1] = xmax;
	// bnd[2] = ymin; bnd[3] = ymax;
	// bnd[4] = zmin; bnd[5] = zmax;

	NMDebugCtx(ctxRimport, << "...");

	NMDebugAI(<< "testing the following regions ... " << endl);
	printRegion(header, "input image:");
	printRegion(bnd, "user bnd:");

	bool xoverlap = false;
	bool yoverlap = false;

	if ( (header.xmin >= bnd[0] && bnd[1] >= header.xmin) ||
		 (bnd[0] >= header.xmin && header.xmax >= bnd[0])    )
	{
		xoverlap = true;
	}

	if ( (header.ymin >= bnd[2] && bnd[3] >= header.ymin) ||
		 (bnd[2] >= header.ymin && header.ymax >= bnd[2]))
	{
		yoverlap = true;
	}

	NMDebugAI( << "do regions overlap? -> " << ((xoverlap && yoverlap) ? "yes" : "no") << endl << endl);

	NMDebugCtx(ctxRimport, << "done!");
	return (xoverlap && yoverlap) ? true : false;
}

bool parseCoordinateString(string bndstr, vector<double>& bnd)
{
	//NMDebugCtx(ctxRimport, << "...");

	size_t startpos = 0;
	size_t endpos = 0;
	string sub;
	while ((endpos = bndstr.find(':', startpos)) != string::npos)
	{
		sub = bndstr.substr(startpos, endpos-startpos);
		//NMDebugInd(1, << "start: " << startpos << "  endpos: " << endpos <<
		//		"  substring: " << sub << endl);
		bnd.push_back(atof(sub.c_str()));
		startpos = endpos+1;
	}
	// get the last coordinate
	if (startpos != 0)
	{
		endpos = bndstr.size()-1;
		sub = bndstr.substr(startpos, endpos-startpos+1);
		//NMDebugInd(1, << "start: " << startpos << "  endpos: " << endpos <<
		//		"  substring: " << sub << endl);
		bnd.push_back(atof(sub.c_str()));
	}
	else if (startpos == 0 && bndstr.size() != 0)
	{
		bnd.push_back(atof(bndstr.c_str()));
	}
	else
		return false;

	//NMDebugCtx(ctxRimport, << "done!");
	return true;
}


void printRegion(Header& reg, string descr)
{
#ifdef DEBUG
	std::cout.precision(9);
	string s = "    ";

	std::cout << s << descr << std::endl;
	std::cout << s << reg.xmin << s << reg.xmax << std::endl;
	std::cout << s << reg.ymin << s << reg.ymax << std::endl;
	std::cout << s << reg.zmin << s << reg.zmax << std::endl;
	std::cout << std::endl;
#endif
}

void printRegion(std::vector<double>& sdom, string descr)
{
#ifdef DEBUG
	std::cout.precision(9);
	string s = "    ";

	std::cout << s << descr << std::endl;
	std::cout << s << sdom[0] << s << sdom[1] << std::endl;
	std::cout << s << sdom[2] << s << sdom[3] << std::endl;
	std::cout << s << sdom[4] << s << sdom[5] << std::endl;
	std::cout << std::endl;
#endif
}



void showHelp()
{
	std::cout << std::endl << "rasimport v0.3" << std::endl << std::endl;

	std::cout << "Usage: rasimport {-f <image file name> | -d <image directory> "
			     "[-s <tiff | img | jpeg | ... ]} -coll <collection name> "
				 "[-conn <connection file>] [-3D <top | bottom> [-csz <z-axis cell size>]] "
				 "[-bnd <xmin:xmax:ymin:ymax[:zmin:zmax]>] [-oid <local_image_OID[:local_image_OID[: ... ]]>] "
				 "[-shift <x:y[:z]>]" << std::endl << std::endl;

	std::cout << "   -f:     path to image file" << std::endl;
	std::cout << "   -d:     path pointing to image directory" << std::endl;
	std::cout << "   -s:     filter files in directory ('-d') by the given suffix; if omitted, all files are considered" << std::endl;
	std::cout << "   -coll:  name of target rasdaman collection" << std::endl;
	std::cout << "   -conn:  connection file specifying rasdaman and postgres DB connection parameters" << std::endl;
	std::cout << "   -3D:    mode for 2D (x/y) image slice import into 3D cubes" << std::endl;
	std::cout << "   -csz:   z-axis cell size; if omitted, rimport assumes x-, y-, and z-cell sizes are identical!" << std::endl;
	std::cout << "   -bnd:   spatial import boundary (e.g. xmin:xmax:ymin:ymax)" << std::endl;
	std::cout << "   -oid:   local object identifier(s) (OID(s)) specifying the target image(s) of an update operation" << std::endl;
	std::cout << "   -shift: shifts the origin of the import image by the specified vector (e.g. x:y)" << std::endl << std::endl;

	std::cout << "Note: Coordinates have to be given in the appropriate (geospatial) coordinate reference system of the image(s)!" << std::endl;


	std::cout << std::endl;
}

// ----------------------------------------- MAIN ------------------------------------------------

int
main(int argc, char** argv)
{
	NMDebugCtx(ctxRimport, << "...");

	// show help if no arguments are passed
	if (argc < 2)
	{
		showHelp();
		return EXIT_SUCCESS;
	}

	// declare variables for command line arguments

	std::string filepath;		 								 // -f
	std::string dirpath; 		 								 // -d
	std::string suffix;			 								 // -s
	std::string collname;		 								 // -coll
	std::string connfile;							     		 // -conn
	std::string mode3D;											 // -3D

	double cellsizez = -1;									     // -csz
	std::vector<double> bnd;									 // -bnd
	std::vector<double> oids;										 // -oid
	std::vector<double> shift; 								     // -shift

	// parse command line arguments
	int arg = 1;
	while (arg < argc-1)
	{
		std::string theArg = argv[arg];
		std::transform(theArg.begin(), theArg.end(),
				theArg.begin(), ::tolower);

		if (theArg == "-f")
		{
			filepath = argv[arg+1];
			if (::access(filepath.c_str(), R_OK) != 0)
			{
				NMErr(ctxRimport, << "invalid parameter for -f: could not "
						"access file '" << filepath << "'!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-d")
		{
			dirpath = argv[arg+1];
			// remove trailing slash
			if (dirpath.at(dirpath.length() - 1) == '/')
				dirpath = dirpath.substr(0, dirpath.length() - 1);
			DIR* dirp = opendir(dirpath.c_str());
			if (dirp == NULL)
			{
				NMErr(ctxRimport, << "invalid parameter for -d: could not "
						"access directory '" << dirpath << "'!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-s")
		{
			suffix = argv[arg+1];
		}
		else if (theArg == "-coll")
		{
			collname = argv[arg+1];
			if (collname.empty())
			{
				NMErr(ctxRimport, << "missing parameter for -coll: please "
						"specify a target collection!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-conn")
		{
			connfile = argv[arg+1];
			if (access(connfile.c_str(), R_OK) != 0)
			{
				NMErr(ctxRimport, << "invalid parameter for -conn: could "
						"not access connection file '" << connfile << "'!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-3d")
		{
			mode3D = argv[arg+1];
			std::transform(mode3D.begin(), mode3D.end(), mode3D.begin(),
					::tolower);
			if (mode3D != "top" && mode3D != "bottom")
			{
				NMErr(ctxRimport, << "invalid parameter for -3D: valid "
						"parameters are {top | bottom}!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-bnd")
		{
			std::string tmp = argv[arg+1];
			if (!parseCoordinateString(tmp, bnd))
			{
				NMErr(ctxRimport, << "invalid parameter for -bnd: "
						"failed reading spatial boundary!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-oid")
		{
			std::string tmp = argv[arg+1];
			std::vector<double> tmpoids;
			if (!parseCoordinateString(tmp, tmpoids))
			{
				NMErr(ctxRimport, << "invalid parameter for -oid: "
						"failed reading object identifier(s)!");
				return EXIT_FAILURE;
			}

			// copy valid oids into final vector
			for (int t=0; t < tmpoids.size(); ++t)
			{
				if (tmpoids[t] > 0)
					oids.push_back(tmpoids[t]);
			}
		}
		else if (theArg == "-csz")
		{
			cellsizez = atof(argv[arg+1]);
			if (cellsizez <= 0)
			{
				NMErr(ctxRimport, << "invalid parameter for -csz: "
						"z-axis cell size must be numeric and > 0!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-shift")
		{
			std::string tmp = argv[arg+1];
			if (!parseCoordinateString(tmp, shift))
			{
				NMErr(ctxRimport, << "invalid parameter for -shift: "
						"failed reading shift coordinates!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "--help")
		{
			showHelp();
			return EXIT_SUCCESS;
		}
		arg++;
	}

	// ---------------------------------------------------------------------
	// let's see what we've got so far
	NMDebugInd(1, << "filepath: " << filepath << std::endl);
	NMDebugInd(1, << "dirpath: " << dirpath << std::endl);
	NMDebugInd(1, << "suffix: " << suffix << std::endl);
	NMDebugInd(1, << "collname: " << collname << std::endl);
	NMDebugInd(1, << "connfile: " << connfile << std::endl);
	NMDebugInd(1, << "mode3D: " << mode3D << std::endl);
	NMDebugInd(1, << "csz: " << cellsizez << std::endl);

	NMDebugInd(1, << "bnd: ");
	for (int i=0; i < bnd.size(); i++)
		NMDebug(<< bnd[i] << " ");
	NMDebug(<< std::endl);

	NMDebugInd(1, << "oid: ");
	for (int v=0; v < oids.size(); v++)
		NMDebug(<< oids[v] << " ");
	NMDebug(<< std::endl);

	NMDebugInd(1, << "shift: ");
	for (int s=0; s < shift.size(); s++)
		NMDebug(<< shift[s] << " ");
	NMDebug(<< std::endl);

	// -----------------------------------------------------------------------
	// EVALUATE ARGUMENTS
	// get the connection file and check readability
	if (connfile.empty())
	{
		connfile = string(getenv("HOME")) + "/" + RAS_USER_RESOURCEDIR + "/rasconnect";
		if (access(connfile.c_str(), R_OK) != 0)
		{
			NMErr(ctxRimport, << "could not access connection file '"
					<< connfile << "'!");
			return EXIT_FAILURE;
		}
	}

	// create the list of filenames
	std::vector< std::string > vnames;
	if (!filepath.empty())
	{
		vnames.push_back(filepath);
	}
	else if (!dirpath.empty())
	{
		getDirContent(dirpath, suffix, vnames);
	}

	if (vnames.size() == 0)
	{
		NMErr(ctxRimport, << "No input files specified!");
		showHelp();
		return EXIT_FAILURE;
	}

    // initiate gdal
	GDALAllRegister();

	// read input image(s) --> populate Header structure
	// overall processing region (applies to multiple files)
	Header header;
	resetHeader(header);

	bool b3D = (!mode3D.empty() || shift.size() == 3) ? true : false;
	vector< string > vimportnames;
	readImageInformation(vnames, header, bnd,
			vimportnames, b3D, cellsizez);
	if (vimportnames.size() == 0)
	{
		NMErr(ctxRimport, << "Empty import region!" << std::endl);
		return EXIT_FAILURE;
	}

	// -----------------------------------------------------------------------
	// IMPORT IMAGE(S)
	try
	{
		RasdamanConnector rasconn(connfile);
		RasdamanHelper2 helper(&rasconn);
		//printImageInformation(header, helper);

		// check, whether the collection already exists
		if (helper.doesCollectionExist(collname) < 0)
		{
			helper.insertCollection(collname, header.rmantype,
					!mode3D.empty());
		}

		// note here shift is in pixel coordinates
		r_Point shiftPt;
		if (shift.size() == 2)
		{
			shiftPt = r_Point(2) << (r_Range)(shift[0] / header.cellsize.x)
				            	 << (r_Range)(shift[1] / header.cellsize.y);
		}
		else if (shift.size() == 3)
		{
			shiftPt = r_Point(3) << (r_Range)(shift[0] / header.cellsize.x)
				            	 << (r_Range)(shift[1] / header.cellsize.y)
				                 << (r_Range)(shift[2] / header.cellsize.z);
		}

		if (!processImageFiles(vimportnames, collname, oids, header, mode3D,
				shiftPt, helper))
		{
			NMErr(ctxRimport, << "Sorry, but something went wrong!");
			return EXIT_FAILURE;
		}
	}
	catch (r_Error& re)
	{
		NMErr(ctxRimport, << re.what());
	}

	NMDebugCtx(ctxRimport, << "done!");
	return EXIT_SUCCESS;
}
