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
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
*/

#include <iostream>
#include <string.h>

#ifdef AIX
#include <strings.h>
#endif

#include "conversion/tiff.hh"
#include "conversion/memfs.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "debug/debug.hh"

const int r_Conv_TIFF::defaultRPS = 32;

const char r_Conv_TIFF::dummyFileFmt[] = "/tmp/%p.tif";

const unsigned int r_Conv_TIFF::TIFF_DEFAULT_QUALITY = 80;

const struct r_Convertor::convert_string_s r_Conv_TIFF::compNames[] =
{
	{"none", COMPRESSION_NONE},
	{"ccittrle", COMPRESSION_CCITTRLE},
	{"ccittfax3", COMPRESSION_CCITTFAX3},
	{"ccittfax4", COMPRESSION_CCITTFAX4},
	{"lzw", COMPRESSION_LZW},
	{"ojpeg", COMPRESSION_OJPEG},
	{"jpeg", COMPRESSION_JPEG},
	{"next", COMPRESSION_NEXT},
	{"ccittrlew", COMPRESSION_CCITTRLEW},
	{"packbits", COMPRESSION_PACKBITS},
	{"thunderscan", COMPRESSION_THUNDERSCAN},
	{"pixarfilm", COMPRESSION_PIXARFILM},
	{"pixarlog", COMPRESSION_PIXARLOG},
	{"deflate", COMPRESSION_DEFLATE},
	{"dcs", COMPRESSION_DCS},
	{"jbig", COMPRESSION_JBIG},
#ifndef LINUX
	{"sgilog", COMPRESSION_SGILOG},
	{"sgilog24", COMPRESSION_SGILOG24},
	{"it8ctpad", COMPRESSION_IT8CTPAD},
	{"it8lw", COMPRESSION_IT8LW},
	{"it8mp", COMPRESSION_IT8MP},
	{"it8bl", COMPRESSION_IT8BL},
#endif
	{NULL, COMPRESSION_NONE}
};

const struct r_Convertor::convert_string_s r_Conv_TIFF::resunitNames[] =
{
	{"none", RESUNIT_NONE},
	{"inch", RESUNIT_INCH},
	{"centimeter", RESUNIT_CENTIMETER},
	{NULL, RESUNIT_NONE}
};

// Change these according to the platform!
// Fill order of bits in bitmap mode. Define 0 for LSB, otherwise MSB
#define _R_TIFF_BITFILLORDER    1

// Setup internal macros according to the fill-order
#if (_R_TIFF_BITFILLORDER == 0)
#define _R_TIFF_MASK_VALUE      1
#define _R_TIFF_MASK_SHIFT(x)   (x) <<= 1;
#else
#define _R_TIFF_MASK_VALUE      (1<<7)
#define _R_TIFF_MASK_SHIFT(x)   (x) >>= 1;
#endif

// TIFF class functions

/// Translate string compression type to libtiff compression type
int r_Conv_TIFF::get_compression_from_name(const char* strComp)
{
	ENTER( "r_Conv_TIFF::get_compression_from_name( " << (strComp?strComp:"(null)") << " )" );

	unsigned short i=0;
	int tiffComp=COMPRESSION_NONE;

	if(strComp != NULL)
	{  
		for (i=0; compNames[i].key != NULL; i++)
		{
			if (strcasecmp(compNames[i].key, strComp) == 0)
			{
				tiffComp = compNames[i].id;
				break;
			}
		}
		if (compNames[i].key == NULL)
		{
			TALK( "r_Conv_TIFF::get_compression_from_name(): error: unsupported compression type " << strComp << "." );
			RMInit::logOut << "Error: unsupported compression type " << strComp << "." << endl;
		}
	}

	LEAVE( "r_Conv_TIFF::get_compression_from_name() -> " << tiffComp );
	return tiffComp;
}

/// Translate string resolution unit type to libtiff resolution unit type
int r_Conv_TIFF::get_resunit_from_name(const char* strResUnit)
{
	ENTER( "r_Conv_TIFF::get_resunit_from_name( " << (strResUnit?strResUnit:"(null)") << " )" );

	unsigned short i=0;
	int tiffResUnit=RESUNIT_NONE;
	if(strResUnit != NULL)
	{  
		for (i=0; resunitNames[i].key != NULL; i++)
		{
			if (strcasecmp(resunitNames[i].key, strResUnit) == 0)
			{
				tiffResUnit = resunitNames[i].id;
				break;
			}
		} 
		if (resunitNames[i].key == NULL)
		{
			TALK( "r_Conv_TIFF::get_resunit_from_name(): error: unsupported resolution unit type " << strResUnit << "." );
			RMInit::logOut << "Error: unsupported resolution unit type " << strResUnit << "." << endl;
		}
	}

	LEAVE( "r_Conv_TIFF::get_resunit_from_name() -> " << tiffResUnit );
	return tiffResUnit;
}

