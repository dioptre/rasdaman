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

#include <iostream>
#include <stdio.h>

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "rasodmg/database.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/set.hh"
#include "rasodmg/oqlquery.hh"


#define STRINGSIZE  256


int main(int argc, char *argv[])
{
    char dbname[STRINGSIZE] = "";
    char servername[STRINGSIZE] = "";
    char collname[STRINGSIZE] = "tTAcoll";
    r_Range range_low = 0;
    r_Range range_high = 15;

    int i;

    i = 1;
    while (i < argc)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 's':
                strcpy(servername, argv[++i]);
                break;
            case 'd':
                strcpy(dbname, argv[++i]);
                break;
            case 'c':
                strcpy(collname, argv[++i]);
                break;
            case 'h':
                cout << "Usage: " << argv[0] << " -s servername -d dbname" << endl;
                exit(0);
            }
        }
        i++;
    }

    if ((strlen(servername) == 0) || (strlen(dbname) == 0))
    {
        cerr << "Must specify servername and databasename!" << endl;
        exit(-1);
    }

    r_Database db;

    try
    {
        cout << "Trying to open " << dbname << " on " << servername << "... ";
        cout << flush;
        db.set_servername(servername);
        db.open(dbname);
        cout << "OK" << endl;
    }
    catch (r_Error &err)
    {
        cout << "failed." << endl;
        cerr << err.what();
        return -1;
    }

    r_Transaction ta;
    char queryString[STRINGSIZE];

    try
    {
        ta.begin();
        sprintf(queryString, "CREATE COLLECTION %s GreySet", collname);
        cout << "Collection " << collname << flush;
        r_OQL_Query query(queryString);
        r_oql_execute(query);
        ta.commit();
        cout << " created." << endl;
    }
    catch (r_Error &err)
    {
        ta.abort();
        cout << " not created: " << err.what() << endl;
    }

    try
    {
        r_Sinterval siv = r_Sinterval(range_low, range_high);
        r_Minterval interv(2);
        interv << siv << siv;
        r_Ref<r_Marray<char> > mddPtr = new ("GreyImage") r_Marray<char>(interv);
        memset(mddPtr->get_array(), 0, interv.cell_count());

        ta.begin();

        cout << "Starting write transaction, press return to commit..." << flush;

        sprintf(queryString, "INSERT INTO %s VALUES $1", collname);
        r_OQL_Query query(queryString);
        query << (*mddPtr);
        r_oql_execute(query);

        while (fgetc(stdin) == EOF) ;

        ta.commit();

        cout << "Write transaction successful." << endl;
    }
    catch(r_Error &err)
    {
        cerr << "Write transaction failed: " << err.what() << endl;
        ta.abort();
        db.close();
        exit(0);
    }

    db.close();

    return 0;
}
