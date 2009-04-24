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
#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"
#include "raslib/rmdebug.hh"
#include "externs.h"

RMINITGLOBALS('C')


int
main(int argc, char *argv[])
	{
	int RManDebug2 = 5;
	int RManModule = 1;
	DatabaseIf database;
	TransactionIf ta;
	char* name = 0; 

	if (argc == 2)
		name = argv[1];
	else
		name = "RASBASE";

	cout << "OPENING DATABASEIF" << endl;
	if (database.open(name))
		{
		cout << "ERROR OPENING DATABASE" << endl;
		return -1;
		}
	cout << "OPENED DATABASEIF" << endl;

	if (!database.databaseExists())
		{
		cout << "DATABASE EXISTS NOT" << endl;
		cout << "CREATEING DATABASE" << endl; 
		database.create(name, 0, 0);
		cout << "DATABASE CREATED" << endl;
		}
	else	{
		cout << "DATABASE EXISTS" << endl;
		cout << "BEGINNING TRANSACTIONIF" << endl; 
		ta.begin(&database);
		cout << "TRANSACTIONIF BEGUN" << endl; 
		cout << "DESTROYING DATABASE" << endl;
		database.destroyDB(&ta, name);
		cout << "DATABASE DESTROYED" << endl;
		}
	cout << "CLOSING DATABASEIF" << endl;
	database.close();
	cout << "CLOSED DATABASEIF" << endl;
	
	cout << "OPENING DATABASEIF" << endl;
	if (database.open(name))
		{
		cout << "ERROR OPENING DATABASE" << endl;
		return -1;
		}
	cout << "OPENED DATABASEIF" << endl;
	if (database.databaseExists())
		cout << "DATABASE EXISTS" << endl;
	else
		cout << "DATABASE EXISTS NOT" << endl;
	database.close();
	cout << "CLOSED DATABASEIF" << endl;
	}
