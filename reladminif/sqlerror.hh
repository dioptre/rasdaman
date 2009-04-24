#ifndef _SQLERROR_HH_
#define _SQLERROR_HH_

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
  
  SQL Errors Handling

*/
#include <iostream>
using std::cout;
using std::endl;

#include "raslib/error.hh"

#ifdef BASEDB_DB2
#define generateException() generateExceptionn(sqlca)

void generateExceptionn(struct sqlca&) throw (r_Error);

/*@Doc:
generates a new r_Ebase_dbms exception and throws it.
*/

#define check(msg) checkk(msg, sqlca)
int checkk(const char* msg, struct sqlca& mysql) throw( r_Error );
	/*@Doc: 
	returns sqlcode, prints error messages when appropriate.
	the msg is inserted in the error message.
	changes are not rolledback, nothing is done to the connection.
	*/

#endif

#ifdef BASEDB_ORACLE
void generateException() throw (r_Error);
/*@Doc:
generates a new r_Ebase_dbms exception and throws it.
*/

int check(const char* msg) throw (r_Error);
/*@Doc: 
returns sqlcode, prints error messages when appropriate.
the msg is inserted in the error message.
changes are not rolledback, nothing is done to the connection.
*/

void printSQLError(void* err, int status);
void printSQLError(void* err) throw (r_Error);
/*@Doc: 
This diplays cli errors.
*/
#endif

#ifdef BASEDB_INFORMIX
void generateException() throw (r_Error);
/*@Doc: 
This generates exceptions.
*/

int check(const char* msg, bool displayWarning = false) throw (r_Error);
/*@Doc: 
This diplays esql errors.
*/

void printSQLError(int error, const char*);
/*@Doc: 
This diplays cli errors.
*/
#endif

#ifdef BASEDB_PGSQL
void generateException() throw (r_Error);
/*@Doc: 
This generates exceptions.
*/

int check(const char* msg) throw (r_Error);
/*@Doc: 
Display error message if SQL errors have occurred.
*/
#endif

#endif

