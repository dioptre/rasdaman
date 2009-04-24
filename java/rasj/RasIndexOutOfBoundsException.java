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
 * This exception is thrown when a cell index for an MDD exceeds
 * the objects's bounds.
 * @version $Revision: 1.6 $
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasIndexOutOfBoundsException extends RasException
{
    static final String rcsid = "@(#)Package rasj, class RasIndexOutOfBoundsException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasIndexOutOfBoundsException.java,v 1.6 2003/12/19 16:22:27 rasdev Exp $";
   
    // lower bound
    private long low;
    // upper bound
    private long high;
    // index which caused the error
    private long index;

    /**
     * stamdard constructor getting lower bound, upper bound and the index.
     * @param dlow lower bound of the object
     * @param dhigh upper bound of the object
     * @param dindex the index that caused this exception
     **/
    protected RasIndexOutOfBoundsException(long dlow, long dhigh, long dindex )
    {
        super(RasGlobalDefs.INDEX_OUT_OF_BOUNDS);
	low = dlow;
	high = dhigh;
	index = dindex;
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
		i = msg.indexOf( RasGlobalDefs.KEYWORD_LOW );
		if(i != -1)
		    buf.replace(i, i+RasGlobalDefs.KEYWORD_LOW.length(), String.valueOf(low));
		msg = buf.toString();
		i = msg.indexOf( RasGlobalDefs.KEYWORD_HIGH );
		if(i != -1)
		    buf.replace(i, i+RasGlobalDefs.KEYWORD_HIGH.length(), String.valueOf(high));
		msg = buf.toString();
		i = msg.indexOf( RasGlobalDefs.KEYWORD_INDEX );
		if(i != -1)
		    buf.replace(i, i+RasGlobalDefs.KEYWORD_INDEX.length(), String.valueOf(index));
		msg = buf.toString();
		return msg;
	    }
        else
	    return super.getMessage();
    }

    /**
     * Returns the lower bound of the accessed object.
     * @return the lower bound of the object
     **/
    public long getLow()
    {
        return low;
    }

    /**
     * Returns the higher bound of the accessed object.
     * @return the higher bound of the object
     **/
    public long getHigh()
    {
        return high;
    }

    /**
     * Returns the index responsible for throwing this exception.
     * @return the index that caused the exception 
     **/
    public long getIndex()
    {
        return index;
    }

}

