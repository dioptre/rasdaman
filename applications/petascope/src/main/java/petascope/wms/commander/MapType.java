/************************************************************************
 *
 * SOURCE: rasgeo.MapType
 *
 * PACKAGE: petascope.wms.commander
 * CLASS: MapType
 *
 * PURPOSE
 * auxiliary class for map type enumeration values
 *
 * CHANGE HISTORY (append further entries):
 * when         who       what
 * ----------------------------------------------------------
 * 2007-jul-27  PB        created
 *
 * COMMENTS
 * - none -
 *
 * Copyright (C) 2007 Dr. Peter Baumann
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

