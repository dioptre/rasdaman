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
#include <string.h>
#include <stdlib.h>
#include "raslib/rmdebug.hh"
#include "raslib/rminit.hh"
#include "raslib/minterval.hh"
#include "raslib/mitera.hh"

RMINITGLOBALS('C')


void
main(unsigned int argc, const char** argv)
{
    r_Minterval indexDomain("[0:99,0:99]");
    cout << "index " << indexDomain << endl;
    r_Minterval iterationDomain("[0:0,0:0]");
    cout << "iterd " << iterationDomain << endl;
    r_Minterval iterationResult(2);
    //r_MiterArea iter(&indexDomain, &iterationDomain);
    r_MiterArea iter(&iterationDomain, &indexDomain);
    unsigned int i = 0;
    while (!iter.isDone())
    {
        i++;
        cout << iter.nextArea() << endl;
    }
    cout << " i is " << i << endl;
}

