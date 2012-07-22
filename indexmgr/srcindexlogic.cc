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
 * MODULE: indexmgr
 * CLASS:   SRCIndexLogic
 *
 * COMMENTS:
 *
*/

static const char rcsiddirix[] = "@(#)dirix, SRCIndexLogic: $Id: srcindexlogic.cc,v 1.8 2002/09/19 11:38:25 hoefner Exp $";

#include <iostream>
#include <math.h>
#include "raslib/rmdebug.hh"
#include "raslib/odmgtypes.hh"
#include "indexmgr/srcindexlogic.hh"
#include "keyobject.hh"
#include "indexds.hh"
#include "relindexif/dbrcindexds.hh"
#include "storagemgr/sstoragelayout.hh"
#include "relblobif/tileid.hh"
#include "relblobif/blobtile.hh"
#include "raslib/mitera.hh"

unsigned int
SRCIndexLogic::computeNumberOfTiles(const StorageLayout& sl, const r_Minterval& mddDomain)
{
    /*
        cout << "mddDomain " << mddDomain << endl;
        cout << "mddDomain extent " << mddDomain.get_extent() << endl;
        cout << "tileConfig " << sl.getTileConfiguration() << endl;
        cout << "tileConfig extent " << sl.getTileConfiguration().get_extent() << endl;
        cout << "normalized " << temp << endl;
        cout << "cell count " << temp.cell_count() << endl;
    */
    return computeNormalizedDomain(mddDomain.get_extent(), sl.getTileConfiguration().get_extent()).cell_count();
}

r_Minterval
SRCIndexLogic::computeNormalizedDomain(const r_Point& mddDomainExtent, const r_Point& tileConfigExtent)
{
    r_Dimension theDim = mddDomainExtent.dimension();
    r_Minterval normalizedDomain(theDim);
    r_Range normalized = 0;

    for (r_Dimension dim = 0; dim < theDim; dim++)
    {
        normalized = (r_Range)(mddDomainExtent[dim]/tileConfigExtent[dim]) - 1;
        //cout << "mdd domain extent [" << dim << "]  " << mddDomainExtent[dim] << endl;
        //cout << "tile config extent [" << dim << "] " << tileConfigExtent[dim] << endl;
        //cout << "division                           " << mddDomainExtent[dim]/tileConfigExtent[dim] << endl;
        //cout << "normalized                         " << normalized << endl;
        //remove this if we support rc index with border tiles
        if ((normalized + 1)* tileConfigExtent[dim] != mddDomainExtent[dim])
        {
            //cout << "got you" << endl;
            RMInit::logOut << "SRCIndexLogic::computeNormalizedDomain() the mdd domain does not fit the tile configuration" << endl;
            throw r_Error(TILECONFIGMARRAYINCOMPATIBLE);
        }
        normalizedDomain[dim] = r_Sinterval(0, normalized);
    }
    RMDBGONCE(4, RMDebug::module_indexmgr, "SRCIndexLogic", "computeNormalizedDomain(" << mddDomainExtent << ", " << tileConfigExtent << ") " << normalizedDomain);
    return normalizedDomain;
}

r_Point
SRCIndexLogic::computeNormalizedPoint(const r_Point& toNormalize, const r_Point& tileConfigExtent, const r_Point& mddDomainOrigin)
{
    r_Dimension theDim = mddDomainOrigin.dimension();
    r_Point normalizedPoint(theDim);

    for (r_Dimension dim = 0; dim < theDim; dim++)
    {
        normalizedPoint[dim] = (r_Range)((toNormalize[dim] - mddDomainOrigin[dim])/tileConfigExtent[dim]);
    }
    RMDBGONCE(4, RMDebug::module_indexmgr, "SRCIndexLogic", "computeNormalizedPoint(" << toNormalize << ", " << tileConfigExtent << ", " << mddDomainOrigin << ") " << normalizedPoint);
    return normalizedPoint;
}

r_Minterval
SRCIndexLogic::computeDomain(const r_Point& toConvert, const r_Point& tileConfigExtent, const r_Point& mddDomainOrigin)
{
    r_Dimension theDim = mddDomainOrigin.dimension();
    r_Minterval result(theDim);
    r_Range high = 0;
    r_Range low = 0;
    r_Range offset = 0;
    r_Range toConvTemp = 0;

    for (r_Dimension dim = 0; dim < theDim; dim++)
    {
        toConvTemp = toConvert[dim];
        offset = fmod((r_Double)(toConvTemp - mddDomainOrigin[dim]), tileConfigExtent[dim]);
        low = toConvTemp - offset;
        //there has to be a check if we support border tiles.
        high = toConvTemp - offset + tileConfigExtent[dim];
        result[dim] = r_Sinterval(low, high);
    }
    RMDBGONCE(4, RMDebug::module_indexmgr, "SRCIndexLogic", "computeDomain(" << toConvert << ", " << tileConfigExtent << ", " << mddDomainOrigin << ") " << result);
    return result;
}

