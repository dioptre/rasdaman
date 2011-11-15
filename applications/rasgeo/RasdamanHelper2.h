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

/*!
 * \brief	Helper class for handling rasdaman collections and images.
 *
 * This class facilitates handling of rasdaman collections by
 * hiding especially rasml queries (r_oql_execute() calls)
 * including transaction handling from the user and attempts
 * to provide a 'non-db-like' interface to rasdaman collections and
 * images.
 *
 * \see RasdamanConnector
 */

#ifndef RASDAMANHELPER2_H_
#define RASDAMANHELPER2_H_

// general includes
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// the rasdaman basic data types
#include "rasdaman.hh"

#ifdef ORFEOMAN
	// otb includes
	#include "itkImageIOBase.h"
#endif

// other includes
#include "RasdamanConnector.h"

// postgresql
#include "libpq-fe.h"

// gdal
#include "gdal_priv.h"
#include "gdal_rat.h"

class RasdamanHelper2
{
public:

	/*! Constructor taking a RasdamanConnector object.
	 *
	 * 	\param rasconn pointer to RasdamanConnector object, which
	 * 	               controls the connection to the underlying
	 * 	               rasdaman data base. Note that the connector
	 * 	               object may be shared with other helper objects.
	 */
	RasdamanHelper2(RasdamanConnector* rasconn);
	virtual ~RasdamanHelper2();

	/*! checks whether the data base contains the specified collection
	 *
	 *  \param collname collection name
	 *  \return when the collection exists, the local OID is returned,
	 *  		otherwise -1;
	 */
	double doesCollectionExist(std::string collname) throw (r_Error);

	/*! Inserts a new collection 'collname' of type rtype into the data base */
	void insertCollection(std::string collname, r_Type::r_Type_Id rtype, bool asCube);

	/*! Deletes collection 'collname' from the data base. */
	void dropCollection(std::string collname);

	/*! Deletes an image from a collection. */
	void dropImage(std::string collname, double oid);

	/*! Returns a collection's (local) image OIDs */
	std::vector<double> getImageOIDs(std::string collname);

	/*! Queries the spatial domain of the given image
	 *
	 *	\param collname name of collection the particular image belongs to
	 *	\param localImgOID local OID of the image for which the spatial domain
	 *	       is requested
	 * 	\return a r_Minterval object specifying the spatial cell domain of the image
	 */
	r_Minterval getImageSdom(std::string collname, double localImgOID);

	/*! Returns the data (pixel) type id of the given collection. */
	r_Type::r_Type_Id getBaseTypeId(std::string collname);

	/*! Returns the base type of the collection */
	std::string getBaseTypeName(std::string collname);

	/*! Returns the size in bytes of the collection's base type */
	unsigned int getBaseTypeSize(std::string collname);

	unsigned int getTypeSize(r_Type::r_Type_Id rtype) throw (r_Error);

	/*! returns the string representation (e.g. 'Grey', 'ULong') of rtype
	 *  which can be used as prefix to build collection and image type strings
	 *  such as 'GreySet', 'UShortImage', etc.
	 */
	std::string getTypePrefixString(r_Type::r_Type_Id rtype);

	// perhaps we don't need this.
	//	bool setComponentType(r_Type::r_Type_Id rtype);

	/*! Copies a given section of a particular image.
	 *
	 *  \param collname The name of the collection the image belongs to.
	 *  \param localImgOID The image's local OID
	 *  \param buf	Pointer to an allocated array the image values are copied into.
	 *  \param sdom An r_Minterval object specifying the spatial domain of the
	 *  			image section to be copied into buf.
	 */
	void getImageBuffer(std::string collname, double localImgOID, char* buf,
			r_Minterval& sdom);

	/*! insert an image into the collection */
	double insertImage(std::string collname, char* buf,
			r_Point& shift, r_Minterval& sdom, bool bRowMajor2ColMajor);

	/*! update an already present image in the collection */
	void updateImage(std::string collname, double imgid,
		char* buf, r_Point& shift, r_Minterval& sdom, bool bRowMajor2ColMajor);

	/*! Transposes a 1D, 2D, or 3D column-major arrays into a row-major array
	 *
	 *	\param colbuf column-major array
	 *	\param rowbuf row-major array
	 *	\param rtype data type of array elements
	 *	\param ncols number of columns of the data set (#pixel in x-direction)
	 *	\param nrows number of rows of the data set (#pixel in y-direction)
	 *	\param nlayers number of layers of the data set (#pixel in z-direction)
	 */
	void colBuf2RowBuf(char* colbuf, char* rowbuf, r_Type::r_Type_Id rtype,
			int ncols, int nrows, int nlayers) throw (r_Error);

