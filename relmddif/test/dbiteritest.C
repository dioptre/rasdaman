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

#include "raslib/rmdebug.hh"

RMINITGLOBALS('C')
int RManDebug2 = 0;
int RManModule = 0;

int
main(int argc, char* argv[])
	{
	AdminIf* myAdmin;
	DatabaseIf database;
	TransactionIf ta;
	RManDebug = 6;

	myAdmin = AdminIf::instance();
	database.open("RMAN");
	ta.begin(&database);
	
	MDDIterator<DBMDDCollOId>* mdditercoll = &MDDInit::createMDDCollOIdIterator();
	for (i = 0; mdditercoll->not_done(); mdditercoll->advance(), i++)
		{
		cout << "Iter returned for " << i << ". " << mdditercoll->get_element()->getOId() << " " << mdditercoll->get_element()->getName() << " " << mdditercoll->get_element()->getCardinality() << endl;
		for (DBIterId iter = mdditercoll->get_element()->createIterator(); iter->not_done(); iter->advance())
			{
			cout << "\tIter returned for MDDObject " << iter->get_element()->getOId() << endl;
			cout << "\t\t"; iter->get_element()->printStatus();
			}
		}
	}
