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
/
/**
 * INCLUDE: mddcoll.hh
 *
 * MODULE:  cachetamgr
 * CLASS:   MDDColl
 *
 * COMMENTS:
 *
*/

#ifndef _MDDCOLL_HH_
#define _MDDCOLL_HH_

#include <iostream>
#include <map>
#include <stdlib.h>

#include "mddobj.hh"
#include "mddcolliter.hh"
#include "mddcoll.hh"
#include "reladminif/oidif.hh"
#include "raslib/error.hh" 
#include "relmddif/mddid.hh"
#include "relcatalogif/collectiontype.hh"     // from catalogif base DBMS interface module

class CollectionType;
class DatabaseIf;

//@ManMemo: Module: {\bf cachetamgr}
/*@Doc:
   An MDDColl object is a collection of references to MDD objects. 
   This class is the abstract base class for all persistent and 
   transient collection classes.
   
   The objects pointed to by the collection exist in a storage domain 
   which may be either the main memory for the transient collections
   or the database for the persistent cases.
   
   An MDDColl is responsible for managing the memory allocated for the 
   MDDObjs which belong to it. In the future, more functions will be provided 
   to allow selected releasing or keeping of MDD objects in an MDDColl.

   {\bf Example }

   {\tt MDDObj* accessedObj; }
  
   {\tt MDDColl* mddObjsColl; }
   
   {\tt mddObjsColl = new ... }
   
   {\tt    // Initialize mddObjsColl to a PersMDDColl or TransMDDColl. } 
   
   {\tt MDDCollIter* objsIt = mddObjsColl->createIterator(); }
  
   {\tt for(int i = 1 ; objsIt->notDone(); i++, objsIt->advance()) }
   
   {\tt \{  }
   
   {\tt accessedObj = objsIt->getElement(); }
   
   {\tt ...}
   
   {\tt } }
  
   {\tt delete objsIt; // Deallocate iterator. }
   
   {\tt ... // Here accessedObj may be used. }
  
   {\tt mddObjsColl->releaseAll(); }
   
   {\bf {\tt ... // In this part accessedObj is no longer valid. } }
   
   {\tt delete mddObjsColl; }
*/

class MDDColl
	{
				
	public:
		///transient collection
		MDDColl(const CollectionType* newType, const char* name = 0);

		static const char* AllCollectionnamesName;

		static MDDColl* getMDDCollection(const char* collName) throw (r_Error);
		/**
			Retrieve a mdd collection from database.
			Throws r_Error::r_Error_ObjectUnknown if the name is unknown.
		*/

		static MDDColl* getMDDCollection(const OId& collOId) throw (r_Error);
		/**
			Retrieve a mdd collection from database.
			Throws r_Error::r_Error_ObjectUnknown if the oid is unknown.
		*/

		static MDDColl* createMDDCollection(const char* name, const CollectionType* ct) throw (r_Error);
		/**
			Throws r_Error::r_Error_NameNotUnique if the name exists already or is NULL.
		*/

		static MDDColl* createMDDCollection(const char* name, const OId& o, const CollectionType* ct) throw (r_Error);
		/**
			Throws r_Error::r_Error_NameNotUnique if the name exists already or is NULL.
			Throws r_Error::r_Error_OIdNotUnique if the oid exists already.
		*/
		static bool dropMDDCollection(const char* name);
		/**
			Delete a mdd collection from database.
			Returns false if the name is unknown.
		*/

		static bool dropMDDCollection(const OId& id);
		/**
			Delete a mdd collection from database.
			Returns false if the oid is unknown.
		*/
		 
		static bool removeMDDObject(const OId& coll, const OId& id);
		/**
			Remove an mdd object from the collection.
			Returns always true.
		*/
		 
		 void printStatus(unsigned int level = 0, ostream& stream = cout) const;
		/**
			Prints current status of the MDD Collection.
		*/
		
		 const CollectionType* getCollectionType() const;
		/**
			Returns the type of this MDD collection.
		*/
		
		 unsigned long getCardinality() const;	
		/**
			Returns the cardinality of the collection.
		*/
		
		 bool getOId(OId& pOId) const;
		/**
			Returns true if collection has an OId.
		*/

		 bool getEOId(EOId& pEOId) const;
		/**
			Returns true0 if collection has an EOId.
		*/
	
		 void insert(const MDDObj* newObj);
		/**
			Inserts reference to MDD object into the collection.
			If the type of the object (transient/persistent) does not fit the collection an exception is thrown.
		*/
	 
		MDDCollIter* createIterator() const;
		/**
			Creates a new iterator for this collection. Returns a pointer to the new allocated iterator object. Returned pointer must be freed afterwards.
		*/

		 void remove(const MDDObj* obj);
		/**
			Remove reference to MDD object from the current collection. The object itself is not obligatorily destroyed from the storage domain where it exists.  If the object does not live in the collection nothing is done.
		*/
		
		 void removeAll(); 
		/**
			Empties the current collection by removing all references to MDD objects from it.
			The objects are not obligatorily destroyed from the storage domain where they exist.
		*/
		
		 void releaseAll();	
		/**
			Releases all dynamic memory allocated for the current collection.
			This method has to be called explicitely, since the destructor doesn't deallocate memory for the elements of the collection.
			The reason for this is to allow the user to use MDDObjs from this collection, even after the collection itself is no longer in main memory.
			This is only true for transient collections.
		*/
		
		 bool isPersistent() const;
		/**
			Tells if collection is persistent.
		*/
		
		 const char* getName() const;

		 ~MDDColl();
		/**
			Doesn't free main memory allocated for the objects of the collection. 
			See ReleaseAll() for an explanation on this issue.  Even if it does nothing, it has to be defined because derived classes may have non-trivial destructors.
		*/
	protected:

		friend class MDDCollIter;

		MDDObj* getMDDObj(const DBMDDObj*) const;
		/**
			Will get from cache/instantiate and return an MDDObj.
			Instantiation: it will just create a new one if isPersistent() == true -> passing it a oid that does not belong to the collection -> problem.
		**/
	
		MDDColl(const DBMDDSetId& coll);

		DBMDDSetId getDBMDDSet() const;
		/**
			Return the reference to the actual persistent object in the base DBMS.
			returned reference must be const -> impossible
		*/

		void insertIntoCache(const MDDObj* objToInsert) const;
		/**
			Insert persistent MDD object in the internal cache.
		*/

		//void removeFromCache(DBMDDObjId& objToGet);

		DBMDDSetId dbColl;
		/**
			Pointer to the actual class stored in the base DBMS.
			Based on the Base DBMS DBMDDSet class from relmddif.
		*/

		typedef std::map<DBMDDObj*, MDDObj*, std::less<DBMDDObj*> > MDDObjMap;
		mutable MDDObjMap mddCache;
		 /** 
			The mdd objs that are instantiated may not be twice in memory.  Therefore it has to be checked if an mdd obj was already created for a specific dbmddobj.
			This is achieved by maintaining this map.
		 */
	};

#endif

