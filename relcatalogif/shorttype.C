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

static const char rcsid[] = "@(#)catalogif,ShortType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/shorttype.C,v 1.11 2003/12/27 23:23:04 rasdev Exp $";

#include "shorttype.hh"
#include <iomanip>
#include <limits.h>
#include <string.h>

ShortType::ShortType(const OId& id) throw (r_Error)
	:IntegralType(id)
	{
	readFromDb();
	}

/*************************************************************
 * Method name...: ShortType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 ShortType.
 ************************************************************/

ShortType::ShortType()
	:	IntegralType(ShortType::Name, 2)
	{
	myType = SHORT;
	myOId = OId(SHORT, OId::ATOMICTYPEOID);
	}

/*************************************************************
 * Method name...: ShortType(const ShortType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

ShortType::ShortType(const ShortType& old)
	:	IntegralType(old)
	{
	}

/*************************************************************
 * Method name...: operator=(const ShortType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

ShortType& ShortType::operator=(const ShortType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	AtomicType::operator=(old);
	return *this;
	}

/*************************************************************
 * Method name...: ~ShortType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

ShortType::~ShortType()
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
 *                 Assumes that Short is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void 
ShortType::printCell( ostream& stream, const char* cell ) const
{
  stream << std::setw(5) << *(short*)cell;
}

r_Long*
ShortType::convertToCLong(const char* cell, r_Long* value) const
{
  *value = *(short*)cell;
  return value;
}


char* 
ShortType::makeFromCLong(char* cell, const r_Long* value) const
{
  r_Long myLong = *value;
  // restricting long to value range of short
  myLong = myLong > SHRT_MAX ? SHRT_MAX : myLong;
  myLong = myLong < SHRT_MIN ? SHRT_MIN : myLong;
  // !!!! HP specific, assumes 2 Byte short
  *(short*)(cell) = (short)myLong;
  return cell;
}

void
ShortType::readFromDb() throw (r_Error)
	{
	setName(ShortType::Name);
	myType = SHORT;
	size = 2;
	myOId = OId(SHORT, OId::ATOMICTYPEOID);
	}
