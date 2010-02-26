/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */


package petascope.wcps.server.core;


import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.ResourceException;
import petascope.wcps.server.exceptions.WCPSException;
import petascope.wcps.server.exceptions.InvalidWcpsRequestException;
import petascope.wcps.server.exceptions.InvalidMetadataException;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.*;

import org.xml.sax.*;
import org.xml.sax.SAXException;


import java.io.File;
import java.io.IOException;
import java.io.InputStream;


import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

/**
 * This is the WCPS entry point. Processing a ProcessCoverage request happens in two stages.
 The first stage is the various pcPrepare functions, where XML is parsed, metadata is loaded,
 and the request is translated into an instance of a ProcessCoverageRequest. This data structure,
 for the time being, is just a list of rasdaman queries (RasQuery). The second stage is pcExecute,
 which executes a ProcessCoverageRequests and returns data.
 */

public class WCPS
{
	private static final String SCHEMA_PACKAGE_PROCESSCOVERAGE = "wcps.xml.processcoverage";
	private IMetadataSource metadataSource;
    private IDynamicMetadataSource dynamicMetadataSource;
	private DocumentBuilder wcpsDocumentBuilder;

	public WCPS(File pcSchema, IMetadataSource metadataSource) throws WCPSException
	{
		try
		{
            System.out.println("WCPS: Loading and parsing XML Schema ...");
			DocumentBuilderFactory dbconfig = DocumentBuilderFactory.newInstance();

			dbconfig.setValidating(false);    // use XML schema not DTD
			dbconfig.setIgnoringComments(true);    // comments are not relevant
			dbconfig.setIgnoringElementContentWhitespace(true);    // remve the ignorable whitespace
//			Schema wcpsProcessCoverageSchema =
//				SchemaFactory.newInstance(
//				    XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(pcSchema);

//			dbconfig.setSchema(wcpsProcessCoverageSchema);

			wcpsDocumentBuilder = dbconfig.newDocumentBuilder();
            System.out.println("WCPS: Finished loading the schema !");
		}
		catch (Exception e)
		{
			throw new WCPSException(
			    "Error while loading the document builder interface!", e);
		}

        this.dynamicMetadataSource = new DynamicMetadataSource(metadataSource);
	}

    public WCPS(IMetadataSource metadataSource) throws ParserConfigurationException,
            ResourceException, InvalidMetadataException
	{
        this.dynamicMetadataSource = new DynamicMetadataSource(metadataSource);
        wcpsDocumentBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
	}

	public List<byte[]> pcExecute(String url, String database, ProcessCoveragesRequest pcRequest)
	    throws ResourceException
	{
		throw new ResourceException("Mothod not implemented! pcExecute");

		/*
		 *         List<RasQuery> queries = pcRequest.getQueries();
		 * List<byte[]> results = new ArrayList<byte[]>( queries.size() );
		 * synchronized( this ) {
		 * Implementation impl = new RasImplementation( url );
		 * Database db = impl.newDatabase();
		 * try {
		 * db.open( database, Database.OPEN_READ_ONLY );
		 * }
		 * catch( ODMGException odmge ) {
		 * try {
		 * db.close();
		 * }
		 * catch (ODMGException e) {}
		 * throw new ResourceException( "Could not connect to rasdaman at " + url + ", database " +
         *          database, odmge );
		 * }
		 * Transaction tr = impl.newTransaction();
		 * tr.begin();
		 * Iterator<RasQuery> queryIterator = queries.iterator();
		 * while( queryIterator.hasNext() ) {
		 * String query = queryIterator.next().toString();
		 * OQLQuery q = impl.newOQLQuery();
		 * DBag resultSet;
		 * try {
		 * q.create( query );
		 * resultSet = (DBag) q.execute();
		 * if( resultSet != null ) {
		 * Iterator resultIterator = resultSet.iterator();
		 * while( resultIterator.hasNext() ) {
		 * RasGMArray result = (RasGMArray) resultIterator.next();
		 * results.add( result.getArray() );
		 * }
		 * }
		 * }
		 * catch (QueryException qe) {
		 * tr.commit();
		 * try {
		 * db.close();
		 * }
		 * catch (ODMGException odmge) {}
		 * throw new ResourceException ( "Could not evaluate rasdaman query: '" + query + "'", qe );
		 * }
		 * }
		 * tr.commit();
		 * try {
		 * db.close();
		 * }
		 * catch (ODMGException odmge) {}
		 * }
		 * return results;
		 */
	}

	public ProcessCoveragesRequest pcPrepare(String url, String database, File f)
	    throws WCPSException, InvalidWcpsRequestException, ResourceException, SAXException,
	    IOException, InvalidCrsException
	{
		return pcPrepare(url, database, wcpsDocumentBuilder.parse(f));
	}

	public ProcessCoveragesRequest pcPrepare(String url, String database, InputStream is,
	    String systemId)
	    throws WCPSException, InvalidWcpsRequestException, ResourceException, SAXException,
	    IOException, InvalidCrsException
	{
		return pcPrepare(url, database, wcpsDocumentBuilder.parse(is, systemId));
	}

	public ProcessCoveragesRequest pcPrepare(String url, String database, String uri)
	    throws WCPSException, InvalidWcpsRequestException, ResourceException, SAXException,
	    IOException, InvalidCrsException
	{
		return pcPrepare(url, database, wcpsDocumentBuilder.parse(uri));
	}

	public ProcessCoveragesRequest pcPrepare(String url, String database, InputSource is)
	    throws WCPSException, InvalidWcpsRequestException, ResourceException, SAXException,
	    IOException, InvalidCrsException
	{
		return pcPrepare(url, database, wcpsDocumentBuilder.parse(is));
	}

    public ProcessCoveragesRequest pcPrepare(String url, String database, InputStream is)
	    throws WCPSException, InvalidWcpsRequestException, ResourceException, SAXException,
	    IOException, InvalidCrsException
	{
		return pcPrepare(url, database, wcpsDocumentBuilder.parse(is));
	}

	private ProcessCoveragesRequest pcPrepare(String url, String database, Document doc)
	    throws WCPSException, InvalidWcpsRequestException, ResourceException, SAXException, IOException, InvalidCrsException
	{
        ProcessCoveragesRequest req = new ProcessCoveragesRequest(url, database, doc, dynamicMetadataSource, this);
        return req;
	}
}
