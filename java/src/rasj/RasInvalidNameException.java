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
 * This exception is thrown if a object name contains unaccepted characters
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasInvalidNameException extends RasRuntimeException
{
    static final String rcsid = "@(#)Package rasj, class RasInvalidNameException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasInvalidNameException.java,v 1.3 2003/12/19 16:22:27 rasdev Exp $";

    String wrongName = null;

    public RasInvalidNameException(String name)
    {
	super(RasGlobalDefs.INVALID_OBJECT_NAME_ERROR);
	
	wrongName = ( (name==null) ? "(null)" : name );
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
		i = msg.indexOf( RasGlobalDefs.KEYWORD_INVNAME );
		if(i != -1)
		    buf.replace(i, i+RasGlobalDefs.KEYWORD_INVNAME.length(),  wrongName);
		msg = buf.toString();
		return msg;
	    }
        else
	    return super.getMessage();
    }

}

