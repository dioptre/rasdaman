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
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.bind.JAXBException;
import javax.xml.parsers.ParserConfigurationException;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import org.antlr.runtime.RecognitionException;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.xml.sax.SAXException;
import wcps.server.core.DbMetadataSource;
import wcps.server.core.InvalidRequestException;
import wcps.server.core.ProcessCoveragesRequest;
import wcps.server.core.ResourceException;
import wcps.server.core.WCPS;
import wcps.server.core.WCPSException;
import wcps.server.core.XmlQuery;
import wcs.server.core.WCSException;
import wcs.server.WcsServer;
import wcst.server.WcstServer;


/** This servlet is a unified entry-point for all the PetaScope services.
 *
 * @author Andrei Aiordachioaie
 */
public class PetascopeInterface extends HttpServlet
{
    // Truncate input strings to this size in exceptions
    public final int STRING_LENGTH_LIMIT = 20;
    
	private DbMetadataSource metadataSource;
	
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
    public void init() throws ServletException
    {
        // Initialize the singleton configuration manager. Now all classes can read the settings.
        String settingsPath = getServletContext().getRealPath(relativeSettingsPath);
        ConfigManager config = ConfigManager.getInstance(settingsPath);

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

        /* Initialize WCPS Service */
        try
        {
            log("WCPS: initializing metadata database");
			metadataSource =
				new DbMetadataSource(ConfigManager.METADATA_DRIVER,
						     ConfigManager.METADATA_URL,
						     ConfigManager.METADATA_USER,
						     ConfigManager.METADATA_PASS, false);

			log("WCPS: initializing WCPS core");
			wcps = new WCPS(metadataSource);

			log("WCPS: initialization complete");
        }
        catch (ParserConfigurationException e)
        {
            throw new ServletException("Fatal: WCPS initialization error", e);
        }
        catch (WCPSException e)
        {
            throw new ServletException("Fatal: WCPS initialization error", e);
        }
        
        /* Initialize WCS Service */
        try
        {
            log("WCS Initialization ...");
            wcs = new WcsServer(settingsPath, metadataSource);
            log("WCS: Initialization complete.");
        }
        catch (Exception e)
        {
            throw new ServletException("Fatal: WCS initialization error", e);
        }

        /* Initialize WCS-T Service */
        try
        {
            log("WCS-T: Initializing ...");
            wcst = new WcstServer(metadataSource);
            log("WCS-T: Initialization complete.");
        }
        catch (WCSException e)
        {
            throw new ServletException("Fatal: WCS-T initialization error", e);
        }
    }

    /* Write a log message in the tomcat file "catalina.out" */
    @Override
    public void log(String msg)
    {
        System.out.println(msg);
    }

    /* Respond to Post requests just like in the case of Get requests */
    @Override
    public void doPost(HttpServletRequest httpRequest, HttpServletResponse httpResponse)
    {
        /* Init the Petascope URL automatically, for GetCapabilities response */
        if (ConfigManager.PETASCOPE_SERVLET_URL == null)
            ConfigManager.PETASCOPE_SERVLET_URL = httpRequest.getRequestURL().toString();
        /* Treat POST requests just like GET requests */
        doGet(httpRequest, httpResponse);
    }

