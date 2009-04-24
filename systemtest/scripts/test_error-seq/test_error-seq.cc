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
 * test behavior on erroneous API calls on a single server
 * (ie, sequential behavior - parallel behavior is tested in
 * the twin program test_rasgeo-par.cc).
 * 
 *
 * PRECONDITIONS:
 * - have a rasdaman server running, with std types inserted
 * - use exactly 1 rasserver
 *
 * COMMENTS:
 * 
 * BUGS:
 *
 ************************************************************/

static const char test_error_seq_rcsid[] = "@(#)test_test_error-seq,test_error-seq.cc: $Id: test_error-seq.cc,v 1.1 2003/12/27 19:30:23 rasdev Exp $";

// error indicator - should eventually go into a central systemtest include
static const char MSG_OK[] = "ok";
static const char MSG_FAILED[] = "error";

/*
COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`
and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
*/
#ifndef RMANVERSION
#error "Please specify RMANVERSION variable!"
#endif

#ifndef COMPDATE
#error "Please specify the COMPDATE variable!"
#endif


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>

using namespace std;

#ifdef __VISUALC__
  #define __EXECUTABLE__
#endif

#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"

#include "rasodmg/ref.hh"
#include "raslib/marraytype.hh"
#include "rasodmg/set.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/oqlquery.hh"

#include "raslib/type.hh"

#include "raslib/minterval.hh"

#include "raslib/primitive.hh"
#include "raslib/complex.hh"
#include "raslib/structure.hh"

#include "raslib/structuretype.hh"
#include "raslib/primitivetype.hh"

#include "cmlparser.hh"

#ifdef __VISUALC__
  #undef __EXECUTABLE__
#endif

// debug facility; relies on -DDEBUG at compile time
// tell debug that here is the place for the variables (to be done in the main() src file)
#define DEBUG_MAIN
#include "debug-clt.hh"

const int MAX_STR_LEN = 255;
const int MAX_QUERY_LEN = 10240;


/// program exit codes
#define EXIT_SUCCESS    0
#define EXIT_USAGE      2
#define EXIT_FAILURE    -1

// parameter names, defaults, and help texts

#define PARAM_HELP_FLAG	'h'
#define PARAM_HELP	"help"
#define HELP_HELP	"show command line switches"

#define PARAM_SERV_FLAG	's'
#define PARAM_SERV	"server"
#define HELP_SERV	"<host-name> rasdaman server"
#define DEFAULT_SERV	"localhost"

#define PARAM_PORT_FLAG	'p'
#define PARAM_PORT	"port"
#define HELP_PORT	"<p> rasmgr port number"
#define DEFAULT_PORT	7001
#define DEFAULT_PORT_STR "7001"

#define PARAM_DB_FLAG	'd'
#define PARAM_DB	"database"
#define HELP_DB		"<db-name> name of database"
#define DEFAULT_DB	"RASBASE"

#define PARAM_USER	"user"
#define HELP_USER	"<user-name> name of user"
#define DEFAULT_USER	"rasguest"

#define PARAM_PASSWD	"passwd"
#define HELP_PASSWD	"<user-passwd> password of user"
#define DEFAULT_PASSWD	"rasguest"

#define PARAM_DEBUG	"debug"
#define HELP_DEBUG	"generate diagnostic output"

// name to be tested in touch()
const char* MDD_TYPE_NAME = "RGBImage";

// global variables and default settings
// -------------------------------------

r_Database db;
r_Transaction ta;

bool dbIsOpen = false;
bool taIsOpen = false;

const char *serverName = DEFAULT_SERV;
r_ULong serverPort = DEFAULT_PORT;  
const char *baseName = DEFAULT_DB;
  
const char *user = DEFAULT_USER;
const char *passwd = DEFAULT_PASSWD;
  
const char *fileName = NULL;
const char *queryString=NULL;

// query result set.
// we define it here because on empty results the set seems to be corrupt which kills the default destructor
r_Set< r_Ref_Any > result_set;

// end of globals

