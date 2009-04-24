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
#include <iostream>
#include <set>
#include "dbmddset.hh"
#include "raslib/rmdebug.hh"
#include "reladminif/sqlerror.hh"
#include "reladminif/externs.h"
#include "dbmddobj.hh"
#include "reladminif/objectbroker.hh"
#include "reladminif/dbref.hh"
#include "relcatalogif/collectiontype.hh"
#include "reladminif/dbobjectiditerator.hh"
#include "raslib/error.hh"

void
DBMDDSet::printStatus(unsigned int level, std::ostream& stream) const
	{
	char* indent = new char[level*2 +1];
	for (int j = 0; j < level*2 ; j++)
		indent[j] = ' ';
	indent[level*2] = '\0';
	DBObject::printStatus(level, stream);
	stream << indent;
	stream << "Collection Entries ";
	for (DBMDDObjIdSet::const_iterator i = mySet.begin(); i != mySet.end(); i++)
		{
		if (isPersistent())
			stream << (*i).getOId() << " ";
		else
 			stream << (r_Ptr)(*i).ptr() << " ";
		}
	stream << endl;
	delete[] indent;
	indent=0;
	}

bool
DBMDDSet::contains_element(const DBMDDObjId& elem) const
	{
	RMDBGENTER(4, RMDebug::module_mddif, "DBMDDSet", "contains_element(" << elem.getOId() << ") " << myOId);
	bool retval = false;
	DBMDDObjIdSet::const_iterator i = mySet.find(elem);
	if (i != mySet.end())
		{
		retval = true;
		}
	RMDBGEXIT(4, RMDebug::module_mddif, "DBMDDSet", "contains_element(" << elem.getOId() << ") " << myOId << " " << retval);
	return retval;
	}

void
DBMDDSet::remove(DBMDDObjId& elem)
	{
	RMDBGENTER(4, RMDebug::module_mddif, "DBMDDSet", "remove(" << elem.getOId() << ")");
	DBMDDObjIdSet::iterator i = mySet.find(elem);
	if (i != mySet.end())
		{
		elem->decrementPersRefCount();
		mySet.erase(i);
		setModified();
		}
	RMDBGEXIT(4, RMDebug::module_mddif, "DBMDDSet", "remove(" << elem.getOId() << ")");
	}

void
DBMDDSet::removeAll()
	{
	DBMDDObjId t;
	while (!mySet.empty())
		{
		((DBMDDObj*)(*(mySet.begin())).ptr())->decrementPersRefCount();
		mySet.erase(mySet.begin());
		}
	setModified();
	}

DBMDDSet::~DBMDDSet() 
	{
	RMDBGENTER(4, RMDebug::module_mddif, "DBMDDSet", "~DBMDDSet() " << myOId);
	validate();
	collType = NULL;
	mySet.clear();
	RMDBGEXIT(4, RMDebug::module_mddif, "DBMDDSet", "~DBMDDSet() " << myOId);
	}

const CollectionType*
DBMDDSet::getCollType() const
	{ 
	RMDBGONCE(4, RMDebug::module_mddif, "DBMDDSet", "getCollType() " << myOId << "\t" << collType->getName());
	return collType;
	}


r_Bytes
DBMDDSet::getMemorySize() const
	{
	return DBNamedObject::getMemorySize() + sizeof(OIdSet) + mySet.size() * sizeof(OId); 
	}

void
DBMDDSet::insert(DBMDDObjId elem)
	{
	if (!contains_element(elem))
		{
		setModified();
		elem->incrementPersRefCount();
		mySet.insert(elem);
		}
	}

DBMDDObjIdIter*
DBMDDSet::newIterator() const
	{
	return new DBMDDObjIdIter(mySet);
	}

unsigned int 
DBMDDSet::getCardinality() const
	{
	return mySet.size();
	}

void
DBMDDSet::deleteName()
	{
	setName("\0");
	setModified();
	}

void
DBMDDSet::releaseAll()
	{
	RMDBGONCE(1, RMDebug::module_mddif, "DBMDDSet", "releaseAll() " << myOId << "\tdoes not do anything");
	}

