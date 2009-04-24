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

static const char rcsid[] = "@(#)catalogif,ULongType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/ulongtype.C,v 1.10 2003/12/27 23:23:04 rasdev Exp $";

#include "ulongtype.hh"
#include <iomanip>
#include <string.h>
#include "reladminif/externs.h"

ULongType::ULongType(const OId& id) throw (r_Error)
	:	UIntegralType(id)
	{
	readFromDb();
	}

/*************************************************************
 * Method name...: ULongType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 ULongType.
 ************************************************************/

ULongType::ULongType()
	:	UIntegralType(ULongType::Name, 4)
	{
	myOId = OId(ULONG, OId::ATOMICTYPEOID);
	myType = ULONG;
	}

/*************************************************************
 * Method name...: ULongType(const ULongType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

ULongType::ULongType(const ULongType& old)
	:	UIntegralType(old)
	{
	}

/*************************************************************
 * Method name...: operator=(const ULongType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

ULongType& ULongType::operator=(const ULongType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	AtomicType::operator=(old);
	return *this;
	}

/*************************************************************
 * Method name...: ~ULongType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

ULongType::~ULongType()
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
 *                 Assumes that ULong is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void 
ULongType::printCell( ostream& stream, const char* cell ) const
{
  stream << std::setw(8) << *(r_ULong*)cell;
}

r_ULong*
ULongType::convertToCULong(const char* cell, r_ULong* value) const
{
  *value = *(r_ULong*)cell;
  return value;
}


char* 
ULongType::makeFromCULong(char* cell, const r_ULong* value) const
{
  *(r_ULong*)(cell) = *value;
  return cell;
}

void
ULongType::readFromDb() throw (r_Error)
	{
	setName(ULongType::Name);
	size = 4;
	myOId = OId(ULONG, OId::ATOMICTYPEOID);
	myType = ULONG;
	}

