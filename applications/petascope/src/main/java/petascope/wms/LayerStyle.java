/*************************************************************
 * <pre>
 * Very basic typed pair<string> class.  Holds the name of the layer and its style.
 * @version $Revision: 1.4 $
 *
 * CHANGE HISTORY (append further entries):
 * when        who         what
 * ----------------------------------------------------------
 * 06-07-2001  Beinhofer   created
 * 2003-may-15  PB      code-embedded version string
 * 2009-jan-21  shikhar implement Comparable interface, so List<LayerStyle> can be sorted
 *
 * COMMENTS:
 *
 * Copyright (C) 2001 Dr. Peter Baumann
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
