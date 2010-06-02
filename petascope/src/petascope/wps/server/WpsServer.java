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
package petascope.wps.server;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.URI;
import java.net.URISyntaxException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.antlr.runtime.RecognitionException;
import org.apache.commons.io.IOUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.wcps.server.core.ProcessCoveragesRequest;
import petascope.wcs.server.exceptions.InputOutputException;

/**
 *
 * @author Michael Owonibi
 */
public class WpsServer {

    private static Logger LOG = LoggerFactory.getLogger(WpsServer.class);
    public String request = null;

    public WpsServer(HttpServletResponse httpResponse, HttpServletRequest httpRequest) throws URISyntaxException, IOException, InputOutputException, RecognitionException {

        if ((httpRequest.getParameter("Request") != null) && httpRequest.getParameter("Request").equalsIgnoreCase("GetCapabilities")) {
            GetCapabilities(httpResponse);
        }

        if ((((httpRequest.getParameter("version") != null) && httpRequest.getParameter("version").equalsIgnoreCase("1.0.0")) || ((httpRequest.getParameter("Version") != null) && httpRequest.getParameter("Version").equalsIgnoreCase("1.0.0")))) {

            if ((httpRequest.getParameter("Request") != null) && httpRequest.getParameter("Request").equalsIgnoreCase("DescribeProcess") && (httpRequest.getParameter("identifier") != null) && httpRequest.getParameter("identifier").equalsIgnoreCase("ProcessCoverages")) {
                DescribeProcess(httpResponse);
            } else if ((httpRequest.getParameter("Request") != null) && httpRequest.getParameter("Request").equalsIgnoreCase("Execute") && (httpRequest.getParameter("identifier") != null) && httpRequest.getParameter("identifier").equalsIgnoreCase("ProcessCoverages")) {

                String wpsRequest = httpRequest.getParameter("DataInputs");
                wpsRequest = wpsRequest.substring(1, (wpsRequest.length() - 1));
                int pos = -1;
                pos = wpsRequest.indexOf("=");
                if (wpsRequest.substring(0, pos).equalsIgnoreCase("WcpsAbstractSyntax")) {
                    wpsRequest = wpsRequest.substring(pos + 1, wpsRequest.length());

                    wpsRequest = ProcessCoveragesRequest.abstractQueryToXmlQuery(wpsRequest);
                    System.out.println("WPS  request is :" + wpsRequest);
                    request = wpsRequest;
                }
            }
        }
    }

    public void GetCapabilities(HttpServletResponse httpResponse) throws URISyntaxException, IOException, InputOutputException {
        URI GetCapabilitiesURI = this.getClass().getResource("../templates/GetCapabilities.xml").toURI();
        String GetCapabilitiesResponse = loadFile(GetCapabilitiesURI);
        LOG.info("Executing operation GetCapabilities...");
        System.out.println("GetCapabilities Response is " + GetCapabilitiesResponse);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(GetCapabilitiesResponse);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        }
    }

    public void DescribeProcess(HttpServletResponse httpResponse) throws URISyntaxException, IOException, InputOutputException {
        URI DesribeProcessURI = this.getClass().getResource("../templates/DescribeProcess.xml").toURI();
        String DescribeProcessResponse = loadFile(DesribeProcessURI);
        LOG.info("Executing operation DescribeProcess...");
        System.out.println("DescribeProcess Document is " + DescribeProcessResponse);
        PrintWriter out;
        try {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(DescribeProcessResponse);
            out.flush();
            out.close();
        } catch (IOException e) {
            throw new InputOutputException(e.getMessage(), e);
        }
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
