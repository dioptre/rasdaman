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
package petascope;

import com.oreilly.servlet.MultipartResponse;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.bind.JAXBException;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.apache.log4j.PropertyConfigurator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;
import petascope.core.DbMetadataSource;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCSTException;
import petascope.wcps.server.core.ProcessCoveragesRequest;
import petascope.wcps.server.core.WCPS;
import petascope.exceptions.WCPSException;
import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.PetascopeException;
import petascope.util.Pair;
import petascope.util.StringUtil;
import petascope.util.XMLUtil;
import petascope.wcs.server.WcsServer;
import petascope.wcs2.extensions.ExtensionsRegistry;
import petascope.wcs2.extensions.FormatExtension;
import petascope.wcs2.extensions.ProtocolExtension;
import petascope.wcs2.handlers.Response;
import petascope.wcs2.templates.Templates;
import petascope.wcs2.templates.WcsNamespaceContext;
import petascope.wcst.server.WcstServer;
import petascope.wps.server.WpsServer;

/** 
 * This servlet is a unified entry-point for all the PetaScope services.
 *
 * @author Andrei Aiordachioaie
 * @author Dimitar Misev
 */
public class PetascopeInterface extends HttpServlet {

    private static Logger log = LoggerFactory.getLogger(PetascopeInterface.class);
    
    public static String LOCAL_SERVLET_ADDRESS = "http://localhost:8080/PetaScope/earthlook";
    
    private DbMetadataSource meta;

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

