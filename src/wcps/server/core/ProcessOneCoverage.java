/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package wcps.server.core;

import grammar.WCPSRequest;
import grammar.wcpsLexer;
import grammar.wcpsParser;
import grammar.wcpsParser.wcpsRequest_return;
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

/**
 *
 * @author Andrei
 */
class ProcessOneCoverage implements IRasNode
    {
        private IRasNode query;

        public IRasNode getQuery()
        {
            return query;
        }
        
        public ProcessOneCoverage(Node parent, ProcessCoveragesRequest pcr, WCPS wcps) throws WCPSException, InvalidRequestException, ResourceException, SAXException, IOException
        {
            /*
            System.err.println("Parsing ProcessOneCoverage: " + parent.getNodeName());
            Node node = parent.getFirstChild();
            while (node.getNodeName().equals("#text"))
                node = node.getNextSibling();
            System.err.println("Parsing One Coverage request: " + node.getNodeName());
            if (node.getNodeName().equals("xmlSyntax"))
            {
                System.err.println("Found XML Syntax query");
                query = new XmlQuery(node, pcr);
            }
            else
            if (node.getNodeName().equals("abstractSyntax"))
            {
                try
                {
                    String abstractQuery = node.getFirstChild().getNodeValue();
                    System.err.println("Found Abstract Syntax query: " + abstractQuery);
                    String xmlQuery = abstractToXml(abstractQuery);
                    InputSource source = new InputSource(new StringReader(xmlQuery));
                    System.err.println("Coverted the Abstract syntax query to an XML query:");
                    System.err.println("***********************************************");
                    System.err.println(xmlQuery);
                    System.err.println("***********************************************");
//                    ProcessCoveragesRequest newRequest = wcps.pcPrepare(url, database, source);
//                    ProcessOneCoverage oneCoverageRequest = (ProcessOneCoverage) newRequest.getProcessOneCoverage();
//                    query = oneCoverageRequest.getQuery();
//                    rasqlQuery = newRequest.getRasqlQuery();
//                    mime = newRequest.getMime();
                }
                catch (RecognitionException e)
                {
                    throw new WCPSException("Abstract Syntax query is invalid: "
                            + e.getMessage());
                }
            }
            else
                throw new WCPSException("Error ! Unexpected node: " + node.getNodeName());
             */
        }

        /*
        private String abstractToXml(String abstractQuery) throws RecognitionException
        {
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

         */
        public String toRasQL()
        {
            return query.toRasQL();
        }
         
    }