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
package petascope.wcps.server.core;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import petascope.core.IMetadataSource;
import petascope.core.DynamicMetadataSource;
import petascope.core.IDynamicMetadataSource;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.PetascopeException;
import petascope.exceptions.WCPSException;

/**
 * This is the WCPS entry point. Processing a ProcessCoverage request happens in two stages.
The first stage is the various pcPrepare functions, where XML is parsed, metadata is loaded,
and the request is translated into an instance of a ProcessCoverageRequest. This data structure,
for the time being, is just a list of rasdaman queries (RasQuery). The second stage is pcExecute,
which executes a ProcessCoverageRequests and returns data.
 */
public class Wcps {
    
    private static Logger log = LoggerFactory.getLogger(Wcps.class);

    private DocumentBuilder wcpsDocumentBuilder;
    private IDynamicMetadataSource dynamicMetadataSource;

    public Wcps(File pcSchema, IMetadataSource metadataSource) throws WCPSException, PetascopeException {
        try {
            log.info("WCPS: Loading and parsing XML Schema ...");
            DocumentBuilderFactory dbconfig = DocumentBuilderFactory.newInstance();

            dbconfig.setValidating(false);    // use XML schema not DTD
            dbconfig.setIgnoringComments(true);    // comments are not relevant
            dbconfig.setIgnoringElementContentWhitespace(true);    // remve the ignorable whitespace

            wcpsDocumentBuilder = dbconfig.newDocumentBuilder();
            log.info("WCPS: Finished loading the schema !");
            
            this.dynamicMetadataSource = new DynamicMetadataSource(metadataSource);
            
        } catch (Exception e) {
            throw new WCPSException(
                    "Error while loading the document builder interface!", e);
        }
    }

    public Wcps(IMetadataSource metadataSource) throws ParserConfigurationException, PetascopeException {
        this.dynamicMetadataSource = new DynamicMetadataSource(metadataSource);
        wcpsDocumentBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
    }

    public List<byte[]> pcExecute(String url, String database, ProcessCoveragesRequest pcRequest) throws WCPSException {
        throw new WCPSException(ExceptionCode.ResourceError, "Method not implemented! pcExecute");
    }

    public ProcessCoveragesRequest pcPrepare(String url, String database, File f)
            throws WCPSException, SAXException,
            IOException {
        return pcPrepare(url, database, wcpsDocumentBuilder.parse(f));
    }

    public ProcessCoveragesRequest pcPrepare(String url, String database, InputStream is,
            String systemId)
            throws WCPSException, SAXException,
            IOException {
        return pcPrepare(url, database, wcpsDocumentBuilder.parse(is, systemId));
    }

    public ProcessCoveragesRequest pcPrepare(String url, String database, String uri)
            throws WCPSException, SAXException,
            IOException {
        return pcPrepare(url, database, wcpsDocumentBuilder.parse(uri));
    }

    public ProcessCoveragesRequest pcPrepare(String url, String database, InputSource is)
            throws WCPSException, SAXException,
            IOException {
        return pcPrepare(url, database, wcpsDocumentBuilder.parse(is));
    }

    public ProcessCoveragesRequest pcPrepare(String url, String database, InputStream is)
            throws WCPSException, SAXException,
            IOException {
        return pcPrepare(url, database, wcpsDocumentBuilder.parse(is));
    }

    private ProcessCoveragesRequest pcPrepare(String url, String database, Document doc)
            throws WCPSException, SAXException, IOException {
        try {
            return new ProcessCoveragesRequest(url, database, doc, dynamicMetadataSource, this);
        } catch (PetascopeException ex) {
            throw (WCPSException) ex;
        }
    }
}
