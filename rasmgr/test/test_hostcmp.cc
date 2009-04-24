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
 * SOURCE: test_hostcmp.cc
 *
 * MODULE: rasmgr
 * CLASS:  
 *
 * SYNOPSIS:
 *	test_hostcmp hostname1 hostname2
 *
 * PURPOSE:
 * test hostname comparison function of rasmgr, Configuration::hostCmp().
 *
 * COMMENTS:
 * 			None
 *
*/

using namespace std;

#include <iostream>

extern int hostCmp( const char *h1, const char *h2 );
 
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "usage: " << argv[0] << " hostname1 hostname2" << endl;
		return -2;
	}

	cout << argv[0] << ": hostCmp( " << argv[1] << ", " << argv[2] << " ) -> " << (hostCmp( argv[1], argv[2]) ? "true" : "false") << endl;
	return 0;
}
