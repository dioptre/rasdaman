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
package petascope.util.ras;

import org.antlr.runtime.RecognitionException;
import org.apache.commons.io.IOUtils;
import org.odmg.Database;
import org.odmg.Implementation;
import org.odmg.ODMGException;
import org.odmg.OQLQuery;
import org.odmg.QueryException;
import org.odmg.Transaction;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.ConfigManager;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCPSException;
import petascope.wcps.server.core.ProcessCoveragesRequest;
import petascope.wcps.server.core.WCPS;
import rasj.RasImplementation;
import rasj.RasConnectionFailedException;

/**
 * Rasdaman utility classes - execute queries, etc.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class RasUtil {

    private static final Logger log = LoggerFactory.getLogger(RasUtil.class);

    // FIXME - should return just String?
    public static Object executeRasqlQuery(String query) throws RasdamanException {
        Implementation impl = new RasImplementation(ConfigManager.RASDAMAN_URL);
        Database db = impl.newDatabase();
	int maxAttempts,timeout,attempts=0;	

	try{
	    timeout=Integer.parseInt(ConfigManager.RASDAMAN_RETRY_TIMEOUT)*1000;
	}catch(NumberFormatException ex){
	    timeout=5000;
	}

	try{
	    maxAttempts=Integer.parseInt(ConfigManager.RASDAMAN_RETRY_ATTEMPTS);
	}catch(NumberFormatException ex){
	    maxAttempts=3;
	}
	
        Transaction tr;
	Object ret=null;

	//Try to connect until the maximum number of attempts is reached
	//This loop handles connection attempts to a saturated rasdaman
	//complex which will refuse the connection until a server becomes
	//available.
	boolean queryCompleted=false,dbOpened=false;
	while(!queryCompleted){
	    try{
		db.open(ConfigManager.RASDAMAN_DATABASE, Database.OPEN_READ_ONLY);
		dbOpened=true;
		tr = impl.newTransaction();
		tr.begin();
		OQLQuery q = impl.newOQLQuery();
		try {
		    q.create(query);
		    log.trace("Executing query {}", query);
		    ret = q.execute();
		} catch (QueryException ex) {
		    tr.abort();
		    throw new RasdamanException(ExceptionCode.InternalSqlError,
						"Error evaluating rasdaman query: '" + query, ex);
		} finally {
		    tr.commit();
		    queryCompleted=true;
		    try {
			db.close();
		    } catch (ODMGException ex) {
			log.error("Error closing database connection", ex);
		    }
		}
	    }catch(RasConnectionFailedException ex){
		attempts++;
		if(dbOpened)
		    try{
			db.close();
		    }catch(ODMGException e){
			log.error("Error closing database connection", e);
		    }
		dbOpened=false;
		if(!(attempts<maxAttempts))
		    throw ex;
		try{
		    Thread.sleep(timeout);
		}catch(InterruptedException e){
		    log.error("Thread "+Thread.currentThread().getName()+
			      " was interrupted while searching a free server.");
		    throw new RasdamanException(ExceptionCode.RuntimeError,
						"Error getting a free rasdaman server");
		}
	    }catch(ODMGException ex){
		throw new RasdamanException(ExceptionCode.RuntimeError,
						"Error getting a free rasdaman server");
	    }
	}            
        return ret;
    }
    
    /**
     * Convert WCPS query in abstract syntax to a rasql query.
     * 
     * @param query WCPS query in abstract syntax
     * @param wcps WCPS engine
     * @return the corresponding rasql query
     */
    public static String abstractWCPSToRasql(String query, WCPS wcps) throws WCPSException {
        if (query == null) {
            throw new WCPSException(ExceptionCode.InvalidParameterValue, "Can't convert null query");
        }
        log.trace("Converting abstract WCPS query\n{}", query);
        String xmlQuery = null;
        try {
            xmlQuery = ProcessCoveragesRequest.abstractQueryToXmlQuery(query);
            log.debug("xmlQuery: " + xmlQuery); // (to_remove)
        } catch (RecognitionException ex) {
            throw new WCPSException(ExceptionCode.InvalidParameterValue,
                    "Error translating abstract WCPS query to XML format.", ex);
        }
        String rasql = xmlWCPSToRasql(xmlQuery, wcps);
        log.debug("rasql: " + rasql);
        return rasql;
        //return xmlWCPSToRasql(xmlQuery, wcps);
    }
    
    /**
     * Convert WCPS query in XML syntax to a rasql query.
     * 
     * @param query WCPS query in XML syntax
     * @param wcps WCPS engine
     * @return the corresponding rasql query
     */
    public static String xmlWCPSToRasql(String query, WCPS wcps) throws WCPSException {
        if (query == null) {
            throw new WCPSException(ExceptionCode.InvalidParameterValue, "Can't convert null query");
        }
        log.trace("Converting XML WCPS query\n{}", query);
        ProcessCoveragesRequest pcReq;
        try {
            pcReq = wcps.pcPrepare(ConfigManager.RASDAMAN_URL,
                    ConfigManager.RASDAMAN_DATABASE, IOUtils.toInputStream(query));
        } catch (Exception ex) {
            throw new WCPSException(ExceptionCode.InternalComponentError,
                    "Error preparing process coverage request.", ex);
        }
        log.trace("Resulting RasQL query: [{}] {}", pcReq.getMime(), pcReq.getRasqlQuery());
        String ret = pcReq.getRasqlQuery();
        return ret;
    }

    /**
     * Execute a WCPS query given in abstract syntax.
     * 
     * @param query a WCPS query given in abstract syntax
     * @param wcps WCPS engine
     * @return result from executing query
     * @throws WCPSException 
     */
    public static Object executeWcpsQuery(String query, WCPS wcps) throws WCPSException, RasdamanException {
        if (query == null) {
            throw new WCPSException(ExceptionCode.InvalidParameterValue, "Can't execute null query");
        }
        query = query.trim();
        log.trace("Executing WCPS query: {}", query);
        if (query.startsWith("<")) {
            return executeXmlWcpsQuery(query, wcps);
        } else {
            return executeAbstractWcpsQuery(query, wcps);
        }
    }

    /**
     * Execute a WCPS query given in abstract syntax.
     * 
     * @param query a WCPS query given in abstract syntax
     * @param wcps WCPS engine
     * @return result from executing query
     * @throws WCPSException 
     */
    public static Object executeAbstractWcpsQuery(String query, WCPS wcps) throws WCPSException, RasdamanException {
        if (query == null) {
            throw new WCPSException(ExceptionCode.InvalidParameterValue, "Can't execute null query");
        }
        log.trace("Executing abstract WCPS query");
        return executeRasqlQuery(abstractWCPSToRasql(query, wcps));
    }

    /**
     * Execute a WCPS query given in XML syntax.
     * 
     * @param query a WCPS query given in XML syntax
     * @param wcps WCPS engine
     * @return the result from executing query
     * @throws WCPSException 
     */
    public static Object executeXmlWcpsQuery(String query, WCPS wcps) throws WCPSException, RasdamanException {
        if (query == null) {
            throw new WCPSException(ExceptionCode.InvalidParameterValue, "Can't execute null query");
        }
        log.trace("Executing XML WCPS query");
        return executeRasqlQuery(xmlWCPSToRasql(query, wcps));
    }
}
