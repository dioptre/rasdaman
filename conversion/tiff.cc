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
#include <sstream>

#ifdef AIX
#include <strings.h>
#endif

#include "conversion/tiff.hh"
#include "conversion/memfs.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "raslib/structuretype.hh"
#include "raslib/primitivetype.hh"
#include "debug/debug.hh"

using namespace std;

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

/// Capture errors
void TIFFError(const char* /*module*/, const char* fmt, va_list argptr)
{
  char msg[10240];  
  vsprintf (msg, fmt, argptr);
  RMInit::logOut << "TIFF error: " << msg << endl;
}

/// Capture warnings
void TIFFWarning(const char* /*module*/, const char* fmt, va_list argptr)
{
  char msg[10240];  
  vsprintf (msg, fmt, argptr);
  RMInit::logOut << "TIFF warning: " << msg << endl;
}

/// internal initialization, common to all constructors
void r_Conv_TIFF::initTIFF( void )
{
	ENTER( "r_Conv_TIFF::initTIFF()" );

	compType = NULL;
  sampleType = NULL;
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
	params->add("sampletype", &sampleType, r_Parse_Params::param_type_string);
  
  // set our error handlers
  TIFFSetErrorHandler(TIFFError);
  TIFFSetWarningHandler(TIFFWarning);

	LEAVE( "r_Conv_TIFF::initTIFF()" );
}

