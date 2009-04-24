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

static const char rcsid[] = "@(#)catalogif,UShortType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/ushorttype.C,v 1.9 2003/12/27 23:23:04 rasdev Exp $";

#include "ushorttype.hh"
#include <iomanip>
#include <string.h>

/*************************************************************
 * Method name...: UShortType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 UShortType.
 ************************************************************/

UShortType::UShortType()
	:	UIntegralType(UShortType::Name, 2)
	{
	myType = USHORT;
	myOId = OId(USHORT, OId::ATOMICTYPEOID);
	}

/*************************************************************
 * Method name...: UShortType(const UShortType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

UShortType::UShortType(const UShortType& old)
	:	UIntegralType(old)
	{
	}

UShortType::UShortType(const OId& id) throw (r_Error)
	:	UIntegralType(id)
	{
	readFromDb();
	}

/*************************************************************
 * Method name...: operator=(const UShortType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

UShortType& UShortType::operator=(const UShortType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	AtomicType::operator=(old);
	return *this;
	}

/*************************************************************
 * Method name...: ~UShortType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

UShortType::~UShortType()
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
 *                 Assumes that UShort is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void 
UShortType::printCell( ostream& stream, const char* cell ) const
{
  // !!!! HP specific, assumes 4 Byte long and MSB..LSB 
  // byte order
  stream << std::setw(5) << *(unsigned short*)cell;
}

r_ULong*
UShortType::convertToCULong(const char* cell, r_ULong* value) const
{
  // !!!! HP specific, assumes 2 Byte short
  *value = *(unsigned short*)cell;
  return value;
}


char* 
UShortType::makeFromCULong(char* cell, const r_ULong* value) const
{
  r_ULong myLong = *value;
  // restricting long to value range of short
  myLong = myLong > USHRT_MAX ? USHRT_MAX : myLong;
  myLong = myLong < 0 ? 0 : myLong;
  // !!!! HP specific, assumes 2 Byte short
  *(unsigned short*)(cell) = (unsigned short)myLong;
  return cell;
}

void
UShortType::readFromDb() throw (r_Error)
	{
	size = 2;
	setName(UShortType::Name);
	myType = USHORT;
	myOId = OId(USHORT, OId::ATOMICTYPEOID);
	}
