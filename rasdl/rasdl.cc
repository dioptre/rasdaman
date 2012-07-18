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
 *   The internal functions return 0 on error, 1 on success. Not really common,
 *   but it was used in some routines and so I stuck with it. Norbert 25-05-2001.
 *
 * BUGS:
 * - for inconsistent dictionnaries strange output might be generated
 *   which is not syntactically correct.
 *
 ************************************************************/

static const char rasdl_rcsid[] = "@(#)rasdl,rasdl.cc: $Id: rasdl.cc,v 1.48 2007/02/21 19:47:28 rasdev Exp $";

#define DEBUG
#define DEBUG_MAIN
#include "debug/debug.hh"

//#ifndef RMANVERSION
//#error "Please specify RMANVERSION variable!"
//#endif
//
//#ifndef COMPDATE
//#error "Please specify the COMPDATE variable!"
///*
//COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`
//
//and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
//*/
//#endif


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "template_inst.hh"
#endif
#endif

#include <iostream>  // cout
#include <sstream>  // cout
#include <iomanip>
#include <set>
#include <map>
using namespace std;

#include <stdio.h>     // fopen, perror
#include <stdlib.h>    // for atol()
#include <string.h>
#include <ctype.h>
#include <sys/times.h>	// time(), ctime()

#include "globals.hh"	// DEFAULT_DBNAME

#include "reladminif/adminif.hh"
#include "reladminif/databaseif.hh"
#include "reladminif/transactionif.hh"

#include "relcatalogif/settype.hh"
#include "relcatalogif/mdddomaintype.hh"
#include "relcatalogif/mdddimensiontype.hh"
#include "relcatalogif/alltypes.hh"
#include "catalogmgr/typefactory.hh"
#ifndef BASEDB_O2
  #include "reladminif/dbref.hh"
#endif

#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"


// error codes, exceptions
#include "rasdl_error.hh"

// error texts
#define      ERROR_NOACTION "EDL000 Error: no action specified."
#define ERROR_UNKNOWNACTION "EDL001 Error: unknown action type: " 
#define         ERROR_PANIC "EDL002 panic: unexpected internal exception."
#define      ERROR_RASDAMAN "EDL003 rasdaman error: "


// init globals for server initialization
RMINITGLOBALS('S')

#include "commline/cmlparser.hh"

// command line flags and options

#define FLAG_DATABASE	'd'
#define PARAM_DATABASE	"database"
#define HELP_DATABASE	"<db-name> name of the database"

#define PARAM_BASENAME	"basename"
#define HELP_BASENAME	"<db-name> name of the database (deprecated, use --database)"
#define DEFAULT_BASENAME DEFAULT_DBNAME

#define FLAG_CREATE	'c'
#define PARAM_CREATE	"createdatabase"
#define HELP_CREATE	"create database and initialize schema information"

#define PARAM_DELDB	"deldatabase"
#define HELP_DELDB	"delete database"

#define FLAG_PRINT	'p'
#define PARAM_PRINT	"print"
#define HELP_PRINT	"print all types"

#define FLAG_READ	'r'
#define PARAM_READ	"read"
#define HELP_READ	"<file> read data definition file"

#define PARAM_DELBASETYPE	"delbasetype"
#define HELP_DELBASETYPE	"<base-name> delete base type with name <base-name>"

#define PARAM_DELMDDTYPE	"delmddtype"
#define HELP_DELMDDTYPE		"<mdd-name> delete mdd type with name <mdd-name>"

#define PARAM_DELSETTYPE	"delsettype"
#define HELP_DELSETTYPE		"<set-name> delete set type with name <set-name>"

#define FLAG_INSERT	'i'
#define PARAM_INSERT	"insert"
#define HELP_INSERT	"insert types into database (requires -r)"

#define PARAM_HH	"hh"
#define HELP_HH		"<hh-file> print C++ header into <hh-file> (requires -r)"

#define PARAM_CONNECT	"connect"
#define HELP_CONNECT	"<connect-str> connect string for underlying database (syntax depending on base DBMS, e.g., rasbase@serverhost)"
#define DEFAULT_CONNECT	"RASBASE"

