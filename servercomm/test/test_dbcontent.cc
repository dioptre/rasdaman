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
 *       Reads the contents of the specified collection and prints
 *      it on the screen.
 *
 * COMMENTS:
 *          none
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

extern char* myExecArgv0 = "";

static void testAccessing( const char* collName );

/*************************************************************
 * Function name.: int main( int argc, char** argv)
 *
 * Arguments.....:
 *   argc: number of arguments given to program
 *   argv: array of char* with arguments
 * Return value..: exit status
 * Description...: none
 ************************************************************/

void
main( int ac, char** av)
{
    char baseName[255];
    char collName[255];

    if( ac > 1 )
        strcpy( baseName, av[1] );
    else
        strcpy( baseName, "RasDaBase" );

    if( ac > 2 )
        strcpy( collName, av[2] );
    else
        strcpy( collName, "Images" );

    pid_t cpid;
    cout << "Parent process id is " << getpid() << endl;

    cpid = fork();

    if( !cpid )
    {
        cout << "Child process id is " << getpid() << endl;
        // sleep(1);
    }

    // variables representing O2 database, ta and session
    DatabaseIf database;
    TransactionIf ta;

    // don't forget to initialize before using AdminIf!
    myExecArgv0 = av[0];
    AdminIf* myAdmin = AdminIf::instance();

    // connect to the database
    cout << getpid() << " Connecting to database " << baseName << "..." << endl;
    database.open( baseName );

    // read coll and print contents
    cout << getpid() << " Read collection and print contents..." << endl;
    ta.begin();
    testAccessing( collName );
    ta.commit();

    cout << getpid() << " closing db ... ";
    cout.flush();
    database.close();
    cout << getpid() << " OK" << endl;

    cout << getpid() << " Ending O2 session..." << endl;
    delete myAdmin;
    cout << getpid() << " OK" << endl;

    return;
}



/*************************************************************
 * Function......: testAccessing()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: reads DirTilesIx's and shows contents
 ************************************************************/

static void testAccessing( const char* collName )
{
    PersMDDObj* accessedObj;

    cout << getpid() << " ....testAccessing" << endl;
    /*
    PersMDDColl objsSet( collName );

    // To test PersMDDColl::printStatus( )
    objsSet.printStatus( );

    // To test PersMDDObj::printStatus( ), MDDCollIter::createIterator( ) and
    // MDDCollIter methods :
    MDDCollIter* objsIt = objsSet.createIterator( );

    for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
    {
      accessedObj = (PersMDDObj*) objsIt->getElement();
      cout << "    --"<<i<<". MDD object in set:" << endl;
      accessedObj->printStatus();
    }
    delete objsIt;
    */
}


