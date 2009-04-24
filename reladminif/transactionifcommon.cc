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
 *   Code common to all DBMS interface implementations
 */

#include <iostream>

#include "transactionif.hh"
#include "adminif.hh"
#include "catalogmgr/typefactory.hh"
#include "oidif.hh"
#include "raslib/rmdebug.hh"
#include "databaseif.hh"
#include "externs.h"
#include "objectbroker.hh"
#include "raslib/error.hh"
#include "dbobject.hh"
#include "sqlerror.hh"

DatabaseIf* TransactionIf::lastBase = NULL;

DatabaseIf*
TransactionIf::getDatabaseIf()
	{
	RMDBGONCE(2, RMDebug::module_adminif, "TransactionIf", "getDatabaseIf() " << lastBase << endl);
	return lastBase;
	}

void 
TransactionIf::begin( DatabaseIf* currBase, bool readOnly ) throw ( r_Error )
	{
	RMDBGENTER(4, RMDebug::module_adminif, "TransactionIf", "begin(" << currBase->getName() << ", " << readOnly << ")");
	try {
	  currBase->baseDBMSOpen();
	}
	catch(r_Error& err) {
	  currBase->baseDBMSClose();
	  throw err;
	}
	lastBase = currBase;
	begin(readOnly);
	RMDBGEXIT(4, RMDebug::module_adminif, "TransactionIf", "begin(" << currBase->getName() << ", " << readOnly << ") ");
	}


