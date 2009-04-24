package rasj;

import rasj.global.*;

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
 * This exception is thrown if a {@link rasj.RasPrimitiveType primitive type} is accessed by a method 
 * having the wrong type.
 * @see rasj.RasPrimitiveType
 * @version $Revision: 1.7 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasTypeInvalidException extends RasException
{
    static final String rcsid = "@(#)Package rasj, class RasTypeInvalidException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasTypeInvalidException.java,v 1.7 2003/12/19 16:22:27 rasdev Exp $";

    // the base type
    String bType = null;

    // the type of the access method
    String aType = null;

    /**
     * Standard constructor getting the type of the base type and the type of the
     * access method that caused the error.
     * @param baseType the type of the base type
     * @param accessType the type of the access method that caused the error
     **/
    protected RasTypeInvalidException(String baseType, String accessType)
    {
	super(RasGlobalDefs.TYPE_INVALID);
	bType = ( (baseType==null) ? "(null)" : baseType );
	aType = ( (accessType==null) ? "(null)" : accessType );
    }

    /**
     * Returns the error message.
     * @return the error message.
     **/
    public String getMessage()
    {
	int i;

	if(super.getMessage() == null)
	    {
		String msg = RasErrorTexts.getErrorMessage(errNo);

		StringBuffer buf = new StringBuffer(msg);
		i = msg.indexOf( RasGlobalDefs.KEYWORD_ATYPE );
		if(i != -1)
		    buf.replace(i, i+RasGlobalDefs.KEYWORD_ATYPE.length(), String.valueOf(bType));
		msg = buf.toString();
		i = msg.indexOf( RasGlobalDefs.KEYWORD_BTYPE );
		if(i != -1)
		    buf.replace(i, i+RasGlobalDefs.KEYWORD_BTYPE.length(), String.valueOf(aType));
		msg = buf.toString();
		return msg;
	    }
        else
	    return super.getMessage();
    }

}
