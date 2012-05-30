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
import petascope.wcps.grammar.WCPSRequest;
import petascope.wcps.grammar.wcpsLexer;
import petascope.wcps.grammar.wcpsParser;
import petascope.wcps.grammar.wcpsParser.wcpsRequest_return;
import java.io.IOException;
import java.io.StringReader;
import org.antlr.runtime.RecognitionException;
import org.w3c.dom.*;
import org.xml.sax.SAXException;
import org.antlr.runtime.ANTLRStringStream;
import org.antlr.runtime.CharStream;
import org.antlr.runtime.CommonTokenStream;
import org.xml.sax.InputSource;
import petascope.core.DbMetadataSource;
import petascope.core.IMetadataSource;
import petascope.exceptions.ExceptionCode;
import petascope.util.ras.RasUtil;

/** A WCPS ProcessCoveragesRequest request provides a (just one) rasdaman query, that it executes.
 *
 * Internally, it relies on XmlRequest, which computes the RasQL query.
 *
 */
public class ProcessCoveragesRequest {

    private String database;
    private DbMetadataSource meta;
    private String url;
    private Wcps wcps;
    private String rasqlQuery;
    private String mime;
    private XmlQuery xmlQuery;

    public ProcessCoveragesRequest(String url, String database, Node node, IMetadataSource meta, Wcps wcps)
            throws WCPSException, SAXException, IOException, PetascopeException {
        super();
        this.meta = (DbMetadataSource) meta;
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
            System.err.println("Found XML Syntax query");
            this.xmlQuery = new XmlQuery(this.meta);
            xmlQuery.startParsing(queryNode);
        } else if (queryNode.getNodeName().equals("abstractSyntax")) {
            try {
                String abstractQuery = queryNode.getFirstChild().getNodeValue();
                System.err.println("Found Abstract Syntax query: " + abstractQuery);
                String xmlString = abstractQueryToXmlQuery(abstractQuery);
                InputSource xmlStringSource = new InputSource(new StringReader(xmlString));
                System.err.println("Coverted the Abstract syntax query to an XML query:");
                System.err.println("***********************************************");
                System.err.println(xmlString);
                System.err.println("***********************************************");
                ProcessCoveragesRequest newRequest = wcps.pcPrepare(url, database, xmlStringSource);
                this.xmlQuery = newRequest.getXmlRequestStructure();
            } catch (RecognitionException e) {
                throw new WCPSException("Abstract Syntax query is invalid: "
                        + e.getMessage());
            }
        } else {
            throw new WCPSException("Error ! Unexpected node: " + queryNode.getNodeName());
        }

        // If everything went well, we now have a proper value for "xmlQuery"
        this.rasqlQuery = xmlQuery.toRasQL();
        System.err.println("Final RasQL query: " + rasqlQuery);
        this.mime = xmlQuery.getMimeType();
    }

    public static String abstractQueryToXmlQuery(String abstractQuery) throws RecognitionException {
        CharStream cs = new ANTLRStringStream(abstractQuery);
        wcpsLexer lexer = new wcpsLexer(cs);
        CommonTokenStream tokens = new CommonTokenStream();
        tokens.setTokenSource(lexer);
        wcpsParser parser = new wcpsParser(tokens);
        wcpsRequest_return rrequest = parser.wcpsRequest();
        WCPSRequest request = rrequest.value;
        String xmlRequest = request.toXML();

        return xmlRequest;
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
