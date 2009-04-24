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
#include "raslib/rmdebug.hh"
#include "mdddomaintype.hh"
#include "mdddimensiontype.hh"
#include "malloc.h"
#include "basetype.hh"
#include <iostream>
#include "reladminif/sqlerror.hh"
#include "reladminif/externs.h"
#include "reladminif/objectbroker.hh"
#include "dbminterval.hh"
#include <cstring>

r_Bytes
MDDDomainType::getMemorySize() const
	{
	return MDDBaseType::getMemorySize() + sizeof(DBMinterval*) + myDomain->getMemorySize();
	}

MDDDomainType::MDDDomainType(const OId& id) throw (r_Error)
	:	MDDBaseType(id),
		myDomain(0)
	{
	RMDBGENTER(4, RMDebug::module_catalogif, "MDDDomainType", "MDDDomainType(" << id <<")");
	if (objecttype == OId::MDDDOMTYPEOID)
		{
		mySubclass = MDDDOMAINTYPE;
		readFromDb();
		}
	RMDBGMIDDLE(8, RMDebug::module_catalogif, "MDDDomainType", "Domain\t:" << *myDomain);
	RMDBGEXIT(4, RMDebug::module_catalogif, "MDDDomainType", "MDDDomainType(" << id <<")");
	}

MDDDomainType::MDDDomainType(const char* newTypeName, const BaseType* newBaseType, const r_Minterval& newDomain)
	:	MDDBaseType(newTypeName, newBaseType)
	{
	objecttype = OId::MDDDOMTYPEOID;
	myDomain = new DBMinterval(newDomain); 
	myDomain->setCached(true);
	mySubclass = MDDDOMAINTYPE;
	}

MDDDomainType::MDDDomainType()
	:	MDDBaseType("unnamed mdddomaintype"),
		myDomain(0)
	{
	objecttype = OId::MDDDOMTYPEOID;
	mySubclass = MDDDOMAINTYPE;
	}

MDDDomainType::MDDDomainType(const MDDDomainType& old)
	:	MDDBaseType(old)
	{
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
	}

MDDDomainType&
MDDDomainType::operator=(const MDDDomainType& old)
	{
	// Gracefully handle self assignment
	if (this == &old)
		return *this;
	MDDBaseType::operator=(old);
	if (myDomain)
		{
		myDomain->setPersistent(false);
		myDomain->setCached(false);
		delete myDomain;
		}
	myDomain = new DBMinterval(*old.myDomain); 
	myDomain->setCached(true);
	return *this;
	}

char* 
MDDDomainType::getTypeStructure() const
	{
	char* baseType = myBaseType->getTypeStructure();
	char* mdom = myDomain->get_string_representation();
	char* result = (char*)mymalloc(12 + strlen(baseType) + strlen(mdom));

	strcpy(result, "marray <");
	strcat(result, baseType);
	strcat(result, ", ");
	strcat(result, mdom);
	strcat(result, ">");
	free(mdom);
	free(baseType);
	return result;
	}

const r_Minterval*
MDDDomainType::getDomain() const
	{
	return myDomain;
	}

void
MDDDomainType::print_status( ostream& s ) const
	{
	s << "\tr_Marray" << "<" << myBaseType->getTypeName() << ", ";
	myDomain->print_status(s);
	s << "\t>";
	}

MDDDomainType::~MDDDomainType()
	{
	RMDBGENTER(4, RMDebug::module_catalogif, "MDDDomainType", "~MDDDomainType() " << myOId << " " << getTypeName());
	
	validate();
	if (myDomain)
		delete myDomain;
	myDomain = 0;
	RMDBGEXIT(4, RMDebug::module_catalogif, "MDDDomainType", "~MDDDomainType() " << myOId << " " << getTypeName());
	}

int
MDDDomainType::compatibleWith(const Type* aType) const
	{
	RMDBGENTER(11, RMDebug::module_catalogif, "MDDDomainType", "compatibleWith(" << aType->getName() << ") " << getName());
	bool retval = false;
	if (aType->getType() == MDDTYPE)
		{
		MDDTypeEnum ttype = ((const MDDType*)aType)->getSubtype();
		if (ttype == MDDDOMAINTYPE)
			{
			if (myBaseType->compatibleWith(((const MDDBaseType*)aType)->getBaseType()))
				{
				if (((const MDDDomainType*)aType)->getDomain()->dimension() == myDomain->dimension())
					{
					if (myDomain->covers(*((const MDDDomainType*)aType)->getDomain()))
						{
						retval = true;
						}
					else	{
						RMDBGMIDDLE(6, RMDebug::module_catalogif, "MDDDomainType", "domain marray types have incompatible domains");
						}
					}
				else	{
					RMDBGMIDDLE(6, RMDebug::module_catalogif, "MDDDomainType", "domain marray types have different dimensions");
					}
				}
			else	{
				RMDBGMIDDLE(6, RMDebug::module_catalogif, "MDDDomainType", "basetypes are not equal");
				}
			}
		else	{
			if (ttype == MDDDIMENSIONTYPE)
				{
				if (myBaseType->compatibleWith(((const MDDBaseType*)aType)->getBaseType()))
					{
					if (myDomain->dimension() == ((const MDDDimensionType*)aType)->getDimension())
						{
						retval = true;
						}
					else	{
						RMDBGMIDDLE(6, RMDebug::module_catalogif, "MDDDomainType", "dimension marray type has wrong dimension")
						}
					}
				else	{
					RMDBGMIDDLE(6, RMDebug::module_catalogif, "MDDDomainType", "basetypes are not equal");
					}
				}
			else	{
				RMDBGMIDDLE(6, RMDebug::module_catalogif, "MDDDomainType", "not a dimension/domain type");
				}
			}
		}
	else	{
		RMInit::logOut << "MDDDomainType::compatibleWith() was passed a type that is not an marray type (" << aType->getName() << endl;
		}
	RMDBGEXIT(11, RMDebug::module_catalogif, "MDDDomainType", "compatibleWith(" << aType->getName() << ") " << getName() << " retval " << retval);
	return retval;
	}

void 
MDDDomainType::setPersistent(bool t) throw (r_Error)
	{
	MDDBaseType::setPersistent(t);
	myDomain->setPersistent(t);
	}

