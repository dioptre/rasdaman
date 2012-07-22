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
#ifndef _DBTCINDEX_HH_
#define _DBTCINDEX_HH_

#include "hierindex.hh"
#include "relblobif/tileid.hh"

//@ManMemo: Module: {\bf relindexif}
/*@Doc:
This class stores data of hierarchical indexes in the database.  It is extended to store tiles _inline_.  this means it is possible for this class to concatenate several tiles into one large blob and store this blob in the database.  the tiles must be inlinetiles.  it is only interessting if the size of the tiles is very small (smaller than a disc page).

for further information on the methods of this class see hierindexds and indexds interface classes in indexmgr.
*/
/**
  * \ingroup Relindexifs
  */

class DBTCIndex :   public DBHierIndex
{
public:
    DBTCIndex(r_Dimension dim, bool isNode);
    /*@Doc:
        constructs a new index with dimension dim.
        instance is imediately persistent
    */

    virtual void printStatus(unsigned int level = 0, std::ostream& stream = std::cout) const;
    /*@Doc:
        Prints current status of index, in hierarchical format.
    */

    ~DBTCIndex();
    /*@Doc:
    */

    virtual bool removeObject(const KeyObject& entry);
    /*@Doc:
        will take care of inlined tiles when they are removed.
    */

    virtual bool removeObject(unsigned int pos);
    /*@Doc:
        will take care of inlined tiles when they are removed.
    */

    virtual void removeInlineTile(InlineTile*);
    /*@Doc:
        this method is called by inlinetile to removeitselve from the index when it is outlined.  the oid of this tile is still stored in the index.  do not confuse with removeObejct!
    */

    virtual void addInlineTile(InlineTile*);
    /*@Doc:
        this method is called by inlinetile when it is told to inline itself into this index.
    */

    void setInlineTileHasChanged();
    /*@Doc:
        called by an inlined inlinetile when it is modified.  the index must know about this in order to update the inlinetile in the database.
    */

    virtual IndexDS* getNewInstance() const;
    /*@Doc:
        used by indexmgr index logic classes to generate new nodes/leaves without knowing what kind of index structure it is operating with.  in essence a clone() pattern.
    */

protected:
    friend class ObjectBroker;
    /*@Doc:
        ObjectBroker needs to access OId constructor and getInlineTile
    */

    InlineTile* getInlineTile(const OId& itid);
    /*@Doc:
        returns the specified inline tile.
        memory management is done by the DBTCIndex object.
    */


    void changeIOIdToBOId();
    /*@Doc:
        changes all inlineoids to bloboids.  needed in order to be able to use dbhierindex database functionality.
    */


    void readyForRemoval(const OId& id);
    /*@Doc:
        inlined inlinetiles must be loaded previous to removing them in order to get them outlined.
    */

    void changeBOIdToIOId();
    /*@Doc:
        changes the bloboids of the inlinetiles which are stored in this index into inlineoids.  also neccessary to be able to use dbhierindex functionality.
    */

    void registerIOIds();
    /*@Doc:
        registers all inline oids with the objectbroker.  changeIOIdToBOId is supposed to be called afterwards.
    */

    void readInlineTiles() throw (r_Error);
    /*
        loads the tiles from the blob tablespace.
        errors are database errors.
    */

    void decideForInlining();
    /*
        makes blobtiles to inlinetiles and vice versa.
    */

    void insertBlob();
    /*@Doc:
        insert empty blob into db
    */

    void storeTiles();
    /*@Doc:
        write the tiles into the blob space if neccessary.
    */

    void writeInlineTiles(char* cells, r_Bytes blobSize) throw (r_Error);
    /*
        writes the tiles into the blob tablespace.
        errors are database errors.
    */

    void updateTileIndexMappings() throw (r_Error);
    /*
        writes the mappings among dbtcindex and inlinetiles into the database for objectbroker to see.
        errors are database errors.
    */

    DBTCIndex(const OId& id);

    void setMappingHasChanged();
    /*@Doc:
        tells the index that it has to update the table for mapping inlined inlinetile oids to dbtcindexes.
    */

    virtual void readFromDb() throw (r_Error);

    virtual void updateInDb() throw (r_Error);

    virtual void deleteFromDb() throw (r_Error);

    virtual void insertInDb() throw (r_Error);

    bool mappingHasChanged;
    /*@Doc:
        is true when an inlinetile has been added or removed.
    */

    bool inlineTileHasChanged;
    /*@Doc:
        is true when the inlined tiles need to be updated.
    */

    bool _isLoaded;
    /*@Doc:
        transient, tells if the object has loaded its inlined tiles.
    */

    bool hasBlob;
    /*@Doc:
        transient, tells if the object has already a blob in the database or not.
    */

    DBObjectPMap inlineTiles;
    /*@Doc:
        transient, contains pointers to materialised inlinetiles.
        is filled on demand by loadInlineTiles.
    */

};
#endif
