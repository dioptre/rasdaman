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
 * FILE: vff.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_VFF
 *
 * COMMENTS:
 *
 * Provides interface to convert data to VFF and back.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <iostream>
#include <strstream>

#include "raslib/rminit.hh"
#include "raslib/endian.hh"
#include "raslib/miterd.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/parseparams.hh"
#include "conversion/vff.hh"


// file magic
const char *r_Conv_VFF::fileMagic = "ncaa";

// keyword enumerators, must be kept in sync with keywords
enum vff_keyword_e {
  vffkey_rank = 0,
  vffkey_type,
  vffkey_format,
  vffkey_size,
  vffkey_origin,
  vffkey_extent,
  vffkey_aspect,
  vffkey_bands,
  vffkey_bits,
  vffkey_value,
  vffkey_endian,
  vffkey_dorder,
  vffkey_NUMBER
};

// Keywords
const char *r_Conv_VFF::keywords[] = {
  // mandatory
  "rank",
  "type",
  "format",
  "size",
  "origin",
  "extent",
  "aspect",
  "bands",
  "bits",
  // optional
  "value",
  "endianness",
  "data_order"
};

// values
const char *r_Conv_VFF::kval_Raster = "raster";
const char *r_Conv_VFF::kval_Slice = "slice";
const char *r_Conv_VFF::kval_LEndian = "little_endian";
const char *r_Conv_VFF::kval_BEndian = "big_endian";

// default data order
const char *r_Conv_VFF::dfltDataOrder2 = "xy";
const char *r_Conv_VFF::dfltDataOrder3 = "yzx";

// default dimension order
const char *r_Conv_VFF::dfltDimOrder2 = "xy";
const char *r_Conv_VFF::dfltDimOrder3 = "yzx";

// end of header
const char r_Conv_VFF::endOfHeader = 0x0c;

// method names
const char *r_Conv_VFF::method_convTo = "r_Conv_VFF::convertTo()";
const char *r_Conv_VFF::method_convFrom = "r_Conv_VFF::convertFrom()";


r_Conv_VFF::r_Conv_VFF( const char *src, const r_Minterval &interv, int tp ) throw(r_Error)
: r_Convertor(src, interv, tp)
{
  initVFF();
}


r_Conv_VFF::r_Conv_VFF( const char *src, const r_Minterval &interv, const r_Type *tp ) throw(r_Error)
: r_Convertor(src, interv, tp, true)
{
  initVFF();
}


r_Conv_VFF::~r_Conv_VFF( void )
{
  if (dorderParam != NULL)
    delete [] dorderParam;
  if (dimOrderParam != NULL)
    delete [] dimOrderParam;
}


void r_Conv_VFF::initVFF( void )
{
  dorderParam = NULL;
  dimOrderParam = NULL;
  dfltEndianness = 0;	// big endian

  if (params == NULL)
    params = new r_Parse_Params;

  params->add("dorder", &dorderParam, r_Parse_Params::param_type_string);
  params->add("dimorder", &dimOrderParam, r_Parse_Params::param_type_string);
  params->add("vffendian", &dfltEndianness, r_Parse_Params::param_type_int);
}


void r_Conv_VFF::skip_white( const char *&str )
{
  while ((*str != endOfHeader) && (isspace((unsigned int)(*str)))) str++;
}

void r_Conv_VFF::write_interval( const char *keyname, std::ostream &str, const r_Minterval &iv, const unsigned int *order, r_Range inc )
{
  r_Dimension i;
  str << keyname << '=' << (iv[order[0]].high() - iv[order[0]].low() + inc);
  for (i=1; i<iv.dimension(); i++)
    str << ' ' << (iv[order[i]].high() - iv[order[i]].low() + inc);
  str << ";\n";
}

void r_Conv_VFF::write_origin( const char *keyname, std::ostream &str, const r_Minterval &iv, const unsigned int *order )
{
  r_Dimension i;
  str << keyname << '=' << iv[order[0]].low();
  for (i=1; i<iv.dimension(); i++)
    str << ' ' << iv[order[i]].low();
  str << ";\n";
}

const char *r_Conv_VFF::read_vector( r_Dimension dim, const char *str, double *&vec )
{
  if (dim == 0)
    return NULL;

  vec = new double[dim];
  r_Dimension i;

  for (i=0; i<dim; i++)
  {
    const char *rest;
    vec[i] = strtod(str, (char**)&rest);
    if (str == rest)
      break;
    str = rest;
  }
  if (i < dim)
  {
    delete [] vec;
    vec = NULL;
    return NULL;
  }
  return str;
}

