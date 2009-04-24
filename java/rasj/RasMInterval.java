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
 * The spatial domain of an MDD is represented by an object
 * of class RasMInterval. It specifies lower and upper bound
 * of the point set for each dimension of an MDD. Internally,
 * the class is realized through an array of intervals of type
 * RasSInterval.
 *
 * For the operations union, difference, and intersection the
 * dimensionalties of the operands must be equal, otherwise an
 * exception is raised. The semantics of the operations are
 * defined as follows for each dimension:
 *
 *    | ...  fixed bound
 *    -* ...  open bound
 *
 *
 * class   orientation       union    difference  intersection
 * -----------------------------------------------------------
 *   1     |-a-| |-b-|       error    a           error
 *
 *   2     |-a-|             [a1,b2]  [a1,b1]     [b1,a2]
 *   2        |-b-|
 *
 *   3     |--a--|           a        error       b
 *   3      |-b-|
 *
 *   4     |-b-|             [b1,a2]  [b2,a2]     [a1,b2]
 *   4        |-a-|
 *
 *   5     |--b--|           b        error       a
 *   5      |-a-|
 *
 *   6     |-b-| |-a-|       error    a           error
 *
 *   7     |-a-|-b-|         [a1,b2]  a           [a2,a2]
 *
 *   8     |-b-|-a-|         [b1,a2]  a           [b2,b2]
 *
 *   9     |--a--|           a        [a1,b1]     b
 *   9       |-b-|
 *
 *  10     |--a--|           a        [b2,a2]     b
 *  10     |-b-|
 *
 *  11     |-a-|             a        error       a
 *  11     |-b-|
 *
 *  12     |--b--|           b        error       a
 *  12     |-a-|
 *
 *  13     |--b--|           b        error       a
 *  13       |-a-|
 *
 *  -----------------------------------------------------
 *
 *  14     |--a--*           a        error       b
 *  14      |-b-|
 *
 *  15     |--a--*           a        [b2,a2]     b
 *  15     |-b-|
 *
 *  16     |-b-| |-a-*       error    a           error
 *
 *  17     |-b-|-a-*         [b1,a2]  a           [b2,b2]
 *
 *  18      |--a--*          [b1,a2]  [b2,a2]     [a1,b2]
 *  18     |-b-|
 *
 *  -----------------------------------------------------
 *
 *  19     *--a--|          a        error       b
 *  19      |-b-|
 *
 *  20     *--a--|          a        [a1,b1]     b
 *  20       |-b-|
 *
 *  21     *-a-| |-b-|       error    a           error
 *
 *  22     *-a-|-b-|         [a1,b2]  a           [a2,a2]
 *
 *  23     *--a--|           [a1,b2]  [a1,b1]     [b1,a2]
 *  23        |-b-|
 *
 *  -----------------------------------------------------
 *
 *  24     |--b--*           b        error       a
 *  24      |-a-|
 *
 *  25     |--b--*           b        error       a
 *  25     |-a-|
 *
 *  26     |-a-| |-b-*       error    a           error
 *
 *  27     |-a-|-b-*         [a1,b2]  a           [a2,a2]
 *
 *  28      |--b--*          [a1,b2]  [a1,b1]     [b1,a2]
 *  28     |-a-|
 *
 *  -----------------------------------------------------
 *
 *  29     *--b--|           b        error       a
 *  29      |-a-|
 *
 *  30     *--b--|           b        error       a
 *  30       |-a-|
 *
 *  31     *-b-| |-a-|       error    a           error
 *
 *  32     *-b-|-a-|         [b1,a2]  a           [b2,b2]
 *
 *  33     *--b--|           [b1,a2]  [b2,a2]     [a1,b2]
 *  33        |-a-|
 *
 *  -----------------------------------------------------
 *
 *  34     *-a-| |-b-*       error    a           error
 *
 *  35     *-a-|-b-*         [a1,b2]  a           [a2,a2]
 *
 *  36     *-a-|             [a1,b2]  [a1,b1]     [b1,a2]
 *  36        |-b-*
 *
 *  -----------------------------------------------------
 *
 *  37     *-b-| |-a-*       error    a           error
 *
 *  38     *-b-|-a-*         [b1,a2]  a           [b2,b2]
 *
 *  39     *-b-|             [b1,a2]  [a1,b1]     [a1,b2]
 *  39        |-a-*
 *
 *  -----------------------------------------------------
 *
 *  40     *-a-|             b        error       a
 *  40      *-b-|
 *
 *  41     *-a-|             a        error       a
 *  41     *-b-|
 *
 *  42     *-b-|             a        [b2,a2]     b
 *  42      *-a-|
 *
 *  -----------------------------------------------------
 *
 *  43     |-a-*             a        [a1,b1]     b
 *  43      |-b-*
 *
 *  44     |-a-*             a        error       a
 *  44     |-b-*
 *
 *  45     |-b-*             b        error       a
 *  45      |-a-*
 *
 *  -----------------------------------------------------
 *  46     *-a-* |-b-|       a        error       b
 *
 *  47     *-b-* |-a-|       b        error       b
 *
 *  48     *-a-*             a        [b2,a2]     b
 *  48      *-b-|
 *
 *  49     *-a-*             a        [a1,b1]     b
 *  49      |-b-*
 *
 *  50     *-b-*             b        error       a
 *  50      *-a-|
 *
 *  51     *-b-*             b        error       a
 *  51      |-a-*
 *
 *  52     *-a-*             a        error       a
 *  52     *-b-*
 *
 * Attention: The difference operation has to be reconsidered in future
 * concerning a discrete interpretation of the intervals.
 *
 * The closure operation defines an interval which is the smallest
 * interval containing the two operands.
 * The method intersectsWith() returns false in the error cases of the
 * intersection operation and true otherwise.
 *
 * * @version $Revision: 1.15 $
 *
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasMInterval
{
    static final String rcsid = "@(#)Package rasj, class RasMInterval: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasMInterval.java,v 1.15 2003/12/10 21:04:23 rasdev Exp $";

    /** array for storing the intervals */
    protected RasSInterval[] intervals;

    /** dimensionality of the domain */
    protected int dimensionality;

    /** number of components initialized already */
    protected int streamInitCnt;

    /**
     * Constructor getting dimensionality for stream initializing.
     * @param dim the dimensionality of this MInterval
     **/
    public RasMInterval(int dim)
    {
	dimensionality = dim;
	streamInitCnt = 0;
	intervals = new RasSInterval[dimensionality];

	for(int i=0; i<dim; i++)
	    intervals[i] = new RasSInterval();
    }

    /**
     * Constructor taking a string representation (for example "[1:255, 1:200]").
     * @param mIntStr a string specifying the MInterval
     **/
    public RasMInterval(String mIntStr) throws RasResultIsNoIntervalException
    {
	dimensionality = 1;
	streamInitCnt = 0;

	if(mIntStr.trim().charAt(0) != '[')
	    {
		// error
		dimensionality = 0;
		return;
	    }

	/** for parsing the string */

	StringTokenizer strTok = new StringTokenizer(mIntStr.trim(), "[:,]");
	String strCurTok = null;

	/** calculate dimensionality */
	dimensionality = strTok.countTokens()/2;

	intervals = new RasSInterval[ dimensionality ];


	for(int i=0; i<dimensionality; i++)
	    {
                RasSInterval sint = new RasSInterval();

		strCurTok = strTok.nextToken();

		if(strCurTok.equals("*"))
		    {
			sint.setLow('*');
		    }
		else
		    {
			sint.setLow(Long.parseLong(strCurTok.trim()));
		    }


		strCurTok = strTok.nextToken();

		if(strCurTok.equals("*"))
		    {
			sint.setHigh('*');
		    }
		else
		    {
			sint.setHigh(Long.parseLong(strCurTok.trim()));
		    }

		intervals[i] = sint;
	    }
    }

    /**
     * Method for stream initialization with intervals.
     * @param newInterval the interval that has to be streamed
     **/
    public void stream(final RasSInterval newInterval) throws RasStreamInputOverflowException
    {
	if(streamInitCnt >= dimensionality)
	    throw new RasStreamInputOverflowException();

	intervals[streamInitCnt++] = newInterval;

    }

    /**
     * Method for stream initialization with point intervals.
     * @param p the point interval that has to be streamed
     **/
    public void stream(long p) throws RasStreamInputOverflowException, RasResultIsNoIntervalException
    {
	if(streamInitCnt >= dimensionality)
	    throw new RasStreamInputOverflowException();

	intervals[streamInitCnt++] =  new RasSInterval(p, p);

    }

    /**
     * Default constructor.
     **/
    public RasMInterval()
    {
	dimensionality = 0;
	streamInitCnt = 0;

	intervals = null;
    }

    /**
     * Copy constructor.
     * @param mInterval the MInterval that is to be copied
     **/
    public RasMInterval(final RasMInterval mInterval)
	throws RasStreamInputOverflowException, RasIndexOutOfBoundsException
    {
	dimensionality = mInterval.dimension();
	streamInitCnt  = mInterval.streamInitCnt;
	intervals      = new RasSInterval[ dimensionality ];

	for(int i=0; i<dimensionality; i++)
	    intervals[i] = new RasSInterval(mInterval.item(i));
    }

    /**
     * Determines if the self mInterval intersects with the delivered one.
     * @param mInterval the MInterval to be intersected
     **/
    public boolean intersectsWith(final RasMInterval mInterval)
	throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
	boolean result = true;

	if(dimensionality != mInterval.dimension())
	    throw new RasDimensionMismatchException( dimensionality, mInterval.dimension());

	/** none of the interval pairs are allowed to be disjoint */
        for(int i=0; i<dimensionality; i++)
        {
            if(0 == intervals[i].intersectsWith(mInterval.item(i)) || -1 == intervals[i].intersectsWith(mInterval.item(i)))
            {
              result = false;
              break;
            }
        }
	return result;
    }

    /**
     * Read access the i-th interval.
     * @param i the dimension to be read
     **/
    public RasSInterval item(int i) throws RasIndexOutOfBoundsException
    {
	if(i < 0 || i >= dimensionality)
	    throw new RasIndexOutOfBoundsException(0, dimensionality-1, i);

	return intervals[i];
    }

    /**
     * Write access to the i-th interval.
     * @param i the dimension that is to be accessed
     * @param value the interval that is to be assigned to the specified dimension
     **/
    public void setItem(int i, RasSInterval value) throws RasIndexOutOfBoundsException
    {
	if(i < 0 || i >= dimensionality)
	    throw new RasIndexOutOfBoundsException(0, dimensionality-1, i);
	intervals[i] = value;
    }

    /*
     * set MInterval
    public final RasMInterval setTo(final RasMInterval mInterval)
	throws RasStreamInputOverflowException, RasIndexOutOfBoundsException
    {
	if(this != mInterval)
	    {
		if((intervals != null) && dimensionality != mInterval.dimension())
		    {
			intervals = null;
		    }

		dimensionality = mInterval.dimension();
		streamInitCnt  = mInterval.streamInitCnt;

		if(intervals == null)
		    intervals = new RasSInterval[ dimensionality ];

		for(int i=0; i<dimensionality; i++)
		    intervals[i] = new RasSInterval(mInterval.item(i));
	    }

	return this;
    }
    */

    /**
     * Method for testing equality of two MIntervals. Two domains are equal
     * if they have the same number of dimensions and each dimension has the
     * same lower and upper bounds.
     * @param mint the MInterval that is compared to this MInterval
     * @return true if the two MIntervals are equal
     **/
    public boolean equals(final RasMInterval mint)
    {
	boolean returnValue = false;

	try {
	    if(dimensionality == mint.dimensionality)
		{
		    returnValue = true;

		    for(int i=0; i<dimensionality && returnValue ; i++)
			returnValue &= (intervals[i] == mint.item(i));
		}

	    return returnValue;
	}

	catch(RasIndexOutOfBoundsException e) {
	    return false;
	}
    }

    /**
     * Method for testing inequality. This is the negation of the equals method.
     * @param mint the MInterval that is compared to this MInterval
     * @return true if the two MIntervals are not equal
     **/
    public boolean notEquals(final RasMInterval mint)
    {
	return !equals(mint);
    }

    /**
     * Method for testing if this interval covers the given point.
     * @param pnt the point to be tested
     * @return
     * <TABLE BORDER=0 CELLPADDING=3>
     * <TR><TD ALIGN=RIGHT><B>-1</B></TD><TD> if the point has not the same dimensionality</TD></TR>
     * <TR><TD ALIGN=RIGHT><B>1</B></TD><TD> if the point is covered by this MInterval</TD></TR>
     * <TR><TD ALIGN=RIGHT><B>0</B></TD><TD> if the point is not covered</TD></TR>
     * </TABLE>
     **/
    public final int covers(RasPoint pnt)
    {
	if (dimensionality != pnt.dimension())
	    return -1;
	try {
	    for (int i = 0; i < pnt.dimension(); i++)
		{
		    if ((intervals[i].isLowFixed() && pnt.item(i) < intervals[i].low()) ||
			(intervals[i].isHighFixed() && pnt.item(i) > intervals[i].high()))
			return 0;
		}
	    return 1;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // This cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","covers(RasPoint pnt)",e.getMessage());
	}
    }

    /**
     * Method for testing if this interval covers the given interval.
     * @param inter2 the MInterval to be tested
     * @return
     * <TABLE BORDER=0i CELLPADDING=3>
     * <TR><TD ALIGN=RIGHT><B>-1</B></TD><TD> if the point has not the same dimensionality</TD></TR>
     * <TR><TD ALIGN=RIGHT><B>1</B></TD><TD> if the point is covered by this MInterval</TD></TR>
     * <TR><TD ALIGN=RIGHT><B>0</B></TD><TD> if the point is not covered</TD></TR>
     **/
    public int covers(RasMInterval inter2)
    {
	if(dimensionality != inter2.dimension())
	    return -1;

	try {
	    for (int i = 0; i < dimensionality ; i++)
		{
		    if (
			( intervals[i].isLowFixed() &&
			  (!(inter2.item(i).isLowFixed()) ||
			   intervals[i].low() > inter2.item(i).low()  // both lows fixed here
			   )
			  )  ||     // end of lows check
			( intervals[i].isHighFixed() &&
			  (!(inter2.item(i).isHighFixed()) ||
			   intervals[i].high() < inter2.item(i).high()  // both highs fixed here
			   )
			  )
			)
			return 0;
		}
	    return 1;
	}
	catch( RasIndexOutOfBoundsException e) {
	    // this can not occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","covers(RasMInterval inter2)",e.getMessage());
	}
    }

    /**
     * Gets the dimensionality of this MInterval.
     * @return the dimensionality of this MInterval
     **/
    public int dimension()
    {
	return dimensionality;
    }

    /**
     * Gets the point with the lowest coordinates in every dimension.
     * @return the origin of this MInterval (the point with the lowest coordinates)
     **/
    public RasPoint getOrigin()
    {
	try {
	    int i;
	    RasPoint pt = new RasPoint(dimensionality);

	    for(i=0; i<dimensionality; i++)
		pt.stream(intervals[i].low());

	    return pt;
	}
	catch(RasStreamInputOverflowException e) {
	    // This cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","getOrigin()",e.getMessage());
	}
    }

    /**
     * Gets the point with the highest coordinates in every dimension.
     * @return the point with the highest coordinates in this MInterval
     **/
    public RasPoint getHigh()
    {
	try {
	    int i;
	    RasPoint pt = new RasPoint(dimensionality);

	    for(i=0; i<dimensionality; i++)
		pt.stream(intervals[i].high());

	    return pt;
	}
	catch(RasStreamInputOverflowException e) {
	    // This cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","getHigh()",e.getMessage());
	}
    }

    /**
     * Gets the size of this MInterval as a point, that means the point specifies the extent
     * of this MInterval ( i.e. high() - low() ) in each dimension.
     * @return the size of this MInterval
     **/
    public RasPoint getExtent()
    {
	try {
	    int i;
	    RasPoint pt = new RasPoint(dimensionality);

	    for(i=0; i<dimensionality; i++)
		pt.stream(intervals[i].high() - intervals[i].low() + 1);

	    return pt;
	}
	catch(RasStreamInputOverflowException e) {
	    // This cannot occur (theoretically)
	    System.err.println("Error in method RasMInterval.getExtent()");
	    return null;
	}
    }

    /**
     * This method checks if this MInterval is mergeable with another MInterval.
     * Two RasMIntervals are "mergeable" if they have the same low() and high()
     * values in all dimensions except in one where they differ by one point,
     * this is, a.low()==b.high()+1 or b.low()==a.high()+1. For instance, the
     * following two blocks are mergeable:
     *
     * <pre>
     * +-------------+---------------------------------------+
     * |      A      |                  B                    |
     * +-------------|---------------------------------------|
     *
     * and the following two are not:
     *
     * +-------------+-------------------------+
     * |             |            B            |
     * |      A      +-------------------------+
     * +-------------+
     *
     *  </pre>
     * @param b the MInterval to be checked
     * @return true if the two intervalls are mergeable
     **/
    public boolean isMergeable(final RasMInterval b)
    {
	final RasMInterval a = this;                    // An alias to this object

	// The blocks must have the same dimensionality to be mergeable
	if (a.dimensionality != b.dimensionality)
	    return false;

	// Count the number of adjacent frontiers
	int onesDifferences = 0;

	// Is Mergeable variable
	boolean isMerg = true;

	// For all dimensions
	try {
	    for (int i=0; i<dimensionality; i++)
		{
		    if (a.item(i).low() != b.item(i).low())                  // Diferente origins
			{
			    if ((a.item(i).low() == b.item(i).high()+1) ||   // If borders are adjacent
				(b.item(i).low() == a.item(i).high()+1))
				{
				    ++onesDifferences;                       // Update counter
				}
			    else
				{
				    isMerg = false;                          // Else non-mergeable blocks
				    break;
				}
			}
		    else                                                     // Same origins
			{
			    if (a.item(i).high() != b.item(i).high())        // Check ending
				{
				    isMerg = false;                          // Not the same, can't be
				    break;                                   //   mergeable
				}
			}
		}

	    if (isMerg && (onesDifferences!=1))            // Only one adjacent borded
		isMerg = false;                                   //   allowed

	    return isMerg;                                  // Return result
	}

	catch(RasIndexOutOfBoundsException e) {
	    // This cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","isMergeable()",e.getMessage());
	}

    }

    /**
     * This methods translates the current MInterval by a point.
     * It adds the coordinates of the given point to the lower bounds of the
     * current MInterval. This operation is only legal if all lower bounds are
     * fixed and the point has the same dimension as the MInterval!<P>
     * Note that this method modifies the current MInterval. If you want to
     * get a new MInterval, use the {@link #createTranslation(RasPoint) createTranslation} method
     * instead.<P>
     * @param t the point to be added
     * @return the current MInterval
     **/
    public RasMInterval translate(RasPoint t)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	int i;

	if(dimensionality != t.dimension())
	    throw new RasDimensionMismatchException(dimensionality, t.dimension());

	try {

	    for(i=0; i<dimensionality; i++)
		{
		    intervals[i].setInterval(intervals[i].low() + t.item(i),
					     intervals[i].high() + t.item(i));
		}
        }
	catch(RasIndexOutOfBoundsException e) {
	    // this cannor occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","translate()",e.getMessage());
	}

	return this;
    }


    /**
     * This method returns a new MInterval resulting from a translation of the current
     * MInterval by the given point.
     * It creates a copy of the current MInterval and adds the coordinates of the given
     * point to its lower bounds.<P>
     * This operation is only legal if all lower bounds are
     * fixed and the point has the same dimension as the MInterval!<P>
     * @param t the point to be added
     * @return the new MInterval
     **/
    public RasMInterval createTranslation(RasPoint t)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != t.dimension())
	    throw new RasDimensionMismatchException(dimensionality, t.dimension());

	RasMInterval result = new RasMInterval(dimensionality);

	try {
	    for(int i=0; i<dimensionality; i++)
		result.intervals[i].setInterval(intervals[i].low() + t.item(i),
						intervals[i].high() + t.item(i));
        }
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createTranslation()",e.getMessage());
	}

	return result;
    }

    /**
     * Calculates the union of two MIntervals. This is only possible for two MIntervals
     * having the same dimensionality.
     * <P>Note that this operation modifies the current object. If you want to get a new MInterval,
     * use {@link #createUnion(RasMInterval) createUnion} instead.</P>
     * @param mint1 the first MInterval
     * @param mint2 the second MInterval
     * @return the current MInterval (representing the union of mint1 and mint2)
     **/
    public RasMInterval unionOf(final RasMInterval mint1, final RasMInterval mint2)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(mint1.dimension() != mint2.dimension())
	    throw new RasDimensionMismatchException( mint1.dimension(), mint2.dimension());

	// cleanup + initializing of this

	dimensionality = mint1.dimension();
	streamInitCnt  = dimensionality;
	intervals      = new RasSInterval[ dimensionality ];

	try {

	    for(int i=0; i<dimensionality; i++)
            {
              intervals[i] = new RasSInterval();
              intervals[i].unionOf(mint1.item(i), mint2.item(i));
            }
	    return this;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","unionOf()",e.getMessage());
	}

    }

    /**
     * Calculates the union of the current MIntervall with another one.
     * <P>Note that this operation modifies the current object. If you want to get a new MInterval,
     * use {@link #createUnion(RasMInterval) createUnion} instead.</P>
     * @param mint the MInterval to be used for the union
     * @return the current MInterval (after the union with mint)
     **/
    public RasMInterval unionWith(final RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != mint.dimension())
	    throw new RasDimensionMismatchException( dimensionality, mint.dimension());

	try {

	    for(int i=0; i<dimensionality; i++)
		intervals[i].unionWith(mint.item(i));
	    return this;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","unionWith()",e.getMessage());
	}

    }

    /**
     * @deprecated This methdod provides just another name for the
     * unionWith method and might not be supported in future versions. Please
     * use {@link #unionWith(RasMInterval) unionWith} instead.
     * @param mint the MInterval to be added
     * @return the current MInterval after adding mint
     **/
    public RasMInterval addToSelf(final RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	return unionWith(mint);
    }


    /**
     * Returns a new MInterval calculated from a union of the current MInterval
     * and the given one.
     * @param mint the MInterval to be unioned with this MInterval
     * @return the union of this MInterval and mint
     **/
    public RasMInterval createUnion(final RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != mint.dimension())
	    throw new RasDimensionMismatchException( dimensionality, mint.dimension());

	RasMInterval result = new RasMInterval(dimensionality);
	try {
	    for(int i=0; i<dimensionality; i++)
		result.stream(intervals[i].createUnion(mint.item(i)));
	    return result;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createUnion()",e.getMessage());
	}
	catch(RasStreamInputOverflowException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createUnion()",e.getMessage());
	}

    }


    /**
     * @deprecated This methdod provides just another name for the
     * createUnion method and might not be supported in future versions. Please
     * use {@link #createUnion(RasMInterval) createUnion} instead.
     * @param mint the MInterval to be added
     * @return the current MInterval after adding mint
     **/
    public RasMInterval add(final RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	return createUnion(mint);
    }


    /**
     * Calculates the difference of two MIntervals.
     * <P>Note that this operation modifies the current object. If you want to get a new MInterval,
     * use {@link #createDifference(RasMInterval) createDifference} instead.</P>
     * @param mint1 the first MInterval
     * @param mint2 the second MInterval
     * @return the current MInterval (representing the difference of mint1 and mint2)
     **/
    public RasMInterval differenceOf(final RasMInterval mint1, final RasMInterval mint2)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(mint1.dimension() != mint2.dimension())
	    throw new RasDimensionMismatchException( mint1.dimension(), mint2.dimension());

	// cleanup + initializing of this

	dimensionality = mint1.dimension();
	streamInitCnt  = dimensionality;
	intervals      = new RasSInterval[ dimensionality ];

	try {

	    for(int i=0; i<dimensionality; i++)
            {
              intervals[i] = new RasSInterval();
              intervals[i].differenceOf(mint1.item(i), mint2.item(i));
            }
	    return this;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","differenceOf()",e.getMessage());
	}
    }

    /**
     * Calculates the difference of the current MInterval and the given one.
     * <P>Note that this operation modifies the current object. If you want to get a new MInterval,
     * use {@link #createDifference(RasMInterval) createDifference} instead.</P>
     * @param mint the MInterval used for building the difference
     * @return the current MInterval (representing the difference of this MInterval and mint)
     **/
    public RasMInterval differenceWith(final RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != mint.dimension())
	    throw new RasDimensionMismatchException(dimensionality, mint.dimension());

	try {
	    for(int i=0; i<dimensionality; i++)
            {
              intervals[i].differenceWith(mint.item(i));
            }
	    return this;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","differenceWith()",e.getMessage());
	}
    }

    /**
     * @deprecated This methdod provides just another name for the
     * differenceWith method and might not be supported in future versions. Please
     * use {@link #differenceWith(RasMInterval) differenceWith} instead.
     * @param mint1 the MInterval used for building the difference
     * @return the difference of this MInterval and mint2
     **/
    public RasMInterval diffFromSelf(RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	return differenceWith(mint);
    }

    /**
     * Returns a new MInterval calculated from a difference of the current MInterval
     * and the given one.
     * @param mint the MInterval used for calculating the difference with the current MInterval
     * @return the difference of this MInterval and mint
     **/
    public RasMInterval  createDifference(RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != mint.dimension())
	    throw new RasDimensionMismatchException(dimensionality, mint.dimension());

	RasMInterval result = new RasMInterval(dimensionality);

	try {
	    for(int i=0; i<dimensionality; i++)
		result.stream(intervals[i].createDifference(mint.item(i)));
	    return result;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createDifference()",e.getMessage());
	}
	catch(RasStreamInputOverflowException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createDifference()",e.getMessage());
	}

    }

    /**
     * @deprecated This methdod provides just another name for the
     * createDifference method and might not be supported in future versions. Please
     * use {@link #createDifference(RasMInterval) createDifference} instead.
     * @param mint the MInterval used for calculating the difference with the current MInterval
     * @return the difference of this MInterval and mint
     **/
    public RasMInterval diff(RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	return createDifference(mint);
    }


    // Methods/Operators for the intersection operation:
    /**
     * This method calculates the intersection of two MIntervals. This is only possible if the
     * two MIntervals have the same dimensionality.
     * <P>Note that this operation modifies the current object. If you want to get a new MInterval,
     * use {@link #createIntersection(RasMInterval) createIntersection} instead.</P>
     * @param mint1 the first MInterval
     * @param mint2 the second MInterval
     * @return the current MInterval (representing the intersection of mint1 and mint2)
     **/
    public RasMInterval intersectionOf(RasMInterval mint1, RasMInterval mint2)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(mint1.dimension() != mint2.dimension())
	    throw new RasDimensionMismatchException( mint1.dimension(), mint2.dimension());

	// cleanup + initializing of this

	dimensionality = mint1.dimension();
	streamInitCnt  = dimensionality;
	intervals      = new RasSInterval[ dimensionality ];

	try {
	    for(int i=0; i<dimensionality; i++)
            {
              intervals[i] = new RasSInterval();
              intervals[i].intersectionOf(mint1.item(i), mint2.item(i));
            }
	    return this;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","intersectionOf()",e.getMessage());
	}

    }

    /**
     * Calculates the intersection of the current MInterval and the given one.
     * <P>Note that this operation modifies the current object. If you want to get a new MInterval,
     * use {@link #createIntersection(RasMInterval) createIntersection} instead.</P>
     * @param mint the MInterval used for building the intersection
     * @return the current MInterval (representing the intersection of this MInterval and mint)
     **/
    public RasMInterval intersectionWith(RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != mint.dimension())
	    throw new RasDimensionMismatchException(dimensionality, mint.dimension());

	try {
	    for(int i=0; i<dimensionality; i++)
		intervals[i].intersectionWith(mint.item(i));
	    return this;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","intersectionWith()",e.getMessage());
	}
    }

    /**
     * @deprecated This methdod provides just another name for the
     * intersectionWith method and might not be supported in future versions. Please
     * use {@link #intersectionWith(RasMInterval) intersectionWith} instead.
     * @param mint the MInterval used for building the intersection
     * @return the intersection of this MInterval and mint
     **/
    public RasMInterval multWithSelf(RasMInterval mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	return intersectionWith(mint);
    }


    /**
     * Returns a new MInterval calculated from the intersection of the current MInterval
     * and the given one.
     * @param mint the MInterval used for calculating the intersection with the current MInterval
     * @return the intersection of this MInterval and mint
     **/
    public RasMInterval createIntersection (RasMInterval  mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != mint.dimension())
	    throw new RasDimensionMismatchException( dimensionality, mint.dimension());

	RasMInterval result = new RasMInterval(dimensionality);
	try {
	    for(int i=0; i<dimensionality; i++)
		result.stream(intervals[i].createIntersection(mint.item(i)));
	    return result;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createIntersection()",e.getMessage());
	}
	catch(RasStreamInputOverflowException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createIntersection()",e.getMessage());
	}
    }

    /**
     * @deprecated This methdod provides just another name for the
     * createIntersection method and might not be supported in future versions. Please
     * use {@link #createIntersection(RasMInterval) createIntersection} instead.
     * @param mint the MInterval used for calculating the intersection with the current MInterval
     * @return the intersection of this MInterval and mint
     **/
    public RasMInterval mult(final RasMInterval  mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	return createIntersection(mint);
    }


    // Methods/Operators for the closure operation:
    /**
     * Calculates the closure of two MIntervals.
     * <P>Note that this operation modifies the current object. If you want to get a new MInterval,
     * use {@link #createClosure(RasMInterval) createClosure} instead.</P>
     * @param mint1 the first MInterval
     * @param mint2 the second MInterval
     * @return the current MInterval (representing the closure of mint1 and mint2)
     **/
    public RasMInterval closureOf(RasMInterval mint1, RasMInterval mint2)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {

	if(mint1.dimension() != mint2.dimension())
	    throw new RasDimensionMismatchException( mint1.dimension(), mint2.dimension());

	// cleanup + initializing of this

	dimensionality = mint1.dimension();
	streamInitCnt  = dimensionality;
        intervals = new RasSInterval[ dimensionality ];

	try
        {
	    for(int i=0; i<dimensionality; i++)
            {
              intervals[i] = new RasSInterval();
              intervals[i].closureOf(mint1.item(i), mint2.item(i));
            }
	    return this;
	}
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","closureOf()",e.getMessage());
	}
    }

    /**
     * Calculates the closure of the current MInterval and the given one.
     * <P>Note that this operation modifies the current object. If you want to get a new MInterval,
     * use {@link #createClosure(RasMInterval) createClosure} instead.</P>
     * @param mint the MInterval used for building the closure
     * @return the current MInterval (representing the closure of this MInterval and mint)
     **/
    public RasMInterval closureWith(final RasMInterval  mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != mint.dimension())
	    throw new RasDimensionMismatchException( dimensionality, mint.dimension());

	try
	    {
		for(int i=0; i<dimensionality; i++)
		    intervals[i].closureWith(mint.item(i));
		return this;
	    }
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","closureWith()",e.getMessage());
	}
    }

    /**
     * Returns a new MInterval calculated from the closure of the current MInterval
     * and the given one.
     * @param mint the MInterval used for calculating the closure with the current MInterval
     * @return the closure of this MInterval and mint
     **/
    public RasMInterval  createClosure(RasMInterval  mint)
	throws RasDimensionMismatchException, RasResultIsNoIntervalException
    {
	if(dimensionality != mint.dimension())
	    throw new RasDimensionMismatchException( dimensionality, mint.dimension());

	RasMInterval result = new RasMInterval(dimensionality);

	try
	    {
		for(int i=0; i<dimensionality; i++)
		    result.stream(intervals[i].createClosure(mint.item(i)));
		return result;
	    }
	catch(RasIndexOutOfBoundsException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createClosure()",e.getMessage());
	}
	catch(RasStreamInputOverflowException e) {
	    // this cannot occur (theoretically)
	    throw new RasClientInternalException("RasMInterval","createClosure()",e.getMessage());
	}

    }

    // Methods for internal use only:

    /**
     * Calculates the number of cells.
     **/
    long cellCount()
    {
	long cellCount=1;

	for(int i=0; i<dimensionality; i++)
	    cellCount *= intervals[i].high() - intervals[i].low() + 1;

	return cellCount;
    }

    /**
     * Calculates the offset in cells for one dimensional (linear) access to
     * the given point (dimension ordering is high first).
     **/
    long cellOffset( final RasPoint point)
	throws RasIndexOutOfBoundsException
    {
	int i = 0;
	long offset = 0;

	// calculate offset
	for(i = 0; i < dimensionality - 1; i++)
	    {
		if(point.item(i) < intervals[i].low() || point.item(i) > intervals[i].high())
		    throw new RasIndexOutOfBoundsException(intervals[i].low(), intervals[i].high(), point.item(i) );

		offset = (offset + point.item(i) - intervals[i].low()) * (intervals[i+1].high() - intervals[i+1].low() + 1);
	    }

	// now i = dimensionality - 1
	if(point.item(i) < intervals[i].low() || point.item(i) > intervals[i].high())
	    throw new RasIndexOutOfBoundsException(intervals[i].low(), intervals[i].high(), point.item(i) );

	offset += point.item(i) - intervals[i].low();

	return offset;
    }

    /**
     * This method calculates the spatial domain
     * coordinates as a point from the offset
     * specified. Lower dimensions are higher
     * valued which means that the highest dimension
     * is stored in a sequence.
     **/
    RasPoint cellPoint(long offset) throws RasResultIsNoCellException
    {
	int i;
	long factor=1;
	RasPoint pt = new RasPoint(dimensionality);

	try {

	    if(offset >= cellCount())
		throw new RasResultIsNoCellException();

	    for(i=0; i<dimensionality; i++)
		factor *= intervals[i].high() - intervals[i].low() + 1;

	    for(i=0; i<dimensionality; i++)
		{
		    factor /= intervals[i].high() - intervals[i].low() + 1;
		    pt.stream(intervals[i].low() + (offset - (offset%factor))/factor);
		    offset %= factor;
		}

	    return pt;
	}
	catch( RasStreamInputOverflowException e ) {
	    // this cannot occur (theoretically)
	    System.err.println("Error in method RasMInterval.cellPoint().");
	    return null;
	}
    }

    // delete the specified dimension
    void deleteDimension(int dim) throws RasException
    {
	if(dim < 0 || dim >= dimensionality)
	    throw new RasIndexOutOfBoundsException(0, dimensionality-1, dim);

	dimensionality -= 1;
	streamInitCnt = dimensionality;
	RasSInterval[] newIntervals = new RasSInterval[ dimensionality ];

	for(int i=0, j=0; i<dimensionality; i++, j++)
	    {
		if(i==dim) j++;
		newIntervals[i] = intervals[j];
	    }

	intervals = newIntervals;
    }

    // calculate the size of the storage space occupied
    long getStorageSize()
    {
	long sz = 26;  //18 + 2 * 4

	if (dimensionality > 0)
	    sz += dimensionality * 18;

	return sz;
    }

    /** gives back the string representation */
    public String toString()
    {
	String retString = "";
	if(dimensionality > 0)
	    {
		for(int i=0; i<dimensionality-1; i++)
		    {
			retString = retString + intervals[i].toString() + ",";
		    }
		retString = retString + intervals[dimensionality-1].toString();
	    }

	return "[" + retString  + "]";
    }
}
