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
 * SOURCE: test_mddops.cc
 *
 * MODULE: example for executing operations on MDDs
 *
 * PURPOSE:
 *   makes operations on MDDs or parts of them
 *
 * COMMENTS:
 *   later has to be moved to the executor
 *
*/

static const char rcsid[] = "@(#)blobif,test_mddops: $Id: test_mddops.cc,v 1.10 2002/09/03 14:46:50 coman Exp $";

#include <stdlib.h>
#include <iostream>
#include "o2lib_CC.hxx"
#include "o2template_CC.hxx"
#include "ulongtype.hh"
#include "mddmgr/persmddobj.hh"
#include "mddmgr/perstile.hh"
#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"
#include "raslib/rminit.hh"
#include "typefactory.hh"


// global variable for AdminIf because of O2 d_Session::begin()
extern char* myExecArgv0 = "";

RMINITGLOBALS('C')

static char O2BenchDBName[] = "DemoBase";
static char O2BenchSchemaName[] = "RasDaSchema";

static void testOperations( DatabaseIf myDB );

BaseType* myType;
static long myCell = 0;

/*************************************************************
 * Function name.: int main( int argc, char** argv)
 *
 * Arguments.....:
 *   argc: number of arguments given to program
 *   argv: array of char* with arguments
 * Return value..: exit status
 * Description...: none
 ************************************************************/

int
main( int argc, char** argv)
{
    // variables representing O2 database, ta and session
    DatabaseIf database;
    TransactionIf ta;

    // don't forget to initialize before using AdminIf!
    myExecArgv0 = argv[0];

    AdminIf* myAdmin = AdminIf::instance();

    // only possible after AdminIf::instance on Sun!
    myType = TypeFactory::mapType("ULong");

    // connect to the database
    cout << "Connecting to database " << O2BenchDBName
         << "..." << endl;
    database.open( O2BenchDBName );

    testOperations( database );

    cout << "Ending O2 session..." << endl;
    database.close();
    delete myAdmin;
}

void
printAllTiles(const MDDObj* mdd)
{
    // contains all tiles of MDD
    vector<Tile*>* allTiles;
    // iterator
    vector<Tile*>::iterator tileIt;
    // domains of a  tile
    r_Minterval tileDom;

    // domain of MDD object
    r_Minterval dom;
    dom = mdd->getCurrentDomain();

    // get all tiles of result MDD
    allTiles = mdd->intersect(dom);

    // and iterate over them
    tileIt = allTiles->begin();
    while (tileIt !=  allTiles->end())
    {
        tileDom = (*tileIt)->getDomain();
        cout << "Domain of Tile: ";
        tileDom.print_status();
        cout << endl;

        cout << "Tile: " << endl;
        (*tileIt)->printStatus();
        cout << endl;

        tileIt++;
    }
}

/*************************************************************
 * Function......: testOperations( DatabaseIf myDB )
 *
 * Arguments.....:
 *   myDB: database to use (should be opened)
 * Return value..: none
 * Description...: constructs BLOBTiles and inserts them
 *                 in root collection.
 ************************************************************/

MDDObj*
execBinaryOp( Ops::OpType op,
              const MDDObj* op1, const r_Minterval& areaOp1,
              const MDDObj* op2, const r_Minterval& areaOp2 )
{
    // contains all tiles of op1
    vector<Tile*>* allTilesOp1;
    // contains all tiles of op2 which intersect a given op1 Tile
    // in the relevant area.
    vector<Tile*>* intersectTilesOp2;
    // iterators for tiles of the MDDs
    vector<Tile*>::iterator tileOp1It;
    vector<Tile*>::iterator intersectTileOp2It;
    // domain of tile of Op1
    r_Minterval tileOp1Dom;
    // domain of tile of Op2
    r_Minterval tileOp2Dom;
    // intersection of domains in relevant area.
    r_Minterval intersectDom;
    // pointer to generated result tile
    PersTile* resTile;
    // MDDObj for result
    PersMDDObj* mddres;
    // translations between the two areas
    r_Point offset12(areaOp1.dimension());
    r_Point offset21(areaOp1.dimension());
    // dummy
    r_Minterval dummy;

    // calculate translations
    r_Point originOp1 = areaOp1.get_origin();
    r_Point originOp2 = areaOp2.get_origin();
    for(r_Dimension i = 0; i<areaOp1.dimension(); i++)
    {
        offset12[i] = originOp2[i] - originOp1[i];
        offset21[i] = originOp1[i] - originOp2[i];
    }

    // create MDDObj for result (type has later to be
    // retrieved from one of the operands)
    mddres = new PersMDDObj( areaOp1, "ULong" );

    // get all tiles in relevant area of MDD op1
    allTilesOp1 = op1->intersect(areaOp1);

    // and iterate over them
    tileOp1It = allTilesOp1->begin();
    while (tileOp1It !=  allTilesOp1->end())
    {
        // domain of the op1 tile
        tileOp1Dom = (*tileOp1It)->getDomain();

        // intersect the tile with MDD op2 (including translation)
        intersectTilesOp2 =
            op2->intersect(tileOp1Dom.create_translation(offset12));

        // iterate over intersecting tiles
        intersectTileOp2It = intersectTilesOp2->begin();
        while (intersectTileOp2It !=  intersectTilesOp2->end())
        {
            tileOp2Dom = (*intersectTileOp2It)->getDomain();

            // the relevant domain is the intersection of the
            // domains of the two tiles with the relevant area.
            intersectDom = tileOp1Dom.create_intersection(
                               tileOp2Dom.create_translation(offset21));
            intersectDom.intersection_with(areaOp1);

            // Creating tile for result. Type should later come from
            // operand.
            resTile = new PersTile(intersectDom, myType);

            // carry out operation on the relevant area of the tiles
            resTile->execBinaryOp(op, intersectDom,
                                  (*tileOp1It), intersectDom,
                                  (*intersectTileOp2It),
                                  intersectDom.create_translation(offset12));

            // insert Tile in result tile
            mddres->insertTile(resTile);

            intersectTileOp2It++;
        }
        tileOp1It++;
    }

    return mddres;
}

