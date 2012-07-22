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
 * SOURCE: test_expix.cc
 *
 * MODULE: test for DirIx<T>
 *
 * PURPOSE:
 *   instantiates DirIx objects, inputs tiles and reads them, comparing
 *   performance for different indexes (for instance, DirIx<PersDirIx> vs.
 *   DirIx<TransDirIx>. Uses O2 directly.
 *
 * COMMENTS:
 *   none
 *
*/

#include <stdlib.h>
#include <iostream.h>
#include <vector.h>

#include "o2lib_CC.hxx" // declaration of O2-collection-classes

#include "indexmgr/transmddobjix.hh"
#include "indexmgr/persmddobjix.hh"
#include "indexmgr/dirix.hh"
#include "indexmgr/transdirix.hh"
#include "dbdirix.hh"
#include "dbmddobjix.hh"
#include "blobtile.hh"
#include "basetype.hh"
#include "ulongtype.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "cachetamgr/perstile.hh"
#include "cachetamgr/tile.hh"
#include "tools/timer.hh"
#include "indexmgr/persdirix.hh"

static char O2BenchDBName[] = "DirIxBase";
// This test program must use a different base because it
// doesn't use catalogif and adminif. It is not a complete RasDaBase.
static char O2BenchSchemaName[] = "TestSMSchema";

extern char* myExecArgv0 = "";

#include "raslib/rminit.hh"
RMINITGLOBALS('C')

static void ClearDB( d_Database &DB );
static void testAccessing();

static void testTransDirIx();
static void testDirIx();


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
    d_Session  session;
    d_Database database;
    d_Transaction ta;

    // initialize the O2 session
    cout << "Initializing O2 session..." << endl;
    session.set_default_env();
    if (session.begin(argc, argv))
    {
        cerr << "Something wrong to start o2" << endl;
        exit(1);
    }

    // clear the DB (in case the base already exists)
    cout << "Deleting contents of database..." << endl;
    ClearDB(database);

    // connect to the database
    cout << "Connecting to database " << O2BenchDBName
         << "..." << endl;
    // database.open( O2BenchDBName ); // doesn't work with O2 V.5

    // create root collection
    cout << "Creating root collection..." << endl;
    ta.begin();
    // database.create_persistent_root( "IndexContainer",
    //                 "d_List<d_Ref<DBDirIx>>",
    //                 OL_CONSTANT);
    ta.commit();

    /*
    cout << "Testing TransDirIx..." << endl;
    ta.begin();
    testTransDirIx( );
    ta.commit();
    */

    cout << "Testing DirIx..." << endl;
    ta.begin();
    testDirIx( );
    ta.commit();

    cout << endl;
    cout << "Ending O2 session..." << endl;
    database.close();
    session.end();

    return 0;
}


void
testTransDirIx( )
{

    cout << "....testTransDirIx"<< endl;

    TransDirIx* ti = new TransDirIx( 2 );
    cout << "TransDirIx just created: "<< endl;
    ti->printStatus( );

    ULongType anyType;
    char anyCell[4];

    TransTile* ttArray[12];

    for( long j = 0; j < 12 ; j++)
    {
        r_Minterval dom(2);
        dom << r_Sinterval( j, j+ 5 ) << r_Sinterval( j , j+5 );
        TransTile* tt = new TransTile( dom, (BaseType* ) &anyType, anyCell );
        ttArray[j] = tt;
    }

    for( j = 0; j < 10 ; j++)
    {
        cout << "Insert Tile Last " << ttArray[j]->getDomain( ) << endl;
        ti->insertObjectLast( ttArray[j] );
        ti->printStatus( );
    }

    cout << "Insert Tile pos  5 " << ttArray[10]->getDomain( ) << endl;
    ti->insertObject( ttArray[10], 5 );
    ti->printStatus( );

    cout << "Insert Tile First " << ttArray[11]->getDomain( ) << endl;
    ti->insertObjectFirst( ttArray[11] );
    ti->printStatus( );

    delete ti;
}

