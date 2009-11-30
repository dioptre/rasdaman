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
 * INCLUDE: tilecompression.cc
 *
 * MODULE:  compression
 * CLASS:   r_Tile_Compression
 *
 * COMMENTS:
 *
*/


#include <stdlib.h>
#include <string.h>


#include "raslib/rmdebug.hh"
#include "raslib/minterval.hh"
#include "raslib/type.hh"
#include "raslib/type.hh"
#include "raslib/basetype.hh"
#include "raslib/parseparams.hh"
#include "raslib/primitivetype.hh"
#include "raslib/structuretype.hh"

#include "compression/tilecompression.hh"
// for creating tile compression objects
#include "compression/tilecompnone.hh"
#include "compression/compresstime.hh"



/*
 *  r_Tile_Compression class
 */

const r_Tile_Compression::tile_comp_format_t r_Tile_Compression::all_formats[] = {
  { r_Array, format_name_array },
  { r_RLE, format_name_rle },
  { r_ZLib, format_name_zlib },
  { r_Sep_RLE, format_name_sep_rle },
  { r_Sep_ZLib, format_name_sep_zlib },
  { r_Wavelet_Haar, format_name_wavelet_haar },
  { r_Wavelet_Daubechies, format_name_wavelet_daubechies },
  { r_Wavelet_QHaar, format_name_wavelet_qhaar },
  // here we go the wavelets defined in r_Ortho_Wavelet_Factory
  { r_Wavelet_Daub6, format_name_wavelet_daub6 },
  { r_Wavelet_Daub8, format_name_wavelet_daub8 },
  { r_Wavelet_Daub10, format_name_wavelet_daub10 },
  { r_Wavelet_Daub12, format_name_wavelet_daub12 },
  { r_Wavelet_Daub14, format_name_wavelet_daub14 },
  { r_Wavelet_Daub16, format_name_wavelet_daub16 },
  { r_Wavelet_Daub18, format_name_wavelet_daub18 },
  { r_Wavelet_Daub20, format_name_wavelet_daub20 },
  { r_Wavelet_Least8, format_name_wavelet_least8 },
  { r_Wavelet_Least10, format_name_wavelet_least10 },
  { r_Wavelet_Least12, format_name_wavelet_least12 },
  { r_Wavelet_Least14, format_name_wavelet_least14 },
  { r_Wavelet_Least16, format_name_wavelet_least16 },
  { r_Wavelet_Least18, format_name_wavelet_least18 },
  { r_Wavelet_Least20, format_name_wavelet_least20 },
  { r_Wavelet_Coiflet6, format_name_wavelet_coiflet6 },
  { r_Wavelet_Coiflet12, format_name_wavelet_coiflet12 },
  { r_Wavelet_Coiflet18, format_name_wavelet_coiflet18 },
  { r_Wavelet_Coiflet24, format_name_wavelet_coiflet24 },
  { r_Wavelet_Coiflet30, format_name_wavelet_coiflet30 },
  { r_TMC, format_name_tmc }
};


r_Tile_Compression::r_Tile_Compression( const r_Minterval &dom, const r_Base_Type *type ) : mydomain(dom)
{
  mytype = (r_Base_Type*)(type->clone());
  compParams = NULL;
  decompParams = NULL;
  myTimer = NULL;
}


r_Tile_Compression::r_Tile_Compression( const r_Tile_Compression &src ) : mydomain(src.mydomain)
{
  mytype = (r_Base_Type*)(src.mytype->clone());
  compParams = NULL;
  decompParams = NULL;
  myTimer = NULL;
  mystore = src.mystore;
}


r_Tile_Compression::~r_Tile_Compression( void )
{
  delete mytype;

  if (compParams != NULL)
    delete compParams;
  if (decompParams != NULL)
    delete decompParams;
  CBENCH_STATEMENT(if (myTimer != NULL) delete myTimer);
}


bool r_Tile_Compression::converts_endianness( void ) const
{
  return false;
}


r_Bytes r_Tile_Compression::get_type_size( void ) const
{
  return mytype->size();
}


r_ULong r_Tile_Compression::get_tile_size( void ) const
{
  unsigned int size;

  if ((size = get_type_size()) == 0) return 0;

  return size * mydomain.cell_count();
}


const r_Minterval &r_Tile_Compression::get_domain( void ) const
{
  return mydomain;
}


const r_Base_Type *r_Tile_Compression::get_base_type( void ) const
{
  return mytype;
}


void r_Tile_Compression::set_storage_handler( const r_Storage_Man &newStore )
{
  mystore = newStore;
}


// should only be called if COMPBENCHMARK is defined
void r_Tile_Compression::instantiate_timer( const char *func, int level )
{
#ifdef COMPBENCHMARK
  if (myTimer != NULL)
    delete myTimer;

  myTimer = new RMTimer(get_name(), func, level);
  myTimer->start();
#endif
}


