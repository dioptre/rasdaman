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

#ifndef _MDDOBJIX_HH_
#define _MDDOBJIX_HH_

class PersTile;
class BaseType;
class Tile;
class r_Point;
#include "raslib/minterval.hh"
#include "raslib/rmdebug.hh"
#include "indexmgr/indexds.hh"
#include "storagemgr/sstoragelayout.hh"
#include "reladminif/lists.h"
#include "relindexif/indexid.hh"
#include <vector>


/****************************************************************************
 *
 *
 * INCLUDE: mddobjix.hh
 *
 * MODULE:  indexmgr
 * CLASS:   MDDObjIx
 *
 *
 * COMMENTS:
 *     none
 *
 ****************************************************************************/


//@ManMemo: Module: {\bf indexmgr}
/*@Doc:
      
Each MDD Object is composed of a set of tiles which are accessed through an index.
The task of the index is to determine the tiles affected by a spatial operation and to allow fast access to them.  It will also take care of memory management of the tiles.

{\bfMDD Objects indexes: }

MDD Objects indexes are multidimensional since they provide access to multidimensional rectangular tiles existing in multidimensional intervals. An MDDObjIx has to be able to deal with different dimensionalities. 

The index of an MDD object keeps the information about the current domain of the MDD object. During the lifetime of the object, it is possible that the definition (or current) domain of an object is not completely covered by the tiles already inserted. 
At each moment, the current domain of an object is the closure of the domains of all tiles. All tiles should be completely contained 
in the definition domain of an object. 
The definition domain may have open boundaries, but the current domain is always a closed interval.

The lower classes in the indexmgr support storage of any kind of persistent object.
This class has to be changed to reflect this aability.

     In the future, functions have to be implemented which allow the user 
     to give indication about priorities for freeing tiles from main memory. 
     
*/

//function pointer to the static function which inserts objects
typedef bool (*IxLogic_insertObject) (IndexDS* theIx, const KeyObject& theObj, const StorageLayout& sl);

//function pointer to the static function which removes objects
typedef bool (*IxLogic_removeObject) (IndexDS* theIx, const KeyObject& theObj, const StorageLayout& sl);

//function pointer to the static function which gets objects from the index
typedef void (*IxLogic_intersect) (const IndexDS* theIx, const r_Minterval& searchInterval, KeyObjectVector& objs, const StorageLayout& sl);

//function pointer to the static function which gets object at point
typedef void (*IxLogic_containPointQuery) (const IndexDS* theIx, const r_Point& searchPoint, KeyObject& result, const StorageLayout& sl);

//function pointer to the static function which inserts objects
typedef void (*IxLogic_getObjects) (const IndexDS* theIx, KeyObjectVector& objs, const StorageLayout& sl);

