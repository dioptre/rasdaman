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
 * SOURCE: csv.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_CSV
 *
 * COMMENTS:
 *
 * Provides functions to convert data to CSV SD and back.
 *
*/

/* Added by Sorin Stancu-Mara. Definition clashed for type int8, define in both 
* /usr/include/csv.h and in /usr/include/tiff.h
* This will supress the tiff.h definition.
* Both definitions are similar
* 
* 2011-may-24  DM          added support for structured types
* 2012-feb-05  DM          convert recursive printing to iterative
*/
#define HAVE_INT8
#define STRUCT_DELIMITER_OPEN "\""
#define STRUCT_DELIMITER_CLOSE "\""
#define STRUCT_DELIMITER_ELEMENT " "

#include "conversion/csv.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "raslib/primitivetype.hh"
#include "raslib/structuretype.hh"
#include <iostream>
#include <fstream>
#include <cstring>

#include "csv.hh"

#include <stdio.h>
#include <iostream>
#include <stack>

#include "debug/debug-srv.hh"

#define DIM_BOUNDARY -1

using namespace std;

r_Conv_CSV::r_Conv_CSV(const char *src, const r_Minterval &interv, const r_Type *tp) throw(r_Error) 
: r_Convertor(src, interv, tp, true)
{
}



r_Conv_CSV::r_Conv_CSV(const char *src, const r_Minterval &interv, int tp) throw(r_Error)
: r_Convertor(src, interv, tp)
{
}

r_Conv_CSV::~r_Conv_CSV(void)
{
}

template <class baseType, class castType> 
void r_Conv_CSV::print(std::ofstream &f, baseType* val, int *dims, int dim) {
  ENTER("r_Conv_CSV::print( dimensions: " << dim << " )");
  
  int dimensions, dimsIndex;
  stack<int> dimensionsStack; 
  stack<int> dimsIndexStack;
  dimensionsStack.push(dim);
  dimsIndexStack.push(0);
  
  while (!dimensionsStack.empty()) {
    dimensions = dimensionsStack.top();
    dimensionsStack.pop();
    if (dimensions == DIM_BOUNDARY) {
      f << "}";
      if (!dimensionsStack.empty() && dimensionsStack.top() != DIM_BOUNDARY)
        f << ",";
      continue;
    }
    
    dimsIndex = dimsIndexStack.top();
    dimsIndexStack.pop();
    
    if (dimensions == 0) {
      f << (castType)val[0];
    } else if (dimensions == 1) {
      f << "{";
      for (int i=0; i<dims[dimsIndex]; ++i, val++) {
        f << (castType)val[0];
        if (i<dims[dimsIndex]-1)
          f << ",";
      }
      f << "}";
      if (!dimensionsStack.empty() && dimensionsStack.top() != DIM_BOUNDARY) {
        f << ",";
      }
    } else {
      if (dimensions != dim) {
        f << "{";
        dimensionsStack.push(DIM_BOUNDARY);
      }
      for (int i = 0; i < dims[dimsIndex]; i++) {
        dimensionsStack.push(dimensions - 1);
        dimsIndexStack.push(dimsIndex + 1);
      }
    }
  }
  
  LEAVE("r_Conv_CSV::print()");
}

void r_Conv_CSV::printStructVal(std::ofstream &f) {
  r_Structure_Type *st = (r_Structure_Type*) desc.srcType;
  r_Structure_Type::attribute_iterator iter(st->defines_attribute_begin());
  while (iter != st->defines_attribute_end()) {
    if ((*iter).type_of().isStructType()) {
      printStructVal(f);
    } else if ((*iter).type_of().isPrimitiveType()) {
      r_Primitive_Type *pt = (r_Primitive_Type*) &(*iter).type_of();
      switch ((*iter).type_of().type_id()) {
        case r_Type::ULONG: f << pt->get_ulong(val); break;
        case r_Type::USHORT: f << pt->get_ushort(val); break;
        case r_Type::BOOL: f << pt->get_boolean(val) ? "T": "F"; break;
        case r_Type::LONG: f << pt->get_long(val); break;
        case r_Type::SHORT: f << pt->get_short(val); break;
        case r_Type::OCTET: f << (int) (pt->get_octet(val)); break;
        case r_Type::DOUBLE: f << pt->get_double(val); break;
        case r_Type::FLOAT: f << pt->get_float(val); break;
        case r_Type::CHAR: f << (int) (pt->get_char(val)); break;
        default: f << (int) (pt->get_char(val)); break;
      }
      val += pt->size();
    }
    iter++;
    if (iter != st->defines_attribute_end())
      f << STRUCT_DELIMITER_ELEMENT;
  }
}

