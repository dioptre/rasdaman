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
/*************************************************************
 * <pre>
 *
 * PURPOSE:
 * This runtime exception indicates an internal error on client-side , which cannot
 * be solved by the user. In case of such an event, please send a bug message to
 * info@active-knowledge.de containing the complete error message and a precise
 * description of the actions that lead to this exception. 
 * @version $Revision: 1.4 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasClientInternalException extends RasRuntimeException
{
    static final String rcsid = "@(#)Package rasj, class RasException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasClientInternalException.java,v 1.4 2003/12/19 16:22:27 rasdev Exp $";

    private int errNo = 0;
    private String errText = null;
    private String cName = null;
    private String mName = null;

    /**
     * standard constructor.
     **/
    public RasClientInternalException(String className, String methodName, String msg)
    {
	errNo = RasGlobalDefs.INTERNAL_CLIENT_ERROR;
	errText = ( (msg==null)        ? "(none)" : msg );
	cName   = ( (className==null)  ? "(none)" : className );
	mName   = ( (methodName==null) ? "(none)" : methodName );
    }

    /**
     * Returns the error message.
     * @return the error message.
     **/
    public String getMessage()
    {
	int index;

	String msg = RasErrorTexts.getErrorMessage(errNo);

	StringBuffer buf = new StringBuffer(msg);
	index = msg.indexOf( RasGlobalDefs.KEYWORD_CLASS );
	if(index != -1)
	    buf.replace(index, index+RasGlobalDefs.KEYWORD_CLASS.length(), cName);
	msg = buf.toString();
 
        buf = new StringBuffer(msg);
	index = msg.indexOf( RasGlobalDefs.KEYWORD_METHOD );
	if(index != -1)
	    buf.replace(index, index+RasGlobalDefs.KEYWORD_METHOD.length(), mName);
	msg = buf.toString();

	buf = new StringBuffer(msg);
	index = msg.indexOf( RasGlobalDefs.KEYWORD_CODE );
	if(index != -1)
	    buf.replace(index, index+RasGlobalDefs.KEYWORD_CODE.length(), errText);
	msg = buf.toString();
	return msg;
    }

}
