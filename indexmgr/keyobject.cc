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

#include "keyobject.hh"
#include "tilemgr/tile.hh"

ostream& operator<<(ostream& in,  const KeyObject& d)
	{
	if (d.isPersCarrier())
		{
		in << "Carrier{" << d.getDomain() << ", " << d.getObject().getOId() << "}";
		}
	else	{
		in << "Carrier{" << d.getDomain() << ", TransTile}";
		}
	return in;
	}

KeyObject::KeyObject()
	:	transobject(NULL)
	{
	}

KeyObject::KeyObject(const KeyObject& old)
	:	persobject(old.persobject),
		domain(old.domain),
		transobject(old.transobject)
	{
	}

KeyObject::KeyObject(const Tile* tile)
	:	persobject(),
		domain(tile->getDomain()),
		transobject(NULL)
	{
	if (tile->isPersistent())
		{
		persobject = (const DBObjectId&)((Tile*)tile)->getDBTile();
		}
	else	{
		transobject = (Tile*)tile;
		}
	}

KeyObject::KeyObject(const DBObjectId& obj, const r_Minterval& dom)
	:	persobject(obj),
		domain(dom),
		transobject(NULL)
	{
	}

KeyObject::~KeyObject()
	{
	transobject = NULL;
	}

void
KeyObject::setDomain(const r_Minterval& dom)
	{
	domain = dom;
	}

void
KeyObject::setTransObject(const Tile* tile)
	{
	domain = tile->getDomain();
	transobject = (Tile*)tile;
	}

void
KeyObject::setObject(const DBObjectId& obj)
	{
	persobject = obj;
	}

bool
KeyObject::isInitialised() const	
	{
	if (transobject)
		return true;
	if (persobject.isInitialised())
		return true;
	return false;
	}

bool
KeyObject::isPersCarrier() const
	{
	return (transobject == NULL);
	}

Tile*
KeyObject::getTransObject() const
	{
	return transobject;
	}

const DBObjectId&
KeyObject::getObject() const
	{
	return persobject;
	}

r_Minterval
KeyObject::getDomain() const
	{
	return domain;
	}
/*
const r_Minterval&
KeyObject::getDomain() const
	{
	return domain;
A	}
*/
