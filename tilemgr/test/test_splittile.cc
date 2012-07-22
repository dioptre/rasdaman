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

/*************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#include <stdlib.h>
#include <iostream>
#include "typefactory.hh"
#include "basetype.hh"
#include "tilemgr/tile.hh"
#include "tilemgr/transtile.hh"


#include "raslib/rminit.hh"
RMINITGLOBALS('C')

#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"

// perhaps used later
static char O2BenchDBName[]  = "TestSMBase";
static char O2BenchSchemaName[] = "TestSMSchema";

// static void ClearDB( d_Database &DB );
extern char* myExecArgv0 = "";

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


    // connect to the database
    cout << "Connecting to database " << O2BenchDBName << "..." << endl;
    database.open( O2BenchDBName );


    // create indexes and put them in TestMDDObjContainer
    cout << " SplitTile..." << endl;
    ta.begin();

    static BaseType* bt = TypeFactory::mapType( "Short" );
    cout << "BaseType " << bt->getTypeName( ) << endl;

    r_Minterval tileDom =
        r_Minterval(2) << r_Sinterval(0l,0l) << r_Sinterval(0l,99l);
    //r_Minterval(2) << r_Sinterval(0l,9l) << r_Sinterval(0l,9l);
    //("[0:9,0:9]");
    cout << "tileDom " << tileDom << endl;
    r_Minterval BigTileDom =
        r_Minterval(2) << r_Sinterval(0l,39l) << r_Sinterval(0l, 99l);
    // ("[0:39,0:39]");
    cout << "BigTileDom " << BigTileDom << endl;

    unsigned long  sz = bt->getSize( ) * BigTileDom.cell_count( );

    char* cells = new char[sz];
    /*
    for (int i = 0; i < sz ; i++ )
    {
       if (i /2) cells[i] = char (i/10);
       else cells[i] = 0;
    }
    */
    short* typedCells = (short*) cells;
    unsigned typedSz = BigTileDom.cell_count( );
    for (int i = 0; i < typedSz; i++ )
        typedCells[i] = i;

    vector< Tile* >* tilesReturned;
    TransTile* tile1Obj1 = new TransTile( BigTileDom,
                                          (const BaseType* ) bt,
                                          ( char*) cells, 1);

    cout << "Calling splitTile " << endl;
    tilesReturned = tile1Obj1->splitTile( tileDom );
    cout << "Finished splitTile " << endl;


    vector<Tile*>::iterator entryIt = tilesReturned->begin();

    i = 0 ;
    while (entryIt !=  tilesReturned->end())
    {
        cout << "Tile " << i << endl;
        (*entryIt)->printStatus();
        entryIt++;
        i++;
    }

    ta.commit();

    delete[] cells;

    cout << endl << "Ending O2 session..." << endl;
    database.close( );
    delete myAdmin;
}

