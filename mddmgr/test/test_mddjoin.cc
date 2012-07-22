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
 * SOURCE: test_mddjoin.cc
 *
 * MODULE: example for making one tile out of a vector of
 *         Tiles
 *
 * COMMENTS:
 *   later has to be moved to the executor
 *
*/

static const char rcsid[] = "@(#)blobif,test_mddops: $Id: test_mddjoin.cc,v 1.9 2002/09/03 14:46:50 coman Exp $";

#include <stdlib.h>
#include <iostream>
#include "ulongtype.hh"
#include "booltype.hh"
#include "mddmgr/persmddobj.hh"
#include "mddmgr/perstile.hh"
#include "mddmgr/transtile.hh"
#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"
#include "raslib/rminit.hh"
#include "typefactory.hh"

// global variable for AdminIf because of O2 d_Session::begin()
extern char* myExecArgv0 = "";

RMINITGLOBALS('C')

static char O2BenchDBName[] = "NorbertBase";
static char O2BenchSchemaName[] = "NorbertSchema";

static void testOperations( DatabaseIf myDB );

static BaseType* myType;
static BaseType* boolType;

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
main( int /* argc */, char** argv)
{
    // variables representing O2 database, ta and session
    DatabaseIf database;
    TransactionIf ta;

    // don't forget to initialize before using AdminIf!
    myExecArgv0 = argv[0];
    AdminIf* myAdmin = AdminIf::instance();

    // connect to the database
    cout << "Connecting to database " << O2BenchDBName
         << "..." << endl;
    database.open( O2BenchDBName );
    ta.begin(&database);

    // only possible after AdminIf::instance on Sun!
    myType = TypeFactory::mapType("ULong");
    boolType = TypeFactory::mapType("Bool");

    testOperations( database );

    ta.commit();
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
 * Function......: testConstructors( DatabaseIf myDB )
 *
 * Arguments.....:
 *   myDB: database to use (should be opened)
 * Return value..: none
 * Description...: constructs BLOBTiles and inserts them
 *                 in root collection.
 ************************************************************/

static void testOperations( DatabaseIf /* myDB */)
{
    unsigned long cell = 1000;

    ULongType ulongtype;
    BaseType* type = &ulongtype;
    Tile* res;
    Tile* boolTile;

    vector<Tile*>* result = new vector<Tile*>;
    vector<Tile*>::iterator tileIt;
    Tile* bigTile;

    cout << "Creating Tile: ";
    r_Minterval bigDom =
        r_Minterval(3) << r_Sinterval(1l,10l) << r_Sinterval(1l,10l)
        << r_Sinterval(1l,10l);
    bigDom.print_status();
    cout << endl;

    bigTile = new PersTile(bigDom, type, (char*)&cell);

    r_Minterval smallDom
    = r_Minterval(3) << r_Sinterval(1l,6l) << r_Sinterval(1l,6l)
      << r_Sinterval(1l,6l);

    cout << "Splitting it into ";
    smallDom.print_status();
    cout << " tiles." << endl;

    result = bigTile->splitTile(smallDom);

    cout << "Result tiles: " << endl;
    // and iterate over them
    tileIt = result->begin();
    while (tileIt !=  result->end())
    {
        cout << "  Domain of Tile " << (tileIt - result->begin()) << ": ";
        ((*tileIt)->getDomain()).print_status();
        cout << endl;

        tileIt++;
    }

    cout << "Joining the Tiles again:" << endl;

    r_Minterval proj =
        r_Minterval(3) << r_Sinterval(2l,9l) << r_Sinterval(2l,9l)
        << r_Sinterval(2l,9l);

    res = new TransTile(result, proj );

    cout << "  Result MDD equals original MDD: ";
    boolTile = new TransTile(res->getDomain(), boolType);
    cout << "Domain of result Tile: ";
    (res->getDomain()).print_status();
    cout << endl;
    res->printStatus();
    boolTile->execBinaryOp(Ops::OP_EQUAL, proj,
                           res, proj,
                           bigTile, proj);

    char init = 1;

    cout << "Checking if Tile are equal: ";
    cout << (int)(*(boolTile->execCondenseOp(Ops::OP_ALL,
                    res->getDomain(),
                    &init ))) << endl;

    cout << "Creating a trimmed, projected 2-D Tile out of the Tile:" << endl;
    set<r_Dimension, less<r_Dimension> >* projSet =
        new set<r_Dimension, less<r_Dimension> >;
    projSet->insert(1);

    r_Minterval projDom =
        r_Minterval(3) << r_Sinterval(2l, 9l) << r_Sinterval(5l, 5l)
        << r_Sinterval(2l, 9l);

    Tile* projectedTile = new TransTile(res, projDom, projSet);

    projectedTile->printStatus();

    cout << endl;
}
