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
 * SOURCE: storagelayout.cc
 *
 * MODULE: rasodmg
 * CLASS:  r_StorageLayout, r_Domain_Storage_Layout
 *
 * COMMENTS:
 *   		None
 *
*/

// Because of NT port
#include <string.h>
#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif
#include <math.h>
#include <vector>

#include "rasodmg/storagelayout.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/tiling.hh"
#include "rasodmg/alignedtiling.hh"
#include "raslib/rmdebug.hh"
#include "raslib/rminit.hh"
#include "raslib/minterval.hh"
#include "rasodmg/gmarray.hh"

r_Storage_Layout::r_Storage_Layout(r_Data_Format init_format, const char* formatParams)
	:	til(0),
		storage_format(init_format),
		storage_params(0)
{
  til = new r_Size_Tiling();
  if (formatParams != NULL)
    storage_params = strdup(formatParams);
}

r_Storage_Layout::r_Storage_Layout(r_Tiling* ts, r_Data_Format init_format, const char* formatParams)
	:	til(ts),
		storage_format(init_format),
		storage_params(0)
{
  if (til == NULL)
    til = new r_Size_Tiling();
  if (formatParams != NULL)
    storage_params = strdup(formatParams);
}

r_Storage_Layout::r_Storage_Layout(const r_Storage_Layout& sl)
	:	til(sl.get_tiling()->clone()),
		storage_format(sl.get_storage_format()),
		storage_params(0)
{
  if (sl.storage_params != NULL)
    storage_params = strdup(sl.storage_params);
}

r_Storage_Layout*
r_Storage_Layout::clone() const
{
  r_Storage_Layout* newSL = new r_Storage_Layout(til->clone(), storage_format, storage_params);
  return newSL;
}

r_Storage_Layout::~r_Storage_Layout()
{
  if(til)
  {
    delete til;
    til = NULL;
  }
  if (storage_params)
  {
    free(storage_params);
    storage_params = NULL;
  }
}

const r_Tiling*
r_Storage_Layout::get_tiling() const
{
  return til;
}

r_Data_Format
r_Storage_Layout::get_storage_format() const
{
  return storage_format;
}

const char*
r_Storage_Layout::get_storage_format_params() const
{
  return storage_params;
}

r_Set< r_GMarray* >* 
r_Storage_Layout::decomposeMDD(const r_GMarray* mar) const throw (r_Error)
{
  r_Bytes cell_size = mar->get_type_length();
  std::vector<r_Minterval>* tiles=NULL;
  r_Set<r_GMarray*>* result=NULL;
  
  if (!til->is_compatible(mar->spatial_domain(), cell_size))
    {
    RMInit::logOut << "r_Storage_Layout::decomposeMDD() gmarray is not compatible with tiling" << endl;
    RMInit::logOut << "\tgmarray domain   : " << mar->spatial_domain() << endl;
    RMInit::logOut << "\tgmarray type size: " << mar->get_type_length() << endl;
    RMInit::logOut << "\tstorage layout   : " << *this << endl;
    throw r_Error(STORAGERLAYOUTINCOMPATIBLEWITHGMARRAY);
    }



  try
  {
   tiles = til->compute_tiles(mar->spatial_domain(), cell_size);
  }
  catch(r_Error& err)
  {
   throw;
  }
  
  result = new r_Set<r_GMarray*>;
  
  for (std::vector<r_Minterval>::iterator it = tiles->begin(); it != tiles->end(); it++)
    result->insert_element(mar->intersect(*it));

  delete tiles;
  return result;
}

void 
r_Storage_Layout::print_status(std::ostream& os) const
{
  os << "r_Storage_Layout[ tiling = "<< *til << " storage format = " << storage_format << " storage parameters = ";
  if (storage_params != NULL)
    os << "upps, not here";
    //os << storage_params;
  else
    os << "none defined";
  os << " ]";
}
  
bool
r_Storage_Layout::is_compatible(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const
{
  return til->is_compatible(obj_domain, cellTypeSize);
}
     
std::ostream& 
operator<<(std::ostream& s, const r_Storage_Layout& sl)
{
  sl.print_status(s);
  return s;
}

