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
 * SOURCE: dirix.cc
 *
 * MODULE: indexmgr
 * CLASS:   SDirIndexLogic
 *
 * COMMENTS:
 *
*/
static const char rcsiddirix[] = "@(#)dirix, SDirIndexLogic: $Id: sdirindexlogic.cc,v 1.10 2002/06/15 18:27:49 coman Exp $";

#include <iostream>
#include "raslib/rmdebug.hh"
#include "indexmgr/sdirindexlogic.hh"
#include "keyobject.hh"
#include "indexds.hh"


bool
SDirIndexLogic::insertObject(IndexDS* ixDS, const KeyObject& newKeyObject, const StorageLayout& sl)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "insertObject(" << newKeyObject << ")");
    r_Minterval newKeyObjectDomain = newKeyObject.getDomain();

    int pos = binarySearch(ixDS, newKeyObjectDomain, Lowest, 0, (int)ixDS->getSize()-1);
    ixDS->insertObject(newKeyObject, pos + 1);
    RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "insertObject(" << newKeyObject << ")");
    //should check if insertion was succesfull
    return true;
}

int
SDirIndexLogic::binarySearch(   const IndexDS* ixDS,
                                const r_Minterval& newDomain,
                                OrderPoint o,
                                int first,
                                int last)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "binarySearch(" << newDomain << ", " << (int)o << ", " << first <<", " << last << ")");
    int retval = 0;
    int middle = 0;
    int compResult = 0;

    if (first > last)
        retval = last;
    else
    {
        middle = (last + first) /2;
        compResult = compare(newDomain, ixDS->getObjectDomain(middle), o , o);
        if (compResult < 0)
            retval = binarySearch(ixDS, newDomain, o, first, middle - 1);
        else if (compResult > 0)
            retval = binarySearch(ixDS, newDomain, o, middle + 1, last);
        else
            retval = middle;
    }
    RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "binarySearch(" << newDomain << ", " << (int)o << ", " << first <<", " << last << ") " << retval);
    return retval;
}


int
SDirIndexLogic::binaryPointSearch(  const IndexDS* ixDS,
                                    const r_Point& pnt,
                                    SDirIndexLogic::OrderPoint o,
                                    int first,
                                    int last)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "binaryPointSearch(" << pnt << ", " << (int)o << ", " << first << ", " << last << ")");
    int retval = 0;
    int middle = 0;
    int compResult = 0;
    r_Minterval KeyObjectDomain;
    r_Point pnt2;

    if (first > last)
        retval = -1;
    else
    {
        middle = (last + first) /2;
        KeyObjectDomain = ixDS->getObjectDomain(middle);
        if (KeyObjectDomain.covers(pnt) == 1)
            retval = middle;
        else
        {
            switch(o)
            {
            case Highest:
                pnt2 = KeyObjectDomain.get_high();
                break;
            case Lowest:
                pnt2 = KeyObjectDomain.get_origin();
                break;
            case None:
                RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "binaryPointSearch(...) o is None");
                break;
            }

            compResult = pnt.compare_with(pnt2);
            RMDBGMIDDLE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "binaryPointSearch compResult " << compResult);

            if (compResult > 0 && o == Highest)
                retval = binaryPointSearch(ixDS, pnt, o, middle+1, last);
            else if (compResult < 0 && o == Lowest)
                retval = binaryPointSearch(ixDS, pnt, o, first, middle -1);
            else
            {
                compResult = binaryPointSearch(ixDS, pnt, o, middle +1, last);
                if (compResult < 0)
                    retval = binaryPointSearch(ixDS, pnt, o, first, middle -1);
                else
                    retval = compResult;
            }
        }
    }
    RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "binaryPointSearch(" << pnt << ", " << (int)o << ", " << first << ", " << last << ") " << retval);
    return retval;
}


int
SDirIndexLogic::binaryRegionSearch( const IndexDS* ixDS,
                                    const r_Minterval& mint,
                                    r_Area& area,
                                    KeyObjectVector& intersectedObjects,
                                    int first,
                                    int last)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "binaryRegionSearch(" << mint << ", " << area << ", vector, " << first << ", " << last << ")");
    int retval = 0;
    int middle = 0;
    r_Minterval t;
    int inc = 0;
    int ix = 0;
    r_Minterval objDomain;
    int compResult = 0;
    KeyObject newObj;
    r_Minterval intersectedRegion;
    // assumes order according to the lowest corner of the objects
    if (first > last)
        retval = -1;
    else
    {
        middle = (last + first) / 2;
        t = ixDS->getObjectDomain(middle);
        if (mint.get_high().compare_with(t.get_origin()) < 0)
        {
            // R.hi < tile.lo  no tiles after this one
            retval = binaryRegionSearch(ixDS, mint, area, intersectedObjects, first, middle - 1);
        }
        else
        {
            if (t.get_high().compare_with(mint.get_origin()) < 0)
            {
                retval = binaryRegionSearch(ixDS, mint, area, intersectedObjects, middle + 1, last);
                if (area > 0)
                    retval = binaryRegionSearch(ixDS, mint, area, intersectedObjects, first, middle - 1);
            }
            else
            {
                inc = 1;
                ix = middle;
                //starting to search forward, starting in the middle
                while (true)
                {
                    objDomain = ixDS->getObjectDomain(ix);
                    compResult = mint.get_high().compare_with(objDomain.get_origin());
                    RMDBGMIDDLE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "position " << ix << " last " << last << " incrementor " << inc << " object domain " << objDomain << " compare " << compResult);
                    // object intersects region
                    if (objDomain.intersects_with(mint))
                    {
                        intersectedRegion = objDomain;
                        intersectedRegion.intersection_with(mint);
                        area = area - intersectedRegion.cell_count();
                        newObj = ixDS->getObject(ix);
                        intersectedObjects.push_back(newObj);
                        RMDBGMIDDLE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "added one entry, intersected region " << intersectedRegion << " area left " << area);
                    }
                    if (inc != -1 && (ix == last || compResult < 0))
                    {
                        RMDBGMIDDLE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "starting again at middle, but going backwards");
                        ix = middle;
                        inc = -1;
                    }
                    if (ix == first && inc == -1)//not needed:||first == last
                    {
                        RMDBGMIDDLE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "breaking loop, arrived at start");
                        retval = ix;
                        break;
                    }
                    if (area <= 0)// || first == last || ix == first)
                    {
                        RMDBGMIDDLE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "breaking loop, area is found");
                        retval = ix;
                        break;
                    }
                    ix += inc;
                }
            }
        }
    }

    RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "binaryRegionSearch(" << mint << ", " << area << ", vector, " << first << ", " << last << ") " << retval);
    return retval;
}


