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
#ifndef _MDDSET_HH_
#define _MDDSET_HH_

class MDDSet;
class OId;
class EOId;
class r_Error;
class DBMDDObj;
class CollectionType;

#include "reladminif/lists.h"
#include "mddid.hh"
#include "reladminif/dbnamedobject.hh"
#include "reladminif/dbref.hh"

//@ManMemo: Module: {\bf mddif}
/*@Doc:
  MDDSet is the persistent class for collections of MDD objects.

  Each instance of MDDSet represents a collection stored in the base
  DBMS.

  This class should only be used by DBMDDColl.
*/

/**
  * \ingroup Relmddifs
  */

class DBMDDSet  :   public DBNamedObject
{
public:
    DBMDDSet(const char* name, const CollectionType* type) throw (r_Error);
    /*@Doc:
        creates a new set
    */

    DBMDDSet(const char* name, const OId& id, const CollectionType* type) throw (r_Error);
    /*@Doc:
        creates a new set
    */

    static DBMDDSetId getDBMDDSet(const char* name) throw (r_Error);

    static DBMDDSetId getDBMDDSet(const OId& id) throw (r_Error);

    static bool deleteDBMDDSet(const OId& id);
    /*@Doc:
        returns succes
    */

    static bool deleteDBMDDSet(const char* name);
    /*@Doc:
        returns succes
    */

    virtual void printStatus(unsigned int level = 0, std::ostream& stream = std::cout) const;

    virtual void setPersistent(bool state) throw (r_Error);
    /*@Doc:
        throws r_Error when the mdd set may not be made persistent.
    */

    void insert(DBMDDObjId newObj);
    /*@Doc:
        Inserts an object into the MDD Collection.
        The persistent reference count of this DBMDDObj is updated.
    */

    DBMDDObjIdIter* newIterator() const;
    /*@Doc:
        Returns a new iterator for this collection.
    */

    unsigned int getCardinality() const;
    /*@Doc:
        Returns the number of elements in the collection.
    */

    void remove(DBMDDObjId& obj);
    /*@Doc:
        Removes an object from the MDD Collection.
        The persistent reference count of this DBMDDObj is updated.
    */

    void removeAll();
    /*@Doc:
        Removes all objects from the MDD Collection.
        The persistent reference count of the DBMDDObjs is updated.
    */

    void releaseAll();
    /*@Doc:
        Releases all dynamic memory used by this collection.
    */

    bool contains_element(const DBMDDObjId& elem) const;
    /*@Doc:
    */

    virtual ~DBMDDSet( );
    /*@Doc:
    */

    void deleteName();
    /*@Doc:
        sets the name of this object to a null string.
        used by DatabaseIf::destroyRoot
    */

    virtual r_Bytes getMemorySize() const;
    /*@Doc:
    */

    const CollectionType* getCollType() const;
    /*@Doc:
        Returns the collectiontype of this entry.
    */

protected:
    friend class ObjectBroker;

    typedef std::set<DBMDDObjId, std::less<DBMDDObjId> > DBMDDObjIdSet;

    DBMDDSet(const OId& id) throw (r_Error);
    /*@Doc:
        gets an existing coll from the db
    */

    virtual void updateInDb() throw(r_Error);
    /*@Doc:
    */

    virtual void insertInDb() throw(r_Error);
    /*@Doc:
    */

    virtual void readFromDb() throw(r_Error);
    /*@Doc:
    */

    virtual void deleteFromDb() throw(r_Error);
    /*@Doc:
    */

private:
    DBMDDObjIdSet mySet;
    /*@Doc:
        Memory representation of the list of oids of DBMDDObjs.
    */

    const CollectionType* collType;
    /*@Doc:
        Pointer to the collectiontype.
    */
};

#endif
