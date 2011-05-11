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
package petascope.util;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.antlr.runtime.RecognitionException;
import org.apache.commons.io.IOUtils;
import org.odmg.DBag;
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
import rasj.RasGMArray;
import rasj.RasImplementation;
import static petascope.util.ListUtil.*;

/**
 * Rasdaman utility classes - execute queries, etc.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class RasUtil {

    private static final Logger log = LoggerFactory.getLogger(RasUtil.class);

    // FIXME - should return just String?
    public static byte[] executeRasqlQuery(String query) throws RasdamanException {
        Implementation impl = new RasImplementation(ConfigManager.RASDAMAN_URL);
        Database db = impl.newDatabase();
        try {
            db.open(ConfigManager.RASDAMAN_DATABASE, Database.OPEN_READ_ONLY);
        } catch (Exception ex) {
            try {
                db.close();
            } catch (ODMGException e) {
            }
            throw new RasdamanException(ExceptionCode.InternalComponentError,
                    "Could not connect to rasdaman server at " + ConfigManager.RASDAMAN_URL
                    + ", database " + ConfigManager.RASDAMAN_DATABASE, ex);
        }

        Transaction tr = impl.newTransaction();
        tr.begin();
        OQLQuery q = impl.newOQLQuery();
        DBag resultSet;
        try {
            q.create(query);
            log.trace("Executing query {}", query);
            resultSet = (DBag) q.execute();

            if (resultSet != null) {
                Iterator rsit = resultSet.iterator();
                while (rsit.hasNext()) {
                    Object current = rsit.next();
                    try {
                        return ((RasGMArray) current).getArray();
                    } catch (Exception ex) {
                        return current.toString().getBytes();
                    }
                }
            }
        } catch (QueryException ex) {
            throw new RasdamanException(ExceptionCode.InternalSqlError,
                    "Error evaluating rasdaman query: '" + query, ex);
        } finally {
            tr.commit();
            try {
                db.close();
            } catch (ODMGException ex) {
                log.error("Error closing database connection", ex);
            }
        }
        return null;
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
        } catch (RecognitionException ex) {
            throw new WCPSException(ExceptionCode.InvalidParameterValue,
                    "Error translating abstract WCPS query to XML format.", ex);
        }
        return xmlWCPSToRasql(xmlQuery, wcps);
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
        return pcReq.getRasqlQuery();
    }

    /**
     * Execute a WCPS query given in abstract syntax.
     * 
     * @param query a WCPS query given in abstract syntax
     * @param wcps WCPS engine
     * @return result from executing query
     * @throws WCPSException 
     */
    public static byte[] executeAbstractWcpsQuery(String query, WCPS wcps) throws WCPSException, RasdamanException {
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
    public static byte[] executeXmlWcpsQuery(String query, WCPS wcps) throws WCPSException, RasdamanException {
        if (query == null) {
            throw new WCPSException(ExceptionCode.InvalidParameterValue, "Can't execute null query");
        }
        log.trace("Executing XML WCPS query");
        return executeRasqlQuery(xmlWCPSToRasql(query, wcps));
    }
}
