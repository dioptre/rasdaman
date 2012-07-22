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
/
/**
 * SOURCE: test_clientcomm.cc
 *
 * MODULE: clientcomm
 *
 * PURPOSE:
 *
 * COMMENTS:
 *          None
*/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "template_inst.hh"
#endif
#endif

#include <iostream>

#include "rasodmg/ref.hh"
#include "clientcomm/clientcomm.hh"


int main()
{
    try
    {
        ClientComm a( "sunwibas15" );

        cout << "Opening db ..." << flush;
        a.openDB( "RolandBase" );
        cout << "OK" << endl;

        cout << "Starting ta ..." << flush;
        a.openTA();
        cout << "OK" << endl;

        cout << "Getting type structure RGBImage ..." << flush;
        char* ts = a.getTypeStructure( "RGBImage", ClientComm::r_MDDType_Type );
        cout << "OK" << endl;

        cout << "Type Structure " << ts << endl;

        free( ts );

        cout << "Comitting ta ..." << flush;
        a.commitTA();
        cout << "OK" << endl;

        cout << "Closing db ..." << flush;
        a.closeDB();
        cout << "OK" << endl;
    }
    catch ( ... )
    {
        cout << "Exception occured: Server or connection problems." << endl;
        return -1;
    }

    return 0;
}
