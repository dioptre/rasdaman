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
 * SOURCE: test_mddobj.cc
 *
 * MODULE: test for mddobj of mddmgr
 *
 * COMMENTS:
 *   in order to compile this test program, getDBMDDObjId from
 *   PersMDDObj has to be made public, which is not the normal
 *   and expected case.
 *
*/

#include <stdlib.h>
#include <iostream>
#include "mddmgr/perstile.hh"
#include "ulongtype.hh"
#include "mddmgr/persmddobj.hh"
#include "mddmgr/persmddcoll.hh"
#include "mddmgr/mddcoll.hh"
#include "mddmgr/mddcolliter.hh"


#include "raslib/rminit.hh"
RMINITGLOBALS('C')

#include "mddif/dbmddobj.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"

// perhaps used later
static char O2BenchDBName[]  = "PaulaRasDaBase";
static char O2BenchSchemaName[] = "SMRasDaSchema";

static void ClearDB( d_Database &DB );
static void testAccessing();
static void testConstructors();
static void testSearch();
static void testGetFunctions();
static void printInterval(r_Minterval* inter);
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
    cout << "Deleting TestMDDObjContainer of database..." << endl;
    database.open( O2BenchDBName);
    ta.begin( );
    Handle collHandle = 0;
    collHandle = o2_get_root( "TestMDDObjContainer" );
    if ( collHandle )
    {
        // I don't know if this is needed.
        o2_unref_handle ( collHandle );
        database.destroy_persistent_root( "TestMDDObjContainer" );
    }
    ta.commit( );

    // connect to the database
    cout << "Connecting to database " << O2BenchDBName
         << "..." << endl;
    database.open( O2BenchDBName );

    // create root collection
    cout << "Creating root collection..." << endl;
    ta.begin();
    database.create_persistent_root( "TestMDDObjContainer",
                                     "d_List< d_Ref<DBMDDObj> >",
                                     OL_CONSTANT);
    ta.commit();

    // create indexes and put them in TestMDDObjContainer
    cout << "Create objects and put in TestMDDObjContainer..." << endl;
    ta.begin();
    testConstructors();
    ta.commit();

    // read index and print contents
    cout << "Read objects and print contents..." << endl;
    ta.begin();
    testAccessing();
    ta.commit();

    // test search operation and print contents
    cout << "Search a rectangular region and print contents..." << endl;
    ta.begin();
    testSearch();
    ta.commit();

    /*
      // test get member functions
     cout << "Get * from object and print result..." << endl;
     ta.begin();
     testGetFunctions();
     ta.commit();
    */

    cout << "Ending O2 session..." << endl;
    session.end();
    database.close();
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
    char anyCell[] = {0,0,0,0};
    unsigned long MDDObj1sz = 0;

    cout << "....testConstructors"<< endl;

    // read root object
    d_List< d_Ref<DBMDDObj> > objsList("TestMDDObjContainer");

    // create MDD Object
    cout << "    mddObj1" << endl;

    cout << "       tile 1 = nil, 10-12, 20-24 "<< endl;
    r_Sinterval limits1Obj1(10l,12l);
    r_Sinterval limits2Obj1(20l,24l);
    r_Minterval dom(2);
    dom << limits1Obj1 << limits2Obj1;
    PersTile* tile1Obj1 = new PersTile( dom, &anyType, (const char*) anyCell);
    MDDObj1sz += tile1Obj1->getSize( );
    PersMDDObj* MDDObj1=new PersMDDObj( dom, "ULong");
    MDDObj1->insertTile(tile1Obj1);

    cout << "       tile 2 = nil, 0-400, 22-24 "<< endl;
    dom[0].set_interval(0l,400l);
    dom[1].set_interval(22l,24l);
    PersTile* tile2Obj1 = new PersTile( dom, &anyType, (const char*) anyCell);

    MDDObj1sz += tile1Obj1->getSize( );
    // MDDObj1->insertTile(tile2Obj1);

    cout << "       tile 3 = nil, 0-600, 10-1000 "<< endl;
    dom[0].set_interval(0l,600l);
    dom[1].set_interval(10l,1000l);
    PersTile*  tile3Obj1= new PersTile( dom, &anyType, (const char*) anyCell);
    MDDObj1sz += tile3Obj1->getSize( );
    // MDDObj1->insertTile(tile3Obj1);

    vector<Tile*>* newTiles = new vector< Tile* >;
    newTiles->push_back(tile2Obj1);
    newTiles->push_back(tile3Obj1);
    MDDObj1->insertTiles(*newTiles);
    delete newTiles;

    objsList.insert_element_last(MDDObj1->getDBMDDObjId());

    cout << "Size of MDDObj1 : " << MDDObj1sz << "  -  " << MDDObj1sz/1024 << endl;
    printMemInfo( );
    cout << "Delete MDDObj1 ..........." << endl;
    delete MDDObj1;
    printMemInfo( );
    cout << endl << endl;


    unsigned long MDDObj2sz = 0;
    cout << "    mddObj2 "<< endl;
    cout << "       tile 1 = nil, 0-19, 20-59, 30-59 "<< endl;
    r_Sinterval limits1Obj2(0l,19l);
    r_Sinterval limits2Obj2(20l,59l);
    r_Sinterval limits3Obj2(30l,59l);
    r_Minterval dom2(3);
    dom2 << limits1Obj2 << limits2Obj2 << limits3Obj2;
    PersTile* tile1Obj2 = new PersTile( dom2, &anyType, (const char*) anyCell);
    MDDObj2sz += tile1Obj2->getSize( );
    PersMDDObj* MDDObj2 = new PersMDDObj( dom2, "ULong");
    MDDObj2->insertTile(tile1Obj2);

    cout << "       tile 2 = nil, 20-39, 60-79, 60-89 "<< endl;
    dom2[0].set_interval(20l,39l);
    dom2[1].set_interval(60l,79l);
    dom2[2].set_interval(60l,89l);
    PersTile* tile2Obj2 = new PersTile( dom2, &anyType, (const char*) anyCell);
    MDDObj2sz += tile2Obj2->getSize( );

    MDDObj2->insertTile(tile2Obj2);

    objsList.insert_element_last(MDDObj2->getDBMDDObjId());

    cout << "Size of MDDObj2 : " << MDDObj2sz << "  -  " << MDDObj2sz/1024 << endl;
    printMemInfo( );
    cout << "Delete MDDObj2 ..........." << endl;
    delete MDDObj2;
    printMemInfo( );
    cout << endl << endl;
}

