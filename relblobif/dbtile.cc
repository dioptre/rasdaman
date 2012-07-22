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

static const char rcsid[] = "@(#)blobif,DBTile: $Id: dbtile.cc,v 1.12 2005/09/03 20:41:40 rasdev Exp $";

#include "dbtile.hh"
#include "raslib/rmdebug.hh"
#include "reladminif/externs.h"
#include "reladminif/sqlerror.hh"
#include "raslib/error.hh"
#include "reladminif/objectbroker.hh"
#include "blobtile.hh"
#include "inlinetile.hh"
#include "reladminif/dbref.hh"
#include "debug.hh" // for ENTER/LEAVE/TALK macros

#include "unistd.h"
#include <iostream>
#include <cstring>
#include <vector>

#ifdef BASEDB_ORACLE
#include <oratypes.h>
#include <oci.h>
#include <sqlca.h>
#endif


r_Data_Format
DBTile::getDataFormat() const
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "getDataFormat() const " << myOId << " " << dataFormat);
    return dataFormat;
}

r_Data_Format
DBTile::getCurrentFormat() const
{
    return currentFormat;
}

void
DBTile::setCurrentFormat(const r_Data_Format& dataformat) const
{
    currentFormat = dataformat;
}

void
DBTile::setDataFormat(const r_Data_Format& dataformat)
{
    dataFormat = dataformat;
    setModified();
}

r_Bytes
DBTile::getMemorySize() const
{
    return size * sizeof(char) + sizeof(char*) + sizeof(r_Data_Format) + DBObject::getMemorySize() + sizeof(r_Bytes);
}

void
DBTile::setCells(char* newCells)
{
    if(cells != newCells)
    {
        if(cells != NULL)
        {
            TALK( "DBTile::setCells() freeing blob cells" );
            free(cells);
            // cells = NULL;    // added PB 2005-jan-10
        }
        cells = newCells;
        setModified();
    }
}

void
DBTile::setNoModificationData(char* newCells) const
{
    if(cells != newCells)
    {
        if(cells != NULL)
        {
            TALK( "DBTile::setNoModificationData() freeing blob cells" );
            free(cells);
            // cells = NULL;    // added PB 2005-jan-10
        }
        cells = newCells;
    }
}

void
DBTile::setNoModificationSize(r_Bytes newSize) const
{
    size = newSize;
}

char*
DBTile::getCells()
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "getCells() " << myOId);
    setModified();
    return cells;
}

const char*
DBTile::getCells() const
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "getCells() const " << myOId);
    return cells;
}

char
DBTile::getCell(r_Bytes index) const
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "getCell(" << index << ") const " << myOId);
    return cells[index];
}

r_Bytes
DBTile::getSize() const
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "getSize() const " << myOId << " " << size);
    return size;
}

void
DBTile::setCell(r_Bytes index, char newCell)
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "setCell(" << index << ", " << (int)newCell << ") " << myOId);
    setModified();
    cells[index] = newCell;
}

DBTile::DBTile(r_Data_Format dataformat)
    :   DBObject(),
        size(0),
        cells(NULL),
        dataFormat(dataformat),
        currentFormat(r_Array)
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "DBTile(" << dataFormat << ")");
    objecttype = OId::INVALID;
}

DBTile::DBTile(r_Bytes newSize, char c, r_Data_Format dataformat)
    :   DBObject(),
        size(newSize),
        cells(NULL),
        currentFormat(r_Array),
        dataFormat(dataformat)
{
    RMDBGENTER(3, RMDebug::module_blobif, "DBTile", "DBTile(" << newSize << ", '(int)" << (int) c << "', " << dataFormat << ")");

    TALK( "DBTile::DBTile() allocating " << newSize << " bytes for blob cells, previous ptr was " << (long) cells );
    cells = (char*)mymalloc(newSize * sizeof(char));
    objecttype = OId::INVALID;
    memset(cells, c, size);

    RMDBGEXIT(3, RMDebug::module_blobif, "DBTile", "DBTile(" << newSize << ", '(int)" << (int) c << "', " << dataFormat << ") " << myOId);
}

