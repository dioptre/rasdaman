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
 * The class represents an interval with lower and upper bound.
 * Operations on the interval are defined according to the
 * ODMG-93 standard.
 * The operations union, difference, and intersection are
 * defined according to the following table:
 *
 *    | ...  fixed bound
 *    -* ...  open bound
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
 *  19     *--a--|           a        error       b
 *  19      |-b-|
 *
 *  20     *--a--|           a        [a1,b1]     b
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
 *  47     *-b-* |-a-|       b        error       a
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
 * The method intersectsWith() returns 0 in the error cases of the
 * intersection operation and 1 otherwise.
 *
 * @version $Revision: 1.7 $
 *
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */



public class RasSInterval
{
    static final String rcsid = "@(#)Package rasj, class RasSInterval: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasSInterval.java,v 1.7 2003/12/10 21:04:23 rasdev Exp $";

  /**Attributes storing the bounds:*/
    long lowerBound;
    long upperBound;

  /**Attributes specifying wheter the lower/upper bound is fixed or not:*/
    boolean lowFixed;
    boolean highFixed;

  /**
   * The default constructor creates an interval with open bounds.
   **/
    public RasSInterval()
    {
	lowerBound = 0;
	upperBound = 0;
	lowFixed = false;
	highFixed = false;
    }

    /**
   * Copy constructor.
   * @param sint SInterval to be copied
   **/
    public RasSInterval(RasSInterval sint)
    {
	lowerBound = sint.lowerBound;
	upperBound = sint.upperBound;
	lowFixed = sint.lowFixed;
	highFixed = sint.highFixed;
    }

    /**
   * Constructor taking a string representation of the SInterval (for example "*:200").
   * @param stringRep the string representation of the SInterval
   **/
    public RasSInterval(String stringRep) throws RasResultIsNoIntervalException
    {
	lowerBound = 0;
	upperBound = 0;
	lowFixed = false;
	highFixed = false;


	/**for parsing the string*/
	StringTokenizer strTok = new StringTokenizer(stringRep.trim(), ":");
	String strCurTok = strTok.nextToken();

	if(strCurTok.equals("*"))
	    {
		setLow('*');
	    }
	else
	    {
		setLow(Long.parseLong(strCurTok.trim()));
	    }


	strCurTok = strTok.nextToken();

	if(strCurTok.equals("*"))
	    {
		setHigh('*');
	    }
	else
	    {
		setHigh(Long.parseLong(strCurTok.trim()));
	    }
    }


    /**
   * Constructor for an SInterval with fixed bounds.
   * @param low the lower bound
   * @param high the upper bound
   **/
    public RasSInterval(long low, long high) throws RasResultIsNoIntervalException
    {
	lowerBound = low;
	upperBound = high;
	lowFixed = true;
	highFixed = true;

	if(low > high)
	    throw new RasResultIsNoIntervalException();
    }


    /**
   * Constructor for an SInterval with an open lower bound.
   * @param c arbitrary character, usually '*'. The value of this parameter is
   * meaningless, it's only needed in order to provide a constructor similar to
   * the one for fixed bounds.
   * @param high the upper bound
   **/
    public RasSInterval(char c, long high)
    {
	lowerBound = 0;
	upperBound = high;
	lowFixed = false;
	highFixed = true;
    }

    /**
   * Constructor for an SInterval with an open upper bound.
   * @param c arbitrary character, usually '*'. The value of this parameter is
   * meaningless, it's only needed in order to provide a constructor similar to
   * the one for fixed bounds.
   * @param low the lower bound
   **/
    public RasSInterval(long low, char c)
    {
	lowerBound = low;
	upperBound = 0;
	lowFixed = true;
	highFixed = false;
    }

    /**
   * Constructor for an SInterval with an open bounds. For both parameters,
   * arbitrary characters (usually '*') can be specified. The values of these parameters are
   * meaningless, they are only needed in order to provide a constructor similar to
   * the one for fixed bounds.
   * @param c1 arbitrary character
   * @param c2 arbitrary character
   **/
    public RasSInterval(char c1, char c2)
    {
	lowerBound = 0;
	upperBound = 0;
	lowFixed = false;
	highFixed = false;
    }