#define PARAM_DEBUG	"debug"
#define HELP_DEBUG	"print diagnostic output"

#define FLAG_HELP	'h'
#define PARAM_HELP	"help"
#define HELP_HELP	"print this help"


#ifdef EXIT_SUCCESS
    #undef EXIT_SUCCESS
#endif
#ifdef EXIT_FAILURE
    #undef EXIT_FAILURE
#endif
#ifdef EXIT_HELP
    #undef EXIT_HELP
#endif
// program return codes
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	-1
#define EXIT_HELP	2

#include "rasdl/symbtbl.hh"
#include "rasdl/yparse.hh"

// globalso for lex and yacc code
extern int          yyparse();
extern FILE         *yyin;
extern YSymbolTable *Symbols;

// pointers representing O2, database, ta and session
AdminIf*       admin = NULL;
DatabaseIf*    db    = NULL;
TransactionIf* ta    = NULL;

// command indicator (what to do)
enum ProgModes
{
	M_INVALID, 
	M_PRINT, 
	M_READ, 
	M_DELBASETYPE, 
	M_DELSETTYPE, 
	M_DELMDDTYPE,
	M_CREATEDATABASE, 
	M_DELDATABASE,
	M_HELP
};

ProgModes progMode = M_INVALID;

// this variable is provided for Admin/DatabaseIf for opening connections, it holds the connect string
char globalConnectId[256];
char globalDbUser[255] = {0};
char globalDbPasswd[255] = {0};


const char *baseName = DEFAULT_BASENAME;
const char *odlFileName = "";
const char *headerFileName = "";
const char *deleteName = "";
const char *dbSchema = "";
const char *dbVolume = "";
int         optionValueIndex;
int         insertIntoDb = 0;
bool        printToFile = false;


void 
printNames()
{
	TypeIterator<StructType> structIter = TypeFactory::createStructIter();

	cout << endl;
	cout << "// rasdaman database '" << baseName << "' type definitions" << endl;
	time_t now = time(NULL);
	cout << "// generated on: " << ctime( &now );
	cout << endl;

	cout << "//" << endl;
	cout << "// marray base types" << endl;
	cout << "//" << endl;

	while(structIter.not_done()) 
	{
		StructType* typePtr       = structIter.get_element();
		char*       typeStructure = typePtr->getTypeStructure();

		cout << "typedef " << typeStructure << " " << typePtr->getTypeName() << ";" << endl;

		free( typeStructure );
		typeStructure = NULL;

		structIter.advance();
	}

	cout << endl;
	cout << "//" << endl;
	cout << "// marray types" << endl;
	cout << "//" << endl;

	TypeIterator<MDDType> mddIter = TypeFactory::createMDDIter();

	while(mddIter.not_done()) 
	{
		MDDType* typePtr       = mddIter.get_element();
		char*    typeStructure = typePtr->getTypeStructure();

		cout << "typedef " << typeStructure << " " << typePtr->getTypeName() << ";" << endl;

		free( typeStructure );
		typeStructure = NULL;

		mddIter.advance();
	}

	cout << endl;
	cout << "//" << endl;
	cout << "// set types" << endl;
	cout << "//" << endl;

	TypeIterator<SetType> setIter = TypeFactory::createSetIter();

	while(setIter.not_done()) 
	{
		SetType* typePtr       = setIter.get_element();
    
    // print the MDD type name instead of the whole type structure -- DM 2012-may-24
    // ticket: http://rasdaman.eecs.jacobs-university.de/trac/rasdaman/ticket/140
		cout << "typedef set <" << typePtr->getMDDType()->getTypeName() << "> " << typePtr->getTypeName() << ";" << endl;

		setIter.advance();
	}

	cout << endl;
}