OId
SRCIndexLogic::computeOId(const r_Minterval& mddDomain, const r_Point& tileConfigExtent, OId::OIdCounter baseCounter, OId::OIdType type, const r_Point& tileOrigin)
{
    OId::OIdCounter counter;
    counter = computeNormalizedDomain(mddDomain.get_extent(), tileConfigExtent).cell_offset(computeNormalizedPoint(tileOrigin, tileConfigExtent, mddDomain.get_origin())) + baseCounter;
    RMDBGONCE(4, RMDebug::module_indexmgr, "SRCIndexLogic", "computeOId(" << mddDomain << ", " << tileConfigExtent << ", " << baseCounter << ", " << tileOrigin << ") " << OId(counter, type));
    return OId(counter, type);
}

bool
SRCIndexLogic::insertObject(IndexDS* ixDS, const KeyObject& newKeyObject, const StorageLayout& sl)
{
    //this method should check if the tile is actually in the tiling
    RMDBGENTER(4, RMDebug::module_indexmgr, "SRCIndexLogic", "insertObject(" << newKeyObject << ")");

    /* RMInit::logOut << "SRCIndexLogic::insertObject(" << ixDS->getIdentifier() << ", " << newKeyObject << ", sl) insert operation not allowed" << endl;
    throw r_Error(INSERTINTORCINDEX); // thrown without a check and therefore commented out in order to make rc_index work MR 29.05.2012 */
    //if src is able to extend:
    r_Minterval newKeyObjectDomain = newKeyObject.getDomain();
    r_Minterval tileConfig = sl.getTileConfiguration();
    r_Minterval mddDomain = ixDS->getCoveredDomain();
    OId newBlobOId(computeOId(mddDomain, tileConfig.get_extent(), ((DBRCIndexDS*)ixDS)->getBaseCounter(), ((DBRCIndexDS*)ixDS)->getBaseOIdType(), newKeyObjectDomain.get_origin()));
    DBTileId tile = newKeyObject.getObject();
    BLOBTile* t = new BLOBTile(tile->getSize(), tile->getCells(), tile->getDataFormat(), newBlobOId);
    // is done in the constructor
    //t->setPersistent(true);
    tile->setPersistent(false);

    RMDBGEXIT(4, RMDebug::module_indexmgr, "SRCIndexLogic", "insertObject(" << newKeyObject << ")");
    //should check if insertion was succesfull
    return true;
}

/*
r_Minterval
SRCIndexLogic::computeDomain(const r_Point& toConvert, const r_Point& tileConfigExtent, const r_Point& mddDomainOrigin)
    {
    r_Dimension theDim = mddDomainOrigin.dimension();
    r_Minterval result(theDim);
    r_Range high = 0;
    r_Range low = 0;
    r_Range offset = 0;
    r_Range toConvTemp = 0;

    for (r_Dimension dim = 0; dim < theDim; dim++)
        {
        toConvTemp = toConvert[dim];
        offset = fmod((toConvTemp - mddDomainOrigin[dim]), tileConfigExtent[dim]);
        low = toConvTemp - offset;
        high = toConvTemp - offset + tileConfigExtent[dim];
        result[dim] = r_Sinterval(low, high);
        }
    return result;
    }
*/

