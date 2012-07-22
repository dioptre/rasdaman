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
 * COMMENTS:
 *
 *
 *
 ************************************************************/

#define FASTCONNECT
#define DEBUG_MAIN
#define DEBUG
#define __EXECUTABLE__
#include "debug.hh"
#include "template_inst.hh"

#define RMANVERSION 6100
#define RASARCHITECTURE X86

extern unsigned long maxTransferBufferSize = 4000000;
bool udfEnabled = true;
extern char* dbSchema = 0;
extern int   noTimeOut= 0;

#include <iostream>      // cout
#include <vector>
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

char globalConnectId[256]="RASBASE";
char globalDbUser[255] = {0};
char globalDbPasswd[255] = {0};

// init globals for server initialization

extern int   yyparse(void *);
extern char* myExecArgv0 = "";
extern int   globalOptimizationLevel = 4;
extern char* beginParseString;
extern char* iterParseString;

extern int RManDebug;
extern int RManInfo = 0;
extern int rewrite = 0;
extern int loadopt = 0;

extern QueryTree* parseQueryTree;


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


int main( int argc, char** argv )
{
    FILE*  inFile;
    char   baseName[255];
    char   query[4096];
    struct timezone tzp;
    struct timeval  startTime, stopTime, deltaTime;
    int    timeTest;
    int    optionValueIndex;
    int    noOutput;

    if( checkArguments( argc, argv, "-h", optionValueIndex ) )
    {
        cout << "Usage: test_evaluate basename queryfile [options]" << endl;
        cout << "Options: -h             ... this help" << endl;
        cout << "         -l <file>      ... log is printed to <file> (default: server.log)" << endl;
        cout << "         -l                 log is printed to standard out" << endl;
        cout << "         -d <file>      ... debug output is printed to <file> (default: server.dbg)" << endl;
        cout << "         -d                 debug output is printed to standard out" << endl;
        cout << "         -dl n          ... debug level is set to n (0-4, default: 0)" << endl;
        cout << "                            - 0 = no / 4 = maximal debug information" << endl;
        cout << "         -t                 time test is enabled" << endl;
        cout << "         -nooutput      ... no output of result" << endl;
        cout << endl;
        return 0;
    }

    strcpy( baseName, "RASBASE" );

    inFile = fopen( "query", "r" );

    if( inFile == NULL )
    {
        cout << "Error opening query file " << argv[2] << endl;
        return -1;
    }

    fread( &query, 1, 4095, inFile );
    fclose( inFile );

    cout << endl << "Query:" << endl << endl << query << endl;

    //
    // open database, start transaction
    //

    // variables representing O2 database, ta and session
    DatabaseIf    db;
    TransactionIf ta;

    // don't forget to initialize before using AdminIf!
    cout << "Connecting to O2 ..." << flush;
    myExecArgv0 = argv[0];
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
    iterParseString  = query;

    parseQueryTree = new QueryTree();   // create a query tree object...

    if( timeTest )
        gettimeofday (&startTime, &tzp);

    RMInit::logOut << "Parsing..." << flush;

    if( yyparse(NULL) == 0 )
    {
        RMInit::logOut << "OK" << endl << endl;

        parseQueryTree->printTree( 2, RMInit::logOut );
        RMInit::logOut << endl;

        parseQueryTree->getRoot()->printAlgebraicExpression();
        cout << endl << endl;

        RMInit::logOut << "Evaluating... " << flush;

        vector<QtData*>*    transColl = 0;

        try
        {
            transColl = parseQueryTree->evaluateRetrieval();
        }
        catch( ParseInfo& info )
        {
            RMInit::logOut << endl << "Query Execution Error" << endl;
            info.printStatus();
            cout << endl;

            return 0;
        }

        RMInit::logOut << "OK" << endl << endl;

        if( timeTest )
        {
            gettimeofday(&stopTime, &tzp);

            if(startTime.tv_usec > stopTime.tv_usec)
            {
                stopTime.tv_usec += 1000000;
                stopTime.tv_sec--;
            }

            deltaTime.tv_usec = stopTime.tv_usec - startTime.tv_usec;
            deltaTime.tv_sec  = stopTime.tv_sec  - startTime.tv_sec;

            cout << "Time for query processing " << deltaTime.tv_sec << " sec " << deltaTime.tv_usec << " msec " << endl;
        }

        vector<QtData*>::iterator  transIter;
        int collNum;
        collNum = transColl->size();

        cout << "The result collection has " << collNum << " entries." << endl;

        if( transColl != 0 && !noOutput )
        {
            int i;

            for( transIter = transColl->begin(), i=0; transIter != transColl->end(); transIter++, i++ )
            {
                QtData* mddObj = *transIter;

                cout << endl << "    --" << i << ". MDD object in set:" << endl << "   ";
                mddObj->printStatus();

                /*vector<Tile* >* tiles = mddObj->getTiles();
                vector<Tile* >::iterator tileIter;

                for( tileIter = tiles->begin(); tileIter!=tiles->end(); tileIter++ )
                {
                  cout << endl << "   Tile" << endl;
                  (*tileIter)->printStatus();
                }

                // delete the Tile elements of the vector and the vector itself
                // release( tiles->begin(), tiles->end() );
                for( tileIter = tiles->begin(); tileIter!=tiles->end(); tileIter++ )
                  delete *tileIter;

                delete tiles;*/
            }

            // release dynamic memory for the collection (delete MDDObjs)
            // transColl->eraseAll();
            delete transColl;

            // delete transIter;
        };

    }
    else
    {
        RMInit::logOut << "  failed" << endl;
        printf("Error parsing query\n");
    }

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
