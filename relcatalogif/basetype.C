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

static const char rcsid[] = "@(#)catalogif,BaseType: $Id: basetype.C,v 1.11 2001/06/20 08:06:37 hoefner Exp $";

#include "basetype.hh"
#include "raslib/rmdebug.hh"
#include "reladminif/externs.h"

BaseType::BaseType(const char* name)
	:	Type(name)
	{
        RMDBGONCE(7, RMDebug::module_catalogif, "BaseType", "BaseType(" << getName() << ")");
	}

/*************************************************************
 * Method name...: BaseType()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: constructor
 ************************************************************/

BaseType::BaseType()
	:	Type("unnamed basetype")
	{
	RMDBGONCE(7, RMDebug::module_catalogif, "BaseType", "BaseType()");
	}

BaseType::BaseType(const OId& id) throw (r_Error)
	:	Type(id)
	{
	RMDBGONCE(7, RMDebug::module_catalogif, "BaseType", "BaseType(" << myOId << ")");
	}

BaseType::BaseType(const BaseType& old)
	:	Type(old)
	{
	}

/*************************************************************
 * Method name...: ~BaseType()
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: virtual destructor
 ************************************************************/

BaseType::~BaseType()
	{
	}

BaseType&
BaseType::operator=(const BaseType& old)
	{
	Type::operator=(old);
	return *this;
	}

/*************************************************************
 * Method name...: UnaryOp* getUnaryOp( Ops::OpType optype )
 *
 * Arguments.....:
 *   optype: operation to return
 * Return value..:
 *   pointer to ther UnaryOp requested
 * Description...: return requested UnaryOp for ULongType
 ************************************************************/

UnaryOp*
BaseType::getUnaryOp( Ops::OpType op, const BaseType* optype ) const
{
  return Ops::getUnaryOp(op, (const BaseType*)this, optype);
}

BinaryOp*
BaseType::getBinaryOp( Ops::OpType op, const BaseType* op1type, 
		       const BaseType* op2type ) const
{
  return Ops::getBinaryOp(op, (const BaseType*)this, op1type, op2type);
}


CondenseOp*
BaseType::getCondenseOp( Ops::OpType op ) const
{ 
  return Ops::getCondenseOp(op, (const BaseType*)this);
}


int
BaseType::compatibleWith(const Type* aType) const
	{
	RMDBGENTER(7, RMDebug::module_catalogif, "BaseType", "compatibleWith(" << aType->getName() << ") " << getName());
	int retval = (myType == aType->getType());
	RMDBGEXIT(7, RMDebug::module_catalogif, "BaseType", "compatibleWith(" << aType->getName() << ") " << getName() << " " << retval);
	return retval;
	}

