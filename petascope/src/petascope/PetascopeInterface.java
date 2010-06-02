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
package petascope;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.io.UnsupportedEncodingException;
import java.lang.String;
import java.net.URISyntaxException;
import java.net.URLDecoder;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.bind.JAXBException;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import org.antlr.runtime.RecognitionException;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.apache.log4j.PropertyConfigurator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;
import petascope.wcps.server.core.DbMetadataSource;
import petascope.wcps.server.core.ProcessCoveragesRequest;
import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.ResourceException;
import petascope.wcps.server.core.WCPS;
import petascope.wcps.server.exceptions.WCPSException;
import petascope.wcs.server.exceptions.WCSException;
import petascope.wcs.server.WcsServer;
import petascope.wcs.server.exceptions.InputOutputException;
import petascope.wcs.server.exceptions.InternalComponentException;
import petascope.wcs.server.exceptions.InvalidRequestException;
import petascope.wcs.server.exceptions.NoApplicableCodeException;
import petascope.wcs.server.exceptions.ServletConnectionException;
import petascope.wcs.server.exceptions.WcsRuntimeException;
import petascope.wcs.server.exceptions.XmlNotValidException;
import petascope.wcs2.server.Wcs2Server;
import petascope.wcs2.server.templates.WcsNamespaceContext;
import petascope.wcst.server.WcstServer;
import petascope.wps.server.WpsServer;

/** This servlet is a unified entry-point for all the PetaScope services.
 *
 * @author Andrei Aiordachioaie
 */
public class PetascopeInterface extends HttpServlet {

    private static Logger LOG = LoggerFactory.getLogger(PetascopeInterface.class);
    private DbMetadataSource metadataSource;

    /* Xml documents utils */
    DocumentBuilder builder = null;
    XPathFactory xpathFactory = XPathFactory.newInstance();
    /* Path to the settings file, in the web archive */
    private String relativeSettingsPath = "/settings.properties";
    // path to the default HTML response of the interface servlet
    private String usageFilePath = "/templates/interface-servlet.html";
    // String containing the HTML code for the default response
    private String usageMessage;
    /* Instance of WcsServer-T service */
    private WcstServer wcst;
    /* Instance of WCPS service */
    private WCPS wcps;
    /* Instance of WcsServer service */
    private WcsServer wcs;
    private Wcs2Server wcs2;

