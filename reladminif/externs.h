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
/*****************************************************************************
 *
 *
 * PURPOSE:
 * provide global definitions for embedded SQL usage (any base DBMS)
 *
 *
 * COMMENTS:
 * - uses embedded SQL
 * - FIXME: should also contain stuff from sqlglobals.h, merge some time
 *
 *****************************************************************************/

#ifndef _EXTERNS_H_
#define _EXTERNS_H_

const int  SQLOK = 0;
const short  INDNULL = -1;

#ifdef BASEDB_INFORMIX
	const int  SQLNULLFETCHED = -245;
	const int  SQLTABLEUNKNOWN = -206;
	const int  SQLNODATAFOUND = 100;

	//SQLCODE and SQLSTATE are defined in the sqlca.h file
#endif // informix

#ifdef BASEDB_DB2
	const int  SQLNULLFETCHED = -1405;
	const int  SQLTABLEUNKNOWN = -942;
	const int  SQLNODATAFOUND = 100;

	//declared in sqlerror.sqC
	extern long SQLCODE;
	extern char SQLSTATE[6];

#endif // db2

#ifdef BASEDB_ORACLE
	const int  SQLNULLFETCHED = -1405;
	const int  SQLTABLEUNKNOWN = -942;
	const int  SQLNODATAFOUND = 100;

	#include <sqlca.h>
	#define SQLCODE sqlca.sqlcode

	//declared in sqlerror.pc
	extern struct sqlca sqlca;
#endif // oracle

#ifdef BASEDB_PGSQL
	#include "ecpgerrno.h"		// PgSQL error codes
	// const int  SQLNULLFETCHED = -1405;	unused
	// const int  SQLTABLEUNKNOWN = -942;	not supported by PG
	const int  SQLNODATAFOUND = ECPG_NOT_FOUND;
	//SQLCODE and SQLSTATE are defined in the sqlca.h file
#endif // pgsql

#endif // _EXTERNS_H_