/*************************************************************
 * Function......: testAccessing()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: reads DirTilesIx's and shows contents
 ************************************************************/

static void testAccessing()
{
    DBMDDObjId accessedObj;

    cout << "....testAccessing"<<endl;

    // read root object
    d_List< DBMDDObjId > objsList("TestMDDObjContainer");
    // used for iterating
    d_Iterator< DBMDDObjId > objsIt = objsList.create_iterator();

    for( int i = 1 ; objsIt.not_done(); i++, objsIt.advance())
    {
        accessedObj = objsIt.get_element();
        cout << "    --"<<i<<". MDD object in list:" << endl;
        accessedObj->printStatus();
        cout << "    -- CellTypeName: " << accessedObj->getCellTypeName( ) << endl;
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

    PersMDDColl mddObjsColl ( "TestMDDObjContainer" );
    MDDObj* accessedObj;
    MDDCollIter* objsIt = mddObjsColl.createIterator( );


    /*
      DBMDDObjId accessedObj;

      cout << "....testSearch"<<endl;

      // read root object
      d_List< DBMDDObjId > objsList("TestMDDObjContainer");
      // used for iterating
      d_Iterator< DBMDDObjId > objsIt = objsList.create_iterator();

    */

    for( int i = 0 ; objsIt->notDone(); i++, objsIt->advance())
    {

        accessedObj = objsIt->getElement();

        if (i == 0 || i == 1)
        {
            vector< Tile* >* entriesList = 0;

            r_Minterval searchInt1(2);
            r_Minterval searchInt2(3);

            printMemInfo( );

            cout << "    -- " << i+1 << ". MDD object in list. Search for:";
            switch (i)
            {
            case 0:
                searchInt1[0].set_interval(10l,20l);
                searchInt1[1].set_interval(10l,30l);
                cout << " 10-20, 10-30" << endl;
                entriesList = accessedObj->intersect(searchInt1);
                break;
            case 1:
                searchInt2[0].set_interval(10l,20l);
                searchInt2[1].set_interval(10l,30l);
                searchInt2[2].set_interval(40l,50l);
                cout << " 10-20, 10-30, 40-50" <<endl;
                entriesList = accessedObj->intersect(searchInt2);
                break;
            default:
                break;
            }

            printMemInfo( );

            cout << "    -- Search result: " << endl;
            vector<Tile*>::iterator entryIt = entriesList->begin();

            while (entryIt !=  entriesList->end())
            {
                // (*entryIt)->printStatus();
                r_Minterval tileInterval = (*entryIt)->getDomain();
                int dimensionality = tileInterval.dimension();

                cout << "   PersTile printStatus";
                cout << "    domain == " << dimensionality << ": ";
                for (int i = 0; i <dimensionality; i++)
                    cout << tileInterval[i].low() << "-" << tileInterval[i].high() <<", ";
                cout << endl << "   Access contents " ;
                char* tileContents = ( *entryIt )->getContents( );

                cout << endl;

                entryIt++;
            }

            printMemInfo( );
            cout << " Release entriesList " << endl;

            delete entriesList;
        }
    }
    delete objsIt;
}

/*************************************************************
 * Function......: testGetFunctions()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: reads Index's and shows contents
 ************************************************************/

static void testGetFunctions()
{

    MDDObj* accessedObj;
    PersMDDColl persMDDObjsColl ( "TestMDDObjContainer" );
    MDDColl* mddObjsColl = &persMDDObjsColl;

    MDDCollIter* objsIt = mddObjsColl->createIterator( );

    vector< Tile* >* entriesList;

    cout << "....testGetTiles"<<endl;

    for( int i = 0 ; objsIt->notDone(); i++, objsIt->advance())
    {
        r_Minterval currDom;
        r_Minterval defDom;

        cout << "  " << i << ". Object" << endl;
        accessedObj = objsIt->getElement( );

        defDom = accessedObj->getDefinitionDomain( );
        cout << "     GetDefinitionDomain result: ";
        printInterval( &defDom );
        cout << endl;

        currDom = accessedObj->getCurrentDomain( );
        cout << "     GetCurrentDomain result:  ";
        printInterval( &currDom );
        cout << endl;

        entriesList = accessedObj->getTiles( );
        cout << "    -- GetTiles result: " << endl;
        vector<Tile*>::iterator entryIt = entriesList->begin();

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
        delete entriesList;
    }
    delete objsIt;
    mddObjsColl->releaseAll( );
}

/*************************************************************
 * Function......:
 *
 * Arguments.....:
 * Return value..:
 * Description...:
 ************************************************************/

static void printInterval(r_Minterval* inter)
{
    for (int i = 0; i <inter->dimension( ); i++)
    {
        (*inter)[i].is_low_fixed( )? cout << (*inter)[i].low( ): cout << "*";
        cout << " - ";
        (*inter)[i].is_high_fixed( )? cout << (*inter)[i].high( ): cout << "*";
        cout << ",";
    }
}

/*************************************************************
 * Function......:
 *
 * Arguments.....:
 * Return value..:
 * Description...:
 ************************************************************/

void printMemInfo( )
{


    // allows to store values in the program
    struct mallinfo meminfo = mallinfo();

    cout << "   Memory Usage Information :  bytes - Kbytes" ;
    cout << endl;

    cout << "      space in arena                 : " << meminfo.arena << " -  " << meminfo.arena/1024 << endl;
    cout << "      number of small blocks         : " << meminfo.smblks << " -  " << meminfo.smblks/1024  << endl;
    cout << "      number of ordinary blocks      : " << meminfo.ordblks  << " -  " << meminfo.ordblks/1024 << endl;
    cout << "      space in free ordinary blocks  : " << meminfo.fordblks  << " -  " << meminfo.fordblks/1024 << endl;
    cout << "      space in used ordinary blocks  : " << meminfo.uordblks  << " -  " << meminfo.uordblks/1024 << endl;

    // cout << "additional space from last call: " << meminfo.uordblks - memUsed < < endl;

}


