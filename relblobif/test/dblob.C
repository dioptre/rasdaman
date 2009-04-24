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
#include "databaseif.hh"
#include "transactionif.hh"
#include "adminif.hh"
#include "typefactory.hh"
#include "blobtile.hh"
#include "raslib/rmdebug.hh"
#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "externs.h"
#include "dbref.hh"

RMINITGLOBALS('C')


int
main(int argc, char *argv[])
	{
	try	{
		RManDebug = 12;
		RMInit::dbgOut = cout.rdbuf();
		if (argc == 1)
			{
			printf("Usage:\n");
			printf("\t%s DBNAME TEST ARGS\n", argv[0]);
			printf("\twith DBNAME the name of the Database\n");
			printf("\tand TEST of:\n");
			printf("\t\t1 : Read BLOBTile ARGS=BlobId\n");
			printf("\t\t2 : Insert new BLOBTile ARGS=Blobdata\n");
			printf("\t\t3 : same as above with commit\n");
			printf("\t\t4 : Update BLOBTile ARGS=BlobId Blobdata\n");
			printf("\t\t5 : Delete BLOBTile ARGS=BlobId\n");
			printf("\t\t6 : Insert new BLOBTile ARGS=Blobdatalength\n");
			}
		else	{
			if (argc >= 3)
				{
				AdminIf* myAdmin;
				DatabaseIf database;
				TransactionIf ta;
				OId* id1 = 0;
				BLOBTile* b = 0;
				DbRef<BLOBTile> d;

				short test = atoi(argv[2]);
				RManDebug = 12;
				switch (test)
					{
					case 1:
						if (argc == 4)
							{
							cout << "Connect to Database: " << argv[1] << endl;
							myAdmin = AdminIf::instance();
							database.open(argv[1]);
							ta.begin(&database);

							id1 = new OId(atof(argv[3])); 
							cout << "Reading BLOBTile with Id " << *id1 << endl;
							cout << "scop1" << endl;
								{
								d = DbRef<BLOBTile>(*id1);
								cout << "Got BLOBTile" << endl;
								cout << *d;
								cout << "Refcount\t:" << d->getReferenceCount() << endl;
								}
							cout << "scop1" << endl;
							ta.abort();
							database.close();
							delete myAdmin;
							}
						else	{
							printf("YOU DIRTY ASHOLE! NOT THE RIGHT COUNT OF ARGS!\n");
							}
						break;
					case 2:
						if (argc == 4)
							{
							cout << "Connect to Database: " << argv[1] << endl;
							myAdmin = AdminIf::instance();
							database.open(argv[1]);
							ta.begin(&database);

							b = new BLOBTile(strlen(argv[3]), argv[3]);
							cout << *b;
	//unneeded					b->setPersistent(1);
							b->validate();

							cout << "Aborting Transaction" << endl;
							ta.abort();
							database.close();
							delete myAdmin;
							}
						else	{
							printf("YOU DIRTY ASHOLE! NOT THE RIGHT COUNT OF ARGS!\n");
							}
						break;

					case 3:
						if (argc == 4)
							{
							cout << "Connect to Database: " << argv[1] << endl;
							myAdmin = AdminIf::instance();
							database.open(argv[1]);
							ta.begin(&database);

							b = new BLOBTile(strlen(argv[3]), argv[3]);
	//unneeded					b->setPersistent(1);
							cout << *b;
							b->validate();

							cout << "Commiting Transaction" << endl;
							ta.commit();
							database.close();
							delete myAdmin;
							}
						else	{
							printf("YOU DIRTY ASHOLE! NOT THE RIGHT COUNT OF ARGS!\n");
							}
						break;

					case 4:
						if (argc == 5)
							{
							cout << "Connect to Database: " << argv[1] << endl;
							myAdmin = AdminIf::instance();
							database.open(argv[1]);
							ta.begin(&database);

							id1 = new OId(atof(argv[3])); 
							cout << "Reading BLOBTile with Id " << *id1 << endl;
							d = DbRef<BLOBTile>(*id1);
							cout << *d;

							cout << "Resizing BLOBTile to " << strlen(argv[4]) << endl;
							d->resize(strlen(argv[4]));
							cout << *d;

							cout << "Overwriting BLOBTile data" << endl;
							memcpy(d->getCells(), argv[4], d->getSize());
							cout << *d;

							cout << "Updating Database" << endl;
							d->setModified();

							cout << "Commiting Transaction" << endl;
							ta.commit();
							database.close();
							delete myAdmin;
							}
						else	{
							printf("YOU DIRTY ASHOLE! NOT THE RIGHT COUNT OF ARGS!\n");
							}


					case 5:
						if (argc == 4)
							{
							cout << "Connect to Database: " << argv[1] << endl;
							myAdmin = AdminIf::instance();
							database.open(argv[1]);
							ta.begin(&database);

							id1 = new OId(atof(argv[3])); 
							cout << "Reading BLOBTile with Id " << *id1 << endl;
							d = DbRef<BLOBTile>(*id1);
							cout << *d;

							cout << "Deleting BLOBTile from Database" << endl;
							d->setPersistent(0);
							cout << *d;

							cout <<  "Commiting Transaction" << endl;
							ta.commit();
							database.close();
							delete myAdmin;
							}
						else	{
							printf("YOU DIRTY ASHOLE! NOT THE RIGHT COUNT OF ARGS!\n");
							}
					case 6:
						if (argc == 4)
							{
							cout << "Connect to Database: " << argv[1] << endl;
							myAdmin = AdminIf::instance();
							database.open(argv[1]);
							ta.begin(&database);

							b = new BLOBTile(atol(argv[3]), 'c');
							cout << *b;
	//unneeded					b->setPersistent(1);
							b->validate();

							cout << "Commiting Transaction" << endl;
							ta.commit();
							database.close();
							delete myAdmin;
							}
						else	{
							printf("YOU DIRTY ASHOLE! NOT THE RIGHT COUNT OF ARGS!\n");
							}
						break;

					}
				}
			else	{
				printf("TOO FEW ARGS\n");
				if (argc == 0)
					{
					TypeFactory type_();
					}
				}
			}
		}
	catch	(r_Error e)
		{
		cout << "caught " << e.what() << " kind " << e.get_kind() << " #" << e.get_errorno() << endl;
		}
	return 0;
	}