void 
printHeader( const char* headerFileName )
{
	cout << "Generating header file " << headerFileName << "..." << flush;

	FILE   *file=fopen( headerFileName, "wt" );
	if(!file)
		throw RasdlError( CANNOTWRITEHDR );

	char* defName = new char[strlen(headerFileName)+1];
	if (defName == NULL)
		throw RasdlError( CANNOTALLOC );

	// generate upper case version for include guard
	int i = 0;
	while( headerFileName[i] != '.' && headerFileName[i] != '\0' )
	{
		defName[i] = toupper( headerFileName[i] );
		i++;
	}
	defName[i] = '\0';

	/* header description */
	fprintf(file,"//------------------------------------------------------------\n");
	fprintf(file,"//  This file is created automatically by the rasdl processor.\n");
	fprintf(file,"//  better than modifying this file is to re-generate it.     \n");
	fprintf(file,"//------------------------------------------------------------\n");
	fprintf(file,"\n");
	fprintf(file,"#ifndef __%s_HH_\n", defName );
	fprintf(file,"#define __%s_HH_\n", defName );
	fprintf(file,"\n");
	fprintf(file,"//------------------------------------------------------------\n");
	fprintf(file,"//  Includes\n");
	fprintf(file,"//------------------------------------------------------------\n");
	fprintf(file,"\n");
	fprintf(file,"#include \"rasdaman.hh\"\n");
	fprintf(file,"\n");
	
	Symbols->global_scope->output(file);
	
	fprintf(file,"#endif\n");
	
	fclose(file);
	delete[] defName;
	defName = NULL;

	cout << "ok" << endl;  
}

void 
disconnectDB( bool commitTa )
{
	ENTER( "disconnectDB, commitTa =" << commitTa  );

	if( ta )
	{
		if(commitTa )
		{
			ta->commit();
			TALK( "TA committed." );
		}
		else
		{
			ta->abort();
			TALK( "TA aborted." );
		}

		delete ta;
		ta = NULL;

		if( db )
		{
			db->close();
			TALK( "DB closed." );
			delete db;
			db = NULL;
		}
	
		if( admin )
		{
			delete admin;
			admin = NULL;
		}
	}
	LEAVE( "disconnectDB" );
}

void 
connectDB( const char* baseName, bool openDb, bool openTa ) throw (r_Error, RasdlError)
{
	ENTER( "connectDB, basename=" << baseName );

	admin = AdminIf::instance();
	if( !admin )
	{
		TALK( "cannot create adminIf instance" );
		throw RasdlError( NOCONNECTION );
	}

	if ( openDb )
	{
		// connect to the database
		db = new DatabaseIf();
		// TALK( "adding dbf to adminif" );
		// admin->setCurrentDatabaseIf( db );
		TALK( "opening db" );
		db->open( baseName );
	}

	if ( openTa )
	{
		// start transaction
		ta = new TransactionIf();
		TALK( "opening ta" );
		ta->begin( db );
	}

	LEAVE( "connectDB" );
}

//analyse params
void 
parseParams(int argc, char* argv[]) throw (r_Error, RasdlError)
{
	CommandLineParser    &cmlInter     	= CommandLineParser::getInstance();

	CommandLineParameter &cmlHelp           = cmlInter.addFlagParameter( FLAG_HELP, PARAM_HELP, HELP_HELP );

	CommandLineParameter &cmlDbName   	= cmlInter.addStringParameter( FLAG_DATABASE, PARAM_DATABASE, HELP_DATABASE, DEFAULT_BASENAME );
	CommandLineParameter &cmlBaseName   	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_BASENAME, HELP_BASENAME, DEFAULT_BASENAME );
	CommandLineParameter &cmlCreateDb   	= cmlInter.addFlagParameter( FLAG_CREATE, PARAM_CREATE, HELP_CREATE );
	CommandLineParameter &cmlDelDb   	= cmlInter.addFlagParameter(CommandLineParser::noShortName, PARAM_DELDB, HELP_DELDB );
	CommandLineParameter &cmlReadMeta    	= cmlInter.addStringParameter( FLAG_READ, PARAM_READ, HELP_READ );
	CommandLineParameter &cmlInsertInDb   	= cmlInter.addFlagParameter( FLAG_INSERT, PARAM_INSERT, HELP_INSERT );

	CommandLineParameter &cmlHHGen    	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_HH, HELP_HH );
	CommandLineParameter &cmlPrintMeta   	= cmlInter.addFlagParameter( FLAG_PRINT, PARAM_PRINT, HELP_PRINT );

	CommandLineParameter &cmlDelBaseType 	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_DELBASETYPE, HELP_DELBASETYPE );
	CommandLineParameter &cmlDelMddType  	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_DELMDDTYPE, HELP_DELMDDTYPE );
	CommandLineParameter &cmlDelSetType   	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_DELSETTYPE, HELP_DELSETTYPE );

