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
/*************************************************************************
 *
 *
 * PURPOSE:
 * test class DatabaseIf.
 *
 *
 * PREREQUISITES:
 * - needs COMPDATE (compilation date) defined, eg. cc -DCOMPDATE=\"`date`\"
 *
 * COMMENTS:
 *
 ***********************************************************************/

static const char rcsid[] = "@(#)reladminif,DatabaseIf: $Id: databaseif.ec,v 1.9 2003/12/27 23:11:43 rasdev Exp $";

using namespace std;

#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.hh"
#include "adminif.hh"
#include "databaseif.hh"
#include "externs.h"

#include "raslib/rmdebug.hh"
#define DEBUG_MAIN
#include "debug.hh"


RMINITGLOBALS('C')


// test environment:
#include "testcenter.h"

#define ELLIPSIS "..."
/// test macro for checking error condition, printing result, and aborting on error
#define CHECK(p)  { cout << "CHECKING " << "p"; \
            if (!(p)) \
            {   cout << TEST_ERROR << endl; \
                return false; \
            } \
            else \
            { \
                cout << TEST_OK << endl; \
            } \
          }

/// ObjectBroker: dummy class to satisfy linker; real ObjectBroker functionality never used here.
class ObjectBroker
{
public:
    void init();
    void deinit();
    bool freeMemory();
};

void ObjectBroker::init()
{
}
void ObjectBroker::deinit()
{
}
bool ObjectBroker::freeMemory()
{
    return true;
}

/// from server/rasserver_main.cc:
char globalConnectId[256];
char globalDbUser[255] = {0};
char globalDbPasswd[255] = {0};

/// doTest(): run the test cases
/// preconditions:
/// database does NOT yet exist
/// @params:
/// dbname  name of database
/// @returns:
/// true    if all tests succeeded
/// false   if some test failed

bool doTest( const char *dbname )
{
    bool result = false;        // overall test result
    DatabaseIf *db = NULL;      // our test candidate

    try             // capture eny eventual exception for reporting
    {
        // --- (1) good scenario

        cout << "DatabaseIf constructor" << ELLIPSIS << flush;
        db = new DatabaseIf();
        cout << "new state is: " << *db;
        CHECK( db != NULL );

        cout << "create" << ELLIPSIS << endl;
        db->createDB( dbname, NULL, NULL );
        CHECK( db->isConnected() );
        CHECK( db->isOpen() );

        cout << "db open" << ELLIPSIS << endl;
        db->open( dbname );
        CHECK( db->isConnected() );
        CHECK( db->isOpen() );

        cout << "db close" << ELLIPSIS << endl;
        db->close();
        CHECK( db->isConnected() );
        CHECK( db->isOpen() );

        cout << "db destroy" << ELLIPSIS << endl;
        db->destroyDB( dbname );
        CHECK( db->isConnected() );
        CHECK( db->isOpen() );

        result = true;
    }
    // FIXME: what else should be caught explicitly?
    catch(r_Error& myErr)
    {
        cout << "caught r_Error exception #" << myErr.get_errorno() << ": " << myErr.what() << endl;
        result = false;
    }
    catch(bad_alloc)
    {
        cout << "caught bad_alloc exception" << endl;
        result = false;
    }
    catch(...)
    {
        cout << "caught unknown exception" << endl;
        result = false;
    }

    return result;
}

int
main(int argc, char *argv[])
{
    const char *progName = argv[0]; // name of this test program

    int RManDebug2 = 5;     // debug trace settings
    int RManModule = 1;     // dito

    char* dbName = "RASBASE";   // default database name
    int result = RC_ERROR;      // overall test exit code
    char *resultText = NULL;    // overall test status string

    // --- parameter evaluation, launch msg
    if (argc == 2)
        dbName = argv[1];
    else if (argc > 2)
    {
        cout << "usage: " << progName << " [dbname]" << endl;
        return( RC_USAGE );
    }
    cout << progName << ", generated on " << COMPDATE << " for rasdaman version " << RMANVERSION << " and base DBMS " << BASEDBSTRING << "; using database '" << dbName << "'" << endl;

    // --- all is fine, let's do the test
    bool allFine = doTest( dbName );

    if (allFine)
    {
        resultText = TEST_OK;
        result = RC_OK;
    }
    else
    {
        resultText = TEST_ERROR;
        result = RC_ERROR;
    }

    // --- done, epilogue
    cout << progName << ": overall test result is " << resultText << endl;

    return( result );
}

