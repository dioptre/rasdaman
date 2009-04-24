package rasj;

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
 * <pre>
 *
 * PURPOSE:
 * This class represents the Base types and is the superclass of
 * the types RasStructureType and RasPrimitiveType in the
 * representation of the RasDaMan type system.
 * <P>
 * <B>Note:</B> if a new base type is created using this class, it is only known on client-side but not 
 * stored within the database. If you want to introduce a new type for the RasDaMan system, you
 * should use the "rasdl" utility.
 * </P>
 * @version $Revision: 1.6 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasBaseType extends RasType
{
    static final String rcsid = "@(#)Package rasj, class RasBaseType: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasBaseType.java,v 1.6 2003/12/10 21:04:23 rasdev Exp $";

    /**
     * Stores the size of type in bytes 
     **/
    protected long typeSize;

    /**
     * Default constructor. 
     **/
    public RasBaseType()
    {
	super();
	typeSize = 0;
    }

    /**
     * Constructor getting name of basetype.
     * @param newTypeName name of the new base type
     * @param newSize size of the new base type
     **/
    public RasBaseType(String newTypeName, long newSize)
    {
	super(newTypeName);
	typeSize = newSize;
    }

    /**
     * Retrieves the id of the type.
     * @return the id of the type
     **/
    public int getTypeID()
    {
	return 0;
    }

    /**
     * Checks if the current type is a base type (primitive type or structure type).
     * @return true if it is a base type, false otherwise.
     **/
    public boolean isBaseType()
    {
	return true;
    }

    /**
     * Retrieves the size of the type.
     * @return the size of the base type.
     **/
    public long getSize()
    {
	return typeSize;
    }

    /**
     * Returns a string representation of this object.
     * @return a string description of this object.
     **/
    public String toString()
    {
	return super.toString() + "typeSize: " + typeSize + "\n";
    }
}
