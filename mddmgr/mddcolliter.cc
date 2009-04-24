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
 * SOURCE: persmddcolliter.cc
 *
 * MODULE: cachetamgr
 * CLASS:	MDDCollIter
 *
 * COMMENTS:
 *	none
 *
*/

#include <iostream>

#include "mddcolliter.hh"
#include "mddcoll.hh"
#include "mddobj.hh"
#include "relmddif/dbmddobj.hh"
#include "relmddif/dbmddset.hh"
#include "reladminif/dbobjectiditerator.hh"
#include "raslib/rmdebug.hh"

MDDCollIter::MDDCollIter(MDDColl* targetColl)
	:	dbIter(0), 
		persColl(targetColl)
	{
	RMDBGONCE(2, RMDebug::module_mddmgr, "MDDCollIter", "MDDCollIter(" << targetColl->getName() << ")");
	dbColl = targetColl->getDBMDDSet();
	dbIter = dbColl->newIterator();
	}

void
MDDCollIter::printStatus(unsigned int level, ostream& stream) const
	{
	stream << "   MDDCollIter printStatus:  " ;
	}

MDDObj*
MDDCollIter::getElement() const
	{
	RMDBGENTER(2, RMDebug::module_mddmgr, "MDDCollIter", "getElement()");
	// Initialization to null: make sure null pointer is returned if the 
	// collection is empty or if the iterator has come to the end already
	
	MDDObj* persEl = NULL;   
	
	if (dbIter->not_done())     
		{
		DBMDDObjId el = dbIter->get_element();
		
		if (!el.is_null())
			{
			persEl = persColl->getMDDObj((DBMDDObj*)el); 
			}
		}
	
	// persEl is null if there is nothing to return
	RMDBGEXIT(2, RMDebug::module_mddmgr, "MDDCollIter", "getElement() " << (r_Ptr)persEl);
	return persEl; 
	}

void
MDDCollIter::reset()
	{
	RMDBGONCE(2, RMDebug::module_mddmgr, "MDDCollIter", "reset()");
	dbIter->reset();
	}

bool
MDDCollIter::notDone() const
	{	
	RMDBGONCE(2, RMDebug::module_mddmgr, "MDDCollIter", "notDone()");
	return dbIter->not_done();
	}

void
MDDCollIter::advance() 
	{
	RMDBGONCE(2, RMDebug::module_mddmgr, "MDDCollIter", "advance()");
	dbIter->advance();
	}

MDDCollIter::~MDDCollIter()
	{
	RMDBGONCE(2, RMDebug::module_mddmgr, "MDDCollIter", "~MDDCollIter()");
	delete dbIter;
	dbIter = NULL;
	}

