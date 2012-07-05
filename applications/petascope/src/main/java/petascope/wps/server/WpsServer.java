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
package petascope.wps.server;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.Map;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.antlr.runtime.RecognitionException;
import org.apache.commons.io.IOUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.wcps.server.core.ProcessCoveragesRequest;


import petascope.ConfigManager;
import petascope.exceptions.WCPSException;
import petascope.util.ras.RasUtil;

/**
 *
 * @author Michael Owonibi
 */
public class WpsServer {

    private static Logger LOG = LoggerFactory.getLogger(WpsServer.class);
    public String request = null;

    public WpsServer(HttpServletResponse httpResponse, HttpServletRequest httpRequest) throws URISyntaxException, IOException, WCPSException {
        Map requestMap = httpRequest.getParameterMap();
        if ((requestMap.containsKey("request") && httpRequest.getParameter("request").equalsIgnoreCase("GetCapabilities")) || (requestMap.containsKey("Request") && httpRequest.getParameter("Request").equalsIgnoreCase("GetCapabilities"))) {
            GetCapabilities(httpResponse);
        } else if ((requestMap.containsKey("version") && httpRequest.getParameter("version").equals("1.0.0")) || (requestMap.containsKey("Version") && httpRequest.getParameter("Version").equals("1.0.0"))) {
            if (((requestMap.containsKey("request") && httpRequest.getParameter("request").equalsIgnoreCase("DescribeProcess")) || (requestMap.containsKey("Request") && httpRequest.getParameter("Request").equalsIgnoreCase("DescribeProcess"))) && ((requestMap.containsKey("identifier") && httpRequest.getParameter("identifier").equalsIgnoreCase("ProcessCoverages")) || (requestMap.containsKey("Identifier") && httpRequest.getParameter("Identifier").equalsIgnoreCase("ProcessCoverages")))) {
                DescribeProcess(httpResponse);
            } else if (((requestMap.containsKey("request") && httpRequest.getParameter("request").equalsIgnoreCase("Execute")) || (requestMap.containsKey("Request") && httpRequest.getParameter("Request").equalsIgnoreCase("Execute"))) && ((requestMap.containsKey("identifier") && httpRequest.getParameter("identifier").equalsIgnoreCase("ProcessCoverages")) || (requestMap.containsKey("Identifier") && httpRequest.getParameter("Identifier").equalsIgnoreCase("ProcessCoverages")))) {
                String wpsRequest = httpRequest.getParameter("DataInputs");
                wpsRequest = wpsRequest.substring(1, (wpsRequest.length() - 1));
                int pos = -1;
                pos = wpsRequest.indexOf("=");
                if (wpsRequest.substring(0, pos).equalsIgnoreCase("WcpsAbstractSyntax")) {
                    wpsRequest = wpsRequest.substring(pos + 1, wpsRequest.length());
                    wpsRequest = RasUtil.abstractWCPStoXML(wpsRequest);
                    System.out.println("WPS  request is :" + wpsRequest);
                    request = wpsRequest;
                }
            }
        } else {
            System.out.println("Invalid Input");
        }

    }

    public void GetCapabilities(HttpServletResponse httpResponse) throws URISyntaxException, IOException {
        String GetCapabilitiesResponse = loadFile(ConfigManager.WPS_GET_CAPABILITIES_URI);
        LOG.info("Executing operation GetCapabilities...");
        System.out.println("GetCapabilities Response is " + GetCapabilitiesResponse);
        PrintWriter out;

        out = httpResponse.getWriter();
        httpResponse.setContentType("text/xml; charset=utf-8");
        out.write(GetCapabilitiesResponse);
        out.flush();
        out.close();

    }

    public void DescribeProcess(HttpServletResponse httpResponse) throws URISyntaxException, IOException {

        String DescribeProcessResponse = loadFile(ConfigManager.WPS_DESCRIBE_PROCESS_URI);
        LOG.info("Executing operation DescribeProcess...");
        System.out.println("DesrcibeProcess Document is " + DescribeProcessResponse);
        PrintWriter out;
        out = httpResponse.getWriter();
        httpResponse.setContentType("text/xml; charset=utf-8");
        out.write(DescribeProcessResponse);
        out.flush();
        out.close();
    }

    private String loadFile(URI fileUri) throws IOException {
        InputStream is = null;
        String contents = null;
        try {
            LOG.debug("Loading file: " + fileUri);
            File f = new File(fileUri);
            is = new FileInputStream(f);
            contents = IOUtils.toString(is);
        } finally {
            try {
                is.close();
            } catch (IOException ex) {
            }
        }
        return contents;
    }
}
