package wcs.server.servlet;

import java.io.File;
import java.io.FileInputStream;
import org.apache.commons.io.FileUtils;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringBufferInputStream;
import java.io.StringReader;
import java.util.List;
import java.util.Properties;
import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse; 

import net.opengis.ows.v_1_0_0.ExceptionReport;
import org.xml.sax.InputSource;
import wcps.server.core.ProcessCoverageRequest;
import wcps.server.core.CachedMetadataSource;
import wcps.server.core.DbMetadataSource;
import wcps.server.core.WCPS;

/**
 *
 * @author Andrei Aiordachioaie
 */
public class newWcsServlet extends HttpServlet {

    private static final long serialVersionUID = 84786549L;

    private Properties dbParams;
    private DbMetadataSource metadataSource;
    private String rasdamanUrl;
    private String rasdamanDatabase;
    private String servletHtmlPath;     // path to the default HTML response of the servlet
    private String defaultHtmlResponse; // String containing the HTML code for the default response
    private WCPS wcps;
	private String metaDbPath;

    @Override
    public void init() throws ServletException {

        try {
            metaDbPath = getServletContext().getRealPath("/dbparams.properties");

            dbParams = new Properties();
            System.out.println ("WCPS: loading database properties");
            dbParams.load( new FileInputStream( metaDbPath ) );
            rasdamanUrl = dbParams.getProperty( "rasdaman_url" );
            rasdamanDatabase = dbParams.getProperty( "rasdaman_database" );

            servletHtmlPath = getServletContext().getRealPath("/misc/wcs-servlet-default.html");
            this.defaultHtmlResponse = FileUtils.readFileToString(new File(servletHtmlPath));

            System.out.println ("WCPS: initializing metadata database");
            metadataSource = new DbMetadataSource( dbParams.getProperty( "metadata_driver" ), dbParams.getProperty( "metadata_url" ), dbParams.getProperty( "metadata_user" ), dbParams.getProperty( "metadata_pass" ), false );

            System.out.println( "WCPS: initializing WCPS core" );
            wcps = new WCPS( new File( getServletContext().getRealPath( "/xml/ogc/wcps/1.0.1/wcpsProcessCoverage.xsd" ) ), new CachedMetadataSource( metadataSource ) );

            System.out.println( "WCPS: initialization complete" );
        }
        catch( Exception e ) {
            System.out.println( "WCPS: initialization error" );
            System.out.println( "WCPS: closing metadata database" );
            if (metadataSource != null) {
                metadataSource.close();
            }
            System.out.println( "WCPS: done with init error" );
            throw new ServletException( "WCPS initialization error", e );
        }

    }


    /**
     * Handles the HTTP <code>GET</code> method.
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException
    {
        System.out.println( "WCS: invoked with GET" );
        printUsage( response );
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException
    {
        // Select the operation
        int op = -1;
        if (request.getParameter("GetCapabilitiesXML") != null)
            op = 3;
        if (request.getParameter("DescribeCoverageXML") != null)
            op = 2;
        if (request.getParameter("GetCoverageXML") != null)
            op = 1;

        // Convert the WCS request into a WCPS request
        WCS wcs = new WCS(metaDbPath);
        try
        {
            // initialize WebService operation arguments here
            String inputXML, wcpsXml;
            PrintWriter out;
            switch (op)
            {
                case 1:
                    inputXML = request.getParameter("GetCoverageXML");
                    wcpsXml = wcs.GetCoverage(inputXML);
                    // Check if this result is an ExceptionReport
                    // Try reading the exception report
                    ExceptionReport report = new ExceptionReport();
                    boolean gotWcsException = false;
                    try
                    {
                        javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(ExceptionReport.class.getPackage().getName());
                        javax.xml.bind.Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
                        report = (ExceptionReport) unmarshaller.unmarshal(new StringReader(wcpsXml));
                        gotWcsException = true;
                    } catch (Exception e)
                    {
                        gotWcsException = false;
                    }

                    // If output is ProcessCOverage, send the WCPS xml to the WCPS servlet
                    if (gotWcsException == false)
                        response = processWcpsRequest(wcpsXml, response);
                    else
                    {
                        out = new PrintWriter(response.getOutputStream());
                        out.write(wcpsXml);
                        out.flush();
                    }
                    break;
                case 2:
                    inputXML = request.getParameter("DescribeCoverageXML");
                    wcpsXml = wcs.DescribeCoverage(inputXML);
                    out = new PrintWriter(response.getOutputStream());
                    out.write(wcpsXml);
                    out.flush();
                    break;
                case 3:
                    inputXML = request.getParameter("GetCapabilitiesXML");
                    wcpsXml = wcs.GetCapabilities(inputXML);
                    out = new PrintWriter(response.getOutputStream());
                    out.write(wcpsXml);
                    out.flush();
                    break;
                default:
                    throw new Exception("No valid operation specified !");
            }

        }
        catch (Exception e)
        {
            e.printStackTrace();
            throw new ServletException("WCS servlet error !");
        }
    }

    /**
     * Returns a short description of the servlet.
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Web Coverage Service @ Jacobs University";
    }


    private void printUsage( HttpServletResponse response ) throws IOException
    {

        System.out.println( "WCS: setting response mimetype to text/html; charset=utf-8" );
        System.out.println( "WPS: returning usage message" );
        response.setContentType( "text/html; charset=utf-8" );
        PrintWriter out = new PrintWriter( response.getOutputStream() );
        
        System.err.println(this.defaultHtmlResponse);
        out.println(defaultHtmlResponse);

        out.close();
        System.out.println( "WCPS: done nothing" );

    }

    /***
     * Sends a request to the WCPS service.
     * The source code is copied from the WCPS servlet, WCPService.java.
     * Make sure to update it if the WCPS code changes.
     */
    private HttpServletResponse processWcpsRequest(String xmlRequest, HttpServletResponse response) throws IOException, Exception
    {
        try
        {
            System.out.println( "WCPS: received the following request:" );
            System.out.println( xmlRequest );

            System.out.println( "WCPS: preparing request" );
            ProcessCoverageRequest processCoverageRequest = wcps.pcPrepare(this.rasdamanUrl, this.rasdamanDatabase, new InputSource(new StringBufferInputStream(xmlRequest)));
            System.out.println( "[" + processCoverageRequest.getMime() + "] " + processCoverageRequest.getQuery());

            String query = processCoverageRequest.getQuery();
            String mimetype = processCoverageRequest.getMime();
            System.out.println( "WCPS: executing request" );

            List<byte[]> results = processCoverageRequest.execute();

            System.out.println( "WCPS: setting response mimetype to " + mimetype );
            response.setContentType( mimetype );
            System.out.println( "WCPS: returning response" );
            ServletOutputStream webOut = response.getOutputStream();
            webOut.write( results.get( 0 ) );
            System.out.println( "WCPS: done" );
        }
        catch (Exception e)
        {
            System.out.println("WCPS Error !");
            PrintWriter webOut = new PrintWriter(response.getOutputStream());
            webOut.write("WCPS Processing error !\n");
            webOut.write("xmlRequest is:\n");
            webOut.write(xmlRequest);
            webOut.write("\n\n\n");
            e.printStackTrace(webOut);
            webOut.flush();
        }
        return response;
    }

}
