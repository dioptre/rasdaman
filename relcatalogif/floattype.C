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

static const char rcsid[] = "@(#)catalogif,FloatType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/floattype.C,v 1.10 2003/12/27 23:23:04 rasdev Exp $";

#include "floattype.hh"
#include <iomanip>
#include <string.h>
#include "reladminif/oidif.hh"
#include <values.h>

FloatType::FloatType(const OId& id) throw (r_Error)
	:	RealType(id)
	{
	readFromDb();
	}

/*************************************************************
 * Method name...: FloatType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 FloatType.
 ************************************************************/

FloatType::FloatType()
	:	RealType(FloatType::Name, 4)
	{
	myType = FLOAT;
	myOId = OId(FLOAT, OId::ATOMICTYPEOID);
	}

/*************************************************************
 * Method name...: FloatType(const FloatType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

FloatType::FloatType(const FloatType& old)
	:	RealType(old)
	{
	}

/*************************************************************
 * Method name...: operator=(const FloatType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

FloatType& FloatType::operator=(const FloatType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	AtomicType::operator=(old);
	return *this;
	}

/*************************************************************
 * Method name...: ~FloatType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

FloatType::~FloatType()
{
}

void
FloatType::readFromDb() throw (r_Error)
	{
	setName(FloatType::Name);
	size = 4;
	myType = FLOAT;
	myOId = OId(FLOAT, OId::ATOMICTYPEOID);
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
 *                 Assumes that Float is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void 
FloatType::printCell( ostream& stream, const char* cell ) const
{
  // !!!! HP specific, assumes 4 Byte float and MSB..LSB 
  // byte order
  stream << std::setw(8) << *(float*)cell;
}

double*
FloatType::convertToCDouble(const char* cell, double* value) const
{
  // !!!! HP specific, assumes 4 Byte float and MSB..LSB 
  // byte order
  *value = *(float*)cell;
  return value;
}


char* 
FloatType::makeFromCDouble(char* cell, const double* value) const
{
  // make sure that a float is not assigned a double (DEC Alpha correctly dumps core)
  double dummy = *value;
  if(dummy > FLT_MAX)
    dummy = FLT_MAX;
  if(dummy < -1.0f * FLT_MAX)
    dummy = -1.0f * FLT_MAX;
  *(float*)(cell) = dummy;
  return cell;
}
