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

#ifndef _DIRIX_HH_
#define _DIRIX_HH_

#include "reladminif/lists.h"
class r_Point;
class StorageLayout;

/***********************
 *
 * INCLUDE: dirix.hh
 *
 * MODULE:  indexmgr
 * CLASS:   SDirIndexLogic
 *
 *
 * COMMENTS:
 *
 ***********/

/**
 *  @file sdirindexlogic.hh
 *
 *  @ingroup indexmgr
 */

/*@Doc:

    The SDirIndexLogic class implements a Directory of Interval Objects Index.

    Objects inserted in the index must be disjunctive, since optimizations
    are made which assume nonoverlapping interval objects.

    It can be used as index on tiles of MDD objects.
    A directory index consists solely of the current domain and
    a list of entries, one for each interval object (for instance, a tile).
    Interval objects must not overlap, since {\tt SDirIndexLogic} optimizes access
    based on the assumption that entries don't overlap. Entries are
    sorted by the lower vertice.
    Each entry contains the interval domain and a reference to the object
    itself.

    SDirIndexLogic implements the higher level index functionality and uses the
    functionality of {\tt IndexDS} to do the operations.

    This way, {\tt SDirIndexLogic} can be used for both persistent or
    main memory indexes, tiles index or intermediate nodes of a multidimensional
    index, etc, by defining appropriate {\tt IndexDS} classes. Examples are
    {\tt TransDirIx} and {\tt DBHierIndex}, for transient and persistent
    indexes, respectively.A

    The logic classes are stack based for performance and memory reasons -> everything is static.
*/

class SDirIndexLogic
{
public:

    static bool insertObject(IndexDS* theIx, const KeyObject& newObject, const StorageLayout& sl);
    /*@Doc:
        Inserts a new object in the index.
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

    static void intersectUnOpt(const IndexDS* theIx, const r_Minterval& searchInter, KeyObjectVector& objs);
    /*@Doc:
        Search the index for a search region.
        Old unoptimized version of intersect(). Just scans all the entries,
        tests each one for intersection, returning all that intersect.
        This method is actually used for removing of tiles (a tile can be stored in multiple nodes).
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

    enum OrderPoint
    {
        Highest = 1,
        Lowest = 2,
        None = 0
    };
    /*@Doc:
    */

    static int compare( const r_Minterval& mint1,
                        const r_Minterval& mint2,
                        OrderPoint o1 = Lowest,
                        OrderPoint o2 = Lowest);
    /*@Doc:
        Compares two intervals based on two points from each one.
        Returns : -1 if mint1.point(o1) < mint2.point(o2);
        0 iff   mint1.point(o1) == mint2.point(o2) and
        1 iff   mint1.point(o1) > mint2.point(o2); where
        mint.point(o) is the lowest corner point of mint if o == Lowest,
        mint.point(o) is the highest corner point of mint if o == Highest.
    */

    static int binarySearch(    const IndexDS* theIx,
                                const r_Minterval& newDomain,
                                OrderPoint o,
                                int first,
                                int last);
    /*@Doc:
        Returns position of searched item or position before the one where
        it should be inserted to keep the order of the list (-1 means it should be
        inserted at the beginning of the list).
    */

    static int binaryPointSearch(   const IndexDS* theIx,
                                    const r_Point& pnt,
                                    OrderPoint o,
                                    int first,
                                    int last);
    /*@Doc:
        Returns position of tile having the point, -1 if point not there.
    */

    static int binaryRegionSearch(  const IndexDS* theIx,
                                    const r_Minterval& mint,
                                    r_Area& area,
                                    KeyObjectVector& intersectedObjects,
                                    int first,
                                    int last);
    /*@Doc:
        Assumes ordering according to the lowest corner of the tiles!!!
    */

};

#endif
