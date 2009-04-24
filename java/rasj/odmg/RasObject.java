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

import rasj.*;
import rasj.global.*;
import org.odmg.*;

public class RasObject implements RasGlobalDefs
{
    static final String rcsid = "@(#)Package rasj.odmg, class RasObject: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/odmg/RasObject.java,v 1.9 2003/12/10 21:04:30 rasdev Exp $";

    /**
     * stores object name if it has one
     */
    protected String objectName;

    /**
     * stores object type name if it has one
     */
    private String typeName;

    private int status;
    private int lock;
    private int type;
    private String typeStructure = "";

    /**
     * object identifier
     */
    private RasOID oid;

    /**
     * default constructor
     */
    public RasObject()
    {
	objectName = "";
	typeName = "";
	status = 0;
	lock = 0;
	oid = new RasOID();
	type = 0;
    }

    public RasObject(int objType)
    {
	objectName = "";
	typeName = "";
	status = 0;
	lock = 0;
	oid = new RasOID();
	type = objType;
    }

    public RasObject(RasObject obj, int objType)
    {
	objectName = "";
	typeName = "";
	status = 0;
	lock = 0;
	oid = new RasOID();
	type = objType;
    }

    /**
     * get oid
     */
    public RasOID getOID()
    {
        return oid;
    }

     /**
     * set oid
     */
    public void setOID(RasOID o)
    {
        oid = o;
    }

    /**
     * set object name
     */
    public void setObjectName(String name) throws RasInvalidNameException
    {
	verifyName(name);
	
	objectName = name;
    }

    /**
     * set object type
     */
    public void setObjectType(int t)
    {
	type = t;
    }

    /**
     * set object type
     */
    public void setObjectTypeName(String name) throws RasInvalidNameException
    {
        verifyName(name);
	
	typeName = name;
    }

    /**
     * set object type structure
     */
    public void setTypeStructure(String structure)
    {
	typeStructure = structure;
    }

    /**
     * get type Structure
     */
    public String getTypeStructure()
    {
	return typeStructure;
    }

    /**
     * get object name
     */
    public String getObjectName()
    {
	return objectName;
    }

    /**
     * get object status
     */
    public int getStatus()
    {
	return status;
    }


    /**
     * get object type
     */
    public int getObjectType()
    {
	return type;
    }

    /**
     * get object typeName
     */
    public String getObjectTypeName()
    {
	return typeName;
    }

    /**
     * set object status
     */
    public void setStatus(int newStatus)
    {
	status = newStatus;
    }

    /**
     * get object lock
     */
    public int getLock()
    {
	return lock;
    }

    /**
     * set object lock
     */
    public void setLock(int lockMode)
    {
	lock = lockMode;
    }

    public void rasDeactivate()
    {
	objectName = "";
	status = 0;
	lock = 0;
	oid.rasDeactivate();
    }

    public RasType getTypeSchema()
    {
	return null;
    }

    
    private void verifyName(String name) throws RasInvalidNameException
      {
        for(int i = 0;i < name.length(); i++)
	  {
	    char c = name.charAt(i);
	    	    
	    if(c == '_' || Character.isLetter(c)) continue;
	    
	    if(Character.isDigit(c) && i > 0) continue;
	    
	    throw new RasInvalidNameException(name);
	   }
       }

}

