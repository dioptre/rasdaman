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
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
/*************************************************************
 * <pre>
 * CLASS:	Request
 *
 * PURPOSE:
 * Holds a query, the thread that made the request should wait on the monitor of the created instance
 * The thread that solves it will notify the waiting thread that the request is done.
 * This class is only used by RasConnection and Connection.
 *
 * USES:
 *
 * COMMENTS:
 *
 * </pre>
 ************************************************************* */

package petascope.wms;

import org.odmg.*;

class Request
{
    // the query string. It contains the query that will be executted and will not be modified
    // by the connection managemenet classes.
    public final String rasQuery;
    // When the query is solved the thread that solved it will store the result here and then
    // will notify the calling thread.
    public volatile DBag result = null;
    // if set it the thread executting the request will kill itself.
    public final boolean kill;
    // if true the request has expired and it should be dropped. This will only stop the query
    // if it hasn't been sent to rasdaman server yet.
    public volatile boolean expired = false;
    // if the execution thread incouters an exception related to the request it
    // will set this.
    public volatile Exception exception = null;
    // Create a request for the given string
    public Request(String query) {
        rasQuery = query;
        kill = false;
    }
    // Create an empty(kill) request
    public Request() {
        rasQuery = null;
        kill = true;
    }
}  // Request