void
parseParams(int argc, char** argv) throw (r_Error)
{
	CommandLineParser    &cmlInter		= CommandLineParser::getInstance();

	CommandLineParameter &clp_help		= cmlInter.addFlagParameter( PARAM_HELP_FLAG, PARAM_HELP, HELP_HELP );

	CommandLineParameter &clp_server	= cmlInter.addStringParameter( PARAM_SERV_FLAG, PARAM_SERV, HELP_SERV, DEFAULT_SERV );
	CommandLineParameter &clp_port		= cmlInter.addStringParameter( PARAM_PORT_FLAG, PARAM_PORT, HELP_PORT, DEFAULT_PORT_STR);
	CommandLineParameter &clp_database      = cmlInter.addStringParameter( PARAM_DB_FLAG, PARAM_DB, HELP_DB, DEFAULT_DB );
	CommandLineParameter &clp_user		= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_USER, HELP_USER, DEFAULT_USER );
	CommandLineParameter &clp_passwd	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_PASSWD, HELP_PASSWD, DEFAULT_PASSWD );

#ifdef DEBUG
	CommandLineParameter &clp_debug		= cmlInter.addFlagParameter( CommandLineParser::noShortName, PARAM_DEBUG, HELP_DEBUG );
#endif

	try
	{
		cmlInter.processCommandLine(argc, argv);
 
		if (cmlInter.isPresent( PARAM_HELP_FLAG ))
		{
			cout << "usage: " << argv[0] << " [options]" << endl;
			cout << "options:" << endl;
			cmlInter.printHelp();
			exit( EXIT_USAGE );
		}
 
		// check optional parameters ====================================================

		// evaluate optional parameter server --------------------------------------
		if (cmlInter.isPresent( PARAM_SERV ))
			serverName = cmlInter.getValueAsString( PARAM_SERV );
 
		// evaluate optional parameter port --------------------------------------
		if (cmlInter.isPresent( PARAM_PORT ))
			serverPort = cmlInter.getValueAsLong( PARAM_PORT );
 
		// evaluate optional parameter database --------------------------------------
		if (cmlInter.isPresent( PARAM_DB ))
			baseName = cmlInter.getValueAsString( PARAM_DB );
 
		// evaluate optional parameter user --------------------------------------
		if (cmlInter.isPresent( PARAM_USER ))
			user = cmlInter.getValueAsString( PARAM_USER );
 
		// evaluate optional parameter passwd --------------------------------------
		if (cmlInter.isPresent( PARAM_PASSWD ))
			passwd = cmlInter.getValueAsString( PARAM_PASSWD );

#ifdef DEBUG
		// evaluate optional parameter MDD type name --------------------------------------
		SET_OUTPUT( cmlInter.isPresent( PARAM_DEBUG ) );
#endif

	}
	catch(CmlException& err)
	{
		cout << argv[0] << ": " << err.what() << endl;
		throw;
	}
} // parseParams()


void
openDatabase() throw (r_Error)
{
	ENTER( "openDatabase -- db is " << (dbIsOpen?"":"not ") << "open." );

	db.set_servername(serverName, serverPort);
	db.set_useridentification(user, passwd);
	TALK( "database was closed, opening database=" << baseName << ", server=" << serverName << ", port=" << serverPort << ", user=" << user << ", passwd=" << passwd << "." );
	db.open(baseName);
	dbIsOpen = true;

	LEAVE( "openDatabase" );
} // openDatabase()

void
closeDatabase() throw (r_Error)
{
	ENTER( "closeDatabase -- db is " << (dbIsOpen?"":"not ") << "open." );

	db.close();
	dbIsOpen = false;

	LEAVE( "closeDatabase" );
	return;
} // closeDatabase()

void
openTransaction(bool readwrite) throw (r_Error)
{
	ENTER( "openTransaction, readwrite=" << (readwrite?"rw":"ro") << ", ta is " << (taIsOpen?"":"not ") << "open." );

	if (readwrite)
		ta.begin(r_Transaction::read_write);
	else
		ta.begin(r_Transaction::read_only);
	taIsOpen = true;

	LEAVE( "openTransaction" );
} // openTransaction()

void 
closeTransaction() throw (r_Error)
{
	ENTER( "closeTransaction: aborting ta; ta is " << (taIsOpen?"":"not ") << "open." );

	ta.abort();
	taIsOpen = false;

	LEAVE( "closeTransaction" );
	return;
} // closeTransaction()

void
touch() throw (r_Error)
{
	ENTER( "touch -- fetching type information for " << MDD_TYPE_NAME << "." );

        char* typeStructure = NULL;

        // get type structure from database just to touch it
	ClientComm *cc = db.getComm();
	if (cc == NULL)
		cout << "got NULL communication object from db...";
	else
	{
		typeStructure = cc->getTypeStructure( MDD_TYPE_NAME, ClientComm::r_MDDType_Type );
		TALK( "type structure is " << typeStructure );
	}

	LEAVE( "touch" );
}

