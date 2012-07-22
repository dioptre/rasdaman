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
 * SOURCE: test_persmddcoll.cc
 *
 * MODULE: test for mddobj of cachetamgr
 *
 * PURPOSE:
 *   creates a persistent collection of objects and iterators.
 *
 * COMMENTS:
 *   none
 *
*/

#include "mymalloc/mymalloc.h"

#include <stdlib.h>
#include <iostream>

#define TEST_PROTECTED
#define TEST_PRIVATE


#include "ulongtype.hh"

#include "tilemgr/persmddcoll.hh"
#include "tilemgr/persmddobj.hh"
#include "tilemgr/perstile.hh"

#include "tilemgr/persmddcolliter.hh"

#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"

#include "raslib/rminit.hh"
#include "typefactory.hh"

#include "mddbasetype.hh"
#include "mdddomaintype.hh"
#include "settype.hh"

extern char* myExecArgv0 = "";

RMINITGLOBALS('C')

static char* O2DBName;
char *collName;
char defaultCollName[]= "ObjsContainer";
static int createMDDColl( const char* collName, DatabaseIf* db );
static void ClearDB( d_Database &DB );
static void testAccessing( );
static void testConstructors( );
static void testRemove( );
static void testSearch( );
static void testGetFunctions( );
static void testLaterInsert( );
PersTile* createPersTile( const r_Minterval& dom, const BaseType* type );
static int openCreateDB( const char* O2DBName, DatabaseIf& database );

TransactionIf ta;

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
    DatabaseIf database1;

    if( argc < 2 )
    {
        cout << "Usage: test_persmddcoll <database> [collName]" << endl;
        return -1;
    }
    O2DBName = argv[1];
    if ( argc == 3 ) collName = argv[2];
    else
        collName = defaultCollName;

    // don't forget to initialize before using AdminIf!
    myExecArgv0 = argv[0];
    AdminIf* myAdmin = AdminIf::instance();

    // connect to the database
    cout << "Connecting to database " << O2DBName
         << "..." << endl;

    int errorDBOpen = openCreateDB( O2DBName, database );
    if ( errorDBOpen < 0 )
    {
        cout << "Failed at opening newly created database " << errorDBOpen << endl;
        cout << "Exiting " << endl;
        return errorDBOpen;
    }



    cout << endl << "Deleting root object from the database ..." <<endl;
    ta.begin( &database );
    int i = PersMDDColl::destroyRoot( collName, &database );
    cout << " i == " << i << endl;
    cout << " &database = " << long( &database) << endl;
    // database.destroyRootObj( collName );
    ta.commit( );


    ta.begin( &database);
    createMDDColl( collName, &database );
    ta.commit( );

    // create collection and objects and put them in the collection
    cout << endl << "Populate collection ..." << endl;
    ta.begin( &database  );
    testConstructors( );
    ta.commit( );

    // read coll and print contents
    cout << endl << "Read collection and print contents..." << endl;
    ta.begin( &database  );
    testAccessing( );
    ta.commit( );

    // connect to the database
    cout << "Closing database " << O2DBName
         << "..." << endl;
    database.close( );

    // connect to the database
    cout << endl << "Connecting to database SecondBase "
         << "..." << endl;
    // database1.open( "SecondBase" );

    errorDBOpen = openCreateDB( "SecondBase", database1 );
    if ( errorDBOpen < 0 )
    {
        cout << "Failed at opening database SecondBase " << errorDBOpen << endl;
        cout << "Exiting " << endl;
        return errorDBOpen;
    }

    // read coll and print contents
    cout << endl << "Read collection and print contents..." << endl;
    cout << "Read only? "<< endl;
    int ro;
    cin >> ro;
    ta.begin( &database1, ro );
    cout << endl << " testAccessing before "<<endl;
    testAccessing( );
    ta.commit( );

    /*
      // add some new tiles
      cout << endl << "Insert more tiles to the existing objects" << endl;
      ta.begin( &database );
      testLaterInsert( );
      ta.commit( );

      // read coll and print contents
      cout << endl << "Read collection and print contents..." << endl;
      ta.begin( &database );
      testAccessing( );
      ta.commit( );


       // search for an interval
      cout << endl << "Test region search ..." << endl;
      ta.begin( &database );
      testSearch( );
      ta.commit( );

       // search for an interval
      cout << endl << "Test get functions of MDDObj ..." << endl;
      ta.begin( &database );
      testGetFunctions( );
      ta.commit( );

      // test remove operation and print contents
      cout << endl << "Test remove ..." << endl;
      ta.begin( &database );
      testRemove( );
      ta.commit( );

      // read coll and print contents
      cout << endl << "Read collection and print contents..." << endl;
      ta.begin( &database );
      testAccessing( );
      ta.commit( );
    */
    cout << endl << "Ending O2 session..." << endl;
    database.close( );
    delete myAdmin;
}
/*************************************************************
 * Function......: testConstructors()
 *
 ************************************************************/
