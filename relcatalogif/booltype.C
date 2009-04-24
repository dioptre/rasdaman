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

static const char rcsid[] = "@(#)catalogif,BoolType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/booltype.C,v 1.8 2003/12/27 23:23:04 rasdev Exp $";

#include "booltype.hh"
#include <string.h>

/*************************************************************
 * Method name...: BoolType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 BoolType.
 ************************************************************/

BoolType::BoolType(const OId& id) throw (r_Error)
	:	UIntegralType(id)
	{
	readFromDb();
	}

BoolType::BoolType()
	:	UIntegralType(BoolType::Name, 1)
	{
	myType = BOOLTYPE;
	myOId = OId(BOOLTYPE, OId::ATOMICTYPEOID);
	}

/*************************************************************
 * Method name...: BoolType(const BoolType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

BoolType::BoolType(const BoolType& old)
	:	UIntegralType(old)
	{
	}

/*************************************************************
 * Method name...: operator=(const BoolType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

BoolType&
BoolType::operator=(const BoolType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	AtomicType::operator=(old);
	return *this;
	}

/*************************************************************
 * Method name...: ~BoolType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

BoolType::~BoolType()
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
 *                 Assumes that Bool is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void 
BoolType::printCell( ostream& stream, const char* cell ) const
{
  if(*cell == 0)
    stream << "FALSE ";
  else
    stream << "TRUE  ";
}

r_ULong*
BoolType::convertToCULong(const char* cell, r_ULong* value) const
{
  // !!!! HP specific, assumes 4 Byte long and MSB..LSB 
  // byte order
  //*value = *(unsigned char*)cell;

  *value = *(unsigned char*)cell ? 1: 0;
  return value;
}


char* 
BoolType::makeFromCULong(char* cell, const r_ULong* value) const
{
  // !!!! HP specific, assumes 4 Byte long and MSB..LSB 
  // byte order
  *(unsigned char*)(cell) = (unsigned char)(*value);
  return cell;
}

void
BoolType::readFromDb() throw (r_Error)
	{
	setName(BoolType::Name);
	size = 1;
	myType = BOOLTYPE;
	myOId = OId(BOOLTYPE, OId::ATOMICTYPEOID);
	}