const char *r_Conv_VFF::read_string( const char *str, char *dest, bool allowSpace )
{
  const char *d = str;
  char *b = dest;

  while (*d != ';')
  {
    if ((allowSpace == 1) && (isspace((unsigned int)(*d))))
      break;
    *b++ = *d++;
  }
  *b++ = '\0';
  return d;
}

const char *r_Conv_VFF::get_endian_id( void )
{
  return ((r_Endian::get_endianness() == r_Endian::r_Endian_Big) ? kval_BEndian : kval_LEndian);
}


const char *r_Conv_VFF::get_default_order( r_Dimension dim )
{
  return (dim == 2) ? dfltDataOrder2 : dfltDataOrder3;
}


const char *r_Conv_VFF::get_default_dim_order( r_Dimension dim )
{
  return (dim == 2) ? dfltDimOrder2 : dfltDimOrder3;
}


int r_Conv_VFF::parse_data_order( r_Dimension dim, const char *dstr, unsigned int *order )
{
  unsigned char *mapped = new unsigned char[dim];
  unsigned int i;

  memset(mapped, 0, dim);
  for (i=0; i<dim; i++)
  {
    unsigned int map;

    switch (dstr[i])
    {
      case 'x':
	map = 1; break;
      case 'y':
	map = 2; break;
      case 'z':
	map = 3; break;
      default:
	map = 0; break;
    }

    if ((map == 0) || (mapped[map-1] != 0))
      break;

    order[i] = map-1;
    mapped[map-1] = 1;
    //cout << order[i] << endl;
  }

  delete [] mapped;

  if (i < dim)
  {
    RMInit::logOut << "r_Conv_VFF::parse_data_order(): descriptor " << dstr
		   << " inconsistent, revert to defaults" << endl;

    return parse_data_order(dim, get_default_order(dim), order);
  }
  
  return 0;
}


unsigned int *r_Conv_VFF::get_dimension_order( r_Dimension dim ) const
{
  unsigned int *order = new unsigned int[dim];
  char dimOrder[8];

  // Anders Ledberg: Karolinska reader _always_ uses yzx as dimension order
  if (dimOrderParam == NULL)
    strncpy(dimOrder, get_default_dim_order(dim), 8);
  else
    strncpy(dimOrder, dimOrderParam, 8);

  dimOrder[7] = '\0';
  parse_data_order(dim, dimOrder, order);

  return order;
}


const char *r_Conv_VFF::get_default_endianness( void ) const
{
  return (dfltEndianness == 0) ? kval_BEndian : kval_LEndian;
}


