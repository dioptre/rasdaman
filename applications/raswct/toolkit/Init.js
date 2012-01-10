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

/**
 * Initializes the toolkit, adding namespaces and initial objects to the DOM tree
 * All the packages should be registered here such that they get loaded on demand
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

TOOLKIT_PATH = "";

Rj = {
  version : "1.0.0",
  versionDetail : {
    major : 1,
    minor : 0,
    patch : 0
  }
}

JS.Packages(function() {
  with(this) {
    
    file(TOOLKIT_PATH + 'Query/BaseQuery.js')
    .provides('Rj.Query.BaseQuery').requires('JS.Class');
    
    file(TOOLKIT_PATH + 'Query/SelectQuery.js')
    .provides('Rj.Query.SelectQuery').requires('JS.Class', 'Rj.Query.BaseQuery');
    
    file(TOOLKIT_PATH + 'Query/UrlQuery.js')
    .provides('Rj.Query.UrlQuery').requires('JS.Class');    
    
    file(TOOLKIT_PATH + 'Query/QueryExecutor.js')
    .provides('Rj.Executor.QueryExecutor').requires('JS.Class', 'JS.Deferrable', 'Rj.Query.BaseQuery', 'Rj.Query.UrlQuery', 'Rj.Query.SelectQuery');
    
    file(TOOLKIT_PATH + "Widgets/BaseWidget.js")
    .provides('Rj.Widget.BaseWidget').requires('Rj.Query.SelectQuery', 'Rj.Query.UrlQuery', 'Rj.Executor.QueryExecutor');

    file(TOOLKIT_PATH + "Widgets/InputWidget.js")
    .provides('Rj.Widget.InputWidget').requires('Rj.Widget.BaseWidget');
    
    file(TOOLKIT_PATH + "Widgets/TextWidget.js")
    .provides('Rj.Widget.TextWidget').requires('Rj.Widget.InputWidget');    
    
    file(TOOLKIT_PATH + "Widgets/SliderWidget.js")
    .provides('Rj.Widget.SliderWidget').requires('Rj.Widget.InputWidget');        

    file(TOOLKIT_PATH + "Widgets/OutputWidget.js")
    .provides('Rj.Widget.OutputWidget').requires('Rj.Widget.BaseWidget');
    
    file(TOOLKIT_PATH + "Widgets/BaseChart.js")
    .provides('Rj.Widget.BaseChart').requires('Rj.Widget.OutputWidget');    
    
    file(TOOLKIT_PATH + "Widgets/LinearZoomDiagram.js")
    .provides("Rj.Widget.LinearZoomDiagram").requires('Rj.Widget.BaseChart');
    
    file(TOOLKIT_PATH + "Widgets/LinearDiagram.js")
    .provides("Rj.Widget.LinearDiagram").requires('Rj.Widget.BaseChart');    
    
    file(TOOLKIT_PATH + "Widgets/ScatterDiagram.js")
    .provides("Rj.Widget.ScatterDiagram").requires('Rj.Widget.BaseChart');        
    
    file(TOOLKIT_PATH + "Widgets/AreaDiagram.js")
    .provides("Rj.Widget.AreaDiagram").requires('Rj.Widget.BaseChart', "Rj.Widget.ScatterDiagram", "Rj.Widget.LinearDiagram");            

    file(TOOLKIT_PATH + "Widgets/LayerWidget.js")
    .provides("Rj.Widget.LayerWidget").requires('Rj.Widget.OutputWidget');            
    
    file(TOOLKIT_PATH + "Widgets/MapWidget.js")
    .provides("Rj.Widget.MapWidget").requires('Rj.Widget.LayerWidget', 'Rj.Widget.OutputWidget');                
    
    
    }
});
