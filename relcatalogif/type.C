#include "mymalloc/mymalloc.h"
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
/*************************************************************
 *
 *
 * PURPOSE:
 *   uses ODMG-conformant O2 classes
 *
 *
 * COMMENTS:
 *   none
 *
 ************************************************************/

#include <stdlib.h>
#include <cstring>
#include <ctype.h>
#include "type.hh"
#include "raslib/rmdebug.hh"
#include "reladminif/externs.h"

/************************************************************
 * Method name...: Type()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: constructor
 ************************************************************/

Type::Type()
	:	DBNamedObject("unnamed type")
	{
	RMDBGONCE(4, RMDebug::module_catalogif, "Type", "Type()");
	}

Type::Type(const OId& id) throw (r_Error)
	:	DBNamedObject(id)
	{
	RMDBGONCE(4, RMDebug::module_catalogif, "Type", "Type(" << myOId << ")");
	}

Type::Type(const char* name)
	:	DBNamedObject(name)
	{
	RMDBGONCE(4, RMDebug::module_catalogif, "Type", "Type(" << name << ")");
	}

Type::Type(const Type& old)
	:	DBNamedObject(old)
	{
	myType = old.myType;
	}

Type&
Type::operator=(const Type& old)
	{
	DBNamedObject::operator=(old);
	myType = old.myType;
	return *this;
	}

void
Type::destroy()
	{
	//does nothing to prevent types from being deleted because of reference counts
	}

/*************************************************************
 * Method name...: getTypeName()
 *
 * Arguments.....: none
 * Return value..: 
 *   name of the type as a C string.
 * Description...: returns name of the type.
 ************************************************************/

const char*
Type::getTypeName() const
{
  return getName();
}

char*
Type::getTypeStructure() const
{
  // default implementation for all non-structured base types.
  char* dummy = (char*)getTypeName();
  char* result = (char*)mymalloc(strlen(dummy) + 1);
  strcpy(result, dummy);
  for(int i = 0; i < strlen(dummy); i++)
    result[i] = tolower(result[i]);
  return result;
}

const TypeEnum
Type::getType() const
{
  return myType;
}

int
Type::compatibleWith(const Type* /* aType */) const
{
  return 0;
}

/*************************************************************
 * Method name...: ~Type()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

Type::~Type()
	{
	}