    /**
     * Method for testing equality of two SIntervals. Two SIntervals are equal
     * if they have the same lower and upper bounds.
     * @param interval the SInterval that is compared to this SInterval
     * @return true if the two SIntervals are equal
     **/
    public boolean equals(final RasSInterval interval)
    {
	boolean returnValue = true;

	if(lowFixed)
	    returnValue &= interval.lowFixed && lowerBound == interval.lowerBound;
	else
	    returnValue &= !interval.lowFixed;

	if(highFixed)
	    returnValue &= interval.highFixed && upperBound == interval.upperBound;
	else
	    returnValue &= !interval.highFixed;

	return returnValue;
    }

    /**
     * Method for testing inequality of two SIntervals. Two SIntervals are inequal
     * if they have different lower or upper bounds.
     * @param interval the SInterval that is compared to this SInterval
     * @return true if the two SIntervals are not equal
     **/
    public boolean notEquals(final RasSInterval interval)
    {
	return !equals(interval);
    }


    // Read/Write methods:
    /**
     * Gets the lower bound of the interval.
     * @return the lower bound
     **/
    public long low()
    {
	return lowerBound;
    }

    /**
     * Gets the upper bound of the interval.
     * @return the upper bound
     **/
    public long high()
    {
	return upperBound;
    }

    /**
     * Checks if the lower bound is fix.
     * @return true if the lower bound is fix, false otherwise
     **/
    public boolean isLowFixed()
    {
	return lowFixed;
    }

    /**
     * Checks if the upper bound is fix.
     * @return true if the lower bound is fix, false otherwise
     **/
    public boolean isHighFixed()
    {
	return highFixed;
    }

    /**
     * Sets the lower bound to a fixed value.
     * @param low the new lower bound
     **/
    public void setLow(long low) throws RasResultIsNoIntervalException
    {
	if(highFixed && low > upperBound)
	    throw new RasResultIsNoIntervalException();

	lowerBound = low;
	lowFixed = true;
    }

    /**
     * Sets the upper bound to a fixed value.
     * @param high the new upper bound
     **/
    public void setHigh(long high) throws RasResultIsNoIntervalException
    {
	if(lowFixed && high < lowerBound)
	    throw new RasResultIsNoIntervalException();

	upperBound = high;
	highFixed = true;
    }

    /**
     * Sets the lower bound open.
     * @param c arbitrary character (usually '*')
     **/
    public void setLow(char c)
    {
	lowerBound  = 0;
	lowFixed = false;
    }

    /**
     * Sets the upper bound open.
     * @param c arbitrary character (usually '*')
     **/
    public void setHigh(char c)
    {
	upperBound = 0;
	highFixed = false;
    }

    /**
     * Sets both bounds to a fixed value.
     * @param low the new lower bound
     * @param high the new upper bound
     **/
    public void setInterval(long low, long high) throws RasResultIsNoIntervalException
    {
	if(low > high)
	    throw new RasResultIsNoIntervalException();

	lowerBound = low;
	upperBound = high;
	lowFixed  = true;
	highFixed = true;
    }

    /**
     * Sets the lower bound open and the upper bound to a fixed value.
     * @param c arbitrary character (usually '*')
     * @param high the new upper bound
     **/
    public void setInterval(char c, long high)
    {
	lowerBound = 0;
	upperBound = high;
	lowFixed  = false;
	highFixed = true;
    }

    /**
     * Sets the upper bound open and the lower bound to a fixed value.
     * @param c arbitrary character (usually '*')
     * @param low the new lower bound
     **/
    public void setInterval(long low, char c)
    {
	lowerBound = low;
	upperBound = 0;
	lowFixed  = true;
	highFixed = false;
    }

    /**
     * Sets both bounds open.
     * @param c1 arbitrary character (usually '*')
     * @param c2 arbitrary character (usually '*')
     **/
    public void setInterval(char c1, char c2)
    {
	lowerBound = 0;
	upperBound = 0;
	lowFixed  = false;
	highFixed = false;
    }

