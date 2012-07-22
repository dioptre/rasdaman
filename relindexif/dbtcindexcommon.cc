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
#include <unistd.h>

#include "dbtcindex.hh"
#include "raslib/rmdebug.hh"
#include "reladminif/lists.h"
#include "reladminif/sqlerror.hh"
#include "reladminif/externs.h"
#include "relblobif/blobtile.hh"
#include "relblobif/inlinetile.hh"
#include "relblobif/tileid.hh"
#include "reladminif/objectbroker.hh"
#include "reladminif/adminif.hh"
#include "reladminif/dbref.hh"
#include "storagemgr/sstoragelayout.hh"
#include "indexmgr/keyobject.hh"

void
DBTCIndex::setMappingHasChanged()
{
    mappingHasChanged = true;
}

void
DBTCIndex::setInlineTileHasChanged()
{
    inlineTileHasChanged = true;
}

DBTCIndex::DBTCIndex(const OId& id)
    :   DBHierIndex(id),
        _isLoaded(false),
        inlineTileHasChanged(false),
        mappingHasChanged(false),
        hasBlob(false)
{
    RMDBGENTER(7, RMDebug::module_indexif, "DBTCIndex", "DBTCIndex(" << myOId << ")");
    throw r_Error(r_Error::r_Error_FeatureNotSupported);
    readFromDb();
    _isLoaded = !hasBlob;
    RMDBGEXIT(7, RMDebug::module_indexif, "DBTCIndex", "DBTCIndex(" << myOId << ")");
}

IndexDS*
DBTCIndex::getNewInstance() const
{
    return (HierIndexDS*)new DBTCIndex(getDimension(), !isLeaf());
}

DBTCIndex::DBTCIndex(r_Dimension dim, bool isNode)
    :   DBHierIndex(dim, isNode, false),
        _isLoaded(true),
        inlineTileHasChanged(false),
        mappingHasChanged(false),
        hasBlob(false)
{
    RMDBGENTER(7, RMDebug::module_indexif, "DBTCIndex", "DBTCIndex(" << dim << ", " << (int)_isLoaded << ") " << myOId);
    throw r_Error(r_Error::r_Error_FeatureNotSupported);
    objecttype = OId::DBTCINDEXOID;
    setPersistent(true);
    setCached(true);
    RMDBGEXIT(7, RMDebug::module_indexif, "DBTCIndex", "DBTCIndex(" << dim << ") " << myOId);
}

void
DBTCIndex::printStatus(unsigned int level, std::ostream& stream) const
{
    char* indent = new char[level*2 +1];
    for (unsigned int j = 0; j < level*2 ; j++)
        indent[j] = ' ';
    indent[level*2] = '\0';

    stream << indent << "DBTCIndex ";
    DBHierIndex::printStatus(level + 1, stream);
    delete[] indent;
}


DBTCIndex::~DBTCIndex()
{
    RMDBGENTER(7, RMDebug::module_indexif, "DBTCIndex", "~DBTCIndex() " << myOId);
    if (!isModified())
    {
        if (!AdminIf::isReadOnlyTA())
        {
            if (isLeaf())
                decideForInlining();
            if (mappingHasChanged)
                updateTileIndexMappings();
            if (inlineTileHasChanged)
            {
                storeTiles();
                changeBOIdToIOId();
            }
            if (isModified())
                DBHierIndex::updateInDb();
            if (isLeaf())
                changeIOIdToBOId();
        }
    }
    else
        validate();
    currentDbRows = 0;
    parent = OId(0);
    RMDBGEXIT(7, RMDebug::module_indexif, "DBTCIndex", "~DBTCIndex() " << myOId);
}

