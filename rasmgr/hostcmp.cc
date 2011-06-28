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
 * SOURCE: hostcmp.cc
 *
 * MODULE: rasmgr
 * CLASS:  none
 *
 * PURPOSE:
 * special comparison function for host names (cf. man gethostname).
 * speciality: "a.x.y"=="a"
 *    
 * COMMENTS:
 * 		  none
 *
*/

#include "debug-srv.hh"
#include "raslib/rminit.hh"
#include <cstring>

bool hostCmp( const char *h1, const char *h2)
{
	ENTER( "hostCmp( " << h1 << ", " << h2 << " )" );

	bool result = false;

	if ( h1 == NULL && h2 == NULL )
		result = true;
	else if ( h1 == NULL )
		result = false;
	else if ( h2 == NULL )
		result = false;
	else
	{
		if (strlen(h1)==strlen(h2))
			result = ( strcmp(h1,h2) == 0 );
		else if (strlen(h1)>strlen(h2))
			result = ( strncmp(h1,h2,strlen(h2))==0 && h1[strlen(h2)]=='.' );
		else // (strlen(h1)<strlen(h2))
			result = ( strncmp(h1,h2,strlen(h1))==0 && h2[strlen(h1)]=='.' );
	}

	LEAVE( "Configuration::hostCmp() -> " << result );
	return result;
}

