/** **********************************************************
 * <pre>
 * Class for representing an wms bounding box.
 *
 * CHANGE HISTORY (append further entries):
 * when         who       what
 * ----------------------------------------------------------
 * 06-07-2001   Beinhofer created
 * 2003-may-15  PB        code-embedded version string
 * 2004-apr-24  PB        constructors: parameter plausi check with exception
 * 2005-jun-20  PB        added disjoint()
 *                        bugfix in covers(): maxY coordinate was not checked
 * 2005-oct-07  PB        srs unused -> commented
 *
 * COMMENTS:
 *
 * Copyright (C) 2001 Dr. Peter Baumann
 * </pre>
 ************************************************************/

package petascope.wms;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WMSException;


public class BoundingBox
{
    private static final Logger log = LoggerFactory.getLogger(BoundingBox.class);
    /**
     * The coordinates for a 2 dim bounding box as defined in wms.
     **/
    public double minX, minY, maxX, maxY = 0;
    
    /**
     * The coordinate system. Not used -- PB 2005-oct-07
     **/
    // public String srs;
    
    /**
     * The resolution in x direction.
     * The ground resolution (for 1 Gauss-Krueger unit a cell is 1m).
     * If this collection is fast scaled then the first scale level (_1) has resolution .5, the second .25 and so on.
     * Note however this is a convention which can be happily overridden in the raswms configuration.
     **/
    public double resX;
    
    /**
     * The resolution in y direction.
     * The ground resolution (for an image that has a resolution of 1 GK unit/ 1 RasDaMan cell it is 1.  If this collection is fast scaled then the first scale level (_1) has resolution .5, the second .25 and so on.
     **/
    public double resY;
    
    /**
     * A very basic check if the supplied bounding box is completely covered by <B>this</B>.
     * @param otherBox the box that is supposed to be completely covered
     **/
    public boolean covers(BoundingBox otherBox)
    {
        boolean result = ((otherBox.minX >= minX) && (otherBox.maxX <= maxX) && (otherBox.minY >= minY) && (otherBox.maxY <= maxY));
        log.debug( "BoundingBox.covers: " + otherBox.minX + ">=" + minX + " && " + otherBox.maxX + "<=" + maxX + " && " + otherBox.minY + ">=" + minY + " && " + otherBox.maxY + "<=" + maxY + " -> result " + result );
        return result;
    }
    
    /**
     * Check whether the supplied bounding box and <B>this</B> are completely disjoint.
     * @param otherBox the box that is to be intersected
     **/
    public boolean disjoint(BoundingBox otherBox)
    {
        boolean result = ((otherBox.minX > maxX) || (otherBox.maxX < minX) || (otherBox.minY > maxY) || (otherBox.maxY < minY));
        log.debug( "BoundingBox.disjoint: " + otherBox.minX + ">" + minX + " || " + otherBox.maxX + "<" + maxX + " || " + otherBox.minY + ">" + minY + " || " + otherBox.maxY + "<" + maxY + " -> result " + result );
        return result;
    }
    
    /**
     * Create a new bounding box object.
     * @param x the minimum x coordinate
     * @param y the minimum y coordinate
     * @param X the maximum x coordinate
     * @param Y the maximum y coordinate
     * @throws WMSException on parameter inconsistencies
     */
    public BoundingBox(double x, double y, double X, double Y)
            throws WMSException
    {
        if (x >= X)
        {
            log.error( "BoundingBox violation, lower X " + x + " higher than upper X " + X );
            throw new WMSException( Globals.ERR_Bbox_X );
        }
        if (y >= Y)
        {
            log.error( "BoundingBox violation, lower Y " + y + " higher than upper Y " + Y );
            throw new WMSException( Globals.ERR_Bbox_Y );
        }
        
        minX = x;
        minY = y;
        maxX = X;
        maxY = Y;
    }
    
    /**
     * Create a new bounding box object.
     * @param x the minimum x coordinate
     * @param y the minimum y coordinate
     * @param X the maximum x coordinate
     * @param Y the maximum y coordinate
     * @param rx the resolution in x direction
     * @param ry the resolution in y direction
     * @throws WMSException on parameter inconsistencies
     */
    public BoundingBox(double x, double y, double X, double Y, double rx, double ry)
            throws WMSException
    {
        this(x, y, X, Y);
        
        if (rx <= 0 || ry <= 0)
        {
            log.error( "BoundingBox violation, illegal resolution: rx=" + rx + ", ry=" + ry );
            throw new WMSException( Globals.ERR_Bbox_Resolution );
        }
        
        resX = rx;
        resY = ry;
    }
    
    /**
     * Return a string containing a comma separated list of min x, min y, max x, max y.
     */
    public String toString()
    {
        return "<BoundingBox minx=" + minX + " miny=" + minY + " maxx=" + maxX + " maxy=" + maxY + " resx=" + resX + " resy=" + resY + " />";
    }
} // BoundingBox