DBTile::DBTile(r_Bytes newSize, r_Bytes patSize, const char* pat, r_Data_Format dataformat)
    :   DBObject(),
        size(newSize),
        cells(NULL),
        currentFormat(r_Array),
        dataFormat(dataformat)
{
    RMDBGENTER(3, RMDebug::module_blobif, "DBTile", "DBTile(" << newSize << ", " << patSize << ", pattern, " << dataFormat << ")");
    objecttype = OId::INVALID;

    TALK( "DBTile::DBTile() allocating " << newSize << " bytes for blob cells, previous ptr was " << (long) cells );
    cells = (char*)mymalloc(newSize * sizeof(char));

    r_Bytes i = 0;
    r_Bytes j = 0;

    if (patSize >= size)
    {
        // fill cells with pattern
        for (j = 0; j < size; j++)
        {
            cells[j] = pat[j];
        }
    }
    else
    {
        if (patSize >= 0)
        {
            // fill cells with repeated pattern
            for (i = 0; i < size; i += patSize)
            {
                for (j = 0; j < patSize; j++)
                {
                    cells[(i+j)]= pat[j];
                }
            }
            // pad end with 0
            if (i != size)
            {
                // no padding necessary
                i -= patSize;
                for (; i < size; i++)
                {
                    cells[i]=0;
                }
            }
        }
        else
        {
            // fill cells with 0
            for (i = 0; i < size; i++)
            {
                cells[i] = 0;
            }
        }
    }

    RMDBGEXIT(3, RMDebug::module_blobif, "DBTile", "DBTile(" << newSize << ", " << patSize << ", pattern, " << dataFormat << ") " << myOId);
}

DBTile::DBTile(r_Bytes newSize, const char* newCells, r_Data_Format dataformat)
    :   DBObject(),
        size(newSize),
        cells(0),
        dataFormat(dataformat),
        currentFormat(r_Array)
{
    RMDBGENTER(3, RMDebug::module_blobif, "DBTile", "DBTile(" << size << ", data, " << dataFormat << ")");

    TALK( "DBTile::DBTile() allocating " << newSize << " bytes for blob cells, previous ptr was " << (long) cells );

    cells = (char*)mymalloc(size * sizeof(char));
    objecttype = OId::INVALID;
    memcpy(cells, newCells, newSize);

    RMDBGEXIT(3, RMDebug::module_blobif, "DBTile", "DBTile(" << size << ", data, " << dataFormat << ") " << myOId);
}

DBTile::DBTile(const OId& id) throw (r_Error)
    :   DBObject(id),
        size(0),
        cells(NULL),
        currentFormat(r_Array)
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "DBTile(" << id <<")");
}

DBTile::~DBTile()
{
    RMDBGENTER(3, RMDebug::module_blobif, "DBTile", "~DBTile() " << myOId);
    if (cells)
    {
        TALK( "DBTile::~DBTile() freeing blob cells" );
        free(cells);
    }
    cells = NULL;
    RMDBGEXIT(3, RMDebug::module_blobif, "DBTile", "~DBTile() " << myOId);
}

void
DBTile::resize(r_Bytes newSize)
{
    RMDBGONCE(3, RMDebug::module_blobif, "DBTile", "resize(" << newSize << ") " << myOId);
    if (size != newSize)
    {
        setModified();
        if (cells)
        {
            TALK( "DBTile::resize() freeing blob cells" );
            free(cells);
            // cells = NULL;    // added PB 2005-jan-10
        }
        TALK( "DBTile::resize() allocating " << newSize << " bytes for blob cells, previous ptr was " << (long) cells );
        cells = (char*)mymalloc(newSize * sizeof(char));
        size = newSize;
    }
}

void
DBTile::printStatus(unsigned int level, std::ostream& stream) const
{
    DBObject::printStatus(level, stream);
    stream << " r_Data_Format " << dataFormat << " size " << size << " ";
    RMDBGIF(20, RMDebug::module_blobif, "DBTile", for (int a = 0; a < size; a++)\
            stream << " " << (int)(cells[a]); stream << endl;)
    }

std::ostream&
operator << (std::ostream& stream, DBTile& b)
{
    stream << "\tDBTile at " << &b << endl;
    stream << "\t\tOId\t\t:" << b.myOId << endl;
    stream << "\t\tId\t\t:" << b.myOId.getCounter() << endl;
    stream << "\t\tSize\t\t:" << b.size << endl;
    stream << "\t\tModified\t:" << (int)b._isModified << endl;
    stream << "\t\tCells\t\t:";
    RMDBGIF(20, RMDebug::module_blobif, "DBTile", for (int a = 0; a < b.size; a++)\
            stream << " " << (int)(b.cells[a]); stream << endl;)
        return stream;
}