    /* Handle Get requests. This function delegates the request to the service
     specified in the request by the "service" parameter. */
    @Override
    public void doGet(HttpServletRequest httpRequest, HttpServletResponse httpResponse)
    {
        /* Init the Petascope URL automatically, for GetCapabilities response */
        if (ConfigManager.PETASCOPE_SERVLET_URL == null)
            ConfigManager.PETASCOPE_SERVLET_URL = httpRequest.getRequestURL().toString();

        /* List all available coverages */
        try {log("PetaScope coverages: " + metadataSource.coverages());}
        catch (ResourceException e){}

        /* Process the request */
        String request = httpRequest.getParameter("request");
            
        try
        {
            try
            {
                // Quick hack to preserve compatibility with previous client versions
                String request2 = httpRequest.getParameter("query");
                if (request2 != null)
                    request2 = ProcessCoveragesRequest.abstractQueryToXmlQuery(request2);
                if (request == null && request2 != null)
                    request = request2;

                if (request == null)
                {
                    printUsage(httpResponse, request);
                    return;
                }

               // delete the XML header info from the request
                if (request.startsWith("<?xml "))
                {
                    int endline = request.indexOf("\n");
                    request = request.substring(endline+1);
                }
                // Delete leading comment blocks
                request = request.trim();
                while (request.startsWith("<!--"))
                {
                    int end = request.indexOf("-->");
                    request = request.substring(end+4).trim();
                }
                
                /* The next request types are defined in the WcsServer standard, and
                 are common to all the PetaScope services. */
                if (request.startsWith("<GetCapabilities") || request.equals("GetCapabilities"))
                    handleGetCapabilities(request, httpResponse);
                else
                if (request.startsWith("<DescribeCoverage"))
                    handleDescribeCoverage(request, httpResponse);
                else

                if (request.startsWith("<GetCoverage"))
                    handleGetCoverage(request, httpResponse);
                else

                /* ProcessCoverages is defined in the WCPS extension to WcsServer */
                if (request.startsWith("<ProcessCoverages"))
                    handleProcessCoverages(request, httpResponse);
                else

                /* Transaction is defined in the WcsServer-T extension to WcsServer */
                if (request.startsWith("<Transaction"))
                    handleTransaction(request, httpResponse);
                else

                /* Print Error Message */
                    handleUnknownRequest(request, httpResponse);
            }
            catch (IOException e)
            {
                throw new WCSException("ServletConnectionError", e.getMessage(), e);
            }
            catch (RecognitionException e)
            {
                throw new WCSException("InvalidRequestString", e.getMessage(), e);
            }
        }
        catch (WCSException e)
        {
            printError(httpResponse, request, e);
        }
    }

    private void printUsage(HttpServletResponse httpResponse,
            String request) throws IOException
    {
        PrintWriter out = httpResponse.getWriter();
        httpResponse.setContentType("text/html");
        out.write(usageMessage);
        out.flush();
    }

    private void printError(HttpServletResponse response,
            String message, Exception e)
    {
        PrintWriter out;
        try
        {
            out = new PrintWriter(response.getOutputStream());
        }
        catch (IOException e1)
        {
            log(e1.getMessage());
            e1.printStackTrace();
            return;
        }
        
        log("error");
        e.printStackTrace();
        if (e instanceof WCSException)
        {
            // We can send an error report
            String output = exceptionToXml((WCSException) e);
            response.setContentType("text/xml; charset=utf-8");
            out.println(output);
            out.close();
        }
        else
        {
            log("setting response mimetype to text/html; charset=utf-8");
            response.setContentType("text/html; charset=utf-8");
            log("returning the following error message");
            e.printStackTrace(System.out);
            log("end of error message");

            out.println(
                "<html><head><title>PetaScope</title></head><body>");
            out.println("<h1>An error has occured</h1>");
            out.println("<p>" + message + "</p>");
            out.println("<p>Stack trace:<br/><small>");
            e.printStackTrace(out);
            out.println("</small></p></body></html>");
            out.close();
            log("done with error");
        }
		
    }
    
    private void handleUnknownRequest(String request, HttpServletResponse httpResponse)
    {
//        if (request.length() > STRING_LENGTH_LIMIT)
//            request = request.substring(0, STRING_LENGTH_LIMIT - 1) + "...";

        // finally
        request = "'" + request + "'";
        try
        {
            throw new WCSException("NoApplicableCode", "Could not understand request " + request);
        }
        catch (WCSException e)
        {
            printError(httpResponse, request, e);
        }
    }

    private String exceptionReportToXml(ExceptionReport report)
    {
        String output = null;
        try
        {
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
            log("Done marshalling Error Report.");
        }
        catch (JAXBException e2)
        {
            e2.printStackTrace();
            log("Error marshalling Exception Report.");
        }
        return output;
    }

