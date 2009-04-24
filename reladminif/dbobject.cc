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
/*************************************************************************
 *
 *
 * PURPOSE:
 *   Code with embedded SQL for relational DBMS
 *
 *
 * COMMENTS:
 *   none
 *
 ***********************************************************************/

// mainly for ostringstream:
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

using namespace std;

#include "objectbroker.hh"
#include "dbobject.hh"
#include "adminif.hh"
#include "externs.h"
#include "raslib/rmdebug.hh"
#include "raslib/error.hh"
#include "eoid.hh"

#ifdef RMANBENCHMARK
RMTimer DBObject::readTimer = RMTimer("DBObject","read");
RMTimer DBObject::updateTimer = RMTimer("DBObject","update");
RMTimer DBObject::insertTimer = RMTimer("DBObject","insert");
RMTimer DBObject::deleteTimer = RMTimer("DBObject","delete");
#endif

const char*
BinaryRepresentation::fileTag = "RMAN";

void
DBObject::printStatus(unsigned int level, std::ostream& stream) const
	{
	char* indent = new char[level*2 +1];
	for (int j = 0; j < level*2 ; j++)
		indent[j] = ' ';
	indent[level*2] = '\0';
	
	stream << indent; 
	stream << myOId;
	delete[] indent;
	indent=0;
	}

r_Bytes
DBObject::getTotalStorageSize() const
	{
	return 0;
	}

r_Bytes
DBObject::getMemorySize() const
	{
	return sizeof(DBObject);
	}

void
DBObject::setCached(bool newCached)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "setCached(" << (int)newCached << ") " << myOId);
	_isCached = newCached;
	}

bool
DBObject::isCached() const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "isCached()" << (int) _isCached)
	return _isCached;
	}

void
DBObject::destroy()
	{
	RMDBGENTER(10, RMDebug::module_adminif, "DBObject", "destroy() " << myOId);
#ifdef RMANDEBUG
	OId tempid(myOId); 
#endif
  	if (referenceCount == 0)
		{
		if (!_isCached)
			{
			//exception may be possible when !isModified()
			if (!AdminIf::isReadOnlyTA())
				{
				RMDBGMIDDLE(10, RMDebug::module_adminif, "DBObject", "deleting object " << myOId);
				delete this;//is dynamic and may be deleted
				}
			else	{
				if (!_isPersistent)
					{
					RMDBGMIDDLE(10, RMDebug::module_adminif, "DBObject", "deleting object " << myOId);
					//is dynamic and may be deleted
					delete this;
					}
				}
			}
		}
#ifdef RMANDEBUG 
	RMDBGEXIT(10, RMDebug::module_adminif, "DBObject", "destroy() " << tempid);
#else
	RMDBGEXIT(10, RMDebug::module_adminif, "DBObject", "destroy()");
#endif
	}

void
DBObject::release()
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "release() ");
	//no dynamic memory
	}

void DBObject::incrementReferenceCount(void)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "incrementReferenceCount() " << referenceCount);
	referenceCount++;
	}

void DBObject::decrementReferenceCount(void)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "decrementReferenceCount() " <<referenceCount);
	referenceCount--;
	if (referenceCount == 0)
		destroy();
	}

int
DBObject::getReferenceCount(void) const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "getReferenceCount() " <<referenceCount);
	return referenceCount;
	}

//public:
DBObject::DBObject()
	:	_isInDatabase(false),
		_isPersistent(false),
		_isModified(true),
		_isCached(false),
		referenceCount(0),
		objecttype(OId::INVALID),
		myOId(0)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "DBObject() " << myOId);
	}

DBObject::DBObject(const DBObject& old)
	:	_isInDatabase(old._isInDatabase),
		_isPersistent(old._isPersistent),
		_isModified(old._isModified),
		_isCached(old._isCached),
		referenceCount(old.referenceCount),
		objecttype(old.objecttype),
		myOId(old.myOId)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "DBObject(const DBObject& old)" << myOId);
	}

//constructs an object and reads it from the database.  the oid must match the type of the object.
//a r_Error::r_Error_ObjectUnknown is thrown when the oid is not in the database.
DBObject::DBObject(const OId& id) throw (r_Error)
	:	referenceCount(0),
		_isCached(false),
		myOId(id),
		objecttype(id.getType())
	{
	//flags must be set by readFromDb()
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "DBObject(" << myOId << ")");
	}