/// internal initialization, common to all constructors
void r_Conv_TIFF::initTIFF( void )
{
	ENTER( "r_Conv_TIFF::initTIFF()" );

	compType = NULL;
	quality = r_Conv_TIFF::TIFF_DEFAULT_QUALITY;
	override_bpp = 0;
	override_bps = 0;
	override_depth = 0;

	if (params == NULL)
		params = new r_Parse_Params();

	params->add("comptype", &compType, r_Parse_Params::param_type_string);
	params->add("quality", &quality, r_Parse_Params::param_type_int);
	params->add("bpp", &override_bpp, r_Parse_Params::param_type_int);
	params->add("bps", &override_bps, r_Parse_Params::param_type_int);
	params->add("depth", &override_depth, r_Parse_Params::param_type_int);

	LEAVE( "r_Conv_TIFF::initTIFF()" );
}

/// constructor using type structure
r_Conv_TIFF::r_Conv_TIFF(const char *src, const r_Minterval &interv, const r_Type *tp) throw(r_Error)
  : r_Convert_Memory(src, interv, tp, true)
{
	ENTER( "r_Conv_TIFF::r_Conv_TIFF( " << (src?src:"(null)") << ", (minterval), (type ptr) )" ); 

	initTIFF();

	LEAVE( "r_Conv_TIFF::r_Conv_TIFF()" ); 
}

/// constructor using int type indicator
r_Conv_TIFF::r_Conv_TIFF(const char *src, const r_Minterval &interv, int type) throw(r_Error)
: r_Convert_Memory(src, interv, type)
{
	ENTER( "r_Conv_TIFF::r_Conv_TIFF( " << (src?src:"(null)") << ", (minterval), " << type << " )" ); 

	initTIFF();

	LEAVE( "r_Conv_TIFF::r_Conv_TIFF()" ); 
}

/// destructor
r_Conv_TIFF::~r_Conv_TIFF(void)
{
	ENTER( "r_Conv_TIFF::~r_Conv_TIFF()" );

	if (compType != NULL)
	{
		delete [] compType;
		compType = NULL;
	}

	LEAVE( "r_Conv_TIFF::~r_Conv_TIFF()" );
}

