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
int RManDebug2 = 8;
int RManModule = 4;

int
main(int argc, char* argv[])
	{
	if (argc >= 2)
		{
		AdminIf* myAdmin;
		DatabaseIf database;
		TransactionIf ta;
		RManDebug = 6;

		myAdmin = AdminIf::instance();
		database.open("RMAN");
		ta.begin(&database);
		
		DBIterId* iter = 0;
		DBMDDColl* coll = 0;
		DBMDDCollOIdEntry* entry = 0;
		DBMDDObj* obj = 0;
		MDDIterator<DBMDDObj>* mdditerobj = 0;
		OId* id1 = 0;
		OId* id2 = 0;
		OId* id3 = 0;
		int i = 0;
		MDDBaseType* type = 0;
		DBMinterval* dom = 0;

		switch (atoi(argv[1]))
			{
			case 1:
				cout << "Create Object with mdddtype " << atol(argv[2]) << " domainid " << atol(argv[3]) << " objIx " << atol(argv[4]) << endl;
				id1 = new OId(atol(argv[2]), OId::MDDTYPEOID);
				id2 = new OId(atol(argv[3]), OId::DBMINTERVALOID);
				id3 = new OId();
				cout << *id1 << " " << *id2 << endl;
				type = (const MDDBaseType*)TypeFactory::mapMDDType(*id1);
				cout << "MDDType\t: " << type->getName() << endl;
				dom = TypeFactory::mapDBMinterval(*id2);
				cout << "Domain\t: " << *dom << endl;
				OId::allocateOId(*id3, OId::MDDOID);
				cout << "MDDOId " << *id3 << endl;
				obj = DBMDDObj::getNonInitializedMDD((char*)0, *id3);
				cout << "Got Noninitialized at" << obj << endl;
				obj->initialize((const MDDBaseType*)type, *dom, atol(argv[4]));
				break;
			case 2:
				cout << "Creating Object Iterator" << endl;
				mdditerobj = &MDDInit::createMDDObjectIterator();
				for (i = 0; mdditerobj->not_done(); mdditerobj->advance(), i++)
					{
					cout << "Iter returned for " << i << ". " << mdditerobj->get_element()->getOId() << " with domain " << mdditerobj->get_element()->getDefinitionDomain() << endl;
					}
				break;
			case 3:
				id1 = new OId(atol(argv[2]), OId::MDDOID);
				cout << "Delete Object with id: " << *id1 << endl;
				MDDInit::deleteMDDObject(*id1);
				cout << "Deleted" << endl;
				mdditerobj = &MDDInit::createMDDObjectIterator();
				for (i = 0; mdditerobj->not_done(); mdditerobj->advance(), i++)
					{
					cout << "Iter returned for " << i << ". " << mdditerobj->get_element()->getOId() << " with domain " << mdditerobj->get_element()->getDefinitionDomain() << endl;
					}
				cout << "DONE " << endl;
				break;
			case 4:
				id1 = new OId(atol(argv[2]), OId::MDDOID);
				id2 = new OId(atol(argv[3]), OId::MDDCOLLOID);
				cout << "Insert Object with id: " << *id1 << " into collection with id: " << *id2 << endl;
				coll = MDDInit::mapMDDCollection(*id2);
				coll->insert(MDDInit::mapMDDObject(*id1));
				for (iter = &coll->createIterator(); iter->not_done(); iter->advance(), i++)
					cout << "Iter returned for " << i << ". " << iter->get_element()->getOId() << endl;
				break;
			case 5:
				id1 = new OId(atol(argv[2]), OId::MDDOID);
				id2 = new OId(atol(argv[3]), OId::MDDCOLLOID);
				cout << "Remove Object with id " << *id1 << " from collection with id: " << *id2 << endl;
				coll = MDDInit::mapMDDCollection(*id2);
				coll->remove(MDDInit::mapMDDObject(*id1));
				for (iter = &coll->createIterator(); iter->not_done(); iter->advance(), i++)
					cout << "Iter returned for " << i << ". " << iter->get_element()->getOId() << endl;
			}
		
		if (argv[5])
			{
			if ((strcmp(argv[5],"commit") == 0))
				{
				cout << "commiting" << endl;
				ta.commit();
				}
			}
		cout << "aborting" << endl;
		ta.abort();
		database.close();
		}
	else	{
		cout << "USAGE:" << endl;
		cout << "\t1:   create object" << endl << "\t\tmddtypeid domainid objix" << endl; 
		cout << "\t2:   object iterator" << endl << "\t\t" << endl; 
		cout << "\t3:   delete object" << endl << "\t\tobjectid" << endl; 
		cout << "\t4:   insert object in collection" << endl << "\t\tobjectid collectionid" << endl; 
		cout << "\t5:   remove object from collection" << endl << "\t\tobjectid collectionid" << endl; 
		cout << "5th argument must be abort/commit" << endl;
		}
	}