DBObject&
DBObject::operator=(const DBObject& old)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBObject", "operator=(" << old.myOId << ")");
	if (this != &old)
		{
		_isPersistent = old._isPersistent;
		_isInDatabase = old._isInDatabase;
		_isModified = old._isModified;
		_isCached = old._isCached;
		objecttype = old.objecttype;
		myOId = old.myOId;
		}
	return *this;
	}

//setPersistent(true) makes the object persistent as soon as validate is called.
//a r_Error::r_Error_TransactionReadOnly is thrown when the transaction is readonly. 
void
DBObject::setPersistent(bool newPersistent) throw (r_Error)
	{
	RMDBGENTER(6, RMDebug::module_adminif, "DBObject", "setPersistent(" << (int)newPersistent << ") " << myOId);
	if (newPersistent)
	     {//make object persistent 
		if (!_isPersistent)
			{//object is not persistent
			if (!AdminIf::isReadOnlyTA())
				{//may be written to database
				OId::allocateOId(myOId, objecttype);
				_isPersistent = true;
				_isModified = true;
				ObjectBroker::registerDBObject(this);
				RMDBGMIDDLE(6, RMDebug::module_adminif, "DBObject", "persistent\t: yes, was not persistent");
				}
			else	{//read only transaction
				RMDBGEXIT(6, RMDebug::module_adminif, "DBObject", "ReadOnlyTransaction");
				RMInit::logOut << "DBObject::setPersistent() read only transaction" << endl;
				throw r_Error(r_Error::r_Error_TransactionReadOnly);
				}
			}
		else	{//is already persitent
			RMDBGMIDDLE(6, RMDebug::module_adminif, "DBObject", "persistent\t: yes, was already persistent");
			}
		}
	else	{//delete the object from database
		if (_isPersistent)
			{
			if (!AdminIf::isReadOnlyTA())
				{//may be deleted to database
				RMDBGMIDDLE(6, RMDebug::module_adminif, "DBObject", "persistent\t: no, was persistent");
				_isPersistent = false;
				_isModified = true;
				}
			else	{//read only transaction
				RMDBGEXIT(6, RMDebug::module_adminif, "DBObject", "ReadOnlyTransaction");
				RMInit::logOut << "DBObject::setPersistent() read only transaction" << endl;
				throw r_Error(r_Error::r_Error_TransactionReadOnly);
				}
			}
		else	{
			RMDBGMIDDLE(6, RMDebug::module_adminif, "DBObject", "persistent\t: no, was not persistent");
			}
		}
	RMDBGEXIT(6, RMDebug::module_adminif, "DBObject", "setPersistent(" << (int)newPersistent << ") " << myOId);
	}
	
//tells if an object is persistent.
bool
DBObject::isPersistent() const
	{
	RMDBGONCE(6, RMDebug::module_adminif, "DBObject", "isPersistent()" << (int)_isPersistent); 
	return _isPersistent;
	}

//writes the object to database/deletes it or updates it.
//a r_Error::r_Error_TransactionReadOnly is thrown when the transaction is readonly. 
void
DBObject::validate() throw (r_Error)
	{
	RMDBGENTER(9, RMDebug::module_adminif, "DBObject", "validate() " << myOId);
	if (_isModified)
		{
		if (!AdminIf::isReadOnlyTA())
			{
			if (!AdminIf::isAborted())
				{
				if (_isInDatabase)
					{
					if (_isPersistent)
						{
						RMDBGMIDDLE(11, RMDebug::module_adminif, "DBObject", "is persistent and modified and in database");
#ifdef RMANBENCHMARK
						updateTimer.resume();
#endif
						this->updateInDb();
#ifdef RMANBENCHMARK
						updateTimer.pause();
#endif
						}
					else	{
						RMDBGMIDDLE(11, RMDebug::module_adminif, "DBObject", "is not persistent and in database");
#ifdef RMANBENCHMARK
						deleteTimer.resume();
#endif
						this->deleteFromDb();
#ifdef RMANBENCHMARK
						deleteTimer.pause();
#endif
						}
					}
				else	{
					if (_isPersistent)
						{
						RMDBGMIDDLE(11, RMDebug::module_adminif, "DBObject", "is persistent and modified and not in database");

#ifdef RMANBENCHMARK
						insertTimer.resume();
#endif
						this->insertInDb();
#ifdef RMANBENCHMARK
						insertTimer.pause();
#endif
						}
					else	{
						//don´t do anything: not in db and not persistent
						}
					}
				}
			else	{
				//don´t do anything: is aborted
				}
			}
		else	{
			//don´t do anything: is read only
			}
		}
	else	{
		//don´t do anything: not modified
		}
	RMDBGEXIT(9, RMDebug::module_adminif, "DBObject", "validate() " << myOId);
	}

