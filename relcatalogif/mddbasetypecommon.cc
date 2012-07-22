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
// This is -*- C++ -*-
/*************************************************************
 *
 *
 * PURPOSE:
 *  code common to all database interface implementations
 *
 * CHANGE HISTORY (append further entries):
 */

#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
#include "raslib/rmdebug.hh"
#include "mddbasetype.hh"
#include "mdddomaintype.hh"
#include "basetype.hh"
#include <iostream>
#include "reladminif/sqlerror.hh"
#include "reladminif/externs.h"
#include "reladminif/objectbroker.hh"
#include <cstring>

r_Bytes
MDDBaseType::getMemorySize() const
{
    return MDDType::getMemorySize() + myBaseType->getMemorySize() + sizeof(BaseType*);
}

MDDBaseType::MDDBaseType(const OId& id) throw (r_Error)
    :   MDDType(id)
{
    RMDBGENTER(5, RMDebug::module_catalogif, "MDDBaseType", "MDDBaseType(" << myOId << ")");
    if (objecttype == OId::MDDBASETYPEOID)
    {
        mySubclass = MDDBASETYPE;
        readFromDb();
    }
    RMDBGEXIT(5, RMDebug::module_catalogif, "MDDBaseType", "MDDBaseType(" << myOId << ")");
}

MDDBaseType::MDDBaseType(const char* newTypeName, const BaseType* newBaseType)
    :   MDDType(newTypeName)
{
    objecttype = OId::MDDBASETYPEOID;
    myBaseType = newBaseType;
    mySubclass = MDDBASETYPE;
}

MDDBaseType::MDDBaseType()
    :   MDDType("unnamed mddbasetype")
{
    objecttype = OId::MDDBASETYPEOID;
    myBaseType = 0;
    mySubclass = MDDBASETYPE;
}

MDDBaseType::MDDBaseType(const char* tname)
    :   MDDType(tname)
{
    objecttype = OId::MDDBASETYPEOID;
    myBaseType = 0;
    mySubclass = MDDBASETYPE;
}

MDDBaseType::MDDBaseType(const MDDBaseType& old)
    :   MDDType(old)
{
    myBaseType = old.myBaseType;
}

MDDBaseType& MDDBaseType::operator=(const MDDBaseType& old)
{
    // Gracefully handle self assignment
    if (this == &old)
        return *this;
    MDDType::operator=(old);
    myBaseType = old.myBaseType;
    return *this;
}

char*
MDDBaseType::getTypeStructure() const
{
    char* baseType = myBaseType->getTypeStructure();
    char* result = (char*)mymalloc(10 + strlen(baseType));

    strcpy(result, "marray <");
    strcat(result, baseType);
    strcat(result, ">");

    free(baseType);
    return result;
}

void
MDDBaseType::print_status( ostream& s ) const
{
    s << "\tr_Marray" << "<" << myBaseType->getTypeName() << "\t>";
}

const BaseType*
MDDBaseType::getBaseType() const
{
    return myBaseType;
}

MDDBaseType::~MDDBaseType()
{
    RMDBGENTER(5, RMDebug::module_catalogif, "MDDBaseType", "~MDDBaseType() " << myOId);
    validate();
    RMDBGEXIT(5, RMDebug::module_catalogif, "MDDBaseType", "~MDDBaseType() " << myOId);
}

int
MDDBaseType::compatibleWith(const Type* aType) const
{
    RMDBGENTER(11, RMDebug::module_catalogif, "MDDBaseType", "compatibleWith(" << aType->getName() << ") " << getName());
    int retval;
    if( ((MDDType*)aType)->getSubtype() != MDDBASETYPE && ((MDDType*)aType)->getSubtype() != MDDDOMAINTYPE && ((MDDType*)aType)->getSubtype() != MDDDIMENSIONTYPE )
    {
        RMDBGMIDDLE(11, RMDebug::module_catalogif, "MDDBaseType", "not mddbasetype or subclass");
        retval = 0;
    }
    else
    {
        // myBaseType has to be specified
        if( myBaseType->compatibleWith(((MDDBaseType*)aType)->getBaseType()) )
        {
            retval = 1;
        }
        else
        {
            RMDBGMIDDLE(11, RMDebug::module_catalogif, "MDDBaseType", "basetypes not compatible");
            retval = 0;
        }
    }
    RMDBGEXIT(11, RMDebug::module_catalogif, "MDDBaseType", "compatibleWith(" << aType->getName() << ") " << getName() << " retval " << retval);
    return retval;
}

int
MDDBaseType::compatibleWithDomain(const r_Minterval* aDomain ) const
{
    RMDBGENTER(11, RMDebug::module_catalogif, "MDDBaseType", "compatibleWithDomain(" << *aDomain << ") " << getName());
    int retval;

    // create an MDDDomainType with aDomain and myBaseType
    MDDDomainType tempType( "tempType", myBaseType, *aDomain );
    // use normal compatibleWith
    retval = this->compatibleWith( &tempType );
    RMDBGEXIT(11, RMDebug::module_catalogif, "MDDBaseType", "compatibleWithDomain(" << aDomain << ") " << getName() << " retval " << retval);
    return retval;
}

