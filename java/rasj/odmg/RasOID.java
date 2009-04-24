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
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package rasj.odmg;

import java.util.*;

/**
 * class represents an object identifier
 */
public class RasOID
{
    static final String rcsid = "@(#)Package rasj.odmg, class RasOID: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/odmg/RasOID.java,v 1.5 2003/12/10 21:04:30 rasdev Exp $";

    /**
     * system name
     */
    private String systemName;

    /**
     * database name
     */
    private String baseName;

    /**
     * local oid
     */
    private double localOID;

    /**
     * default constructor
     */
    public RasOID()
    {
	systemName = "";
	baseName = "";
	localOID = 0;
    }


    /**
     * constructor
     */
    public RasOID(String system, String base, double oid)
    {
        systemName = system;
	baseName = base;
	localOID = oid;
    }

    /**
     * String constructor
     */
    public RasOID(String oidString)
    {
        StringTokenizer str = new StringTokenizer(oidString, "|");
        if(str.hasMoreTokens())
          systemName = str.nextToken();
        if(str.hasMoreTokens())
	  baseName = str.nextToken();
        if(str.hasMoreTokens())
	  localOID = Double.parseDouble(str.nextToken());
    }

    /**
     * returns system name as a string
     */
    public String getSystemName()
    {
	return systemName;
    }

    /**
     * returns database name as a string
     */
    public String getBaseName()
    {
	return baseName;
    }

    /**
     * returns local oid as a double
     */
    public double getLocalOID()
    {
	return localOID;
    }

    /**
     * determines if oid is valid
     */
    public boolean isValid()
    {
	if (localOID != 0.0)
	    return true;
	else
	    return false;
    }


    public void rasDeactivate()
    {
	if(!systemName.equals(""))
	    {
		systemName = "";
	    }

	if(!baseName.equals(""))
	    {
		baseName = "";
	    }
    }


    /** gets the String representation of the oid */
    public String toString()
    {
	return systemName+"|"+baseName+"|"+localOID;
    }

}

