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

static const char rcsid[] = "@(#)raslib, r_Marray_Type: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/marraytype.cc,v 1.7 2003/12/27 23:01:21 rasdev Exp $";

#include "raslib/marraytype.hh"
#include "raslib/basetype.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"

r_Marray_Type::r_Marray_Type()
	:	r_Type(),
		baseType(NULL)
	{
	}

r_Marray_Type::r_Marray_Type(const r_Base_Type& newBaseType)
	:	r_Type(),
		baseType((r_Base_Type*)newBaseType.clone())
	{
	}

r_Marray_Type::r_Marray_Type(const r_Marray_Type& oldObj) throw (r_Error)
	:	r_Type(oldObj)
	{ 
	if (oldObj.baseType) 
		baseType =  (r_Base_Type*)oldObj.baseType->clone();           
	else	{
		RMInit::logOut << "r_Marray_Type::r_Marray_Type( oldObj ) the element type is NULL." << endl;
		throw r_Error(MARRAYTYPEHASNOELEMENTTYPE);
		}
	}

const r_Marray_Type& 
r_Marray_Type::operator=(const r_Marray_Type& oldObj) throw (r_Error)
	{
	// Gracefully handle self assignment
	if (this == &oldObj)
		return *this;

	r_Type::operator=(oldObj);
	if (baseType) 
		{
		 delete baseType;
		 baseType = 0;
		}

	if (oldObj.baseType)
		 baseType = (r_Base_Type*)oldObj.baseType->clone();
	else	{
		RMInit::logOut << "r_Marray_Type::operator=( oldObj ) the element type is NULL." << endl;
		throw r_Error(MARRAYTYPEHASNOELEMENTTYPE);
		}

	return *this;
	}

bool
r_Marray_Type::isMarrayType() const
	{
	return true;
	}

const r_Base_Type&
r_Marray_Type::base_type() const
	{
	return *baseType;
	}

r_Type*
r_Marray_Type::clone() const
	{
	return new r_Marray_Type(*this); 
	}

r_Type::r_Type_Id 
r_Marray_Type::type_id() const
	{
	return MARRAYTYPE;
	}

void
r_Marray_Type::convertToLittleEndian(char* cells, r_Area noCells) const
	{
	}

void
r_Marray_Type::convertToBigEndian(char* cells, r_Area noCells) const
	{
	}

void 
r_Marray_Type::print_status(std::ostream& s) const
	{
	s << "marray< "; 
	baseType->print_status(s);
	s << " >";
	} 

r_Marray_Type::~r_Marray_Type()
	{
	if (baseType) 
		 delete baseType;
	}

std::ostream &operator<<( std::ostream &str, const r_Marray_Type &type )
{
  type.print_status(str);
  return str;
}
    