	/*! Transposes a n-dimensional column-major array into a row-major array
	 *
	 *	\param colbuf column-major array
	 *	\param rowbuf row-major array
	 *	\param pixelsize size of array elements in byte
	 *	\param sdom dimensions of the array
	 */
	void colBuf2RowBuf(char* colbuf, char* rowbuf, unsigned int pixelsize,
			r_Minterval& sdom);

	/*! Transposes a 1D, 2D, or 3D row-major array into a column-major array
	 *
	 *	\param colbuf column-major array
	 *	\param rowbuf row-major array
	 *	\param rtype data type of array elements
	 *	\param ncols number of columns of the data set (#pixel in x-direction)
	 *	\param nrows number of rows of the data set (#pixel in y-direction)
	 *	\param nlayers number of layers of the data set (#pixel in z-direction)
	 */
	void rowBuf2ColBuf(char* rowbuf, char* colbuf, r_Type::r_Type_Id rtype,
			int ncols, int nrows, int nlayers) throw (r_Error);

	/*! Transposes a n-dimensional row-major array into a column-major array
	 *
	 *	\param colbuf column-major array
	 *	\param rowbuf row-major array
	 *	\param pixelsize size of array elements in byte
	 *	\param sdom dimensions of the array
	 */
	void rowBuf2ColBuf(char* rowbuf, char* colbuf, unsigned int pixelsize,
			r_Minterval& sdom);

	/*! Calculates the n-dimensional 0-based index of an array element
	 *  given its 0-based index (offset) within the underlying 1D array.
	 *
	 *  The method requires the dimensions being specified starting
	 *  with the fastest moving index (i.e. {column, row, layer, ...}
	 *  for row-major arrays and {row, column, layer, ...} for column-major
	 *  arrays).
	 */
	std::vector<int> offset2index(int offset, std::vector<int>& sdom);

	/*! Calculates n-dimensional 0-based index of an array element
	 *  given its 0-based index (offset) within the underlying 1D array */
	std::vector<int> offset2index(int offset, r_Minterval& sdom);

	int index2offset(r_Minterval& sdom, std::vector<int>& index);
	int index2offset(std::vector<int>& sdom, std::vector<int>& index);

	void createCollection(r_Database& db, r_Ref< r_Set< r_Ref< r_GMarray > > >& imgSet,
			r_Type::r_Type_Id rtype, bool asCube) throw (r_Error);

	void createMDD(r_Ref< r_GMarray >& image, r_Minterval sdom,
			r_Type::r_Type_Id rtype) throw (r_Error);

	std::string getNumConstChar(r_Type::r_Type_Id rtype) throw (r_Error);
	int getWCPSTypeId(r_Type::r_Type_Id rtype);
	std::string getDataTypeString(r_Type::r_Type_Id rtype);

	long getMaxImgSize(void) {return this->m_maximgsize;};
	void setMaxImgSize(long maximgsize) {this->m_maximgsize = maximgsize;};

	bool isNMMetaAvailable(void);

	void writeNMMetadata(
			string collname,
			long oid,
			long epsgcode,
			string crsname,
			double minx,
			double maxx,
			double miny,
			double maxy,
			double minz,
			double maxz,
			double csx,
			double csy,
			double csz,
			string pixeltype,
			double stats_min,
			double stats_max,
			double stats_mean,
			double stats_stddev,
			string RATName);

	int writeNMRAT(std::string filename, double oid, int band);
	int writePSMetadata(
			std::string collname,
			std::string crs,
			r_Type::r_Type_Id rtype,
			double xmin,
			double xmax,
			double ymin,
			double ymax,
			double zmax,
			double zmin,
			int xpix, int ypix, int zpix);

	std::vector<double> getNMMetaGeoDomain(long oid);
	std::vector<double> getNMMetaCellSize(long oid);
	std::string getNMMetaCrsName(long oid);

	int deletePSMetadata(std::string collname);
	int deleteNMMetadata(std::string collname, double oid);


	bool checkDbConnection(void);

protected:
	//! pointer to the connection object
	RasdamanConnector* m_pRasconn;
	//! private transaction object of this helper
	r_Transaction m_transaction;

	//! the maximum image (tile/buffer) size that
	//  gets read or write by this helper
	long m_maximgsize;


};


#endif /* RASDAMANHELPER2_H_ */
