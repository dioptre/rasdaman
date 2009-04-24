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
/
/**
 * SOURCE: hdf.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_HDF
 *
 * COMMENTS:
 *
 * Provides functions to convert data to HDF SD and back.
 *
*/

/* Added by Sorin Stancu-Mara. Definition clashed for type int8, define in both 
* /usr/include/hdf.h and in /usr/include/tiff.h
* This will supress the tiff.h definition.
* Both definitions are similar
*/
#define HAVE_INT8

#include "conversion/hdf.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "raslib/primitivetype.hh"


#ifdef HAVE_HDF_H
    #include "hdf.h"
#elif HAVE_HDF_HDF_H
    #include "hdf/hdf.h"
#else
    #error "No hdf.h header available."
#endif 

#ifdef HAVE_MFHDF_H
    #include "mfhdf.h"
#elif HAVE_HDF_MFHDF_H
    #include "hdf/mfhdf.h"
#else
    #error "No mfhdf.h header available."
#endif

#include <stdio.h>
#include <iostream>



const r_Convertor::convert_string_t r_Conv_HDF::compNames[] = {
  {"none", COMP_CODE_NONE},
  {"rle", COMP_CODE_RLE},
  {"huffman", COMP_CODE_SKPHUFF},
  {"deflate", COMP_CODE_DEFLATE},
  {NULL, COMP_CODE_NONE}
};

// Buffer used for switching the majorness (column <--> row) of the array data
const int r_Conv_HDF::MaxSwapBufferSize = 0x10000;




void r_Conv_HDF::initHDF( void )
{
  compType = NULL;
  quality = 80;
  skiphuff = 0;

  if (params == NULL)
    params = new r_Parse_Params;

  params->add("comptype", &compType, r_Parse_Params::param_type_string);
  params->add("quality", &quality, r_Parse_Params::param_type_int);
  params->add("skiphuff", &skiphuff, r_Parse_Params::param_type_int);
}


int r_Conv_HDF::getHDFtype(int intType, int &size)
{
  int result=0;

  switch (intType)
  {
    case ctype_int8: result = DFNT_CHAR8; size = 1; break;
    case ctype_uint8:
    case ctype_char:
    case ctype_bool: result = DFNT_UCHAR8; size = 1; break;
    case ctype_int16: result = DFNT_INT16; size = 2; break;
    case ctype_uint16: result = DFNT_UINT16; size = 2; break;
    case ctype_int32: result = DFNT_INT32; size = 4; break;
    case ctype_uint32: result = DFNT_UINT32; size = 4; break;
    case ctype_int64: result = DFNT_INT64; size = 8; break;
    case ctype_uint64: result = DFNT_UINT64; size = 8; break;
    case ctype_float32: result = DFNT_FLOAT32; size = 4; break;
    case ctype_float64: result = DFNT_FLOAT64; size = 8; break;
    default: result = 0; size = 1; break;
  }
  return result;
}


int r_Conv_HDF::getIntType( int hdfType, int &size )
{
  int result=0;

  switch (hdfType)
  {
    case DFNT_CHAR8: result = ctype_int8; size = 1; break;
    case DFNT_UCHAR8: result = ctype_uint8; size = 1; break;
    case DFNT_INT16: result = ctype_int16; size = 2; break;
    case DFNT_UINT16: result = ctype_uint16; size = 2; break;
    case DFNT_INT32: result = ctype_int32; size = 4; break;
    case DFNT_UINT32: result = ctype_uint32; size = 4; break;
    case DFNT_INT64: result = ctype_int64; size = 8; break;
    case DFNT_UINT64: result = ctype_uint64; size = 8; break;
    case DFNT_FLOAT32: result = ctype_float32; size = 4; break;
    case DFNT_FLOAT64: result = ctype_float64; size = 8; break;
    default: result = ctype_void; size = 1; break;
  }
  return result;
}



r_Conv_HDF::r_Conv_HDF(const char *src, const r_Minterval &interv, const r_Type *tp) throw(r_Error) 
: r_Convertor(src, interv, tp, true)
{
  initHDF();

  if (tp->isStructType())
  {
    RMInit::logOut << "r_Conv_HDF::r_Conv_HDF(): structured types not supported." << endl;
    throw r_Error(r_Error::r_Error_General);
  }
}



r_Conv_HDF::r_Conv_HDF(const char *src, const r_Minterval &interv, int tp) throw(r_Error)
: r_Convertor(src, interv, tp)
{
  initHDF();
}



r_Conv_HDF::~r_Conv_HDF(void)
{
  if (compType != NULL)
  {
    delete [] compType;
    compType = NULL;
  }
}