    /**
     * Determines if the current interval intersects with the given one.
     * @param interval the interval to be compared
     * @return
     * <TABLE BORDER=0 CELLPADDING=3>
     * <TR><TD ALIGN=RIGHT><B>-1</B></TD><TD> if the intervals do not intersect</TD></TR>
     * <TR><TD ALIGN=RIGHT VALIGN=TOP><B>&lt;classNo&gt;</B></TD><TD> if the intervals do intersect, where &lt;classNo&gt; is one of the classes specified in the description for class {@link RasSInterval RasSInterval}.</TD></TR>
     * </TABLE>
     **/
    public int intersectsWith(RasSInterval interval)
    {
	int classnr = classify(this, interval);

	if(classnr !=  1 && classnr !=  6 && classnr != 16 && classnr != 21 &&
	   classnr != 26 && classnr != 31 && classnr != 34 && classnr != 37)
	    {
		return classnr;
	    }
	else
	    {
		return  -1;
	    }
    }

    //Methods/Operators for the union operation:
    /**
     * Calculates the union of two SIntervals.
     * <P>Note that this operation modifies the current object. If you want to get a new SInterval,
     * use {@link #createUnion(RasSInterval) createUnion} instead.</P>
     * @param interval1 the first SInterval
     * @param interval2 the second SInterval
     * @return the current SInterval (representing the union of interval1 and interval2)
     **/
    public RasSInterval unionOf(RasSInterval interval1, RasSInterval interval2)
	throws RasResultIsNoIntervalException
    {
	copySInterval(calcUnion(interval1, interval2),this);
	return this;
    }

    /**
     * Calculates the union of the current SIntervall with another one.
     * <P>Note that this operation modifies the current object. If you want to get a new SInterval,
     * use {@link #createUnion(RasSInterval) createUnion} instead.</P>
     * @param interval the SInterval to be used for the union
     * @return the current SInterval (after the union with interval)
     **/
    public RasSInterval unionWith(RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	copySInterval(calcUnion(this, interval),this);
	return this;
    }

    /**
     * @deprecated This methdod provides just another name for the
     * unionWith method and might not be supported in future versions. Please
     * use {@link #unionWith(RasSInterval) unionWith} instead.
     * @param interval the SInterval to be unioned with this SInterval
     * @return the union of this SInterval and interval
     **/
    public RasSInterval addToSelf(RasSInterval interval) throws RasResultIsNoIntervalException
    {
	RasSInterval retInterval = new RasSInterval();
	retInterval = calcUnion(interval, this);
	return retInterval;
    }

    /**
     * Returns a new SInterval calculated from a union of the current SInterval
     * and the given one.
     * @param mint the SInterval to be unioned with this SInterval
     * @return the union of this SInterval and interval
     **/
    public RasSInterval createUnion(RasSInterval interval) throws RasResultIsNoIntervalException
    {
	RasSInterval result = calcUnion(interval, this);
	return result;
    }

    /**
     * @deprecated This methdod provides just another name for the
     * createUnion method and might not be supported in future versions. Please
     * use {@link #createUnion(RasSInterval) createUnion} instead.
     * @param interval the SInterval to be unioned with this SInterval
     * @return the union of this SInterval and interval
     **/
    public RasSInterval add(RasSInterval interval) throws RasResultIsNoIntervalException
    {
	RasSInterval result;
	result = calcUnion(interval, this);
	return result;
    }


    // Methods/Operators for the difference operation:

    /**
     * Calculates the difference of two SIntervals.
     * <P>Note that this operation modifies the current object. If you want to get a new SInterval,
     * use {@link #createDifference(RasSInterval) createDifference} instead.</P>
     * @param interval1 the first SInterval
     * @param interval2 the second SInterval
     * @return the current SInterval (representing the difference of interval1 and interval2)
     **/
    public RasSInterval differenceOf(RasSInterval interval1, RasSInterval interval2)
	throws RasResultIsNoIntervalException
    {
	copySInterval(calcDifference(interval1, interval2),this);
	return this;
    }


    /**
     * Calculates the difference of the current SInterval and the given one.
     * <P>Note that this operation modifies the current object. If you want to get a new SInterval,
     * use {@link #createDifference(RasSInterval) createDifference} instead.</P>
     * @param interval the SInterval used for building the difference
     * @return the current SInterval (representing the difference of this SInterval and interval)
     **/
    public RasSInterval differenceWith(RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
        copySInterval(calcDifference(interval, this), this);
        return this;
    }

