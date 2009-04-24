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

//
//	wrapper for malloc on server side - throw bad_alloc if malloc fails
//
#include "mymalloc/mymalloc.h"
#include "reladminif/objectbroker.hh"
#include "raslib/rmdebug.hh"
#include <stdlib.h>
#include <new>

using namespace std;

// try to allocate requested memory;
// if impossible, try to free some, then retry allocation (by recursion)
// if nothing can be freed & allocated, give up & throw exception

void* mymalloc(size_t size) // throw(bad_alloc)	// FIXME: gcc3 doesn't like it, & can't do that unless other places are adapted too
{
	void* p = malloc(size);

#ifdef OLD_VERSION
// replaced this weird coding by the following below which should be semantically equivalent -- PB 2005-feb-01
	// ...except for more detailed error messages
	// FIXME: And SITF resolve this totally screwed up recursion to a while loop.
	if(!p) 
		if(!ObjectBroker::freeMemory() || !(p = mymalloc(size))) {
			RMInit::logOut << "mymalloc: memory allocation failed." << endl;
			throw bad_alloc();				
		}
#else	// improved structure, same logic:
	if (p == (void*) NULL) 
	{
		bool freePossible = ObjectBroker::freeMemory();
		if (freePossible)
		{
			p = mymalloc(size);
			if (p == (void*) NULL)
			{
				RMInit::logOut << "Error: mymalloc(): memory allocation failed." << endl;
				throw bad_alloc();				
			}
			else
			{
				// all went fine, nothing to do, return p
			}
		}
		else	// mem full, according to ObjectBroker, so throw alloc exception
		{
			RMInit::logOut << "Error: mymalloc(): ObjectBroker::freeMemory() failed." << endl;
			throw bad_alloc();				
		}
	}
#endif OLD_VERSION

	return p;
}
