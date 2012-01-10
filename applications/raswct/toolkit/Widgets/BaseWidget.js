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
 * @class BaseWidget
 * Base class for widgets, exposing methods for easy communication between the current
 * widget and other widgets on the page.
 * All widgets also contain a descendant of BaseQuery, be it SelectQuery or TransportQuery
 * which it can use to receive information from ther server
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace("Rj.Widget");

Rj.Widget.BaseWidget = new JS.Class({
  
  /**
   * @param <BaseQuery> query - the query that this widget will be able to modify
   * @param <mixed> widget - a widget from one of the included libraries. E.g. new HighCharts.StockChart();
   */
  initialize : function(query, widget){
    if(!query){
      console.warn("No query object given for this Widget.");
    }
    this.query = query;
    this.widget = widget || null;
    this.listeners = {};
    this.selector = null;
  },
  
  /**
   * Adds a listener to the events fired by this widget
   * @param <string> caller - a identifier for the object that is requesting the addon
   * @param <string> event - the name of the event you want to listen to
   * @param <function> callback - a callback function that will be executed once the event is fired
   */
  addListener : function(caller, event, callback){
    if(!this.listeners[event]){
      this.listeners[event] = {};
    }
    this.listeners[event][caller] = callback;
  },
  
  /**
   * Removes a listnener from the list
   * @param <string> caller - @see addListener
   * @param <string> event - @see addListener
   */
  removeListener : function(caller, event){
    delete(this.listeners[event][caller]);
  },
  
  /**
   * Fires an event associated with the widgets. All objects that registered for this event
   * will be notified
   * @param <string> event - the name of the event to be fired
   * @param <object> args - any aditional parameters you might pass to the handlers
   */
  fireEvent : function(event, args){
    var callers = this.listeners[event];
    var status = true;
    for(var callerId in callers){
      var currentCaller = callers[callerId];
      var currentStatus = currentCaller.call(this, args);
      if(currentStatus === false){
          status = false;
      }
      else{
        status = (currentStatus !== null && currentStatus !== undefined && currentStatus !== false) ? currentStatus : status;
      }
    }
    return status;
  },
  
  /**
   * Placeholder function that should be extended by any showing widget
   * @param <string> selector - a valid CSS3 or xPath selector or any other selectors
   * that work with the jQuery library
   */
  renderTo : function(selector){
    this.selector = selector;
    $(selector).html("Loading...");    
  },
  
  /**
   * Shows the widget if it is hidden
   */
  show : function(){
    if(this.selector !== null){
      $(this.selector).show();
    }
  },
  
  /**
   * Hides the widget
   */
  hide : function(){
    if(this.selector !== null){
      $(this.selector).hide();
    }
  }
  
  
})