void
DBObject::setModified() throw (r_Error)
	{
	RMDBGENTER(8, RMDebug::module_adminif, "DBObject", "setModified() " << myOId);
	if (!AdminIf::isReadOnlyTA())
		_isModified = true;
	else	{
		RMDBGMIDDLE(5, RMDebug::module_adminif, "DBObject", "readonly transaction " << myOId);
		//this happens really a lot.
		//RMInit::logOut << "DBObject::setModified() read only transaction" << endl;
		//throw r_Error(r_Error::r_Error_TransactionReadOnly);
		_isModified = true;
		}
	RMDBGEXIT(8, RMDebug::module_adminif, "DBObject", "setModified() " << myOId);
	}

bool
DBObject::isModified() const
	{
	RMDBGONCE(8, RMDebug::module_adminif, "DBObject", "isModified() " << (int)_isModified);  
	return _isModified;
	}

OId
DBObject::getOId() const
	{
	RMDBGONCE(8, RMDebug::module_adminif, "DBObject", "getOId() " << myOId);  
	return myOId;
	}

EOId
DBObject::getEOId() const
	{
        RMDBGONCE(8, RMDebug::module_adminif, "DBObject", "getEOId() " << myOId);  
	return EOId(myOId);
	}

OId::OIdType
DBObject::getObjectType() const
	{
	RMDBGONCE(8, RMDebug::module_adminif, "DBObject", "getObjectType() " << objecttype);  
	return objecttype;
	}

DBObject::~DBObject()
	{
	RMDBGENTER(10, RMDebug::module_adminif, "DBObject", "~DBObject() " << myOId);
	ObjectBroker::deregisterDBObject(myOId);
	RMDBGEXIT(10, RMDebug::module_adminif, "DBObject", "~DBObject() " << myOId);
	}

void
DBObject::updateInDb() throw (r_Error)
	{
	RMDBGENTER(10, RMDebug::module_adminif, "DBObject", "updateInDb() " << myOId);
	_isModified = false;
	_isInDatabase = true;
	_isPersistent = true;
	RMDBGEXIT(10, RMDebug::module_adminif, "DBObject", "updateInDb() " << myOId);
	}

//writes the object into the database.  the object must not be in the database.
void
DBObject::insertInDb() throw (r_Error)
	{
	RMDBGENTER(10, RMDebug::module_adminif, "DBObject", "insertInDb() " << myOId);
	_isModified = false;
	_isInDatabase = true;
	_isPersistent = true;
	RMDBGEXIT(10, RMDebug::module_adminif, "DBObject", "insertInDb() " << myOId);
	}

void
DBObject::deleteFromDb() throw (r_Error)
	{
	RMDBGENTER(10, RMDebug::module_adminif, "DBObject", "deleteFromDb() " << myOId);
	_isModified = false;
	_isInDatabase = false;
	_isPersistent = false;
	RMDBGEXIT(10, RMDebug::module_adminif, "DBObject", "deleteFromDb() " << myOId);
	}

void
DBObject::readFromDb() throw (r_Error)
	{
	RMDBGENTER(10, RMDebug::module_adminif, "DBObject", "readFromDb() " << myOId);
	_isPersistent = true;
	_isModified = false;
	_isInDatabase = true;
	RMDBGEXIT(10, RMDebug::module_adminif, "DBObject", "readFromDb() " << myOId);
	}

BinaryRepresentation
DBObject::getBinaryRepresentation() const throw (r_Error)
	{
	RMInit::logOut << "getBinaryRepresentation() for " << objecttype << " not implemented" << endl;
	throw r_Error(BINARYEXPORTNOTSUPPORTEDFOROBJECT);
	}

void
DBObject::setBinaryRepresentation(const BinaryRepresentation& br) throw (r_Error)
	{
	RMInit::logOut << "setBinaryRepresentation() for " << objecttype << " not implemented" << endl;
	throw r_Error(BINARYIMPORTNOTSUPPORTEDFOROBJECT);
	}

char*
DBObject::getBinaryName() const
	{
	//if we use 64bit oids we have at most 20 digits + "_" + type
	ostringstream o;
	o << (int)objecttype << '_' << myOId.getCounter() << ".raw";
	const char* temp = o.str().c_str();
	char* retval = new char[strlen(temp) + 1];
	memcpy(retval, temp, strlen(temp) + 1);
	return retval;
	}
