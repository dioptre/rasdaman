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
 *  This class represents the Collection type in the ODMG conformant
 *  representation of the RasDaMan type system.
 * @version $Revision: 1.6 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasCollectionType extends RasType
{
    static final String rcsid = "@(#)Package rasj, class RasCollectionType: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasCollectionType.java,v 1.6 2003/12/10 21:04:23 rasdev Exp $";

    private RasType elementType;

    /**
     * Simple constructor.
     **/
    public RasCollectionType()
    {
	super();
	elementType = null;
    }

    /**
     * Constructor getting the element type of the new collection type.
     * @param type the element type of the new collection type
     **/
    public RasCollectionType(RasType type)
    {
	super("RAS_COLLECTION");
	typeID = RasGlobalDefs.RAS_COLLECTION;
	elementType = type;
    }

    /**
     * Retrieves the collection type's element type.
     * @return the element type of this collection type
     **/
    public RasType getElementType()
    {
	return elementType;
    }

    /**
     * Returns the type of this object (which is RAS_SET in this case).
     * @return the type of this object (RAS_SET)
     **/
    public String getKind()
    {
	return "RAS_SET";
    }

    /**
     * Creates a string representation of this object.
     * @return the string representation of this object
     **/
    public String toString()
    {
	return super.toString() + "ElementType of Set: \n" + elementType + "\n ";
    }
}
