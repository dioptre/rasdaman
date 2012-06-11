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
/************************************************************************
 *
 * PURPOSE
 * auxiliary class for map type enumeration values
 *
 * COMMENTS
 *
 *********************************************************** */

package petascope.wms.commander;

import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;

public final class MapType
{
    // instance fields
    private final String mapType;
    
    // static field
    private static final List _VALUES = new ArrayList();
    
    /** Unmodifiable view of all map types */
    public static final List VALUES = Collections.unmodifiableList( _VALUES );
    // the different values of the enum
    public static final MapType GREY = new MapType("grey");	// 8-bit int pixels
    public static final MapType RGB  = new MapType("RGB");	// 3*8-bit int pixels
    public static final MapType DEM  = new MapType("DEM");	// double pixels
    
    private MapType( String name )
    {
        this.mapType = name;
        MapType._VALUES.add( this );
    }
    
    public String toString()
    {
        return mapType;
    }
    
    public static MapType valueOf( String i )
    {
        Iterator iter = VALUES.iterator();
        while( iter.hasNext() )
        {
            Object obj = iter.next();
            if( i.equalsIgnoreCase( obj.toString() ) )
            {
                return (MapType)obj;
            }
        }
        
        throw new IllegalArgumentException( "Illegal map type: " + i );
    }

}

