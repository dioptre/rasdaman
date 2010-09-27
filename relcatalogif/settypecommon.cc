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
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
#include "settype.hh"
#include <stdlib.h>
#include <string.h>
#include "reladminif/sqlerror.hh"
#include "reladminif/externs.h"
#include "raslib/rmdebug.hh"
#include "reladminif/objectbroker.hh"
#include "mddtype.hh"


SetType::SetType(const OId& id) throw (r_Error)
	:	CollectionType(id)
	{
	objecttype = OId::SETTYPEOID;
	readFromDb();
	}

char* 
SetType::getTypeStructure() const
	{
	char* dummy = myMDDType->getTypeStructure();
	char* result = (char*)mymalloc(5 + strlen(dummy) + 2);
	
	strcpy(result, "set <");
	strcat(result, dummy);
	strcat(result, ">");

	free(dummy);
	return result;
	}

SetType::SetType(const char* newTypeName, const MDDType* newMDDType) 
	:	CollectionType(newTypeName, newMDDType)
	{
	myType = SETTYPE;
	objecttype = OId::SETTYPEOID;
	}

SetType::SetType(const SetType& old)
	:	CollectionType(old)
	{
	}

SetType& SetType::operator=(const SetType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	CollectionType::operator=(old);
	return *this;
	}

SetType::SetType()
	:	CollectionType("unnamed settype")
	{
	myType = SETTYPE;
	objecttype = OId::SETTYPEOID;
	}

SetType::~SetType()
	{
	RMDBGENTER(4, RMDebug::module_catalogif, "SetType", "~SetType() " << myOId);
	validate();
	RMDBGEXIT(4, RMDebug::module_catalogif, "SetType", "~SetType() " << myOId);
	}
