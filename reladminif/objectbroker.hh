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
#ifndef _OBJECTBROKER_HH_
#define _OBJECTBROKER_HH_

class ObjectBroker;
class ULongType;
class LongType;
class CharType;
class BoolType;
class UShortType;
class ShortType;
class OctetType;
class DoubleType;
class FloatType;
class ComplexType1;
class ComplexType2;
class StructType;
class BaseType;
class SetType;
class MDDType;
class Type;
class DBObject;

#include "oidif.hh"
#include "raslib/error.hh"
#include "lists.h"

#ifdef RMANBENCHMARK
#include "raslib/rmdebug.hh"
#endif

//@ManMemo: Module: {\bf reladminif}.
/*@Doc:
the ObjectBroker is one of the three columns one which the persistence layer rests:
ObjectBroker, DBRef, DBObject.
the ObjectBroker keeps track of all persistent objects which are loaded into memory.
objects register themselves at this facility and deregister themselves when they are 
deleted.

The ObjectBroker supplies functionality for accessing persistent objects by oid and by
name.
The ObjectBroker can be configured to serve as a cross transaction cache.
The ObejctBroker can retrieve the oids of all objects of a specific type which are stored
in the database.
The list of objects which are kept by the ObjectBroker are cleared by TransactionIf.
*/
/**
  * \ingroup Reladminifs
  */
