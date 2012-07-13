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
  var changeDiagramSource = function(source, type){
    //Initialize the query - we are using an URL Query object
    var query = new Rj.Query.UrlQuery("wcpsParser.php", 'GET', {
      'coverageId': source
    });
    //Create the widget
    var diagram = new Rj.Widget[type](query, "#chartPlace", source);          
    /**
           * Get the diagram axis and labels after the data is loaded.
           * @see event datapreload
           */
    diagram.addListener('wcps','datapreload', function(response){
      if(response.error){
        $("body").append("<div id='dialog'>"+response.error+'</div>');
        $( "#dialog" ).dialog({
          modal : true,
          title : 'Parse Error'
        }).show();
        $("#chartPlace").html("");
        throw "Error while processing the data";        
      }
      
      var values = [];
      for(var i = 0; i < response.data.length; i++){
        values.push([i, parseInt(response.data[i], 10)]);
      }
      
      //Configure the widget labels
      this.configure({
        axes : {
          xaxis:{
            title : response.domainInfo.axisLabel
          },
          yaxis : {
            title : "Values"
          }
        }
      });
      return {
        data : [values]
        };
    });
    
    diagram.loadData(true);
  }
    changeDiagramSource("NN3_10", $("#change-diagram").val());
    $("#change-source").click(function(){
      changeDiagramSource($("#chart-source").val(), $("#change-diagram").val());
    })
  })

