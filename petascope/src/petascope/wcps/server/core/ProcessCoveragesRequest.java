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
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.ResourceException;
import petascope.wcps.server.exceptions.WCPSException;
import petascope.wcps.server.exceptions.InvalidWcpsRequestException;
import petascope.wcps.grammar.WCPSRequest;
import petascope.wcps.grammar.wcpsLexer;
import petascope.wcps.grammar.wcpsParser;
import petascope.wcps.grammar.wcpsParser.wcpsRequest_return;
import java.io.IOException;
import java.io.StringReader;
import org.antlr.runtime.RecognitionException;
import org.odmg.DBag;
import org.odmg.Database;
import org.odmg.Implementation;
import org.odmg.ODMGException;
import org.odmg.OQLQuery;
import org.odmg.QueryException;
import org.odmg.Transaction;

import org.w3c.dom.*;

import org.xml.sax.SAXException;
import rasj.RasGMArray;
import rasj.RasImplementation;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.antlr.runtime.ANTLRStringStream;
import org.antlr.runtime.CharStream;
import org.antlr.runtime.CommonTokenStream;
import org.xml.sax.InputSource;

/** A WCPS ProcessCoveragesRequest request provides a (just one) rasdaman query, that it executes.
 *
 * Internally, it relies on XmlRequest, which computes the RasQL query.
 *
 */
public class ProcessCoveragesRequest {

    private String database;
    private IDynamicMetadataSource source;
    private String url;
    private WCPS wcps;
    private String rasqlQuery;
    private String mime;
    private XmlQuery xmlQuery;

    public ProcessCoveragesRequest(String url, String database, Node node, IDynamicMetadataSource source, WCPS wcps)
            throws WCPSException, InvalidWcpsRequestException, ResourceException, SAXException, IOException, InvalidCrsException {
        super();
        this.source = source;
        this.url = url;
        this.database = database;
        this.wcps = wcps;
        Node child = node.getFirstChild();
        this.rasqlQuery = null;

        System.err.println("Parsing ProcessCoveragesRequest node: " + child.getNodeName());

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
            this.xmlQuery = new XmlQuery(this.source);
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

    public List<byte[]> execute() throws ResourceException {
        ArrayList<byte[]> results = new ArrayList<byte[]>();

        if (this.rasqlQuery != null) {
            Implementation impl = new RasImplementation(url);
            Database db = impl.newDatabase();

            try {
                db.open(database, Database.OPEN_READ_ONLY);
            } catch (ODMGException odmge) {
                try {
                    db.close();
                } catch (ODMGException e) {
                }

                throw new ResourceException("Could not connect to rasdaman at "
                        + url + ", database "
                        + database, odmge);
            }

            Transaction tr = impl.newTransaction();

            tr.begin();
            OQLQuery q = impl.newOQLQuery();
            DBag resultSet;

            try {
                q.create(this.getRasqlQuery());
                resultSet = (DBag) q.execute();

                if (resultSet != null) {
                    Iterator resultIterator = resultSet.iterator();

                    while (resultIterator.hasNext()) {
                        Object current = resultIterator.next();

                        try {
                            RasGMArray resultArray =
                                    (RasGMArray) current;

                            results.add(resultArray.getArray());
                        } catch (ClassCastException e) {    // not a RasGMarray
                            if (!mime.equals("text/plain")) {
                                throw new ResourceException(
                                        "Incompatible mime and data type!");
                            }

                            System.err.println("result="
                                    + current.toString());
                            results.add(current.toString().getBytes());

                        }


                        /*
                         *                   if (mime.equals("text/plain")) {
                         * System.err.println("dataType is :" + resultArray.getBaseTypeSchema().toString());
                         * }
                         */

                    }
                }
            } catch (QueryException qe) {
                tr.commit();

                try {
                    db.close();
                } catch (ODMGException odmge) {
                }

                throw new ResourceException("Could not evaluate rasdaman query: '"
                        + getRasqlQuery() + "'. Cause: " + qe.getMessage(), qe);
            }

            tr.commit();

            try {
                db.close();
            } catch (ODMGException odmge) {
            }
        }

        if (mime.equals("text/plain")) {
        }

        return results;

    }
}
