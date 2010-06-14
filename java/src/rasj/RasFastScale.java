package rasj;

import rasj.clientcommhttp.*;
import rasj.odmg.*;
import rasj.global.*;
import org.odmg.*;

import java.io.*;
import java.net.*;
import java.util.*;
import java.sql.*;

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
 *  Abstract base class for fast scaling
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


abstract class RasFastBaseScale implements RasGlobalDefs
{

    OQLQuery query = null;
    String collectionName = null;
    RasMInterval fullDomain = null;
    double lastScaleUsed;

    /**
     * Default constructor.
     *
     * This constructor gets the name of the image collection and and an OQLQuery object as parameters.
     * <P><B>Important:</B>This class does not open its own connection to the database nor start or
     * commit any transactions. This has to be done by the application using FastScale.</P>
     * @param collection the name of the collection containing the image(s)
     * @param queryObject the OQLQuery object for connecting to the database
     **/
    public RasFastBaseScale( final String collection, final OQLQuery queryObject )
    {
        collectionName = collection;
	query = queryObject;
        fullDomain = readFullDomain(collectionName);
    }

    /**
     * Returns the full (unscaled) Domain of the image.
     * @return the original domain of the image
     **/
    public final RasMInterval getFullDomain()
    {
	return fullDomain;
    }

    /**
     * Returns the scaled domain of the image.
     **/
    public RasMInterval scaleGetDomain( final RasMInterval areaOp, final RasPoint origin, double scale)
	throws RasClientInternalException
    {
        try
	    {
		int i;
		int dim = areaOp.dimension();
		RasMInterval areaScale = new RasMInterval(dim);

		for( i=0 ; i<dim ; i++ )
		    {
			int low, high;

			// simple trafo of low coordinate
			low = (int)(origin.item(i) + (areaOp.item(i).low() - origin.item(i)) * scale);

			// for the high coordinate use the low coordinate of the _next_ tile
			// ( = areaOp.item(i).high() + 1 ) and subtract 1 ==> seamless tiling.
			high = (int)(origin.item(i) +
				     (areaOp.item(i).high() + 1 - origin.item(i)) * scale - 1);

/* FIXME At some point we need to have a clean design
			// now make sure these values transformed back into the original domain are legal
			// values. Due to floor rounding low <= true_low, i.e. underflow is possible.
			if ((int)((low - origin.item(i)) / scale) < areaOp.item(i).low() - origin.item(i))
			    {
				low++;
			    }

			// Because of floor rounding, high is always legal. However, if the low
			// coordinate of the next tile underflows, the high value of this one would
			// no longer match seamlessly. Therefore we have to check whether the cell
			// at high+1 transformed back into the original domain is a legal value too
			// and use that as the real high boundary if so.
			if ((int)((high+1 - origin.item(i)) / scale) <=
			    areaOp.item(i).high() - origin.item(i))
			    {
				high++;
			    }
*/

			if (high < low)
			    return null;

			areaScale.stream(new RasSInterval(low, high));

		    }
		return areaScale;
	    }
        catch(RasIndexOutOfBoundsException e1)
	    {
		throw new RasClientInternalException("RasFastBaseScale","scaleGetDomain",e1.getMessage());
	    }
        catch(RasStreamInputOverflowException e2)
	    {
		throw new RasClientInternalException("RasFastBaseScale","scaleGetDomain",e2.getMessage());
	    }
        catch(RasResultIsNoIntervalException e3)
	    {
		throw new RasClientInternalException("RasFastBaseScale","scaleGetDomain",e3.getMessage());
	    }
    }

    /**
     * Reads the full domain of the given image. This method is called in the RasFastScale constructor.
     * @param name name of the collection containing the image
     * @return the full domain of the image
     **/
    protected RasMInterval readFullDomain(String collection) throws RasClientInternalException
    {
	try {
	    query.create("SELECT sdom(img) FROM " + collection + " AS img");
	    DBag resultBag = (DBag)query.execute();
	    if (resultBag != null)
		{
		    Iterator iter = resultBag.iterator();
		    if(iter.hasNext())
			return (RasMInterval)iter.next();
		    else
			throw new RasClientInternalException("RasFastScale","readFullDomain","query produced no result.");
		}
	    else
		throw new RasClientInternalException("RasFastScale","readFullDomain","query produced no result.");

	}
	catch(Exception e1) {
	    throw new RasClientInternalException("RasFastScale","readFullDomain",e1.getMessage());
	}
    }

    /**
     * gets the minimal array
     **/
    protected RasGMArray getMinimalArray(final String collection) throws RasClientInternalException
    {
	try {
	    RasMInterval iv;
	    iv = readFullDomain(collection);
	    RasMInterval getDomain = new RasMInterval(iv.dimension());
	    int i;
	    for(i=0 ; i<iv.dimension() ; i++)
		{
		    // we actually only get one point.
		    getDomain.stream(new RasSInterval(iv.item(i).low(), iv.item(i).low()));
		}
	    query.create("SELECT img" + getDomain.toString() + " FROM " + collection + " AS img");
	    DBag resultBag = (DBag)query.execute();
	    if (resultBag != null)
		{
		    Iterator iter = resultBag.iterator();
		    if(iter.hasNext())
			return (RasGMArray)iter.next();
		    else
			throw new RasClientInternalException("RasFastScale","getMinimalArray","query produced no result.");
		}
	    else
		throw new RasClientInternalException("RasFastScale","getMinimalArray","query produced no result.");
	}
	catch(Exception e1) {
	    throw new RasClientInternalException("RasFastScale","getMinimalArray",e1.getMessage());
	}
    }

