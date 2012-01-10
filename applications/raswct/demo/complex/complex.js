/**
 * Demonstration of input widgets and chart widgets, combined in a nice interface
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @version 1.0.0
 */

$(document).ready(function(){
/**
   * Creates a new chart widget with a different data source
   */
var changeDiagram = function(source, type, selector, skip, url){
  selector = selector || "#chartPlace"
  skip = skip || "";
  url = "wcpsParserThr.php";
  //Initialize the query - we are using an URL Query object
  var query = new Rj.Query.UrlQuery(url, 'GET', {
    'coverageId': source,
    'skip' : skip
  });
    
  //Create the widget
  var title = "Important data";
  var diagram = new Rj.Widget.LinearDiagram(query, selector, title);          
    
  /**
     * Get the diagram axis and labels after the data is loaded.
     * @see event datapreload
     */
  diagram.addListener('wcps','datapreload', function(response){
    //error handling
    if(response.error){
      $("body").append("<div id='dialog'>"+response.error+'</div>');
      $( "#dialog" ).dialog({
        modal : true,
        title : 'Parse Error'
      }).show();
      $("#chartPlace").html("");
      throw "Error while processing the data";        
    }
      
    //Process the xAxis and yAxis values so that we can render them as a graph
    var categories = [], values = [];            
    var k = 0;
    for(var i = parseInt(response.domainInfo.lowLimit); i < parseInt(response.domainInfo.highLimit); i++){
      categories.push(i);
      if(response.data[i] !== undefined && response.data[i] !== null){
        values[k] = [k, parseInt(response.data[i])];
        k++
      }
    }            
      
    return {
      data : [values]
    };
  });
    
  diagram.loadData(true);
}

changeDiagram("NN3_03", "LinearDiagram", "#chartPlace", 8000, "wcpsParserThr.php");
window.chartSource = 1;
  
//Default threshold values for each source
var thresholds = [0, 8000, 5000, 3000, 6000, 4500, 6000, 5000];
    
//The vertical threshold slider
var thresholdSlider = new Rj.Widget.SliderWidget(1000, 11000, "vertical", 500, 8000);
thresholdSlider.addListener("complex", "slidevalue", function(val){
  changeDiagram("NN3_" + window.chartSource.toString(), "LinearDiagram", "#chartPlace", val, "wcpsParserThr.php");
  $("#threshold-value").html(val);
});
thresholdSlider.renderTo("#thresh-vert");
    
//The horizontal threshold slider
var sourceSlider = new Rj.Widget.SliderWidget(1, 6, "horizontal", 1, 1);
sourceSlider.renderTo("#thresh-horiz");
sourceSlider.addListener("complex", "changevalue", function(val){
  window.chartSource = val;
  changeDiagram("NN3_"+val.toString(), "LinearDiagram", "#chartPlace", thresholds[val], "wcpsParserThr.php");
  $("#source-id").html(val);
  thresholdSlider.setSlideLevel(thresholds[val]);
})
  
  })
