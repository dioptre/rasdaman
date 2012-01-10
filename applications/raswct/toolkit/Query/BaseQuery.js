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

/* 
 * Base class for the query adapter. The specialized query classes (SelectQuery, InsertQuery, UpdateQuery, DeleteQuery)
 * will be children of this class.
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace('Rj.Query.BaseQuery', 'Rj.Query.BaseQueryConfig');

/**
 * Unique identifier of the BaseQuery object.
 * Starts from 1 and increments automatically whnever a new object is created.
 */
Rj.Query.BaseQueryConfig.staticId = 1;


Rj.Query.BaseQuery = new JS.Class({   
   
  /**
    * Standard class constructor, defining attributes
    * @param <string> query - The query that will eventually be executed
    */ 
  initialize: function(query){
    this.id = Rj.Query.BaseQueryConfig.staticId;
    Rj.Query.BaseQueryConfig.staticId++;
    this.query = query || "";
  },
   
  /**
    * Standard getter for the id attribute
    * @return <int> - The id of the BaseQuery object
    */
  getId: function(){
    return this.id;
  },
   
  /**
    * Standard getter for the query attribute
    * @return <string> - The string query attached to this base query
    */
  getQuery: function(){
    return this.query;
  },
   
  /**
    * Sets the query to a new value
    * @param <string> query - the new query
    * @return <BaseQuery> - The BaseQuery object on which the operation has been performed
    */
  setQuery: function(query){
    this.query = query;
    return this;
  }  
   
});


