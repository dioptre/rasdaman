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
static const char rcsid[] = "@(#)commline,functest: $Id: functest.cc,v 1.1 2005/09/15 12:55:37 rasdev Exp $";

#include<cmlparser.hh>

using std::cout;
using std::endl;


int main()
{
    CommandLineParser &cmlp = CommandLineParser::getInstance();

    try
    {
        cmlp.addFlagParameter('f', "fragmented", "fragment file until there is no file any more");
        cmlp.addFlagParameter(0  , "delete", "delete root directory");
        cmlp.addFlagParameter('s', NULL, "use server for crushing system");

        cmlp.addStringParameter('u', "user", "<name> user name\n\t\tcucubau", "rasguest");
        cmlp.addStringParameter( CommandLineParser::noShortName, "passwd", "<password> user password", "rasguest");
        cmlp.addStringParameter('d', CommandLineParser::noLongName, "<databasename> database name");
    }
    catch(CmlException e)
    {
        cout << "Error defining options:" << endl;
        cout << e.what() << endl;
        return EXIT_FAILURE;
    }

    cmlp.printHelp();

    return EXIT_SUCCESS;
}




