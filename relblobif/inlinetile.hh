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
#ifndef _INLINETILE_HH_
#define _INLINETILE_HH_
// -*-C++-*- (for Emacs)

/*************************************************************
 *
 *
 * PURPOSE:
 *   The inlinetile class is used to store small tiles efficiently.
 *   Potentially many inlinetiles are grouped together in a blob and
 *   stored in the database. highly dependend on DBTCIndex.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

class OId;
class r_Error;

#include "raslib/mddtypes.hh"
#include "tileid.hh"
#include "blobtile.hh"

//@ManMemo: Module: {\bf relblobif}.

/*@Doc:

InlineTile is the persistent class for storing the contents of MDD tiles
in the database.  it can be stored as a blobtile or inlined:
in inlined mode multiple inlinetiles are stored as one blob in the database.
memory management and modification management is critical.
there are special functions in objectbroker to retrieve inlinetiles.
they can only be inlined by a dbtcindex.

*/

/**
  * \ingroup Relblobifs
  */
class InlineTile    : public BLOBTile
{
public:

    //@Man: constructors
    //@{
    InlineTile(const OId& id, char*& thecells);
    /*@Doc:
        construct a new inline tile with the oid of
        the dbtilecontainerindex and the array which
        holds the contents of the tile.
        thecells will be automagically forwarded to the beginning of the next inline tile.
    */

    InlineTile(r_Data_Format dataformat = r_Array);
    /*@Doc:
    constructs a new empty InlineTile and gets an id for it.
    */

    InlineTile(const OId& BlobId) throw (r_Error);
    /*@Doc:
    constructs a InlineTile out of the database
    */

    InlineTile(r_Bytes newSize, char c = 0, r_Data_Format dataformat = r_Array);
    /*@Doc:
    constructs a new InlineTile of size newSize filled with c.
    */

    InlineTile(r_Bytes newSize, r_Bytes patSize, const char* pat, r_Data_Format dataformat = r_Array);
    /*@Doc:
     Constructs a new InlineTile of size newSize filled with the repeated
     char array pat of size patSize. If after filling some chars are
     left, they are filled with 0
    */
    /*@ManMemo: constructs a new InlineTile with the char array newCells
            with newSize elements as contents. */

    InlineTile(r_Bytes newSize, const char* newCells, r_Data_Format dataformat = r_Array);
    /*@Doc:
    constructs a new InlineTile of size newSize filled with the contents of newCells.
    */

    //@}
    virtual void destroy();
    /*@Doc:
    may not destroy the object because it is inlined and therefore depending on its parent index.
    */

    const OId& getIndexOId() const;
    /*@Doc:
    returns the oid of the index which contains the inlined tile.  if the tile is outlined then this oid is invalid.
    */

    void setIndexOId(const OId& oid);
    /*@Doc:
    make the inlinetile use this index as its parent and storage structure.
    */

    r_Bytes getStorageSize() const;
    /*@Doc:
    returns the size this tile will consume in as an inlined array.
    */

    virtual char* insertInMemBlock(char* test);
    /*@Doc:
        inserts the Blob into the char.
        the returned pointer is after the end of this tiles data.
    */

    virtual void setModified() throw(r_Error);
    /*@Doc:
        does not only set itself modified but also informs its parent of changes.
    */

    virtual bool isCached() const;
    /*@Doc:
        returns true if it is inlined.
    */

    virtual void inlineTile(const OId& ixOId);
    /*@Doc:
        do everything so that this tile is inlined and uses ixOId as its index parent.
        it will not check if this tile is already inlined.
    */

    virtual void outlineTile();
    /*@Doc:
        does everything necessary to act as a blobtile:
        remove it from the index parent.
    */

    virtual bool isInlined() const;
    /*@Doc:
        checks if it has a valid index parent.
    */

    virtual ~InlineTile();
    /*@Doc:
        no functionality.  if it is inlined the dbtcindex will take care of storing it.
        if it is not inlined the blobtile functionality will take over.
    */

    virtual void printStatus(unsigned int level = 0, std::ostream& stream = std::cout) const;

protected:

    OId myIndexOId;
    /*@Doc:
        when this inlinetile is in inlined mode the myIndexOId points to the parent index.
        if this oid is invalid the inlinetile is not in inline mode.
    */

};

#endif
