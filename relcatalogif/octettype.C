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
/*************************************************************
 *
 *
 * PURPOSE:
 *   uses ODMG-conformant O2 classes
 *
 *
 * COMMENTS:
 *   none
 *
 ************************************************************/

static const char rcsid[] = "@(#)catalogif,OctetType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/octettype.C,v 1.9 2003/12/27 23:23:04 rasdev Exp $";

#include "octettype.hh"
#include <iomanip>
#include <string.h>
#include <limits.h>

OctetType::OctetType(const OId& id)
	:	IntegralType(id)
	{
	readFromDb();
	}

/*************************************************************
 * Method name...: OctetType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 OctetType.
 ************************************************************/

OctetType::OctetType()
	:	IntegralType(OctetType::Name, 1)
	{
	myType = OCTET;
	myOId = OId(OCTET, OId::ATOMICTYPEOID);
	}

/*************************************************************
 * Method name...: OctetType(const OctetType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

OctetType::OctetType(const OctetType& old)
	:	IntegralType(old)
	{
	}

/*************************************************************
 * Method name...: operator=(const OctetType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

OctetType& OctetType::operator=(const OctetType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	AtomicType::operator=(old);
	return *this;
	}

/*************************************************************
 * Method name...: ~OctetType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

OctetType::~OctetType()
{
}

/*************************************************************
 * Method name...: void printCell( ostream& stream, 
 *                                 const char* cell )
 *
 * Arguments.....:
 *   stream: stream to print on
 *   cell:   pointer to cell to print
 * Return value..: none
 * Description...: prints a cell cell in hex on stream 
 *                 followed by a space.
 *                 Assumes that Octet is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void 
OctetType::printCell( ostream& stream, const char* cell ) const
{
  // !!!! HP specific, assumes 1 Byte char
  stream << std::setw(4) << (r_Long)(*(char*)cell);
}

r_Long*
OctetType::convertToCLong(const char* cell, r_Long* value) const
{
  // !!!! HP specific, assumes 4 Byte long and MSB..LSB 
  // byte order
  *value = *(char*)cell;
  return value;
}


char* 
OctetType::makeFromCLong(char* cell, const r_Long* value) const
{
  r_Long myLong = *value;
  // restricting long to value range of short
  myLong = myLong > SCHAR_MAX ? SCHAR_MAX : myLong;
  myLong = myLong < SCHAR_MIN ? SCHAR_MIN : myLong;
  // !!!! HP specific, assumes 4 Byte long and MSB..LSB 
  // byte order
  *(char*)(cell) = (char)myLong;
  return cell;
}

void
OctetType::readFromDb() throw (r_Error)
	{
	setName(OctetType::Name);
	size = 1;
	myType = OCTET;
	myOId = OId(OCTET, OId::ATOMICTYPEOID);
	}