class ObjectBroker
	{
	public:
		static bool freeMemory() throw (r_Error);
		/*@Doc: 
		this will handle the memory issue.
		returns true if memory was freed, false otherwise.
		if false is returned there is really to much memory allocated and the transaction should be stopped.
		*/

		static void deregisterTileIndexMapping(const OId& tileoid, const OId& indexoid);
		/*@Doc: 
		deletes the object in the correct list
		*/

		static void registerTileIndexMapping(const OId& tileoid, const OId& indexoid);
		/*@Doc: 
		inserts the object in the correct list
		*/

		static void registerDBObject(DBObject* object);
		/*@Doc: 
		inserts the object in the correct list
		*/

		static void deregisterDBObject(const OId& id);
		/*@Doc: 
		removes the object with the specified oid from the list
		*/
	
		static DBObject* getObjectByOId(const OId& id) throw (r_Error);
		/*@Doc: 
		retrieve a dbobject from db.  passes (r_Error)s from DBObject up.
		*/

		static DBObject* isInMemory(const OId& id) throw (r_Error);
		/*@Doc: 
		does not retrieve the object from db.  only retrieves from memory.
		if there is no matching object a 0 pointer is returned.
		throws an (r_Error) if the OId has a invalid type 
		*/

		static OIdSet* getAllObjects(OId::OIdType type);
		/*@Doc: 
		generates a list of all objects in the db and in memory with the
		specified type.  the vector must be deleted by the calling function
		*/

		static MDDType* getMDDTypeByName(const char* name) throw (r_Error);
		/*@Doc:
		retrieves an object with that name and that type from the db.
		this method was introduced to remove the spurious sqlwarnings.
		passes (r_Error)s from DBObject up or ObjectNotFound when there is
		no matching object
		*/

		static DBObject* getObjectByName(OId::OIdType type, const char* name) throw (r_Error);
		/*@Doc:
		retrieves an object with that name and that type from the db.
		passes (r_Error)s from DBObject up or ObjectNotFound when there is
		no matching object
		*/

		static void clearBroker() throw (r_Error);
		/*@Doc:
		deletes persistent objects from memory, depending on cache ofcourse.
		any (r_Error)s thrown during the deletion/update/insert process are
		handed up.  the objects are removed before errors can occur.  multiple
		calls to clearBroker until the method completes are possible.  memory
		leaks may occur in this event.
		*/

		static void clearCache() throw (r_Error);
		/*@Doc:
		deletes _all_ persistent objects from memory.
		any (r_Error)s thrown during the deletion/update/insert process are
		handed up.  the objects are removed before errors can occur.  multiple
		calls to clearBroker until the method completes are possible.  memory
		leaks may occur in this event.
		*/

		static void init();
		/*@Doc:
		initialize the atomic types and maps 
		*/
		
		static void deinit();
		/*@Doc:
		delete the atomic types and maps 
		*/
		
		static DBObjectPMap& getMap(OId::OIdType type) throw (r_Error);
		/*@Doc:
		returns a pointer to the store of objects of that particular type.
		(r_Error) is thrown if there is no map for that type
		*/

	protected:
		static DBObject* loadDBMDDObj(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves MDDObjects from memory and from database
		*/

		static DBObject* loadMDDSet(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves MDDSets from memory and from database
		*/

		static DBObject* loadMDDType(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves MDDTypes from memory and from database
		*/

		static DBObject* loadMDDBaseType(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves MDDBaseTypes from memory and from database
		*/

		static DBObject* loadMDDDimensionType(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves MDDDimTypes from memory and from database
		*/

		static DBObject* loadMDDDomainType(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves MDDDomTypes from memory and from database
		*/

		static DBObject* loadStructType(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves StructTypes from memory and from database
		*/

		static DBObject* loadSetType(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves SetTypes from memory and from database
		*/

		static DBObject* loadBLOBTile(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves BLOBTiles from memory and from database.
		knows how to get a inlinetile from a dbtcindex.
		*/

		static DBObject* loadDBMinterval(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves DBMintervals from memory and from database
		*/

		static DBObject* loadDBStorage(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves DBStorageLayout from memory and from database
		*/

		static DBObject* loadDBHierIndex(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves HierIxs from memory and from database
		*/

		static DBObject* loadDBTCIndex(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves DBTCIndex from memory and from database
		*/

		static DBObject* loadInlineTile(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves InlineTiles from memory and from database
		*/

		static DBObject* loadDBRCIndexDS(const OId& id) throw (r_Error);
		/*@Doc:
		retrieves DBRCIndexDS from memory and from database
		*/

		static OId getOIdByName(OId::OIdType type, const char* name) throw (r_Error);
		/*@Doc:
		finds the oid of an object with the given type and name
		*/

		static void clearMap(DBObjectPMap& theMap) throw (r_Error);
		/*@Doc:
		deletes entries in this map as long as they are not cached
		(r_Error) is thrown when there is a problem in ~DBObject.
		the object is removed from the map though.
		*/

		static void completelyClearMap(DBObjectPMap& theMap) throw (r_Error);
		/*@Doc:
		deletes entries in this map
		(r_Error) is thrown when there is a problem in ~DBObject.
		the object is removed from the map though.
		*/


		static OId getOIdOfMDDSet(const char* name) throw (r_Error);
		/*@Doc:
		retrieves MDDSets from memory and from database
		*/

		static OId getOIdOfMDDType(const char* name) throw (r_Error);
		/*@Doc:
		retrieves MDDTypes from memory and from database
		*/

		static OId getOIdOfMDDBaseType(const char* name) throw (r_Error);
		/*@Doc:
		retrieves MDDBaseTypes from memory and from database
		*/

		static OId getOIdOfMDDDimensionType(const char* name) throw (r_Error);
		/*@Doc:
		retrieves MDDDimTypes from memory and from database
		*/

		static OId getOIdOfMDDDomainType(const char* name) throw (r_Error);
		/*@Doc:
		retrieves MDDDomTypes from memory and from database
		*/

		static OId getOIdOfStructType(const char* name) throw (r_Error);
		/*@Doc:
		retrieves StructTypes from memory and from database
		*/

		static OId getOIdOfSetType(const char* name) throw (r_Error);
		/*@Doc:
		retrieves SetTypes from memory and from database
		*/

		static OIdSet* getAllMDDObjects() throw (r_Error);
		/*@Doc:
		retrieves the oids of all MDD Objects from memory and from database
		oidlist must be deallocated by the caller
		(r_Error) occures in a database failure
		*/

		static OIdSet* getAllMDDSets() throw (r_Error);
		/*@Doc:
		retrieves the oids of all MDDSets from memory and from database
		oidlist must be deallocated by the caller
		(r_Error) occures in a database failure
		*/

		static OIdSet* getAllMDDTypes() throw (r_Error);
		/*@Doc:
		retrieves the oids of all MDDTypes from memory and from database
		oidlist must be deallocated by the caller
		(r_Error) occures in a database failure
		*/

		static OIdSet* getAllMDDBaseTypes() throw (r_Error);
		/*@Doc:
		retrieves the oids of all MDDBaseTypes from memory and from database
		oidlist must be deallocated by the caller
		(r_Error) occures in a database failure
		*/

		static OIdSet* getAllMDDDimensionTypes() throw (r_Error);
		/*@Doc:
		retrieves the oids of all MDDDimTypes from memory and from database
		oidlist must be deallocated by the caller
		(r_Error) occures in a database failure
		*/

		static OIdSet* getAllMDDDomainTypes() throw (r_Error);
		/*@Doc:
		retrieves the oids of all MDDDomTypes from memory and from database
		oidlist must be deallocated by the caller
		(r_Error) occures in a database failure
		*/

		static OIdSet* getAllStructTypes() throw (r_Error);
		/*@Doc:
		retrieves the oids of all StructTypes from memory and from database
		oidlist must be deallocated by the caller
		(r_Error) occures in a database failure
		*/

		static OIdSet* getAllSetTypes() throw (r_Error);
		/*@Doc:
		retrieves the oids of all SetTypes from memory and from database
		oidlist must be deallocated by the caller
		(r_Error) occures in a database failure
		*/

		static OIdSet* getAllAtomicTypes() throw (r_Error);
		/*@Doc:
		retrieves the oids of all AtomicTypes from memory
		oidlist must be deallocated by the caller
		(r_Error) should never occur 
		*/

	private:
		///the types have to be in fornt of* the maps because of static destructor!
		static ULongType* theULong;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/

		static CharType* theChar;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/

		static BoolType* theBool;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/
	
		static UShortType* theUShort;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/
		
		static LongType* theLong;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/
		
		static ShortType* theShort;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/
		
		static OctetType* theOctet;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/
		
		static DoubleType* theDouble;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/
		
		static FloatType* theFloat;
		/*@Doc:
		a pointer to this member is returned by getObjectByOId().
		*/
		
		static ComplexType1* theComplex1;
		/*@Doc:
	 	a pointer to this member is returned by getObjectByOId().
		*/
	
		static ComplexType2* theComplex2;
		/*@Doc:
	 	a pointer to this member is returned by getObjectByOId().
		*/
	
		static DBObjectPMap theDBMDDObjs;
		/*@Doc:
		holds all MDDObjects in memory
		*/

		static DBObjectPMap theMDDSets;
		/*@Doc:
		holds all MDDSets in memory
		*/

		static DBObjectPMap theMDDTypes;
		/*@Doc:
		holds all MDDTypes in memory
		*/

		static DBObjectPMap theMDDBaseTypes;
		/*@Doc:
		holds all MDDBaseTypes in memory
		*/

		static DBObjectPMap theMDDDimensionTypes;
		/*@Doc:
		holds all MDDDimTypes in memory
		*/

		static DBObjectPMap theMDDDomainTypes;
		/*@Doc:
		holds all MDDDomTypes in memory
		*/

		static DBObjectPMap theStructTypes;
		/*@Doc:
		holds all StructTypes in memory
		*/

		static DBObjectPMap theSetTypes;
		/*@Doc:
		holds all SetTypes in memory
		*/

		static DBObjectPMap theBLOBTiles;
		/*@Doc:
		holds all BLOBTiles in memory
		*/

		static DBObjectPMap theDBMintervals;
		/*@Doc:
		holds all DBMintervals in memory
		*/

		static DBObjectPMap theDBStorages;
		/*@Doc:
		holds all MDDEntries in memory
		*/

		static DBObjectPMap theDBHierIndexs;
		/*@Doc:
		holds all HierIxs in memory
		*/

		static DBObjectPMap theDBTCIndexs;
		/*@Doc:
		holds all HierIxs in memory
		*/

		static DBObjectPMap theInlineTiles;
		/*@Doc:
		holds all InlineTiles in memory
		*/

		static DBObjectPMap theAtomicTypes;
		/*@Doc:
		holds all SetTypes in memory
		*/

		static DBObjectPMap theRCIndexes;
		/*@Doc:
		holds all RC Indexes in memory
		*/

		static OIdMap theTileIndexMappings;
		/*@Doc:
		key (first/double) is oid of tile, value (second/oid) is oid of index.
		*/

	};

#endif

