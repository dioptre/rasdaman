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
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
/*************************************************************
 * <pre>
 * This is a little helper class that keeps information on the rasdaman collection name, the ground resolution of the collection, and its bounding box.
 * @version $Revision: 1.3 $
 *
 * COMMENTS:
 *
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
