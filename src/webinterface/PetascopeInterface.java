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
package webinterface;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.List;
import java.util.Properties;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.xml.sax.InputSource;
import wcps.server.core.CachedMetadataSource;
import wcps.server.core.DbMetadataSource;
import wcps.server.core.ProcessCoveragesRequest;
import wcps.server.core.WCPS;
import wcps.server.core.WCPSException;
import wcst.server.ConfigManager;
import wcst.server.wcstServer;

/** This servlet is a unified entry-point for all the PetaScope services.
 *
 * @author Andrei Aiordachioaie
 */
public class PetascopeInterface extends HttpServlet
{
    // Truncate input strings to this size in exceptions
    public final int STRING_LENGTH_LIMIT = 20;
    
    private Properties settings;
	private DbMetadataSource metadataSource;
	private String rasdamanDatabase;
	private String rasdamanUrl;
	
    /* Path to the settings file, in the web archive */
    private String settingsFilePath = "/settings.properties";
    // path to the default HTML response of the interface servlet
    private String usageFilePath = "/templates/interface-servlet.html";
    // String containing the HTML code for the default response
    private String usageMessage;
    
    /* Instance of WCS-T service */
    private wcstServer server;
    /* Instance of WCPS service */
    private WCPS wcps;

    /* Initialize the various services: WCPS, WCS and WCS-T */
    public void init() throws ServletException
    {
        // Read global configuration files and options
        try
        {
            settings = new Properties();
            settings.load(new FileInputStream(getServletContext().getRealPath(settingsFilePath)));
            rasdamanUrl      = settings.getProperty("rasdaman_url");
			rasdamanDatabase = settings.getProperty("rasdaman_database");
        }
        catch (IOException e)
        {
            log("Fatal error: could not read settings file.");
            throw new ServletException("Fatal error: Could not read settings", e);
        }

        // Read servlet HTML usage message from disk
        try
        {
            usageFilePath = getServletContext().getRealPath(usageFilePath);
            usageMessage = FileUtils.readFileToString(new File(usageFilePath));
        }
        catch (IOException e)
        {
            log("Could not read default servlet HTML response.");
            throw new ServletException("Could not read interface servlet HTML response", e);
        }

        // Initialize WCPS
        try
        {
            log("WCPS: initializing metadata database");
			metadataSource =
				new DbMetadataSource(settings.getProperty("metadata_driver"),
						     settings.getProperty("metadata_url"),
						     settings.getProperty("metadata_user"),
						     settings.getProperty("metadata_pass"), false);

			log("WCPS: initializing WCPS core");
			wcps = new WCPS(
                        new File(getServletContext().getRealPath("/xml/ogc/wcps/1.0.0/wcpsProcessCoverages.xsd")),
                        new CachedMetadataSource(metadataSource));

			log("WCPS: initialization complete");
        }
        catch (WCPSException e)
        {
            log("Fatal: Initialization error");
        }

        // Initialize WCS-T
        // Initialize the configuration manager. Now all classes can read the settings.
        String settingsPath = getServletContext().getRealPath(settingsFilePath);
        ConfigManager config = ConfigManager.getInstance(settingsPath);

        // Initialize the WCS-T server with proper metadata
        server = new wcstServer(settingsPath);
    }

    /* Write a log message in the tomcat file "catalina.out" */
    public void log(String msg)
    {
        System.out.println(msg);
    }

    /* Respond to Post requests just like in the case of Get requests */
    public void doPost(HttpServletRequest httpRequest, HttpServletResponse httpResponse)
	    throws ServletException, IOException
    {
        doGet(httpRequest, httpResponse);
    }

    /* Handle Get requests. This function delegates the request to the service
     specified in the request by the "service" parameter. */
    public void doGet(HttpServletRequest httpRequest, HttpServletResponse httpResponse)
	    throws ServletException, IOException
    {
        String service = httpRequest.getParameter("service");
        String request = httpRequest.getParameter("request");
        if (service == null && request == null)
        {
            printUsage(httpResponse, service, request);
            return;
        }
        if (service == null || request == null)
        {
            handleErrorMessage(service, request, httpResponse);
            return;
        }

       // delete the first line from the request if needed
        if (request.startsWith("<?xml "))
        {
            int pos = request.indexOf("\n");
            request = request.substring(pos+1);
        }
        /* The next request types are defined in the WCS standard, and
         are common to all the PetaScope services. */
        if (request.startsWith("<GetCapabilities"))
            handleGetCapabilities(request, httpResponse);
        else
        if (request.startsWith("<DescribeCoverage"))
            handleDescribeCoverage(request, httpResponse);
        else

        if (request.startsWith("<GetCoverage"))
            handleGetCoverage(request, httpResponse);
        else

        /* ProcessCoverages is defined in the WCPS extension to WCS */
        if (service.equals("WCPS") && request.startsWith("<ProcessCoverages"))
            handleProcessCoverages(request, httpResponse);
        else

        /* Transaction is defined in the WCS-T extension to WCS */
        if (service.equals("WCS-T") && request.startsWith("<Transaction"))
            handleTransaction(request, httpResponse);
        else

        /* Print Error Message */
            handleErrorMessage(service, request, httpResponse);
    }

