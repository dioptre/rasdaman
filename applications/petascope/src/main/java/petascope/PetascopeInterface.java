/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public License
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
import java.util.Map.Entry;
import java.util.Set;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.bind.JAXBException;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import nu.xom.Document;
import nu.xom.Element;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.apache.log4j.PropertyConfigurator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.xml.sax.SAXException;
import petascope.core.DbMetadataSource;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCSTException;
import petascope.wcps.server.core.ProcessCoveragesRequest;
import petascope.wcps.server.core.Wcps;
import petascope.exceptions.WCPSException;
import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.PetascopeException;
import petascope.util.ListUtil;
import petascope.util.Pair;
import petascope.util.StringUtil;
import petascope.util.XMLUtil;
import petascope.util.ras.RasQueryResult;
import petascope.util.ras.RasUtil;
import petascope.wcs.server.WcsServer;
import petascope.wcs2.extensions.ExtensionsRegistry;
import petascope.wcs2.extensions.FormatExtension;
import petascope.wcs2.extensions.ProtocolExtension;
import petascope.wcs2.handlers.RequestHandler;
import petascope.wcs2.handlers.Response;
import petascope.wcs2.templates.Templates;
import petascope.wcst.server.WcstServer;

/** 
 * This servlet is a unified entry-point for all the PetaScope services.
 *
 * @author Andrei Aiordachioaie
 * @author Dimitar Misev
 */
public class PetascopeInterface extends HttpServlet {

    private static final Logger log = LoggerFactory.getLogger(PetascopeInterface.class);
    private DbMetadataSource meta;

    // path to the default HTML response of the interface servlet
    private String usageFilePath = "/templates/interface-servlet.html";
    // String containing the HTML code for the default response
    private String usageMessage;
    /* Instance of WcsServer-T service */
    private WcstServer wcst;
    /* Instance of WCPS service */
    private Wcps wcps;
    /* Instance of WcsServer service */
    private WcsServer wcs;

