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
 * PURPOSE
 *  has common code for all database interface implementations
 */

#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <cstring>

#include "blobtile.hh"

#include "raslib/error.hh"
#include "reladminif/externs.h"
#include "raslib/rmdebug.hh"
#include "reladminif/sqlerror.hh"
#include "tileid.hh"
#include "inlinetile.hh"
#include "reladminif/objectbroker.hh"
#include "reladminif/dbref.hh"

// defined in rasserver.cc
extern char globalConnectId[256];

BLOBTile::BLOBTile(r_Data_Format dataformat)
    :   DBTile(dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "BLOBTile", "BLOBTile(" << dataformat << ")");
    objecttype = OId::BLOBOID;
}

/*************************************************************
 * Method name...: BLOBTile(r_Bytes newSize, char c)
 *
 * Arguments.....:
 *   newSize: size in number of chars
 *   c:       value for all cells
 * Return value..: none
 * Description...: creates a new BLOBTile containing with all
 *                 cells set to c.
 ************************************************************/

BLOBTile::BLOBTile(r_Bytes newSize, char c, r_Data_Format dataformat)
    :   DBTile(newSize, c, dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "BLOBTile", "BLOBTile(" << newSize << ", data, " << dataformat << ")");
    objecttype = OId::BLOBOID;
}


/*************************************************************
 * Method name...:   BLOBTile(r_Bytes newSize,
 *                             int patSize, char* pat);
 *
 * Arguments.....:
 *   newSize: size in number of chars
 *   patSize: number of chars in pattern
 *   pat:     char array with the pattern
 * Return value..: none
 * Description...: creates a new BLOBTile containing the
 *                 repeated pattern pat. newSize shoud be
 *                 a multiply of patSize, otherwise the
 *                 cells are filled up with 0.
 ************************************************************/

BLOBTile::BLOBTile(r_Bytes newSize, r_Bytes patSize, const char* pat, r_Data_Format dataformat)
    :   DBTile(newSize, patSize, pat, dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "BLOBTile", "BLOBTile(" << newSize << ", " << patSize << ", pattern, " << dataformat << ")");
    objecttype = OId::BLOBOID;
}

/*************************************************************
 * Method name...: BLOBTile(r_Bytes newSize,
 *                           char* newCells)
 *
 * Arguments.....:
 *   newSize:  size in number of chars
 *   newCells: char array with the new cells (must
 *             have at least newSize elements)
 * Return value..: none
 * Description...: creates a new BLOBTile. The elements of
 *                 the char array are copied!
 ************************************************************/

BLOBTile::BLOBTile(r_Bytes newSize, const char* newCells, r_Data_Format dataformat)
    :   DBTile(newSize, newCells, dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "BLOBTile", "BLOBTile(" << size << ", data, " << dataformat << ")");
    objecttype = OId::BLOBOID;
}

BLOBTile::BLOBTile(r_Bytes newSize, const char* newCells, r_Data_Format dataformat, const OId& id)
    :   DBTile(newSize, newCells, dataformat)
{
    RMDBGONCE(3, RMDebug::module_blobif, "BLOBTile", "BLOBTile(" << size << ", data, " << dataformat << ", " << id << ")");
    objecttype = OId::BLOBOID;
    myOId = id;
    // copied from DBObject::setPersistent()
    // if we don't do this the blob will get a new oid
    _isPersistent = true;
    _isModified = true;
    ObjectBroker::registerDBObject(this);
}

BLOBTile::BLOBTile(const OId& id) throw (r_Error)
    :   DBTile(id)
{
    RMDBGENTER(3, RMDebug::module_blobif, "BLOBTile", "BLOBTile(" << id <<")");
    readFromDb();
    RMDBGEXIT(3, RMDebug::module_blobif, "BLOBTile", "BLOBTile(" << id << ")");
}

BLOBTile::BLOBTile(const OId& id, r_Bytes newSize, r_Data_Format newFmt)
    :   DBTile(id)
{
    _isInDatabase = false;
    _isPersistent = true;
    _isModified = false;
    _isCached = false;
    dataFormat = newFmt;
    currentFormat = r_Array;
    size = newSize;
    cells = (char*)mymalloc(size * sizeof(char));
    memset(cells, 0, size);
    ObjectBroker::registerDBObject(this);
}

BLOBTile::~BLOBTile()
{
    RMDBGENTER(3, RMDebug::module_blobif, "BLOBTile", "~BLOBTile() " << myOId);
    validate();
    RMDBGEXIT(3, RMDebug::module_blobif, "BLOBTile", "~BLOBTile() " << myOId << ")");
}

char*   BLOBTile::BLOBBuffer = NULL;
r_Bytes BLOBTile::BLOBBufferLength = 131072;

