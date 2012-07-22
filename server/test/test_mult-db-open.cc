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
 *
 *
 * COMMENTS:
 *
 ************************************************************/

static const char rcsid[] = "@(#)exportutils,ExportData: $Id: exporttif.cc,v 1.1 2003/12/27 21:44:18 rasdev Exp $";

#ifndef RMANVERSION
#error "Please specify RMANVERSION variable!"
#endif

#ifndef COMPDATE
#error "Please specify the COMPDATE variable!"
/*
COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`

and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
*/
#endif


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif

#include <iostream>
#include <cstdio>
#include <cctype>

using std::vector;
using std::iterator;

#if defined(SOLARIS)
#   include <strings.h>
#else
#   include <cstring>
#endif

#include "mymalloc/mymalloc.h"

// here is the main prog for allocating debug macro vars
#define DEBUG_MAIN
#include "debug.hh"

#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"


// global variables and default settings
// -------------------------------------

r_Database db;
r_Transaction ta;

const char* server          = DEFAULT_SERV;
unsigned int port           = DEFAULT_PORT;
const char* database        = DEFAULT_DB;
const char* user            = DEFAULT_USER;
const char* passwd          = DEFAULT_PASSWD;


void
openDatabase() throw (r_Error)
{
    ENTER( "openDatabase" );

    if (! dbIsOpen)
    {
        cout << "Opening database " <<  database  << " at " << server << ":" << port << "..." << flush;
        db.set_servername(server, port);
        db.set_useridentification( user, passwd);
        TALK( "database was closed, opening database=" <<  database  << ", server=" << server << ", port=" << port << ", user=" <<  user << ", passwd=" << passwd << "..." );
        db.open( database );
        TALK( "done" );
        dbIsOpen = true;
        cout << "ok" << endl << flush;
    }

    LEAVE( "openDatabase" );
}

void
closeDatabase() throw (r_Error)
{
    ENTER( "closeDatabase" );

    if (dbIsOpen)
    {
        TALK( "database was open, closing it" );
        db.close();
        dbIsOpen = false;
    }

    LEAVE( "closeDatabase" );
    return;
}


int main(int argc, char** argv)
{
    SET_OUTPUT( false );        // inhibit unconditional debug output, await cmd line evaluation

    int retval = EXIT_FAILURE;

    cout << argv[0] << ": rasdaman test 'failure upon multiple db open' for rasdaman v" << RMANVERSION/1000 << " -- generated on " << COMPDATE << endl;

    try
    {
        openDatabase();

        closeDatabase();

        retval = EXIT_SUCCESS;
    }
    catch (ExportError& e)
    {
        cout << argv[0] << ": " << e.what() << endl;
        retval = EXIT_FAILURE;
    }
    catch (const r_Error& e)
    {
        cout << argv[0] << ": rasdaman error " << e.get_errorno() << ": " << e.what() << endl;
        retval = EXIT_FAILURE;
    }
    catch (...)
    {
        cout << argv[0] << ": panic: unexpected internal exception." << endl;
        retval = EXIT_FAILURE;
    }

    cout << argv[0] << " done." << endl;

    return retval;
}