    /* Initialize the various services: WCPS, WcsServer and WcsServer-T */
    @Override
    public void init() throws ServletException {
        // Initialize the logging system
        PropertyConfigurator.configure(getServletContext().getRealPath("/log4j.properties"));
        log.info("Petascope {} starting", ConfigManager.PETASCOPE_VERSION);
        
        // External libraries licensing issues
        log.info("This program contains software which is in the public domain:");
        log.info("date4j (C) 2002-2011, Hirondelle Systems");
        
        // Force GeoTools referencing libraries to X->Y ordered CRSs
        System.setProperty("org.geotools.referencing.forceXY", "true");

        // Initialize the singleton configuration manager. Now all classes can read the settings.
        ConfigManager.getInstance();

        // Read servlet HTML usage message from disk
        try {
            usageFilePath = getServletContext().getRealPath(usageFilePath);
            usageMessage = FileUtils.readFileToString(new File(usageFilePath));
        } catch (IOException e) {
            log.error("Could not read default servlet HTML response. Stack trace: {}", e);
            throw new ServletException("Could not read interface servlet HTML response", e);
        }

        try {
            log.info("Initializing metadata database");
            meta = new DbMetadataSource(ConfigManager.METADATA_DRIVER,
                    ConfigManager.METADATA_URL,
                    ConfigManager.METADATA_USER,
                    ConfigManager.METADATA_PASS, false);
            log.info("Metadata initialization complete.");
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new ServletException("Error initializing metadata database", e);
        }

        /* Initialize WCPS Service */
        try {
            log.info("WCPS: Initializing...");
            wcps = new Wcps(meta);
            log.info("WCPS: initialization complete.");
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new ServletException("WCPS initialization error", e);
        }

        /* Initialize WCS Service */
        try {
            log.info("WCS: Initialization ...");
            wcs = new WcsServer(meta);
            log.info("WCS: Initialization complete.");
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new ServletException("WCS initialization error", e);
        }

        /* Initialize WCS-T Service */
        try {
            log.info("WCS-T: Initializing ...");
            wcst = new WcstServer(meta);
            log.info("WCS-T: Initialization complete.");
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new ServletException("WCS-T initialization error", e);
        }

        log.info("-----------------------------------------------");
        log.info("      PetaScope {} successfully started      ", ConfigManager.PETASCOPE_VERSION);
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
                map.put(key.toLowerCase(), val);
            }
        }

        return map;
    }

    private void setServletURL(HttpServletRequest req) {
        ConfigManager.PETASCOPE_SERVLET_URL = req.getRequestURL().toString();
    }
    
    /**
     * @return a parameter map of the query string in lower case parameters
     */
    private Map<String, String> buildParameterMap(HttpServletRequest req) {
        Map<String, String> ret = new HashMap<String, String>();
        Set<Entry<String, String[]>> p = req.getParameterMap().entrySet();
        for (Entry<String, String[]> e : p) {
            ret.put(e.getKey().toLowerCase(), e.getValue()[0]);
        }
        return ret;
    }

    /* Respond to Post requests just like in the case of Get requests */
    @Override
    public void doPost(HttpServletRequest httpRequest, HttpServletResponse httpResponse) {
        doGet(httpRequest, httpResponse);
    }

    /* Handle Get requests. This function delegates the request to the service
    specified in the request by the "service" parameter. */
    @Override
    public void doGet(HttpServletRequest httpRequest, HttpServletResponse httpResponse) {
        setServletURL(httpRequest);
        meta.clearCache();
        String request = null, requestBody = null;

        /* Process the request */
        try {
            try {
                requestBody = IOUtils.toString(httpRequest.getReader());

                log.trace("POST Request length: {}", httpRequest.getContentLength());
                log.trace("POST request body  : \n------START REQUEST--------\n{}\n------END REQUEST------\n", requestBody);
                log.trace("GET Query string   : {}", httpRequest.getQueryString());

                Map<String, String> params = buildParameterDictionary(requestBody);
                Map<String, String> paramMap = buildParameterMap(httpRequest);
                log.trace("Request parameters : {}", params);
                request = StringUtil.urldecode(params.get("request"), httpRequest.getContentType());

                // GET interface processing
                String service = paramMap.get("service");
                if (service != null) {
                  // Removed WPS support as it doesn't work. Using 52n instead -- DM 2012-may-24
//                    if (service.equals("WPS")) {
//                        WpsServer wpsServer = new WpsServer(httpResponse, httpRequest);
//                        request = wpsServer.request;
//                    } else
                    if (service.equals("WCS")) {
                        // extract version
                        String version = null;
                        String operation = paramMap.get("request");
                        if (operation.equals(RequestHandler.GET_CAPABILITIES)) {
                            version = paramMap.get("acceptversions");
                            log.trace("acceptversions: " + version);
                            if (version == null) {
                                version = ConfigManager.WCS_DEFAULT_VERSION;
                            } else {
                                String[] versions = version.split(",");
                                version = "";
                                for (String v : versions) {
                                    if (ConfigManager.WCS_VERSIONS.contains(v) &&
                                            !v.startsWith("1")) { // the WCS 1.1 server doesn't support GET-KVP
                                        version = v;
                                        break;
                                    }
                                }
                            }
                        } else if (operation.equals(RequestHandler.DESCRIBE_COVERAGE) || operation.equals(RequestHandler.GET_COVERAGE)) {
                            version = paramMap.get("version");
                        }
                        
                        // handle request
                        handleWcsRequest(version, paramMap.get("request"),
                                httpRequest.getQueryString(), false, httpResponse);
                        return;
                    }
                }

                // To preserve compatibility with previous client versions, we allow
                // GET requests with parameter "query"
                String request2 = null;
                request2 = paramMap.get("query");
                if (request2 == null) {
                    request2 = StringUtil.urldecode(params.get("query"), httpRequest.getContentType());
                }
                if (request2 != null) {
                    log.debug("Received Abstract Syntax Request via GET: \n\t\t{}", request2);
                    request2 = RasUtil.abstractWCPStoXML(request2);
                }
                if (request == null && request2 != null) {
                    request = request2;
                }

                // Empty request ?
                if (request == null && (requestBody == null || requestBody.length() == 0)) {
                    if (paramMap.size() > 0) {
                        throw new WCSException(ExceptionCode.NoApplicableCode,
                                "Couldn't understand the recieved request, is the service attribute missing?");
                    } else {
                        printUsage(httpResponse, request);
                        return;
                    }
                }

                // No parameters, just XML in the request body
                if (request == null && requestBody != null && requestBody.length() > 0) {
                    request = StringUtil.urldecode(requestBody, httpRequest.getContentType());
                }

                log.debug("Petascope Request: \n------START REQUEST--------\n{}"
                        + "\n------END REQUEST------\n", request);

                String root = XMLUtil.getRootElementName(request);
                log.debug("Root Element name: {}", root);
                if (root == null) {
                    return;
                }
                if (root.equals("Envelope")) {
                    handleWcs2Request(request, true, httpResponse);
                    return;
                } else if (root.endsWith("ProcessCoveragesRequest")) {  /* ProcessCoverages is defined in the WCPS extension to WcsServer */
                    handleProcessCoverages(request, httpResponse);
                    return;
                } else if (root.endsWith("Transaction")) { /* Transaction is defined in the WcsServer-T extension to WcsServer */
                    handleTransaction(request, httpResponse);
                    return;
                } else if (root.equals(RequestHandler.GET_CAPABILITIES)) {
                    // extract the version that the client prefers
                    Document doc = XMLUtil.buildDocument(null, request);
                    String version = "";
                    List<Element> acceptVersions = XMLUtil.collectAll(doc.getRootElement(), "AcceptVersions");
                    if (!acceptVersions.isEmpty()) {
                        List<Element> versions = XMLUtil.collectAll(ListUtil.head(acceptVersions), "Version");
                        for (Element v : versions) {
                            String val = XMLUtil.getText(v);
                            if (val != null && ConfigManager.WCS_VERSIONS.contains(val)) {
                                version = val;
                                break;
                            }
                        }
                    } else {
                        version = ConfigManager.WCS_DEFAULT_VERSION;  // by default the latest supported by petascope
                    }
                    handleWcsRequest(version, root, request, true, httpResponse);
                } else if (root.equals(RequestHandler.DESCRIBE_COVERAGE) || root.equals(RequestHandler.GET_COVERAGE)) {
                    Document doc = XMLUtil.buildDocument(null, request);
                    String version = doc.getRootElement().getAttributeValue("version");
                    handleWcsRequest(version, root, request, true, httpResponse);
                } else {
                    // error
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

    private void printUsage(HttpServletResponse httpResponse, String request) throws IOException {
        PrintWriter out = httpResponse.getWriter();
        httpResponse.setContentType("text/html");
        out.write(usageMessage);
        out.flush();
    }

    private void printError(HttpServletResponse response, String message, Exception e) {
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
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(report.getClass().getPackage().getName());
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
     * Handle a WCS request.
     * @param version WCS version
     * @param operation WCS operation
     * @param request the actual request
     * @param response stream to which the result will be written
     * @throws WCSException
     * @throws PetascopeException 
     */
    private void handleWcsRequest(String version, String operation,
            String request, boolean soap, HttpServletResponse response) throws WCSException, PetascopeException {
        if (version == null) {
            throw new WCSException(ExceptionCode.InvalidRequest, "No WCS version specified.");
        }
        if (version.startsWith("2")) {
            handleWcs2Request(request, soap, response);
        } else if (version.startsWith("1")) {
            handleWcs1Request(operation, request, response);
        } else {
            throw new WCSException(ExceptionCode.VersionNegotiationFailed);
        }
    }

    /**
     * Handle WCS 1.1 request.
     * 
     * @param operation WCS operation
     * @param request request string
     * @param response
     * @throws WCSException in case of I/O error, or if the server is unable to handle the request
     */
    private void handleWcs1Request(String operation, String request,
            HttpServletResponse response) throws WCSException, PetascopeException {
        log.info("Handling WCS 1.1 request");
        
        // compute result
        String result = null;
        if (operation.endsWith("GetCapabilities")) {
            result = wcs.GetCapabilities(request);
        } else if (operation.endsWith("DescribeCoverage")) {
            result = wcs.DescribeCoverage(request);
        } else if (operation.endsWith("GetCoverage")) {
            String xmlRequest = wcs.GetCoverage(request, wcps);
            log.debug("Received GetCoverage Request: \n{}", xmlRequest);
            // redirect the request to WCPS
            handleProcessCoverages(xmlRequest, response);
            return; // the result has been written already so we return
        }
        
        // write result to output stream
        if (result != null) {
            PrintWriter out;
            try {
                out = response.getWriter();
                response.setContentType("text/xml; charset=utf-8");
                out.write(result);
                out.flush();
                out.close();
            } catch (IOException e) {
                throw new WCSException(ExceptionCode.IOConnectionError, e.getMessage(), e);
            }
        }
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
            log.info("Protocol binding extension found: {}", pext.getExtensionIdentifier());
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

    private void handleProcessCoverages(String xmlRequest, HttpServletResponse response)
            throws WCSException {
        OutputStream webOut = null;
        try {
            log.debug("Received a ProcessCoverages request: \n{}", xmlRequest);

            log.debug("-------------------------------------------------------");
            log.debug("Converting to rasql");
            ProcessCoveragesRequest processCoverageRequest =
                    wcps.pcPrepare(ConfigManager.RASDAMAN_URL, ConfigManager.RASDAMAN_DATABASE,
                    IOUtils.toInputStream(xmlRequest));
            log.debug("-------------------------------------------------------");

            String query = processCoverageRequest.getRasqlQuery();
            String mime = processCoverageRequest.getMime();
            
            response.setContentType(mime);
            webOut = response.getOutputStream();

            if (processCoverageRequest.isRasqlQuery()) {
                log.debug("executing request");
                log.debug("[" + mime + "] " + query);
                
                RasQueryResult res = new RasQueryResult(processCoverageRequest.execute());
                if (!res.getMdds().isEmpty() || !res.getScalars().isEmpty()) {
                    for (String s : res.getScalars()) {
                        webOut.write(s.getBytes());
                    }
                    for (byte[] bs : res.getMdds()) {
                        webOut.write(bs);

                        if (ConfigManager.CCIP_HACK == true) {
                            try {
                                String dir = getServletContext().getRealPath("/");
                                File f = new File(dir + "image.jpeg");

                                log.info("HACK: Writing image to: " + f.getAbsolutePath());
                                OutputStream os = new DataOutputStream(new FileOutputStream(f, false));
                                os.write(bs);
                                os.close();
                                log.info("HACK: Wrote image successfully !");
                            } catch (Exception e) {
                                log.warn("Error while evaluating CCIP hack: '{}'", e.getMessage());
                            }
                        }
                    }
                } else {
                    log.warn("WCPS: Warning! No result returned from rasql query.");
                }
            } else {
                log.debug("metadata result, no rasql to execute");
                webOut.write(query.getBytes());
            }

            log.debug("WCPS: done");
        } catch (WCPSException e) {
            throw new WCSException(ExceptionCode.InternalComponentError, e.getMessage(), e);
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
            throw new PetascopeException(ExceptionCode.IOConnectionError, e.getMessage(), e);
        }
    }
}
