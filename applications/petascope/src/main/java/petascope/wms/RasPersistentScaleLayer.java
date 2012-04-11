/*************************************************************
 * <pre>
 * This is a little helper class that keeps information on the rasdaman collection name, the ground resolution of the collection, and its bounding box.
 * @version $Revision: 1.3 $
 *
 * CHANGE HISTORY (append further entries):
 * when         who             what
 * ----------------------------------------------------------
 * 2003-may-15  PB      started documentation
 *                      added revision string in src
 *
 * COMMENTS:
 *
 * Copyright (C) 2001 Dr. Peter Baumann
 * </pre>
 ************************************************************/

package petascope.wms;


public class RasPersistentScaleLayer
{
    /**
     * The rasdaman collection name
     **/
    protected String collectionName;
    
    /**
     * The area in GK which is covered by the RasDaMan collection.  The collection must start at the origin ([0,0]).
     **/
    protected BoundingBox boundingBox;
    
    /**
     * Create a new object:
     * @param theRasCollectionName the name of the RasDaMan collection
     * @param theGroundResolution the ground resolution (scale factor to GK)
     * @param bbox the bounding box of the image in the collection
     **/
    public RasPersistentScaleLayer(String theRasCollectionName, BoundingBox bbox)
    {
        collectionName = theRasCollectionName;
        boundingBox = bbox;
    }
    
    public double getGroundResolutionX()
    {
        return boundingBox.resX;
    }
    
    public double getGroundResolutionY()
    {
        return boundingBox.resY;
    }
    
    /**
     * accessor method
     * @return a reference to the bounding box of the persistent collection
     **/
    public BoundingBox getBoundingBox()
    {
        return boundingBox;
    }
    
    /**
     * accessor method
     * @return a reference to the persistent collections name
     **/
    public String getName()
    {
        return collectionName;
    }
}