PersTile*
createPersTile( const r_Minterval& dom, const BaseType* type )
{
    char* cells = (char*)mymalloc(dom.cell_count() * type->getSize());
    PersTile* t = new PersTile( dom, type, cells );
    return t;
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
    const BaseType* ulongTypeObj = TypeFactory::mapType("ULong");
    const BaseType* boolTypeObj = TypeFactory::mapType("Bool");
    char* uLongCells;
    char* boolCells;

    const MDDBaseType* mType1 =
        (const MDDBaseType* ) TypeFactory::mapMDDType("TestSMDomainType2D");
    const MDDBaseType* mType2 =
        (const MDDBaseType* ) TypeFactory::mapMDDType("TestSMDomainType3D");

    cout << "....testConstructors"<< endl;


    // read root object

    try
    {
        PersMDDColl objsSet1("WrongName");
    }
    catch (...)
    {
        cout <<"Error caught ................."<< endl;
    }

    PersMDDColl objsSet(collName);

    /*
      // test for crash of O2 iterator reset in case the collection is empty
      cout << "createIterator " << endl;
      MDDCollIter* it = objsSet.createIterator( );
      // cout <<  " Iterator not Done returns " << it->notDone( );
      // if ( it->notDone( ) ) it->reset( );
      cout << " Iterator reset " << endl;
      it->reset( );
      // cout << " Testing advance on an empty collection" << endl;
      // it->advance( ); // OK
      // cout <<" Testing get_element on an empty collection" << endl;
      // MDDObj* testIterMDDObj = it->getElement( ); // FATAL ERROR
      // cout <<" get_element returned " << testIterMDDObj << endl;
    */

    // create MDD Object 1

    cout << "    mddObj1" << endl;
    cout << "       tile 1 = nil, 10-12, 20-24 "<< endl;
    r_Sinterval limits1Obj1(10l,12l);
    r_Sinterval limits2Obj1(20l,24l);
    r_Minterval dom(2);
    dom << limits1Obj1 << limits2Obj1;


    r_Minterval tmpInt =  *( ( MDDDomainType* ) mType1 )->getDomain( );
    PersMDDObj* MDDObj1 = new PersMDDObj( mType1, tmpInt );

    PersTile* tile1Obj1 = createPersTile( dom, ulongTypeObj);
    MDDObj1->insertTile(tile1Obj1);

    cout << "       tile 2 = nil, 0-400, 22-24 "<< endl;
    dom[0].set_interval(0l,400l);
    dom[1].set_interval(22l,24l);
    PersTile* tile2Obj1 = createPersTile( dom, ulongTypeObj);
    MDDObj1->insertTile(tile2Obj1);

    cout << "       tile 3 = nil, 0-600, 10-1000 "<< endl;
    dom[0].set_interval(0l,600l);
    dom[1].set_interval(10l,1000l);
    PersTile* tile3Obj1 = createPersTile( dom, ulongTypeObj);
    MDDObj1->insertTile(tile3Obj1);

    cout << " MDDObj1 == isPersistent:" << MDDObj1->isPersistent( )<< ";" ;
    MDDObj1->printStatus( );
    cout << endl;

    objsSet.insert(MDDObj1);


    // create MDD Object

    cout << "    mddObj2 "<< endl;
    cout << "       tile 1 = nil, 0-19, 20-59, 30-59 "<< endl;
    r_Sinterval limits1Obj2(0l,19l);
    r_Sinterval limits2Obj2(20l,59l);
    r_Sinterval limits3Obj2(30l,59l);
    r_Minterval dom2(3);
    dom2 << limits1Obj2 << limits2Obj2 << limits3Obj2;
    // PersMDDObj* MDDObj2 = new PersMDDObj( dom2, "Bool");

    tmpInt = *( ( MDDDomainType* ) mType2 )->getDomain( );
    PersMDDObj* MDDObj2 = new PersMDDObj( mType2, tmpInt );

    PersTile* tile1Obj2 = createPersTile( dom2, boolTypeObj);
    MDDObj2->insertTile( tile1Obj2 );

    cout << "       tile 2 = nil, 20-39, 60-79, 60-89 "<< endl;
    dom2[0].set_interval(20l,39l);
    dom2[1].set_interval(60l,79l);
    dom2[2].set_interval(60l,89l);
    PersTile* tile2Obj2 = createPersTile( dom2, boolTypeObj);

    MDDObj2->insertTile(tile2Obj2);

    cout << " MDDObj2 == isPersistent:" << MDDObj2->isPersistent( )<< ";" ;
    MDDObj2->printStatus( );
    cout << endl;

    objsSet.insert(MDDObj2);

    objsSet.releaseAll( );

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
    PersMDDObj* accessedObj;

    cout << "....testAccessing"<<endl;
    try
    {
        PersMDDColl objsSet(collName);

        // To test PersMDDColl::printStatus( )
        // objsSet.printStatus( );

        // To test PersMDDObj::printStatus( ), MDDCollIter::createIterator( ) and
        // MDDCollIter methods :


        cout << "Iterating through the collection with PersMDDCollIter " << endl;
        MDDCollIter* objsIt = objsSet.createIterator( );

        for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
        {
            accessedObj = (PersMDDObj*) objsIt->getElement();
            cout << i<<". MDD object in set:" << endl;
            accessedObj->printStatus();
            cout << endl << endl;
        }
        delete objsIt;
        objsSet.releaseAll( );
    }
    catch(...)
    {
        cout << "Error opening collection !!!! "<<endl;
        exit(-1);
    }
}

