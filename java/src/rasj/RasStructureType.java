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
 * This class represents all user defined structured types in the ODMG conformant
 * representation of the RasDaMan type system.
 * @version $Revision: 1.8 $
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasStructureType extends RasBaseType
{
    static final String rcsid = "@(#)Package rasj, class RasStructureType: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasStructureType.java,v 1.8 2003/12/10 21:04:23 rasdev Exp $";

    private RasBaseType[] baseTypes;
    private String[] attributes;

    public RasStructureType()
    {
	super();
	baseTypes = null;
	attributes = null;
    }

    public RasStructureType(String name, RasBaseType[] btyp, String[] attr)
    {
	super(name, 0);
	baseTypes = btyp;
	attributes = attr;

	for(int i=0; i < baseTypes.length/2; i++)
        {
	    typeSize = typeSize + baseTypes[i].getSize();
            //System.out.println(typeSize);
        }
	typeID = RasGlobalDefs.RAS_STRUCTURE;
    }

    public int getTypeID()
    {
	return RasGlobalDefs.RAS_STRUCTURE;
    }


    public boolean isStructType()
    {
	return true;
    }

    public String toString()
    {
        //System.out.println("struct type: ");
	String s = super.toString() + "struct " + super.typeName + "\n{\n";
	for(int i=0; i < (attributes.length/2)-1; i++)
	    {
		s = s + "  " + baseTypes[i] + " " + attributes[i] + ", \n";

		if(i == (attributes.length/2)-2)
		    {
			s = s + baseTypes[i+1] + " " + attributes[i+1] + "\n}\n";
		    }

	    }
	    return s;
    }
}
