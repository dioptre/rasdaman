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
 * CHANGE HISTORY (append further entries):
 * when 	who		what
 * -----------------------------------------------------------
 * 2007-dec-06  smsorin		created
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