#ifdef BASEDB_O2
	CommandLineParameter &cmlSchemaDb   	= cmlInter.addStringParameter(CommandLineParser::noShortName, "schema", "<sch-name> name of o2 schema used for new bases", "RasDaSchema");
	CommandLineParameter &cmlVolumeDb    	= cmlInter.addStringParameter(CommandLineParser::noShortName, "volume", "<vol-name> name of o2 volume used for new bases");
#endif

	CommandLineParameter &cmlConnectStr     = cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_CONNECT, HELP_CONNECT, DEFAULT_CONNECT );

#ifdef DEBUG
	CommandLineParameter &cmlDebug          = cmlInter.addFlagParameter(CommandLineParser::noShortName, PARAM_DEBUG, HELP_DEBUG );
#endif

	try
	{
		cmlInter.processCommandLine(argc, argv);
	
		if (argc == 1 || cmlHelp.isPresent())
		{
			static const char* ExDbName      = DEFAULT_BASENAME;
			static const char* ExBaseName    = "char";  
			static const char* ExMddName     = "GreyImage";  
			static const char* ExSetName     = "GreySet";  
			static const char* ExMetaFile    = "basictypes.dl";
			static const char* ExHHFile      = "basictypes.hh";

			cout << "Usage: rasdl [options]" << endl;		
			cout << "Options:" << endl;
			cmlInter.printHelp();

			cout << "Examples:" << endl;
			cout << "   create database:\trasdl " << CommandLineParser::LongSign << cmlDbName.getLongName() << " " << ExDbName   << " " << CommandLineParser::LongSign << cmlCreateDb.getLongName()    << endl;
			cout << "   delete database:\trasdl "   << CommandLineParser::LongSign << cmlDbName.getLongName() << " " << ExDbName   << " " << CommandLineParser::LongSign << cmlDelDb.getLongName()       << endl;
			cout << "   delete base type:\trasdl "   << CommandLineParser::LongSign << cmlDbName.getLongName() << " " << ExDbName   << " " << CommandLineParser::LongSign << cmlDelBaseType.getLongName() << " " << ExBaseName << endl;
			cout << "   delete mdd type:\trasdl "   << CommandLineParser::LongSign << cmlDbName.getLongName() << " " << ExDbName   << " " << CommandLineParser::LongSign << cmlDelMddType.getLongName()  << " " << ExMddName << endl;  
			cout << "   delete set type:\trasdl "   << CommandLineParser::LongSign << cmlDbName.getLongName() << " " << ExDbName   << " " << CommandLineParser::LongSign << cmlDelSetType.getLongName()  << " " << ExSetName  << endl;  
			cout << "   print all types:\trasdl "   << CommandLineParser::LongSign << cmlDbName.getLongName() << " " << ExDbName << " " << CommandLineParser::LongSign << cmlPrintMeta.getLongName()   << endl;
			cout << "   insert types:\trasdl " << CommandLineParser::LongSign << cmlDbName.getLongName() << " " << ExDbName << " " << CommandLineParser::LongSign << cmlReadMeta.getLongName() << " " << ExMetaFile << " " << CommandLineParser::LongSign << cmlInsertInDb.getLongName()  << endl;  
			cout << "   generate C++ header from type file:\trasdl " << CommandLineParser::LongSign << cmlReadMeta.getLongName()    << " " << ExMetaFile << " " << CommandLineParser::LongSign << cmlHHGen.getLongName() << " " << ExHHFile   << endl;  
			exit( EXIT_HELP );	// bash ret code for usage; FIXME: exit() no good style!
		}

#ifdef DEBUG
		// debug
		SET_OUTPUT( cmlDebug.isPresent() );
#endif

		if (cmlBaseName.isPresent())
			baseName = cmlBaseName.getValueAsString();

		if (cmlDbName.isPresent())
			baseName = cmlDbName.getValueAsString();

		//read mode
		if( cmlReadMeta.isPresent())
		{
			progMode = M_READ;
			odlFileName = cmlReadMeta.getValueAsString();
		}

		//schema && volume
#ifdef BASEDB_O2
		// to be done
		dbSchema = cmlSchemaDb.getValueAsString());
		if( cmlVolumeDb.isPresent())
		{
			if( cmlVolumeDb.getValueAsString())
			  	dbVolume = cmlVolumeDb.getValueAsString());
		}