void
testDirIx( )
{
    cout << "....testDirIx"<< endl;

    ULongType anyType;
    char anyCell[4];

    // TransDirIx* t = new TransDirIx( 2 );
    // PersDirIx* t = new PersDirIx( 2, &anyType );

    // char index[] =  "MDDObjIx* DirIx<TransDIrIx>";
    // char index[] =  "MDDObjIx* DirIx<PersDirIx>";
    // char index[] =  "MDDObjIx* PersMDDObjIx    ";
    // char index[] =  "TransMDDObjIx* DirIx<TransDirIx>   ";
    char index[] =  "PersMDDObjIx* DirIx< >   ";
    // MDDObjIx* /* DirIx< TransDirIx >* */ di  = new DirIx< TransDirIx >( t );
    // DirIx< PersDirIx >* di = new DirIx< PersDirIx >( t );
    MDDStorage ms1;
    ms1.setIndexType( MDDStorage::DirTilesIx );
    PersMDDObjIx* di = new PersMDDObjIx(  2, &anyType, &ms1 );
    // TransMDDObjIx* di = new TransMDDObjIx(  2, 0 );

    // char index1[] =  "TransMDDObjIx* RegDirIx<TransDirIx>";
    char index1[] = "PersMDDObjIx* RegDirIx< >";
    // char index1[] = "TransMDDObjIx*            ";
    // TransMDDObjIx* di1 = new TransMDDObjIx( 2, 1 );
    // DirIx< TransDirIx >* di1  = new DirIx< TransDirIx >( t );
    MDDStorage ms;
    ms.setIndexType( MDDStorage::RegDirIx );
    PersMDDObjIx* di1 = new PersMDDObjIx( 2, &anyType, &ms );

    cout << "Comparison of      Index  " << index << endl;
    cout << "with               Index1 " << index1 << endl;

    // NumTiles should be a square from something:
    unsigned const sqrtnumTiles = 32;
    unsigned const NumTiles = sqrtnumTiles * sqrtnumTiles ;
    unsigned const NumIntersections = 30;

    // cout << "Index " << index << " just created: "<< endl;
    // di->printStatus( );


    cout <<"Random insertion of tiles into the indexe(s) ..." << endl;
    Tile* ttArray[NumTiles];
    Tile* tt1Array[NumTiles];
    unsigned alreadyUsed[NumTiles];

    long j = 0;

    for( long k = 0; k < sqrtnumTiles ; k++ )
    {
        for( long l = 0; l < sqrtnumTiles ; l++)
        {
            r_Minterval dom(2);
            // dom << r_Sinterval( k*5 , k*5+5-1 ) << r_Sinterval( l*5 , l*5+5-1 );
            dom << r_Sinterval( k , k ) << r_Sinterval( l , l );

            // Tile* tt = new TransTile( dom, (BaseType* ) &anyType, anyCell );
            Tile* tt = new PersTile( dom, (const BaseType* ) &anyType, anyCell );
            // Tile* tt1 = new TransTile( dom, (BaseType* ) &anyType, anyCell );
            Tile* tt1 = new PersTile( dom, (const BaseType* ) &anyType, anyCell );

            ttArray[j] = tt;
            tt1Array[j] = tt1;
            alreadyUsed[j]=0;
            j++;
        }
    }

    cout <<" j "<< j << endl;

    for( j = 0; j < NumTiles ; j++)
    {
        int ixToUse = rand( ) % NumTiles;
        if ( alreadyUsed[ ixToUse ] )
        {
            // calculate another index;
            ixToUse = 0;
            for( int i = 0; !ixToUse && i < NumTiles; i++ )
            {
                if ( !alreadyUsed[i] ) ixToUse = i;
            }
        }
        alreadyUsed[ ixToUse ] = 1;
        cout << "Insert Tile ttArray[ " << ixToUse << " ] : ";
        cout << ttArray[ixToUse]->getDomain( ) << endl;

        // Index
        di->insertTile( ttArray[ ixToUse ] );
        // Index 1
        di1->insertTile( tt1Array[ ixToUse ] );
        // di->printStatus( );
    }

    cout << index << " contents: "<< endl;
    di->printStatus( );

    cout << index1 << " contents: "<< endl;
    di1->printStatus( );
    cout << endl;

    r_Minterval cd = di->getCurrentDomain( );

    r_Minterval badQuery1("[33:43,139:149]");
    // not bad r_Minterval badQuery1("[65:75,32:42]");

    r_Minterval badQuery2("[139:149,144:154]");

    r_Minterval badQuery3("[43:53,81:91]");
    // not bad r_Minterval badQuery3("[107:117,53:63]");

    r_Minterval badQuery4("[37:47,151:161]");
    // not bad r_Minterval badQuery4("[89:99,11:21]");

    r_Minterval badQuery5("[71:81,113:123]"); // not bad

    // test extreme cases: no intersection at all with the current domain
    r_Minterval badQuery6("[161:162,113:113]");

    // test extreme cases: no intersection at all with the current domain
    r_Minterval badQuery7("[170:172,140:140]");

    for( int i = 0; i < NumIntersections; i++ )
    {
        Timer time;
        Timer time1;
        RMTimer* rtime = new RMTimer( "DirIx", "intersect" );
        RMTimer* rtime1 = new RMTimer( "RegDirIx", "intersect" );
        r_Minterval intRegion( 2 );
        long l1 = cd.get_origin( )[0];
        long l2 = cd.get_origin( )[1];
        long h1 = cd.get_high( )[0];
        long h2 = cd.get_high( )[1];
        long length1 = h1 - l1 +1;
        long length2 = h2 - l2 +1;
        l1 = l1 + rand( ) % length1;
        l2 = l2 + rand( ) % length2;
        h1 = l1 + (rand( )  % (h1-l1+1) );
        h2 = l2 + (rand( )  % (h2-l2+1) );
        intRegion << r_Sinterval( l1,h1) << r_Sinterval( l2,h2);

        /*
            if ( i == NumIntersections - 1 )
               intRegion = badQuery1;
            if ( i == NumIntersections - 2 )
               intRegion = badQuery2;
            if ( i == NumIntersections - 3 )
               intRegion = badQuery3;
            if ( i == NumIntersections - 4 )
               intRegion = badQuery4;
            if ( i == NumIntersections - 5 )
               intRegion = badQuery5;
        */
        if ( i == NumIntersections - 6 )
            intRegion = badQuery6;
        if ( i == NumIntersections - 7 )
            intRegion = badQuery7;


        cout << "Intersect with "<<  intRegion << endl;

        // Index
        time.start( );
        rtime->start( );
        vector< Tile* >* rqResult = di->intersect( intRegion );
        rtime->stop( );
        time.stop( );
        if ( rqResult )
        {
            cout << index << endl << " No. of tiles, time , time/noTiles = "
                 << rqResult->size( ) << " , "<< time << " , ";
            if ( rqResult->size( ) )  cout << time.ellapsed_sec( )/rqResult->size( ) << endl;
        }
        else
            cout << "No tiles intersected "<< endl;
        delete rtime;

        // Index 1
        time1.start( );
        rtime1->start( );
        vector< Tile* >* rqResult1 = di1->intersect( intRegion );
        rtime1->stop( );
        time1.stop( );
        if ( rqResult1 )
        {
            cout << index1 << endl << " No. of tiles, time1, time1/noTiles = "
                 << rqResult1->size( ) << " , "<< time1 << " , ";
            if ( rqResult1->size( ) )  cout << time1.ellapsed_sec( )/rqResult1->size( ) << endl;
        }
        else
            cout << "No tiles intersected "<< endl;
        delete rtime1;


        // Index
        cout << "Result " << index << endl;
        if ( rqResult )
        {
            for( int j = 0; j < rqResult->size( ); j++)
                cout << ( *rqResult )[j]->getDomain( ) << endl;
            delete rqResult;
        }

        // Index1
        cout << "Result " << index1 << endl;
        if ( rqResult1 )
        {
            for( j  = 0; j < rqResult1->size( ); j++)
                cout << ( *rqResult1 )[j]->getDomain( ) << endl;
            delete rqResult1;
        }
        cout << endl;
    }

    for( i = 0; i < NumIntersections; i++ )
    {
        Timer time;
        Timer time1;
        r_Point pnt(2);
        RMTimer* rtime = new RMTimer( "DirIx", "Point query" );
        RMTimer* rtime1 = new RMTimer( "RegDirIx", "Point query" );
        long l1 = cd.get_origin( )[0];
        long l2 = cd.get_origin( )[1];
        long length1 = cd.get_high( )[0] - l1;
        long length2 = cd.get_high( )[1] - l2;
        l1 = l1 + rand( ) % length1;
        l2 = l2 + rand( ) % length2;
        pnt << l1 << l2;

        cout << "Point query with "<<  pnt << endl;


        // Index
        time.start( );
        rtime->start( );
        Tile* rqResult = di->containPointQuery( pnt );
        rtime->stop( );
        time.stop( );
        cout << index << " tile, time   = "
             << rqResult->getDomain( ) << " , "<< time << endl;
        delete rtime;

        // Index 1
        time1.start( );
        rtime1->start( );
        Tile* rqResult1 = di1->containPointQuery( pnt );
        rtime1->stop( );
        time1.stop( );
        cout << index1 << " tile, time1  = "
             << rqResult1->getDomain( ) << " , "<< time1 << endl;
        delete rtime1;

        cout << endl;
    }

    delete di;
    delete di1;
}

