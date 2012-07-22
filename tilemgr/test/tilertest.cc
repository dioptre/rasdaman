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
#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "server/template_inst.hh"
#endif
#endif

#include "raslib/rmdebug.hh"
#include "raslib/minterval.hh"
#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector.h>
#include <pair.h>
#include "tilemgr/transtile.hh"
#include "octettype.hh"
#include "tilemgr/tiler.hh"

RMINITGLOBALS('C')


int
main(int argc, char *argv[])
{
    vector<r_Minterval> sourceDomains;
    vector<r_Minterval> targetDomains;
    vector<r_Minterval> retvalDomains;
    vector<r_Minterval>::iterator sourceDIt;
    vector<r_Minterval>::iterator targetDIt;
    vector<r_Minterval>::iterator retvalDIt;


    vector<Tile*>::iterator sourceIt;
    vector<Tile*>::iterator targetIt;
    vector<Tile*>::iterator retvalIt;

    r_Minterval targetDom("[0:50,0:50]");
    r_Minterval targetInt1("[0:10,0:10]");
    r_Minterval targetInt2("[15:25,10:20]");
    r_Minterval targetInt3("[15:25,21:30]");
    r_Minterval targetInt4("[15:25,31:40]");
    r_Minterval targetInt5("[40:50,40:50]");
    r_Minterval targetInt6("[35:41,25:35]");
    r_Minterval targetInt7("[28:32,28:32]");
    r_Minterval targetInt8("[40:40,0:10]");
    r_Minterval targetInt9("[40:50,11:11]");
    r_Minterval targetInt10("[35:35,0:10]");
    r_Minterval targetInt11("[30:34,10:10]");
    OctetType* o = new OctetType();

    char* cont1 = 0;
    char* cont2 = 0;
    char* cont3 = 0;
    char* cont4 = 0;
    char* cont5 = 0;
    char* cont6 = 0;
    char* cont7 = 0;
    char* cont8 = 0;
    char* cont9 = 0;
    char* cont10 = 0;
    char* cont11 = 0;
    char* outc = 0;
    cont1 = (char*)mymalloc(500);
    cont2 = (char*)mymalloc(500);
    cont3 = (char*)mymalloc(500);
    cont4 = (char*)mymalloc(500);
    cont5 = (char*)mymalloc(500);
    cont6 = (char*)mymalloc(500);
    cont7 = (char*)mymalloc(500);
    cont8 = (char*)mymalloc(500);
    cont9 = (char*)mymalloc(500);
    cont10 = (char*)mymalloc(500);
    cont11 = (char*)mymalloc(500);
    outc = (char*)mymalloc(101*101);
    memset(cont1, 1, 500);
    memset(cont2, 2, 500);
    memset(cont3, 3, 500);
    memset(cont4, 4, 500);
    memset(cont5, 5, 500);
    memset(cont6, 6, 500);
    memset(cont7, 7, 500);
    memset(cont8, 8, 500);
    memset(cont9, 9, 500);
    memset(cont10, 10, 500);
    memset(cont11, 11, 500);
    memset(outc, 0, 3500);
    TransTile output(targetDom, o, outc);

    TransTile* ttile1 = new TransTile(targetInt1, (const BaseType*)o, cont1);
    TransTile* ttile2 = new TransTile(targetInt2, (const BaseType*)o, cont2);
    TransTile* ttile3 = new TransTile(targetInt3, (const BaseType*)o, cont3);
    TransTile* ttile4 = new TransTile(targetInt4, (const BaseType*)o, cont4);
    TransTile* ttile5 = new TransTile(targetInt5, (const BaseType*)o, cont5);
    TransTile* ttile6 = new TransTile(targetInt6, (const BaseType*)o, cont6);
    TransTile* ttile7 = new TransTile(targetInt7, (const BaseType*)o, cont7);
    TransTile* ttile8 = new TransTile(targetInt8, (const BaseType*)o, cont8);
    TransTile* ttile9 = new TransTile(targetInt9, (const BaseType*)o, cont9);
    TransTile* ttile10 = new TransTile(targetInt10, (const BaseType*)o, cont10);
    TransTile* ttile11 = new TransTile(targetInt11, (const BaseType*)o, cont11);

    r_Minterval sourceDom("[5:45,5:45]");
    r_Minterval sourceInt1("[5:10,5:10]");
    r_Minterval sourceInt2("[15:25,12:45]");
    r_Minterval sourceInt3("[30:45,30:38]");
    r_Minterval sourceInt4("[40:45,40:45]");
    r_Minterval sourceInt5("[30:45,25:28]");
    r_Minterval sourceInt6("[39:41,9:11]");
    r_Minterval sourceInt7("[34:36,9:11]");

    char* scont1 = 0;
    char* scont2 = 0;
    char* scont3 = 0;
    char* scont4 = 0;
    char* scont5 = 0;
    char* scont6 = 0;
    char* scont7 = 0;
    scont1 = (char*)mymalloc(500);
    scont2 = (char*)mymalloc(500);
    scont3 = (char*)mymalloc(500);
    scont4 = (char*)mymalloc(500);
    scont5 = (char*)mymalloc(500);
    scont6 = (char*)mymalloc(500);
    scont7 = (char*)mymalloc(500);
    memset(scont1, 21, 500);
    memset(scont2, 22, 500);
    memset(scont3, 23, 500);
    memset(scont4, 24, 500);
    memset(scont5, 25, 500);
    memset(scont6, 26, 500);
    memset(scont7, 27, 500);
    TransTile* stile1 = new TransTile(sourceInt1, (const BaseType*)o, scont1);
    TransTile* stile2 = new TransTile(sourceInt2, (const BaseType*)o, scont2);
    TransTile* stile3 = new TransTile(sourceInt3, (const BaseType*)o, scont3);
    TransTile* stile4 = new TransTile(sourceInt4, (const BaseType*)o, scont4);
    TransTile* stile5 = new TransTile(sourceInt5, (const BaseType*)o, scont5);
    TransTile* stile6 = new TransTile(sourceInt6, (const BaseType*)o, scont6);
    TransTile* stile7 = new TransTile(sourceInt7, (const BaseType*)o, scont7);

    vector<Tile*> sourceTiles;
    sourceTiles.push_back(stile1);
    sourceTiles.push_back(stile2);
    sourceTiles.push_back(stile3);
    sourceTiles.push_back(stile4);
    sourceTiles.push_back(stile5);
    sourceTiles.push_back(stile6);
    sourceTiles.push_back(stile7);

    vector<Tile*> targetTiles;
    targetTiles.push_back(ttile1);
    targetTiles.push_back(ttile2);
    targetTiles.push_back(ttile3);
    targetTiles.push_back(ttile4);
    targetTiles.push_back(ttile5);
    targetTiles.push_back(ttile6);
    targetTiles.push_back(ttile7);
    targetTiles.push_back(ttile8);
    targetTiles.push_back(ttile9);
    targetTiles.push_back(ttile10);
    targetTiles.push_back(ttile11);

    vector<Tile*> retval;

    unsigned long targetTileArea = 0;
    unsigned long sourceTileArea = 0;
    unsigned long targetTileDomain = 0;
    unsigned long updatedArea = 0;
    bool intersection = false;

    r_Minterval intersectUpdateDomain;

    bool computed = true;

    cout << "Target Tiles" << endl;
    for (targetIt = targetTiles.begin(); targetIt != targetTiles.end(); targetIt++)
        cout << (*targetIt)->getDomain() << endl;
    cout << endl << "Source Tiles" << endl;
    for (sourceIt = sourceTiles.begin(); sourceIt != sourceTiles.end(); sourceIt++)
        cout << (*sourceIt)->getDomain() << endl;
    cout << endl;
    cout << "Checking if only update, insert or update and insert" << endl;

    for (sourceIt = sourceTiles.begin(); sourceIt != sourceTiles.end(); sourceIt++)
    {
        intersection = false;
        sourceTileArea = sourceTileArea + (*sourceIt)->getDomain().cell_count();
        cout << "SourceDomain " << (*sourceIt)->getDomain() << " source area is " << sourceTileArea << endl;
        for (targetIt = targetTiles.begin(); targetIt != targetTiles.end(); targetIt++)
        {
            if (computed)
            {
                targetTileArea = targetTileArea + (*targetIt)->getDomain().cell_count();
                cout << "Target Tile " << (*targetIt)->getDomain() << " area " << targetTileArea << endl;
            }

            if( (*sourceIt)->getDomain().intersects_with( (*targetIt)->getDomain() ) )
            {
                cout << "Intersection" << endl;
                intersection = true;
                intersectUpdateDomain = (*sourceIt)->getDomain().create_intersection( (*targetIt)->getDomain() );
                cout << "Updated " << intersectUpdateDomain << " on source " << (*sourceIt)->getDomain() << " and target " << (*targetIt)->getDomain() << endl;
                updatedArea = updatedArea + intersectUpdateDomain.cell_count();
                (*targetIt)->execUnaryOp(Ops::OP_IDENTITY, intersectUpdateDomain, *sourceIt, intersectUpdateDomain );
            }
        }
        if (!intersection)
        {
            // Create a new persistent tile, copy the transient data,
            // and insert it into the target mdd object.
            cout << "No Intersection" << endl;
            TransTile* newPersTile = new TransTile((*sourceIt)->getDomain(), o, (*sourceIt)->getDataFormat());
            newPersTile->execUnaryOp(Ops::OP_IDENTITY, (*sourceIt)->getDomain(), *sourceIt, (*sourceIt)->getDomain());
            //targetObj->insertTile( newPersTile );
            updatedArea = updatedArea + (*sourceIt)->getDomain().cell_count();
        }
        computed = false;
    }
    cout << "Source Domain    " << sourceDom << " has area " << sourceDom.cell_count() << endl;
    cout << "Source Tile Area " << sourceTileArea << endl;
    cout << "Target Tile Area " << targetTileArea << endl;
    cout << "Updated Area     " << updatedArea << endl << endl;
    // insert the tile

    if (sourceTileArea <= updatedArea)
    {
        cout << "and all sources were used" << endl;
    }
    else
    {
        cout << "and there are still some cells to do" << endl << endl;

        for (retvalIt = sourceTiles.begin(); retvalIt != sourceTiles.end(); retvalIt++)
        {
            sourceDomains.push_back((*retvalIt)->getDomain());
        }

        for (retvalIt = targetTiles.begin(); retvalIt != targetTiles.end(); retvalIt++)
        {
            targetDomains.push_back((*retvalIt)->getDomain());
        }

        r_Tiler tiler(sourceDomains, targetDomains);
        tiler.split();
        tiler.removeCoveredDomains();
        tiler.removeDoubleDomains();
        retvalDomains = tiler.getTiledDomains();
        cout << "we have now" << endl;
        for (retvalDIt = retvalDomains.begin(); retvalDIt != retvalDomains.end(); retvalDIt++)
            cout << (*retvalDIt) << endl;
        tiler.mergeDomains();
        retvalDomains = tiler.getTiledDomains();
        cout << "we have now" << endl;
        for (retvalDIt = retvalDomains.begin(); retvalDIt != retvalDomains.end(); retvalDIt++)
            cout << (*retvalDIt) << endl;
        vector<Tile*> t = tiler.generateTransTiles(sourceTiles);
        for (retvalIt = targetTiles.begin(); retvalIt != targetTiles.end(); retvalIt++)
            output.execUnaryOp(Ops::OP_IDENTITY, (*retvalIt)->getDomain(), (*retvalIt), (*retvalIt)->getDomain());
        for (retvalIt = t.begin(); retvalIt != t.end(); retvalIt++)
            output.execUnaryOp(Ops::OP_IDENTITY, (*retvalIt)->getDomain(), (*retvalIt), (*retvalIt)->getDomain());
        RManDebug = 30;
        output.printStatus(cout);
        RManDebug = 1;
    }
}

