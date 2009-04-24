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

#include "dbmddcoll.hh"
#include "dbmddcolloidentry.hh"
#include "dbmddobj.hh"

#include "dbminterval.hh"
#include "raslib/rmdebug.hh"
int RManDebug2 = 8;
int RManModule = 3;
RMINITGLOBALS('C')

int
main(int argc, char* argv[])
	{
	if (argc >= 2)
		{
		
		AdminIf* myAdmin;
		DatabaseIf database;
		TransactionIf ta;

		myAdmin = AdminIf::instance();
		database.open("RMAN");
		ta.begin(&database);

		cout << endl << endl << endl;

		DBMinterval* in = 0;
		OId* oi = 0;
		switch (atoi(argv[1]))
			{
			case 1://insert
				in = new DBMinterval(argv[2]);
				cout << "built interval with:" << argv[2] << endl;
				in->setPersistent(1);
				cout << "OId of interval:" << in->getOId() << endl;
				cout << "Minterval:" << in->get_string_representation() << endl;
				delete in;
				cout << "Domain deleted" << endl;
				break;
			case 2://update
				oi = new OId(atol(argv[2]),OId::DBMINTERVALOID);
				cout << "OId of interval:" << *oi << endl;
				in = new DBMinterval(*oi);
				cout << "Minterval:" << in->get_string_representation() << endl;
				(*in)[in->dimension() - 2].set_low(1L); 
				(*in)[in->dimension() - 2].set_high(3L); 
				cout << "Minterval:" << in->get_string_representation() << endl;
				in->setModified();
				delete in;
				cout << "Minterval deleted" << endl;
				break;
			case 3://delete
				oi = new OId(atol(argv[2]),OId::DBMINTERVALOID);
				cout << "OId of interval:" << *oi << endl;
				in = new DBMinterval(*oi);
				cout << "Minterval:" << in->get_string_representation() << endl;
				in->setPersistent(0);
				delete in;
				cout << "Minterval deleted" << endl;
				break;
			case 4://read
				oi = new OId(atol(argv[2]),OId::DBMINTERVALOID);
				cout << "OId of interval:" << *oi << endl;
				in = new DBMinterval(*oi);
				cout << "Minterval:" << in->get_string_representation() << endl;
				delete in;
				cout << "Minterval deleted" << endl;
				break;
			default:
				cout << "Don´t know what to do!" << endl;
				break;
			}
		cout << endl << endl << endl;

		if (argv[3])
			{
			if (strcmp(argv[3], "commit") == 0)
				ta.commit();
			}
		ta.abort();
		database.close();
		}
	else	{
		cout << "Usage:\n\t1 insert domain\n\t2 update oid\n\t3 delete oid\n\t4 read oid\n\t\tcommit/abort" << endl;
		}
	}
