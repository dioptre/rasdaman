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
#include <malloc.h>
#include <iostream>
#include <string.h>
#include "raslib/minterval.hh"
#include "raslib/rmdebug.hh"
#include "reladminif/sqlerror.hh"
#include "mddtype.hh"
#include "reladminif/externs.h"

r_Bytes
MDDType::getMemorySize() const
	{
	return sizeof(MDDType::MDDTypeEnum) + DBNamedObject::getMemorySize();
	}

MDDType::MDDType(const OId& id) throw (r_Error)
	:	Type(id)
	{
	if (objecttype == OId::MDDTYPEOID)
		{
		readFromDb();
		mySubclass = MDDONLYTYPE;
		}
	myType = MDDTYPE;
	}

MDDType::MDDType()
	:	Type("unnamed mddtype")
	{
	myType = MDDTYPE;
	mySubclass = MDDONLYTYPE;
	objecttype = OId::MDDTYPEOID;
	}

MDDType::MDDType(const char* newTypeName)
	:	Type(newTypeName)
	{
	myType = MDDTYPE;
	mySubclass = MDDONLYTYPE;
	objecttype = OId::MDDTYPEOID;
	}

MDDType::MDDType(const MDDType& old)
	:	Type(old)
	{
	myType = old.myType;
	mySubclass = old.mySubclass;
	}

MDDType& MDDType::operator=(const MDDType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	Type::operator=(old);
	myType = old.myType;
	mySubclass = old.mySubclass;
	return *this;
	}

MDDType::~MDDType()
	{
	RMDBGENTER(6, RMDebug::module_catalogif, "MDDType", "~MDDType() " << myOId);
	validate();
	RMDBGEXIT(6, RMDebug::module_catalogif, "MDDType", "~MDDType() " << myOId);
	}

char* 
MDDType::getTypeStructure() const
	{
	char* result = (char*)mymalloc(10);

	strcpy(result, "marray <>");
	return result;
	}

void
MDDType::print_status( ostream& s ) const
	{
	s << "\tr_Marray" << "<" << ">";
	}

int
MDDType::compatibleWith(const Type* aType) const
	{
	RMDBGONCE(11, RMDebug::module_catalogif, "MDDType", "compatibleWith(" << aType->getName() << ") " << (aType->getType() != MDDTYPE));
	if(aType->getType() != MDDTYPE)
		return 0;
	else
		return 1;
	}

int
MDDType::compatibleWithDomain(const r_Minterval* aDomain ) const
	{
	RMDBGONCE(11, RMDebug::module_catalogif, "MDDType", "compatibleWithDomain(" << *aDomain << ") " << 1);
	return 1;
	}

MDDType::MDDTypeEnum 
MDDType::getSubtype() const
	{
	return mySubclass;
	}

