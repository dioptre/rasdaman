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
#ifndef _DATABASEIF_HH_
#define _DATABASEIF_HH_

/************************************************************************
 *
 *
 * PURPOSE:
 *   
 *
 * COMMENTS:
 *
 ***********************************************************************/

//@ManMemo: Module: {\bf reladminif}.
/*@Doc:
With a DatabaseIf instance a database can be opened or closed. There
is also functionality for creating and deleting DBs. A
database has to be open, before persistence capable classes can be
used (see also \Ref{AdminIf}).

{\bf Example}

{\tt  DatabaseIf database;}

{\tt database.open( "myDatabase" )}

...

{\tt database.close();}
*/

class DatabaseIf;
class TransactionIf;

#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

#include "raslib/error.hh"
/**
  * \ingroup Reladminifs
  */
class DatabaseIf              
	{
	public:
		friend std::ostream& operator<< (std::ostream& stream, DatabaseIf& db);
		/*@Doc: 
		prints the status of the database (connected, online, offline, name)
		*/
		
		/// opens database with name {\tt dbName}.
		void open( const char* dbName ) throw(r_Error);
		/*@Doc: 
			Precondition: not opened, not connected, db exists
			Postcondition: open, not connected, db exists
			If last opened database was not closed (throw r_Error::r_Error_DatabaseOpen)),
			If database does not exist (throw r_Error::r_Error_DatabaseUnknown). 
			In the current implementation this value is returned, when dbName is not RASBASE,
			regardles if the db exists or not.
		*/

		void close();
		/*@Doc: 
			Precondition: open, not connected, db exists
			Postcondition: not open, not connected, db exists
			closes currently opened database.  only frees name and sets connected/opened to false.
		*/

		static void createDB( const char* dbName, const char* schemaName, const char* volumeName=0 ) throw(r_Error);
		/*@Doc: 
			Precondition: not open, not connected, db does not exist
			Postcondition: not open, not connected, db exists
			creates a new database.  schemaName and volumeName are ignored.
			only successful if dbName is RASBASE
		*/

		static void destroyDB(const char* dbName) throw(r_Error); 
		/*@Doc:
			Precondition: not open, not connected, db exists
			Postcondition: not open, not connected, db does not exist
			destroys an existing database with name {\tt dbName}.
			Database must not be open in order to be destroyed.
			A transaction must not be opened.  
			Returns -1 if database does not exist.
		*/

		
		void garbage();
		/*@Doc: 
		this method does not do anything
		*/
		
		const char* getName() const;
		/*@Doc: 
		returns a pointer to the name of the db.
		*/
		
		DatabaseIf();
		/*@Doc: 
		constructor.  Initializes opened, myName and connected to 0
		*/

		bool isConnected() const;
		/*@Doc: 
		true when there has been an EXEC SQL CONNECT
		*/

		bool isOpen() const;
		/*@Doc: 
		true when it was opened by a transaction
		*/

		~DatabaseIf();
		/*@Doc: 
		executes a baseDBMSClose() if it is still connected.
		*/
		
		static bool databaseExists(const char* dbname) throw (r_Error);
		/*@Doc: 
			Precondition: none checked.  db must be open and connected.
			Postcondition: none
			basedbms error thrown.
			checks if a database has been created.
		*/

		static bool isConsistent() throw (r_Error);
		/*@Doc: 
			Precondition: none checked.  db must be open and connected.
			Postcondition: none
			basedbms error thrown if something really bad happens.
			checks if counters are ok.
		*/

	protected:
		friend class TransactionIf;

		void baseDBMSOpen() throw (r_Error);
		/*@Doc: 
		Precondition: current database = 0
		Postcondition: current database = this
		issues a CONNECT.
		sets the DatabaseIf object in AdminIf to this.
		*/

		void baseDBMSClose();
		/*@Doc: 
			Precondition: current database = this
			Postcondition: current database = 0
			issues a ROLLBACK WORK RELEASE in oracle.
			issues a DISCONNECT in db2.
			sets the DatabaseIf object in AdminIf to 0, if it was the same.
		*/

		static void connect() throw (r_Error);
		/*@Doc:
			Precondition: none checked.
			Postcondition: none.
			issues a CONNECT.
			throws r_Error if there is a problem during connection.
		*/
		
		static void disconnect() throw (r_Error);
		/*@Doc:
			Precondition: none checked.
			Postcondition: none.
			issues a CONNECT.
			throws r_Error if there is a problem during disconnection.
		*/
		
		void checkCompatibility() throw (r_Error);
		/*@Doc:
			Precondition: none checked.
			Postcondition: none.
			throws r_Error if the current rasdaman system does not match the database.
		*/

	private:
		bool opened;
		/*@Doc: 
		TRUE only if database is open.
		*/

		char* myName;
		/*@Doc: 
		Valid only if opened.
		*/

		bool connected;
		/*@Doc: 
		TRUE only if database connection exists ; )
		*/

		static const char* DefaultDatabaseName;
		/*@Doc: 
		only one database is supported.  any database name given is compared to this string.
		access to the db is only granted if the name of the database is the same as this string.
		*/

	};

#endif