    /* Initialize the various services: WCPS, WcsServer and WcsServer-T */
    @Override
    public void init() throws ServletException {

        LOG.info("-----------------------------------------------");
        LOG.info("              PetaScope {} starting ...", ConfigManager.PETASCOPE_VERSION);
        LOG.info("-----------------------------------------------");

        // Initialize the singleton configuration manager. Now all classes can read the settings.
        String settingsPath = getServletContext().getRealPath(relativeSettingsPath);
        ConfigManager config = ConfigManager.getInstance(settingsPath, getServletContext().getRealPath("/"));

        // Initialize the logging system
        PropertyConfigurator.configure(getServletContext().getRealPath("/log4j.properties"));


        // Read servlet HTML usage message from disk
        try {
            usageFilePath = getServletContext().getRealPath(usageFilePath);
            usageMessage = FileUtils.readFileToString(new File(usageFilePath));
        } catch (IOException e) {
            LOG.error("Could not read default servlet HTML response. Stack trace: {}", e);
            throw new ServletException("Could not read interface servlet HTML response", e);
        }

        /* Initialize WCPS Service */
        try {
            LOG.info("WCPS: initializing metadata database");
            metadataSource =
                    new DbMetadataSource(ConfigManager.METADATA_DRIVER,
                    ConfigManager.METADATA_URL,
                    ConfigManager.METADATA_USER,
                    ConfigManager.METADATA_PASS, false);

            LOG.debug("WCPS: initializing WCPS core");
            wcps = new WCPS(metadataSource);

            LOG.info("WCPS: initialization complete");
        } catch (ParserConfigurationException e) {
            LOG.error("Stack trace: {}", e);
            throw new ServletException("Fatal: WCPS initialization error", e);
        } catch (WCPSException e) {
            LOG.error("Stack trace: {}", e);
            throw new ServletException("Fatal: WCPS initialization error", e);
        }

        /* Initialize WCS Service */
        try {
            LOG.info("WCS Initialization ...");
            wcs = new WcsServer(settingsPath, metadataSource);
            LOG.info("WCS: Initialization complete.");
        } catch (Exception e) {
            LOG.error("Stack trace: {}", e);
            throw new ServletException("Fatal: WCS initialization error", e);
        }

        /* Initialize WCS 2.0 Service */
        try {
            LOG.info("WCS 2.0 Initialization ...");
            wcs2 = new Wcs2Server(settingsPath, metadataSource);
            LOG.info("WCS 2.0: Initialization complete.");
        } catch (Exception e) {
            LOG.error("Stack trace: {}", e);
            throw new ServletException("Fatal: WCS 2.0 initialization error", e);
        }

        /* Initialize WCS-T Service */
        try {
            LOG.info("WCS-T: Initializing ...");
            wcst = new WcstServer(metadataSource);
            LOG.info("WCS-T: Initialization complete.");
        } catch (WCSException e) {
            LOG.error("Stack trace: {}", e);
            throw new ServletException("Fatal: WCS-T initialization error", e);
        }

        /* Initialize XML parsing for request redirection */
        try {
            DocumentBuilderFactory domFactory = DocumentBuilderFactory.newInstance();
            domFactory.setNamespaceAware(true);		// never forget this!
            builder = domFactory.newDocumentBuilder();
        } catch (Exception e) {
            LOG.error("Stack trace: {}", e);
            throw new ServletException("Fatal: Error initializing XML parser", e);
        }

        LOG.info("-----------------------------------------------");
        LOG.info("        PetaScope {} successfully started         ", ConfigManager.PETASCOPE_VERSION);
        LOG.info("-----------------------------------------------");
    }

    /* Build a dictionary of parameter names and values, given a request string */
    private Map<String, String> buildParameterDictionary(String request) {
        HashMap<String, String> map = new HashMap<String, String>(3);
        if (request == null) {
            return map;
        }

        String[] pairs = request.split("&");
        String key = null, val = null;
        int pos = -1;
        for (int i = 0; i < pairs.length; i++) {
            pos = pairs[i].indexOf("=");
            if (pos != -1) {
                key = pairs[i].substring(0, pos);
                val = pairs[i].substring(pos + 1, pairs[i].length());
                map.put(key, val);
            }
        }

        return map;
    }

    /* URL-decode a string, if needed */
    private String urldecode(String encodedText, String contentType) throws UnsupportedEncodingException {
        if (encodedText == null) {
            return null;
        }
        String decoded = encodedText;
        LOG.trace("Found URL encoded text: {}", encodedText);
        if (contentType != null && contentType.equals("application/x-www-form-urlencoded") && encodedText.indexOf(" ") == -1) {
            decoded = URLDecoder.decode(encodedText, "UTF-8");
        }
        LOG.trace("Returning decoded text: {}", decoded);
        return decoded;
    }

    /* Respond to Post requests just like in the case of Get requests */
    @Override
    public void doPost(HttpServletRequest httpRequest, HttpServletResponse httpResponse) {
        /* Init the Petascope URL automatically, for GetCapabilities response */
        if (ConfigManager.PETASCOPE_SERVLET_URL == null) {
            ConfigManager.PETASCOPE_SERVLET_URL = httpRequest.getRequestURL().toString();
        }
        /* Treat POST requests just like GET requests */
        doGet(httpRequest, httpResponse);
    }

