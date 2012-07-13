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

/* 
 * Defines an a widget used for displaying linear graphs.
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace('Rj.Widget');

Rj.Widget.LinearDiagram = new JS.Class(Rj.Widget.BaseChart, {
  
  /**
   * @param <BaseQuery> query - the Query that is used to retrieve the data
   * @param <string> selector - any valid CSS3 or xPath selector that will identify the div in which the graph is placed
   * @param <string> title - the title of this diagram
   * @param <string> xAxisTitle - the title of the x Axis
   * @param <string> yAxisTitle - the title of the y Axis
   */
  initialize: function(query, selector, title, xAxisTitle, yAxisTitle){
    this.callSuper(query, selector, title, xAxisTitle, yAxisTitle);    
    this.processed = false;
  },
  
  configure : function(cfg){
    this.cfg.seriesDefaults = this.cfg.seriesDefaults || {};
    this.cfg.seriesDefaults.renderer = jQuery.jqplot.LineRenderer;    
    this.cfg.seriesDefaults.lineWidth = 1;    
    
    if(!cfg){
      cfg = {};
    }
    this.cfg = jQuery.extend(this.cfg, cfg);
    return this.cfg
  }
    
});
