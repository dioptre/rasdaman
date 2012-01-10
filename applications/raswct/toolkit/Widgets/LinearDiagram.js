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
