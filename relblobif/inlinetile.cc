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
#include "mymalloc/mymalloc.h"
// This is -*- C++ -*-
/*************************************************************
 *
 *
 * PURPOSE:
 *

 *
 * COMMENTS:
 *   uses embedded SQL
 *
 ************************************************************/

static const char rcsid[] = "@(#)blobif,BLOBTile: $Id: inlinetile.cc,v 1.5 2002/06/15 16:47:29 coman Exp $";

#include <iostream>

#include "inlinetile.hh"
#include "raslib/rmdebug.hh"
#include "reladminif/externs.h"
#include "raslib/error.hh"
#include "reladminif/objectbroker.hh"
#include "blobtile.hh"
#include "relindexif/dbtcindex.hh"
#include "reladminif/dbref.hh"
#include "storagemgr/sstoragelayout.hh"

#include <cstring>

InlineTile::InlineTile(r_Data_Format dataformat)
    :   BLOBTile(dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "InlineTile", "InlineTile()");
    objecttype = OId::INLINETILEOID;
}

InlineTile::InlineTile(r_Bytes newSize, char c, r_Data_Format dataformat)
    :   BLOBTile(newSize, c, dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "InlineTile", "InlineTile(" << newSize << ", data)");
    objecttype = OId::INLINETILEOID;
}

InlineTile::InlineTile(r_Bytes newSize, r_Bytes patSize, const char* pat, r_Data_Format dataformat)
    :   BLOBTile(newSize, patSize, pat, dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "InlineTile", "InlineTile(" << newSize << ", " << patSize << ", pattern)");
    objecttype = OId::INLINETILEOID;
}

InlineTile::InlineTile(r_Bytes newSize, const char* newCells, r_Data_Format dataformat)
    :   BLOBTile(newSize, newCells, dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "InlineTile", "InlineTile(" << size << ", data)");
    objecttype = OId::INLINETILEOID;
}

InlineTile::InlineTile(const OId& id) throw (r_Error)
    :   BLOBTile(id)
{
    RMDBGONCE(5, RMDebug::module_blobif, "InlineTile", "InlineTile(" << id <<")");
    objecttype = OId::INLINETILEOID;
}

InlineTile::InlineTile(const OId& id, char*& thecells)
    :   BLOBTile(r_Array)
{
    RMDBGENTER(5, RMDebug::module_blobif, "InlineTile", "InlineTile(" << id << ", cells " << (r_Ptr)thecells << ")");
    objecttype = OId::INLINETILEOID;
    myIndexOId = id;

    //restore the size of the blob
    memcpy(&size, thecells, sizeof(r_Bytes));
    thecells = thecells + sizeof(r_Bytes);

    //restore the dataformat
    memcpy(&dataFormat, thecells, sizeof(r_Data_Format));
    thecells = thecells + sizeof(r_Data_Format);

    //restore my own oid
    memcpy(&myOId, thecells, sizeof(OId));
    thecells = thecells + sizeof(OId);

    //restore the cells
    cells = (char*)mymalloc(size * sizeof(char));
    memcpy(cells, thecells, size);
    thecells = thecells + size;
    RMDBGMIDDLE(5, RMDebug::module_blobif, "InlineTile", "OId " << myOId << " size " << size << " DataFormat " << dataFormat);

    DBObject::readFromDb();
    _isInDatabase = false;
    ObjectBroker::registerDBObject(this);
    RMDBGEXIT(5, RMDebug::module_blobif, "InlineTile", "InlineTile(" << id << ", cells " << (r_Ptr)thecells << ") " << myOId);
}

void
InlineTile::printStatus(unsigned int level, std::ostream& stream) const
{
    char* indent = new char[level*2 +1];
    for (unsigned int j = 0; j < level*2 ; j++)
        indent[j] = ' ';
    indent[level*2] = '\0';

    stream << indent << "InlineTile ";
    if (isInlined())
        stream << "is Inlined at " << myIndexOId << endl;
    else
        stream << "is not Inlined " << endl;
    BLOBTile::printStatus(level + 1, stream);
    delete[] indent;
}

