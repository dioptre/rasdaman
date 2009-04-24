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
#include "raslib/rmdebug.hh"
#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dbnamedobject.hh"
#include "dbref.hh"
#include "dbobject.hh"
#include "oidif.hh"
#include "adminif.hh"
#include "databaseif.hh"
#include "oidif.hh"
#include "transactionif.hh"

#include "alltypes.hh"
#include "typefactory.hh"

#include "dbmddcoll.hh"
#include "dbmddcolloidentry.hh"
#include "dbmddobj.hh"

#include "raslib/rmdebug.hh"

RMINITGLOBALS('C')

int RManDebug2 = 8;
int RManModule = 3;

int
main(int argc, char *argv[])
	{
	OId t(10, OId::BASETYPEOID);

	AdminIf* myAdmin;
	DatabaseIf database;
	TransactionIf ta;
	RManDebug = 6;

	myAdmin = AdminIf::instance();
	database.open("RMAN");
	ta.begin(&database);
	
	DbRef<BaseType> p(t);
	cout << "Type " << p->getName() << endl;

	ta.abort();
	database.close();
	}