r_convDesc &r_Conv_VFF::convertTo( const char *options ) throw(r_Error)
{
  r_Dimension dim;
  int bits, typeSize;
  char header[1024]; // and ``640k should be enough for everyone''...
  char dataOrder[8];

  dim = desc.srcInterv.dimension();
  if ((dim < 2) || (dim > 3))
  {
    r_Error err(r_Error::r_Error_General);
    throw(err);
  }

  switch (desc.baseType)
  {
    case ctype_bool:
    case ctype_char:
    case ctype_int8:
    case ctype_uint8:
      bits = 8; typeSize = 1; break;
    case ctype_int16:
    case ctype_uint16:
      bits = 16; typeSize = 2; break;
    case ctype_int32:
    case ctype_uint32:
      bits = 32; typeSize = 4; break;
    default:
      r_Error err(r_Error::r_Error_General);
      throw(err);
      break;
  }
  //cout << "TYPE " << desc.baseType << ", SIZE " << typeSize << endl;

  if (options != NULL)
    params->process(options);

  if (dorderParam != NULL)
    strcpy(dataOrder, dorderParam);
  else
    strcpy(dataOrder, get_default_order(dim));

  std::ostrstream memstr(header, 1024);
  
  // order of dimensions in vectors
  unsigned int *dimOrder = get_dimension_order(dim);

  // write header
  memstr << fileMagic << '\n';
  memstr << keywords[vffkey_rank] << '=' << dim << ";\n";
  memstr << keywords[vffkey_type] << '=' << kval_Raster << ";\n";
  memstr << keywords[vffkey_format] << '=' << kval_Slice << ";\n";
  // the dimensions are still ordered like in the VFF file, only the internal
  // linearization differs!
  write_interval(keywords[vffkey_size], memstr, desc.srcInterv, dimOrder, 1);
  write_origin(keywords[vffkey_origin], memstr, desc.srcInterv, dimOrder);
  write_interval(keywords[vffkey_extent], memstr, desc.srcInterv, dimOrder);
  if (dim == 2)
  {
    // FIXME, aspect should be modelled too
    memstr << keywords[vffkey_aspect] << "=1.0 1.0;\n";
  }
  else
  {
    // FIXME, ditto
    memstr << keywords[vffkey_aspect] << "=1.0 1.0 1.0;\n";
  }
  
  // only suypport one band ATM
  memstr << keywords[vffkey_bands] << "=1;\n";
  memstr << keywords[vffkey_bits] << '=' << bits << ";\n";
  memstr << keywords[vffkey_endian] << '=' << get_endian_id() << ";\n";
  memstr << keywords[vffkey_dorder] << '=' << dataOrder << ";\n";
  memstr << '\n' << endOfHeader << '\n' << '\0';

  unsigned long headerSize, dataSize, totalSize;

  headerSize = strlen(header);
  dataSize = desc.srcInterv.cell_count() * typeSize;
  totalSize = headerSize + dataSize;

  if ((desc.dest = (char*)mystore.storage_alloc(headerSize + dataSize)) == NULL)
  {
    r_Error err(r_Error::r_Error_General);
    throw(err);
  }
  memcpy(desc.dest, header, headerSize);

  // treat all dimensions alike thanks to generic iterators
  
  // source iterator, iterate in user order
  r_MiterDirect iter((void*)desc.src, desc.srcInterv, desc.srcInterv, typeSize, 1);
  unsigned int *order, *steps;

  order = new unsigned int[dim];
  steps = new unsigned int[dim];

  for (unsigned int i=0; i<dim; i++)
    steps[i] = 1;
    
  parse_data_order(dim, dataOrder, order);

  // note: dest may not be aligned to a position conforming to its base type,
  // so copy byte-wise.
  unsigned char *dptr = (unsigned char*)(desc.dest + headerSize);
  switch (typeSize)
  {
    case 1:
      {
	while (iter.isDone() == 0)
	{
	  *dptr = *((const unsigned char*)iter.getData(order));
	  iter.iterateUserOrder(order, steps);
	  dptr++;
	}
      }
      break;
    case 2:
      {
	while (iter.isDone() == 0)
	{
	  const unsigned char *sptr = (const unsigned char*)iter.getData(order);
	  dptr[0] = sptr[0];
	  dptr[1] = sptr[1];
	  //cout << iter << ':' << (long)((const char*)sptr-desc.src) << ':' << (unsigned short)((sptr[0] << 8) | (sptr[1])) << ' ';
	  iter.iterateUserOrder(order, steps);
	  dptr+=2;
	}
      }
      break;
    case 4:
      {
	while (iter.isDone() == 0)
	{
	  const unsigned char *sptr = (const unsigned char*)iter.getData(order);
	  dptr[0] = sptr[0];
	  dptr[1] = sptr[1];
	  dptr[2] = sptr[2];
	  dptr[3] = sptr[3];
	  iter.iterateUserOrder(order, steps);
	  dptr+=4;
	}
      }
      break;
    default:
      break;
  }

  delete [] steps;
  delete [] order;
  delete [] dimOrder;
  
  desc.destInterv = r_Minterval(1);
  desc.destInterv << r_Sinterval((r_Range)0, (r_Range)totalSize-1);
  desc.destType = r_Type::get_any_type("char");

  return desc;
}


