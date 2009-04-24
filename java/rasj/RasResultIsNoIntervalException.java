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
 * This exception is thrown if the result of an operation is no interval.
 * @version $Revision: 1.6 $
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasResultIsNoIntervalException extends RasException 
{
  static final String rcsid = "@(#)Package rasj, class RasResultIsNoIntervalException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasResultIsNoIntervalException.java,v 1.6 2003/12/10 21:04:23 rasdev Exp $";

    /**
     * Standard constructor.
     **/
  protected RasResultIsNoIntervalException()
	{
        super(RasGlobalDefs.RESULT_IS_NO_INTERVAL);
	}

    /**
     * Returns the error message.
     * @return the error message.
     **/
    public String getMessage()
    {
	if(super.getMessage() == null)
	    return RasErrorTexts.getErrorMessage(errNo);
        else
	    return super.getMessage();
    }
  
}

