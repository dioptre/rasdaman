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

#include "dbdirix.hh"

#include "raslib/rmdebug.hh"

RMINITGLOBALS('C')
int RManDebug2 = 8;
int RManModule = 5;

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
		database.open("RASBASE");
		ta.begin(&database);
		
		DBMDDColl* coll = 0;
		MDDSet* set = 0;
		DBMDDCollOIdEntry* entry = 0;
		DBMDDObjId obj = 0;
		DBIterId iter = 0;
		MDDIterator<DBMDDCollOIdEntry>* mdditer = 0;
		MDDIterator<DBMDDObj>* mdditerobj = 0;
		MDDIterator<MDDSet>* mdditercoll = 0;
		DBMDDObjIxId ix1 = 0;
		DBMDDObjIxId ix2 = 0;
		DBMDDObjIxId ix3 = 0;
		DBMDDObjIxId ix4 = 0;
		DBDirIx* dirix = 0;
		OId* id2 = 0;
		OId* id = 0;
		int i = 0;
		OId o;
		StructType st((char*)"StructType6", 2);
		st.addElement("_ase6", "Bool");
		st.addElement("_ase7", "Octet");
		st.addElement("_ase8", "Float");
		SetType settype((char*)"MySet3", &MDDDomainType("MyMDDDomainType", &st, r_Minterval("[1:200,*:300,20:*,*:*,20:133]")));;
		SetType* s = 0;
		cout << endl << endl << endl;

		switch (atoi(argv[1]))
			{
			case 1:
				cout << "Creating Root:" << argv[2] << endl;
				coll = DBMDDColl::createRoot(argv[2], &database);
				cout << "Creating Entry:" << argv[3] << endl;
				s = TypeFactory::mapSetType("MySet3");
				if (!s)
					{
					
					s = TypeFactory::addSetType(&settype);
					}
				DBMDDCollOIdEntry::newDBMDDCollEntry(argv[3], TypeFactory::mapSetType("MySet3"));
				cout << "Getting Entry " << argv[3] << endl; 
				entry = MDDInit::mapMDDCollOIdEntry(argv[3]);
				cout << "EntryName: " << entry->getCollName() << endl;
				cout << "Inserting Objects" << endl;
				id = new OId();
				cout << endl;

				OId::allocateOId(*id, OId::MDDOID);
				dirix = new DBDirIx(1, DBMDDObjIx::DIRIX);
				cout << "DBDIRIX AT " << dirix << endl;
				ix1 = dirix;
				cout << "DBREF IS_NULL " << ix1.is_null() << endl; 
				cout << "ALLOCATED OID " << *id << endl;
				obj = DBMDDObj::getNonInitializedMDD(0, *id);
				cout << "INITIALIZING" << endl;
				obj->initialize((const MDDBaseType*)TypeFactory::mapMDDType("MyMDDDomainType"), *((MDDDomainType*)TypeFactory::mapMDDType("MyMDDDomainType"))->getDomain(), ix1);
				cout << "INSERTING" << endl;
				coll->insert(obj);
				cout <<  endl << endl;

				OId::allocateOId(*id, OId::MDDOID);
				ix2 = new DBDirIx(1, DBMDDObjIx::DIRIX);
				cout << "ALLOCATED OID " << *id << endl;
				obj = DBMDDObj::getNonInitializedMDD(0, *id);
				cout << "INITIALIZING" << endl;
				obj->initialize((const MDDBaseType*)TypeFactory::mapMDDType("MyMDDDomainType"), *((MDDDomainType*)TypeFactory::mapMDDType("MyMDDDomainType"))->getDomain(), ix2);
				cout << "INSERTING" << endl;
				coll->insert(obj);
				cout <<  endl << endl;

				OId::allocateOId(*id, OId::MDDOID);
				ix3 = new DBDirIx(1, DBMDDObjIx::DIRIX);
				cout << "ALLOCATED OID " << *id << endl;
				obj = DBMDDObj::getNonInitializedMDD(0, *id);
				cout << "INITIALIZING" << endl;
				obj->initialize((const MDDBaseType*)TypeFactory::mapMDDType("MyMDDDomainType"), *((MDDDomainType*)TypeFactory::mapMDDType("MyMDDDomainType"))->getDomain(), ix3);
				cout << "INSERTING" << endl;
				coll->insert(obj);
				cout <<  endl << endl;

				for (iter = coll->createNewIterator(); iter->not_done(); iter->advance())
					{
					cout << "Iter returned for oid " << iter->get_element()->getOId() << " domain at " << &(iter->get_element()->getDefinitionDomain());
					iter->get_element()->printStatus();
					}
				cout << "Size of Collection: " << coll->getCardinality() << endl;
				coll->getOId(&o);
				cout << "OId of Collection: " << o << endl;
				break;
			case 2:
				cout << "Getting Root:" << argv[2] << endl;
				coll = DBMDDColl::getRoot(argv[2]);
				i = 0;
				for (iter = coll->createNewIterator(); iter->not_done(); iter->advance(), i++)
					cout << "Iter returned for " << i << ". " << iter->get_element()->getOId() << endl;
				cout << "Size of Collection: " << coll->getCardinality() << endl;
				coll->getOId(&o);
				cout << "OId of Collection: " << o << endl;
				break;
			case 3:
				cout << "Creating Object Iterator" << endl;
				mdditerobj = &MDDInit::createMDDObjectIterator();
				for (i = 0; mdditerobj->not_done(); mdditerobj->advance(), i++)
					{
					cout << "Iter returned for " << i << ". " << mdditerobj->get_element()->getOId() << endl;
					}
				break;
			case 4:
				cout << "Creating Entry Iterator" << endl;
				mdditer = &MDDInit::createMDDCollOIdEntryIterator();
				for (i = 0; mdditer->not_done(); mdditer->advance(), i++)
					{
					cout << "Iter returned for " << i << ". " << mdditer->get_element()->getOId() << " " << mdditer->get_element()->getCollName() << " " << mdditer->get_element()->getCollType()->getTypeName() << endl;
					}
				break;
			case 5:
				cout << "Creating Coll Iterator" << endl;
				mdditercoll = &MDDInit::createMDDSetIterator();
				for (i = 0; mdditercoll->not_done(); mdditercoll->advance(), i++)
					{
					mdditercoll->get_element()->getOId(&o);
					cout << "Iter returned for " << i << ". " << o << " " << mdditercoll->get_element()->getName() << " " << mdditercoll->get_element()->getCardinality() << endl;
					}
				break;
			case 6:
				id = new OId(atol(argv[2]), OId::MDDCOLLOID);
				cout << "Delete Coll with id: " << *id << endl;
				MDDInit::deleteMDDSet(*id);
				cout << "Deleted" << endl;
				mdditercoll = &MDDInit::createMDDSetIterator();
				for (i = 0; mdditercoll->not_done(); mdditercoll->advance(), i++)
					{
					cout << "Iter returned for " << i << ". " << mdditercoll->get_element()->getOId() << " " << mdditercoll->get_element()->getName() << " " << mdditercoll->get_element()->getCardinality() << endl;
					}
				break;
			case 7:
				id = new OId(atol(argv[2]), OId::MDDCOLLOID);
				cout << "Delete Coll Entry with id: " << *id << endl;
				MDDInit::deleteMDDCollOIdEntry(*id);
				cout << "Deleted" << endl;
				mdditer = &MDDInit::createMDDCollOIdEntryIterator();
				for (i = 0; mdditer->not_done(); mdditer->advance(), i++)
					{
					cout << "Iter returned for " << i << ":" << endl;
					cout << "\t" << mdditer->get_element()->getOId() << endl;
					cout << "\t" << mdditer->get_element()->getCollName() << endl;
					cout << "\t" << mdditer->get_element()->getCollType()->getTypeName() << endl;
					}
				cout << "Done!" << endl;
				break;
			case 8:
				id = new OId(atol(argv[2]), OId::MDDOID);
				cout << "Delete Object with id: " << *id << endl;
				MDDInit::deleteMDDObject(*id);
				cout << "Deleted" << endl;
				mdditerobj = &MDDInit::createMDDObjectIterator();
				for (i = 0; mdditerobj->not_done(); mdditerobj->advance(), i++)
					{
					cout << "Iter returned for " << i << ". " << mdditerobj->get_element()->getOId() << endl;
					}
				cout << "DONE! " << endl;
				break;
			case 9:
				id = new OId(atol(argv[2]), OId::MDDOID);
				id2 = new OId(atol(argv[3]), OId::MDDCOLLOID);
				cout << "Insert Object with id: " << *id << " into collection with id: " << *id2 << endl;
				set = MDDInit::mapMDDSet(*id2);
				set->insert(MDDInit::mapMDDObject(*id));
				for (iter = set->createNewIterator(); iter->not_done(); iter->advance(), i++)
					cout << "Iter returned for " << i << ". " << iter->get_element()->getOId() << endl;
				break;
			case 10:
				cout << "Creating Root:" << argv[2] << endl;
				coll = DBMDDColl::createRoot(argv[2], &database);
				break;
			case 11:
				cout << "Creating Entry:" << argv[2] << endl;
				DBMDDCollOIdEntry::newDBMDDCollEntry(argv[2], TypeFactory::mapSetType("MySet3"));
				cout << "Getting Entry " << argv[2] << endl; 
				entry = MDDInit::mapMDDCollOIdEntry(argv[2]);
				cout << "EntryName: " << entry->getCollName() << endl;
				break;
			case 12:
				id = new OId(atol(argv[2]), OId::MDDOID);
				id2 = new OId(atol(argv[3]), OId::MDDCOLLOID);
				cout << "Remove Object with id " << *id << " from collection with id: " << *id2 << endl;
				set = MDDInit::mapMDDSet(*id2);
				set->remove(MDDInit::mapMDDObject(*id));
				for (iter = set->createNewIterator(); iter->not_done(); iter->advance(), i++)
					cout << "Iter returned for " << i << ". " << iter->get_element()->getOId() << endl;
				break;
			case 13:
				cout << "Create new MDDObject" << endl;
				id = new OId();
				ix1 = new DBDirIx(1, DBDirIx::DIRIX);
				cout << endl;

				OId::allocateOId(*id, OId::MDDOID);
				cout << "ALLOCATED OID " << *id << endl;
				obj = DBMDDObj::getNonInitializedMDD(0, *id);
				cout << "INITIALIZING" << endl;
				obj->initialize((const MDDBaseType*)TypeFactory::mapMDDType("MyMDDDomainType"), *((MDDDomainType*)TypeFactory::mapMDDType("MyMDDDomainType"))->getDomain(), ix1);
				cout <<  endl << endl;
				break;
			}
		if (argv[4])
			if ((strcmp(argv[4],"dbiter") == 0))
				{
				cout << "DIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << endl;
				MDDIterator<MDDSet>* mdditercoll = &MDDInit::createMDDSetIterator();
				for (i = 0; mdditercoll->not_done(); mdditercoll->advance(), i++)
					{
					mdditercoll->get_element()->getOId(&o);
					cout << "Iter returned for " << i << ". " << o << " " << " " << mdditercoll->get_element()->getCardinality() << endl;
					for (DBIter* iter = mdditercoll->get_element()->createNewIterator(); iter->not_done(); iter->advance())
						{
						cout << "\tIter returned for MDDObject " << iter->get_element()->getOId() << endl;
						cout << "\t\t"; iter->get_element()->printStatus();
						}
					}
				}
		cout << endl << endl << endl;

		if (argv[4])
			{
			if ((strcmp(argv[4],"commit") == 0))
				{
				cout << "commiting" << endl;
				ta.commit();
				}
			}
		else	{
			cout << "aborting" << endl;
			ta.abort();
			}
		database.close();
		}
	else	{
		cout << "USAGE:" << endl;
		cout << "\t1:   create Coll and Entry" << endl << "\t\t collectionname entryname" << endl; 
		cout << "\t2:   getRoot" << endl << "\t\trootname" << endl; 
		cout << "\t3:   mddobject iterator" << endl << "\t\t" << endl; 
		cout << "\t4:   entry iterator" << endl << "\t\t" << endl; 
		cout << "\t5:   collection iterator" << endl << "\t\t" << endl; 
		cout << "\t6:   delete collection" << endl << "\t\tcollectionoid" << endl; 
		cout << "\t7:   delete entry" << endl << "\t\tentryoid" << endl; 
		cout << "\t8:   delete mddobject" << endl << "\t\tmddobjectoid" << endl; 
		cout << "\t9:   insert mddobject into collection" << endl << "\t\tmddobjectoid collectionoid" << endl; 
		cout << "\t10:  create collection" << endl << "\t\tcollectionname" << endl; 
		cout << "\t11:  create entry" << endl << "\t\tentryname" << endl; 
		cout << "\t12:  remove mddobject from collection" << endl << "\t\tmddobjectoid collectionoid" << endl; 
		cout << "\t13:  create mddobject" << endl << "\t\t" << endl; 
		}
	}