/*************************************************************
 * Function......: testLaterInsert()
 *
 * Description...:
 ************************************************************/

static void testLaterInsert()
{
    PersMDDObj* accessedObj;

    cout << "....testAccessing"<<endl;

    PersMDDColl objsSet(collName);

    // To test PersMDDColl::printStatus( )
    // objsSet.printStatus( );

    // To test PersMDDObj::printStatus( ), MDDCollIter::createIterator( ) and
    // MDDCollIter methods :


    cout << "Iterating through the collection with PersMDDCollIter " << endl;
    MDDCollIter* objsIt = objsSet.createIterator( );


    PersTile *t, *t2, *t3;
    for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
    {
        accessedObj = (PersMDDObj*) objsIt->getElement();
        switch( accessedObj->getDimension( ) )
        {
        case 2 :
            t2 = new PersTile( r_Minterval("[40:60,80:100]"),
                               accessedObj->getCellType( ) );
            t = t2;
            break;
        case 3 :
            t3 = new PersTile(r_Minterval("[40:60,80:100,0:20]"),
                              accessedObj->getCellType( )  );
            t = t3;
            break;
        default:
            cout << "Error Dimensionality not expected" << endl;
            break;
        }
        accessedObj->insertTile(t);
        cout << i<<". MDD object in set:" << endl;
        accessedObj->printStatus();
        cout << endl << endl;
    }
    delete objsIt;
    objsSet.releaseAll( );
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

    MDDObj* accessedObj;

    cout << "....testSearch"<<endl;

    PersMDDColl objsSet(collName);

    MDDCollIter* objsIt = objsSet.createIterator( );

    for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
    {
        accessedObj =  objsIt->getElement();

        cout << "Accessed Object " << endl;
        accessedObj->printStatus( );
        cout << endl;

        if (i == 1 || i == 2)
        {
            r_Minterval searchInt1(2);
            r_Minterval searchInt2(3);
            vector< Tile* >* entriesList;

            cout << "    -- " << i << ". MDD object in list. Search for:";
            switch (i)
            {
            case 1:
                searchInt1[0].set_interval(10l,20l);
                searchInt1[1].set_interval(10l,30l);
                cout << " 10-20, 10-30" << endl;
                entriesList = accessedObj->intersect(searchInt1);
                break;
            case 2:
                searchInt2[0].set_interval(10l,20l);
                searchInt2[1].set_interval(10l,30l);
                searchInt2[2].set_interval(40l,50l);
                cout << " 10-20, 10-30, 40-50" <<endl;
                entriesList = accessedObj->intersect(searchInt2);
                break;
            default:
                break;
            }
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
                    cout << tileInterval[i].low() << "-"
                         << tileInterval[i].high() <<", ";
                cout << endl;

                entryIt++;
            }
            delete entriesList;
        }
    }
    delete objsIt;
    objsSet.releaseAll( );
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
    PersMDDObj* accessedObj;

    cout << "....testGetFunctions"<<endl;

    PersMDDColl objsSet(collName);

    MDDCollIter* objsIt = objsSet.createIterator( );

    vector< Tile* >* entriesList;

    for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
    {
        r_Minterval currDom;
        r_Minterval defDom;

        cout << "  " << i << ". Object" << endl;
        accessedObj =  (PersMDDObj*) objsIt->getElement();


        defDom = accessedObj->getDefinitionDomain( );
        cout << "     GetDefinitionDomain result: ";
        defDom.print_status( );
        cout << endl;

        currDom = accessedObj->getCurrentDomain( );
        cout << "     GetCurrentDomain result:  ";
        currDom.print_status( );
        cout << endl;

        entriesList = accessedObj->getTiles( );
        cout << "     -- GetTiles result: " << endl;
        vector<Tile*>::iterator entryIt = entriesList->begin();

        while (entryIt !=  entriesList->end())
        {
            // (*entryIt)->printStatus();
            r_Minterval tileInterval = (*entryIt)->getDomain();
            int dimensionality = tileInterval.dimension();

            cout << "    PersTile ";
            cout << " domain == " << dimensionality << ": ";
            for (int i = 0; i <dimensionality; i++)
                cout << tileInterval[i].low() << "-" << tileInterval[i].high() <<", ";
            cout << endl;
            entryIt++;
        }
        delete entriesList;
    }
    delete objsIt;
    objsSet.releaseAll( );
}

