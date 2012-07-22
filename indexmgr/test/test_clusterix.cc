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
 * SOURCE: test_clusterix.cc
 *
 * MODULE: test for ClusterIx
 *
 * COMMENTS:
 *   none
 *
*/

#include <stdlib.h>
#include <iostream.h>
#include <vector.h>

#include "o2lib_CC.hxx" // declaration of O2-collection-classes

#include "indexmgr/clusterix.hh"
#include "dbclusterix.hh"
#include "dbmddobjix.hh"
#include "blobtile.hh"
#include "basetype.hh"
#include "ulongtype.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "cachetamgr/perstile.hh"
#include "cachetamgr/tile.hh"

static char O2BenchDBName[] = "ClusterIxBase";
// This test program must use a different base because it
// doesn't use catalogif and adminif. It is not a complete RasDaBase.
static char O2BenchSchemaName[] = "TestSMSchema";

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
    database.create_persistent_root( "IndexContainer",
                                     "d_List<d_Ref<DBClusterIx>>",
                                     OL_CONSTANT);
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
    ClusterIx* indexObj2;
    ULongType anyType;
    char anyCell[4];

    cout << "....testConstructors"<< endl;

    // read root object
    d_List<DBMDDObjIxId> indexList("IndexContainer");

    // create Index Object
    cout << "    indexObj1" << endl;

    cout << "       tile 1 = nil, 10-12, 20-24 "<< endl;
    r_Sinterval limits1Obj1(10l,12l);
    r_Sinterval limits2Obj1(20l,24l);
    r_Minterval dom(2);
    dom << limits1Obj1 << limits2Obj1;
    PersTile* tile1Obj1 = new PersTile( dom, &anyType, (const char*) anyCell);

    cout << "       tile 2 = nil, 0-400, 22-24 "<< endl;
    dom[0].set_interval(0l,400l);
    dom[1].set_interval(22l,24l);
    PersTile* tile2Obj1 = new PersTile( dom, &anyType, (const char*) anyCell);

    cout << "       tile 3 = nil, 0-600, 10-1000 "<< endl;
    dom[0].set_interval(0l,600l);
    dom[1].set_interval(10l,1000l);
    PersTile* tile3Obj1 = new PersTile( dom, &anyType, (const char*) anyCell);

    vector<PersTile*> newTiles;
    newTiles.push_back(tile1Obj1);
    newTiles.push_back(tile2Obj1);
    newTiles.push_back(tile3Obj1);

    ClusterIx* indexObj1 = new ClusterIx(newTiles);
    indexList.insert_element_last(indexObj1->getDBMDDObjIxId());

    // create DBclusterix Object

    cout << "    indexObj2 "<< endl;
    cout << "       tile 1 = nil, 0-19, 20-59, 30-59 "<< endl;
    r_Sinterval limits1Obj2(0l,19l);
    r_Sinterval limits2Obj2(20l,59l);
    r_Sinterval limits3Obj2(30l,59l);
    r_Minterval dom2(3);
    dom2 << limits1Obj2 << limits2Obj2 << limits3Obj2;
    PersTile* tile1Obj2 = new PersTile( dom2, &anyType,  (const char*) anyCell);
    indexObj2 = new ClusterIx(tile1Obj2);

    cout << "       tile 2 = nil, 20-39, 60-79, 60-89 "<< endl;
    dom2[0].set_interval(20l,39l);
    dom2[1].set_interval(60l,79l);
    dom2[2].set_interval(60l,89l);
    PersTile* tile2Obj2 = new PersTile( dom2, &anyType, (const char*) anyCell);
    indexObj2->insertTile(tile2Obj2);
    indexList.insert_element_last(indexObj2->getDBMDDObjIxId());

    delete indexObj1;
    delete indexObj2;
}

/*************************************************************
 * Function......: testAccessing()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: reads DBClusterIx's and shows contents
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
                entriesList = accessedIndex->intersect(searchInt1, &anyType);
                break;
            case 1:
                searchInt2[0].set_interval(10l,20l);
                searchInt2[1].set_interval(10l,30l);
                searchInt2[2].set_interval(40l,50l);
                cout << " 10-20, 10-30, 40-50" <<endl;
                entriesList = accessedIndex->intersect(searchInt2, &anyType);
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
