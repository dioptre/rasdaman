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
*/
/*************************************************************
 *
 *
 * PURPOSE:
 * Test program for the RasQL parser.
 *
 *
 * COMMENTS:
 *
 *
 ************************************************************/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "qlparser/symtab.hh"
#include "raslib/template_inst.hh"
#endif
#endif


using namespace std;

#define DEBUG_MAIN
#include "debug-clt.hh"


#include <iostream.h>      // cout
#include <stdio.h>         // fopen, perror
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "qlparser/querytree.hh"

#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"

#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"

#include "mddmgr/mddobj.hh"
#include "mddmgr/mddcoll.hh"
#include "mddmgr/mddcolliter.hh"

// init globals for server initialization
// RMINITGLOBALS('C')

extern int   yyparse();
extern char* myExecArgv0 = "";
extern char* beginParseString;
extern char* iterParseString;

extern QueryTree* parseQueryTree;

int main( int ac, char** av )
{
    FILE* inFile;
    char  baseName[255];
    char  query[4096];

    //
    // read program arguments
    //

    if( ac <= 2 )
    {
        cout << "usage: test_qlparser basename queryfile" << endl;
        return -1;
    }

    strcpy( baseName, av[1] );

    inFile = fopen( av[2], "r" );

    if( inFile == NULL )
    {
        cout << "Error opening file " << av[1] << endl;
        return -1;
    }

    fread( &query, 1, 4095, inFile );
    fclose( inFile );

    cout << "Query:" << endl << endl << query << endl;

    //
    // open database, start transaction
    //

    // variables representing O2 database, ta and session
    DatabaseIf    db;
    TransactionIf ta;

    // don't forget to initialize before using AdminIf!
    cout << "Connecting to O2 ..." << flush;
    myExecArgv0 = av[0];
    AdminIf* myAdmin = AdminIf::instance();
    cout << "OK" << endl;

    // connect to the database
    cout << "Opening database " << baseName << "... " << flush;
    db.open( baseName );
    cout << "OK" << endl;

    cout << "Starting transaction ... " << flush;
    ta.begin(&db);
    cout << "OK" << endl;

    //
    // body of test program
    //

    beginParseString = query;
    iterParseString = query;

    parseQueryTree = new QueryTree();   // create a query tree object...

    cout << endl << "Parsing ..." << flush;

    if( !yyparse() )
        cout << "  worked" << endl;
    else
        cout << "  failed" << endl;

    cout << endl;
    parseQueryTree->printTree(2);
    cout << endl;
    parseQueryTree->getRoot()->printAlgebraicExpression();
    cout << endl;

    cout << "Simplifying ..." << flush;
    try
    {
        parseQueryTree->getRoot()->simplify();
    }
    catch( ParseInfo& info )
    {
        info.printStatus( RMInit::logOut );
        cout << "FAILED" << endl;
    }

    cout << "OK" << endl;

    cout << endl;
    parseQueryTree->printTree(2);
    cout << endl;
    parseQueryTree->getRoot()->printAlgebraicExpression();
    cout << endl;

    delete parseQueryTree;

    //
    // end of body
    //

    cout << "Committing transaction ... " << flush;
    ta.commit();
    cout << "OK" << endl;

    cout << "Closing database ..." << flush;
    db.close();
    cout << "OK" << endl;
    cout << "Ending O2 session ..." << endl;
    delete myAdmin;
    cout << "OK" << endl;

    return 0;
}













