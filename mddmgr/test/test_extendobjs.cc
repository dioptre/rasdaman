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
 * SOURCE: test_extendobjs.cc
 *
 * MODULE: cachetamgr
 *
 * PURPOSE:
 *   Extends the objects created by the test of directional tiling
 *   rasodmg/test/test_dir1 (the resulting objects have 4 times more
 *   products and 2 times more stores than the original ones ).
 *   Needed because rasodmg doens't support updating of MDDobjects
 *   at the time this was created.
 *
 *
 * COMMENTS:
 *   none
 *
*/

#define TEST_PROTECTED
#define TEST_PRIVATE

#include <stdlib.h>
#include <iostream>

#include "ulongtype.hh"

#include "mddmgr/persmddcoll.hh"
#include "mddmgr/persmddobj.hh"
#include "mddmgr/perstile.hh"

#include "mddmgr/persmddcolliter.hh"

#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"
#include "oidif.hh"

#include "raslib/rminit.hh"
#include "typefactory.hh"

#include "mddif/dbmddobj.hh"
#include "mddif/dbmddcoll.hh"

#include "mddbasetype.hh"
#include "mdddomaintype.hh"
#include "settype.hh"


/*
  Global Variables
*/
extern char* myExecArgv0 = "";

RMINITGLOBALS('C')

static char* O2DBName;
char *collName;
char defaultCollName[]= "ObjsContainer";

TransactionIf ta;

/*
  Functions
*/

// 2 - Populate collection with MDD objects
static void testConstructors( char* cn , int s, int p);

// 3 - Retrieves an MDD collection with name cn and prints contents:
static void testAccessing( char* cn );


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

    if( argc < 2 )
    {
        cout << "Usage: test_persmddcoll <database> [collName]" << endl;
        return -1;
    }
    O2DBName = strdup( argv[1] );
    if ( argc == 3 ) collName = strdup( argv[2] );
    else
        collName = defaultCollName;

    // don't forget to initialize before using AdminIf!
    myExecArgv0 = argv[0];
    AdminIf* myAdmin = AdminIf::instance();


    // connect to the database
    cout << "Connecting to database " << O2DBName
         << "..." << endl;
    int errorDBOpen;
    try
    {
        errorDBOpen =  database.open( O2DBName );
    }
    catch( ...)
    {
        cout << "Caught Exception " << endl;
        errorDBOpen = -6;
    }
    if ( errorDBOpen < 0 )
    {
        cout << "Database doesn't exist. Create it new ... " << endl;
        cout << "Creating new database " << O2DBName
             << "..." << endl;
        database.create( O2DBName, "TestSMSchema" );
        cout << "Connecting to database " << O2DBName
             << "..." << endl;
        try
        {
            errorDBOpen =  database.open( O2DBName );
        }
        catch(...)
        {
            errorDBOpen = -6;
        }
    }
    if ( errorDBOpen < 0 )
    {
        cout << "Failed at opening newly created database " << errorDBOpen << endl;
        cout << "Exiting " << endl;
        return errorDBOpen;
    }

    char c;

    for( int p = 0; p < 4; p++ )
    {
        for ( int s = 0; s < 2 ; s++)
        {
            ta.begin( &database );
            cout << endl << "Populate collection " << s <<" " << p << " ..." << endl;
            testConstructors( collName, s, p );
            cout <<"Transaction abort (A/a) or commit (default)? ";
            cin >> c;
            if ( c == 'A' || c == 'a' )
            {
                ta.abort( );
                cout <<"End of Transaction Abort..."<<endl;
            }
            else
            {
                ta.commit( );
                cout <<"End of transaction commit... "<<endl;
            }
        }
    }


    ta.begin(&database );
    // read coll and print contents
    cout << endl << "Read collection " << collName << " and print contents..." << endl;
    testAccessing( collName );
    ta.commit( );
    cout <<"End of transaction commit... "<<endl;

    cout << endl << "Ending O2 session..." << endl;
    database.close( );
    delete myAdmin;

    free( O2DBName );
    if ( collName != defaultCollName ) free( collName );
    return 0;

}

/*************************************************************
 * Functions......:
 *
 *    static void
 *    testConstructors( char* collName )
 *
 ************************************************************/
static void testConstructors( char* collName, int s, int p )
{

    const BaseType* mddType;
    char* uLongCells;

    cout << "....testConstructors"<< endl;


    PersMDDObj* accessedObj;

    try
    {
        PersMDDColl objsSet(collName);

        cout << "Iterating through the collection with PersMDDCollIter " << endl;
        MDDCollIter* objsIt = objsSet.createIterator( );

        for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
        {
            cout << endl << i<<". MDD object in set:" << endl;
            accessedObj = (PersMDDObj*) objsIt->getElement();
            // accessedObj->printStatus();
            EOId eoid;
            if ( accessedObj->getEOId( &eoid ) ==0 )
                cout <<"EOId: " << eoid;
            cout << endl << endl;

            mddType = accessedObj->getCellType( );

            r_Minterval firstYear("[1:365,1:60,1:100]");

            vector< Tile* >* firstYearTiles = accessedObj->intersect( firstYear );

            for ( int j = 0; j < firstYearTiles->size( ); j++ )
            {
                r_Minterval dom = (*firstYearTiles)[j]->getDomain( );

                /*
                        for( int p = 0; p < 4; p++ )
                        {
                          for ( int s = 0; s < 2 ; s++)
                          {
                */
                r_Point desl( (r_Range) 730, (p+1)*60, (s+1)* 100 );

                r_Minterval newDom( dom.dimension( ) );
                newDom.intersection_of( dom, "[1:365,1:60,1:100]");
                newDom.translate( desl );
                cout << "dom" << dom << "newDom " << newDom << endl;

                int sz = mddType->getSize( ) * newDom.cell_count( );
                uLongCells = new char[sz];
                PersTile* tile1Obj1 = new PersTile( newDom, mddType, uLongCells );
                accessedObj->insertTile(tile1Obj1);

                /*
                          }
                        }

                */
            }
            delete firstYearTiles;
        }

        delete objsIt;

        cout << "Release all " << endl;
        objsSet.releaseAll( );
    }
    catch ( r_Error& errObj)
    {
        cout << "Error caught when opening collection" << endl;
    }

}


/*************************************************************
 * Function......: testAccessing( char* cn )
 ************************************************************/

static void testAccessing( char* cn )
{
    PersMDDObj* accessedObj;

    cout << "....testAccessing collection "<< cn << endl;

    try
    {
        PersMDDColl objsSet( cn );

        // To test PersMDDColl::printStatus( )
        // objsSet.printStatus( );

        // To test PersMDDObj::printStatus( ), MDDCollIter::createIterator( ) and
        // MDDCollIter methods :

        cout << "Iterating through the collection with PersMDDCollIter " << endl;
        MDDCollIter* objsIt = objsSet.createIterator( );

        for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
        {
            cout << i<<". MDD object in set:" << endl;
            accessedObj = (PersMDDObj*) objsIt->getElement();
            accessedObj->printStatus();
            EOId eoid;
            if ( accessedObj->getEOId( &eoid ) ==0 )
                cout <<"EOId: " << eoid;
        }
        delete objsIt;
        objsSet.releaseAll( );
    }
    catch ( r_Error& errObj)
    {
        cout <<"Error caught ................."<< endl;
    }
}