    /**
     * @deprecated This methdod provides just another name for the
     * differenceWith method and might not be supported in future versions. Please
     * use {@link #differenceWith(RasSInterval) differenceWith} instead.
     * @param interval the SInterval used for building the difference
     * @return the current SInterval (representing the difference of this SInterval and interval)
     **/
    public RasSInterval diffFromSelf(RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	return differenceWith(interval);
    }

    /**
     * Returns a new SInterval calculated from a difference of the current SInterval
     * and the given one.
     * @param interval the SInterval used for calculating the difference with the current SInterval
     * @return the difference of this SInterval and interval
     **/
    public RasSInterval createDifference(RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	RasSInterval result = calcDifference(interval, this);
	return result;
    }

    /**
     * @deprecated This methdod provides just another name for the
     * createDifference method and might not be supported in future versions. Please
     * use {@link #createDifference(RasSInterval) createDifference} instead.
     * @param interval the SInterval used for calculating the difference with the current SInterval
     * @return the difference of this SInterval and interval
     **/
    public RasSInterval diff(RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	RasSInterval result = calcDifference(interval, this);
	return result;
    }


    // Methods/Operators for the intersection operation:
    /**
     * This method calculates the intersection of two SIntervals.
     * <P>Note that this operation modifies the current object. If you want to get a new SInterval,
     * use {@link #createIntersection(RasSInterval) createIntersection} instead.</P>
     * @param interval1 the first SInterval
     * @param interval2 the second SInterval
     * @return the current SInterval (representing the intersection of interval1 and interval2)
     **/
    public RasSInterval intersectionOf(final RasSInterval interval1, final RasSInterval interval2)
	throws RasResultIsNoIntervalException
    {
	copySInterval(calcIntersection(interval1, interval2),this);
	return this;
    }

    /**
     * Calculates the intersection of the current SInterval and the given one.
     * <P>Note that this operation modifies the current object. If you want to get a new SInterval,
     * use {@link #createIntersection(RasSInterval) createIntersection} instead.</P>
     * @param interval the SInterval used for building the intersection
     * @return the current SInterval (representing the intersection of this SInterval and interval)
     **/
    public RasSInterval intersectionWith(final RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	copySInterval(calcIntersection(interval, this),this);
	return this;
    }

    /**
     * @deprecated This methdod provides just another name for the
     * intersectionWith method and might not be supported in future versions. Please
     * use {@link #intersectionWith(RasSInterval) intersectionWith} instead.
     * @param interval the SInterval used for building the intersection
     * @return the intersection of this SInterval and interval
     **/
    public RasSInterval multWithSelf(final RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	return intersectionWith(interval);
    }

    /**
     * Returns a new SInterval calculated from the intersection of the current SInterval
     * and the given one.
     * @param interval the SInterval used for calculating the intersection with the current SInterval
     * @return the intersection of this SInterval and interval
     **/
    public RasSInterval createIntersection(final RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	RasSInterval result = calcIntersection(interval, this);
	return result;
    }

    /**
     * @deprecated This methdod provides just another name for the
     * createIntersection method and might not be supported in future versions. Please
     * use {@link #createIntersection(RasSInterval) createIntersection} instead.
     * @param interval the SInterval used for calculating the intersection with the current SInterval
     * @return the intersection of this SInterval and interval
     **/
    public RasSInterval mult(final RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	return createIntersection(interval);
    }


    // Methods/Operators for the closure operation:
    /**
     * Calculates the closure of two SIntervals.
     * <P>Note that this operation modifies the current object. If you want to get a new SInterval,
     * use {@link #createClosure(RasSInterval) createClosure} instead.</P>
     * @param interval1 the first SInterval
     * @param interval2 the second SInterval
     * @return the current SInterval (representing the closure of interval1 and interval2)
     **/
    public RasSInterval closureOf(final RasSInterval interval1, final RasSInterval interval2)
	throws RasResultIsNoIntervalException
    {
	copySInterval(calcClosure(interval1, interval2),this);
	return this;
    }

    /**
     * Calculates the closure of the current SInterval and the given one.
     * <P>Note that this operation modifies the current object. If you want to get a new SInterval,
     * use {@link #createClosure(RasSInterval) createClosure} instead.</P>
     * @param interval the SInterval used for building the closure
     * @return the current SInterval (representing the closure of this SInterval and interval)
     **/
    public RasSInterval closureWith(final RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	RasSInterval retInterval = new RasSInterval();
	retInterval = calcClosure(interval, this);
	return retInterval;
    }

