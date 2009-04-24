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
#include "eoid.hh"
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
	typedef map< EOId, string, less<EOId> > ap;
	typedef pair< EOId, string > air;
	ap* myMap = new ap();
	AdminIf* myAdmin;
	DatabaseIf database;
	myAdmin = AdminIf::instance();
	database.open("RMAN");
	TransactionIf ta;
	ta.begin(&database);

	cout << "DBName " << database.getName() << endl;
	cout << " or " << AdminIf::getCurrentDatabaseIf()->getName() << endl;;

	OId* o = new OId(1, OId::BASETYPEOID);
	EOId* e = new EOId("asys", "abase", 0L, 3);
	string* s = new string("aa0");
	myMap->insert(air(*e, *s));

	e = new EOId("asys", "abase", 5L, 3);
	s = new string("aa5");
	myMap->insert(air(*e, *s));

	e = new EOId("asys", "abase", 10L, 3);
	s = new string("aa10");
	myMap->insert(air(*e, *s));

	e = new EOId("bsys", "abase", 0L, 3);
	s = new string("ba0");
	myMap->insert(air(*e, *s));

	e = new EOId("bsys", "abase", 5L, 3);
	s = new string("ba5");
	myMap->insert(air(*e, *s));

	e = new EOId("bsys", "abase", 10L, 3);
	s = new string("ba10");
	myMap->insert(air(*e, *s));

	e = new EOId("asys", "bbase", 0L, 3);
	s = new string("ab0");
	myMap->insert(air(*e, *s));

	e = new EOId("asys", "bbase", 5L, 3);
	s = new string("ab5");
	myMap->insert(air(*e, *s));

	e = new EOId("asys", "bbase", 10L, 3);
	s = new string("ab10");
	myMap->insert(air(*e, *s));

	e = new EOId("bsys", "bbase", 0L, 3);
	s = new string("bb0");
	myMap->insert(air(*e, *s));

	e = new EOId("bsys", "bbase", 10L, 3);
	s = new string("bb10");
	myMap->insert(air(*e, *s));

	e = new EOId("bsys", "bbase", 15L, 3);
	s = new string("bb15");
	myMap->insert(air(*e, *s));

	e = new EOId("asys", "abase", 5L, 3);
	s = new string("fuenf b");
	myMap->insert(air(*e, *s));

	int i = 0;
	ap::iterator iter = myMap->begin();
	for (; iter != myMap->end(); iter++)
		{
		i++;
		cout << "Nummer " << i << " key " << (*iter).first << " value " << (*iter).second.c_str() << endl;
		}
	}