    /* Initialize the various services: WCPS, WcsServer and WcsServer-T */
    @Override
    public void init() throws ServletException {
        log.info("Petascope {} starting", ConfigManager.PETASCOPE_VERSION);

        // Initialize the singleton configuration manager. Now all classes can read the settings.
        String settingsPath = getServletContext().getRealPath(relativeSettingsPath);
        ConfigManager config = ConfigManager.getInstance(settingsPath, getServletContext().
                getRealPath("/"));

        // Initialize the logging system
        PropertyConfigurator.configure(getServletContext().getRealPath("/log4j.properties"));

        // Read servlet HTML usage message from disk
        try {
            usageFilePath = getServletContext().getRealPath(usageFilePath);
            usageMessage = FileUtils.readFileToString(new File(usageFilePath));
        } catch (IOException e) {
            log.error("Could not read default servlet HTML response. Stack trace: {}", e);
            throw new ServletException("Could not read interface servlet HTML response", e);
        }

        /* Initialize WCPS Service */
        try {
            log.info("WCPS: initializing metadata database");
            meta =
                    new DbMetadataSource(ConfigManager.METADATA_DRIVER,
                    ConfigManager.METADATA_URL,
                    ConfigManager.METADATA_USER,
                    ConfigManager.METADATA_PASS, false);

            log.debug("WCPS: initializing WCPS core");
            wcps = new WCPS(meta);

            log.info("WCPS: initialization complete");
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new ServletException("Fatal: WCPS initialization error", e);
        }

        /* Initialize WCS Service */
        try {
            log.info("WCS Initialization ...");
            wcs = new WcsServer(settingsPath, meta);
            log.info("WCS: Initialization complete.");
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new ServletException("Fatal: WCS initialization error", e);
        }

        /* Initialize WCS-T Service */
        try {
            log.info("WCS-T: Initializing ...");
            wcst = new WcstServer(meta);
            log.info("WCS-T: Initialization complete.");
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new ServletException("Fatal: WCS-T initialization error", e);
        }

        /* Initialize XML parsing for request redirection */
        try {
            DocumentBuilderFactory domFactory = DocumentBuilderFactory.
                    newInstance();
            domFactory.setNamespaceAware(true);		// never forget this!
            builder = domFactory.newDocumentBuilder();
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new ServletException("Fatal: Error initializing XML parser", e);
        }

        log.info("-----------------------------------------------");
        log.info("        PetaScope {} successfully started         ", ConfigManager.PETASCOPE_VERSION);
        log.info("-----------------------------------------------");
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

    /* Respond to Post requests just like in the case of Get requests */
    @Override
    public void doPost(HttpServletRequest httpRequest, HttpServletResponse httpResponse) {
        /* Init the Petascope URL automatically, for GetCapabilities response */
        if (ConfigManager.PETASCOPE_SERVLET_URL == null) {
            ConfigManager.PETASCOPE_SERVLET_URL = httpRequest.getRequestURL().
                    toString();
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
            ConfigManager.PETASCOPE_SERVLET_URL = httpRequest.getRequestURL().
                    toString();
        }

        /* Process the request */
        try {
            try {
                requestBody = IOUtils.toString(httpRequest.getReader());

                log.trace("POST Request length: " + httpRequest.getContentLength());
                log.trace("POST request body: \n------START REQUEST--------\n"
                        + requestBody + "\n------END REQUEST------\n");
                log.trace("GET Query string: " + httpRequest.getQueryString());

                Map<String, String> params = buildParameterDictionary(requestBody);
                log.trace("Request parameters: {}", params);
                request = StringUtil.urldecode(params.get("request"), httpRequest.
                        getContentType());

                // WPS 1.0.0 GET interface processing
                String service = httpRequest.getParameter("service");
                if (service != null) {
                    if (service.equals("WPS")) {
                        WpsServer wpsServer = new WpsServer(httpResponse, httpRequest);
                        request = wpsServer.request;
                    } else if (service.equals("WCS")) {
                        handleWcs2Request(httpRequest.getQueryString(), false, httpResponse);
                        return;
                    }
                }

                // To preserve compatibility with previous client versions, we allow
                // GET requests with parameter "query"
                String request2 = null;
                request2 = httpRequest.getParameter("query");
                if (request2 == null) {
                    request2 = StringUtil.urldecode(params.get("query"), httpRequest.
                            getContentType());
                }
                if (request2 != null) {
                    log.debug("Received Abstract Syntax Request via GET: \n\t\t{}", request2);
                    request2 = ProcessCoveragesRequest.abstractQueryToXmlQuery(request2);
                }
                if (request == null && request2 != null) {
                    request = request2;
                }

                // Empty request ?
                if (request == null && (requestBody == null || requestBody.
                        length() == 0)) {
                    printUsage(httpResponse, request);
                    return;
                }

                // No parameters, just XML in the request body
                if (request == null && requestBody != null && requestBody.length() > 0) {
                    request = StringUtil.urldecode(requestBody, httpRequest.
                            getContentType());

//                    if (request.matches(" *<.*") == false)
//                    {
//                        handleUnknownRequest(request, httpResponse);
//                        return;
//                    }
                }

                log.debug("Petascope Request: \n------START REQUEST--------\n"
                        + request + "\n------END REQUEST------\n");

                if (XMLUtil.isFirstTag(request, "Envelope")) {
                    handleWcs2Request(request, true, httpResponse);
                    return;
                }

                Document doc = builder.parse(IOUtils.toInputStream(request));
                Element rootElem = doc.getDocumentElement();
                String root = rootElem.getTagName();
                log.debug("Root Element name: {}", root);

                String version = "2.0";
                try {
                    XPath xpath = xpathFactory.newXPath();
                    xpath.setNamespaceContext(new WcsNamespaceContext());
                    String query = "/*/@version";
                    version = (String) xpath.evaluate(query, doc, XPathConstants.STRING);
                } catch (XPathExpressionException e) {
                    log.warn("The root XML node has no attribute called \"version\"");
                }


                /* The next request types are defined in the WcsServer standard, and
                are common to all the PetaScope services. */
                log.trace("WCS version: {}", version);
                if (version.startsWith("1")) {
                    if (root.endsWith("GetCapabilities")) {
                        handleGetCapabilities(request, httpResponse);
                        return;
                    } else if (root.endsWith("DescribeCoverage")) {
                        handleDescribeCoverage(request, httpResponse);
                        return;
                    } else if (root.endsWith("GetCoverage")) {
                        handleGetCoverage(request, httpResponse);
                        return;
                    }
                } else {
                    handleWcs2Request(request, false, httpResponse);
                    return;
                }
                if (root.endsWith("ProcessCoveragesRequest")) {  /* ProcessCoverages is defined in the WCPS extension to WcsServer */
                    handleProcessCoverages(request, httpResponse);
                } else if (root.endsWith("Transaction")) { /* Transaction is defined in the WcsServer-T extension to WcsServer */
                    handleTransaction(request, httpResponse);
                } else { /* Print Error Message */
                    handleUnknownRequest(request, httpResponse);
                }
            } catch (WCSException e) {
                throw e;
            } catch (Exception e) {
                // Finally, cast all other exceptions into a WCSException
                log.error("Runtime error : {}", e.getMessage());
                throw new WCSException(ExceptionCode.RuntimeError,
                        "Runtime error while processing request", e);
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
            log.error("Could not print exception because of IO error. Stack trace:", e1);
            return;
        }

        log.error("Error stack trace:", e);
        if (e instanceof WCSException) {
            // We can send an error report
            String output = exceptionToXml((WCSException) e);
            response.setContentType("text/xml; charset=utf-8");
            out.println(output);
            out.close();
        } else {
            log.trace("setting response mimetype to text/html; charset=utf-8");
            response.setContentType("text/html; charset=utf-8");
            log.trace("returning the following error message.", e);
            log.trace("end of error message");

            out.println(
                    "<html><head><title>PetaScope</title></head><body>");
            out.println("<h1>An error has occured</h1>");
            out.println("<p>" + message + "</p>");
            out.println("<p>Stack trace:<br/><small>");
            e.printStackTrace(out);
            out.println("</small></p></body></html>");
            out.close();
            log.trace("done with error");
        }

    }

    private void handleUnknownRequest(String request, HttpServletResponse httpResponse) {
        request = "'" + request + "'";
        printError(httpResponse, request, new WCSException(
                ExceptionCode.NoApplicableCode, "Could not understand request " + request));
    }

    private String exceptionReportToXml(ExceptionReport report) {
        String output = null;
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.
                    newInstance(report.getClass().getPackage().getName());
            javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
            marshaller.setProperty("jaxb.formatted.output", true);
            marshaller.setProperty("jaxb.schemaLocation",
                    "http://www.opengis.net/ows http://schemas.opengis.net/ows/2.0/owsExceptionReport.xsd");
            marshaller.setProperty("com.sun.xml.bind.namespacePrefixMapper", new PetascopeXmlNamespaceMapper());
            StringWriter strWriter = new StringWriter();
            marshaller.marshal(report, strWriter);
            output = strWriter.toString();
            String sub = output.substring(output.indexOf("<ows:Exception "), output.indexOf("</ows:ExceptionReport>"));
            try {
                output = Templates.getTemplate(Templates.EXCEPTION_REPORT, Pair.of("\\{exception\\}", sub));
            } catch (Exception ex) {
                log.warn("Error handling exception report template");
            }
            log.debug("Done marshalling Error Report.");
        } catch (JAXBException e2) {
            log.error("Stack trace: {}", e2);
            log.error("Error stack trace: " + e2);
        }
        return output;
    }

    private String exceptionToXml(PetascopeException e) {
        return exceptionReportToXml(e.getReport());
    }

    /**
     * Handle WCS 2.0 request.
     * 
     * @param request request string
     * @param response
     * @throws WCSException in case of I/O error, or if the server is unable to handle the request
     */
    private void handleWcs2Request(String request, boolean soap, HttpServletResponse response)
            throws WCSException, PetascopeException {
        try {
            log.info("Handling WCS 2.0 request");
            ProtocolExtension pext = ExtensionsRegistry.getProtocolExtension(request);
            if (pext == null) {
                throw new WCSException(ExceptionCode.NoApplicableCode,
                        "No protocol binding extension that can handle this request was found ");
            }
            log.info("Protocol binding extension found: {}", pext.
                    getExtensionIdentifier());
            Response res = pext.handle(request, meta);
            
            OutputStream os = response.getOutputStream();
            if (res.getXml() != null && res.getData() != null) {
                MultipartResponse multi = new MultipartResponse(response);
                multi.startResponse(FormatExtension.MIME_GML);
                IOUtils.write(res.getXml(), os);
                multi.endResponse();
                multi.startResponse(res.getMimeType());
                IOUtils.write(res.getData(), os);
                multi.endResponse();
                multi.finish();
            } else {
                try {
                    if (res.getMimeType() != null) {
                        response.setContentType(res.getMimeType());
                    } else {
    //                    response.setContentType(WcsUtil.MIME_GML);
                        response.setContentType(FormatExtension.MIME_TEXT);
                    }
                    if (res.getXml() != null) {
                        IOUtils.write(res.getXml(), os);
                    } else if (res.getData() != null) {
                        IOUtils.write(res.getData(), os);
                    }
                } finally {
                    if (os != null) {
                        os.flush();
                        os.close();
                    }
                }
            }
        } catch (Exception ex) {
            if (!(ex instanceof PetascopeException)) {
                ex = new WCSException(ExceptionCode.RuntimeError, ex);
            }
            throw ((PetascopeException) ex);
        }
    }

    /**
     * GetCapabilities of WCS 1.1
     * @param request
     * @param httpResponse
     * @throws WCSException
     */
    private void handleGetCapabilities(String request, HttpServletResponse httpResponse)
            throws WCSException, PetascopeException {
        String output = wcs.GetCapabilities(request);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new WCSException(ExceptionCode.IOConnectionError, e.getMessage(), e);
        }
    }

