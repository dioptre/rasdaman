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
*/
#define HAVE_INT8

#include "conversion/csv.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "raslib/primitivetype.hh"
#include <iostream>
#include <fstream>
#include <cstring>

#include "csv.hh"

#include <stdio.h>
#include <iostream>


r_Conv_CSV::r_Conv_CSV(const char *src, const r_Minterval &interv, const r_Type *tp) throw(r_Error) 
: r_Convertor(src, interv, tp, true)
{
  if (tp->isStructType())
  {
    RMInit::logOut << "r_Conv_CSV::r_Conv_CSV(): structured types not supported." << endl;
    throw r_Error(r_Error::r_Error_General);
  }
}



r_Conv_CSV::r_Conv_CSV(const char *src, const r_Minterval &interv, int tp) throw(r_Error)
: r_Convertor(src, interv, tp)
{
}


/*
int r_Conv_HDF::getTypeSize(int intType, char *format)
{

  switch (intType)
  {
    case ctype_int8: strcpy(format, "%c"); return 1; break;
    case ctype_uint8:
    case ctype_char:
    case ctype_bool: return 1; break;
    case ctype_int16: return 2; break;
    case ctype_uint16: return 2; break;
    case ctype_int32: return 4; break;
    case ctype_uint32: return 4; break;
    case ctype_int64: return 8; break;
    case ctype_uint64: return 8; break;
    case ctype_float32: return 4; break;
    case ctype_float64: return 8; break;
    default: return 1; break;
  }
  return 1;
}
*/

r_Conv_CSV::~r_Conv_CSV(void)
{
}

template <class baseType, class castType> 
void r_Conv_CSV::print(std::ofstream &f, baseType* val, int *dims, int dim) {
  if (dim==1) {
    for (int i=0; i<dims[0]-1; ++i, val++)
      f << (castType)val[0] << ",";
    f << (castType) val[0]; val++;
  } else {
    for (int i=0; i<dims[0]-1; ++i, val++) {
      f << "{";
      print<baseType, castType>(f, val, dims+1, dim-1);
      f << "},";
    }
      f << "{";
      print<baseType, castType>(f, val, dims+1, dim-1);
      f << "}";    
  }
}


r_convDesc &r_Conv_CSV::convertTo( const char *options ) throw(r_Error)
{
  char name[256];
  strncpy(name, tmpnam(NULL), 256);
  std::ofstream ftemp(name);
  //int size = getTypeSize(desc.baseType);
  int rank, i;
  int *dimsizes;
  rank = desc.srcInterv.dimension();
  const char *src = desc.src;

  char *t;

  dimsizes = new int[rank]; 

  for (i=0; i<rank; i++)
  {
    dimsizes[i] = desc.srcInterv[i].high() - desc.srcInterv[i].low() + 1;
  }

  switch (desc.baseType)
    {
    case ctype_int8: print<const r_Octet, int>(ftemp, (const r_Octet* &)src, dimsizes, rank); break;
    case ctype_uint8: 
    case ctype_char: 
    case ctype_bool: print<r_Char, int>(ftemp, (r_Char* &)src, dimsizes, rank); break;      
    case ctype_int16: print<r_Short, int>(ftemp, (r_Short* &)src, dimsizes, rank); break;
    case ctype_uint16: print<r_UShort, int>(ftemp, (r_UShort* &) src, dimsizes, rank); break;
    case ctype_int32: print<r_Long, int>(ftemp, (r_Long* &) src, dimsizes, rank);  break;
    case ctype_uint32: print<r_ULong, int>(ftemp, (r_ULong* &) src, dimsizes, rank);  break;
    case ctype_int64: print<long long, long long>(ftemp, (long long* &) src, dimsizes, rank); break;
    case ctype_uint64: print<unsigned long long, unsigned long long>(ftemp, (unsigned long long* &) src, dimsizes, rank); break;
    case ctype_float32: print<r_Float, float>(ftemp, (r_Float* &) src, dimsizes, rank); break;
    case ctype_float64: print<r_Double, float>(ftemp, (r_Double* &) src, dimsizes, rank); break;
    default: print<r_Char, int>(ftemp, (r_Char* &)src, dimsizes, rank);
  }

  delete [] dimsizes; dimsizes=NULL;
  ftemp.close();

  FILE *fp;
  int filesize;
  
  if ((fp = fopen(name, "rb")) == NULL)
  {
    RMInit::logOut << "r_Conv_CSV::convertTo(): unable to read back file." << endl;
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
