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
 * SOURCE:   scalar.cc
 *
 * MODULE:   raslib
 * CLASS:    r_Scalar
 *
 * COMMENTS:
 *
*/

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif
#include <string.h>
#include <fstream>
#include <stdlib.h>

#include "raslib/rminit.hh"
#include "raslib/scalar.hh"
#include "raslib/basetype.hh"
#include "raslib/error.hh"




r_Scalar::r_Scalar( const r_Base_Type* newType )
  : valueType(NULL)
{
  if( newType )
    valueType = (r_Base_Type*)newType->clone();
  else	{
	RMInit::logOut << "r_Scalar::r_Scalar(NULL) base type must be not NULL" << endl;
	throw r_Error(SCALARWASPASSEDNULLTYPE);
	}
}



r_Scalar::r_Scalar( const r_Scalar& obj )
  : valueType(obj.valueType)
{
}



r_Scalar::~r_Scalar() 
{
  delete valueType;
}



const r_Scalar&
r_Scalar::operator=( const r_Scalar& obj )
{
  if( this != &obj )
  {
      delete valueType;
      valueType = (r_Base_Type*)obj.valueType->clone();
  }

  return *this;
}

bool
r_Scalar::isStructure() const
	{
	return false;
	}

bool
r_Scalar::isComplex() const
	{
	return false;
	}

bool
r_Scalar::isPrimitive() const
	{
	return false;
	}

const r_Base_Type* 
r_Scalar::get_type() const
{
  return valueType;
}


std::ostream& operator<<( std::ostream& s, const r_Scalar& obj )
{
  obj.print_status( s );
  return s;
}