/*************************************************************
 * Function......: testRemove()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: reads DirTilesIx's and shows contents
 ************************************************************/

static void testRemove()
{
    PersMDDObj* accessedObj;

    cout << "....testRemove"<<endl;

    PersMDDColl objsSet(collName);
    // PersMDDColl objsSet("Qualquercoisa");
    // To test PersMDDColl::printStatus and PersMDDColl::remove


    MDDCollIter* objsIt = objsSet.createIterator( );

    cout << "-- Remove second element from collection " << endl;

    for( int i = 1 ; objsIt->notDone( ) && i < 2; i++, objsIt->advance( ))
    {
        accessedObj = (PersMDDObj*) objsIt->getElement();
    }
    cout << "Delete of objsIt:" << endl;
    delete objsIt;
    cout << "Finished Delete of objsIt." << endl;
    cout << "Remove accessedObj:" << endl;
    objsSet.remove( accessedObj );
    cout << "Finished Remove accessedObj." << endl;
}

/*************************************************************
 * Function......: openCreateDB()
 *
 ************************************************************/

static int openCreateDB( const char* O2DBName, DatabaseIf& database )
{
    cout << "openCreateDB " << O2DBName << endl;

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
        cout << endl << "Connecting to database " << O2DBName
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
    return errorDBOpen;
}

