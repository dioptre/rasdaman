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
#include "mymalloc/mymalloc.h"
#include "raslib/rmdebug.hh"
#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dbnamedobject.hh"

RMINITGLOBALS('C')

int RManDebug2 = 6;
int RManModule = 4;

int
main(int argc, char *argv[])
	{
	if (argc == 1)
		{
		cout << "Usage:" << endl;
		cout << "\t1\t: " << endl << "\t\t" << endl;
		}
	else	{
		cout << "arg 1: " << argv[0] << " arg 2: " << argv[1] << " arg 3: " << argv[2] << endl;
		OId* id = 0;
		DBNamedObject* obj = 0;
		char* text = (char*)mymalloc(sizeof(char) * 300);
		struct	{
			short	length;
			char	data[200];
			}	VARCHAR;

		cout << "DATADUMP\t:" << VARCHAR.data << endl;
		strcpy(VARCHAR.data, "aslkjfdhaskldjfjsd\0askdjfhdkjsladf\0laksjddf\0");
		cout << "DATADUMP\t:" << VARCHAR.data << endl;
		switch (atoi(argv[1]))
			{
			case 1:
				strcpy(text, "maxidaxi\0waxitaxi");
//				id = new OId(1,1);
				DBNamedObject::MAXNAMELENGTH = atoi(argv[2]);
				cout << "Name: " << text << endl;
				obj = new DBNamedObject(text);//*id, text);
				cout << "Get Name: " << obj->getName() << endl;
				break;
			case 2:
				DBNamedObject::MAXNAMELENGTH = atoi(argv[2]);
				obj = new DBNamedObject(text);//*id, text);
				cout << "Get Name: " << obj->getName() << endl;
				VARCHAR.length = atoi(argv[3]);
				obj->setName(VARCHAR.length, (char*)VARCHAR.data);
				cout << "Get Name: " << obj->getName() << endl;
				break;
			case 3:
				DBNamedObject::MAXNAMELENGTH = atoi(argv[2]);
				obj = new DBNamedObject();
				cout << "Get Name: " << obj->getName() << endl;
				VARCHAR.length = atoi(argv[3]);
				obj->setName(VARCHAR.length, (char*)VARCHAR.data);
				cout << "Get Name: " << obj->getName() << endl;
				break;
			default:
				cout << "DONT KNOW WHAT TO DO" << endl;
				break;
			}
		}
	}	
