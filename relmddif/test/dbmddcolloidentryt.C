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
#include "relcatalogif/alltypes.hh"
#include "relcatalogif/typefactory.hh"
#include "raslib/rmdebug.hh"
#include "raslib/minterval.hh"

#include <string.h>
#include <stdlib.h>
#include <iostream.h>

#include "relblobif/blobtile.hh"

#include "reladminif/adminif.hh"
#include "reladminif/transactionif.hh"
#include "reladminif/databaseif.hh"
#include "reladminif/oidif.hh"

#include "relmddif/dbmddcolloidentry.hh"
#include "relmddif/dbmddcollentry.hh"
#include "relmddif/dbmddcolloid.hh"
#include "relmddif/dbmddcoll.hh"
#include "relmddif/dbmddobj.hh"

RMINITGLOBALS('C')

int
main(int argc, char* argv[])
	{
	if (argc == 0)
		{
		OId* t = new OId();
		BLOBTile b(*t);
		r_Minterval c();
		TypeFactory::instance();
		}
	AdminIf* myAdmin;
	DatabaseIf database;
	TransactionIf ta;

	myAdmin = AdminIf::instance();
	database.open("RMAN");
	ta.begin(&database);

	if (argc >= 2)
		{
		DBMDDCollOIdEntry* mycoll1 = 0;
		OId* id = 0;
		switch(atoi(argv[1]))
			{
			case 1:
				cout << "Creating Entry: " << argv[2] << endl;
				DBMDDCollEntry::newDBMDDCollEntry(argv[2], TypeFactory::mapSetType("MySet2"));
				cout << "aborting" << endl;
				ta.abort();
				break;
			case 2:
				cout << "Creating Entry: " << argv[2] << endl;
				DBMDDCollEntry::newDBMDDCollEntry(argv[2], TypeFactory::mapSetType("MySet2"));
				cout << "commiting" << endl;
				ta.commit();
				break;
			case 3:
				cout << "Reading Entry with OId: " << argv[2] << endl;
				id = new OId(atol(argv[2]), OId::MDDCOLLOID);
				mycoll1 = MDDInit::mapMDDCollOIdEntry(*id);

				cout << "Collection Name\t:" << mycoll1->getCollName() << endl;
				cout << "Collection OId\t:" << mycoll1->getOId() << endl;
				cout << "Collection SetType\t:" << mycoll1->getCollType()->getTypeName() << endl;
				cout << "aborting" << endl;
				ta.abort();
				break;
			default:
				cout << "DONT KNOW WHAT TO DO!" << endl;
			}
		}
	else	{
		cout << "Usage: 1 write abort, 2 write commit, 3 read" << endl << "\t1: name" << endl << "\t2: name" << endl << "\t3: id" << endl; 
		}
	}
