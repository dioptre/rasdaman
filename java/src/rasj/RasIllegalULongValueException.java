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
 * This exception is thrown if a {@link rasj.RasMArrayLong RasMArrayLong} is trying to be sent to the
 * server where one or more cell values exceed 2^32 or are negative. Such cell values are illegal 
 * because the ODMG standard restricts unsigned long values in C++ (i.e. in the RasDaMan server) 
 * to 4 bytes, whereas java long values have 8 bytes.
 * <p>
 * Although in this case the server would store only the least 4 bytes of the cell value without throwing an 
 * exception, the java client interface does not allow sending such illegal long values in order
 * to enforce application integrity.    
 * @see rasj.RasMArrayLong
 * @version $Revision: 1.3 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasIllegalULongValueException extends RasRuntimeException
{
    static final String rcsid = "@(#)Package rasj, class RasIllegalULongValueException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasIllegalULongValueException.java,v 1.3 2003/12/19 16:22:27 rasdev Exp $";

    // the base type
    long illegalValue = 0;

    /**
     * Standard constructor getting the illegal long value
     * @param illegalLongValue the cell value that caused the error
     **/
    public RasIllegalULongValueException(long illegalLongValue)
    {
	super(RasGlobalDefs.ILLEGAL_ULONG_VALUE);
	illegalValue = illegalLongValue;
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
		i = msg.indexOf( RasGlobalDefs.KEYWORD_VAL );
		if(i != -1)
		    buf.replace(i, i+RasGlobalDefs.KEYWORD_VAL.length(), String.valueOf(illegalValue));
		msg = buf.toString();
		return msg;
	    }
        else
	    return super.getMessage();
    }

}
