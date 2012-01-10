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