    private void printUsage(HttpServletResponse httpResponse,
            String service, String request) throws IOException
    {
        PrintWriter out = httpResponse.getWriter();
        out.write(usageMessage);
        out.flush();
    }

    private void printWcpsError(HttpServletResponse response,
            String message, Exception e) throws IOException
    {
        log("WCPS: error");
		log("WCPS: setting response mimetype to text/html; charset=utf-8");
		response.setContentType("text/html; charset=utf-8");
		log("WCPS: returning the following error message");
		e.printStackTrace(System.out);
		log("WCPS: end of error message");
		PrintWriter out = new PrintWriter(response.getOutputStream());

		out.println(
		    "<html><head><title>Web Coverage Processing Service</title></head><body>");
		out.println("<h1>An error has occured</h1>");
		out.println("<p>" + message + "</p>");
		out.println("<p>Stack trace:<br/><small>");
		e.printStackTrace(out);
		out.println("</small></p></body></html>");
		out.close();
		log("WCPS: done with error");
    }
    
    private void handleErrorMessage(String service, String request, HttpServletResponse httpResponse)
    {
        if (service == null && request != null)
            throw new UnsupportedOperationException("Please specify the service you want to query: WCS, WCPS, WCS-T.");
        if (service != null && request == null)
            throw new UnsupportedOperationException("Please specify the request to be executed.");
        if (request.length() > STRING_LENGTH_LIMIT)
            request = request.substring(0, STRING_LENGTH_LIMIT - 1) + "...";
        if (service.length() > STRING_LENGTH_LIMIT)
            service = service.substring(0, STRING_LENGTH_LIMIT - 1) + "...";
        if (service != null && !service.equalsIgnoreCase("WCS") && !service.equalsIgnoreCase("WCPS")
                 && !service.equalsIgnoreCase("WCS-T"))
            throw new UnsupportedOperationException("Could not recognize service '"
                + service + "'");

        // finally
        request = "'" + request + "'";
        service = "'" + service + "'";
        throw new UnsupportedOperationException("Could not recognize request " +
                "type for service " + service + ". Request is " + request);
    }

    private void handleGetCapabilities(String request, HttpServletResponse httpResponse)
    {
        throw new UnsupportedOperationException("WCS is not yet integrated in PetaScope.");
    }

    private void handleDescribeCoverage(String request, HttpServletResponse httpResponse)
    {
        throw new UnsupportedOperationException("WCS is not yet integrated in PetaScope.");
    }

    private void handleGetCoverage(String request, HttpServletResponse httpResponse)
    {
        throw new UnsupportedOperationException("WCS is not yet integrated in PetaScope.");
    }

    private void handleProcessCoverages(String xmlRequest, HttpServletResponse response) throws IOException
    {
        OutputStream webOut = null;
        try
        {
            webOut = response.getOutputStream();
            log("WCPS: received the following request:");
			log(xmlRequest);

			log("WCPS: preparing request");
			ProcessCoveragesRequest processCoverageRequest =
				wcps.pcPrepare(
				    rasdamanUrl, rasdamanDatabase,
				    new InputSource(IOUtils.toInputStream(xmlRequest)));

            

            String query = processCoverageRequest.getRasqlQuery();
            String mime = processCoverageRequest.getMime();

			log("[" + mime + "] " + query);

			log("Resulting RasQL query: " + query);

			log("WCPS: executing request");

			List<byte[]> results = processCoverageRequest.execute();

			log("WCPS: setting response mimetype to " + mime);
			response.setContentType(mime);
			log("WCPS: returning response");
			webOut = response.getOutputStream();
			webOut.write(results.get(0));
			log("WCPS: done");
        }
        catch (Exception e)
        {
            printWcpsError(response, "WCPS Error: " + e.getMessage(), e);
        }
        finally
        {
            if (webOut != null)
            {
                try
                {
                    webOut.close();
                }
                catch (IOException e) {}
            }
        }
    }

    private void handleTransaction(String request, HttpServletResponse httpResponse) throws IOException
    {
        String outputXml = server.Transaction(request);
        PrintWriter out = new PrintWriter(httpResponse.getOutputStream());
        out.write(outputXml);
        out.flush();
    }
}