void
DBTCIndex::registerIOIds()
{
    RMDBGENTER(7, RMDebug::module_indexif, "DBTCIndex", "registerIOIds() " << myOId);
    for (KeyObjectVector::iterator i = myKeyObjects.begin(); i != myKeyObjects.end(); i++)
        if ((*i).getObject().getOId().getType() == OId::INNEROID)
        {
            RMDBGMIDDLE(7, RMDebug::module_indexif, "DBTCIndex", "registering tileoid " << OId((*i).getObject().getOId().getCounter(), OId::INLINETILEOID) << " indexoid " << myOId);
            ObjectBroker::registerTileIndexMapping(OId((*i).getObject().getOId().getCounter(), OId::INLINETILEOID), myOId);
            hasBlob = true;
        }
    RMDBGEXIT(7, RMDebug::module_indexif, "DBTCIndex", "registerIOIds() " << myOId);
}

void
DBTCIndex::changeIOIdToBOId()
{
    for (KeyObjectVector::iterator it = myKeyObjects.begin(); it != myKeyObjects.end(); it++)
        if ((*it).getObject().getOId().getType() == OId::INNEROID)
        {
            OId o((*it).getObject().getOId().getCounter(), OId::INLINETILEOID);
            DBObjectPPair p(o, 0);
            inlineTiles.insert(p);
            (*it).setObject(o);
        }
}

void
DBTCIndex::changeBOIdToIOId()
{
    for (KeyObjectVector::iterator it = myKeyObjects.begin(); it != myKeyObjects.end(); it++)
    {
        DBObjectPMap::iterator itit = inlineTiles.find((*it).getObject().getOId());
        if (itit != inlineTiles.end())
        {
            (*it).setObject(OId((*it).getObject().getOId().getCounter(), OId::INNEROID));
        }
    }
}

void
DBTCIndex::removeInlineTile(InlineTile* it)
{
    DBObjectPMap::iterator itit = inlineTiles.find(it->getOId());
    if (itit != inlineTiles.end())
    {
        inlineTiles.erase(itit);
        setMappingHasChanged();
        setInlineTileHasChanged();
        ObjectBroker::deregisterTileIndexMapping(it->getOId(), myOId);
    }
    else
    {
        RMDBGONCE(0, RMDebug::module_indexif, "DBTCIndex", "deregisterInlineTile(" << it->getOId() << ") it not found")
    }
}

void
DBTCIndex::addInlineTile(InlineTile* it)
{
    if (!_isLoaded)
        readInlineTiles();
    DBObjectPPair p(it->getOId(), it);
    inlineTiles.insert(p);
    ObjectBroker::registerTileIndexMapping(it->getOId(), myOId);
    setMappingHasChanged();
    setInlineTileHasChanged();
}

void
DBTCIndex::insertInDb() throw (r_Error)
{
    RMDBGENTER(5, RMDebug::module_indexif, "DBTCIndex", "insertInDb() " << myOId);
    if (isLeaf())
    {
        decideForInlining();
        if (inlineTileHasChanged || mappingHasChanged)
        {
            updateTileIndexMappings();
            insertBlob();
            storeTiles();
            changeBOIdToIOId();
        }
    }
    DBHierIndex::insertInDb();
    if (isLeaf())
        changeIOIdToBOId();
    RMDBGEXIT(5, RMDebug::module_indexif, "DBTCIndex", "insertInDb() " << myOId);
}

void
DBTCIndex::readFromDb() throw (r_Error)
{
    RMDBGENTER(5, RMDebug::module_indexif, "DBTCIndex", "readFromDb() " << myOId);
    DBHierIndex::readFromDb();
    if (isLeaf())
    {
        registerIOIds();
        changeIOIdToBOId();
    }
    inlineTileHasChanged = false;
    mappingHasChanged = false;
    RMDBGEXIT(5, RMDebug::module_indexif, "DBTCIndex", "readFromDb() " << myOId);
}

void
DBTCIndex::updateInDb() throw (r_Error)
{
    RMDBGENTER(5, RMDebug::module_indexif, "DBTCIndex", "updateInDb() " << myOId);
    if (isLeaf())
        decideForInlining();
    if (mappingHasChanged)
        updateTileIndexMappings();
    if (inlineTileHasChanged)
        storeTiles();
    if (inlineTiles.size() != 0)
        changeBOIdToIOId();
    DBHierIndex::updateInDb();
    if (isLeaf())
        changeIOIdToBOId();
    RMDBGEXIT(5, RMDebug::module_indexif, "DBTCIndex", "updateInDb() " << myOId);
}

