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
 * This abstract class is the superclass of all runtime exceptions in the rasj package
 * except for {@link rasj.RasConnectionFailedException RasConnectionFailedException}.
 * @see rasj.RasException
 * @version $Revision: 1.4 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public abstract class RasRuntimeException extends RuntimeException
{
  static final String rcsid = "@(#)Package rasj, class RasRuntimeException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasRuntimeException.java,v 1.4 2003/12/19 16:22:27 rasdev Exp $";

 protected int errNo = 0;
 protected String errText = null;

    /**
     * Standard constructor.
     **/
 protected RasRuntimeException()
	{
	}

    /**
     * Constructor getting the error number.
     * @param num the RasDaMan error number
     **/
 protected RasRuntimeException( int num )
	{
        errNo = num;
	}

    /**
     * Constructor getting the error message.
     * @param msg the error message
     **/
 protected RasRuntimeException( String msg )
	{
        errText = ( (msg==null) ? "(null)" : msg );
	}

    /**
     * Constructor getting error number and error message.
     * @param num the RasDaMan error number
     * @param msg the error message
     **/
 protected RasRuntimeException( int num, String msg )
	{
        errNo = num;
        errText = ( (msg==null) ? "(null)" : msg );
	}
 
    /**
     * Method for retrieving the error number.
     * @return the error number
     **/
 public int getErrNo()
	{
        return errNo;
	}
 
    /**
     * Method retrieving the error message.
     * @return the error message
     **/
 public String getMessage()
	{
        return errText;
	}

}
