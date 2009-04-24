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

static const char rcsid[] = "@(#)catalogif,CharType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/chartype.C,v 1.10 2003/12/27 23:23:04 rasdev Exp $";

#include "chartype.hh"
#include <iomanip>
#include <string.h>
#include <limits.h>

/*************************************************************
 * Method name...: CharType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 CharType.
 ************************************************************/

CharType::CharType()
	:	UIntegralType(CharType::Name, 1)
	{
	myType = CHAR;
	myOId = OId(CHAR, OId::ATOMICTYPEOID);
	}

/*************************************************************
 * Method name...: CharType(const CharType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

CharType::CharType(const CharType& old)
	:	UIntegralType(CharType::Name, old.size)
	{
	}

CharType::CharType(const OId& id) throw (r_Error)
	:	UIntegralType(OId(CHAR, OId::ATOMICTYPEOID))
	{
	readFromDb();
	}

/*************************************************************
 * Method name...: operator=(const CharType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

CharType&
CharType::operator=(const CharType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	AtomicType::operator=(old);
	return *this;
	}

void
CharType::readFromDb() throw (r_Error)
	{
	setName(CharType::Name);
	myType = CHAR;
	myOId = OId(CHAR, OId::ATOMICTYPEOID);
	size = 1;
	}

/*************************************************************
 * Method name...: ~CharType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

CharType::~CharType()
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
 *                 Assumes that Char is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void 
CharType::printCell( ostream& stream, const char* cell ) const
{
  // !!!! HP specific, assumes 1 Byte char
  stream << std::setw(4) << (r_Long)(*(unsigned char*)cell);
}

r_ULong*
CharType::convertToCULong(const char* cell, r_ULong* value) const
{
  // !!!! HP specific, assumes 4 Byte long and MSB..LSB 
  // byte order
  *value = *(unsigned char*)cell;
  return value;
}


char* 
CharType::makeFromCULong(char* cell, const r_ULong* value) const
	{
	r_ULong myLong = *value;
	//restricting long to value range of short
	myLong = myLong > UCHAR_MAX ? UCHAR_MAX : myLong;
	myLong = myLong < 0 ? 0 : myLong;
	// !!!! HP specific, assumes 4 Byte long and MSB..LSB
	// byte order  
	*(unsigned char*)(cell)=(unsigned char)myLong;
	return cell;
	}
