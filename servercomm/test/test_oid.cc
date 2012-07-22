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
 * SOURCE: test_dbcontent.cc
 *
 * MODULE: test
 *
 * PURPOSE:
 *          Reads the contents of the specified collection and prints
 *          it on the screen.
 *
 * COMMENTS:
 *           none
 *
*/

#include <stdlib.h>
#include <iostream.h>

#include "o2template_CC.hxx"         // declaration of O2 ref and coll classes

#include "ulongtype.hh"

#include "cachetamgr/persmddcoll.hh"
#include "cachetamgr/persmddobj.hh"
#include "cachetamgr/perstile.hh"

#include "cachetamgr/persmddcolliter.hh"

#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"

#include <unistd.h>

RMINITGLOBALS('C')

extern char* myExecArgv0 = "";

static void
insertObj( char* dbName, OId o, char* cn )
{
    MDDBaseType* mddType  = (MDDBaseType*)TypeFactory::mapMDDType( "GreyImage" );
    r_Minterval domain( "[0:9,0:9]" );

    //  cout << " " << o << "," << cn << " ... " << flush;

    PersMDDObj* obj = new PersMDDObj( mddType, domain, dbName, o );
    PersMDDColl objsSet( cn );
    objsSet.insert( obj );
    delete obj;
}


static void removeObj( char* dbName, char* collName, OId o )
{
    // open collection
    PersMDDColl* coll = 0;

    char answer = 'n';
    cout << endl << "SCAN (y/n) ?" << flush;
    cin >> answer;

    if( answer == 'y' )
    {
        coll = new PersMDDColl( collName );
        MDDCollIter* collIter = coll->createIterator();
        MDDObj* mddObj;
        for( collIter->reset(); collIter->notDone(); collIter->advance() )
        {
            mddObj = collIter->getElement();
            /*
            if( mddObj->isPersistent() )
            {
              EOId eOId;
              ((PersMDDObj*)mddObj)->getEOId( &eOId );

              cout << "MDD " << eOId.getOId() << flush;
            }
            */

        }
        /*
        cout << endl << "PRINT OBJECT (y/n) ?" << flush;
        cin >> answer;
        if( answer == 'y' )
          mddObj->printStatus( );
        */
        delete collIter;

        coll->releaseAll();
        delete coll;

        cout << endl << "SCAN end" << endl;
    }

    coll = new PersMDDColl( collName );

    if( coll )
    {
        cout <<"o == " << o << " dbName == " << dbName << endl;
        coll->remove( o, dbName );

        coll->releaseAll();
        delete coll;
    }
}


int checkArguments( int argc, char** argv, const char* searchText, int& optionValueIndex )
{
    int found = 0;
    int i=1;

    while( !found && i<argc )
        found = !strcmp( searchText, argv[i++] );

    if( found && i<argc && !strchr(argv[i],'-') )
        optionValueIndex = i;
    else
        optionValueIndex = 0;

    return found;
}


int
main( int ac, char** av)
{
    int optionValueIndex;
    char dbName[255];
    char collName[255];

    if( ac < 3 || checkArguments( ac, av, "-h", optionValueIndex ) )
    {
        cout << "Usage:   test_oid base_name collection_name [options]" << endl << endl;
        cout << "Options: -h  ... this help" << endl;
        cout << endl;
        return 0;
    }

    strcpy( dbName, av[1] );
    strcpy( collName, av[2] );

    // variables representing O2 database, ta and session
    DatabaseIf database;
    TransactionIf ta;

    // don't forget to initialize before using AdminIf!
    myExecArgv0 = av[0];
    AdminIf* myAdmin = AdminIf::instance();

    // connect to the database
    cout << "Connecting to database " << dbName << "..." << flush;
    database.open( dbName );
    cout << "OK" << endl;

    // create collection with one object
    cout << "Create collection ... " << flush;
    ta.begin( &database );
    cout << "getting type ... " << flush;
    CollectionType* collType = (CollectionType*)TypeFactory::mapSetType( "GreySet" );
    cout << "getting oid ... " << flush;
    OId oidColl;
    if( !OId::allocateMDDCollOId(  &oidColl ) ) cout << oidColl << " ... " << flush;
    PersMDDColl* pc = PersMDDColl::createRoot( collName, oidColl, collType, &database );
    delete pc;
    cout << "comitting ... " << flush;
    ta.commit();
    cout << "OK" << endl;

    cout << "Insert object into collection ... " << flush;
    ta.begin( &database );
    cout << "getting oid ... " << flush;
    OId oidMDD;
    if( !OId::allocateMDDOId(  &oidMDD ) ) cout << oidMDD << " ... " << flush;
    cout << "inserting ..." << flush;
    insertObj( dbName, oidMDD, collName );
    cout << "comitting ... " << flush;
    ta.commit();
    cout << "OK" << endl;

    cout << "Remove object again ... " << flush;
    ta.begin( &database );
    cout << "removing oid " << oidMDD << " ... " << flush;
    removeObj( dbName, collName, oidMDD );
    cout << "comitting ... " << flush;
    ta.commit();
    cout << "OK" << endl;

    cout << "Delete collection ... " << flush;
    ta.begin( &database );
    cout << "destroying ... " << flush;
    PersMDDColl::destroyRoot( collName, &database );
    cout << "comitting ... " << flush;
    ta.commit();
    cout << "OK" << endl;

    cout << "Closing db ... " << flush;
    database.close();
    cout << "OK" << endl;

    cout << "Ending O2 session ... " << flush;
    delete myAdmin;
    cout << "OK" << endl;

    return 0;
}

