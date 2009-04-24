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

static const char rcsid[] = "@(#)catalogif,LongType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/longtype.C,v 1.9 2003/12/27 23:23:04 rasdev Exp $";

#include "longtype.hh"
#include <iomanip>
#include <string.h>

LongType::LongType(const OId& id) throw (r_Error)
	:	IntegralType(id)
	{
	readFromDb();
	}

/*************************************************************
 * Method name...: LongType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 LongType.
 ************************************************************/

LongType::LongType()
	:	IntegralType(LongType::Name, 4)
	{
	myType = LONG;
	myOId = OId(LONG, OId::ATOMICTYPEOID);
	}

void
LongType::readFromDb() throw (r_Error)
	{
	setName(LongType::Name);
	size = 4;
	myType = LONG;
	myOId = OId(LONG, OId::ATOMICTYPEOID);
	}

/*************************************************************
 * Method name...: LongType(const LongType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

LongType::LongType(const LongType& old)
	:	IntegralType(old)
	{
	}

/*************************************************************
 * Method name...: operator=(const LongType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

LongType& LongType::operator=(const LongType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	AtomicType::operator=(old);
	return *this;
	}

/*************************************************************
 * Method name...: ~LongType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

LongType::~LongType()
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
 *                 Assumes that Long is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void 
LongType::printCell( ostream& stream, const char* cell ) const
{
  stream << std::setw(8) << *(r_Long*)cell;
}

r_Long*
LongType::convertToCLong(const char* cell, r_Long* value) const
{
  *value = *(r_Long*)cell;
  return value;
}


char* 
LongType::makeFromCLong(char* cell, const r_Long* value) const
{
  *(r_Long*)(cell) = *value;
  return cell;
}