/*************************************************************
 * Function......: testAccessing()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: reads DBDirIx's and shows contents
 ************************************************************/

static void testAccessing()
{
    DBMDDObjIxId accessedIndex;

    cout << "....testAccessing"<<endl;

    // read root object
    d_List< DBMDDObjIxId > indexList("IndexContainer");
    // used for iterating
    d_Iterator< DBMDDObjIxId > indexIt = indexList.create_iterator();

    for( int i = 1 ; indexIt.not_done(); i++, indexIt.advance())
    {
        accessedIndex = indexIt.get_element();
        cout << "    --"<<i<<". index object in list:" << endl;
        accessedIndex->printStatus();
        cout<<endl;
    }

}


/*************************************************************
 * Function......: clearDB( d_Database &DB )
 *
 * Arguments.....: none
 *   DB: reference to a d_Database-object to use
 * Return value..: none
 * Description...: delete the O2-base (in case it already
 *                 existed) and recreates an empty base
 ************************************************************/

static void ClearDB( d_Database &DB )
{
    d_Transaction trans;
    trans.begin();

    cout << "Destroying " << O2BenchDBName << endl;

    // destroy the database in case it already exists
    DB.destroy( O2BenchDBName );

    // and create a new one
    cout << "Creating " << O2BenchDBName <<" on schema "
         << O2BenchSchemaName << endl;
    DB.create( O2BenchDBName, O2BenchSchemaName );

    trans.commit();
}