    /**
     * gets the scaling domain
     **/
    public RasMInterval getScaledDomain( final RasMInterval area, double scale)
        throws RasClientInternalException
    {
		RasMInterval retval = null;
	        int nearest = -1, i = 0;
		double nearScale = 0.0;

                // find the nearest scaling factor larger than the requested scale
	        for ( i = 0; i < (int)NUM_FAST_PRE_SCALE; i++ )
		    {
			if(FAST_PRE_SCALE_FACTORS[i] < scale) break;
			nearest = i;
		    }
		if ( nearest != -1 )
                    {
    			retval=scaleGetDomain(area, fullDomain.getOrigin(), FAST_PRE_SCALE_FACTORS[nearest]);
			if ( (scale != FAST_PRE_SCALE_FACTORS[nearest]) && (retval != null) )
			    {
			      nearScale=scale/ FAST_PRE_SCALE_FACTORS[nearest];
			      retval=scaleGetDomain(retval, fullDomain.getOrigin(), nearScale);
			    }			
		    }
		else 
		    {
			retval = scaleGetDomain(area, fullDomain.getOrigin(), scale);
		    }

		return retval;
    }

    /**
     * gets the last used scale
     **/
    public double getLastScale()
    {
        return lastScaleUsed;
    }

}

/**
 *  class for fast scaling
 * @version $Revision: 1.17 $
 **/
public class RasFastScale extends RasFastBaseScale implements RasGlobalDefs
{
  static final String rcsid = "@(#)Package rasj, class RasFastScale, RasFastBaseScale: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasFastScale.java,v 1.17 2003/12/10 21:04:23 rasdev Exp $";

    /**
     * Default constructor.
     *
     * This constructor gets the name of the image collection and and an OQLQuery object as parameters.
     * <P><B>Important:</B>This class does not open its own connection to the database nor start or
     * commit any transactions. This has to be done by the application using FastScale.</P>
     * @param collection the name of the collection containing the image(s)
     * @param queryObject the OQLQuery object for connecting to the database
     **/
    public RasFastScale( final String collection, final OQLQuery queryObject )
    {
	super(collection,queryObject);

    }

    /**
     * Returns the scaled image.
     *
     * @param trimDomain the domain of the desired image sector
     * @param scalingFactor the scaling factor ( values greater than 1 mean magnification, lesser than
     * 1 lead to a reduction )
     * @param domType determines wether the scaling or the trimmimg have to be executed first. Currently
     * only domType=1 is supported by the server.
     *
     */
    public Object getScaledImage (final RasMInterval trimDomain, double scalingFactor, int domType)
	throws QueryException
    {
        // That one should be the simpler part. Just choose the appropriate
        // scaling factor from the list and the corresponding collection,
        // adapt the factor given here and execute the query.

        int i;
        int nearest = -1;
        String suffix = null;
        RasGMArray res = null;
        Object result = null;

        // find the nearest scaling factor larger than the requested scale
        for ( i = 0; i < (int)NUM_FAST_PRE_SCALE; i++ )
	    {
		if(FAST_PRE_SCALE_FACTORS[i] < scalingFactor) break;
		nearest = i;
	    }

        while(true)
	    {
		try {

		    double nearScale;

		    // Try all collections, therefore decide on this here.
		    if (nearest == -1 || nearest == 0)
			{
			    nearScale = 1.0;
			    suffix = "";
			}
		    else
			{
			    nearScale = FAST_PRE_SCALE_FACTORS[nearest];
			    suffix = FAST_PRE_SCALE_SUFFIXES[nearest];
			}

		    // depending on the value of domType apply the trim domain to the scaled object
		    // (domType == 0) or to the unscaled object (domType != 0).
		    if (domType == 0)
			query.create("SELECT scale(img," + scalingFactor / nearScale + ')' + trimDomain +
				     " FROM " + collectionName + suffix + " AS img");
		    else
			{
			    RasMInterval scaledTrimDomain = null;

			    scaledTrimDomain = getScaledDomain(trimDomain, nearScale);
			    if ( scaledTrimDomain == null )
				{
				    nearest--;      // we can't get an error here for the highest scale!
				    continue;       // try the next higher scaling factor instead.
				}

			    query.create("SELECT scale(img" + scaledTrimDomain + ',' + scalingFactor / nearScale +
					 ") FROM " + collectionName + suffix + " AS img");
			    //System.err.println("Query : " + query );

			}

		    DBag resultBag = (DBag)query.execute();
		    if (resultBag != null)
                    {
                        result = (DBag)resultBag;
                        break;
                    }
		    else
			throw new RasClientInternalException("RasFastScale","getScaledImage","query returned no result.");
		}
		catch(Exception e1) {
		    throw new RasClientInternalException("RasFastScale","getScaledImage",e1.getMessage());
		}

	    }
	return result;
    }

}