/// constructor using type structure
r_Conv_TIFF::r_Conv_TIFF(const char *src, const r_Minterval &interv, const r_Type *tp) throw(r_Error)
  : r_Convert_Memory(src, interv, tp, true)
{
	ENTER( "r_Conv_TIFF::r_Conv_TIFF( " << (src?"(src)":"(null)") << ", (minterval), (type ptr) )" ); 

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
  
	TIFF *tif = NULL;
	char dummyFile[256];
	uint16 cmap[256];             // Colour map (for greyscale images)
	uint32 pixelAdd=0, lineAdd=0;  // number of _bytes_ to add to a pointer
	// to the source data to get the address
	// of the pixel to the right / downwards.
	uint16 bps=0, bpp=0;
	uint32 width=0, height=0, i=0;
	int tiffcomp = COMPRESSION_NONE;
  
  int sampleFormat = SAMPLEFORMAT_INT;
  int spp; // samples per pixel

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
		case ctype_int8:
    case ctype_uint8:
			bps = 8; bpp = 8; pixelAdd = height; lineAdd = 1;
			break;
		case ctype_rgb:
			bps = 8; bpp = 24; pixelAdd = 3*height; lineAdd = 3;
			break;  
		case ctype_uint16:
		case ctype_int16:
			bps = 16; bpp = 16; pixelAdd = 2*height; lineAdd = 2;
			break;
    case ctype_int32:
    case ctype_uint32:
		case ctype_float32:
			bps = 32; bpp = 32; pixelAdd = 4*height; lineAdd = 4;
			break;
    case ctype_int64:
    case ctype_uint64:
    case ctype_float64:
			bps = 64; bpp = 64; pixelAdd = 8*height; lineAdd = 8;
			break;
    case ctype_struct:
    {
      r_Structure_Type *st = (r_Structure_Type*) desc.srcType;
      spp = st->count_elements();
      
      int structSize = 0;
      r_Type::r_Type_Id bandType = r_Type::UNKNOWNTYPE;
      
      // iterate over the attributes of the struct
      r_Structure_Type::attribute_iterator iter(st->defines_attribute_begin());
      while (iter != st->defines_attribute_end()) {
        
        if ((*iter).type_of().isPrimitiveType()) {
          r_Primitive_Type *pt = (r_Primitive_Type*) &(*iter).type_of();
          structSize += pt->size();

          if (bandType == r_Type::UNKNOWNTYPE) {
            bandType = (*iter).type_of().type_id();
            
            // set sample format
            switch ((*iter).type_of().type_id()) {
              case r_Type::CHAR:
              case r_Type::USHORT:
              case r_Type::ULONG:
                sampleFormat = SAMPLEFORMAT_UINT;
                break;
              case r_Type::FLOAT:
              case r_Type::DOUBLE:
                sampleFormat = SAMPLEFORMAT_IEEEFP;
            }
          }
          // check if all bands are of the same type
          if ((*iter).type_of().type_id() != bandType) {
            RMInit::logOut << "r_Conv_TIFF::convertTo(): can't handle bands of different types" << endl;
            throw r_Error(BASETYPENOTSUPPORTEDBYOPERATION);
          }
        } else {
          RMInit::logOut << "r_Conv_TIFF::convertTo(): can't handle band of non-primitive type "
                  << (*iter).type_of().name() << endl;
          throw r_Error(BASETYPENOTSUPPORTEDBYOPERATION);
        }
        iter++;
      }
      bpp = structSize*8; bps = bpp / spp; pixelAdd = structSize*height; lineAdd = structSize;
      break;
    }
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
      TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16)1);
    else if (desc.baseType == ctype_struct)
      TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, spp);
		else
      TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16)1);
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, (uint16)PHOTOMETRIC_MINISBLACK);
    
    // set sample format tag
    switch (desc.baseType)
    {
      case ctype_float32:
      case ctype_float64:
        TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP );
        break;
      case ctype_char:
      case ctype_uint8:
      case ctype_uint16:
      case ctype_uint32:
      case ctype_uint64:
        TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT );
        break;
      case ctype_int8:
      case ctype_int16:
      case ctype_int32:
      case ctype_int64:
        TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT );
        break;
      case ctype_struct:
        TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, sampleFormat );
        break;
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
  if (desc.baseType == ctype_rgb) {
    for (i=0; i<256; i++)
      cmap[i] = (uint16)(i*((1L << 16) - 1)/255);
    TIFFSetField(tif, TIFFTAG_COLORMAP, cmap, cmap, cmap);
  }

	// Be VERY, VERY careful about the order and the items you write
	// out. TIFFWriteDirectory, e.g.,  has very ugly side-effects.
	uint32 *tbuff=NULL;
	const char *l=NULL, *line = desc.src;
	uint8 *normal=NULL;  // normalised source data
	uint32 row=0;

	if ((tbuff = new uint32[((width * height * bpp) >> 5)]) != NULL)
	{
    int error = 0; // indicates if writing succeeded
		// now go line by line
		for (row = 0; row < height && !error; row++, line += lineAdd)
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
        default:
          {
						// copy data (and transpose)
						for (i=0; i < width; i++, l += pixelAdd, normal += lineAdd)
						{
              memcpy(normal, l, lineAdd);
						}
          }
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

	params->process(options); //==> CHECK THIS "IMP"
	TIFF *tif=NULL;
	char dummyFile[256];
	int typeSize=0;
  int bandType=ctype_void;
  uint16 sampleFormat=0;
	uint16 bps=0, bpp=0, spp=0, planar=0, photometric=0, Bpp=0, Bps=0;
	uint32 width=0, height=0, pixelAdd=0, lineAdd=0, i=0;
	uint16 *reds=NULL, *greens=NULL, *blues=NULL;

	// Init simple (chunky) memFS
	memfs_chunk_initfs(handle, (char*)desc.src, (r_Long)(desc.srcInterv[0].high()-desc.srcInterv[0].low()+1)); //==> CHECK THIS 
	desc.dest = NULL;

	// Create dummy file for use in the TIFF open function
	sprintf(dummyFile, dummyFileFmt, (void*)handle);
	fclose(fopen(dummyFile, "wb"));
  
	// Open and force memory mapping mode
	tif = TIFFClientOpen(dummyFile, "rM", handle,
			memfs_chunk_read, memfs_chunk_read, memfs_chunk_seek, memfs_chunk_close,
			memfs_chunk_size, memfs_chunk_map, memfs_chunk_unmap);

	if (tif == NULL)
	{
		RMInit::logOut << "r_Conv_TIFF::convertFrom(): unable to open file!" << endl;
		throw r_Error(r_Error::r_Error_General);
	}

	//TIFFPrintDirectory(tif, stdout, 0);

	TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
	TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
  
  if (override_bps)
    bps = override_bps;
	bpp = spp * bps;
  if (override_bpp)
    bpp = override_bpp;
  Bpp = bpp/8; // bytes per pixel
  Bps = bps/8; // bytes per sample
  if (override_depth) {
    Bpp = Bps = override_depth/8;
  }
  lineAdd = typeSize = Bpp;
  
	TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar);
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
  TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sampleFormat);
  pixelAdd = Bpp*height;
  

	if (planar == PLANARCONFIG_CONTIG) // must be contiguous for our case to handle, other cases not dealt yet
	{
    switch (sampleFormat)
    {
      case SAMPLEFORMAT_INT:
        {
          switch (Bps)
          {
            case 1: bandType = ctype_int8;   break;
            case 2: bandType = ctype_int16;  break;
            case 4: bandType = ctype_int32;  break;
            default: {
              RMInit::logOut << "r_Conv_TIFF::convertFrom(): can't handle band type of signed integer, of length: " << Bps << endl;
              throw r_Error(BASETYPENOTSUPPORTEDBYOPERATION);
            }
          }
        }
        break;
      case SAMPLEFORMAT_UINT:
        {
          switch (Bps)
          {
            case 1: bandType = ctype_char;    break;
            case 2: bandType = ctype_uint16;  break;
            case 4: bandType = ctype_uint32;  break;
            default: {
              RMInit::logOut << "r_Conv_TIFF::convertFrom(): can't handle band type of unsigned integer, of length: " << Bps << endl;
              throw r_Error(BASETYPENOTSUPPORTEDBYOPERATION);
            }
          }
        }
        break;
      case SAMPLEFORMAT_IEEEFP:
        {
          switch (Bps)
          {
            case 4: bandType = ctype_float32; break;
            case 8: bandType = ctype_float64; break;
            default: {
              RMInit::logOut << "r_Conv_TIFF::convertFrom(): can't handle band type of floating point, of length: " << Bps << endl;
              throw r_Error(BASETYPENOTSUPPORTEDBYOPERATION);
            }
          }
        }
        break;
      default:
        {
          switch (bpp)
          {
            case 1 : bandType = ctype_bool;    break;
            case 8 : bandType = ctype_char;    break;
            case 16: bandType = ctype_uint16;  break;
            case 24: bandType = ctype_rgb;     break;
            case 32: bandType = ctype_float32; break;
            case 64: bandType = ctype_float64; break;
          }
        }
    }
    
    if ((photometric == PHOTOMETRIC_PALETTE) && (override_depth != 0))
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

      switch (override_depth)
      {
        case 1 : desc.baseType = ctype_bool;    break;
        case 8 : desc.baseType = ctype_char;    break;
        case 16: desc.baseType = ctype_uint16;  break;
        case 24: desc.baseType = ctype_rgb;     break;
        case 32: desc.baseType = ctype_float32; break;
        case 64: desc.baseType = ctype_float64; break;
      }
    }
    else if (spp > 1 && (spp != 3 || (spp == 3 && bpp != 24)))
    {
      // multiband when not rgb and more than 1 sample per pixel
      desc.baseType = ctype_struct;
    }
    else
    {
      desc.baseType = bandType;
    }


		if ((desc.dest = (char*)mystore.storage_alloc(width*height*typeSize*sizeof(char))) == NULL)
		{
			RMInit::logOut << "r_Conv_TIFF::convertFrom(): out of memory error!" << endl;
		}
		else
		{
			uint32 *tbuff=NULL;
			char *l=NULL, *line = desc.dest;
			uint8 *normal=NULL;
			uint32 row = 0;

			if ((tbuff = new uint32[(width * bpp + 31) >> 5]) != NULL)
			{
				for (row = 0; row < height; row++, line += lineAdd)
				{
          if (desc.baseType != ctype_struct) {
            if (TIFFReadScanline(tif, (tdata_t)tbuff, row, 0) < 0)
              break;
            normal = (uint8 *)tbuff; l = line;
          }
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
            case ctype_struct:
              {
                for (int j=0; j < spp; j++)
                {
                  TIFFReadScanline(tif, (tdata_t)tbuff, row, j); // read the j-th band
                  
                  int offset = j*Bps; // an offset to the j-th band
                  l = line + offset;
                  normal = (uint8 *)tbuff + offset;
                  for (int i = 0; i < width; i++, l += pixelAdd, normal += lineAdd)
                  {
                    memcpy(l, normal, Bps);
                  }
                }
              }
              break;
						default:
							{
								for (i=0; i < width; i++, l += pixelAdd, normal += lineAdd)
                {
                  memcpy(l, normal, lineAdd);
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
	} else {
    RMInit::logOut << "r_Conv_TIFF::convertFrom(): can't handle bitplanes!" << endl;
  }

	TIFFClose(tif);
	remove(dummyFile);

	// Build destination interval
	desc.destInterv = r_Minterval(2);
	desc.destInterv << r_Sinterval(r_Range(0), r_Range(width - 1))
		<< r_Sinterval( r_Range(0), r_Range(height - 1));

  // build destination type
  if (desc.baseType == ctype_struct) {
    // construct and set the structure type
    char* bt = NULL;
    if (sampleType != NULL)
      bt = sampleType;
    else
      bt = type_to_string(bandType);
    
    stringstream destType(stringstream::out);
    destType << "struct { ";
    for (int i = 0; i < spp; i++)
    {
      if (i > 0)
        destType << ", ";
      destType << bt;
    }
    destType << " }";
    desc.destType = r_Type::get_any_type(destType.str().c_str());
  } else
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

