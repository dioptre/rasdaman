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
 * This class the superclass for all types in the ODMG conformant
 * representation of the RasDaMan type system.
 * @version $Revision: 1.10 $
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasType
{
    static final String rcsid = "@(#)Package rasj, class RasType: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasType.java,v 1.10 2003/12/10 21:04:23 rasdev Exp $";

    protected String typeName;
    protected int typeID;

    /**
     * Default constructor.
     **/
    public RasType()
    {
	typeName = "";
	typeID = 0;
    }

    /**
     * Constructor getting the name of the new type.
     * @param newTypeName the name of the new type
     **/
    public RasType(String newTypeName)
    {
	typeName = newTypeName;
	typeID = 0;
    }

    /**
     * Retrieves the id of this type.
     * @return the type id
     **/
    public int getTypeID()
    {
	return typeID;
    }

    /**
     * Retrieves the name of this type.
     * @return the type name
     **/
    public String  getName()
    {
	return typeName;
    }

    /**
     * Checks if this type is a primitive or structured type.
     * @return true if this is a structured type, otherwise false
     **/
    public boolean isStructType()
    {
	return false;
    }

    /**
     * Checks if this type is a base type (primitive type or structure type).
     * @return true for a primitive or structured type, otherwise false
     **/
    public boolean isBaseType()
    {
	return false;
    }


    /**
     * Builds the type schema from a string representation.
     * @param typeString the string representation of the type
     **/
    public static RasType getAnyType(String typeString)
    {
	StringTokenizer strTok = new StringTokenizer(typeString, "{}[]<>,: ");
	String currentStr = "";
	RasType returnValue = null;
	if(strTok.hasMoreTokens())
	    {
		currentStr = strTok.nextToken();
		if(currentStr.equals("set"))
		    {
	              //System.out.println("getSet");
		      returnValue = getSet(strTok.nextToken(""));
		    }
		else if(currentStr.equals("marray"))
		    {
                      //System.out.println("getMArray");
		      returnValue = getMArrayType(strTok.nextToken(""));
		    }
		else
		    {
		      //System.out.println("getType");
		      returnValue = getType(typeString);
		    }
	    }
	return returnValue;
    }

    // converts array of cells from NT byte order to Unix byte order.
    //public void convertToLittleEndian(String cells, int noCells)
    //{
    //}

    // converts array of cells from Unix byte order to NT byte order.
    //public void convertToBigEndian(String cells, int noCells)
    //{
    //}

    private static RasType getSet(String setStr)
    {
	StringTokenizer setTok = new StringTokenizer(setStr, "{}[]<>,: ");
	String currentStr = "";
	RasType returnValue = null;
	RasType elementType = null;
	if(setTok.hasMoreTokens())
	    {
		currentStr = setTok.nextToken();
		//Fehler falls kein <!
		if(currentStr.equals("marray"))
		    elementType = getMArrayType(setTok.nextToken(""));
		else elementType = getType(setStr);
	    }

	returnValue = new RasCollectionType(elementType);

	return returnValue;
    }

    private static RasType getType(String typeStr)
    {
	StringTokenizer typeTok = new StringTokenizer(typeStr, "{}[]<>,: ");
	String currentStr = "";
	RasType returnValue = null;
	if(typeTok.hasMoreTokens())
	    {
		currentStr = typeTok.nextToken();
		if(currentStr.equals("struct"))
		    {
			//System.out.println("getStructureType");
			returnValue = getStructureType(typeTok.nextToken(""));
		    }
		else if(currentStr.equals("interval"))
		    {
			returnValue = getSIntervalType(typeTok.nextToken(""));
                        returnValue.typeID = RasGlobalDefs.RAS_SINTERVAL;
		    }
		else if(currentStr.equals("minterval"))
		    {
			returnValue = getMIntervalType(typeTok.nextToken(""));
                        returnValue.typeID = RasGlobalDefs.RAS_MINTERVAL;
		    }
		else if(currentStr.equals("point"))
		    {
			returnValue = getPointType(typeTok.nextToken(""));
                        returnValue.typeID = RasGlobalDefs.RAS_POINT;
		    }
		else if(currentStr.equals("oid"))
		    {
			returnValue = getOIDType(typeTok.nextToken(""));
                        returnValue.typeID = RasGlobalDefs.RAS_OID;
		    }
		else
		    {
			//System.out.println("getPrimitiveType");
			returnValue = getPrimitiveType(typeStr);
		    }
	    }
	return returnValue;
    }

    private static RasMArrayType getMArrayType(String mArrayStr)
    {
	//StringTokenizer mArrayTok = new StringTokenizer(mArrayStr, "{}[]<>,: ");
        //System.out.println("enter getMArrayType" + mArrayStr);
	RasMArrayType returnValue = null;
	RasBaseType baseType = null;
	//Fehler falls kein <!
	// get base type (structure or primitive type)
	baseType = getBaseType(mArrayStr);
	//System.out.println("base type: " + baseType);
	returnValue = new RasMArrayType(baseType);

	return returnValue;
    }


    private static RasBaseType getBaseType(String baseStr)
    {
	StringTokenizer baseTok = new StringTokenizer(baseStr, "{}[]<>,: ");
	String currentStr = "";
        //System.out.println(baseStr);
	RasBaseType returnValue = null;
	if(baseTok.hasMoreTokens())
	    {
		currentStr = baseTok.nextToken();
		if(currentStr.equals("struct"))
		    {
			returnValue = getStructureType(baseTok.nextToken(""));
		    }
		else
		    {
			returnValue = getPrimitiveType(baseStr);
			//System.out.println("getPrimitiveType");
		    }
	    }

	return returnValue;
    }


    private static RasPrimitiveType getPrimitiveType(String primStr) throws RasTypeUnknownException
    {
	StringTokenizer primTok = new StringTokenizer(primStr, "{}[]<>,: ");
	String currentStr = "";
	RasPrimitiveType returnValue = null;
	if(primTok.hasMoreTokens())
	    {
		currentStr = primTok.nextToken();
		if(currentStr.equals("char"))
		    returnValue = new RasPrimitiveType("RAS_CHAR", RasGlobalDefs.RAS_CHAR);
		else if(currentStr.equals("octet"))
		    returnValue = new RasPrimitiveType("RAS_BYTE", RasGlobalDefs.RAS_BYTE);
		else if(currentStr.equals("short"))
		    returnValue = new RasPrimitiveType("RAS_SHORT", RasGlobalDefs.RAS_SHORT);
		else if(currentStr.equals("ushort"))
		    returnValue = new RasPrimitiveType("RAS_USHORT", RasGlobalDefs.RAS_USHORT);
		else if(currentStr.equals("long"))
		    returnValue = new RasPrimitiveType("RAS_LONG", RasGlobalDefs.RAS_LONG);
		else if(currentStr.equals("ulong"))
		    returnValue = new RasPrimitiveType("RAS_ULONG", RasGlobalDefs.RAS_ULONG);
		else if(currentStr.equals("bool"))
		    returnValue = new RasPrimitiveType("RAS_BOOLEAN", RasGlobalDefs.RAS_BOOLEAN);
		else if(currentStr.equals("float"))
		    returnValue = new RasPrimitiveType("RAS_FLOAT", RasGlobalDefs.RAS_FLOAT);
		else if(currentStr.equals("double"))
		    returnValue = new RasPrimitiveType("RAS_DOUBLE", RasGlobalDefs.RAS_DOUBLE);
		else
		    throw new RasTypeUnknownException(currentStr);
	    }

	return returnValue;
    }


    private static RasStructureType getStructureType(String structStr) throws RasTypeUnknownException
    {
	StringTokenizer structTok = new StringTokenizer(structStr, "[]<>,: ");
	RasStructureType returnValue = null;
	String structName = "";
	RasBaseType[] baseType = new RasBaseType[structTok.countTokens()/2];
	String[] attributeName = new String[structTok.countTokens()/2];
	//System.out.println("enter getStructureType");
	// get struct name
	if(structTok.hasMoreTokens())
		    {
			structName = structTok.nextToken();
                        if(structName.equals("{"))
                          structName="";
                        else
                          structTok.nextToken();
			//System.out.println("StructName:" + structName);
		    }
	else
	    {
		// no struct name
		throw new RasTypeUnknownException("");
	    }

	//Fehler falls kein { so lange bis } dazwischen ,!!!
	for(int i=0; i < attributeName.length; i++)
	    {
		// get type
                String more = structTok.nextToken();
                if(more.equals("}"))
                  break;
                else
                {
		  baseType[i] = getBaseType(more);
		  //System.out.println("BaseType:"+ i + baseType[i]);
		  // get attribut name
		  if(structTok.hasMoreTokens())
		    {
			attributeName[i] = structTok.nextToken();
			//System.out.println("Attribut:"+ i + attributeName[i]);
		    }
		  else
		    // no attribut name
		    throw new RasTypeUnknownException("");
                }
	    }

	returnValue = new RasStructureType(structName, baseType, attributeName);
	return returnValue;
    }

    private static RasSIntervalType getSIntervalType(String sintStr)
    {
	return new RasSIntervalType();
    }

    private static RasMIntervalType getMIntervalType(String mintStr)
    {
	return new RasMIntervalType();
    }

    private static RasPointType getPointType(String pointStr)
    {
	return new RasPointType();
    }

    private static RasOIDType getOIDType(String oidStr)
    {
	return new RasOIDType();
    }

    /**
     * @return the string represntation of this type
     **/
    public String toString()
    {
	return "typeName: " + typeName + "\n typeID:  " + typeID +"\n ";
    }

}