int
SDirIndexLogic::compare(const r_Minterval& mint1,
                        const r_Minterval& mint2,
                        OrderPoint o1,
                        OrderPoint o2)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "compare(" << mint1 << ", " << mint2 << ", " << (int)o1 << ", " << (int)o2 << ")");
    r_Point point1,point2;
    switch(o1)
    {
    case Highest:
        point1 = mint1.get_high();
        break;
    case    Lowest:
        point1 = mint1.get_origin();
        break;
    case None:
        RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "compare(...) o1 is None");
        break;
    }
    switch(o2)
    {
    case Highest:
        point2 = mint2.get_high();
        break;
    case    Lowest:
        point2 = mint2.get_origin();
        break;
    case None:
        RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "compare(...) o2 is None");
        break;
    }
    RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "compare(" << mint1 << ", " << mint2 << ", " << (int)o1 << ", " << (int)o2 << ") " << point1.compare_with(point2));
    return point1.compare_with(point2);
}

void
SDirIndexLogic::intersect(const IndexDS* ixDS, const r_Minterval& searchInter, KeyObjectVector& intersectedObjs, const StorageLayout& sl)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "intersect(" << searchInter << ")");
    r_Area area = 0;
    int result = 0;
    r_Minterval intersectArea(searchInter.dimension());
    r_Minterval currDom(ixDS->getCoveredDomain());
    // avoid exceptions from r_Minterval
    if (!searchInter.intersects_with(currDom))
    {
        RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "intersect(" << searchInter << ") search interval does not intersect wit current domain " << currDom);
    }
    else
    {
        // Optimization: no need to search the whole area.
        // only the area which is intersected by the current domain.
        intersectArea.intersection_of(searchInter, currDom);
        area = intersectArea.cell_count();
        RMDBGMIDDLE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "Area = " << area);
        result = binaryRegionSearch(ixDS, intersectArea, area, intersectedObjs, 0, (int) ixDS->getSize() - 1);
        RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "intersect(" << searchInter << ") vectorsize: " << intersectedObjs.size());
    }
}

void
SDirIndexLogic::intersectUnOpt(const IndexDS* ixDS, const r_Minterval& searchInter, KeyObjectVector& intersectedObjs)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "intersectUnOpt(" << searchInter << ")");
    for(int i=0; i< ixDS->getSize(); i++)
    {
        r_Minterval objInterval = ixDS->getObjectDomain(i);
        if (searchInter.intersects_with(objInterval))
        {
            KeyObject obj = ixDS->getObject(i);
            intersectedObjs.push_back(obj);
        }
    }
    RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "intersectUnOpt(" << searchInter << ") vectorsize: " << intersectedObjs.size());
}

void
SDirIndexLogic::containPointQuery(const IndexDS* ixDS, const r_Point& searchPoint, KeyObject& result, const StorageLayout& sl)
{
    RMDBGENTER(4, RMDebug::module_indexmgr, "SDirIndexLogic", "containPointQuery(" << searchPoint << ")");
    int ix = binaryPointSearch(ixDS, searchPoint, Lowest, 0, (int) ixDS->getSize() - 1);
    RMDBGMIDDLE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "result from binaryPointSearch ix " << ix);

    if (ix >= 0)
    {
        result = ixDS->getObject(ix);
        RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "containPointQuery(" << searchPoint << ") " << result);
    }
    else
    {
        RMDBGEXIT(4, RMDebug::module_indexmgr, "SDirIndexLogic", "containPointQuery(" << searchPoint << ") nothing found ");
    }
}

void
SDirIndexLogic::getObjects(const IndexDS* ixDS, KeyObjectVector& objs, const StorageLayout& sl)
{
    RMDBGONCE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "getObjects()");
    ixDS->getObjects(objs);
}

bool
SDirIndexLogic::removeObject(IndexDS* ixDS, const KeyObject& objToRemove, const StorageLayout& sl)
{
    RMDBGONCE(4, RMDebug::module_indexmgr, "SDirIndexLogic", "removeObject(" << objToRemove << ")");
    return ixDS->removeObject(objToRemove);
}

