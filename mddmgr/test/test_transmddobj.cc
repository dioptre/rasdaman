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
 * SOURCE: test_transmddcoll.cc
 *
 * MODULE: test for transmddobj of cachetamgr
 *
 * COMMENTS:
 *   none
 *
*/

#include "rt_odb_Database.hxx"

#define TEST_PRIVATE

#include <stdlib.h>
#include <iostream>
#include <vector.h>                  // STL
#include "mddmgr/transtile.hh"
#include "mddmgr/transmddobj.hh"

#include "typefactory.hh"
#include "ulongtype.hh"              // from catalogif
#include "mddbasetype.hh"            //  from catalogif

#include "raslib/minterval.hh"
#include "mddmgr/transmddcoll.hh"
#include "mddmgr/mddcolliter.hh"
#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"

#include "raslib/rminit.hh"
RMINITGLOBALS('C')

#include <malloc.h>

// Needed by Adminif. Adminif has to be instantiated because
// of cell base types.
extern char* myExecArgv0 = "";

void testConstruction( );
void testRemovetile( );
void testIntersection( );
void printMemInfo( );


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
    myExecArgv0 = argv[0];
    AdminIf* myAdmin = AdminIf::instance();
    TransactionIf ta;
    char O2DBName[] = "BaseTestTransMDDObj";

    // variables representing O2 database, ta and session
    DatabaseIf database;
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

    ta.begin( &database, 1 );

    cout << " Main - begin " << endl;
    printMemInfo( );

    /*
    cout << "---------- ";
    cout << "Testing constructor and insertTile for TransMDDObj: " << endl;
    testConstruction( );
    cout << "Testing constructor - end " << endl;
    printMemInfo( );
    */

    cout << "Testing intersection for TransMDDObj: " << endl;
    testIntersection( );
    cout << "Testing intersection - end " << endl;
    printMemInfo( );


    /*
    cout << "---------- ";
    cout << "Testing removeTile from TransMDDObj: " << endl;
    testRemovetile( );
    cout << "Main - end " << endl;
    printMemInfo( );
    */

    ta.abort( );
    database.close( );

    delete myAdmin;

}

void testConstruction( )
{
    const int numTilesObj = 20;
    r_Sinterval domSinterval;
    cout <<"here 1" << endl;
    ULongType ult;
    BaseType* anyType = new( (d_Database*)(d_Database::transient_memory) ) ULongType; //  anyType;

    cout << "here 2"<< endl;
    const MDDBaseType* anyBaseType =
        new( (d_Database*)(d_Database::transient_memory) ) MDDBaseType( "AnyType", &ult/* anyType */ );

    // const MDDBaseType anyBaseType( "AnyType", &ult );

    char anyCell[4];
    int j;
    MDDObj* testMDDObj;


    cout << "Creating transient tiles for the MDD object ... " << endl;
    vector<TransTile*> tilesVect(numTilesObj);


    // initialize array of tiles
    for (j=0; j<numTilesObj ; j++)
    {
        r_Minterval dom(2);
        TransTile* tt;

        domSinterval.set_interval( r_Range(j* 10), r_Range((j+1)*10-1) );
        dom <<  domSinterval << domSinterval;
        cout << "Newly created tile domain : " << dom << "  " << endl;
        // printMemInfo();
        tt = new TransTile( dom, anyType, anyCell );
        cout << "Tile created " << endl;
        // printMemInfo();
        tilesVect[j] = tt ;
    }

    // Test trans tiles part - BEGIN
    // works fine
    /*
    for ( int h=0; h < tilesVect.size(); h++)
       delete tilesVect[h];
    */
    // release (tilesVect.begin( ), tilesVect.end( ) );

    // Test trans tiles part - END


    cout << "Creating the transient MDD object ... " << endl;

    r_Minterval dom(2);
    domSinterval.set_interval( r_Range( 0 ), r_Range( 100 ) );
    domSinterval.set_low( '*' );
    dom << domSinterval << domSinterval;
    // testMDDObj = new TransMDDObj( dom, "ULong" );
    testMDDObj = new TransMDDObj( anyBaseType, dom );
    for(j=0; j < numTilesObj; j++)
    {
        testMDDObj->insertTile(  tilesVect[j] );
    }
    // printMemInfo();

    cout << "Printing contents of the created object ... " << endl;
    testMDDObj->printStatus( );

    cout << "Deleting the created object ... " << endl;
    delete testMDDObj;
    // printMemInfo();


}


