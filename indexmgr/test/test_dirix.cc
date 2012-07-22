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
 * SOURCE: test_dirix.cc
 *
 * MODULE: test for DirIx
 *
 * PURPOSE:
 *   instantiates DBDirIx objects and reads them
 *
 * COMMENTS:
 *   none
 *
*/

#include <stdlib.h>
#include <iostream.h>
#include <vector.h>

#define TEST_PROTECTED

#include "o2lib_CC.hxx" // declaration of O2-collection-classes

#include "indexmgr/dirix.hh"
#include "dbdirix.hh"
#include "dbmddobjix.hh"
#include "blobtile.hh"
#include "basetype.hh"
#include "ulongtype.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "cachetamgr/perstile.hh"
#include "cachetamgr/tile.hh"
#include "indexmgr/persdirix.hh"
#include "indexmgr/regdirix.hh"

static char O2BenchDBName[] = "DirIxBase";
// This test program must use a different base because it
// doesn't use catalogif and adminif. It is not a complete RasDaBase.
static char O2BenchSchemaName[] = "TestSMSchema";

extern char* myExecArgv0 = "";

#include "raslib/rminit.hh"
RMINITGLOBALS('C')

static void ClearDB( d_Database &DB );
static void testAccessing();
static void testConstructors();
static void testSearch();

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
    /*
    database.create_persistent_root( "IndexContainer",
                   "d_List<d_Ref<DBDirIx>>",
                   OL_CONSTANT);
    */
    ta.commit();


    // create indexes and put them in IndexContainer
    cout << "Create indices and put in IndexContainer..." << endl;
    ta.begin();
    testConstructors();
    ta.commit();

    // read index and print contents
    cout << "Read indices and print contents..." << endl;
    ta.begin();
    testAccessing();
    ta.commit();

    // test search operation and print contents
    cout << "Search a rectangular region and print contents..." << endl;
    ta.begin();
    testSearch();
    ta.commit();

    cout << "Ending O2 session..." << endl;
    database.close();
    session.end();
}

/*************************************************************
 * Function......: testConstructors()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: constructs Indices and inserts them
 *                 in root collection.
 ************************************************************/