void r_Conv_CSV::printStruct(std::ofstream &f, int *dims, int dim) {
  if (dim == 1) {
    for (int i = 0; i < dims[0]; ++i) {
      f << STRUCT_DELIMITER_OPEN;
      printStructVal(f);
      f << STRUCT_DELIMITER_CLOSE;
      if (i < dims[0] - 1)
        f << ",";
    }
  } else {
    for (int i = 0; i < dims[0]; ++i) {
      f << "{";
      printStruct(f, dims + 1, dim - 1);
      f << "}";
      if (i < dims[0] - 1)
        f << ",";
    }
  }
}

r_convDesc &r_Conv_CSV::convertTo( const char *options ) throw(r_Error)
{
  ENTER("r_Conv_CSV::convertTo()");
  char name[256];
  strncpy(name, tmpnam(NULL), 256);
  std::ofstream ftemp(name);
  //int size = getTypeSize(desc.baseType);
  int rank, i;
  int *dimsizes;
  rank = desc.srcInterv.dimension();
  char *src = (char*) desc.src;

  dimsizes = new int[rank]; 

  for (i=0; i<rank; i++)
  {
    dimsizes[i] = desc.srcInterv[i].high() - desc.srcInterv[i].low() + 1;
  }
  if (desc.srcType->isStructType()) {
    val = (char*) src;
    printStruct(ftemp, dimsizes, rank);
    val = NULL;
  } else
  switch (desc.baseType)
    {
    case ctype_int8: print<const r_Octet, int>(ftemp, (const r_Octet*)src, dimsizes, rank); break;
    case ctype_uint8: 
    case ctype_char: 
    case ctype_bool: print<r_Char, int>(ftemp, (r_Char*)src, dimsizes, rank); break;      
    case ctype_int16: print<r_Short, int>(ftemp, (r_Short*)src, dimsizes, rank); break;
    case ctype_uint16: print<r_UShort, int>(ftemp, (r_UShort*) src, dimsizes, rank); break;
    case ctype_int32: print<r_Long, int>(ftemp, (r_Long*) src, dimsizes, rank);  break;
    case ctype_uint32: print<r_ULong, int>(ftemp, (r_ULong*) src, dimsizes, rank);  break;
    case ctype_int64: print<long long, long long>(ftemp, (long long*) src, dimsizes, rank); break;
    case ctype_uint64: print<unsigned long long, unsigned long long>(ftemp, (unsigned long long*) src, dimsizes, rank); break;
    case ctype_float32: print<r_Float, float>(ftemp, (r_Float*) src, dimsizes, rank); break;
    case ctype_float64: print<r_Double, float>(ftemp, (r_Double*) src, dimsizes, rank); break;
    default:
    {
      RMInit::logOut << "r_Conv_CSV::convertTo(): can not convert data of base type " << desc.baseType << endl;
      LEAVE("r_Conv_CSV::convertTo()");
      throw r_Error(r_Error::r_Error_TypeInvalid);
    }
  }

  delete [] dimsizes; dimsizes=NULL;
  ftemp.close();

  FILE *fp;
  int filesize;
  
  if ((fp = fopen(name, "rb")) == NULL)
  {
    RMInit::logOut << "r_Conv_CSV::convertTo(): unable to read back file." << endl;
    LEAVE("r_Conv_CSV::convertTo()");
    throw r_Error(r_Error::r_Error_General);
  }
  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);

  desc.destInterv = r_Minterval(1);
  desc.destInterv << r_Sinterval((r_Range)0, (r_Range)filesize - 1);

  if ((desc.dest = (char*)mystore.storage_alloc(filesize)) == NULL)
  {
    RMInit::logOut << "r_Conv_CSV::convertTo(): out of memory error" << endl;
    fclose(fp);
    LEAVE("r_Conv_CSV::convertTo()");
    throw r_Error(MEMMORYALLOCATIONERROR);
  }
  fseek(fp, 0, SEEK_SET);
  fread(desc.dest, 1, filesize, fp);

  fclose(fp);

  remove(name);

  // Result is just a bytestream
  desc.destType = r_Type::get_any_type("char");

  LEAVE("r_Conv_CSV::convertTo()");
  return desc;
}



r_convDesc &r_Conv_CSV::convertFrom(const char *options) throw(r_Error)
{
  RMInit::logOut << "importing CSV data not yet implemented" << endl;
  throw new r_Error(CONVERSIONFORMATNOTSUPPORTED);
  return desc;
}



const char *r_Conv_CSV::get_name( void ) const
{
  return "csv";
}


r_Data_Format r_Conv_CSV::get_data_format( void ) const
{
  return r_CSV;
}


r_Convertor *r_Conv_CSV::clone( void ) const
{
  return new r_Conv_CSV(desc.src, desc.srcInterv, desc.baseType);
}