#endif

		//hhgen
		if( cmlHHGen.isPresent())
		{
			if( cmlHHGen.getValueAsString())
			{
				headerFileName = cmlHHGen.getValueAsString();
				printToFile = true;
			}
		}

		//insertindb
		insertIntoDb = cmlInsertInDb.isPresent();

		//print mode
		if( cmlPrintMeta.isPresent())
			progMode = M_PRINT;

		//create mode
		if( cmlCreateDb.isPresent())
			progMode = M_CREATEDATABASE;
	 
		//delete mode
		if( cmlDelDb.isPresent() )
			progMode = M_DELDATABASE;

		//delbasetype mode
		if( cmlDelBaseType.isPresent() )
		{
			progMode = M_DELBASETYPE;
			if( cmlDelBaseType.getValueAsString() )
				deleteName = cmlDelBaseType.getValueAsString();
		}

		//delmddtype mode
		if( cmlDelMddType.isPresent() )
		{
			progMode = M_DELMDDTYPE;
			if( cmlDelMddType.getValueAsString())
				deleteName = cmlDelMddType.getValueAsString();
		}

		//delsettype mode
		if( cmlDelSetType.isPresent() )
		{
			progMode = M_DELSETTYPE;
			if( cmlDelSetType.getValueAsString() )
				deleteName = cmlDelSetType.getValueAsString();
		}

		// connectstr
		if( cmlConnectStr.isPresent() )
			strcpy( globalConnectId, cmlConnectStr.getValueAsString() );
		else
			strcpy( globalConnectId, DEFAULT_CONNECT );
	}
	catch ( CmlException & err)
	{
		throw RasdlError( CMDLINE );
	}

	// --- plausi checks ------------------------------------
	// FIXME: to complete

	// if something is to be deleted in the database, then a name must be specified
	if ( cmlDelBaseType.isPresent() || cmlDelMddType.isPresent() || cmlDelSetType.isPresent() )
	{
		if ( deleteName == NULL || strlen(deleteName) == 0)
			throw RasdlError( EMPTYTYPENAME );
	}

	// -r needs either -i or -hh
	if( cmlReadMeta.isPresent() && ( !cmlInsertInDb.isPresent() && !cmlHHGen.isPresent() ) )
		throw RasdlError( ILLEGALREADCOMBI );
	if( ( cmlInsertInDb.isPresent() && cmlHHGen.isPresent() ) )
		throw RasdlError( ILLEGALREADCOMBI );

	if( cmlInsertInDb.isPresent() && !cmlReadMeta.isPresent() )
		throw RasdlError( ILLEGALINSERTCOMBI );

	if( cmlHHGen.isPresent() && !cmlReadMeta.isPresent() )
		throw RasdlError( ILLEGALHHCOMBI );

	TALK( "connect=" << globalConnectId );
} // parseParams()

