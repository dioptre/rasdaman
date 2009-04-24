package rasj;

import java.lang.*;
import java.util.*;

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
 * This class represents an n-dimensional point vector.
 * @version $Revision: 1.9 $
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasPoint
{
    static final String rcsid = "@(#)Package rasj, class RasPoint: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasPoint.java,v 1.9 2003/12/10 21:04:23 rasdev Exp $";

  // array holding the point coordinates
    private long[] points;
    // dimensionality of the point
    private int dimensionality;
    // number of components initialized already
    private int streamInitCnt;

    /**
     * Constructor getting the dimensionality for stream initialization.
     * @param dim the dimensionality of this point
     **/
    public RasPoint(int dim)
    {
	dimensionality = dim;
	streamInitCnt = 0;

	points = new long[dimensionality];

	for(int i=0; i< dimensionality; i++)
	    points[i] = 0;
    }

    /**
     * Method for stream initialization of this point.
     * @param newElement a new dimension that is added to this point
     **/
    public RasPoint stream(long newElement) throws RasStreamInputOverflowException
    {
	if(streamInitCnt >= dimensionality)
	    throw new RasStreamInputOverflowException();

	points[streamInitCnt++] = newElement;
	return this;
    }

    /**
     * constructor taking a string representation for this point (for example "[1, 2, 3]").
     * @param stringRep the string representation for this point
     **/
    public RasPoint(String stringRep)
    {
	dimensionality = 1;
	streamInitCnt = 0;

	if(stringRep.trim().charAt(0) != '[')
	    {
		// error
		dimensionality = 0;
		return;
	    }

	// for parsing the string
	StringTokenizer strTok = new StringTokenizer(stringRep.trim(), "[,]");
	String strCurTok = null;

	// calculate dimensionality
	dimensionality = strTok.countTokens();

	points = new long[dimensionality];

	for(int i=0; i<dimensionality; i++)
	    {
		strCurTok = strTok.nextToken();
		points[i] = Long.parseLong(strCurTok.trim());
	    }
    }

    /**
     * Easy-to-use constructor for two dimensional points.
     * @param p1 the value of the first dimension
     * @param p2 the value of the second dimension
     **/
    public RasPoint(long p1, long p2)
    {
	dimensionality = 2;
	streamInitCnt = 2;

	points    = new long[dimensionality];
	points[0] = p1;
	points[1] = p2;
    }

    /**
     * Easy-to-use constructor for three dimensional points.
     * @param p1 the value of the first dimension
     * @param p2 the value of the second dimension
     * @param p3 the value of the third dimension
     **/
    public RasPoint(long p1, long p2, long p3)
    {
	dimensionality =3;
	streamInitCnt = 3;

	points    = new long[dimensionality];
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
    }

    /**
     * Easy-to-use constructor for four dimensional points.
     * @param p1 the value of the first dimension
     * @param p2 the value of the second dimension
     * @param p3 the value of the third dimension
     * @param p4 the value of the fourth dimension
     **/
    public RasPoint(long p1, long p2, long p3, long p4)
    {
	dimensionality = 4;
	streamInitCnt = 4;

	points    = new long[dimensionality];
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
    }

    /**
     * Easy-to-use constructor for five dimensional points.
     * @param p1 the value of the first dimension
     * @param p2 the value of the second dimension
     * @param p3 the value of the third dimension
     * @param p4 the value of the fourth dimension
     * @param p5 the value of the fifth dimension
     **/
    public RasPoint(long p1, long p2, long p3, long p4, long p5)
    {
	dimensionality = 5;
	streamInitCnt = 5;

	points    = new long[dimensionality];
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
	points[4] = p5;
    }

    /**
     * Default constructor.
     **/
    public RasPoint()
    {
	dimensionality = 0;
	streamInitCnt = 0;

	points = null;
    }

    /**
     * Copy constructor that initializes this point with the values of the given one.
     * @param pt the point used to copy the values from
     **/
    public RasPoint(final RasPoint pt)
    {
	dimensionality = pt.dimension();
	streamInitCnt  = dimensionality;
	points         = new long[ dimensionality ];

	try {
	    for(int i=0; i<dimensionality; i++)
		points[i] = pt.item(i);
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    System.out.println("Error in RasPoint copy constructor.");
	}
    }

    /**
     * Read access to the i-th coordinate.
     * @param i the dimension that is to be read
     * @return the value of the i-th coordinate
     **/
    public long item(int i) throws RasIndexOutOfBoundsException
    {
	if(i < 0 || i >= dimensionality)
	    throw new RasIndexOutOfBoundsException(0, dimensionality-1, i);

	return points[i];
    }

    /**
     * write access to the i-th coordinate
     * @param i the coordinate that is to be accessed
     * @param value the value that is to be assigned to the specified coordinate
     **/
    public void setItem(int i, long value) throws RasIndexOutOfBoundsException
    {
	if(i < 0 || i >= dimensionality)
	    throw new RasIndexOutOfBoundsException(0, dimensionality-1, i);
	points[i] = value;
    }

    /**
     * This method copies the values of the given point to the current point. All
     * previously defined values of the current point will be deleted.
     * @param pt the point to be copied
     * @return the current point with its new values
     **/
    public final RasPoint setTo(final RasPoint pt) throws RasIndexOutOfBoundsException
    {
	if(this != pt)
	    {
		if((points != null) && dimensionality != pt.dimension())
		    {
			points = null;
		    }

		dimensionality = pt.dimension();
		streamInitCnt  = dimensionality;

		if(points == null)
		    points = new long[ dimensionality ];

		for(int i=0; i<dimensionality; i++)
		    points[i] = pt.item(i);

	    }

	return this;
    }

    /**
     * Compares this point to another point.
     * @param p the point to be compared
     * @return
     * <TABLE BORDER=0 CELLPADDING=3>
     * <TR><TD ALIGN=RIGHT VALIGN=TOP><B>-2</B></TD><TD> if the points have not the same dimensionality</TD></TR>
     * <TR><TD ALIGN=RIGHT VALIGN=TOP><B>-1</B></TD><TD> if the current point is "lesser" than point p.
     * <BR>In this context, "lesser" refers to the comparison of the coordinates in decreasing order 
     * of magnitude For example, the point (2,2,9) is "lesser" than the point (2,3,1).</TD></TR>
     * <TR><TD ALIGN=RIGHT VALIGN=TOP><B>0</B></TD><TD> if the two points have an equal value</TD></TR>
     * <TR><TD ALIGN=RIGHT VALIGN=TOP><B>1</B></TD><TD> if the current point is "greater" than point p
     * ("greater" is the opposite of "lesser").</TD></TR>
     * </TABLE>
     **/
    public final int comparedWith(final  RasPoint p)
    {
	if(dimensionality != p.dimensionality)
	    return -2;

	try {
	    for (int i = 0; i < dimensionality; i++)
		{
		    if (points[i] > p.item(i))
			return 1;
		    if (points[i] < p.item(i))
			return -1 ;
		}
	    return 0;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    System.out.println("Error in method RasPoint.comparedWith().");
	    return -2;
	}
    }

    /**
     * Method for testing equality of two points.<BR>Two points are equal 
     * if they have the same dimensionality and identic values in each dimension.
     * @param pt the point that is compared to the current point
     * @return true if the two points are equal
     **/
    public boolean equals(final RasPoint pt) throws RasDimensionMismatchException
    {
	boolean returnValue = false;

	if(dimensionality != pt.dimensionality)
        {
            throw new RasDimensionMismatchException(dimensionality, pt.dimensionality);
        }
	try {
	    returnValue = true;
	    for(int i=0; i<dimensionality && returnValue ; i++)
		returnValue &= (points[i] == pt.item(i));
	    return returnValue;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    return false;
	}
    }

    /**
     * Method for testing inequality. This is the negation of the equals method.
     * @param pt the point that is compared to the current point
     * @return true if the two points are not equal
     **/
    public boolean notEquals(final RasPoint pt) throws RasDimensionMismatchException
    {
	return !equals(pt);
    }

    /**
     * Method for vector addition. The current point will be modified to contain
     * the result of the addition.
     * @param pt the point ot be added to the current point
     * @return the current point after the addition
     **/
    public RasPoint add(final RasPoint pt) throws RasDimensionMismatchException
    {
	if(dimensionality != pt.dimension())
	    throw new RasDimensionMismatchException(dimensionality, pt.dimension());

	try {
	    RasPoint result = new RasPoint(dimensionality);
	    for(int i=0; i<dimensionality; i++)
		{
		    result.stream(points[i] + pt.item(i));
		}
	    return result;
	}
	catch(RasException e) {
	    // this cannot occur (theoretically)
	    System.err.println("Error in method RasPoint.add().");
	    return null;
	}
    }


    /**
     * Method for vector multiplication. The current point will be modified to contain
     * the result of the multiplication.
     * @param pt the point ot be multiplied to the current point
     * @return the current point after the multiplication
     **/
    public RasPoint mult(final RasPoint pt) throws RasDimensionMismatchException
    {
	if(dimensionality != pt.dimension())
	    throw new RasDimensionMismatchException(dimensionality, pt.dimension());

	try {
	    RasPoint result= new RasPoint(dimensionality);
	    for(int i=0; i<dimensionality; i++)
		{
		    result.stream(points[i] * pt.item(i));
		}
	    return result;
	}
	catch(RasException e) {
	    // this cannot occur (theoretically)
	    System.err.println("Error in method RasPoint.mult().");
	    return null;
	}
    }


    /**
     * Gets the dimensionality of this point.
     * @return the dimensionality of this point
     **/
    public int dimension()
    {
	return dimensionality;
    }


    /**
     * Gives back the string representation of this point.
     * @return the string representation of this point
     **/
    public String toString()
    {
	String retString = "";
	if(dimensionality > 0)
	    {
		for(int i=0; i<dimensionality-1; i++)
		    {
			retString = retString + Long.toString(points[i]) + ",";
		    }
		retString = retString + Long.toString(points[dimensionality-1]);
	    }

	return "[" + retString  + "]";
    }

}
