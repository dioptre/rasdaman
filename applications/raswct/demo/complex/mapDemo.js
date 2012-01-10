/**
 * Demonstration of the map widget using the WMS protocol
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @version 1.0.0
 */

  jQuery(document).ready(function(){
  
    var WMS_SERVICE_URL = "http://212.201.49.173:8080/rasogc/rasogc";
  
    var map = new Rj.Widget.MapWidget({
      projection : "EPSG:32633",
      maxExtent : new OpenLayers.Bounds(489750,7988500,492750,7990000),
      tileSize : new OpenLayers.Size(500, 500),
      numZoomLevel : 4
    });  
  
    var layerHakoon = new Rj.Widget.LayerWidget("Hakoon Drive", WMS_SERVICE_URL, {
      layers: 'Hakoon_Dive_1',
      styles: 'standard',
      format : "image/png",
      transparent: "true",            
      version : "1.1.0",
      exceptions : 'application/vnd.ogc.se_xml',
      customdem : 'minLevel,maxLevel,T'
    },
    {
      isBaseLayer: false,
      transitionEffect : 'resize'
    }); 
    
    var layerHakoon2 = new Rj.Widget.LayerWidget("Hakoon Drive 2", WMS_SERVICE_URL, {
      layers: 'Hakoon_Dive_2',
      styles: 'standard',
      format : "image/png",
      transparent: "true",            
      version : "1.1.0",
      exceptions : 'application/vnd.ogc.se_xml',
      customdem : 'minLevel,maxLevel,T'
    },
    {
      isBaseLayer: false,
      transitionEffect : 'resize'
    }); 

    var layerHakoon2b = new Rj.Widget.LayerWidget("Hakoon Drive 2b", WMS_SERVICE_URL, {
      layers: 'Hakoon_Dive_2b',
      styles: 'standard',
      format : "image/png",
      transparent: "true",            
      version : "1.1.0",
      exceptions : 'application/vnd.ogc.se_xml',
      customdem : 'minLevel,maxLevel,T'
    },
    {
      isBaseLayer: false,
      transitionEffect : 'resize'
    }); 
    
    var HakoonBathymetryLayer = new Rj.Widget.LayerWidget("Hakon_Bathymetry", WMS_SERVICE_URL, {
      layers: 'Hakon_Bathymetry',
      styles: 'colored',
      format : "image/png",
      version : "1.1.0",    
      exceptions : 'application/vnd.ogc.se_xml',
      customdem : 'minLevel,maxLevel,T'
    },{
      transitionEffect : 'resize'
    });        
    
    map.addLayers([HakoonBathymetryLayer, layerHakoon, layerHakoon2, layerHakoon2b]);    
    map.renderTo("#maps");                  
  })