void 
readMode() throw (r_Error, RasdlError)
{
	cout << "Reading rasdaman data definition file " << odlFileName << "..." << flush;

	// yyin is used by yyparse()
	yyin=fopen( odlFileName,"r" );
	if( yyin==NULL )
		throw RasdlError( ODLFILEFAILED );

	int parseResult = yyparse();
	if(yyin && (yyin != stdin))
	      fclose( yyin );
	if( parseResult != 0)
		throw RasdlError( ODLPARSEERROR );

	if( insertIntoDb )
	{
		cout << "inserting symbols into database..." << flush;  
		Symbols->global_scope->insertData();   
		cout << "ok" << endl;  
	}
	if( printToFile )
		printHeader( headerFileName );

	cout << "ok" << endl;
}



int 
main( int argc, char* argv[] )
{
	int result = EXIT_FAILURE;

	cout << "rasdl: rasdaman schema and database manipulation tool, rasdaman v" << RMANVERSION / 1000.0 << " on base DBMS "  << BASEDBSTRING << " -- generated on " << COMPDATE << "." << endl;

	try
	{
		parseParams(argc,argv);
	
    // don't overwrite the value of the --connect!! -- DM 2012-jul-11
		//strcpy(globalConnectId, baseName);
		switch( progMode )
		{
			case M_READ:     
				connectDB( baseName, true, true );
				readMode();
				disconnectDB( true );
		        	break;
			case M_DELBASETYPE:
				cout << "Deleting basetype " << deleteName << "..." << flush;		       
				connectDB( baseName, true, true );
				TypeFactory::deleteStructType( deleteName );
				disconnectDB( true );
				cout << "ok" << endl;
		        	break;
			case M_DELMDDTYPE:
				cout << "Deleting MDD type " << deleteName << "..." << flush;		       
				connectDB( baseName, true, true );
				TypeFactory::deleteMDDType( deleteName );
				disconnectDB( true );
				cout << "ok" << endl;
		        	break;
			case M_DELSETTYPE:
				cout << "Deleting set type " << deleteName << "..." << flush;		       
				connectDB( baseName, true, true );
				TypeFactory::deleteSetType( deleteName );
				disconnectDB( true );
				cout << "ok" << endl;
		        	break;
			case M_CREATEDATABASE:
		        	cout << "Creating base " << baseName;
#ifdef BASEDB_O2
				cout << " with schema " << dbSchema;
#endif
		        	if( strcasecmp(dbVolume,"") )
		        		cout << " on volume " << dbVolume;
		        	cout << "..." << flush;
				connectDB( baseName, false, false );
		        	db = new DatabaseIf();
 				if( !strlen(dbVolume) )
					db->createDB( baseName, dbSchema, dbVolume );
				else
					db->createDB( baseName, dbSchema );
				disconnectDB( true );
				cout << "ok" << endl;
		        	break;
			case M_DELDATABASE:
				cout << "Deleting database " << baseName << "...";
				TALK( "connecting" );
				connectDB( baseName, false, false );
				TALK( "creating new DatabaseIf" );
		        	db = new DatabaseIf();                      
				TALK( "destroying db" );
				db->destroyDB(baseName);
				disconnectDB( true );
				cout << "ok" << endl;
		        	break;
			case M_PRINT:
				connectDB( baseName, true, true );
				printNames();
				disconnectDB( false );	// abort TA - we did only reading
		        	break;
			case M_INVALID:
				cerr << ERROR_NOACTION << endl;
				break;
			default:
				cerr << ERROR_UNKNOWNACTION << progMode << endl;
		}
		result = EXIT_SUCCESS;
	}
	catch ( RasdlError& e)
	{
		cout << argv[0] << ": " << e.what() << endl;
		result = EXIT_FAILURE;
	}
	catch ( const r_Error& e )
	{
		cout << ERROR_RASDAMAN << e.get_errorno() << ": " << e.what() << endl;
		result = EXIT_FAILURE;
	}
	catch (...)
	{
		cout << argv[0] << " " << ERROR_PANIC << endl;
		result = EXIT_FAILURE;
	}

	if (result != EXIT_SUCCESS)
		disconnectDB(false);

	cout << argv[0] << " done." << endl;

	return( result );
}