    /* Handle Get requests. This function delegates the request to the service
    specified in the request by the "service" parameter. */
    @Override
    public void doGet(HttpServletRequest httpRequest, HttpServletResponse httpResponse) {
        String request = null, requestBody = null;

        /* Init the Petascope URL automatically, for GetCapabilities response */
        if (ConfigManager.PETASCOPE_SERVLET_URL == null) {
            ConfigManager.PETASCOPE_SERVLET_URL = httpRequest.getRequestURL().toString();
        }

        /* List all available coverages, to make sure metadata is available */
        try {
            LOG.debug("PetaScope coverages: " + metadataSource.coverages());
        } catch (ResourceException e) {
        }

        /* Process the request */

        try {
            try {
                requestBody = IOUtils.toString(httpRequest.getReader());

                LOG.trace("POST Request length: " + httpRequest.getContentLength());
                LOG.trace("POST request body: \n------START REQUEST--------\n"
                        + requestBody + "\n------END REQUEST------\n");

                Map<String, String> params = buildParameterDictionary(requestBody);
                LOG.trace("Request parameters: {}", params);
                request = urldecode(params.get("request"), httpRequest.getContentType());

                // WPS 1.0.0 GET interface processing
                if ((httpRequest.getParameter("Service") != null) && (httpRequest.getParameter("Service").equalsIgnoreCase("WPS"))) {
                    WpsServer wpsServer = new WpsServer(httpResponse, httpRequest);
                    request = wpsServer.request;
                }

                // To preserve compatibility with previous client versions, we allow
                // GET requests with parameter "query"
                String request2 = null;
                request2 = httpRequest.getParameter("query");
                if (request2 == null) {
                    request2 = urldecode(params.get("query"), httpRequest.getContentType());
                }
                if (request2 != null) {
                    LOG.debug("Received Abstract Syntax Request via GET: \n\t\t{}", request2);
                    request2 = ProcessCoveragesRequest.abstractQueryToXmlQuery(request2);
                }
                if (request == null && request2 != null) {
                    request = request2;
                }

                // Empty request ?
                if (request == null && (requestBody == null || requestBody.length() == 0)) {
                    printUsage(httpResponse, request);
                    return;
                }

                // No parameters, just XML in the request body
                if (request == null && requestBody != null && requestBody.length() > 0) {
                    request = urldecode(requestBody, httpRequest.getContentType());

//                    if (request.matches(" *<.*") == false)
//                    {
//                        handleUnknownRequest(request, httpResponse);
//                        return;
//                    }
                }

                LOG.debug("Petascope Request: \n------START REQUEST--------\n"
                        + request + "\n------END REQUEST------\n");

                Document doc = builder.parse(IOUtils.toInputStream(request));
                Element rootElem = doc.getDocumentElement();
                String root = rootElem.getTagName();
                LOG.debug("Root Element name: {}", root);

                String version = "1.1.0";
                try {
                    XPath xpath = xpathFactory.newXPath();
                    xpath.setNamespaceContext(new WcsNamespaceContext());
                    String query = "/*/@version";
                    version = (String) xpath.evaluate(query, doc, XPathConstants.STRING);
                } catch (XPathExpressionException e) {
                    LOG.warn("The root XML node has no attribute called \"version\"");
                }


                /* The next request types are defined in the WcsServer standard, and
                are common to all the PetaScope services. */
                if (root.endsWith("GetCapabilities")) {
                    if (version.startsWith("2")) {
                        handleGetCapabilities2(request, httpResponse);
                    } else {
                        handleGetCapabilities(request, httpResponse);
                    }
                } else if (root.endsWith("DescribeCoverage")) {
                    if (version.startsWith("2")) {
                        handleDescribeCoverage2(request, httpResponse);
                    } else {
                        handleDescribeCoverage(request, httpResponse);
                    }
                } else if (root.endsWith("GetCoverage")) {
                    if (version.startsWith("2")) {
                        handleGetCoverage2(request, httpResponse);
                    } else {
                        handleGetCoverage(request, httpResponse);
                    }
                } else /* ProcessCoverages is defined in the WCPS extension to WcsServer */
                if (root.endsWith("ProcessCoveragesRequest")) {
                    handleProcessCoverages(request, httpResponse);
                } else /* Transaction is defined in the WcsServer-T extension to WcsServer */
                if (root.endsWith("Transaction")) {
                    handleTransaction(request, httpResponse);
                } else /* Print Error Message */ {
                    handleUnknownRequest(request, httpResponse);
                }
            } catch (IOException e) {
                throw new ServletConnectionException(e.getMessage(), e);
            } catch (RecognitionException e) {
                throw new InvalidRequestException(e.getMessage(), e);
            } catch (SAXException e) {
                throw new InvalidRequestException(e.getMessage(), e);
            } catch (WCSException e) {
                throw e;
            } catch (Exception e) {
                // Finally, cast all other exceptions into a WCSException
                LOG.error("Runtime error : {}", e.getMessage());
                throw new WcsRuntimeException(e.getMessage(), e);
            }
        } // And catch all WCSExceptions, to display to the client
        catch (WCSException e) {
            printError(httpResponse, request, e);
        }
    }

