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
package petascope.wcs2;

import com.oreilly.servlet.MultipartResponse;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.ConfigManager;
import petascope.core.DbMetadataSource;
import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.PetascopeException;
import petascope.util.StringUtil;
import petascope.util.WcsUtil;
import petascope.wcs2.extensions.ExtensionsRegistry;
import petascope.wcs2.extensions.FormatExtension;
import petascope.wcs2.extensions.ProtocolExtension;
import petascope.wcs2.handlers.Response;

/** 
 * A servlet handling WCS 2.0 requests.
 * 
 * @author Dimitar Misev
 */
public class Wcs2Servlet extends HttpServlet {

    private static Logger log = LoggerFactory.getLogger(Wcs2Servlet.class);
    public static String LOCAL_SERVLET_ADDRESS = "http://localhost:8080/petascope/wcs2";
    // path to the default HTML response of the interface servlet
    private static final String USAGE_FILE = "/templates/wcs2-servlet.html";
    // String containing the HTML code for the default response
    private DbMetadataSource meta;
    private String usageMessage;

    @Override
    public void init() throws ServletException {

        String confDir = this.getServletContext().getInitParameter(ConfigManager.CONF_DIR);
        ConfigManager.getInstance(confDir);
        
        log.info("WCS 2.0 servlet starting");

        // Read servlet HTML usage message from disk
        try {
            usageMessage = FileUtils.readFileToString(new File(getServletContext().getRealPath(USAGE_FILE)));
        } catch (IOException e) {
            log.error("Error reading default servlet HTML response.", e);
            throw new ServletException("Error reading default servlet HTML response.", e);
        }

        try {
            meta = new DbMetadataSource(ConfigManager.METADATA_DRIVER, ConfigManager.METADATA_URL,
                    ConfigManager.METADATA_USER, ConfigManager.METADATA_PASS, false);
        } catch (Exception e) {
            log.error("Error initializing metadata database", e);
            throw new ServletException("Metadata database initialization error", e);
        }
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

    private void setServletURL(HttpServletRequest req) {
        ConfigManager.PETASCOPE_SERVLET_URL = req.getRequestURL().toString();
    }

    @Override
    public void doPost(HttpServletRequest req, HttpServletResponse res) {
        doGet(req, res);
    }

    @Override
    public void doGet(HttpServletRequest req, HttpServletResponse res) {
        setServletURL(req);
        meta.clearCache();

        String request = null;
        try {
            try {
                request = IOUtils.toString(req.getReader());

                log.trace("POST request length: " + req.getContentLength());
                log.trace("GET query string: " + req.getQueryString());
                log.trace("POST request body:\n" + request + "\n----------------------------------------\n");

                Map<String, String> params = buildParameterDictionary(request);
                if (params.containsKey("request")) {
                    request = params.get("request");
                }
                request = StringUtil.urldecode(request, req.getContentType());
                if (request == null || request.length() == 0) {
                    if (req.getQueryString() != null && req.getQueryString().length() > 0) {
                        request = req.getQueryString();
                    } else {
                        printUsage(res, request);
                        return;
                    }
                }
                
                log.debug("Petascope Request: \n------START REQUEST--------\n"
                        + request + "\n------END REQUEST------\n");

                handleWcs2Request(request, false, res);
            } catch (WCSException e) {
                throw e;
            } catch (Exception e) {
                log.error("Runtime error : {}", e.getMessage());
                throw new WCSException(ExceptionCode.RuntimeError,
                        "Runtime error while processing request", e);
            }
        } catch (WCSException e) {
            printError(res, request, e);
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

    private void printUsage(HttpServletResponse httpResponse,  String request) throws IOException {
        PrintWriter out = httpResponse.getWriter();
        httpResponse.setContentType("text/html");
        out.write(usageMessage);
        out.flush();
    }

    private void printError(HttpServletResponse response, String message, Exception e) {
        PrintWriter out;
        try {
            out = response.getWriter();
        } catch (IOException e1) {
            log.error("Could not print exception because of IO error.", e1);
            return;
        }

        log.error("Error stack trace:", e);
        if (e instanceof WCSException) {
            String output = WcsUtil.exceptionToXml((WCSException) e);
            response.setContentType("text/xml; charset=utf-8");
            out.println(output);
            out.close();
        } else {
            log.trace("setting response mimetype to text/html; charset=utf-8");
            response.setContentType("text/html; charset=utf-8");
            log.trace("returning the following error message.", e);
            log.trace("end of error message");

            out.println("<html><head><title>PetaScope</title></head><body>");
            out.println("<h1>An error has occured</h1>");
            out.println("<p>" + message + "</p>");
            out.println("<p>Stack trace:<br/><small>");
            e.printStackTrace(out);
            out.println("</small></p></body></html>");
            out.close();
            log.trace("done with error");
        }
    }
}