    /**
     * Returns a new SInterval calculated from the closure of the current SInterval
     * and the given one.
     * @param interval the SInterval used for calculating the closure with the current SInterval
     * @return the closure of this SInterval and interval
     **/
    public RasSInterval createClosure(final RasSInterval interval)
	throws RasResultIsNoIntervalException
    {
	RasSInterval result;
	result = calcClosure(interval, this);
	return result;
    }


    // Methods for internal use only:
    // calculate the size of the storage space occupied
    private long getStorageSize()
    {
	return (18); // 2 * (8 + 1)
    }

    // copy the values of source to target
    private void copySInterval(RasSInterval source, RasSInterval target)
    {
	target.lowerBound = source.lowerBound;
	target.upperBound = source.upperBound;
	target.highFixed = source.highFixed;
	target.lowFixed = source.lowFixed;
    }

    // Calculation methods for the operations:
    private RasSInterval calcUnion(final RasSInterval a, final RasSInterval b)
	throws RasResultIsNoIntervalException
    {
	RasSInterval result = new RasSInterval();

	switch(classify(a, b))
	    {
	    case  2:
	    case  7:
	    case  9:
	    case 12:
	    case 22:
	    case 23:
	    case 27:
	    case 28:
	    case 35:
	    case 36:
		// result = [a1:b2]

		if(a.isLowFixed())
		    result.setLow(a.low());
		else
		    result.setLow('*');

		if(b.isHighFixed())
		    result.setHigh(b.high());
		else
		    result.setHigh('*');

		break;

	    case  4:
	    case  8:
	    case 10:
	    case 13:
	    case 17:
	    case 18:
	    case 32:
	    case 33:
	    case 38:
	    case 39:
		// result = [b1:a2]

		if(b.isLowFixed())
		    result.setLow(b.low());
		else
		    result.setLow('*');

		if(a.isHighFixed())
		    result.setHigh(a.high());
		else
		    result.setHigh('*');

		break;

	    case  3:
	    case 11:
	    case 14:
	    case 15:
	    case 19:
	    case 20:
	    case 41:
	    case 42:
	    case 43:
	    case 44:
	    case 46:
	    case 48:
	    case 49:
	    case 52:
		result = a;
		break;

	    case  5:
	    case 24:
	    case 25:
	    case 29:
	    case 30:
	    case 40:
	    case 45:
	    case 47:
	    case 50:
	    case 51:
		result = b;
		break;

	    default: // case in { 1, 6, 16, 21, 26, 31, 34, 37 }
		throw new RasResultIsNoIntervalException();
	    }

	return result;
    }


    //
    private RasSInterval calcDifference(final RasSInterval a, final RasSInterval b)
	throws RasResultIsNoIntervalException
    {
	RasSInterval result = new RasSInterval();

	switch(classify(a, b))
	    {
	    case  2:
	    case  9:
	    case 20:
	    case 23:
	    case 28:
	    case 36:
	    case 39:
	    case 43:
	    case 49:
		// result = [a1:b1]

		if(a.isLowFixed())
		    result.setLow(a.low());
		else
		    result.setLow('*');

		if(b.isLowFixed())
		    result.setHigh(b.low());
		else
		    result.setHigh('*');

		break;

	    case  1:
	    case  6:
	    case  7:
	    case  8:
	    case 16:
	    case 17:
	    case 21:
	    case 22:
	    case 26:
	    case 27:
	    case 31:
	    case 32:
	    case 34:
	    case 35:
	    case 37:
	    case 38:
		result = a;
		break;

	    case  4:
	    case 10:
	    case 15:
	    case 18:
	    case 33:
	    case 42:
	    case 48:
		// result = [b2:a2]

		if(b.isHighFixed())
		    result.setLow(b.high());
		else
		    result.setLow('*');

		if(a.isHighFixed())
		    result.setHigh(a.high());
		else
		    result.setHigh('*');

		break;

	    default: // case in { 3, 5, 11, 12, 13, 14, 19, 24, 25, 29, 30, 40, 41, 44, 45, 46, 47, 50, 51, 52 }
		throw new RasResultIsNoIntervalException();
	    }

	return result;
    }


