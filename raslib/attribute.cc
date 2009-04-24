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

static const char rcsid[] = "@(#) raslib, r_Attribute: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/attribute.cc,v 1.11 2003/12/27 23:01:21 rasdev Exp $";

#include "raslib/attribute.hh"
#include "raslib/basetype.hh" 
#include "raslib/structuretype.hh" 
#include "raslib/rminit.hh"
#include "raslib/error.hh"

r_Attribute::r_Attribute() 
	:	r_Property(),
		localOffset(0),
		globalOffset(0)
	{
	}

r_Attribute::r_Attribute(const char* newTypeName, const r_Base_Type& newType) 
	:	r_Property(newTypeName, newType),
		localOffset(0),
		globalOffset(0) 
	{
	}

r_Attribute::r_Attribute(const r_Attribute& oldObj) 
	:	r_Property(oldObj) ,
		localOffset(oldObj.localOffset),
		globalOffset(oldObj.globalOffset) 
	{
	}

const r_Attribute& 
r_Attribute::operator=(const r_Attribute& oldObj) 
	{
	// Gracefully handle self assignment
	if (this != &oldObj)
		{
		r_Property::operator=(oldObj) ;
		localOffset = oldObj.localOffset;
		globalOffset = oldObj.globalOffset;
		}

	return *this;
	}

r_Attribute::~r_Attribute() 
	{
	}

r_Bytes
r_Attribute::offset() const
	{
	return localOffset;
	}

void
r_Attribute::set_offset(r_Bytes newOffset) 
	{
	localOffset = newOffset;
	}

r_Bytes
r_Attribute::global_offset() const
	{
	return globalOffset;
	}

void
r_Attribute::set_global_offset(r_Bytes newOffset) 
	{
	globalOffset = newOffset;
	}

void 
r_Attribute::print_status(std::ostream& s) const
	{
	type_of().print_status(s);
	s << " " << name() << std::flush;
	}



r_Attribute 
r_Attribute::operator[](unsigned int number) const throw(r_Error) 
	{
	if (type_of().type_id() != r_Type::STRUCTURETYPE) 
		{
		RMInit::logOut << "r_Attribute::operator[](" << number << ") not a struct type" << endl;
		throw r_Error(r_Error::r_Error_TypeInvalid) ;
		}
		
	const r_Structure_Type& structValue = (const r_Structure_Type&)type_of();

	return structValue[number];
	}



r_Boolean 
r_Attribute::get_boolean(const char* cell) const throw(r_Error) 
	{
	if (type_of().type_id() != r_Type::BOOL) 
		{
		RMInit::logOut << "r_Attribute::get_boolean(data) not a boolean" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_Boolean*) (cell+globalOffset) ) ;
	}



r_Char	 
r_Attribute::get_char(const char* cell) const throw(r_Error) 
	{
	if (type_of() .type_id() != r_Type::CHAR) 
		{
		RMInit::logOut << "r_Attribute::get_char(data) not a char" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_Char*) (cell+globalOffset) ) ;
	}



r_Octet	 
r_Attribute::get_octet(const char* cell) const throw(r_Error) 
	{
	if (type_of() .type_id() != r_Type::OCTET) 
		{
		RMInit::logOut << "r_Attribute::get_octet(data) not a octet" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_Octet*) (cell+globalOffset) ) ;
	}



r_Short	 
r_Attribute::get_short(const char* cell) const throw(r_Error) 
	{
	if (type_of() .type_id() != r_Type::SHORT) 
		{
		RMInit::logOut << "r_Attribute::get_short(data) not a short" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_Short*) (cell+globalOffset) ) ;
	}



r_UShort	
r_Attribute::get_ushort(const char* cell) const throw(r_Error) 
	{
	if (type_of() .type_id() != r_Type::USHORT) 
		{
		RMInit::logOut << "r_Attribute::get_ushort(data) not a ushort" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_UShort*) (cell+globalOffset) ) ;
	}



r_Long		
r_Attribute::get_long(const char* cell) const throw(r_Error) 
	{
	if (type_of() .type_id() != r_Type::LONG) 
		{
		RMInit::logOut << "r_Attribute::get_long(data) not a long" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_Long*) (cell+globalOffset) ) ;
	}



r_ULong	 
r_Attribute::get_ulong(const char* cell) const throw(r_Error) 
	{
	if (type_of() .type_id() != r_Type::ULONG) 
		{
		RMInit::logOut << "r_Attribute::get_ulong(data) not a ulong" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_ULong*) (cell+globalOffset) ) ;
	}



r_Float	 
r_Attribute::get_float(const char* cell) const throw(r_Error) 
	{
	if (type_of() .type_id() != r_Type::FLOAT) 
		{
		RMInit::logOut << "r_Attribute::get_float(data) not a float" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_Float*) (cell+globalOffset) ) ;
	}



r_Double	
r_Attribute::get_double(const char* cell) const throw(r_Error) 
	{
	if (type_of() .type_id() != r_Type::DOUBLE) 
		{
		RMInit::logOut << "r_Attribute::get_double(data) not a double" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid) ;
		throw(err) ;		 
		}

	return *((r_Double*) (cell+globalOffset) ) ;
	}

std::ostream &operator<<( std::ostream &str, const r_Attribute &type )
{
  type.print_status(str);
  return str;
}
