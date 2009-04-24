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
 * This runtime exception is raised when the 
 * {@link rasj.RasType RasType} of a query result is unknown
 * on client-side.
 * @version $Revision: 1.6 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasTypeUnknownException extends RasRuntimeException
{
  static final String rcsid = "@(#)Package rasj, class RasDimensionMismatchException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasTypeUnknownException.java,v 1.6 2003/12/19 16:22:27 rasdev Exp $";

    // the unknown type
    private String rType = null;

    /**
     * Standard constructor getting the type that caused the error.
     * @param type the unknown type that caused this exception
     **/
    protected RasTypeUnknownException(String type)
    {
	super(RasGlobalDefs.TYPE_UNKNOWN);
	rType = ( (type==null) ? "(null)" : type );
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
		i = msg.indexOf( RasGlobalDefs.KEYWORD_TYPE );
		if(i != -1)
		    buf.replace( i, i+RasGlobalDefs.KEYWORD_TYPE.length(), String.valueOf(rType) );
		msg = buf.toString();
		return msg;
	    }
        else
	    return super.getMessage();
    }

}
