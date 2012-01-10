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
 * The QueryExecutor is a class that is responsible for the communication with the server.
 * It can receive queries from any BaseQuery descendants and then send them to the server through an
 * HTTP request responding to the requester with the result object received from the server.
 * The requests are done asynchronous in a non-blocking way so that multiple widgets can request
 * queries from the server without waiting one for the other.
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace('Rj.Executor.QueryExecutor');

/**
 * Configuration object for QueryExecutor
 */
Rj.Executor.Config = {
    url: '',
    method: 'POST',
    queryParameter: 'query'
}

Rj.Executor.QueryExecutor = new JS.Class({
    
    /**
     * The objects of this class are deffarable objects, meaning you will be able
     * to get the response using: 
     * object.callback(function(response){
     *   //code goes here
     * })
     */
    include: JS.Deferrable,
    
    /**
     * Executes the request to the server and communicates the response back.
     * @param <BaseQuery> query - An object of type descendent of BaseQuery
     */
    initialize: function(query){
        var queryTransport = query.transport();
        var url = queryTransport.url || Rj.Executor.Config.url;
        var params = queryTransport.params || {};
        var self = this;
        var method = queryTransport.type.toLowerCase() || Rj.Executor.Config.method.toLowerCase();
        
        if(queryTransport.value){
            params[Rj.Executor.Config.queryParameter] = queryTransport.value;
        }
        jQuery[method](url, params, function(response){
            self.succeed(response);
        });
    }
})