    private void printUsage(HttpServletResponse httpResponse,
            String request) throws IOException {
        PrintWriter out = httpResponse.getWriter();
        httpResponse.setContentType("text/html");
        out.write(usageMessage);
        out.flush();
    }

    private void printError(HttpServletResponse response,
            String message, Exception e) {
        PrintWriter out;
        try {
            out = new PrintWriter(response.getOutputStream());
        } catch (IOException e1) {
            LOG.error("Could not print exception because of IO error. Stack trace:", e1);
            return;
        }

        LOG.error("Error stack trace:", e);
        if (e instanceof WCSException) {
            // We can send an error report
            String output = exceptionToXml((WCSException) e);
            response.setContentType("text/xml; charset=utf-8");
            out.println(output);
            out.close();
        } else {
            LOG.trace("setting response mimetype to text/html; charset=utf-8");
            response.setContentType("text/html; charset=utf-8");
            LOG.trace("returning the following error message.", e);
            LOG.trace("end of error message");

            out.println(
                    "<html><head><title>PetaScope</title></head><body>");
            out.println("<h1>An error has occured</h1>");
            out.println("<p>" + message + "</p>");
            out.println("<p>Stack trace:<br/><small>");
            e.printStackTrace(out);
            out.println("</small></p></body></html>");
            out.close();
            LOG.trace("done with error");
        }

    }

    private void handleUnknownRequest(String request, HttpServletResponse httpResponse) {
        request = "'" + request + "'";
        WCSException e = new NoApplicableCodeException("Could not understand request " + request);
        printError(httpResponse, request, e);
    }

