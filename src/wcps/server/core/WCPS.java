/*
 * This file is part of %PACKAGENAME%.
 *
 * %PACKAGENAME% is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * %PACKAGENAME% is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with %PACKAGENAME%. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.%PACKAGENAME%.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package wcps.server.core;

import java.io.File;
import java.io.InputStream;
import java.io.Reader;
import java.io.StringReader;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Unmarshaller;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.xml.sax.*;
import org.odmg.Database;
import org.odmg.DBag;
import org.odmg.Implementation;
import org.odmg.ODMGException;
import org.odmg.OQLQuery;
import org.odmg.QueryException;
import org.odmg.Transaction;
import org.w3c.dom.*;
import org.xml.sax.SAXException;
import rasj.RasGMArray;
import rasj.RasImplementation;

// This is the WCPS entry point. Processing a ProcessCoverage request happens in two stages. The first stage is the various pcPrepare functions, where XML is parsed, metadata is loaded, and the request is translated into an instance of a ProcessCoverageRequest. This data structure, for the time being, is just a list of rasdaman queries (RasQuery). The second stage is pcExecute, which executes a ProcessCoverageRequests and returns data.

public class WCPS {

    private static final String SCHEMA_PACKAGE_PROCESSCOVERAGE = "wcps.xml.processcoverage";

    private MetadataSource metadataSource;
    private DocumentBuilder wcpsDocumentBuilder; 

    public WCPS( File pcSchema, MetadataSource metadataSource ) throws  WCPSException {
	try {
		DocumentBuilderFactory dbconfig = DocumentBuilderFactory.newInstance();
		dbconfig.setValidating(false); // use XML schema not DTD
		dbconfig.setIgnoringComments(true); // comments are not relevant
		dbconfig.setIgnoringElementContentWhitespace(true);  // remve the ignorable whitespace
		Schema wcpsProcessCoverageSchema = SchemaFactory.newInstance( XMLConstants.W3C_XML_SCHEMA_NS_URI ).newSchema( pcSchema );
		dbconfig.setSchema(wcpsProcessCoverageSchema); 
		wcpsDocumentBuilder = dbconfig.newDocumentBuilder();
	} catch (Exception e) {
		throw new WCPSException("Error while loading the document builder interface!", e);
	}

	this.metadataSource = metadataSource;
    }

    public List<byte[]> pcExecute( String url, String database, ProcessCoverageRequest pcRequest ) throws ResourceException {
	throw new ResourceException("Mothod not implemented! pcExecute");
/*        List<RasQuery> queries = pcRequest.getQueries();
        List<byte[]> results = new ArrayList<byte[]>( queries.size() );
        synchronized( this ) {
            Implementation impl = new RasImplementation( url );
            Database db = impl.newDatabase();
            try {
                db.open( database, Database.OPEN_READ_ONLY );
            }
            catch( ODMGException odmge ) {
                try {
                    db.close();
                }
                catch (ODMGException e) {}
                throw new ResourceException( "Could not connect to rasdaman at " + url + ", database " + database, odmge );
            }
            Transaction tr = impl.newTransaction();
            tr.begin();
            Iterator<RasQuery> queryIterator = queries.iterator();
            while( queryIterator.hasNext() ) {
                String query = queryIterator.next().toString();
                OQLQuery q = impl.newOQLQuery();
                DBag resultSet;
                try {
                    q.create( query );
                    resultSet = (DBag) q.execute();
                    if( resultSet != null ) {
                        Iterator resultIterator = resultSet.iterator();
                        while( resultIterator.hasNext() ) {
                            RasGMArray result = (RasGMArray) resultIterator.next();
                            results.add( result.getArray() );
                        } 
                    }
                }
                catch (QueryException qe) {
                    tr.commit();
                    try {
                        db.close();
                    }
                    catch (ODMGException odmge) {}
                    throw new ResourceException ( "Could not evaluate rasdaman query: '" + query + "'", qe );
                }
            }
            tr.commit();
            try {
                db.close();
            }
            catch (ODMGException odmge) {}
        }
        return results;
*/
    }

    public ProcessCoverageRequest pcPrepare(String url, String database, File f) throws WCPSException, InvalidRequestException, ResourceException, SAXException, IOException {
	return pcPrepare(url, database, wcpsDocumentBuilder.parse(f));	
    }

    public ProcessCoverageRequest pcPrepare(String url, String database, InputStream is, String systemId ) throws WCPSException, InvalidRequestException, ResourceException, SAXException, IOException {
	return pcPrepare(url, database, wcpsDocumentBuilder.parse(is, systemId));	
    }
    
    public ProcessCoverageRequest pcPrepare(String url, String database, String uri) throws  WCPSException, InvalidRequestException, ResourceException, SAXException, IOException {
	return pcPrepare(url, database, wcpsDocumentBuilder.parse(uri));	
    }
    
    public ProcessCoverageRequest pcPrepare(String url, String database, InputSource is) throws WCPSException, InvalidRequestException, ResourceException, SAXException, IOException {
	return pcPrepare(url, database, wcpsDocumentBuilder.parse(is));	
    }

    private ProcessCoverageRequest pcPrepare(String url, String database, Document doc ) throws WCPSException, InvalidRequestException, ResourceException {
	return new ProcessCoverageRequest(url, database, doc, metadataSource);
    }

}
