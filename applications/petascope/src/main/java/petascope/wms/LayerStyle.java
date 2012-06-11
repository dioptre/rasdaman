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
 * Very basic typed pair<string> class.  Holds the name of the layer and its style.
 * @version $Revision: 1.4 $
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package petascope.wms;

public class LayerStyle implements Comparable<LayerStyle>
{
    /**
     * Name of the layer.
     **/
    protected String layer;
    
    /**
     * Name of the layer's style.
     **/
    protected String style;
    
    /**
     * Create a new object holding a layer name and a style name.
     * @param l layer name
     * @param s stye name
     **/
    public LayerStyle(String l, String s)
    {
        layer = l;
        style = s;
    }
    
    /**
     * Return a string containing layer name ", " style
     **/
    public String toString()
    {
        return "[" + layer + "." + style + "]";
    }
    
    /**
     * Return a reference to the layer name
     **/
    public String getLayer()
    {
        return layer;
    }
    
    /**
     * Return a reference to the style name
     **/
    public String getStyle()
    {
        return style;
    }
    
    public int compareTo(LayerStyle ls) {
        return (this.toString().compareTo(ls.toString()));
    }

}