r_convDesc &r_Conv_HDF::convertTo( const char *options ) throw(r_Error)
{
  char name[256];
  int32 handle=0, sds_id=0, rank=0;
  comp_coder_t comp_type=COMP_CODE_NONE;
  int32 *dimsizes=NULL, *start=NULL;
  size_t filesize=0;
  int i=0, j=0;
  FILE *fp=NULL;
  comp_info c_info;

  strncpy(name, tmpnam(NULL), 256);
  //name = "testfile.hdf";

  if ((handle = SDstart(name, DFACC_CREATE)) == FAIL)
  {
    RMInit::logOut << "r_Conv_HDF::convertTo(): unable to open output file." << endl;
    throw r_Error(r_Error::r_Error_General);
  }
  rank = desc.srcInterv.dimension();

  dimsizes = new int32[rank]; start = new int32[rank];
  datatype = getHDFtype(desc.baseType, datasize);

  for (i=0; i<rank; i++)
  {
    dimsizes[i] = desc.srcInterv[i].high() - desc.srcInterv[i].low() + 1;
    start[i] = 0;
  }

  if ((sds_id = SDcreate(handle, "RasDaMan object", datatype, rank, dimsizes)) == FAIL)
  {
    RMInit::logOut << "r_Conv_HDF::convertTo(): unable to create object." << endl;
    SDend(handle); remove(name);
    throw r_Error(r_Error::r_Error_General);
  }
  SDsetfillmode(sds_id, SD_NOFILL);

  params->process(options);

  comp_type = COMP_CODE_DEFLATE;
  if (compType != NULL)
  {
    for (i=0; compNames[i].key != NULL; i++)
    {
      if (strcasecmp(compNames[i].key, compType) == 0)
      {
	comp_type = (comp_coder_t)compNames[i].id;
	break;
      }
    }
    if (compNames[i].key == NULL)
    {
      RMInit::logOut << "r_Conv_HDF::convertTo(): unsupported compression type " << compType << endl;
    }
  }
  c_info.skphuff.skp_size = skiphuff;
  c_info.deflate.level = quality;

  SDsetcompress(sds_id, comp_type, &c_info);

  SDwritedata(sds_id, start, NULL, dimsizes, (VOIDP)(desc.src));

  delete [] dimsizes; dimsizes=NULL;
  delete [] start; start=NULL;

  SDendaccess(sds_id);

  SDend(handle);

  if ((fp = fopen(name, "rb")) == NULL)
  {
    RMInit::logOut << "r_Conv_HDF::convertTo(): unable to read back file." << endl;
    throw r_Error(r_Error::r_Error_General);
  }
  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);

  desc.destInterv = r_Minterval(1);
  desc.destInterv << r_Sinterval((r_Range)0, (r_Range)filesize - 1);

  if ((desc.dest = (char*)mystore.storage_alloc(filesize)) == NULL)
  {
    RMInit::logOut << "r_Conv_HDF::convertTo(): out of memory error" << endl;
    fclose(fp);
    throw r_Error(MEMMORYALLOCATIONERROR);
  }
  fseek(fp, 0, SEEK_SET);
  fread(desc.dest, 1, filesize, fp);

  fclose(fp);

  remove(name);

  // Result is just a bytestream
  desc.destType = r_Type::get_any_type("char");

  return desc;
}



r_convDesc &r_Conv_HDF::convertFrom(const char *options) throw(r_Error)
{
  char name[256];
  int32 handle=0, sds_id=0, rank=0, dtype=0, numattr=0, array_size=0;
  int32 dimsizes[MAX_VAR_DIMS];
  int32 *start=NULL;
  int dsize=0;
  size_t filesize=0;
  FILE *fp=NULL;
  int i=0;

  if (desc.srcInterv.dimension() != 1)
  {
    RMInit::logOut << "r_Conv_HDF::convertFrom(): source data must be a bytestream!" << endl;
    throw r_Error(r_Error::r_Error_General);
  }

  strncpy(name, tmpnam(NULL), 256);
  if ((fp = fopen(name, "wb")) == NULL)
  {
    RMInit::logOut << "r_Conv_HDF::convertFrom(): unable to write temporary file!" << endl;
    throw r_Error(r_Error::r_Error_General);
  }
  filesize = desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1;
  if ((i = fwrite(desc.src, 1, filesize, fp)) != filesize)
  {
    RMInit::logOut << "r_Conv_HDF::convertFrom(): error writing to temporary file ("
		   << i << " / " << filesize << ')' << endl;
    throw r_Error(r_Error::r_Error_General);
  }
  fclose(fp);

  if ((handle = SDstart(name, DFACC_READ)) == FAIL)
  {
    RMInit::logOut << "r_Conv_HDF::convertFrom(): can't read temporary file!" << endl;
    throw r_Error(r_Error::r_Error_General);
  }
  // Only read the first object in the file
  if ((sds_id = SDselect(handle, 0)) == FAIL)
  {
    RMInit::logOut << "r_Conv_HDF::convertFrom(): unable to open first object" << endl;
    SDend(handle); remove(name);
    throw r_Error(r_Error::r_Error_General);
  }
  
  SDgetinfo(sds_id, NULL, &rank, dimsizes, &dtype, &numattr);

  // Ignore native datatype flag
  dtype &= ~DFNT_NATIVE;

  desc.destType = get_external_type(getIntType(dtype, dsize));

  start = new int32[rank];
  desc.destInterv = r_Minterval(rank);
  array_size = (int32)dsize;
  for (i=0; i<rank; i++)
  {
    desc.destInterv << r_Sinterval(r_Range(0), r_Range(dimsizes[i]-1));
    array_size *= dimsizes[i];
    start[i] = 0;
  }

  if ((desc.dest = (char*)mystore.storage_alloc(array_size)) == NULL)
  {
    RMInit::logOut << "r_Conv_HDF::convertFrom(): out of memory error!" << endl;
    SDend(handle); remove(name);
    throw r_Error(MEMMORYALLOCATIONERROR);
  }

  if (SDreaddata(sds_id, start, NULL, dimsizes, (VOIDP)desc.dest) == FAIL)
  {
    RMInit::logOut << "r_Conv_HDF::convertFrom(): error reading data" << endl;
    SDend(handle); remove(name);
    throw r_Error(r_Error::r_Error_General);
  }

  delete [] start; start=NULL;

  SDendaccess(sds_id);
  
  SDend(handle);

  remove(name);

  return desc;
}



const char *r_Conv_HDF::get_name( void ) const
{
  return format_name_hdf;
}


r_Data_Format r_Conv_HDF::get_data_format( void ) const
{
  return r_HDF;
}


r_Convertor *r_Conv_HDF::clone( void ) const
{
  return new r_Conv_HDF(desc.src, desc.srcInterv, desc.baseType);
}
