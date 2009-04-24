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

static const char rcsid[] = "@(#)catalogif,AtomicType: $Id: atomictype.C,v 1.4 2001/06/20 08:06:37 hoefner Exp $";

#include "atomictype.hh"
#include "reladminif/externs.h"

/*************************************************************
 * Method name...: AtomicType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 AtomicType.
 ************************************************************/

AtomicType::AtomicType(unsigned int newSize)
	:	BaseType("unnamed atomictype"),
		size(newSize)
	{
	objecttype = OId::ATOMICTYPEOID;
	_isPersistent = 1;
	_isInDatabase = 1;
	_isModified = 0;
	}

AtomicType::AtomicType(const char* name, unsigned int newSize)
	:	BaseType(name),
		size(newSize)
	{
	objecttype = OId::ATOMICTYPEOID;
	_isPersistent = 1;
	_isInDatabase = 1;
	_isModified = 0;
	}

AtomicType::AtomicType(const AtomicType& old)
	:	BaseType(old),
		size(old.size)
	{
	objecttype = OId::ATOMICTYPEOID;
	_isPersistent = 1;
	_isInDatabase = 1;
	_isModified = 0;
	}

AtomicType::AtomicType(const OId& id) throw (r_Error)
	:	BaseType(id)
	{
	objecttype = OId::ATOMICTYPEOID;
	_isPersistent = 1;
	_isInDatabase = 1;
	_isModified = 0;
	}

AtomicType::~AtomicType()
	{
	}

AtomicType&
AtomicType::operator=(const AtomicType& old)
	{
	BaseType::operator=(old);
	size = old.size;
	return *this;
	}

/*************************************************************
 * Method name...: unsigned int getSize() const
 *
 * Arguments.....: none
 * Return value..: size in d_Octets
 * Description...: returns size of AtomicType in chars
 ************************************************************/

unsigned int 
AtomicType::getSize() const
{
  return size;
}