static void testConstructors()
{
    ULongType anyType;
    char anyCell[4];

    cout << "....testConstructors"<< endl;

    // read root object
    d_List<DBMDDObjIxId> indexList("IndexContainer");

    // create Index Object
    cout << "    indexObj1" << endl;

    r_Minterval dom( "[10:12,20:24]");
    cout << "       tile 1 = nil, "<< dom << endl;
    PersTile* tile1Obj1 =
        new PersTile( dom, ( const BaseType* ) &anyType, (const char*) anyCell);

    dom = r_Minterval( "[0:400,22:24]");
    cout << "       tile 2 = nil, "<< dom << endl;
    PersTile* tile2Obj1 =
        new PersTile( dom, ( const BaseType* ) &anyType, (const char*) anyCell);

    dom = r_Minterval( "[0:600,10:1000]");
    cout << "       tile 3 = nil, "<< dom << endl;
    PersTile* tile3Obj1 =
        new PersTile( dom, ( const BaseType* ) &anyType, (const char*) anyCell);

    vector<Tile*> newTiles;
    newTiles.push_back(tile1Obj1);
    newTiles.push_back(tile2Obj1);
    newTiles.push_back(tile3Obj1);

    PersDirIx* pd1 =
        new PersDirIx( tile1Obj1->getDimension( ), ( const BaseType* ) &anyType );
    RegDirIx<PersDirIx, Tile >* indexObj1 = new RegDirIx<PersDirIx, Tile >( pd1 );
    MultiDimIx<Tile>::insertObjectsTransform( newTiles, indexObj1 );
    // indexObj1->insertObjectsTransform( newTiles, indexObj1 );
    indexList.insert_element_last(pd1->getDBMDDObjIxId());

    // create DBDirIx Object

    cout << "    indexObj2 "<< endl;
    r_Minterval dom2( "[0:19,20:59,30:59]");
    cout << "       tile 1 = nil, "<< dom2 << endl;
    PersTile* tile1Obj2 =
        new PersTile( dom2, ( const BaseType* ) &anyType,  (const char*) anyCell);
    PersDirIx* pd2 =
        new PersDirIx( tile1Obj2->getDimension( ), ( const BaseType* ) &anyType );
    RegDirIx<PersDirIx, Tile>* indexObj2 = new RegDirIx<PersDirIx, Tile>( pd2 );
    indexObj2->insertObject( tile1Obj2 );

    dom2 = r_Minterval( "[20:39,60:79,60:89]");
    cout << "       tile 2 = nil, "<< dom2 << endl;
    PersTile* tile2Obj2 =
        new PersTile( dom2, ( const BaseType* ) &anyType, (const char*) anyCell);
    indexObj2->insertObject(tile2Obj2);
    indexList.insert_element_last(pd2->getDBMDDObjIxId());

    // PerDirIx doesn't free tiles
    delete tile1Obj1;
    delete tile2Obj1;
    delete tile3Obj1;

    delete tile1Obj2;
    delete tile2Obj2;
    delete indexObj1;
    delete indexObj2;
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
 * Function......: testSearch()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: reads Index's and shows contents
 ************************************************************/

static void testSearch()
{

    DBMDDObjIxId accessedIndex;
    ULongType anyType;

    cout << "....testSearch"<<endl;

    // read root object
    d_List< DBMDDObjIxId > indexList("IndexContainer");
    // used for iterating
    d_Iterator< DBMDDObjIxId > indexIt = indexList.create_iterator();

    for( int i = 0 ; indexIt.not_done(); i++, indexIt.advance())
    {
        vector< Tile* >* entriesList;

        accessedIndex = indexIt.get_element();

        if (i == 0 || i == 1)
        {
            r_Minterval searchInt1(2);
            r_Minterval searchInt2(3);

            cout << "    -- " << i+1 << ". index object in list. Search for:";
            switch (i)
            {
            case 0:
                searchInt1[0].set_interval(10l,20l);
                searchInt1[1].set_interval(10l,30l);
                cout << " 10-20, 10-30" << endl;
                entriesList = ((d_Ref< DBDirIx>) accessedIndex)->intersect(searchInt1, &anyType);
                break;
            case 1:
                searchInt2[0].set_interval(10l,20l);
                searchInt2[1].set_interval(10l,30l);
                searchInt2[2].set_interval(40l,50l);
                cout << " 10-20, 10-30, 40-50" <<endl;
                entriesList = ((d_Ref<DBDirIx>)accessedIndex)->intersect(searchInt2, &anyType);
                break;
            default:
                break;
            }
            cout << "    -- Search result: " << endl;
            vector<Tile*>::iterator entryIt = entriesList->begin();

            // O2 d_List d_Iterator< d_Ref<TilesIxEntry> > entryIt = entriesList->create_iterator();
            // O2 d_List for ( ; entryIt.not_done() ; entryIt.advance() )
            // O2 d_List     entryIt.get_element()->printStatus();

            while (entryIt !=  entriesList->end())
            {
                // (*entryIt)->printStatus();
                r_Minterval tileInterval = (*entryIt)->getDomain();
                int dimensionality = tileInterval.dimension();

                cout << "   PersTile printStatus";
                cout << "    domain == " << dimensionality << ": ";
                for (int i = 0; i <dimensionality; i++)
                    cout << tileInterval[i].low() << "-" << tileInterval[i].high() <<", ";
                cout << endl;

                entryIt++;
            }
        }

        // release(entriesList->begin(), entriesList->end());
        for( vector<Tile*>::iterator entIt = entriesList->begin();
                entIt != entriesList->end();
                entIt++ )
            delete *entIt;

        delete entriesList;

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
