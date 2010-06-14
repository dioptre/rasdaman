package rasj;

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
 * This class is used to express the storage options for MDD objects.
 * In the current version, either the tile-size (for example 256000 bytes) or the 
 * tiling-domain (for example "[0:127,0:511]") can be specified. If 
 * neither is done, the default tile-size will be 128 kBytes.
 *
 * @version $Revision: 1.10 $
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasStorageLayout
   
{
    static final String rcsid = "@(#)Package rasj.odmg, class RasStorageLayout: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasStorageLayout.java,v 1.10 2003/12/10 21:04:23 rasdev Exp $";

    /**
     * Default tile size in kBytes. This value is currently set
     * to 128000 and is used if the user has not specified
     * a tile size explicitly.
     **/
    static final int DEFAULT_TILE_SIZE = 128000;

    /**
     * the spatial domain of the current storageLayout
     **/
    private RasMInterval spatialDomain;

    /**
     * the current tile size in bytes
     **/
    private int tileSize;
   
    /**
     * Default constructor.
     **/
    public RasStorageLayout()
    {
	spatialDomain = null;
	tileSize = DEFAULT_TILE_SIZE;
    }   

    /**
     * Copy constructor taking another storage layout object as a parameter.
     * @param stl the sorage layout object to be copied
     **/
    public RasStorageLayout(RasStorageLayout stl)
    {
	spatialDomain = stl.getSpatialDomain();
	tileSize = stl.getTileSize();
    }

    /**
     * Returns the current tiling-domain of this storage layout object.
     * @return the tiling-domain of this storage layout object
     **/
    public final RasMInterval getSpatialDomain()
    {
	return spatialDomain;
    }

    /**
     * Sets the tile-size (in bytes) and resets the tiling-domain to null.
     * @param size the tile-size (in bytes)
     **/
    public void setTileSize(int size)
    {
	tileSize = size;
	spatialDomain = null;
    }

    /**
     * Returns the current tile-size (in bytes).
     * @return the tile-size
     **/
    public final int getTileSize()
    {
	return tileSize;
    }

    /**
     * Sets the tiling-domain for this storage layout object and resets the tile-size
     * to null.
     * @param domain the tiling-domain
     **/
    public void setTileDomain(String domain) 
	throws RasResultIsNoIntervalException, RasStreamInputOverflowException, RasIndexOutOfBoundsException
    {
	spatialDomain = new RasMInterval(domain);
	tileSize = 0;
    }

    /**
     * This method tests if the current storageLayout object is compatible to a given domain.
     * @param domain the domain to be compared 
     * @return true if the tiling-domain of this storage layout object is either null or 
     * has the same number of dimensions as the given domain, otherwise false 
     **/
    public boolean isCompatible(RasMInterval domain)
    {
	if(spatialDomain == null)
	    return true;
        else
	    return (domain.dimension() == spatialDomain.dimension());

    }

    /**
     * Returns a string representation of this class.
     **/
    public String toString()
    {
	if(spatialDomain != null)
	    return "\nTileDomain: " + spatialDomain + "\n";
        else
	    return "\nTileSize: " + tileSize + "\n";

    }
    
    
}
  