DBMDDSetId
DBMDDSet::getDBMDDSet(const char* name) throw (r_Error)
	{
	RMDBGENTER(4, RMDebug::module_mddif, "DBMDDSet", "getDBMDDSet(" << name << ")");
	DBMDDSetId set((DBMDDSet*)ObjectBroker::getObjectByName(OId::MDDCOLLOID, name));
	RMDBGEXIT(4, RMDebug::module_mddif, "DBMDDSet", "getDBMDDSet(" << name << ")");
	return set;
	}

DBMDDSetId
DBMDDSet::getDBMDDSet(const OId& o) throw (r_Error)
	{
	RMDBGENTER(4, RMDebug::module_mddif, "DBMDDSet", "getDBMDDSet(" << o << ")");
	DBMDDSetId set((DBMDDSet*)ObjectBroker::getObjectByOId(o));
	RMDBGEXIT(4, RMDebug::module_mddif, "DBMDDSet", "getDBMDDSet(" << o << ")");
	return set;
	}

bool
DBMDDSet::deleteDBMDDSet(const OId& oid)
	{
	bool retval = true;
	try	{
		DBObject* set = ObjectBroker::getObjectByOId(oid);
		set->setPersistent(false);
		}
	catch	(r_Error& e)
		{
		if (e.get_kind() == r_Error::r_Error_ObjectUnknown)
			retval = false;
		else
			throw;
		}
	return retval;
	}

bool
DBMDDSet::deleteDBMDDSet(const char* name)
	{
	bool retval = true;
	try	{
		DBObject* set = ObjectBroker::getObjectByName(OId::MDDCOLLOID, name);
		set->setPersistent(false);
		}
	catch	(r_Error& e)
		{
		if (e.get_kind() == r_Error::r_Error_ObjectUnknown)
			retval = false;
		else
			throw;
		}
	return retval;
	}

DBMDDSet::DBMDDSet(const char* name, const CollectionType* type) throw (r_Error)
	:	DBNamedObject(name),
		collType(type)
	{
	if (name == NULL)
		setName("unnamed collection");
	if (type == NULL)
		{
		RMDBGONCE(0, RMDebug::module_mddif, "DBMDDSet", "DBMDDSet(" << name << ", NULL)")
		RMInit::logOut << "DBMDDSet::DBMDDSet() the collection type is NULL" << endl;
		throw r_Error(COLLECTIONTYPEISNULL);
		}
	objecttype = OId::MDDCOLLOID;
	}

void
DBMDDSet::setPersistent(bool state) throw (r_Error)
	{
	DBMDDSet* set = NULL;
	if (state)
		{
		if (!collType->isPersistent())
			{
			r_Error t(RASTYPEUNKNOWN);
			t.setTextParameter("type", collType->getName());
			RMDBGONCE(0, RMDebug::module_mddif, "DBMDDSet", "setPersistent(" << state << ") " << getName() << ", " << collType->getName() << " not persistent");
			throw t;
			}
		try	{
			set = (DBMDDSet*)ObjectBroker::getObjectByName(OId::MDDCOLLOID, getName());
			}
		catch	(r_Error& err)
			{
			if (err.get_kind() == r_Error::r_Error_ObjectUnknown)
				set = NULL;
			else
				throw;
			}
		if (set)
			{
			RMDBGMIDDLE(6, RMDebug::module_mddif, "DBMDDSet", "already have a set with name " << getName());
			RMInit::logOut << "DBMDDSet::DBMDDSet() mdd collection with name \"" << getName() << "\" exists already" << endl;
			throw r_Error(r_Error::r_Error_NameNotUnique);
			}
		}
	else	{
		removeAll();
		}
	DBNamedObject::setPersistent(state);
	}

DBMDDSet::DBMDDSet(const OId& id) throw (r_Error)
	:	DBNamedObject(id),
		collType(0)
	{
	RMDBGENTER(4, RMDebug::module_mddif, "DBMDDSet", "DBMDDSet(" << myOId << ")");
	objecttype = OId::MDDCOLLOID;
	readFromDb();
	RMDBGEXIT(4, RMDebug::module_mddif, "DBMDDSet", "DBMDDSet(" << myOId << ")");
	}

void
DBMDDSet::updateInDb() throw (r_Error)
	{
	RMDBGENTER(4, RMDebug::module_mddif, "DBMDDSet", "updateInDb() " << myOId);
	deleteFromDb();
	insertInDb();
	DBObject::updateInDb();
	RMDBGEXIT(4, RMDebug::module_mddif, "DBMDDSet", "updateInDb() " << myOId);
	}

