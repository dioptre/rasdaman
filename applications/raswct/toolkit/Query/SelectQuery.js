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
 * The SelectQuery class provides an abstraction over the select query sent to the server.
 * It allows widget developers to easy modify queries by adding new fields to the
 * query or creating conditions which will be transformed into a query that can be sent to the server.
 *
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace("Rj.Query.SelectQuery");

Rj.Query.SelectQuery = new JS.Class(Rj.Query.BaseQuery, {

  /**
   * Constructor for SelectQuery
   * @param <string> query - the initial query attached to this object
   * @param <object> variables - the variables that are attached to the query, each of them can be modified using setVariable
   */
  initialize : function(url, query, variables){
    this.url = url;
    this.variables = variables || {};
    this.callSuper(query);
  },

  /**
   * Returns the value attached to the given variable
   * @param <string> the name of the variable that needs to be returned
   */
  getVariable : function(variable){
    return this.variables[variable];
  },

  /**
   * Assigns a value to a variable
   * @param <string> variable - the variable name
   * @param <mixed> value - the value of the variable
   * @return <SelectQuery> the query the actions is performed
   */
  setVariable : function(variable, value){
    this.variables[variable] = value;
    return this;
  },

  replaceVariablesInQuery : function(){
    var vars = this.variables;
    var query = this.query;
    for (var variable in vars){
      query = query.replace(new RegExp(variable, 'g'), vars[variable].toString());
    }
    return query;
  },


  /**
   * Return the query in a transport format, as requested by the QueryExecutor specs
   * The returned query should be of form "SELECT fields FROM collections WHERE conditions" or similar
   * The prefered transport method is POST
   * @return <object> A transport object
   */
  transport : function(){
    var processedQuery = this.replaceVariablesInQuery();
    return {
      url : this.url,
      type : 'POST',
      value : processedQuery
    };
  }

})