    //
    private RasSInterval calcIntersection(final RasSInterval a, final RasSInterval b)
	throws RasResultIsNoIntervalException
    {
	RasSInterval result = new RasSInterval();

	switch(classify(a, b))
	    {
	    case  4:
	    case 18:
	    case 33:
	    case 39:
		// result = [a1:b2]

		if(a.isLowFixed())
		    result.setLow(a.low());
		else
		    result.setLow('*');

		if(b.isHighFixed())
		    result.setHigh(b.high());
		else
		    result.setHigh('*');

		break;

	    case  2:
	    case 23:
	    case 28:
	    case 36:
		// result = [b1:a2]

		if(b.isLowFixed())
		    result.setLow(b.low());
		else
		    result.setLow('*');

		if(a.isHighFixed())
		    result.setHigh(a.high());
		else
		    result.setHigh('*');

		break;

	    case  5:
	    case 11:
	    case 12:
	    case 13:
	    case 24:
	    case 25:
	    case 29:
	    case 30:
	    case 40:
	    case 41:
	    case 44:
	    case 45:
	    case 47:
	    case 50:
	    case 51:
	    case 52:
		result = a;
		break;

	    case  3:
	    case  9:
	    case 10:
	    case 14:
	    case 15:
	    case 19:
	    case 20:
	    case 42:
	    case 43:
	    case 46:
	    case 48:
	    case 49:
		result = b;
		break;

	    case  7:
	    case 22:
	    case 27:
	    case 35:
		// result = [a2:a2]

		if(a.isHighFixed())
		    result.setInterval(a.high(), a.high());
		else
		    result.setInterval('*', '*');

		break;

	    case  8:
	    case 17:
	    case 32:
	    case 38:
		// result = [b2:b2]

		if(b.isHighFixed())
		    result.setInterval(b.high(), b.high());
		else
		    result.setInterval('*', '*');

		break;

	    default: // case in { 1, 6, 16, 21, 26, 31, 34, 37 }
		throw new RasResultIsNoIntervalException();
	    }

	return result;
    }



    private RasSInterval calcClosure(final RasSInterval a, final RasSInterval b)
	throws RasResultIsNoIntervalException
    {
	RasSInterval closure = new RasSInterval();

	if(!a.isLowFixed() || !b.isLowFixed())
	    closure.setLow('*');
	else
	    closure.setLow(Math.min(a.low(), b.low()));

	if(!a.isHighFixed() || !b.isHighFixed())
	    closure.setHigh('*');
	else
	    closure.setHigh(Math.max(a.high(), b.high()));

	return closure;
    }


    // compute the class of the two operands


  /*************************************************************
   * Method name...: classify
   *
   * Arguments.....: Two intervals for the classification.
   * Return value..: The classification class number (1..52).
   * Description...: The method classifies the two intervals into
   *                 one of 13 classes according to their spatial
   *                 relationship. Based on the classification, the
   *                 result of the operations union, difference,
   *                 and intersection can be calculated as shown
   *                 in the table
   ************************************************************/