    /**
     * DescribeCoverage for WCS 1.1
     * @param request
     * @param httpResponse
     * @throws WCSException
     */
    private void handleDescribeCoverage(String request, HttpServletResponse httpResponse)
            throws WCSException {
        String output = wcs.DescribeCoverage(request);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new WCSException(ExceptionCode.IOConnectionError, e.getMessage(), e);
        }
    }

    private void handleGetCoverage(String request, HttpServletResponse httpResponse)
            throws WCSException {
        String xmlRequest = wcs.GetCoverage(request, wcps);
        log.debug("Received GetCoverage Request: \n{}", xmlRequest);
        // Redirect the request to WCPS
        handleProcessCoverages(xmlRequest, httpResponse);
    }

    private void handleProcessCoverages(String xmlRequest, HttpServletResponse response)
            throws WCSException {
        OutputStream webOut = null;
        try {
            log.debug("Received a ProcessCoverages request: \n{}", xmlRequest);

            log.debug("WCPS: preparing request");
            ProcessCoveragesRequest processCoverageRequest =
                    wcps.pcPrepare(ConfigManager.RASDAMAN_URL, ConfigManager.RASDAMAN_DATABASE,
                    IOUtils.toInputStream(xmlRequest));

            String query = processCoverageRequest.getRasqlQuery();
            String mime = processCoverageRequest.getMime();

            log.debug("Resulting RasQL query: [{}] {}", mime, query);

            log.trace("WCPS: executing request");

            List<byte[]> results = processCoverageRequest.execute();

            log.debug("WCPS: setting response mimetype to " + mime);
            response.setContentType(mime);
            log.trace("WCPS: returning response");
            webOut = response.getOutputStream();
            if (results.size() > 0) {
                webOut.write(results.get(0));

                if (ConfigManager.CCIP_HACK == true) {
                    try {
                        String dir = getServletContext().getRealPath("/");
                        File f = new File(dir + "image.jpeg");

                        log.info("HACK: Writing image to: " + f.getAbsolutePath());
                        {
                            OutputStream os = new DataOutputStream(new FileOutputStream(f, false));
                            os.write(results.get(0));
                            os.close();
                            log.info("HACK: Wrote image successfully !");
                        }
                    } catch (Exception e) {
                        log.warn("Error while evaluating CCIP hack: '{}'", e.
                                getMessage());
                    }
                }
            } else {
                log.warn("WCPS: Warning! No result returned from rasql query.");
            }

            log.debug("WCPS: done");
        } catch (WCPSException e) {
            throw new WCSException(ExceptionCode.InternalComponentError, e.
                    getMessage(), e);
        } catch (SAXException e) {
            throw new WCSException(ExceptionCode.XmlNotValid, e.getMessage(), e);
        } catch (IOException e) {
            throw new WCSException(ExceptionCode.IOConnectionError, e.getMessage(), e);
        } finally {
            if (webOut != null) {
                try {
                    webOut.close();
                } catch (IOException e) {
                }
            }
        }
    }

    private void handleTransaction(String request, HttpServletResponse httpResponse)
            throws WCSTException, RasdamanException, WCPSException,
            PetascopeException {
        try {
            String outputXml = wcst.Transaction(request);
            PrintWriter out = new PrintWriter(httpResponse.getOutputStream());
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(outputXml);
            out.flush();
        } catch (IOException e) {
            throw new PetascopeException(ExceptionCode.IOConnectionError, e.
                    getMessage(), e);
        }
    }
}