void
InlineTile::destroy()
{
    if (isCached())
        return;
    else
        DBObject::destroy();
}

bool
InlineTile::isCached() const
{
    char retval = true;
    //not previously cached
    if (!_isCached)
        if (!isInlined())
        {
            //outlined
            if (getSize() > StorageLayout::DefaultMinimalTileSize)//size is ok
                retval = false;
        }
        else     //inlined
        {
            if (getSize() < StorageLayout::DefaultPCTMax)//size is ok
                retval = false;
        }
    return retval;
}

void
InlineTile::setModified() throw(r_Error)
{
    RMDBGENTER(4, RMDebug::module_blobif, "InlineTile", "setModified() " << myOId);
    DBObject::setModified();
    if (isInlined())
    {
        RMDBGMIDDLE(4, RMDebug::module_blobif, "InlineTile", " index will be modified");
        DBTCIndexId t(myIndexOId);
        t->setInlineTileHasChanged();
    }
    else
    {
        RMDBGMIDDLE(4, RMDebug::module_blobif, "InlineTile", "index will not be modified");
    }
    RMDBGEXIT(4, RMDebug::module_blobif, "InlineTile", "setModified() " << myOId);
}

const OId&
InlineTile::getIndexOId() const
{
    return myIndexOId;
}

void
InlineTile::setIndexOId(const OId& oid)
{
    myIndexOId = oid;
}

r_Bytes
InlineTile::getStorageSize() const
{
    return size + sizeof(r_Bytes) + sizeof(r_Data_Format) + sizeof(OId);
}

void
InlineTile::outlineTile()
{
    DBTCIndexId t(myIndexOId);
    t->removeInlineTile(this);
    setIndexOId(OId(0, OId::INVALID));
    _isInDatabase = false;
    _isModified = true;
}

void
InlineTile::inlineTile(const OId& ixId)
{
    setIndexOId(ixId);
    DBTCIndexId t(myIndexOId);
    t->addInlineTile(this);
    if (_isInDatabase)
    {
        bool pers = _isPersistent;
        bool modi = _isModified;
        BLOBTile::deleteFromDb();
        _isPersistent = pers;
        _isModified = modi;
    }
}

bool
InlineTile::isInlined() const
{
    return (myIndexOId.getType() == OId::DBTCINDEXOID);
}

char*
InlineTile::insertInMemBlock(char* thecontents)
{
    RMDBGENTER(5, RMDebug::module_blobif, "InlineTile", "insertInMemBlock(" << (void*)thecontents << ")");
    //store size of blob
    memcpy(thecontents, &size, sizeof(r_Bytes));
    thecontents = thecontents + sizeof(r_Bytes);

    //store the dataformat
    memcpy(thecontents, &dataFormat, sizeof(r_Data_Format));
    thecontents = thecontents + sizeof(r_Data_Format);

    //store my own oid
    memcpy(thecontents, &myOId, sizeof(OId));
    thecontents = thecontents + sizeof(OId);

    //store the blob
    memcpy(thecontents, cells, size * sizeof(char));
    thecontents = thecontents + size * sizeof(char);
    RMDBGMIDDLE(5, RMDebug::module_blobif, "InlineTile", "OId " << myOId << " size " << size << " DataFormat " << dataFormat);
    RMDBGIF(20, RMDebug::module_blobif, "InlineTile", for (int i = 0; i < size; i++) RMInit::dbgOut << (unsigned int)(cells[i]) << " "; RMInit::dbgOut << endl;)
        DBObject::updateInDb();
    RMDBGEXIT(5, RMDebug::module_blobif, "InlineTile", "insertInMemBlock(" << (void*)thecontents << ")");
    return thecontents;
}

InlineTile::~InlineTile()
{
}

