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
 *       EOId is optimized for maps that contain only EOId of one system/database. 
 *
 *
 * COMMENTS:
 * 
 **********************************************************************/

#include "eoid.hh"
#include "externs.h"
#include "adminif.hh"
#include "databaseif.hh"
#include "raslib/rmdebug.hh"
#include "raslib/error.hh"

void
EOId::print_status(std::ostream& s) const
	{
	s << systemName.c_str() << "|" << databaseName.c_str() << "|";
	OId::print_status(s);
	}

std::ostream&
operator<<(std::ostream& s, const EOId& d )
	{
	s << "EOId(" << d.getSystemName() << "|" << d.getBaseName() << "|" << d.getOId() << ")";
	return s;
	}

std::ostream&
operator<<(std::ostream& s, EOId& d )
	{
	s << "EOId(" << d.getSystemName() << "|" << d.getBaseName() << "|" << d.getOId() << ")";
	return s;
	}

EOId::EOId(const char* systemname, const char* dbname, OId::OIdCounter id, OId::OIdType type)
	:	OId(id, type),
		systemName(systemname),
		databaseName(dbname)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "EOId(" << systemname << "," << dbname << "," << id << "," << type << ")"); 
	}

EOId::EOId(const OId& id) 
	:	OId(id)
	{
	RMDBGENTER(10, RMDebug::module_adminif, "EOId", "EOId(" << id << ")"); 
	if (AdminIf::getCurrentDatabaseIf())
		{
		systemName = (AdminIf::getSystemName());
		databaseName = (AdminIf::getCurrentDatabaseIf()->getName());
		}
	else	{
	        RMDBGMIDDLE(10, RMDebug::module_adminif, "EOId", "EOId(" << id << ") no current databaseif");
		throw r_Error(r_Error::r_Error_TransactionNotOpen);
		}
	RMDBGEXIT(10, RMDebug::module_adminif, "EOId", "EOId(" << id << ")"); 
	}

EOId::EOId()
	:	OId()
	{
	RMDBGENTER(10, RMDebug::module_adminif, "EOId", "EOId()"); 
	if (AdminIf::getCurrentDatabaseIf())
		{
		systemName = (AdminIf::getSystemName());
		databaseName = (AdminIf::getCurrentDatabaseIf()->getName());
		}
	else	{
	        RMDBGMIDDLE(10, RMDebug::module_adminif, "EOId", "EOId() no current databaseif");
		throw r_Error(r_Error::r_Error_TransactionNotOpen);
		}
	RMDBGEXIT(10, RMDebug::module_adminif, "EOId", "EOId()"); 
	}

EOId::~EOId()
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "~EOId()"); 
	}

const char*
EOId::getSystemName() const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "getSystemName() " << systemName.c_str()); 
	return systemName.c_str();
	}


const char*
EOId::getBaseName() const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "getBaseName() " << databaseName.c_str()); 
	return databaseName.c_str();
	}


OId
EOId::getOId() const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "getOId() " << (OId)*this); 
	return(OId)*this;
	}


void
EOId::allocateEOId(EOId& eoid, OId::OIdType t) throw (r_Error)
	{
	RMDBGENTER(10, RMDebug::module_adminif, "EOId", "allocateEOId(" << eoid << "," << t << ")"); 
	if (AdminIf::getCurrentDatabaseIf())
		{
		eoid.systemName = AdminIf::getSystemName();
		eoid.databaseName = AdminIf::getCurrentDatabaseIf()->getName();
		}
	else	{
		RMDBGMIDDLE(10, RMDebug::module_adminif, "EOId", "allocateEOId(" << eoid << ") no current databaseif");
		throw r_Error(r_Error::r_Error_TransactionNotOpen);
		}
	allocateOId(eoid, t);
	RMDBGEXIT(10, RMDebug::module_adminif, "EOId", "allocateEOId(" << eoid << "," << t << ")"); 
	}

bool
EOId::operator==(const EOId& one) const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "operator==(" << one << ")"); 
	bool retval=false;
	if(OId::operator==(one))
		if(systemName == one.systemName)
			if(databaseName == one.databaseName)
				retval=true;
	return retval;
	}

bool
EOId::operator!=(const EOId& one) const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "operator!=(" << one << ")"); 
	return !EOId::operator==(one);
	}

EOId&
EOId::operator=(const EOId& old)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "operator=(" << old << ")"); 
	if(this != &old)
	{
	 OId::operator=(old);
	 systemName = old.systemName;
	 databaseName = old.databaseName;
	}
	return *this;
	}

bool
EOId::operator<(const EOId& old) const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "operator<(" << old << ")"); 
	bool retval=false;
	if (OId::operator<(old))
		retval=true;
	if (!retval && (databaseName < old.databaseName))
		retval=true;
	if (!retval && (systemName < old.systemName))
		retval=true;
	return retval;
	}

bool
EOId::operator>(const EOId& old) const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "operator>(" << old << ")"); 
	bool retval=false;
	if (OId::operator>(old))
		retval=true;
	if (!retval && (databaseName > old.databaseName))
		retval=true;
	if (!retval && (systemName > old.systemName))
		retval=true;
	return retval;
	}

bool
EOId::operator<=(const EOId& old) const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "operator<=(" << old << ")"); 
	bool retval=false;
	if (operator<(old))
		retval=true;
	if (!retval && (operator==(old)))
		retval=true;
	return retval;
	}

bool
EOId::operator>=(const EOId& old) const
	{
	RMDBGONCE(10, RMDebug::module_adminif, "EOId", "operator<=(" << old << ")"); 
	bool retval=false;
	if (operator>(old))
		retval=true;
	if (!retval && (operator==(old)))
		retval=true;
	return retval;
	}

