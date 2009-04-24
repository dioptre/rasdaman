package rasj;

import rasj.*;
import rasj.global.*;

import java.util.*;

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
/** ***********************************************************
 * <pre>
 *
 * PURPOSE:
 * This class represents the primitive types in the ODMG conformant
 * representation of the RasDaMan type system.
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasPrimitiveType extends RasBaseType implements RasGlobalDefs
{
    static final String rcsid = "@(#)Package rasj, class RasPrimitiveType: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasPrimitiveType.java,v 1.8 2003/12/10 21:04:23 rasdev Exp $";

    private String rasTypeName = null;

    public RasPrimitiveType()
    {
	super();
    }

    public RasPrimitiveType(String name, int type)
    {
	super(name, 0);
	typeID = type;

	switch(typeID)
	    {
            case RAS_LONG:     typeSize = SIZE_OF_RAS_LONG; rasTypeName = "RAS_LONG"; break;
	    case RAS_ULONG:    typeSize = SIZE_OF_RAS_ULONG; rasTypeName = "RAS_ULONG"; break;
	    case RAS_SHORT:    typeSize = SIZE_OF_RAS_SHORT; rasTypeName = "RAS_SHORT"; break;
	    case RAS_USHORT:   typeSize = SIZE_OF_RAS_USHORT; rasTypeName = "RAS_USHORT"; break;
	    case RAS_BOOLEAN:  typeSize = SIZE_OF_RAS_BOOLEAN; rasTypeName = "RAS_BOOLEAN"; break;
	    case RAS_BYTE:     typeSize = SIZE_OF_RAS_BYTE; rasTypeName = "RAS_BYTE"; break;
	    case RAS_DOUBLE:   typeSize = SIZE_OF_RAS_DOUBLE; rasTypeName = "RAS_DOUBLE"; break;
	    case RAS_FLOAT:    typeSize = SIZE_OF_RAS_FLOAT; rasTypeName = "RAS_FLOAT"; break;
	    case RAS_CHAR:     typeSize = SIZE_OF_RAS_CHAR; rasTypeName = "RAS_CHAR"; break;
	    }
    }


    public int getTypeID()
    {
	return typeID;
    }

    public String toString()
    {
	return super.toString();
    }

    public Boolean getBoolean(Object cell) throws RasTypeInvalidException
    {
	if(typeID != RAS_BOOLEAN)
	    {
		throw new RasTypeInvalidException("RAS_BOOLEAN",rasTypeName);
	    }

	return (Boolean)cell;
    }


    public Character getCharacter(Object cell) throws RasTypeInvalidException
    {
	if(typeID != RAS_CHAR)
	    {
		throw new RasTypeInvalidException("RAS_CHAR",rasTypeName);
	    }

	return (Character)cell;
    }


    public Byte getByte(Object cell) throws RasTypeInvalidException
    {
	if(typeID != RAS_BYTE)
	    {
	 	throw new RasTypeInvalidException("RAS_BYTE",rasTypeName);
	    }

	return (Byte)cell;
    }


    public Short getShort(Object cell) throws RasTypeInvalidException
    {
	if(typeID != RAS_SHORT)
	    {
		throw new RasTypeInvalidException("RAS_SHORT",rasTypeName);
	    }

	return (Short)cell;
    }


    public Integer getInteger(Object cell) throws RasTypeInvalidException
    {
	if((typeID != RAS_LONG) || (typeID != RAS_USHORT))
	    {
		throw new RasTypeInvalidException("RAS_LONG",rasTypeName);
	    }

	return (Integer)cell;
    }

    public Long getLong(Object cell) throws RasTypeInvalidException
    {
	if(typeID != RAS_ULONG)
	    {
		throw new RasTypeInvalidException("RAS_ULONG",rasTypeName);
	    }

	return (Long)cell;
    }

    public Float getFloat(Object cell) throws RasTypeInvalidException
    {
	if(typeID != RAS_FLOAT)
	    {
		throw new RasTypeInvalidException("RAS_FLOAT",rasTypeName);
	    }

	return (Float)cell;
    }

    public Double getDouble(Object cell) throws RasTypeInvalidException
    {
	if(typeID != RAS_DOUBLE)
	    {
		throw new RasTypeInvalidException("RAS_DOUBLE",rasTypeName);
	    }

	return (Double)cell;
    }

}
