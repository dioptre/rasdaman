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

/**
 * Demonstration of diagram widgets
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @version 1.0.0
 */
jQuery(document).ready(function(){       
var changeSource = function(source, type){
  console.log(type);
  //Initialize the query - we are using an URL Query object
  var query = new Rj.Query.UrlQuery("multipleWcpsParser.php", 'GET', {
    'coverageId': source
  });
  //Create the widget
  var title = source.replace('NN3', 'ts').replace('_', '_00');
  var diagram = new Rj.Widget[type](query, "#chartPlace", title);          
  /**
           * Get the diagram axis and labels after the data is loaded.
           * @see event datapreload
           */
  diagram.addListener('wcps','datapreload', function(response){
    //Process the xAxis and yAxis values
    
    var series = [];
    for(var i = 0; i < response.data.length; i++){
      var values = [];
      for(var j = 0; j < response.data[i].length; j++){
        values.push([j, parseInt(response.data[i][j])]);
      }
      series.push(values);
    }
      console.log(series);
    return {
      data : series
    }
  //Configure the widget labels
  });
  diagram.loadData(true);
}

  changeSource("NN3_1,NN3_2", $("#change-diagram-2").val());
    
  $('.nns').click(function(){
    var sources = [];
    $('.nns').each(function(){
      if($(this).attr('checked')){
        sources.push($(this).val());
      }
    })
    changeSource(sources.toString(), $("#change-diagram-2").val());
  })
  $("#change-diagram-2").change(function(){
    var sources = [];
    $('.nns').each(function(){
      if($(this).attr('checked')){
        sources.push($(this).val());
      }
    })
    changeSource(sources.toString(), $(this).val());
  })
})
