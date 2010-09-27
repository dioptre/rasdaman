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
// This is -*- C++ -*-
/*************************************************************************
 *
 *
 * PURPOSE:
 *	Contains all code that is shared by the database interface implementations
 *
 *
 * COMMENTS:
 * - schema version depending on release doesn't make sense; rather change
 *   it when the schema _really_ changes!
 *
 ***********************************************************************/


#include <string.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

#include "globals.hh"	// DEFAULT_DBNAME

#include "databaseif.hh"
#include "adminif.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "externs.h"
#include "sqlerror.hh"
#include "raslib/error.hh"
#include "relcatalogif/alltypes.hh"

// defined in rasserver.cc
extern char globalConnectId[256];

const char* DatabaseIf::DefaultDatabaseName = DEFAULT_DBNAME;

DatabaseIf::~DatabaseIf()
{
        RMDBGENTER(4, RMDebug::module_adminif, "DatabaseIf", "~DatabaseIf()");      
	if (isConnected())
	{
		baseDBMSClose();
	}
	if (myName)
	{
		free(myName);
		myName = NULL;
	}

	connected = false;
	opened = false;
	RMDBGEXIT(4, RMDebug::module_adminif, "DatabaseIf", "~DatabaseIf()");      
}

bool
DatabaseIf::isConnected() const
{
	RMDBGONCE(4, RMDebug::module_adminif, "DatabaseIf", "isConnected() " << connected);      
	return connected;
}

bool
DatabaseIf::isOpen() const
{
	RMDBGONCE(4, RMDebug::module_adminif, "DatabaseIf", "isOpen() " << opened);      
	return opened;
}

void 
DatabaseIf::open(const char* dbName) throw(r_Error)
{
	RMDBGENTER(4, RMDebug::module_adminif, "DatabaseIf", "open(" << dbName << ")");

	if (opened)
	{
                RMDBGMIDDLE(4, RMDebug::module_adminif, "DatabaseIf", "another database is already open");  
		throw r_Error(r_Error::r_Error_DatabaseOpen);
	}
	else
	{
		//cannot do any further error checking
		if ( 0 ) // we allow any other database name -- strcmp(dbName, DefaultDatabaseName))
		{
			RMDBGMIDDLE(4, RMDebug::module_adminif, "DatabaseIf", "database name unknown");  
			RMInit::logOut << "b_DatabaseIf::open(" << dbName << ") dbName=" << dbName << std::endl;  
			throw r_Error(r_Error::r_Error_DatabaseUnknown);
		}
		else
		{	
			opened = true;
			myName = strdup(dbName);
		}
	}
	RMDBGEXIT(4, RMDebug::module_adminif, "DatabaseIf", "open(" << dbName << ")");
}

void
DatabaseIf::baseDBMSOpen() throw (r_Error)
{
	RMDBGENTER(4, RMDebug::module_adminif, "DatabaseIf", "baseDBMSOpen() " << ((myName)? myName:"NULL"));
	if (connected)
	{
		RMDBGMIDDLE(4, RMDebug::module_adminif, "DatabaseIf", "databasename is already connected");
		throw r_Error(r_Error::r_Error_TransactionOpen);
	}
#ifdef RMANDEBUG
	if (AdminIf::getCurrentDatabaseIf())
	{
		RMDBGMIDDLE(0, RMDebug::module_adminif, "DatabaseIf", "baseDBMSOpen() CurrentDatabaseIf != 0");
		RMInit::logOut << "Transaction begin:" << std::endl \
		<< "There seems to be another database connection active (Internal State 1)." << std::endl \
		<< "Please contact Customer support." << std::endl;
		throw r_Error(DATABASE_OPEN);
	}
#endif
	AdminIf::setCurrentDatabaseIf(this);
	connect();
	connected = true;

#ifdef DBMS_PGSQL // cannot have this check in PostgreSQL -- PB 2005-jan-09
	if (!databaseExists(myName))
	{
		RMInit::logOut << "Database " << ((myName)? myName: "NULL") << " unknown" << std::endl;
		throw r_Error(r_Error::r_Error_DatabaseUnknown);
	}
#endif // DBMS_PGSQL

#ifndef FASTCONNECT
	checkCompatibility();
	if (!isConsistent())
	{
		RMInit::logOut << "Database " << ((myName)? myName: "NULL") << " inconsistent" << std::endl;
		throw r_Error(DATABASE_INCONSISTENT);
	}
#endif
	RMDBGEXIT(4, RMDebug::module_adminif, "DatabaseIf", "baseDBMSOpen()");
}

void 
DatabaseIf::close()
{
	RMDBGENTER(4, RMDebug::module_adminif, "DatabaseIf", "close()");
	opened = false;
	if (myName)
	{
		free(myName);
		myName = NULL;
	}
	if (connected)
	{
		disconnect();
		connected = false;
	}
	RMDBGEXIT(4, RMDebug::module_adminif, "DatabaseIf", "close()");
}

void
DatabaseIf::baseDBMSClose()
{
	RMDBGENTER(4, RMDebug::module_adminif, "DatabaseIf", "baseDBMSClose()");
#ifdef RMANDEBUG
	if (AdminIf::getCurrentDatabaseIf() == this)
	{
#endif
		AdminIf::setCurrentDatabaseIf(0);
#ifdef RMANDEBUG
	}
	else
	{
		//this happens when a transaction is killed by the server
		RMDBGONCE(0, RMDebug::module_adminif, "DatabaseIf", "baseDBMSClose() current DatabaseIf != this");
	}
#endif
	disconnect();
	connected = false;
	RMDBGEXIT(4, RMDebug::module_adminif, "DatabaseIf", "baseDBMSClose()");
}

DatabaseIf::DatabaseIf()
	:	opened(false),
		connected(false),
		myName(NULL)
{
	RMDBGONCE(4, RMDebug::module_adminif, "DatabaseIf", "DatabaseIf()");
}

const char* 
DatabaseIf::getName() const
{
	RMDBGONCE(4, RMDebug::module_adminif, "DatabaseIf", "getName() " << ((myName)? myName:"NULL"));
	return myName;
}

ostream&
operator << (ostream& stream, DatabaseIf& db)
{
	stream << "DatabaseIf" << std::endl;
	stream << "\tConnected To\t: " << ((db.getName())? db.getName():" ") << std::endl;
	if (db.opened)
	{
		if (db.connected)
		{
			stream << "\tDatabase is really ONLINE" << std::endl;
		}
		else
		{
			stream << "\tDatabase is only FAKE ONLINE" << std::endl;
		}
	}
	else
	{
		stream << "\tDatabase is OFFLINE" << std::endl;
	}
	return stream;
}

void
DatabaseIf::garbage( )
{
	RMDBGONCE(0,	RMDebug::module_adminif, "DatabaseIf", "garbage() NOT IMPLEMENTED");
}

