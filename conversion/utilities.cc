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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

#include "utilities.h"

using namespace std;
using namespace RasNITF;

long RasNITF::read_verify(std::istream &fNITF, char *destination, long length, char *sErrorMessage, bool nullify)

{
    long rc;

    fNITF.read(destination, length);
    rc = fNITF.gcount();

    if (!fNITF.good())
    {
        cout<<sErrorMessage<<endl;
        exit(1);
    }

    if (rc != length)
    {
        cout<<sErrorMessage<<endl;
        exit(1);
    }

    if (nullify)
        destination[length] = '\0';

    return rc;
}

long RasNITF::read_verify2(std::istream &fNITF, char *destination, long length)

{
    long rc;

    fNITF.read(destination, length);
    rc = fNITF.gcount();

    if (!fNITF.good())
    {
        exit(3);
    }

    if (rc != length)
    {
        exit(4);
    }

    return rc;
}


int RasNITF::charptrtoint(const char *str, int length)
{

    int num;
    string temp;

    if (length > 5)
    {

        cout << "WARNING LOSING DATA from long to int conversion" << endl;

    }


    temp.assign(str, length);
    num = atoi(temp.c_str());

    return num;
}

long RasNITF::charptrtolong(const char *str, int length)
{

    long num;
    string temp;

    temp.assign(str, length);
    num = atol(temp.c_str());

    return num;
}