void r_Tile_Compression::resume_timer( void )
{
  CBENCH_STATEMENT(myTimer->resume());
}

void r_Tile_Compression::pause_timer( void )
{
  CBENCH_STATEMENT(myTimer->pause());
}



void r_Tile_Compression::write_short( void *dest, r_Short val )
{
  unsigned char *dptr = (unsigned char*)dest;
  
  dptr[0] = (val & 0xff);
  dptr[1] = ((val >> 8) & 0xff);
}


void r_Tile_Compression::write_long( void *dest, r_Long val )
{
  unsigned char *dptr = (unsigned char*)dest;

  dptr[0] = (val & 0xff);
  dptr[1] = ((val >> 8) & 0xff);
  dptr[2] = ((val >> 16) & 0xff);
  dptr[3] = ((val >> 24) & 0xff);
}


void r_Tile_Compression::read_short( const void *src, r_Short &val )
{
  const unsigned char *sptr = (const unsigned char *)src;

  val = (r_Short)(sptr[0] | (sptr[1] << 8));
}


void r_Tile_Compression::read_long( const void *src, r_Long &val )
{
  const unsigned char *sptr = (const unsigned char *)src;

  val = (r_Long)(sptr[0] | (sptr[1] << 8) | (sptr[2] << 16) | (sptr[3] << 24));
}


r_Data_Format r_Tile_Compression::get_decomp_format( void ) const
{
  return r_Array;
}


unsigned int r_Tile_Compression::get_atom_info( const r_Base_Type* baseType, unsigned int* sizes, unsigned int* idxptr )
{
  unsigned int sum;
  unsigned int idx;

  RMDBGONCE( 3, RMDebug::module_compression, "r_Tile_Compression", "get_atom_info()" );

  if (idxptr == NULL)
    idx = 0;
  else
    idx = *idxptr;

  if (baseType->isStructType())
  {
    r_Structure_Type* structType = (r_Structure_Type*)baseType;
    r_Structure_Type::attribute_iterator iter(structType->defines_attribute_begin());

    sum = 0;
    while (iter != structType->defines_attribute_end())
    {
      r_Base_Type *newType = (r_Base_Type*)((*iter).type_of().clone());
      if (newType->isStructType())
	sum += get_atom_info(newType, sizes);
      else
      {
	sum++;
	if (sizes != NULL)
	  sizes[idx++] = newType->size();
      }
      delete newType;
      iter++;
    }
  }
  else
  {
    sum = 1;
    if (sizes != NULL)
      sizes[idx++] = baseType->size();
  }
  
  if (idxptr != NULL)
    *idxptr = idx;

  return sum;
}


const char *r_Tile_Compression::get_format_info( unsigned int number, r_Data_Format &fmt )
{
  unsigned int numFormats = sizeof(all_formats) / sizeof(tile_comp_format_t);

  if (number >= numFormats)
    return NULL;

  fmt = all_formats[number].format;
  return all_formats[number].name;
}


r_Tile_Compression::Support_Format r_Tile_Compression::check_data_format( r_Data_Format fmt )
{
  return r_Tile_Compression::COMPRESSION;
}


// create tile compression object from data format
r_Tile_Compression *r_Tile_Compression::create( r_Data_Format fmt, const r_Minterval &dom, const r_Base_Type *type ) throw(r_Error)
{
  r_Tile_Compression *result;

  result = new r_Tile_Comp_None(dom, type);

  /*
  switch (fmt)
    {
      // raw array data
    case r_Array:
      result = new r_Tile_Comp_None(dom, type);
      break;
    default:
      RMInit::logOut << "r_Tile_Compression::create(): Unknown or unsupported tile compression " << fmt << endl;
      r_Error err(r_Error::r_Error_General);
      throw(err);
      break;
    }
  */
  return result;
}


// create tile compression object from algorithm name
r_Tile_Compression *r_Tile_Compression::create( const char *name, const r_Minterval &dom, const r_Base_Type *type) throw(r_Error)
{
  return create(get_format_from_name(name), dom, type);
}


// get the data format for a compression name
r_Data_Format r_Tile_Compression::get_format_from_name( const char *name ) throw(r_Error)
{
  unsigned int numFormats = sizeof(all_formats) / sizeof(tile_comp_format_t);
  unsigned int i;

  for (i=0; i<numFormats; i++)
  {
    if (strcasecmp(name, all_formats[i].name) == 0)
      break;
  }
  if (i >= numFormats)
  {
    RMInit::logOut << "r_Tile_Compression::create(): Unknown compression algorithm " << name << endl;
    r_Error err(r_Error::r_Error_General);
    throw(err);
  }
  return all_formats[i].format;
}

