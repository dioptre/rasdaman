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
 * MODULE:	indexmgr
 * CLASS:	SRCIndexLogic
 *
 * COMMENTS:
 *
*/

#ifndef _SRCINDEXLOGIC_HH_
#define _SRCINDEXLOGIC_HH_

#include "reladminif/lists.h"
#include "raslib/minterval.hh"
#include "reladminif/oidif.hh"

class r_Point;
class StorageLayout;
class SRCIndexLogic;

//@ManMemo: Module: {\bf indexmgr}
/*@Doc:

The regular computed index uses DBRCIndexDS and StorageLayout to store its persistent data.
The point is:
	The tiles all have the same layout.
	The domain of the mdd is fixed.
With those preliminaries it is possible to speed up the index considerably.
DBRCIndexDS stores the domain of the mdd object, the first OIdCounter and the OIdType of the entries.
The storage layout stores the tile configuration.

An algorithm computes the oid of a tile using the complete domain and the tile configuration.

The number of allocated oids has to be stored in the DBRCIndexDS because the DBRCIndexDS has to delete
all tiles belonging to it without information on the tile configuration (an hence cannot compute its
size).

*/

class SRCIndexLogic
	{
	public:

		static unsigned int computeNumberOfTiles(const StorageLayout& sl, const r_Minterval& mddDomain);
		/*@Doc:
			Compute the number of tiles that will have to be allocated by the index.  This is based on the tile config from storagelayout and the domain covered by the mdd.
		*/

		static bool insertObject(IndexDS* theIx, const KeyObject& newObject, const StorageLayout& sl);
		/*@Doc:
			Inserts a new object in the index.  Creates a new tile which has the oid that belongs to that domain.  The original tile is deleted from database.
		*/
		
		static bool removeObject(IndexDS* theIx, const KeyObject& tileToRemove, const StorageLayout& sl);
		/*@Doc:
			Removes the tile from the object. 
		*/
		
		static void intersect(const IndexDS* theIx, const r_Minterval& searchInter, KeyObjectVector& objs, const StorageLayout& sl);
		/*@Doc:
			Search the index for a search region. 
			Determines all the tiles in the index which intersect a given 
			search interval (given by {\tt searchInter}). 
			The memory space allocated by this function for the contents
			of the keyobjects in the returned vector (only) must be released
			afterwards by the caller.
		*/
		
		static void containPointQuery(const IndexDS* theIx, const r_Point& searchPoint, KeyObject& result, const StorageLayout& sl);
		/*@Doc:
			Passes a pointer to the searched item. 
			Memory is for the KeyObject is not to be released by the caller.
		*/
		
		static void getObjects(const IndexDS* ixDS, KeyObjectVector& objs, const StorageLayout& sl);
		/*@Doc:
			Returns all the tiles belonging to the object.
		*/

	protected:

		static r_Minterval computeNormalizedDomain(const r_Point& mddDomainExtent, const r_Point& tileConfigExtent);
		/*@Doc:
			compute the normalized matrix from the extent of the domain and the extent of the tile config.
		*/

		static r_Point computeNormalizedPoint(const r_Point& toNormalize, const r_Point& tileConfigExtent, const r_Point& mddDomainOrigin);
		/*@Doc:
			compute the normalized point from the point, the extent of the tile config and the origin of the mdd domain.
			this point is then used to calculate the number of the tile in the normailzed matrix.
		*/

		static r_Minterval computeDomain(const r_Point& toConvert, const r_Point& tileConfigExtent, const r_Point& mddDomainOrigin);
		/*@Doc:
			compute the tile domain based on the point inside that domain, the tile config extent and the origin of the mdd.
		*/

		static OId computeOId(const r_Minterval& mddDomain, const r_Point& tileConfigExtent, OId::OIdCounter baseCounter, OId::OIdType type, const r_Point& tileOrigin);
		/*@Doc:
			compute the oid of an entry based on the domain of the mdd, the extent of the tile config, the starting oid adress, the type of oid and the origin of the tile domain.
		*/

		static r_Minterval computeTiledDomain(const r_Minterval& completeDomain, const r_Point& tileConfigExtent, const r_Minterval& widenMe);
		/*@Doc:
			compute the completely tiled domain, which is the intersection of the complete domain and the domain to be tiled (widenMe), based on the tile config.
			Example: user requests tiles in domain [3:5].  the tile config is [1], the complete domain is [2:7].  the result will be [2:5] because [3:3] belongs to the tile [2:3].
		*/

	};

#endif
