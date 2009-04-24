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
 * INCLUDE: flatbasetype.cc
 *
 * MODULE:  raslib
 * CLASS:   r_Flat_Base_Type
 *
 * COMMENTS:
 *
*/

#include <iostream>
#include <stdio.h>

#include "raslib/primitivetype.hh"
#include "raslib/structuretype.hh"
#include "raslib/flatbasetype.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"


r_Flat_Base_Type::r_Flat_Base_Type( void )
{
  init_shared();
}


r_Flat_Base_Type::r_Flat_Base_Type( const r_Base_Type *type )
{
  init_shared();
  process_type(type);
}


r_Flat_Base_Type::r_Flat_Base_Type( const r_Flat_Base_Type &src )
{
  init_shared();
  copy_flat_type(src);
}


r_Flat_Base_Type::~r_Flat_Base_Type( void )
{
  free_type_data();
}


unsigned int r_Flat_Base_Type::get_num_types( void ) const
{
  return numPrimTypes;
}


const r_Primitive_Type *r_Flat_Base_Type::type( unsigned int num ) const throw (r_Eindex_violation)
{
  if (num < numPrimTypes)
	{
	return primTypes[num];
	}
  else	{
	RMInit::logOut << "r_Flat_Base_Type::type(" << num << ") index out of bounds (" << numPrimTypes - 1 << ")" << endl;
	throw r_Eindex_violation(0, numPrimTypes - 1, num);
	}
}


const r_Primitive_Type *r_Flat_Base_Type::operator[]( unsigned int num ) const throw (r_Eindex_violation)
{
  if (num < numPrimTypes)
	{
	return primTypes[num];
	}
  else	{
	RMInit::logOut << "r_Flat_Base_Type::operator[](" << num << ") index out of bounds (" << numPrimTypes - 1 << ")" << endl;
	throw r_Eindex_violation(0, numPrimTypes - 1, num);
	}
}


unsigned int r_Flat_Base_Type::offset( unsigned int num ) const throw (r_Eindex_violation)
{
  if (num < numPrimTypes)
	{
	return offsets[num];
	}
  else	{
	RMInit::logOut << "r_Flat_Base_Type::offset(" << num << ") index out of bounds (" << numPrimTypes - 1 << ")" << endl;
	throw r_Eindex_violation(0, numPrimTypes - 1, num);
	}
}


r_Bytes r_Flat_Base_Type::size( void ) const
{
  return typeSize;
}


r_Flat_Base_Type &r_Flat_Base_Type::operator=( const r_Flat_Base_Type &src )
{
  free_type_data();
  copy_flat_type(src);
  return (*this);
}


r_Flat_Base_Type &r_Flat_Base_Type::operator=( const r_Base_Type *type )
{
  free_type_data();
  process_type(type);
  return (*this);
}


bool r_Flat_Base_Type::operator==( const r_Flat_Base_Type &src ) const
{
  if (numPrimTypes == src.numPrimTypes)
  {
    unsigned int i;
    for (i=0; i<numPrimTypes; i++)
    {
      if (primTypes[i]->type_id() != src.primTypes[i]->type_id())
	break;
    }
    if (i >= numPrimTypes)
      return true;
  }
  return false;
}


void r_Flat_Base_Type::init_shared( void )
{
  typeSize = 0;
  numPrimTypes = 0;
  primTypes = NULL;
  offsets = NULL;
}


void r_Flat_Base_Type::free_type_data( void )
{
  if (primTypes != NULL)
  {
    for (unsigned int i=0; i<numPrimTypes; i++)
      delete primTypes[i];
    delete [] primTypes;
    primTypes = NULL;
  }

  if (offsets != NULL)
  {
    delete [] offsets;
    offsets = NULL;
  }
  numPrimTypes = 0;
}


void r_Flat_Base_Type::process_type( const r_Base_Type *type )
{
  typeSize = type->size();

  if (type->isStructType())
  {
    const r_Structure_Type *stype = (const r_Structure_Type*)type;
    numPrimTypes = parse_structure_type(stype, 0, 0);
    primTypes = new r_Primitive_Type*[numPrimTypes];
    offsets = new unsigned int[numPrimTypes];
    parse_structure_type(stype, 0, 0);
  }
  else
  {
    numPrimTypes = 1;
    primTypes = new r_Primitive_Type*[1];
    offsets = new unsigned int[1];
    parse_primitive_type((r_Primitive_Type*)(type->clone()), 0, 0);
  }
}


void r_Flat_Base_Type::copy_flat_type( const r_Flat_Base_Type &src )
{
  typeSize = src.typeSize;

  if (src.numPrimTypes == 0)
  {
    numPrimTypes = 0;
    primTypes = NULL;
    offsets = NULL;
  }
  else
  {
    numPrimTypes = src.numPrimTypes;
    primTypes = new r_Primitive_Type*[numPrimTypes];
    offsets = new unsigned int[numPrimTypes];
    for (unsigned int i=0; i<numPrimTypes; i++)
    {
      primTypes[i] = (r_Primitive_Type*)(src.primTypes[i]->clone());
      offsets[i] = src.offsets[i];
    }
  } 
}


void r_Flat_Base_Type::parse_primitive_type( r_Primitive_Type *type, unsigned int num, unsigned int off )
{
  if (primTypes == NULL)
  {
    delete type;
  }
  else
  {
    primTypes[num] = type;
    offsets[num] = off;
    //cout << "TYPE "; type->print_status(); cout << ", NUM " << num << ", OFF " << off << endl;
  }
}


unsigned int r_Flat_Base_Type::parse_structure_type( const r_Structure_Type *type, unsigned int num, unsigned int off )
{
  r_Structure_Type::attribute_iterator iter(type->defines_attribute_begin());
  unsigned int numPrim = 0;

  while (iter != type->defines_attribute_end())
  {
    r_Type *newType = (*iter).type_of().clone();
    if (newType->isStructType())
    {
      numPrim += parse_structure_type((const r_Structure_Type*)newType, num + numPrim, off + (*iter).offset());
      delete newType;
    }
    else
    {
      parse_primitive_type((r_Primitive_Type*)newType, num + numPrim, off + (*iter).offset());
      numPrim++;
    }
    iter++;
  }

  return numPrim;
}


void r_Flat_Base_Type::print_status( std::ostream &str ) const
{
  if (numPrimTypes == 0)
  {
    str << "<nn>";
  }
  else
  {
    str << typeSize << ':';
    if (numPrimTypes == 1)
    {
      primTypes[0]->print_status(str);
    }
    else
    {
      unsigned int i;
      str << '{'; primTypes[0]->print_status(str); str << '(' << offsets[0] << ')';
      for (i=1; i<numPrimTypes; i++)
      {
	str << ", ";
	primTypes[i]->print_status(str);
	str << '(' << offsets[i] << ')';
      }
      str << '}';
    }
  }
}



std::ostream &operator<<( std::ostream &str, const r_Flat_Base_Type &type )
{
  type.print_status(str);
  return str;
}
