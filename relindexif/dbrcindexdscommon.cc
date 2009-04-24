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
 * C++ part (i.e., RDBMS independent) of the RC index adaptor.
 *
 *
 * COMMENTS:
 *   uses embedded SQL
 *
 ************************************************************/

#include "mymalloc/mymalloc.h"
#include "dbrcindexds.hh"
#include "reladminif/objectbroker.hh"
#include "reladminif/dbref.hh"
#include "raslib/rmdebug.hh"
#include "reladminif/lists.h"
#include "reladminif/sqlerror.hh"
#include "reladminif/externs.h"
#include "relblobif/blobtile.hh"
#include "indexmgr/keyobject.hh"
#include "storagemgr/sstoragelayout.hh"

DBRCIndexDS::DBRCIndexDS(const OId& id)
	:	IndexDS(id),
		currentDbRows(0),
		myDomain(0),
		myBaseCounter(0),
		myBaseOIdType(OId::INVALID),
		mySize(0)
{
	RMDBGENTER(7, RMDebug::module_indexif, "DBRCIndexDS", "DBRCIndexDS(" << myOId << ")");
	objecttype = OId::MDDRCIXOID;
	readFromDb();
	RMDBGEXIT(7, RMDebug::module_indexif, "DBRCIndexDS", "DBRCIndexDS(" << myOId << ")");
}

DBRCIndexDS::DBRCIndexDS(const r_Minterval& definedDomain, unsigned int size, OId::OIdType theEntryType)
	:	IndexDS(),
		myDomain(definedDomain),
		currentDbRows(-1),
		myBaseCounter(0),
		myBaseOIdType(theEntryType),
		mySize(size)
{
	RMDBGENTER(7, RMDebug::module_indexif, "DBRCIndexDS", "DBRCIndexDS(" << definedDomain << ", " << size << ", " << theEntryType << ") " << myOId);
	objecttype = OId::MDDRCIXOID;
	OId t;
	OId::allocateOId(t, myBaseOIdType, mySize);
	myBaseCounter = t.getCounter();
	RMDBGMIDDLE(7, RMDebug::module_indexif, "DBRCIndexDS", "base counter " << myBaseCounter);
	setPersistent(true);
	setCached(true);
	RMDBGEXIT(7, RMDebug::module_indexif, "DBRCIndexDS", "DBRCIndexDS(" << definedDomain << ", " << size << ", " << theEntryType << ") " << myOId);
}

