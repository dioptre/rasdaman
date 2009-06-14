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

#ifndef _KEYOBJECT_HH_
#define _KEYOBJECT_HH_

class Tile;
class KeyObject;

#include "reladminif/dbobject.hh"
#include "raslib/minterval.hh"
#include "reladminif/dbref.hh"

/**
 *	@file keyobject.hh
 *
 *	@ingroup indexmgr
 *
 * @Doc:
 * This class is a carrier structure which carries objects and their domains
 * through the different levels of the index.
*/
class KeyObject
	{
	public:
		KeyObject();

		KeyObject(const KeyObject& old);
		/*@Doc:
			Copy constructor.  Copies the tile pointer.
		*/
	
		KeyObject(const Tile* p);
		/*@Doc:
			Construccts a new KeyObject.  The type (persistent/transinet)
			is deremined based on the Tile.  The domain also.
			This constructor is sometimes misused by the compiler in statements like KeyObject p = 0.
			This will cause a crash because the domain is read from the tile which is NULL : )
		*/

		KeyObject(const DBObjectId& obj, const r_Minterval& dom);
		/*@Doc:
			Constructs a new KeyObject for a persistent object.
		*/

		const DBObjectId& getObject() const;
		/*@Doc:
			Returns a smartpointer to the persistent object attribute.
			If this is a transient object carrier the returned smart-
			pointer is invalid.
		*/

		Tile* getTransObject() const;
		/*@Doc:
			Returns the transient object.  If this KeyObject carries
			a persistent object a NULL is returned.
		*/

		r_Minterval getDomain() const;
		/*@Doc:
			Returns the domain which is associated with the objects that
			is carried.
		*/
	
		~KeyObject();
		/*@Doc:
			Does not delete the TransTile!!
		*/

		bool isInitialised() const;
		/*@Doc:
			Returns true if trans object is initialised or the dbref is initialised.
		*/

		bool isPersCarrier() const;
		/*@Doc:
			Returns true if the transobject attribute is NULL.
		*/

		void setDomain(const r_Minterval& dom);
		/*@Doc:
			Alters the domain the KeyObject carries.
		*/

		void setTransObject(const Tile* tile);
		/*@Doc:
			makes the KeyObject a transient carrier and copies the pointer.
		*/

		void setObject(const DBObjectId& obj);
		/*@Doc:
			makes the KeyObject a persistent carrier and copies object.
		*/

	protected:
		DBObjectId persobject;
		/*@Doc:
			A smartpointer to the carried object if it is persistent.
		*/

		r_Minterval domain;
		/*@Doc:
			The domain which the carried object belongs to.
		*/

		Tile* transobject;
		/*@Doc:
			Attribute for storing a transtile.  is NULL if a persistent
			object is carried.
		*/
	};

/*@Doc:
	Prints the status of KeyObject object.
*/
extern std::ostream& operator<<(std::ostream& in,  const KeyObject& d);

#endif
