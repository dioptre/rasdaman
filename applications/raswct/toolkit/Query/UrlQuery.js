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
 * The UrlQuery class provides an abstraction over queries sent to a server
 * It allows seamless request-response transactions to an http server.
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace("Rj.Query.UrlQuery", "Rj.Constants");

Rj.Constants.UrlQuery = {
  POST : "POST",
  GET : "GET"
}

Rj.Query.UrlQuery = new JS.Class({
  
  /**
   * Constructor for the class
   * @param <string> baseUrl - the url to start from
   * @param <string> type - the request type, e.g GET | POST
   * @param <array> parameters - an array to be used 
   */
  initialize : function(baseUrl, type, parameters){
    this.baseUrl = baseUrl;
    this.type = type || Rj.Constants.UrlQuery.GET;
    this.parameters = parameters || {};
  },
  
  /**
   * Adds a parameter to the request
   */
  addParameter : function(parameter){
    this.parameters[parameter.name] = parameter.value;
  },
  
  /**
   * Removes a parameter from the request
   */
  removeParameter : function(parameter){
    delete(this.parameters[parameter.name]);
  },
  
  /**
   * Returns a formatted get string url
   */
  transportGet : function(){
    var paramStr = '';
    for(var index in this.parameters){
      paramStr += index + "=" + this.parameters[index] + "&";
    }
    return (this.baseUrl + "?" + paramStr);
  },
  
  /**
   * Implements the transport method required for all objects that are handled
   * by an executor.
   */
  transport : function(){
    var tr = {
      type : this.type,
      params : this.parameters,
      url : this.baseUrl
    };
    return tr;
  }
})
