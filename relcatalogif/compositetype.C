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

static const char rcsid[] = "@(#)catalogif,CompositeType: $Header: /home/rasdev/CVS-repository/rasdaman/relcatalogif/compositetype.C,v 1.6 2003/12/27 23:23:04 rasdev Exp $";

#include "compositetype.hh"
#include "reladminif/externs.h"

/*************************************************************
 * Method name...: unsigned int getSize() const
 *
 * Arguments.....: none
 * Return value..: size in d_Octets
 * Description...: returns size of CompositeType in chars
 ************************************************************/

unsigned int 
CompositeType::getSize() const
	{
	return size;
	}

/*************************************************************
 * Method name...: CompositeType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an 
 *                 CompositeType.
 ************************************************************/

CompositeType::CompositeType()
	:	BaseType("unnamed compositetype"),
		size(0)
	{
	}

CompositeType::CompositeType(unsigned int newSize)
	:	BaseType("unnamed compositetype"),
		size(newSize)
	{
	}

CompositeType::CompositeType(const char* name, unsigned int newSize)
	:	BaseType(name),
		size(newSize)
	{
	}

CompositeType::CompositeType(const CompositeType& old)
	:	BaseType(old),
		size(old.size)
	{
	}

CompositeType::CompositeType(const OId& id) throw (r_Error)
	:	BaseType(id)
	{
	}

CompositeType::~CompositeType()
	{
	}

CompositeType&
CompositeType::operator=(const CompositeType& old)
	{
	BaseType::operator=(old);
	size = old.size;
	return *this;
	}