class MDDObjIx             
	{ 
	public:

		MDDObjIx(const StorageLayout& sl, const r_Minterval& dom, const BaseType* bt = 0);
		/*@Doc:
			When bt is NULL this index will behave as if it were a transient index.
		*/

		MDDObjIx(DBObjectId newDBIx, const StorageLayout& sl, const BaseType* bt);
		/*@Doc:
			When bt is NULL this index will behave as if it were a transient index.
		*/

		void printStatus(unsigned int level = 0, std::ostream& stream = std::cout) const;
	
		~MDDObjIx();

		DBObjectId getDBMDDObjIxId() const;

		r_Minterval getCurrentDomain() const;

		r_Dimension getDimension() const;

		void insertTile(const Tile* newTile);

		bool removeTile(const Tile *);

		std::vector<Tile *> * intersect(const r_Minterval &) const;

		char* pointQuery(const r_Point& searchPoint);

		const char* pointQuery(const r_Point& searchPoint) const;

		Tile* containPointQuery(const r_Point& searchPoint) const;

		std::vector< Tile* >* getTiles() const;
		
		bool isPersistent() const;
		
		void releasePersTiles();
		/*@Doc:
			This function has to be called by the destructors of persistent subclasses which use the cache, to make sure that the memory for the tiles in the cache is freed.
			This will only have effect on persistent indexes.
			Transient indexes keep their tiles in TransDirIx which deletes its tiles in the destructor.
		*/
		
	protected:

		void setNewLastAccess(const r_Minterval& newLastAccess, const std::vector<Tile*>* newLastTiles);

		void setNewLastAccess(const Tile* newLastTile, bool te = true);
		/*@Doc:
			Add a new tile to the cache and reset the access domain 
			If clear:
				clear the cache
				release all tiles
		*/
		
		std::vector< Tile* >* lastAccessIntersect(const r_Minterval& searchInter) const;
		
		Tile* lastAccessPointQuery(const r_Point& searchPoint) const;
		
		bool removeTileFromLastAccesses(const Tile* tileToRemove);
		/*@Doc:
			Does NOT free tileToRemove allocated memory. Only removes this pointer from lastAccesses list if it finds it there.
			Returns true if found. 
			Subclasses which use the cache must call this function before removing a tile from the index, or else the tile may remain in main memory. 
		*/
		
		r_Minterval lastAccess;
		/*@Doc:
			Either empty interval, if not to search in the cache, or an interval 
			corresponding to the search done the last time. The algorithms which
			search this cache for an intersection take this into account.
			It works simultaneously as a flag and as the specification for the last
			access, if valid.
			Last searched region.
		*/
		
		std::vector< Tile* > lastAccessTiles; 
		/*@Doc:
			Internal cache of {\tt Tile}s accessed the last time.
			Contents change everytime there is an insert, an intersect, a getTiles 
			or a poin query, in the following way:
			- insert: cache invalid, last Access is put to empty interval, 
				lastAccessTiles contains the inserted tiles. This is needed 
				if we want the index to automatically manage the memory 
				allocated for persistent tiles or else the just inserted 
				tiles are immediately invalid for the user after insertion 
				in the object. The lastAccess can not be put to the area 
				because it's impossible to determine the area corresponding 
				to inserted tiles (there may be empty areas and so on, it 
				would be rather complex to calculate these things).
			- intersect/pointquery: cache and lastAccessTiles get the new result. 
						The old access and tiles are thrown away. 
			- getTiles: it would be very inefficient to search here for smaller 
					regions, so lastAccess is made empty and lastAccessTiles 
					contains the tiles accessed (because of PersTiles and 
					automatic management of their memory, they have to be 
					kept by the MDDObjIx).
			For this reason, tiles passed by PersMDDObj are only valid until the 
			next intersect/pointQuery/insert/getTiles operation.
			Alternative implementation for getTiles - always put in the cache 
			correctly.
			Make a check about the search interval and what is in the cache (for 
			example, if cell_count < 90% cache cell count , access index).
			Such a check should also be done in lastAccessPointQuery(): if the 
			cache has more than 10 elements, it is not worth searching in it for 
			a point.
		*/

		const BaseType* cellBaseType;  
		/*@Doc:
			This is needed because the PersTile constructor expects a BaseType.
			It Should be considered to move the creation of PersTiles into the
			MDDObj class.
		*/
		
		
		IndexDS* actualIx;
		/*@Doc:
			The real index structure
		*/
		
		IxLogic_insertObject do_insertObj;
		/*@Doc:
			Function pointer to insert tile logic
		*/

		IxLogic_removeObject do_removeObj;
		/*@Doc:
			Function pointer to remove tile logic
		*/

		IxLogic_intersect do_intersect;
		/*@Doc:
			Function pointer to find tiles logic
		*/

		IxLogic_containPointQuery do_pointQuery;
		/*@Doc:
			Function pointer to find tile logic
		*/

		IxLogic_getObjects do_getObjs;
		/*@Doc:
			Function pointer to get all tiles logic
		*/
		
	#ifdef RMANBENCHMARK

		void initializeTimerPointers();
		/*@Doc:
			This code was commented out because it crashed
		*/
		
		RMTimer *pointQueryTimer;
		RMTimer *intersectTimer;
		RMTimer *getTilesTimer;
	#endif
		void initializeLogicStructure();
		/**
			{\tt actualDBIx} and {\tt cellBaseType} must be already correctly set.
			The function pointers are set according to the index type.
		*/
	
		bool _isPersistent;
		/*@Doc:
			This class is used for both persistent and tranisent objects
			To be able to distinguish whether it is persistent or transient
			this attribute is used.
		*/

		const StorageLayout& myStorageLayout;
		/*@Doc:
			Nifty object which holds information on how to store data in the database.
			It tells you which index to use, hos large a index might become and so on.
		*/
	};

#endif