void
doStuff() throw (r_Error)
{
	ENTER( "doStuff" );

	cout << "- good cycle..." << flush;
	openDatabase();
	openTransaction( false );
	touch();
	closeTransaction();
	closeDatabase();
	cout << MSG_OK << endl;

	cout << "- bad cycle: do not close db, reopen..." << flush;
	openDatabase();
	openTransaction( false );
	touch();
	closeTransaction();
	try
	{
		openDatabase();
		cout << MSG_FAILED << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_OK << ": " << e.get_errorno() << ": " << e.what() << endl;
	}
	closeDatabase();

	cout << "- bad cycle: do not close ta, reopen..." << flush;
	openDatabase();
	openTransaction( false );
	touch();
	try
	{
		openTransaction( false );
		cout << MSG_FAILED << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_OK << ": " << e.get_errorno() << ": " << e.what() << endl;
	}
	closeDatabase();

	cout << "- bad cycle: do not open ta, touch..." << flush;
	openDatabase();
	try
	{
		touch();
		closeTransaction();
		cout << MSG_FAILED << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_OK << ": " << e.get_errorno() << ": " << e.what() << endl;
	}
	closeDatabase();

	cout << "- bad cycle: double open ta, touch..." << flush;
	openDatabase();
	openTransaction( false );
	try
	{
		openTransaction( false );
		cout << MSG_FAILED << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_OK << ": " << e.get_errorno() << ": " << e.what() << endl;
	}
	touch();
	closeDatabase();

	cout << "- bad cycle: double open db, touch..." << flush;
	openDatabase();
	try
	{
		openDatabase();
		cout << MSG_FAILED << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_OK << ": " << e.get_errorno() << ": " << e.what() << endl;
	}
	openTransaction( false );
	touch();
	closeDatabase();

	cout << "- bad cycle: double open ta, touch..." << flush;
	openDatabase();
	openTransaction( false );
	try
	{
		openTransaction( false );
		cout << MSG_FAILED << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_OK << ": " << e.get_errorno() << ": " << e.what() << endl;
	}
	touch();
	closeTransaction();
	closeDatabase();

	cout << "- bad cycle: double abort ta (handled gracefully)..." << flush;
	openDatabase();
	openTransaction( false );
	touch();
	closeTransaction();
	try
	{
		closeTransaction();
		cout << MSG_FAILED << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_OK << ": " << e.get_errorno() << ": " << e.what() << endl;
	}
	closeDatabase();

	cout << "- bad cycle: double close db (handled gracefully)..." << flush;
	openDatabase();
	openTransaction( false );
	touch();
	closeTransaction();
	closeDatabase();
	try
	{
		closeDatabase();
		cout << MSG_OK << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_FAILED << ": " << e.get_errorno() << ": " << e.what() << endl;
	}

	cout << "- bad cycle: touch without any db/ta open..." << flush;
	try
	{
		touch();
		cout << MSG_OK << endl;
	}
	catch (r_Error& e)
	{
		cout << MSG_FAILED << ": " << e.get_errorno() << ": " << e.what() << endl;
	}

	LEAVE( "doStuff" );
}

/*
 * returns 0 on success, -1 on error
 */
int main(int argc, char** argv)
{
	SET_OUTPUT( true );

	int retval = EXIT_SUCCESS;	// overall result status

	try
	{
		parseParams( argc, argv );

		cout << argv[0] << " v1.0, rasdaman v" << RMANVERSION/1000 << " -- generated on " << COMPDATE << "." << endl;

		doStuff();
		retval = EXIT_SUCCESS;
		cout << argv[0] << " done." << endl;
	}
	catch (const r_Error& e)
	{
		cout << argv[0] << ": error " << e.get_errorno() << ": " << e.what() << endl;
		retval = EXIT_FAILURE;
	}
	catch (...)
	{
		cerr << argv[0] << ": panic: unexpected internal exception." << endl;
		retval = EXIT_FAILURE;
	}

	if (retval != EXIT_SUCCESS && (dbIsOpen || taIsOpen) )
	{
		cout << "aborting transaction..." << flush;
		closeTransaction();	// abort transaction and close database, ignore any further exceptions
		cout << "ok" << endl;
		closeDatabase();
	}
	
	return retval;
} // main()

// end of test_error-seq.cc

