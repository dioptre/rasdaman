/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann / rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/

/* 
 * Defines an a widget used for displaying maps composed
 * of several layers
 * The implementation is based on the OpenLayers library <http://http://openlayers.org/>
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace("Rj.Widget");

Rj.Widget.MapWidget = new JS.Class(Rj.Widget.OutputWidget, {
  
  /**
   * Constructor for the map widget
   * @param <object> options
   */
  initialize : function(options){
    this.map = new OpenLayers.Map(options);
    this.callSuper();
  },
  
  /**
   * Returns the raw OpenLayers Map
   */
  getRawMap : function(){
    return this.map;
  },
  
  /**
   * Adds layers to the map
   * @param <array> layers
   */
  addLayers : function(layers){
    var rawLayers = [];
    for(var i = 0; i < layers.length; i++){
      rawLayers.push(layers[i].getRawLayer());
    }    
    this.map.addLayers(rawLayers);
  },
  
  /**
   * @override Rj.OutputWidget.renderTo
   */
  renderTo : function(selector){
    this.map.addControl( new OpenLayers.Control.LayerSwitcher() );
    this.map.addControl(new OpenLayers.Control.MousePosition());
    this.map.addControl(new OpenLayers.Control.OverviewMap());
    this.map.addControl(new OpenLayers.Control.KeyboardDefaults());    
    this.map.zoomToMaxExtent();
    this.map.render(Rj.getId(selector));
  }
  
})
