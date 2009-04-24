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
#include <stdlib.h>
#include <iostream.h>
#include <string.h>

#include "adminif.hh"
#include "databaseif.hh"
#include "oidif.hh"
#include "transactionif.hh"

#include "alltypes.hh"
#include "typefactory.hh"

#include "dbmddobj.hh"

#include "raslib/rmdebug.hh"

RMINITGLOBALS('C')

int
main(int argc, char* argv[])
	{
	if (argc >= 2)
		{
		AdminIf* myAdmin;
		DatabaseIf database;
		TransactionIf ta;
		DBMDDObj* obj = 0;
		DBMDDColl* dbColl = 0;
		OId* id = 0;
		DBIterId* iter = 0;
		
		myAdmin = AdminIf::instance();
		database.open("RMAN");
		ta.begin(&database);

		
		switch (atoi(argv[1]))
			{
			default:
				cout << "DEFAULT" << argv[0] << argv[1] << argv[2] << endl;
				break;
			case 1:
				cout << "Case 1" << endl;
				dbColl = new DBMDDCollId(argv[2]);
				obj = new DBMDDObj();
				dbColl->insert_element(obj);

				obj = new DBMDDObj();
				dbColl->insert_element(obj);

				obj = new DBMDDObj();
				dbColl->insert_element(obj);

				for (iter = &dbColl->create_iterator(); iter->not_done(); iter->advance())
					cout << "Iter returned: " << iter->get_element()->getOId() << endl;
				cout << "Name of Collection: " << dbColl->getName() << endl;
				cout << "Size of Collection: " << dbColl->cardinality() << endl;
				cout << "OId of Collection: " << dbColl->getOId() << endl;
				cout << "Writing DBMDDCollId" << endl;
				dbColl->writeToDb();
				ta.commit();
				break;
			case 2:
				cout << "Case 2" << endl;
				id = new OId(atol(argv[2]), OId::MDDCOLLOID);
				cout << "Reading DBMDDCollId " << *id << endl;
				dbColl = new DBMDDCollId(*id);
				for (iter = &dbColl->create_iterator(); iter->not_done(); iter->advance())
					cout << "Iter returned: " << iter->get_element()->getOId() << endl;
				cout << "Name of Collection: " << dbColl->getName() << endl;
				cout << "Size of Collection: " << dbColl->cardinality() << endl;
				cout << "OId of Collection: " << dbColl->getOId() << endl;
				ta.commit();
				break;
			case 3:
				cout << "Case 3" << endl;
				id = new OId(atol(argv[2]), OId::MDDCOLLOID);
				cout << "Deleteing DBMDDCollId " << *id << endl;
				dbColl = new DBMDDCollId(*id);
				dbColl->deleteFromDb();
				ta.commit();
				break;
			}
		database.close();
		}
	else	{
		cout << "Usage: 1 write, 2 read, 3 delete" << endl << "\t1: collname" << endl << "\t2: collid" << endl << "\t3: collid" << endl;
		}
	}