    private String exceptionToXml(WCSException e)
    {
        return exceptionReportToXml(e.getReport());
    }

    private void handleGetCapabilities(String request, HttpServletResponse httpResponse) throws WCSException
    {
        String output = wcs.GetCapabilities(request);
        PrintWriter out;
        try
        {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        }
        catch (IOException e)
        {
            throw new WCSException("InputOutputError", e.getMessage(), e.getMessage());
        }
    }

    private void handleDescribeCoverage(String request, HttpServletResponse httpResponse) throws WCSException
    {
        String output = wcs.DescribeCoverage(request);
        PrintWriter out;
        try
        {
            out = httpResponse.getWriter();
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(output);
            out.flush();
            out.close();
        }
        catch (IOException e)
        {
            throw new WCSException("InputOutputError", e.getMessage(), e.getMessage());
        }
    }

    private void handleGetCoverage(String request, HttpServletResponse httpResponse) throws WCSException
    {
        String xmlRequest = wcs.GetCoverage(request, wcps);
        log("***************");
        log(xmlRequest);
        // Redirect the request to WCPS
        handleProcessCoverages(xmlRequest, httpResponse);
    }

    private void handleProcessCoverages(String xmlRequest, HttpServletResponse response) throws WCSException
    {
        OutputStream webOut = null;
        try
        {
            log("WCPS: received the following request:");
			log(xmlRequest);

			log("WCPS: preparing request");
			ProcessCoveragesRequest processCoverageRequest =
				wcps.pcPrepare(ConfigManager.RASDAMAN_URL, ConfigManager.RASDAMAN_DATABASE, 
                IOUtils.toInputStream(xmlRequest));

            String query = processCoverageRequest.getRasqlQuery();
            String mime = processCoverageRequest.getMime();

			log("Resulting RasQL query: [" + mime + "] " + query);

			log("WCPS: executing request");

			List<byte[]> results = processCoverageRequest.execute();

			log("WCPS: setting response mimetype to " + mime);
			response.setContentType(mime);
			log("WCPS: returning response");
			webOut = response.getOutputStream();
            if (results.size() > 0)
                webOut.write(results.get(0));
            else
                log("WCPS: Warning! No result returned from rasql query.");

            /* TODO: remove CCIP hack */
            if (ConfigManager.hack)
            {
                try
                {
                    String dir = getServletContext().getRealPath("/");
                    File f = new File(dir + "image.jpeg");

                    log("HACK: Writing image to: " + f.getAbsolutePath());
//                    if (f.exists())
                    {
                        OutputStream os = new DataOutputStream(new FileOutputStream(f, false));
                        os.write(results.get(0));
                        os.close();
                        log("HACK: Wrote image successfully !");
                    }
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
            }

			log("WCPS: done");
        }
        catch (InvalidRequestException e)
        {
            throw new WCSException("InternalWcpsError", e.getMessage(), e);
        }
        catch (ResourceException e)
        {
            throw new WCSException("InternalWcpsError", e.getMessage(), e);
        }
        catch (WCPSException e)
        {
            throw new WCSException("InternalWcpsError", e.getMessage(), e);
        }
        catch (SAXException e)
        {
            throw new WCSException("XmlInputNotValid", e.getMessage(), e);
        }
        catch (IOException e)
        {
            throw new WCSException("IOConnectionError", e.getMessage(), e);
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

    
    private void handleTransaction(String request, HttpServletResponse httpResponse) throws WCSException
    {
        try
        {
            String outputXml = wcst.Transaction(request);
            PrintWriter out = new PrintWriter(httpResponse.getOutputStream());
            httpResponse.setContentType("text/xml; charset=utf-8");
            out.write(outputXml);
            out.flush();
        }
        catch (IOException e)
        {
            throw new WCSException("InputOutputError", e.getMessage(), e);
        }
    }
}