    private String exceptionReportToXml(ExceptionReport report) {
        String output = null;
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(report.getClass().getPackage().getName());
            javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
            marshaller.setProperty("jaxb.formatted.output", true);
            marshaller.setProperty("jaxb.schemaLocation",
                    "http://www.opengis.net/ows http://schemas.opengis.net/ows/1.0.0/owsExceptionReport.xsd");
            marshaller.setProperty("com.sun.xml.bind.namespacePrefixMapper", new PetascopeXmlNamespaceMapper());
            StringWriter strWriter = new StringWriter();
            marshaller.marshal(report, strWriter);
            output = strWriter.toString();
            System.err.println(output);
            LOG.debug("Done marshalling Error Report.");
        } catch (JAXBException e2) {
            LOG.error("Stack trace: {}", e2);
            LOG.error("Error stack trace: " + e2);
        }
        return output;
    }

    private String exceptionToXml(WCSException e) {
        return exceptionReportToXml(e.getReport());
    }

    /**
     * GetCapabilities of WCS 1.1
     * @param request
     * @param httpResponse
     * @throws WCSException
     */
    private void handleGetCapabilities(String request, HttpServletResponse httpResponse) throws WCSException {
        String output = wcs.GetCapabilities(request);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        }
    }

    /**
     * GetCapabilities of WCS 2.0
     * @param request
     * @param httpResponse
     * @throws WCSException
     */
    private void handleGetCapabilities2(String request, HttpServletResponse httpResponse) throws WCSException {
        String output = wcs2.GetCapabilities(request);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        }
    }

    /**
     * DescribeCoverage for WCS 1.1
     * @param request
     * @param httpResponse
     * @throws WCSException
     */
    private void handleDescribeCoverage(String request, HttpServletResponse httpResponse) throws WCSException {
        String output = wcs.DescribeCoverage(request);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        }
    }

    /**
     * Describe Coverage for WCS 2.0
     * @param request
     * @param httpResponse
     * @throws WCSException
     */
    private void handleDescribeCoverage2(String request, HttpServletResponse httpResponse) throws WCSException {
        String output = wcs2.DescribeCoverage(request);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        }
    }

    private void handleGetCoverage(String request, HttpServletResponse httpResponse) throws WCSException, InvalidCrsException {
        String xmlRequest = wcs.GetCoverage(request, wcps);
        LOG.debug("Received GetCoverage Request: \n{}", xmlRequest);
        // Redirect the request to WCPS
        handleProcessCoverages(xmlRequest, httpResponse);
    }

    private void handleGetCoverage2(String request, HttpServletResponse httpResponse) throws WCSException {
        String output = wcs2.GetCoverage(request);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        }
    }

    private void handleProcessCoverages(String xmlRequest, HttpServletResponse response) throws WCSException, InvalidCrsException {
        OutputStream webOut = null;
        try {
            LOG.debug("Received a ProcessCoverages request: \n{}", xmlRequest);

            LOG.debug("WCPS: preparing request");
            ProcessCoveragesRequest processCoverageRequest =
                    wcps.pcPrepare(ConfigManager.RASDAMAN_URL, ConfigManager.RASDAMAN_DATABASE,
                    IOUtils.toInputStream(xmlRequest));

            String query = processCoverageRequest.getRasqlQuery();
            String mime = processCoverageRequest.getMime();

            LOG.debug("Resulting RasQL query: [{}] {}", mime, query);

            LOG.trace("WCPS: executing request");

            List<byte[]> results = processCoverageRequest.execute();

            LOG.debug("WCPS: setting response mimetype to " + mime);
            response.setContentType(mime);
            LOG.trace("WCPS: returning response");
            webOut = response.getOutputStream();
            if (results.size() > 0) {
                webOut.write(results.get(0));

                if (ConfigManager.CCIP_HACK == true) {
                    try {
                        String dir = getServletContext().getRealPath("/");
                        File f = new File(dir + "image.jpeg");

                        LOG.info("HACK: Writing image to: " + f.getAbsolutePath());
                        {
                            OutputStream os = new DataOutputStream(new FileOutputStream(f, false));
                            os.write(results.get(0));
                            os.close();
                            LOG.info("HACK: Wrote image successfully !");
                        }
                    } catch (Exception e) {
                        LOG.warn("Error while evaluating CCIP hack: '{}'", e.getMessage());
                    }
                }
            } else {
                LOG.warn("WCPS: Warning! No result returned from rasql query.");
            }

            LOG.debug("WCPS: done");
        } catch (WCPSException e) {
            throw new InternalComponentException(e.getMessage(), e);
        } catch (SAXException e) {
            throw new XmlNotValidException(e.getMessage(), e);
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        } finally {
            if (webOut != null) {
                try {
                    webOut.close();
                } catch (IOException e) {
                }
            }
        }
    }

    private void handleTransaction(String request, HttpServletResponse httpResponse) throws WCSException {
        try {
            String outputXml = wcst.Transaction(request);
            PrintWriter out = new PrintWriter(httpResponse.getOutputStream());
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(outputXml);
            out.flush();
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        }
    }
}
