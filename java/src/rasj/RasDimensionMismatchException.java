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
 * This exception is thrown if the dimensions of two objects do not match.
 * @version $Revision: 1.6 $
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasDimensionMismatchException extends RasException 
{
    static final String rcsid = "@(#)Package rasj, class RasDimensionMismatchException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasDimensionMismatchException.java,v 1.6 2003/12/19 16:22:27 rasdev Exp $";

    // first dimensionality    
    private long dim1;
    // second dimensionality    
    private long dim2;

    /**
     * Standard constructor getting two dimensionalities.
     * @param pdim1 the dimension of the first object
     * @param pdim2 the dimension of the second object
     **/
    protected RasDimensionMismatchException( long pdim1, long pdim2)
    {
	super(RasGlobalDefs.DIMENSION_MISMATCH);
	dim1 = pdim1;
	dim2 = pdim2;
    }

    /**
     * Returns the error message.
     * @return the error message.
     **/
    public String getMessage()
    {
	int index;

	if(super.getMessage() == null)
	    {
		String msg = RasErrorTexts.getErrorMessage(errNo);

		StringBuffer buf = new StringBuffer(msg);
		index = msg.indexOf( RasGlobalDefs.KEYWORD_DIM1 );
		if(index != -1)
		    buf.replace(index, index+RasGlobalDefs.KEYWORD_DIM1.length(), String.valueOf(dim1));
		msg = buf.toString();
		index = msg.indexOf( RasGlobalDefs.KEYWORD_DIM1 );
		if(index != -1)
		    buf.replace(index, index+RasGlobalDefs.KEYWORD_DIM1.length(), String.valueOf(dim2));
		msg = buf.toString();
		return msg;
	    }
        else
	    return super.getMessage();
    }

    /**
     * Returns the dimension of the first object.
     * @return the dimension of the first object
     **/
    public long getDim1()
    {
        return dim1;
    }

    /**
     * Returns the dimension of the second object.
     * @return the dimension of the second object
     **/
    public long getDim2()
    {
        return dim2;
    }
}