/// convert array to TIFF stream
// Compression modes recommended:
// Bitmap, Greyscales:  COMPRESSION_LZW, COMPRESSION_DEFLATE
// RGB:                 COMPRESSION_JPEG, COMPRESSION_SGILOG24
r_convDesc &r_Conv_TIFF::convertTo( const char *options ) throw(r_Error)
{
	ENTER( "r_Conv_TIFF::convertTo( " << (options?options:"(null)") << " )" );

	if (options != NULL)
		printf("tiff convert option = %s\n", options);
	else
		printf("tiff options are null = %s\n", options);
	TIFF *tif=NULL;
	char dummyFile[256];
	uint16 cmap[256];             // Colour map (for greyscale images)
	uint32 pixelAdd=0, lineAdd=0;  // number of _bytes_ to add to a pointer
	// to the source data to get the address
	// of the pixel to the right / downwards.
	uint16 bps=0, bpp=0;
	uint32 width=0, height=0, i=0;
	int tiffcomp = COMPRESSION_NONE;

	params->process(options);

	// translate string compression type to libtiff compression type
	if (compType != NULL)
		tiffcomp=get_compression_from_name(compType);

	// Set dimensions
	width  = (uint32)(desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1);
	height = (uint32)(desc.srcInterv[1].high() - desc.srcInterv[1].low() + 1);

	switch (desc.baseType)
	{
		// MDD arrays are transposed compared to the format needed for images.
		// Therefore the pixelAdd and lineAdd values change places.
		case ctype_bool:
			bps = 1; bpp = 1; pixelAdd = height; lineAdd = 1;
			break;
		case ctype_char:
			bps = 8; bpp = 8; pixelAdd = height; lineAdd = 1;
			break;
		case ctype_rgb:
			bps = 8; bpp = 24; pixelAdd = 3*height; lineAdd = 3;
			break;  
		case ctype_uint16:
			bps = 16; bpp = 16; pixelAdd = 2*height; lineAdd = 2;
			break;
		case ctype_float32:
			bps = 32; bpp = 32; pixelAdd = 4*height; lineAdd = 4;
			break;
		default:
			TALK( "r_Conv_TIFF::convertTo(): error: unsupported base type " << desc.baseType << "." );
			RMInit::logOut << "Error: encountered unsupported TIFF base type." << endl;
			throw r_Error(BASETYPENOTSUPPORTEDBYOPERATION);
	}

	// Just to make sure nothing serious goes wrong if this conversion
	// function is called more than once.
	memfs_newfile(handle);

	// Open a dummy output file (all operations will be redirected to
	// Memory). Make dummy file unique for each object by using the
	// address of its memFSContext (kind of a hack, I know...). That
	// should ensure re-entrancy.
	sprintf(dummyFile, dummyFileFmt, (void*)handle);
	tif = TIFFClientOpen(dummyFile, "w", handle,
			memfs_read, memfs_write, memfs_seek, memfs_close, memfs_size,
			memfs_map, memfs_unmap);

	if (tif == NULL)
	{
		RMInit::logOut << "r_Conv_TIFF::convertTo(): couldn't open file " << dummyFile << endl;
		throw r_Error(r_Error::r_Error_General);
	}

	TIFFSetField(tif, TIFFTAG_ARTIST, "rasdaman");
	TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, "exported from rasdaman database");
	TIFFSetField(tif, TIFFTAG_SOFTWARE, "rasdaman");
	//TIFFSetField(tif, TIFFTAG_SUBFILETYPE, (uint32)0);
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bps);
	// UNIX doesn't mind which fill-order. NT only understands this one.
	TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, (uint16)tiffcomp);
	TIFFSetField(tif, TIFFTAG_ORIENTATION, (uint16)ORIENTATION_TOPLEFT);
	// Format-dependent tags
	if (desc.baseType == ctype_rgb)
	{
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, (uint16)PHOTOMETRIC_RGB);
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16)3);
	}
	else
	{
		if (desc.baseType == ctype_char)
		{
			TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, (uint16)PHOTOMETRIC_PALETTE);
		}
		else
		{
			TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, (uint16)PHOTOMETRIC_MINISBLACK);
		}
		// FIXME: STILL HAVE TO ADD FOR other CASES eg ctype_uint16 and ctype_float32 ???
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16)1);
    
    // set sample format tag
    if (desc.baseType == ctype_float32) {
      TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP );
    } else if (desc.baseType == ctype_uint16) {
      TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT );
    }
	}
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, (uint16)PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, (uint32)-1));
	//TIFFSetField(tif, TIFFTAG_MINSAMPLEVALUE, (uint16)0);
	//TIFFSetField(tif, TIFFTAG_MAXSAMPLEVALUE, (uint16)255);
	TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, (uint16)RESUNIT_INCH);
	TIFFSetField(tif, TIFFTAG_XRESOLUTION, (float)90.0);
	TIFFSetField(tif, TIFFTAG_YRESOLUTION, (float)90.0);
	TIFFSetField(tif, TIFFTAG_XPOSITION, (float)0.0);
	TIFFSetField(tif, TIFFTAG_YPOSITION, (float)0.0);
	if ((tiffcomp == COMPRESSION_JPEG) || (tiffcomp == COMPRESSION_OJPEG))
	{
		if (quality == 100)
		{
			TIFFSetField(tif, TIFFTAG_JPEGPROC, JPEGPROC_LOSSLESS);
		}
		else
		{
			TIFFSetField(tif, TIFFTAG_JPEGQUALITY, quality);
		}
	}

	// build the colour-map (greyscale, i.e. all 3 components identical)
	// TIFF needs 16 bit values for this (--> tools/tiffdither.c)
	for (i=0; i<256; i++)
		cmap[i] = (uint16)(i*((1L << 16) - 1)/255);
	TIFFSetField(tif, TIFFTAG_COLORMAP, cmap, cmap, cmap);

	// Be VERY, VERY careful about the order and the items you write
	// out. TIFFWriteDirectory, e.g.,  has very ugly side-effects.
	uint32 *tbuff=NULL;
	const char *l=NULL, *line = desc.src;
	uint8 *normal=NULL;  // normalised source data
	uint32 row=0;

	// cout << "r_Conv_TIFF: Main Loop:" << endl;
	if ((tbuff = new uint32[((width * bpp + 31) >> 5)]) != NULL)
	{
		// now go line by line
		for (row = 0; row < height; row++, line += lineAdd)
		{
			normal = (uint8 *)tbuff; l = line;

			// copy data in the correct format to the buffer
			switch (desc.baseType)
			{
				case ctype_bool:
					{
						uint8 val = 0, mask = _R_TIFF_MASK_VALUE;

						// convert 8bpp bitmap to 1bpp bitmap
						for (i=0; i < width; i++, l += pixelAdd)
						{
							// fill bits in lsb order
							if (*l != 0)
								val |= mask;
							_R_TIFF_MASK_SHIFT(mask);
							if (mask == 0)
							{
								*normal++ = val;
								val = 0; 
								mask = _R_TIFF_MASK_VALUE;
							}
						}
						if (mask != _R_TIFF_MASK_VALUE)
							*normal++ = val;
					}
					break;
				case ctype_char:
					{
						// copy data (and transpose)
						for (i=0; i < width; i++, l += pixelAdd)
						{
							*normal++ = *l;
						}
					}
					break;
				case ctype_rgb:
					{
						// copy data (and transpose)
						for (i=0; i < width; i++, l += pixelAdd)
						{
							*normal++ = l[0]; *normal++ = l[1]; *normal++ = l[2];
						}
					}
					break;

					// ADDED FOR ctype_uint16 case
				case ctype_uint16:
					{
						for (i =0; i<width; i++)
						{
							*((uint16*)normal) = *((uint16*)l);
							l +=pixelAdd;
							normal += 2;
						}
					}
					break;

				case ctype_float32:
					{
						for(i=0; i<width; i++)
						{
							*((float*)normal) = *((float*)l);
							l += pixelAdd; normal += 4;
						}
					}
					break;
			}
			if (TIFFWriteScanline(tif, (tdata_t)tbuff, row, 0) < 0)
				break;
		}

		delete [] tbuff;
		tbuff = NULL;
	}

	if (row < height)  // error
	{
		TALK( "r_Conv_TIFF::convertTo(): error writing data after " << row << " rows out of " << height << "." );
		RMInit::logOut << "Error: cannot write all TIFF data." << endl;
		TIFFClose(tif); remove(dummyFile);
		throw r_Error(r_Error::r_Error_General);
	}

	TIFFClose(tif);
	// Now delete the dummy file
	remove(dummyFile);

	r_Long tifSize = memfs_size(handle);

	// Allocate an array of just the right size and "load" object there
	if ((desc.dest = (char*)mystore.storage_alloc(sizeof(char) * tifSize)) == NULL)
	{
		TALK( "r_Conv_TIFF::convertTo(): out of memory." );
		RMInit::logOut << "Error: out of memory." << endl;
		throw r_Error(MEMMORYALLOCATIONERROR);
	}
	memfs_seek(handle, 0, SEEK_SET);
	memfs_read(handle, desc.dest, tifSize);

	// Set up destination interval
	desc.destInterv = r_Minterval(1);
	desc.destInterv << r_Sinterval(r_Range(0), r_Range(tifSize - 1));

	// define the base type as char for now
	desc.destType = r_Type::get_any_type("char");

	LEAVE( "r_Conv_TIFF::convertTo()" );
	return desc;
}


