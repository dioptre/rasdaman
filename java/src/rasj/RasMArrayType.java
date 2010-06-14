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
 * This class represents the MArray type in the ODMG conformant
 * representation of the RasDaMan type system.
 * @version $Revision: 1.8 $
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasMArrayType extends RasType
{
    static final String rcsid = "@(#)Package rasj, class RasMArrayType: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasMArrayType.java,v 1.8 2003/12/19 16:22:27 rasdev Exp $";

    private RasBaseType baseType;

    /**
     * Default constructor.
     **/
    public RasMArrayType()
    {
	super();
	baseType = null;
    }

    /**
     * Constructor getting the type of th new MArray.
     * @param newBaseType the type of the new MArray
     **/
    public RasMArrayType(RasBaseType newBaseType)
    {
	super("RAS_MARRAY");
	typeID = RasGlobalDefs.RAS_MARRAY;
	baseType = newBaseType;
    }

    /**
     * Retrieves the base type of this MArray.
     * @return the base type of this MArray
     **/
    public RasBaseType getBaseType()
    {
	return baseType;
    }

    /**
     * Returns a string representing this object.
     * @return the string representation of this object
     **/
    public String toString()
    {
	return super.toString() + "BaseType of MArray: \n" + baseType + "\n ";
    }



}
