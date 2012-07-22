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
 * SOURCE: test_extbmarkint.cc
 *
 * MODULE: mddmgr
 *
 * PURPOSE:
 *   Extends the objects created by the test of areas of interest tiling
 *   rasodmg/test/test_int1 (the resulting objects have 5000 frames more
 *   than the original ones ).
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
static void testConstructors( OId o,
                              int numberFramesTile,
                              int f,  // first frame
                              int n ); // number frames

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
        cout << "Usage: test_extbmarkint <database> [collName] [oid] [frames] " << endl;
        return -1;
    }
    O2DBName = strdup( argv[1] );
    if ( argc >= 3 ) collName = strdup( argv[2] );
    else
        collName = defaultCollName;

    int numberFramesTile = 3; // 9
    OId o( double( 2)); //
    if( argc >= 4 )
    {
        int oid;
        o = OId( double( atoi( argv[3] )));
        cout << "OId " << o << endl;
    }
    if( argc >= 5 )
    {
        numberFramesTile = atoi( argv[4] );
        cout << "Number of frames per tile "<< numberFramesTile << endl;
    }

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

    unsigned totalNumberFrames = 200; /* 2000 */

    // The first 2000 frames
    for( int i = 0; i < totalNumberFrames ; i+= numberFramesTile*30 )
    {
        ta.begin( &database );
        cout << endl << "Populate collection " << i << " ..." << endl;
        int numberFrames = numberFramesTile*30;
        if ( i+numberFrames > totalNumberFrames ) numberFrames = totalNumberFrames-i;
        testConstructors( o, numberFramesTile, i, numberFrames );

        /*
            cout <<"Transaction abort (A/a) or commit (default)? ";
            cin >> c;
            if ( c == 'A' || c == 'a' )
            {
              ta.abort( );
              cout <<"End of Transaction Abort..."<<endl;
            }
            else
            {
        */
        ta.commit( );

        cout <<"End of transaction commit... "<<endl;
//    }
    }


    const int lastFrame = 2400; // 5120

    // The last frames up to lastFrame
    for( i = 2121 ; i < lastFrame ; i+= numberFramesTile*30 ) // 2121
    {
        ta.begin( &database );
        cout << endl << "Populate collection " << i << " ..." << endl;
        int numberFrames = numberFramesTile*30;
        if ( i+numberFrames > lastFrame ) numberFrames = lastFrame-i;
        testConstructors( o, numberFramesTile, i, numberFrames );

        /*
            cout <<"Transaction abort (A/a) or commit (default)? ";
            cin >> c;
            if ( c == 'A' || c == 'a' )
            {
              ta.abort( );
              cout <<"End of Transaction Abort..."<<endl;
            }
            else
            {
        */
        ta.commit( );
        /*
              cout <<"End of transaction commit... "<<endl;
            }
        */
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
static void testConstructors( OId o,
                              int numberFramesTile,
                              int f,  // first frame
                              int n ) // number frames
{

    const BaseType* mddType;

    cout << "....testConstructors"<< endl;

    try
    {

        PersMDDObj* accessedObj = new PersMDDObj( O2DBName, o );
        int typeSize= accessedObj->getCellType( )->getSize( );
        cout << "currDom  " << accessedObj->getCurrentDomain( ) << endl;
        accessedObj->printStatus();

        EOId eoid;
        if ( accessedObj->getEOId( &eoid ) ==0 )
            cout <<"EOId: " << eoid;
        cout << endl << endl;
        mddType = accessedObj->getCellType( );

        r_Minterval firstTile( 3 );
        firstTile << r_Sinterval( long ( 0 ), long ( numberFramesTile-1 ) )
                  << r_Sinterval( long ( 0 ), long ( 79 ) )
                  << r_Sinterval( long ( 0 ), long ( 119 ) );

        r_Minterval secondTile( 3 );
        secondTile << r_Sinterval( long ( 0 ) , long ( numberFramesTile-1 ) )
                   << r_Sinterval( long ( 80 ), long ( 159 ) )
                   << r_Sinterval( long ( 0 ) , long ( 119 ) );

        for ( int j = f; j < f+n ; j+=numberFramesTile )
        {
            r_Minterval dom1 = firstTile;
            r_Minterval dom2 = secondTile;
            r_Point desl( (r_Range) j, 0, 0 );
            dom1.translate( desl );
            dom2.translate( desl );
            if ( dom1[0].high( ) >= f+n )
            {
                dom1[0].set_high( long( f+n-1) );
                dom2[0].set_high( long( f+n-1) );
            }
            cout << "dom1" << dom1 << " dom2 " << dom2
                 << " type " << mddType->getTypeName( ) << endl;

            int sz1 = dom1.cell_count( ) * typeSize;
            char* cells1 = new char[sz1];
            PersTile* tile1 = new PersTile( dom1, mddType, cells1);
            accessedObj->insertTile(tile1);

            int sz2 = dom2.cell_count( ) * typeSize;
            char* cells2 = new char[sz2];
            PersTile* tile2 = new PersTile( dom2, mddType, cells2 );

            accessedObj->insertTile(tile2);
            // accessedObj->printStatus( );
        }

        delete accessedObj;

    }
    catch ( r_Error& errObj)
    {
        cout << "Error caught when opening object" << endl;
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