void testIntersection( )
{
    const int numTilesObj = 20;
    r_Sinterval domSinterval;

    const BaseType* ulongTypeObj = TypeFactory::mapType("ULong");

    char uLongCell[] = {0,1,2,3};

    const MDDBaseType* anyBaseType =
        new MDDBaseType( "AnyType1", (BaseType*) ulongTypeObj );

    TypeFactory::addTempType( (Type*) anyBaseType );

    BaseType* bt = ( (MDDBaseType* ) anyBaseType )->getBaseType( );
    cout << "Base type size " <<  bt->getSize( )<< endl;


    int j;
    MDDObj* testMDDObj;


    cout << "Creating transient tiles for the MDD object ... " << endl;
    vector<TransTile*> tilesVect(numTilesObj);


    // initialize array of tiles
    for (j=0; j<numTilesObj ; j++)
    {
        r_Minterval dom(2);
        TransTile* tt;

        domSinterval.set_interval( r_Range(j* 10), r_Range((j+1)*10-1) );
        dom <<  domSinterval << domSinterval;
        cout << "Newly created tile domain : " << dom << "  " << endl;
        // printMemInfo();
        tt = new TransTile( dom,  (BaseType*) ulongTypeObj, uLongCell );
        cout << "Tile created " << endl;
        if (!j)
            tt->printStatus( );
        // printMemInfo();
        tilesVect[j] = tt ;
    }

    // Test trans tiles part - BEGIN
    // works fine
    /*
    for ( int h=0; h < tilesVect.size(); h++)
       delete tilesVect[h];
    */
    // release (tilesVect.begin( ), tilesVect.end( ) );

    // Test trans tiles part - END


    cout << "Creating the transient MDD object ... " << endl;

    r_Minterval dom(2);
    domSinterval.set_interval( r_Range( 0 ), r_Range( 100 ) );
    domSinterval.set_low( '*' );
    dom << domSinterval << domSinterval;
    testMDDObj = new  TransMDDObj( anyBaseType, dom );
    for(j=0; j < numTilesObj; j++)
    {
        testMDDObj->insertTile(  tilesVect[j] );
    }
    // printMemInfo();

    cout << "Printing contents of the created object ... " << endl;
    testMDDObj->printStatus( );

    r_Minterval searchInterval(2);
    domSinterval.set_interval( r_Range( 4 ), r_Range( 96 ) );
    searchInterval<< domSinterval ;
    domSinterval.set_interval( r_Range( 22 ), r_Range( 84 ) );
    searchInterval<< domSinterval ;
    cout << "Intersection with "<< searchInterval << " :" << endl ;

    vector<Tile*>* intersectResult =
        ( testMDDObj )->intersect( searchInterval );
    cout << "Result of intersection:" << endl;
    cout << "Intersect result size " << intersectResult->size( ) << endl;
    for (int tilesIter = 0; tilesIter < intersectResult->size( ) ; tilesIter++)
    {
        Tile* currTile;
        currTile = (*intersectResult)[tilesIter];
        cout << "Tile " << tilesIter << " domain : " ;
        cout << currTile->getDomain( ) << endl;
    }

    // printMemInfo();
    cout << "Deleting intersection result " << endl;
    // Individual tiles in the intersectResult shouldn't be deleted, since
    // they are part of the TransMDDObj. They are deleted whenever the object
    // is deleted.
    delete intersectResult;
    // printMemInfo();

    cout << "Testing point query " << endl;
    r_Point pnt1( r_Range(2) , r_Range(3) );
    unsigned long* c1 = (unsigned long*) testMDDObj->pointQuery( pnt1 );
    // char* c1 = testMDDObj->pointQuery( pnt1 );
    cout << "1. Result "<< pnt1 << ": "<< *c1 << endl;

    r_Point pnt2( r_Range(20) , r_Range(30) );
    unsigned long* c2 = (unsigned long*) testMDDObj->pointQuery( pnt2 );
    // char* c2 = testMDDObj->pointQuery( pnt2 );
    cout << "2. Result "<< pnt2 << ": "<<  c2 << endl;

    cout << "Deleting the created object ... " << endl;
    delete testMDDObj;
    printMemInfo();
}


