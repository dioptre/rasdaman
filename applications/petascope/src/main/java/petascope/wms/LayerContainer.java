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
 * This class is used to get the actual configuration options.
 *
 * COMMENTS:
 * A quick solution fo replacing a TreeMap which holded the layer definitions
 * needed because we want to get the layers in the same order they where defined
 * not in alfabetical order.
 * 
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