/// convert TIFF stream into array
r_convDesc &r_Conv_TIFF::convertFrom(const char *options) throw(r_Error) // CONVERTION FROM TIFF TO DATA
{
	ENTER( "r_Conv_TIFF::convertFrom( " << (options?options:"(null") << " )" );

	if (options != NULL)
		printf("tiff convert option = %s\n", options);
	else
		printf("tiff options are null\n");
	params->process(options); //==> CHECK THIS "IMP"
	TIFF *tif=NULL;
	char dummyFile[256];
	int isOK=0, typeSize=0;
	uint16 bps=0, bpp=0, spp=0, planar=0, photometric=0;
	uint32 width=0, height=0, pixelAdd=0, lineAdd=0, i=0;
	uint16 *reds=NULL, *greens=NULL, *blues=NULL;

	// Init simple (chunky) memFS
	memfs_chunk_initfs(handle, (char*)desc.src, (r_Long)(desc.srcInterv[0].high()-desc.srcInterv[0].low()+1)); //==> CHECK THIS 
	desc.dest = NULL;

	// Create dummy file for use in the TIFF open function
	sprintf(dummyFile, dummyFileFmt, (void*)handle);
	fclose(fopen(dummyFile, "wb"));
	std::cout << "r_Conv_TIFF: Dummy created OK" << endl;

	// Open and force memory mapping mode
	tif = TIFFClientOpen(dummyFile, "rM", handle,
			memfs_chunk_read, memfs_chunk_read, memfs_chunk_seek, memfs_chunk_close,
			memfs_chunk_size, memfs_chunk_map, memfs_chunk_unmap);

	if (tif == NULL)
	{
		RMInit::logOut << "r_Conv_TIFF::convertFrom(): unable to open file!" << endl;
		throw r_Error(r_Error::r_Error_General);
	}
	//cout << "r_Conv_TIFF: Opened OK" << endl;

	//TIFFPrintDirectory(tif, stdout, 0);

	TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
	TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
	bpp = spp * bps;
	TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar);
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);

	// Filter out the kind of image we understand.
	isOK = -1;
	if ((bps != 1) && (bps != 8) && (bps != 32) && (bps != 16)) /* ADDED (bps != 32) since this is float data; also added (bps != 16)*/
	{
		RMInit::logOut << "r_Conv_TIFF::convertFrom(): bad number of bits per sample: " << bps << " (must be 1 or 8 or 32 or 16)" << endl; /* ADDED 'or 32' */
		if (override_bps)
			bps = override_bps;
		else
			isOK = 0;
	}
	if ((bpp != 1) && (bpp != 8) && (bpp != 24) && (bpp != 32) && (bpp != 16)) /* ADDED bps = 32 and samples/pixel = 1 so bpp = 32; Also added bpp = 16 */
	{
		RMInit::logOut << "r_Conv_TIFF::convertFrom(): bad number of bits per pixel: " << bpp << " (must be 1, 8 or 24 or 32 or 16)" << endl; /* ADDED 'or 32' */
		if (override_bpp)  
			bpp = override_bpp;
		else
			isOK = 0;
	}
	if (planar != PLANARCONFIG_CONTIG)// must be contiguous for our case to handle, other cases not dealt yet
	{
		RMInit::logOut << "r_Conv_TIFF::convertFrom(): can't handle bitplanes!" << endl;
		isOK = 0;
	}

	if (isOK) // isOK = -1
	{
		std::cout << "r_Conv_TIFF: Image OK: bps = " << bps << ", spp = " << spp << ", width = " << width << ", height = " << height << endl;
		if (bpp == 32){
			desc.baseType = ctype_float32;
			pixelAdd = 4*height; lineAdd = 4; typeSize = 4;

		}
		else if (bpp == 24) 
		{
			pixelAdd = 3*height; lineAdd = 3; typeSize = 3;
			desc.baseType = ctype_rgb;
		}
		else if (bpp == 16)
		{
			desc.baseType = ctype_uint16;
			pixelAdd = 2*height; lineAdd = 2; typeSize = 2;
		}
		else
		{
			if ((photometric == PHOTOMETRIC_PALETTE) && (override_depth != 1))
			{
				TIFFGetField(tif, TIFFTAG_COLORMAP, &reds, &greens, &blues);
				for (i=0; i<256; i++) 
				{
					if ((reds[i] != greens[i]) || (greens[i] != blues[i])) break;
				}

				if (i < 256)
				{
					pixelAdd = 3*height; lineAdd = 3; typeSize = 3;
					desc.baseType = ctype_rgb;
				}
				else
				{
					pixelAdd = height; lineAdd = 1; typeSize = 1;
					desc.baseType = ctype_char;
				}

				if (override_depth)
				{
					switch (override_depth)
					{
						case 1:
							pixelAdd = height; lineAdd = 1; typeSize = 1;
							desc.baseType = ctype_bool;
							break;
						case 8:
							pixelAdd = height; lineAdd = 1; typeSize = 1;
							desc.baseType = ctype_char;
							break;
						case 24:
							pixelAdd = 3*height; lineAdd = 3; typeSize = 3;
							desc.baseType = ctype_rgb;
							break;
							/*
							// MUST add here for case 32 */
						case 32:
							pixelAdd = 4*height; lineAdd = 4; typeSize = 4;
							desc.baseType = ctype_float32;
							break;
							// add for 16 bit unsigned and signed case
						case 16:
							pixelAdd = 2*height; lineAdd = 2; typeSize = 2;
							desc.baseType = ctype_uint16;
							break;

					}
				}
			} // if ((photometric == PHOTOMETRIC_PALETTE) && (override_depth != 1)) CLOSED
			else
			{
				pixelAdd = height; lineAdd = 1; typeSize = 1;
				desc.baseType = (bpp == 1) ? ctype_bool : ctype_char;
			}
		} // if sample is not RGB CLOSED


		if ((desc.dest = (char*)mystore.storage_alloc(width*height*typeSize*sizeof(char))) == NULL)
		{
			RMInit::logOut << "r_Conv_TIFF::convertFrom(): out of memory error!" << endl;
		}
		else
		{
			std::cout << "r_Conv_TIFF: baseType = " << desc.baseType << ", size = " << typeSize << ", pixelAdd = " << pixelAdd << ", lineAdd = " << lineAdd << endl;
			uint32 *tbuff=NULL;
			char *l=NULL, *line = desc.dest;
			uint8 *normal=NULL;
			uint32 row = 0;

			if ((tbuff = new uint32[(width * bpp + 31) >> 5]) != NULL)
			{
				for (row = 0; row < height; row++, line += lineAdd)
				{
					if (TIFFReadScanline(tif, (tdata_t)tbuff, row, 0) < 0) break;
					normal = (uint8 *)tbuff; l = line;
					switch (desc.baseType)
					{
						case ctype_bool: // when cytpe is bool 
							{
								uint8 mask = _R_TIFF_MASK_VALUE;
								for (i=0; i < width; i++, l += pixelAdd)
								{
									*l = (((*normal) & mask) == 0) ? 0 : 1;
									_R_TIFF_MASK_SHIFT(mask);
									if (mask == 0) {normal++; mask = _R_TIFF_MASK_VALUE;}
								}
							}
							break;

						case ctype_char: // when ctype is char
							{
								if (reds != NULL)
								{
									for (i=0; i < width; i++, l += pixelAdd)
									{
										*l = (reds[*normal++]) >> 8;
									}
								}
								else
								{
									for (i=0; i < width; i++, l += pixelAdd)
									{
										*l = *normal++;
									}
								}
							}
							break;

						case ctype_rgb: // when cytpe is rgb
							{
								if (reds != NULL)
								{
									for (i=0; i < width; i++, l += pixelAdd)
									{
										uint8 val = *normal++;
										l[0] = (reds[val]) >> 8;
										l[1] = (greens[val]) >> 8;
										l[2] = (blues[val]) >> 8;
									}
								}
								else
								{
									for (i=0; i < width; i++, l += pixelAdd)
									{
										l[0] = *normal++; l[1] = *normal++; l[2] = *normal++;
									}
								}
							}
							break;


							/* FOR NEW float type */
						case ctype_float32:
							{
								for (i=0; i < width; i++)
								{
									*((float*)l) = *((float*)normal);
									l += pixelAdd;
									normal += 4;
								}
							}
							break;

							/* FOR NEW UNSIGNED INT 16 BIT TYPE */
						case ctype_uint16:
							{
								for(i=0; i < width; i++)
								{
									*((uint16*)l) = *((uint16*)normal);
									l += pixelAdd;
									normal += 2;
								}
							}
							break;

					} // switch CLOSED
				} // for loop CLOSED
				delete [] tbuff; tbuff = NULL;
			} // if ((tbuff = new uint32[(width * bpp + 31) >> 5]) != NULL) CLOSED

			if (row < height)
			{
				TALK( "r_Conv_TIFF::convertFrom(): error reading data: got only " << row << " rows out of " << height << "." );
				RMInit::logOut << "Error: cannot read all data." << endl;
				TIFFClose(tif);
				remove(dummyFile);
				throw r_Error(r_Error::r_Error_General);
			}
		}
	} // is OK CLOSED

	TIFFClose(tif);
	remove(dummyFile);

	// Build destination interval
	desc.destInterv = r_Minterval(2);
	desc.destInterv << r_Sinterval(r_Range(0), r_Range(width - 1))
		<< r_Sinterval( r_Range(0), r_Range(height - 1));

	desc.destType = get_external_type(desc.baseType);

	LEAVE( "r_Conv_TIFF::convertFrom()" );
	return desc;
}


const char *r_Conv_TIFF::get_name( void ) const
{
	return format_name_tiff;
}


r_Data_Format r_Conv_TIFF::get_data_format( void ) const
{
	return r_TIFF;
}


r_Convertor *r_Conv_TIFF::clone( void ) const
{
	return new r_Conv_TIFF(desc.src, desc.srcInterv, desc.baseType);
}