r_convDesc &r_Conv_VFF::convertFrom( const char *options ) throw(r_Error)
{
  const char *header = desc.src;
  unsigned char *keysRead;
  unsigned int i;
  r_Dimension dim=0;
  int bits, bands;
  double *vecSize, *vecOrigin, *vecValue;
  char endian[32];
  char dataOrder[8];

  if (strncmp(header, fileMagic, strlen(fileMagic)) != 0)
  {
    r_Error err(r_Error::r_Error_General);
    throw(err);
  }

  keysRead = new unsigned char[vffkey_NUMBER];
  memset(keysRead, 0, vffkey_NUMBER);

  header += strlen(fileMagic);
  skip_white(header);

  vecSize = NULL;
  vecOrigin = NULL;
  vecValue = NULL;

  strcpy(endian, get_default_endianness());

  if (options != NULL)
    params->process(options);

  // no order -- check later if one was defined and init it _after_ we know the rank
  dataOrder[0] = '\0';

  do
  {
    if (*header != endOfHeader);
    {
      const char *rest;
      double *vecTemp;

      for (i=0; i<(unsigned int)vffkey_NUMBER; i++)
      {
	if (strncmp(header, keywords[i], strlen(keywords[i])) == 0)
	  break;
      }

      if (i == vffkey_NUMBER)
      {
	// skip unknown keywords
	while ((*header != ';') && (*header != endOfHeader))
	  header++;
	if (*header == ';')
	  header++;
      }
      else
      {
	//cout << "KEY " << keywords[i] << endl;

	keysRead[i] = 1;

	header += strlen(keywords[i]);
	skip_white(header);

	if (*header != '=')
	  break;

	header++;
	skip_white(header);

	rest = header;
	switch ((vff_keyword_e)i)
	{
	  case vffkey_rank:
	    dim = strtol(header, (char**)&rest, 10);
	    if ((rest == header) || (dim < 2) || (dim > 3))
	    {
	      RMInit::logOut << method_convFrom << ": bad rank " << dim << endl;
	      rest = NULL;
	    }
	    break;
	  case vffkey_type:
	    if (strncmp(header, kval_Raster, strlen(kval_Raster)) == 0)
	      rest = header + strlen(kval_Raster);
	    else
	    {
	      RMInit::logOut << method_convFrom << ": bad type" << endl;
	      rest = NULL;
	    }
	    break;
	  case vffkey_format:
	    if (strncmp(header, kval_Slice, strlen(kval_Slice)) == 0)
	      rest = header + strlen(kval_Slice);
	    else
	    {
	      RMInit::logOut << method_convFrom << ": bad format" << endl;
	      rest = NULL;
	    }
	    break;
	  case vffkey_size:
	    rest = read_vector(dim, header, vecSize);
	    break;
	  case vffkey_origin:
	    rest = read_vector(dim, header, vecOrigin);
	    break;
	  case vffkey_extent:
	    // extent just parsed, otherwise ignored
	    rest = read_vector(dim, header, vecTemp);
	    delete [] vecTemp;
	    break;
	  case vffkey_aspect:
	    // FIXME: aspect ignored
	    rest = read_vector(dim, header, vecTemp);
	    delete [] vecTemp;
	    break;
	  case vffkey_bands:
	    bands = strtol(header, (char**)&rest, 10);
	    if ((rest == header) || (bands != 1))
	    {
	      RMInit::logOut << method_convFrom << ": bad number of bands " << bands << endl;
	      rest = NULL;
	    }
	    break;
	  case vffkey_bits:
	    bits = strtol(header, (char**)&rest, 10);
	    if ((rest == header) || ((bits != 8) && (bits != 16) && (bits != 32)))
	    {
	      RMInit::logOut << method_convFrom << ": bad number of bits " << bits << endl;
	      rest = NULL;
	    }
	    break;
	  case vffkey_value:
	    // FIXME: values ignored
	    rest = read_vector(2, header, vecValue);
	    break;
	  case vffkey_endian:
	    rest = read_string(header, endian);
	    break;
	  case vffkey_dorder:
	    rest = read_string(header, dataOrder);
	    break;
	  default:
	    break;
	}
	if (rest == NULL)
	  break;

	header = rest;
	skip_white(header);
	if (*header == ';')
	  header++;
      }
    }
    skip_white(header);
  }
  while (*header != endOfHeader);

  // was there a data order? no ==> init either used-defined or default
  if (dataOrder[0] == '\0')
  {
    if (dorderParam != NULL)
      strcpy(dataOrder, dorderParam);
    else
      strcpy(dataOrder, get_default_order(dim));
  }
  
  //cout << "RANK " << dim << ", BANDS " << bands << ", BITS " << bits << endl;

  // check whether all mandatory keywords are present
  for (i=0; i<(unsigned int)vffkey_value; i++)
  {
    if (keysRead[i] == 0)
    {
      header = NULL;
      break;
    }
  }

  if ((header == NULL) || (*header != endOfHeader))
  {
    // parse error
    RMInit::logOut << method_convFrom << ": PARSE ERROR" << endl;

    delete [] keysRead;

    if (vecSize != NULL)
      delete [] vecSize;
    if (vecOrigin != NULL)
      delete [] vecOrigin;
    if (vecValue != NULL)
      delete [] vecValue;

    r_Error err(r_Error::r_Error_General);
    throw(err);
  }

  // after this instruction, header points to the linefeed
  header++;
  // after this instruction, header points to the data
  header++;

  desc.destInterv = r_Minterval(dim);
  
  // order of dimensions in vectors
  unsigned int *dimOrder = get_dimension_order(dim);
  
  // the order of the dimensions is the same, only the linearization differs
  for (i=0; i<dim; i++)
  {
    // the old method didn't work for high<0 (wrong rounding direction)
    r_Range low, width;
    low = (r_Range)(vecOrigin[i]);
    width = (r_Range)(vecSize[i]);
    //cout << "SIZE " << dimOrder[i] << ": " << width << ", ORG " << low << endl;
    desc.destInterv[dimOrder[i]] = r_Sinterval(low, low+width-1);
  }
  //cout << desc.destInterv << endl;

  delete [] vecOrigin;
  delete [] vecSize;

  int typeSize = (bits>>3);
  unsigned long dataSize = desc.destInterv.cell_count() * typeSize;
  //cout << "Type size " << typeSize << ", dim " << dim << endl;

  if ((desc.dest = (char*)mystore.storage_alloc(dataSize)) == NULL)
  {
    delete [] keysRead;

    if (vecValue != NULL)
      delete [] vecValue;

    r_Error err(r_Error::r_Error_General);
    throw(err);
  }
  
  // treat all dimensions alike

  // must copy byte-by-byte because the source data may not be aligned!
  r_MiterDirect iter(desc.dest, desc.destInterv, desc.destInterv, typeSize, 1);
  unsigned int *order, *steps;
  const unsigned char *sptr = (const unsigned char *)header;

  order = new unsigned int[dim];
  steps = new unsigned int[dim];

  for (i=0; i<dim; i++)
    steps[i] = 1;

  parse_data_order(dim, dataOrder, order);

  switch (typeSize)
  {
    case 1:
      while (iter.isDone() == 0)
      {
	*((r_Char*)iter.getData(order)) = *sptr;
	iter.iterateUserOrder(order, steps);
	sptr++;
      }
      break;
    case 2:
      while (iter.isDone() == 0)
      {
	unsigned char *dptr = (unsigned char*)iter.getData(order);
	dptr[0] = sptr[0];
	dptr[1] = sptr[1];
	//cout << iter << ':' << (long)((char*)dptr - desc.dest) << ':' << (long)((char*)sptr - header) << ':' << (unsigned short)((sptr[0] << 8) | sptr[1]) << ' ';
	iter.iterateUserOrder(order, steps);
	sptr+=2;
      }
      break;
    case 4:
      while (iter.isDone() == 0)
      {
	unsigned char *dptr = (unsigned char*)iter.getData(order);
	dptr[0] = sptr[0];
	dptr[1] = sptr[1];
	dptr[2] = sptr[2];
	dptr[3] = sptr[3];
	iter.iterateUserOrder(order, steps);
	sptr+=4;
      }
      break;
    default:
      break;
  }

  delete [] order;
  delete [] steps;
  delete [] dimOrder;

  switch (typeSize)
  {
    case 1:
      desc.destType = get_external_type(ctype_char);
      break;
    case 2:
      desc.destType = get_external_type(ctype_uint16);
      if (strcmp(endian, get_endian_id()) != 0)
	r_Endian::swap_array(dataSize, (const r_UShort*)desc.dest, (r_UShort*)desc.dest);
      break;
    case 4:
      desc.destType = get_external_type(ctype_uint32);
      if (strcmp(endian, get_endian_id()) != 0)
	r_Endian::swap_array(dataSize, (const r_ULong*)desc.dest, (r_ULong*)desc.dest);
      break;
    default:
      break;
  }

  //desc.destType->print_status();

  if (vecValue != NULL)
    delete [] vecValue;

  delete [] keysRead;

  return desc;
}


const char *r_Conv_VFF::get_name( void ) const
{
  return format_name_vff;
}


r_Data_Format r_Conv_VFF::get_data_format( void ) const
{
  return r_VFF;
}


r_Convertor *r_Conv_VFF::clone( void ) const
{
  return new r_Conv_VFF(desc.src, desc.srcInterv, desc.baseType);
}