IndexDS*
DBRCIndexDS::getNewInstance() const
{
	RMInit::logOut << "DBRCIndexDS::getNewInstance() not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

OId::OIdPrimitive
DBRCIndexDS::getIdentifier() const
{
	return myOId;
}

bool
DBRCIndexDS::removeObject(const KeyObject& entry)
{
	RMInit::logOut << "DBRCIndexDS::removeObject(" << entry << ") not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

bool
DBRCIndexDS::removeObject(unsigned int pos)
{
	RMInit::logOut << "DBRCIndexDS::removeObject(" << pos << ") not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}


void
DBRCIndexDS::insertObject(const KeyObject& theKey, unsigned int pos)
{
	RMInit::logOut << "DBRCIndexDS::insertObject(" << theKey << ", " << pos << ") not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

void
DBRCIndexDS::setObjectDomain(const r_Minterval& dom, unsigned int pos)
{
	RMInit::logOut << "DBRCIndexDS::setObjectDomain(" << dom << ", " << pos << ") not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

void
DBRCIndexDS::setObject(const KeyObject& theKey, unsigned int pos)
{
	RMInit::logOut << "DBRCIndexDS::setObject(" << theKey << ", " << pos << ") not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

r_Minterval
DBRCIndexDS::getCoveredDomain() const
{
	RMDBGONCE(7, RMDebug::module_indexif, "DBRCIndexDS", "getCoveredDomain() const " << myOId << " " << myDomain);
	return myDomain;
}

r_Dimension
DBRCIndexDS::getDimension() const
{
	RMDBGONCE(7, RMDebug::module_indexif, "DBDDObjIx", "getDimension() const " << myOId << " " << myDomain.dimension());
	return myDomain.dimension();
}

r_Bytes
DBRCIndexDS::getTotalStorageSize() const
{
	RMDBGENTER(4, RMDebug::module_indexif, "DBRCIndexDS", "getTotalStorageSize() " << myOId);
	r_Bytes sz = 0;

	RMDBGEXIT(4, RMDebug::module_indexif, "DBRCIndexDS", "getTotalStorageSize() " << myOId << " " << sz);
	return sz;
}

bool
DBRCIndexDS::isValid() const
{
	return true;
}

void
DBRCIndexDS::printStatus(unsigned int level, std::ostream& stream) const
{
	char* indent = new char[level*2 +1];
	for (unsigned int j = 0; j < level*2 ; j++)
		indent[j] = ' ';
	indent[level*2] = '\0';
	
	stream << indent << "DBRCIndexDS" << std::endl;
	stream << indent << " current db rows   " << currentDbRows << std::endl;
	stream << indent << " domain            " << myDomain << std::endl;
	stream << indent << " base oid counter  " << myBaseCounter << std::endl;
	stream << indent << " base oid type     " << myBaseOIdType << std::endl;
	stream << indent << " size              " << mySize << std::endl;
	DBObject::printStatus(level, stream);
	delete[] indent;
}

OId::OIdCounter
DBRCIndexDS::getBaseCounter() const
{
	return myBaseCounter;
}

OId::OIdType
DBRCIndexDS::getBaseOIdType() const
{
	return myBaseOIdType;
}

unsigned int
DBRCIndexDS::getSize() const
{
	return mySize;
}

bool
DBRCIndexDS::isUnderFull() const
{
	//redistribute in srptindexlogic has to be checked first before any other return value may be assigned
	return false;
}

bool
DBRCIndexDS::isOverFull() const
{
	return false;
}
	
unsigned int
DBRCIndexDS::getOptimalSize() const
{
	return getSize();
}

r_Minterval
DBRCIndexDS::getAssignedDomain() const
{
	return myDomain;
}

void
DBRCIndexDS::setAssignedDomain(const r_Minterval& newDomain)
{
	RMInit::logOut << "DBRCIndexDS::setAssignedDomain(" << newDomain << ") not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

void
DBRCIndexDS::freeDS()
{
	setPersistent(false);
}

bool
DBRCIndexDS::isSameAs(const IndexDS* other) const
{
	if (other->isPersistent())
		if (myOId == other->getIdentifier())
			return true;
	return false;	
}

const KeyObject&
DBRCIndexDS::getObject(unsigned int pos) const
{
	RMInit::logOut << "DBRCIndexDS::getObject(" << pos << ") not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

void
DBRCIndexDS::getObjects(KeyObjectVector& objs) const
{
	RMInit::logOut << "DBRCIndexDS::getObjects(vec) not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

r_Minterval
DBRCIndexDS::getObjectDomain(unsigned int pos) const
{
	RMInit::logOut << "DBRCIndexDS::getObjectDomain(" << pos << ") not suported" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
}

void
DBRCIndexDS::destroy()
{
	DBObject::destroy();
}

DBRCIndexDS::~DBRCIndexDS()
{
	RMDBGENTER(7, RMDebug::module_indexif, "DBRCIndexDS", "~DBRCIndexDS() " << myOId);
	validate();
	currentDbRows = 0;
	RMDBGEXIT(7, RMDebug::module_indexif, "DBRCIndexDS", "~DBRCIndexDS() " << myOId);
}

void
DBRCIndexDS::updateInDb() throw (r_Error)
{
	RMDBGENTER(5, RMDebug::module_indexif, "DBRCIndexDS", "updateInDb() " << myOId);
	// this operation is illegal
	RMInit::logOut << "DBRCIndexDS::updateInDb() update is not possible" << std::endl;
	throw r_Error(r_Error::r_Error_FeatureNotSupported);
	RMDBGEXIT(5, RMDebug::module_indexif, "DBRCIndexDS", "updateInDb() " << myOId);
}

