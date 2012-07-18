/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/**
 * INCLUDE: storageman.cc
 *
 * MODULE: raslib
 *
 * CLASSES: r_Storage_Man, r_Storage_Man_C, r_Storage_Man_CPP
 *
 * COMMENTS:
 *
 * Transparent handling of malloc/free vs. new/delete
 *
*/

#include "mymalloc/mymalloc.h"

#include "raslib/storageman.hh"


// auxiliary static functions
static void *alloc_c_style( size_t size )
{
  return mymalloc( size );
}

static void free_c_style( void *data )
{
  free(data);
}

static void *alloc_cpp_style( size_t size )
{
  return new char[size];
}

static void free_cpp_style( void *data )
{
  delete [] data;
}



r_Storage_Man::r_Storage_Man( void )
{
  myalloc = alloc_c_style;
  myfree = free_c_style;
}

r_Storage_Man::r_Storage_Man( storage_man_alloc a, storage_man_free f )
{
  myalloc = a;
  myfree = f;
}

r_Storage_Man::r_Storage_Man( const r_Storage_Man &src )
{
  myalloc = src.myalloc;
  myfree = src.myfree;
}

r_Storage_Man::~r_Storage_Man( void )
{
}

void r_Storage_Man::set_storage_functions( storage_man_alloc a, storage_man_free f )
{
  myalloc = a;
  myfree = f;
}

r_Storage_Man &r_Storage_Man::operator=( const r_Storage_Man &src )
{
  myalloc = src.myalloc;
  myfree = src.myfree;
  return *this;
}

void *r_Storage_Man::storage_alloc( size_t size ) const throw(r_Error)
{
  void *result;

  if ((result = myalloc(size)) == NULL)
  {
    r_Error err(r_Error::r_Error_General);
    throw(err);
  }
  return result;
}

void r_Storage_Man::storage_free( void *data ) const
{
  myfree(data);
}



r_Storage_Man_C::r_Storage_Man_C( void ) : r_Storage_Man( alloc_c_style, free_c_style )
{
}

r_Storage_Man_C::~r_Storage_Man_C( void )
{
}


r_Storage_Man_CPP::r_Storage_Man_CPP( void ) : r_Storage_Man( alloc_cpp_style, free_cpp_style )
{
}

r_Storage_Man_CPP::~r_Storage_Man_CPP( void )
{
}
