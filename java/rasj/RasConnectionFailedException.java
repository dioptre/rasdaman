package rasj;

import rasj.global.*;
import org.odmg.*;

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
 * This exception is raised when the connection to the RasDaMan server fails. This typically happens
 * when the RasDaMan server is either not running or unable to connect to the base DBMS. Also 
 * communication failures between RasManager and RasServer may produce this kind of exception.
 * @version $Revision: 1.8 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasConnectionFailedException extends ODMGRuntimeException
{
    static final String rcsid = "@(#)Package rasj.clientcommhttp, class RasConnectionFailedException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasConnectionFailedException.java,v 1.8 2003/12/19 16:22:27 rasdev Exp $";

    /**
     * The RasDaMan erro number
     **/
    private int errorNo = 0;

    /**
     * Optional parameter
     **/
    private String param = null;

    /**
     * Default constructor taking the rasdaman error number and an optional parameter.
     * @param errNo the rasdaman error number
     * @param parameter optional parameter that can be inserted into the error message
     **/
    public RasConnectionFailedException(int errNo, String parameter)
    {
	super();
	errorNo = errNo;
	param = ( (parameter==null) ? "(null)" : parameter );
    }
 
    /**
     * Returns the RasDaMan error number
     */
    public int getErrorNo()
    {   
        return errorNo;
     }
   
    /**
     * Returns the error message.
     **/
    public String getMessage()
    {
	String msg = super.getMessage();
	if(msg == null)
	    {
		msg = RasErrorTexts.getErrorMessage(errorNo);
		int index = 0;

		// replace parameters
		switch(errorNo)
		    {
		    case RasGlobalDefs.MANAGER_CONN_FAILED:
			StringBuffer buf = new StringBuffer(msg);
			index = msg.indexOf( RasGlobalDefs.KEYWORD_URL );
			if(index != -1)
			    buf.replace(index, index+RasGlobalDefs.KEYWORD_URL.length(), param);
			msg = buf.toString();
			break;
		    }
	    }
	return msg;
    }


}
