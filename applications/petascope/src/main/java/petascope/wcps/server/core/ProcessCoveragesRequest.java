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

import petascope.exceptions.PetascopeException;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCPSException;
import java.io.IOException;
import java.io.StringReader;
import org.w3c.dom.*;
import org.xml.sax.SAXException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.xml.sax.InputSource;
import petascope.exceptions.ExceptionCode;
import petascope.util.ras.RasUtil;
import petascope.core.IDynamicMetadataSource;

/** A WCPS ProcessCoveragesRequest request provides a (just one) rasdaman query, that it executes.
 *
 * Internally, it relies on XmlRequest, which computes the RasQL query.
 *
 */
public class ProcessCoveragesRequest {
    
    private static Logger log = LoggerFactory.getLogger(ProcessCoveragesRequest.class);

    private String database;
    private IDynamicMetadataSource source;
    private String url;
    private Wcps wcps;
    private String rasqlQuery;
    private String mime;
    private XmlQuery xmlQuery;

    public ProcessCoveragesRequest(String url, String database, Node node, IDynamicMetadataSource source, Wcps wcps)
            throws WCPSException, SAXException, IOException, PetascopeException {
        super();
        this.source = source;
        this.url = url;
        this.database = database;
        this.wcps = wcps;
        Node child = node.getFirstChild();
        this.rasqlQuery = null;

        if (child.getNodeName().equals("ProcessCoveragesRequest") == false) {
            throw new WCPSException("The document contains an unrecognized node : "
                    + child.getNodeName());
        }

        child = child.getFirstChild();
        while (child.getNodeName().equals("#text")) {
            child = child.getNextSibling();
        }

        if (child.getNodeName().equals("query") == false) {
            throw new WCPSException("Could not find node <query>: " + child.getNodeName());
        }

        // "child" is now the node <query>.
        Node queryNode = child.getFirstChild();
        while (queryNode.getNodeName().equals("#text")) {
            queryNode = queryNode.getNextSibling();
        }

        /**
         * The following code is essential. It handles the two cases:
         * 1) the xml contains an <xmlSyntax> request
         * 2) the xml contains an <abstractSyntax> request
         */
        if (queryNode.getNodeName().equals("xmlSyntax")) {
            log.debug("Found XML Syntax query");
            this.xmlQuery = new XmlQuery(this.source);
            xmlQuery.startParsing(queryNode);
        } else if (queryNode.getNodeName().equals("abstractSyntax")) {
            String abstractQuery = queryNode.getFirstChild().getNodeValue();
            log.debug("Found Abstract Syntax query: " + abstractQuery);
            String xmlString = RasUtil.abstractWCPStoXML(abstractQuery);
            InputSource xmlStringSource = new InputSource(new StringReader(xmlString));
            log.debug("Coverted the Abstract syntax query to an XML query:");
            log.debug("***********************************************");
            log.debug(xmlString);
            log.debug("***********************************************");
            ProcessCoveragesRequest newRequest = wcps.pcPrepare(url, database, xmlStringSource);
            this.xmlQuery = newRequest.getXmlRequestStructure();
        } else {
            throw new WCPSException("Error ! Unexpected node: " + queryNode.getNodeName());
        }

        // If everything went well, we now have a proper value for "xmlQuery"
        this.rasqlQuery = xmlQuery.toRasQL();
        log.debug("Final RasQL query: " + rasqlQuery);
        this.mime = xmlQuery.getMimeType();
    }

    public String getMime() {
        return mime;
    }

    private XmlQuery getXmlRequestStructure() {
        return xmlQuery;
    }

    public String getRasqlQuery() {
        return this.rasqlQuery;
    }

    public Object execute() throws WCPSException {
        try {
            return RasUtil.executeRasqlQuery(rasqlQuery);
        } catch (RasdamanException ex) {
            throw new WCPSException(ExceptionCode.ResourceError, "Could not evaluate rasdaman query: '"
                        + getRasqlQuery() + "'\n Cause: " + ex.getMessage(), ex);
        }
    }
}