/*************************************************************
 * Function......: testConstructors( DatabaseIf myDB )
 *
 * Arguments.....:
 *   myDB: database to use (should be opened)
 * Return value..: none
 * Description...: constructs BLOBTiles and inserts them
 *                 in root collection.
 ************************************************************/

// function for creating demo tiles

Tile*
create2DTile( long xmin, long xmax, long ymin, long ymax,
              BaseType* type )
{
    // is copied anyway in constructor
    unsigned long cell = 0x10000L;

    r_Sinterval s1(xmin, xmax);
    r_Sinterval s2(ymin, ymax);
    r_Minterval dom(2);
    dom << s1 << s2;
    cout << "  Domain of Tile ";
    dom.print_status();
    cout << endl;
    return new PersTile( dom, type, (char*)&cell);
}

static void testOperations( DatabaseIf myDB )
{
    Tile* aTile;
    ULongType ulongtype;
    BaseType* type = &ulongtype;
    MDDObj* res;

    r_Sinterval limits1(1l,10l);
    r_Sinterval limits2(1l,10l);
    r_Minterval dom(2);
    dom << limits1 << limits2;

    r_Sinterval oplimits1(2l,9l);
    r_Sinterval oplimits2(2l,9l);
    r_Minterval opdom(2);
    opdom << oplimits1 << oplimits2;

    r_Sinterval limits21(11l,20l);
    r_Sinterval limits22(11l,20l);
    r_Minterval dom2(2);
    dom2 << limits21 << limits22;

    r_Sinterval oplimits21(12l,19l);
    r_Sinterval oplimits22(12l,19l);
    r_Minterval opdom2(2);
    opdom2 << oplimits21 << oplimits22;

    // create MDD Object for 1st operand
    cout << "MDD Op1" << endl;

    PersMDDObj* mddop1 = new PersMDDObj( dom, "ULong" );

    cout << "  Tile 1 [ 1:5, 1:10 ] " << endl;
    aTile = create2DTile(1, 5, 1, 10, type);
    mddop1->insertTile(aTile);

    cout << "  Tile 2 [ 6:10, 1:5 ] " << endl;
    aTile = create2DTile(6, 10, 1, 5, type);
    mddop1->insertTile(aTile);

    cout << "  Tile 3 [ 6:10, 6:10 ] " << endl;
    aTile = create2DTile(6, 10, 6, 10, type);
    mddop1->insertTile(aTile);

    mddop1->printStatus();

    // create MDD Object for 2nd operand
    cout << "MDD Op2" << endl;

    PersMDDObj* mddop2 = new PersMDDObj( dom2, "ULong" );

    cout << "  Tile 1 [ 11:17, 11:15 ] " << endl;
    aTile = create2DTile(11, 17, 11, 15, type);
    mddop2->insertTile(aTile);

    cout << "  Tile 2 [ 11:17, 16:20 ] " << endl;
    aTile = create2DTile(11, 17, 16, 20, type);
    mddop2->insertTile(aTile);

    cout << "  Tile 3 [ 18:20, 11:20 ] " << endl;
    aTile = create2DTile(18, 20, 11, 20, type);
    mddop2->insertTile(aTile);

    mddop2->printStatus();

    res = execBinaryOp(Ops::OP_PLUS, mddop1, opdom, mddop2, opdom2);

    // output result (cast should not be necessary!)
    ((PersMDDObj*)res)->printStatus();
    printAllTiles(res);

}