    private int classify(final RasSInterval a, final RasSInterval b)
    {
	int classification = 0;

	if(a.isLowFixed() && a.isHighFixed() && b.isLowFixed() && b.isHighFixed())
	    {
		// classification 1..13

		if(a.low() < b.low())
		    {
			if(a.high() < b.high())
			    {
				if(a.high() < b.low())
				    classification = 1;
				else
				    if(a.high() == b.low())
					classification = 7;
				    else
					classification = 2;
			    }
			else  if(a.high() == b.high())
			    classification = 9;
			else
			    classification = 3;
		    }
		else if(a.low() == b.low())
		    {
			if(a.high() < b.high())
			    classification = 12;
			else  if(a.high() == b.high())
			    classification = 11;
			else
			    classification = 10;
		    }
		else
		    if(a.high() < b.high())
			classification = 5;
		    else if(a.high() == b.high())
			classification = 13;
		    else
			{
			    if(a.low() < b.high())
				classification = 4;
			    else if(a.low() == b.high())
				classification = 8;
			    else
				classification = 6;
			}
	    }
	else if(a.isLowFixed() && !a.isHighFixed() && b.isLowFixed() && b.isHighFixed())
	    {
		// classification 14..18

		if(a.low() < b.low())
		    classification = 14;
		else if(a.low() == b.low())
		    classification = 15;
		else
		    {
			if(b.high() < a.low())
			    classification = 16;
			else if(b.high() == a.low())
			    classification = 17;
			else
			    classification = 18;
		    }
	    }
	else if(!a.isLowFixed() && a.isHighFixed() && b.isLowFixed() && b.isHighFixed())
	    {
		// classification 19..23

		if(a.high() > b.high())
		    classification = 19;
		else if(a.high() == b.high())
		    classification = 20;
		else
		    {
			if(a.high() < b.low())
			    classification = 21;
			else if(a.high() == b.low())
			    classification = 22;
			else
			    classification = 23;
		    }
	    }
	else if(a.isLowFixed() && a.isHighFixed() && b.isLowFixed() && !b.isHighFixed())
	    {
		// classification 24..28

		if(b.low() < a.low())
		    classification = 24;
		else if(b.low() == a.low())
		    classification = 25;
		else
		    {
			if(a.high() < b.low())
			    classification = 26;
			else if(a.high() == b.low())
			    classification = 27;
			else
			    classification = 28;
		    }
	    }
	else if(a.isLowFixed() && a.isHighFixed() && !b.isLowFixed() && b.isHighFixed())
	    {
		// classification 29..33

		if(b.high() > a.high())
		    classification = 29;
		else if(b.high() == a.high())
		    classification = 30;
		else
		    {
			if(b.high() < a.low())
			    classification = 31;
			else if(b.high() == a.low())
			    classification = 32;
			else
			    classification = 33;
		    }
	    }
	else if(!a.isLowFixed() && a.isHighFixed() && b.isLowFixed() && !b.isHighFixed())
	    {
		// classification 34..36

		if(a.high() < b.low())
		    classification = 34;
		else if(a.high() == b.low())
		    classification = 35;
		else
		    classification = 36;
	    }
	else if(a.isLowFixed() && !a.isHighFixed() && !b.isLowFixed() && b.isHighFixed())
	    {
		// classification 37..39

		if(b.high() < a.low())
		    classification = 37;
		else if(b.high() == a.low())
		    classification = 38;
		else
		    classification = 39;
	    }
	else if(!a.isLowFixed() && a.isHighFixed() && !b.isLowFixed() && b.isHighFixed())
	    {
		// classification 40..42

		if(a.high() < b.high())
		    classification = 40;
		else if(a.high() == b.high())
		    classification = 41;
		else
		    classification = 42;
	    }
	else if(a.isLowFixed() && !a.isHighFixed() && b.isLowFixed() && !b.isHighFixed())
	    {
		// classification 43..45

		if(a.low() < b.low())
		    classification = 43;
		else if(a.low() == b.low())
		    classification = 44;
		else
		    classification = 45;
	    }
	else  if(!a.isLowFixed() && !a.isHighFixed() &&  b.isLowFixed() &&  b.isHighFixed())
	    classification = 46;
	else  if( a.isLowFixed() &&  a.isHighFixed() && !b.isLowFixed() && !b.isHighFixed())
	    classification = 47;
	else  if(!a.isLowFixed() && !a.isHighFixed() && !b.isLowFixed() &&  b.isHighFixed())
	    classification = 48;
	else  if(!a.isLowFixed() && !a.isHighFixed() &&  b.isLowFixed() && !b.isHighFixed())
	    classification = 49;
	else  if(!a.isLowFixed() &&  a.isHighFixed() && !b.isLowFixed() && !b.isHighFixed())
	    classification = 50;
	else  if( a.isLowFixed() && !a.isHighFixed() && !b.isLowFixed() && !b.isHighFixed())
	    classification = 51;
	else //   !a.isLowFixed() && !a.isHighFixed() && !b.isLowFixed() && !b.isHighFixed()
	    classification = 52;

	return classification;
    }


    /** gives back the string representation */
    public String toString()
    {
	return (lowFixed ? Long.toString(lowerBound) : "*")
	    + ":"
	    + (highFixed ? Long.toString(upperBound) : "*");
    }

}