r_Minterval
SRCIndexLogic::computeTiledDomain(const r_Minterval& completeDomain, const r_Point& tileConfigExtent, const r_Minterval& widenMe)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SRCIndexLogic", "computeTiledDomain(" << completeDomain << ", " << tileConfigExtent << ", " << widenMe << ")");
    r_Minterval searchDomain(completeDomain.create_intersection(widenMe));
    r_Dimension theDim = searchDomain.dimension();
    r_Minterval retval(theDim);
    r_Range high = 0;
    r_Range low = 0;
    r_Range offsetlow = 0;
    r_Range offsethigh = 0;
    r_Range mddOrigin = 0;
    r_Range tileExtent = 0;
    r_Sinterval currSi;
    r_Point mddDomainOrigin = completeDomain.get_origin();
    RMDBGMIDDLE(5, RMDebug::module_indexmgr, "SRCIndexLogic", "search domain " << searchDomain << " mdd origin " << mddDomainOrigin)

    for (r_Dimension dim = 0; dim < theDim; dim++)
    {
        // make retval fit the tiling layout
        currSi = searchDomain[dim];
        low = currSi.low();
        high = currSi.high();
        mddOrigin = mddDomainOrigin[dim];
        tileExtent = tileConfigExtent[dim];
        offsetlow = (low - mddOrigin)%tileExtent;
        offsethigh = (high - mddOrigin)%tileExtent;
        //this has to be revised if we support border tiles
        retval[dim] = r_Sinterval(low - offsetlow, high - offsethigh + tileExtent - 1);
        RMDBGMIDDLE(6, RMDebug::module_indexmgr, "SRCIndexLogic", "mdd interval " << currSi << " offset low " << offsetlow << " offset high " << offsethigh << " tile extent " << tileExtent)
    }
    RMDBGEXIT(4, RMDebug::module_indexmgr, "SRCIndexLogic", "computeTiledDomain(" << completeDomain << ", " << tileConfigExtent << ", " << widenMe << ") " << retval);
    return retval;
}

void
SRCIndexLogic::intersect(const IndexDS* ixDS, const r_Minterval& searchInter, KeyObjectVector& intersectedObjs, const StorageLayout& sl)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SRCIndexLogic", "intersect(" << searchInter << ")");
    r_Minterval mddDomain = ixDS->getCoveredDomain();
    if (searchInter.intersects_with(mddDomain))
    {
        r_Minterval tileConfig = sl.getTileConfiguration();
        r_Point tileConfigExtent = tileConfig.get_extent();
        OId::OIdCounter baseCounter = ((DBRCIndexDS*)ixDS)->getBaseCounter();
        OId::OIdType type = ((DBRCIndexDS*)ixDS)->getBaseOIdType();
        r_Dimension dim = mddDomain.dimension();
        r_Point searchPoint(dim);
        r_Minterval searchDomain = computeTiledDomain(mddDomain, tileConfigExtent, searchInter);
        r_MiterArea iter(&tileConfig, &searchDomain);
        r_Minterval iterArea(dim);

        while (!iter.isDone())
        {
            //iterate through the partitions in the search domain
            iterArea = iter.nextArea();
            r_Point orig = iterArea.get_origin();
            OId t = computeOId(mddDomain, tileConfigExtent, baseCounter, type, orig);
            DBObjectId theObject(t);
            //the domain of the object has to be adapted to the border tiles
            if (!theObject.is_null())
            {
                theObject->setCached(true);
            }
            else
            {
                theObject = new BLOBTile(t, sl.getTileSize(), sl.getDataFormat(orig));
                theObject->setCached(true);
            }
            intersectedObjs.push_back(KeyObject(theObject, iterArea));
        }
    }
}

void
SRCIndexLogic::containPointQuery(const IndexDS* ixDS, const r_Point& searchPoint, KeyObject& result, const StorageLayout& sl)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SRCIndexLogic", "containPointQuery(" << searchPoint << ")");
    r_Minterval mddDomain = ixDS->getCoveredDomain();
    if (mddDomain.covers(searchPoint))
    {
        r_Minterval tileConfig = sl.getTileConfiguration();
        r_Point tileConfigExtent = tileConfig.get_extent();
        OId t = computeOId(mddDomain, tileConfigExtent, ((DBRCIndexDS*)ixDS)->getBaseCounter(), ((DBRCIndexDS*)ixDS)->getBaseOIdType(), searchPoint);
        DBObjectId theObject(t);
        if (!theObject.is_null())
        {
            theObject->setCached(true);
        }
        else
        {
            theObject = new BLOBTile(t, sl.getTileSize(), sl.getDataFormat(searchPoint));
        }
        result.setDomain(computeDomain(searchPoint, tileConfigExtent, mddDomain.get_origin()));
        result.setObject(theObject.getOId());
    }
    RMDBGEXIT(4, RMDebug::module_indexmgr, "SRCIndexLogic", "containPointQuery(" << searchPoint << ") " << result);
}

void
SRCIndexLogic::getObjects(const IndexDS* ixDS, KeyObjectVector& objs, const StorageLayout& sl)
{
    RMDBGONCE(4, RMDebug::module_indexmgr, "SRCIndexLogic", "getObjects()");
    intersect(ixDS, ixDS->getCoveredDomain(), objs, sl);
}

bool
SRCIndexLogic::removeObject(IndexDS* ixDS, const KeyObject& objToRemove, const StorageLayout& sl)
{
    RMDBGONCE(4, RMDebug::module_indexmgr, "SRCIndexLogic", "removeObject(" << objToRemove << ")");
    return true;
}