InlineTile*
DBTCIndex::getInlineTile(const OId& itid)
{
    InlineTile* retval = 0;
    DBObjectPMap::iterator itit;
    if (!_isLoaded)
    {
        readInlineTiles();
    }
    itit = inlineTiles.find(itid);
    if (itit != inlineTiles.end())
    {
        retval = (InlineTile*)(*itit).second;
    }
    return retval;
}

void
DBTCIndex::readyForRemoval(const OId& id)
{
    if (id.getType() == OId::INLINETILEOID)
    {
        DBObjectPMap::iterator itit;

        itit = inlineTiles.find(id);
        if (inlineTiles.end() != itit)
        {
            if (!_isLoaded)
            {
                readInlineTiles();
                itit = inlineTiles.find(id);
                ((*itit).second)->setCached(false);
                ((InlineTile*)(*itit).second)->outlineTile();
            }
            else
            {
                ((*itit).second)->setCached(false);
                ((InlineTile*)(*itit).second)->outlineTile();
            }
        }
    }
}

bool
DBTCIndex::removeObject(const KeyObject& entry)
{
    RMDBGENTER(4, RMDebug::module_indexif, "DBTCIndex", "removeObject(" << entry << ") " << myOId);
    if (isLeaf())
        readyForRemoval(entry.getObject().getOId());
    bool found = DBHierIndex::removeObject(entry);
    RMDBGEXIT(4, RMDebug::module_indexif, "DBTCIndex", "removeEntry(" << entry << ") " << myOId << " " << found);
    return found;
}

bool
DBTCIndex::removeObject(unsigned int pos)
{
    RMDBGENTER(4, RMDebug::module_indexif, "DBTCIndex", "removeObject(" <<  pos << ") " << myOId);
    if (isLeaf())
        if (pos <= myKeyObjects.size())
            readyForRemoval(myKeyObjects[pos].getObject().getOId());
    bool found = DBHierIndex::removeObject((unsigned int)pos);
    RMDBGEXIT(4, RMDebug::module_indexif, "DBTCIndex", "removeEntry(" << pos << ") " << myOId << " " << found);
    return found;
}

void
DBTCIndex::decideForInlining()
{
    RMDBGENTER(5, RMDebug::module_indexif, "DBTCIndex", "decideForInlining() " << myOId);
    if (isLeaf())
    {
        InlineTile* itile = NULL;
        KeyObjectVector::iterator it;
        for (it = myKeyObjects.begin(); it != myKeyObjects.end(); it++)
        {
            RMDBGMIDDLE(5, RMDebug::module_indexif, "DBTCIndex", " we do oid " << (*it));
            if ((*it).getObject().getOId().getType() == OId::INLINETILEOID)
                if ((itile = (InlineTile*)ObjectBroker::isInMemory((*it).getObject().getOId())) != 0)
                {
                    RMDBGMIDDLE(5, RMDebug::module_indexif, "DBTCIndex", "in memory");
                    //decide for inlineing
                    if (itile->isInlined())
                    {
                        RMDBGMIDDLE(5, RMDebug::module_indexif, "DBTCIndex", "inlined");
                        if (itile->getSize() > StorageLayout::DefaultPCTMax)
                        {
                            RMDBGMIDDLE(5, RMDebug::module_indexif, "DBTCIndex", "needs to be outlined");
                            itile->outlineTile();
                        }
                    }
                    else
                    {
                        RMDBGMIDDLE(5, RMDebug::module_indexif, "DBTCIndex", "outlined");
                        if (itile->getSize() < StorageLayout::DefaultMinimalTileSize)
                        {
                            RMDBGMIDDLE(5, RMDebug::module_indexif, "DBTCIndex", "needs to be inlined");
                            itile->inlineTile(myOId);
                        }
                    }
                }
                else
                {
                    RMDBGMIDDLE(5, RMDebug::module_indexif, "DBTCIndex", "not in memory");
                }
        }
    }
    RMDBGEXIT(5, RMDebug::module_indexif, "DBTCIndex", "decideForInlining() " << myOId);
}

