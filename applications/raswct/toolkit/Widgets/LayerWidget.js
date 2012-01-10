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
 * Defines a layer used as an abstraction for map layrs that can be added 
 * to any Rj.Widget.MapWidget
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace("Rj.Widget");

Rj.Widget.LayerWidget = new JS.Class(Rj.Widget.OutputWidget, {
  
  /**
   * Constructor for layers
   * @param <string> id - a unique identifier for this layer
   * @param <string> url - an url pointing to an appropriate service(e.g WMS)
   * @param <object> serviceOptions - any parameters to be passed to the service
   * @param <object> options - additional options to be passed to the layer
   */
  initialize : function(id, url, serviceOptions, options){
    this.layer = new OpenLayers.Layer.WMS(id, url, serviceOptions, options);
    this.callSuper();
  },
  
  /**
   * Returns the raw OpenLayers layer
   */
  getRawLayer : function(){
    return this.layer
  }
  
})