/*************************************************************
 * Function......: createMDDColl( )
 ************************************************************/
static int
createMDDColl( const char* collName, DatabaseIf* db )
{
    MDDDomainType* mType1 = 0;
    MDDDomainType* mType2 = 0;
    MDDType* mt = 0;
    CollectionType* collType1 = 0;

    const BaseType* ulongTypeObj = TypeFactory::mapType("ULong");

    const MDDDomainType* cmType1 =
        ( MDDDomainType* ) TypeFactory::mapMDDType( "TestSMDomainType2D" );
    const MDDDomainType* cmType2 =
        ( MDDDomainType* ) TypeFactory::mapMDDType( "TestSMDomainType3D" );

    const CollectionType* collType =
        (CollectionType*)TypeFactory::mapSetType( "ObjsContainerType" );


    if( !cmType1 || !cmType2 || !collType )
    {
        char name1[] = "TestSMDomainType2D";
        char name2[] = "TestSMDomainType3D";

        r_Sinterval limits1Obj1(0l,1000l);
        r_Sinterval limits2Obj1(0l,800l);
        r_Minterval dom1(2);
        dom1 << limits1Obj1 << limits2Obj1;
        cout << "MDD Type 1 , domain "<< dom1 << endl;

        cout << "       tile 1 = nil, 0-19, 20-59, 30-59 "<< endl;
        r_Sinterval limits1Obj2(0l,19l);
        r_Sinterval limits2Obj2(20l,59l);
        r_Sinterval limits3Obj2(30l,59l);
        r_Minterval dom2(3);
        dom2 << limits1Obj2 << limits2Obj2 << limits3Obj2;
        cout << "MDD Type 2 , domain "<< dom2 << endl;

        // MDDDomainType* mType1 =
        mType1 =
            new  MDDDomainType((char*) name1, ( BaseType* ) ulongTypeObj, dom1 );
        // MDDDomainType* mType2 =
        mType2 =
            new  MDDDomainType((char*) name2, ( BaseType* ) ulongTypeObj, dom2 );

        cout << "MDD Type1 == ";
        mType1->print_status( cout  );
        cout << endl;
        cout << "MDD Type2 == ";
        mType2->print_status( cout );
        cout << endl;

        TypeFactory::addMDDType( mType1 );
        TypeFactory::addMDDType( mType2 );

        if ( !collType )
        {
            cout << "Collection type newly created " << endl;
            // MDDType* mt = new MDDType( );
            mt = new MDDType( );
            cout << "MDDtype created "<< endl;
            collType1 = new SetType(  "ObjsContainerType", mType1 );
            cout << "Set Type created ... ";
            collType = collType1;
            TypeFactory::addSetType( (SetType*)  collType );
            cout <<" and added "<< endl;

        }

    }
    cout << "Creating root collection" << endl;

    OId oColl;
    if( OId::allocateMDDCollOId(  &oColl ) == 0 )
        cout <<"Successfully allocated OId for collection " << oColl << endl;
    else
        cout <<"Error allocating OId for collection " << endl;

    PersMDDColl* col;
    try
    {
        // CollectionType* ct = TypeFactory::mapSetType( "ObjsContainerType" );
        col = PersMDDColl::createRoot( collName, oColl, collType, db );
    }
    catch (...)
    {
        cout <<"Error caught ................."<< endl;
        return -1;
    }

    cout << "Committing TA ..."<< endl;
    ta.commit( );

    if ( col ) delete col;
    cout << "Col freed . "<< endl;

    ta.begin( db );
    return 0;
}
