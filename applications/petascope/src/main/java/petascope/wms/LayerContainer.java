/*************************************************************
 * <pre>
 * This class is used to get the actual configuration options.
 *
 * CHANGE HISTORY (append further entries):
 * when        who      what
 * ----------------------------------------------------------
 * 20-03-2002  schatz   created
 * 2003-may-15 PB       code-embedded version string
 *
 * COMMENTS:
 * A quick solution fo replacing a TreeMap which holded the layer definitions
 * needed because we want to get the layers in the same order they where defined
 * not in alfabetical order
 * Copyright (C) 2001 Dr. Peter Baumann
 * </pre>
 *********************************************************** */

package petascope.wms;

import java.util.Vector;

class LayerDefinition
{
    String layerName = null;
    WmsCapabilities.WmsLayer layerInfo = null;
    
    LayerDefinition(String ln , WmsCapabilities.WmsLayer li)
    {
        layerName = ln;
        layerInfo = li;
    }
}

public class LayerContainer
{
    
    Vector layers = null;
    
    LayerContainer()
    {
        layers = new Vector();
    }
    
    void add(String layerName, WmsCapabilities.WmsLayer layerInfo)
    {
        LayerDefinition ld = new LayerDefinition(layerName, layerInfo);
        
        layers.add(ld);
    }
    
    int getLayerCounter()
    {
        return layers.size();
    }
    
    String getLayerName(int x)
    {
        LayerDefinition ld = (LayerDefinition)layers.get(x);
        
        return ld.layerName;
    }
    
    WmsCapabilities.WmsLayer getLayerInfo(int x)
    {
        LayerDefinition ld = (LayerDefinition)layers.get(x);
        
        return ld.layerInfo;
    }
    
    WmsCapabilities.WmsLayer getLayerInfo(String x)
    {
        LayerDefinition ld = null;
        
        for(int i=0;i<layers.size();i++)
        {
            ld = (LayerDefinition)layers.get(i);
            
            if(ld.layerName.equals(x)) return ld.layerInfo;
        }
        
        return null;
    }
}