void testRemovetile( )
{
    const int numTilesObj = 20;
    r_Sinterval domSinterval;
    BaseType* anyType = new( (d_Database*)(d_Database::transient_memory) )  ULongType; //  ULongType anyType;
    char anyCell[4]= {'a','b','c','\0'};
    int j,i;
    TransMDDObj* testMDDObj;
    vector<Tile*>* allTilesObj;
    vector<Tile*> tilesToDelete;
    const MDDBaseType* anyBaseType =
        new( (d_Database*)(d_Database::transient_memory) )  MDDBaseType( "AnyType", anyType );

    cout << "Creating transient tiles for the MDD object ... " << endl;
    vector<TransTile*> tilesVect;

    // initialize array of tiles
    for ( j=0; j<numTilesObj ; j++)
    {
        r_Minterval dom(2);
        domSinterval.set_interval( r_Range(j* 10), r_Range((j+1)*10-1) );
        dom <<  domSinterval << domSinterval;
        cout << "New TransTile " << endl;
        tilesVect.push_back( new TransTile( dom, anyType, anyCell ) );
        // printMemInfo( );
    }

    cout << "Creating the transient MDD object ... " << endl;

    r_Minterval dom(2);
    domSinterval.set_interval( r_Range( 0 ), r_Range( 100 ) );
    domSinterval.set_low( '*' );
    dom << domSinterval << domSinterval;
    testMDDObj = new TransMDDObj( anyBaseType, dom );
    for( j=0; j < numTilesObj; j++)
    {
        cout << "InsertTile " << endl ;
        testMDDObj->insertTile( ( Tile*) tilesVect[j] );
        // printMemInfo( );
    }

    cout << "Printing contents of the created object ... " << endl;
    testMDDObj->printStatus( );

    cout << "Getting all tiles from the object... " << endl;
    allTilesObj = testMDDObj->getTiles( );
    // printMemInfo( );

    cout << "Removing tile 2, 4, 6, and 12 from the object... " << endl;
    for ( i = 0; i < allTilesObj->size( ); i++)
    {
        if ( i==2 || i == 4 || i == 6 || i == 12 )
        {
            tilesToDelete.push_back( (*allTilesObj)[i]);
        }
    }
    for ( i = 0; i < tilesToDelete.size( ); i++)
    {
        cout << " Tile is going to be removed " << endl;
        testMDDObj->removeTile(tilesToDelete[i] );
        cout << endl << " Value of  pointer to tile : " << tilesToDelete[i] << endl<< endl;
        // printMemInfo();
    }
    testMDDObj->printStatus( );

    cout << "Deleting the created object ... " << endl;
    // printMemInfo( );
    delete testMDDObj;
    cout << "Already deleted " << endl;
    // printMemInfo( );

    delete  allTilesObj;
}


void printMemInfo( )
{


    // allows to store values in the program
    struct mallinfo meminfo = mallinfo();

    cout << "   Memory Usage Information :  " ;
    cout << endl;

    cout << "      space in arena                 : " << meminfo.arena << endl;
    cout << "      number of small blocks         : " << meminfo.smblks << endl;
    cout << "      number of ordinary blocks      : " << meminfo.ordblks << endl;
    cout << "      space in free ordinary blocks  : " << meminfo.fordblks << endl;
    cout << "      space in used ordinary blocks  : " << meminfo.uordblks << endl;

    // cout << "additional space from last call: " << meminfo.uordblks - memUsed << endl;

}


