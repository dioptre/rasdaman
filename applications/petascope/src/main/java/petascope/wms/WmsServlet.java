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
/*************************************************************
 * <pre>
 * WMS servlet
 * to be done:
 * <UL>
 * <LI>Request parameter names have to be upper case: make String.toLower and compare them then (this class and WmsRequest)</LI>
 * <LI>Transparent option throws an exception because I did not test if the images returned from rasdaman are transparent (disabled, to not distract clients)</LI>
 * <LI>Styles are not supported (RasQL should be integrated into the styles/the queryable interface should be supported)</LI>
 * <LI>There is a problem when requesting images that are not squares but rectangles (just happened to come accross one)</LI>
 * </UL>
 * @version $Revision: 1.8 $ * 
 *
 * COMMENTS:
 * - exception type BLANK and INIMAGE return only JPEG, FORMAT is ignored
 * - these exception types do not consider the corresp. rasocg.cfg settings
 * - look at FIXME for syncing!
 *
 * </pre>
 *********************************************************** */

package petascope.wms;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.odmg.DBag;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.ConfigManager;
import rasj.RasGMArray;
import petascope.util.ras.RasUtil;
import petascope.exceptions.WMSException;
import javax.imageio.ImageIO;

public class WmsServlet extends HttpServlet
{
    private static final Logger log = LoggerFactory.getLogger(WmsServlet.class);
    /**
     * version string for output at the beginning
     **/
    static final String RASWMS_VERSION = "raswms: rasdaman OGC WMS servlet, rasdaman v" + Version.VERSION + ", generated on " + Version.DATE + ".";
    
    /**
     * The RasDaMan user name
     **/
    private static String user = null;
    
    /**
     * The RasDaMan password
     **/
    private static String passwd = null;
    
    /**
     * The RasDaMan port
     **/
    private static String port = null;
    
    /**
     * servlet full path
     */
    String servletPath;
    
    /**
     * The RasDaMan database to connect to
     **/
    private static String databaseName = null;
    
    /**
     * The RasDaMan server host
     **/
    private static String server = null;
    
    /**
     * The capabilities that hold the meta data and generate the xml for the capabilities request (should be generated once and then cached in a string)
     **/
    private static WmsCapabilities myCapabilities = null;
    
    /**
     * For quicker access this object is requested once from the myCapabilities
     **/
    private static WmsCapabilities.WmsCapability myCapability = null;
    
    /**
     * This is a counter which tells how many servlets have been instantiated during the life time of this servlet engine
     * (or how often this servlet has been instantiated)
     **/
    private static int counter = 0;
    
    /**
     * This tells if the initialisation was successful.  It should be queried each time there is a request.  If the initialisation was not successful the string in failedMessage should be output to the client.
     **/
    private static boolean isInitialised = false;
    
    /**
     * Configuration object
     **/
    private static WmsConfig configuration = null;
    
    /**
     * Servlet configuration, to be remembered for new init() triggered by ReloadCapabilities request
     **/
    private static ServletConfig srvConfig = null;
    
    /**
     * debug trace output threshold level (can be set through web.xml parameter "debug")
     **/
    public static int initDebugVal = 0;
    
    /**
     * path/file to message text file;
     * read by WMSException
     **/
    public static String messageFile = "";
    
    // --- methods ---------------------------------------------
    
    /**
     * init method of the servlet.  Reads all meta information and rasdaman information.  If everything is ok then isInitialised is set to true.
     * Any exceptions should be converted into a string and stored in failedMessage.
     * A check for null is not enough, there also needs to be a check for the empty string (length == 0).
     **/
    @Override
    public void init(ServletConfig servletConfig) throws ServletException
    {
        super.init(servletConfig);
        
        // no other code - especially output - before the init sequence!
        
        servletPath = servletConfig.getServletContext().getRealPath("/WEB-INF/");
        
        srvConfig = servletConfig; 	// remember in case of a new init()
        
        log.info( RASWMS_VERSION );
        log.info( "WmsServlet.init start." );
        
        log.info( "WmsServlet.init: start." );
        log.info( "Servlet started at path " + servletPath );
        
        System.setProperty("java.awt.headless","true");
        
        log.info( "WmsServlet.init: initialising servlet #" + counter);
        // read config file and set rasdaman access data
        
        // BEGIN uniqueInit() code
        
        // drop preexisting configuration object (if any, get the unique instance
        configuration = WmsConfig.resetInstance();
        
        // -- BEGIN messageFile -- get message property file name from web deployment file (web.xml)
        // will be used by WMSException
        messageFile = getInitParameter( Globals.MESSAGE_FILE );
        if(messageFile==null)
        {
            log.warn( "Warning: startup parameter '" + Globals.MESSAGE_FILE + "' missing, will not be able to display error messages, just numeric codes." );
        }
        else
        {
            messageFile = servletPath + "/" + messageFile;
            log.info( "Info: startup parameter '" + Globals.MESSAGE_FILE + "' is: " + messageFile );
        }
        // -- END messageFile --
        
        /* WMS initialization priority order:
         *  1. (@Deprecated) Configuration file (.cfg)
         *  2. Database 
         *  3. Servlet arguments
         */
        String configFile = getInitParameter( Globals.CONFIGFILE );
        if(configFile==null)
        {
            log.warn( "Info: startup parameter '" + Globals.CONFIGFILE + "' is not specified." );
            String driver = ConfigManager.METADATA_DRIVER;
            if (driver != null)
            {
                log.warn( "Info: now trying to init from database..." );
                configuration.initFromDatabase(this);
            } else {
                log.warn( "Info: no DB driver found in ConfigManager. Try to init form args.");
                configuration.initFromInitArgs(servletConfig);
            }
        }
        else
        {
            log.info( "Info: startup parameter '" + Globals.CONFIGFILE + "' is: " + configFile );
            String absPath = servletConfig.getServletContext().getRealPath(configFile);
            configuration.initFromFile( absPath );
        }
        
        // END uniqueInit() code
        
        // open just one connection so that we can start operation; rest will be done concurrently, later
        try
        {
            openConnection( configuration );
            // this was missing -- PB 2004-apr-24
            isInitialised = true;
        }
        catch (ServletException s)
        {
            log.error( "Error: servlet exception, cannot open first connection: " + s.getMessage() );
        }
        catch (WMSException r)
        {
            log.error( "Error: database exception, cannot open first connection: " + r.getMessage() );
        }
        
        log.info( "WmsServlet.init: done, isInitialised=" + isInitialised );
    } // init()
    
    /**
     * Clean up: close the database connection
     **/
    @Override
    public void destroy()
    {
        closeConnection();
    }
    
    
    /**
     * This should actually report information on the servlet.  but does not.
     **/
    @Override
    public String toString()
    {
        return new String(this.getClass().getName() + ": user=" + user + ", passwd=" + passwd + ", database=" + databaseName + "@" + server + ":" + port + "\n");// + myCapabilities.toString());
    }
    
    
    /**
     * GET access of the servlet
     */
    @Override
    public void doGet(HttpServletRequest requ, HttpServletResponse resp)
    {
        log.info( "doGet: start. query is: " + requ.getQueryString() );
        
        WmsRequest request = null;
        try
        {
            if (requ.getQueryString() == null)
            {
                log.warn( "doGet: empty request string, cowardly refusing." );
                throw new WMSException( Globals.ERR_InvalidRequest );
            }
            
            // generate new request object
            // changed to allocating a fresh object per request -- PB 2004-apr-24
            request = new WmsRequest();
            
            // initialize from request parameters
            request.parseRequest(requ);
            
            // rethrow exception if parseRequest() yielded an error
            if (request.valid == false)
            {
                log.info( "parseRequest() followup exception to: " + request.errorMsg );
                throw new WMSException( Globals.ERR_Followup );
            }
            
            // check this after parseRequest() etc. so that we have more chances on a valid request object
            log.info( "doGet after parse, before answer: isInitialised=" + isInitialised );
            // EXP if (!isInitialised)
            // EXP throw new WMSException( Globals.ERR_ServletNotInit, failedMessage );
            
            // set default error response type, according to WMS spec
            resp.setContentType( WmsRequest.WMSCAPABILITIES );
            
            switch (request.getRequestType())
            {
                case WmsRequest.EXPINFO:
                    answerExportReq(request, requ, resp);
                    break;
                case WmsRequest.MAP:
                    answerMapReq(request, requ, resp);
                    break;
                case WmsRequest.FEATURE:
                    answerFeatureReq(request, requ, resp);
                    break;
                case WmsRequest.CAPABILITIES:
                    answerCapabilitiesReq(request, requ, resp);
                    break;
                case WmsRequest.RELOAD_CAPABILITIES:	// does its own error handling and response
                    answerReloadReq(request, requ, resp);
                    break;
                case WmsRequest.FETCH_CONFIGURATION_FILE:
                    answerFetchConfigurationFile(request, requ, resp);
                    break;
                default:
                    throw new WMSException( Globals.ERR_IllegalRequest );
            }
        }
        catch (IllegalThreadStateException t)
        {
            log.error( "doGet thread error: " + t.getMessage());
        }
        catch (Exception e)
        {
            try
            {
                // write first into the log!
                log.error( "doGet failed: " + e.getClass().toString() + ":" + e.getMessage() );
                
                // "null" should never happen according to parseRequest and the try{} block sequence, but to be sure:
                if (request == null)
                {
                    log.info( "doGet: Error: null request object." );
                    throw new WMSException( Globals.ERR_InvalidRequestObject, e.getMessage() );
                }
                if (request.answerPossible == false)
                {
                    log.info( "doGet: Error: cannot answer request." );
                    throw new WMSException( Globals.ERR_InvalidRequestObject, e.getMessage() );
                }
                
                // generate error message for client
                switch ( request.getExceptionType() )
                {
                    case WmsRequest.XML :
                        log.info( "reporting error as XML string.");
                        // this could raise exception if OutputStream has already been instantiated (how do we avoid this?)
                        PrintWriter outXml = resp.getWriter();
                        writeErrorMsg(outXml, e);
                        break;
                    case WmsRequest.INIMAGE:
                        log.info( "reporting error as image.");
                        paintErrorMsg(resp, request, e);
                        break;
                    case WmsRequest.BLANK:
                        log.info( "responding with a BLANK image.");
                        paintBlank( resp, request );
                        break;
                    default:
                        log.error( "unknown exception type for error report, using XML.");
                        PrintWriter outUnkown = resp.getWriter();
                        writeErrorMsg(outUnkown, e);
                }
            }
            catch (Exception e1)	// was: IOException
            {
                log.error( "generation of error msg failed (retrying as XML): "  + e1.getMessage());
                try
                {
                    PrintWriter outFallback = resp.getWriter();
                    writeErrorMsg(outFallback, e);
                }
                catch  (IOException e2)
                {
                    // if we end up here we have no way to report an error, we just can log it
                    log.error( "panic: XMLing the error failed too. giving up on this request.");
                }
            }
        }
        
        log.info( "doGet: done." );
    } // doGet()
    
    /**
     * Answer a feature request (not implemented throws an exception).
     **/
    protected void answerFeatureReq(WmsRequest request, HttpServletRequest requ, HttpServletResponse resp) throws WMSException
    {
        throw new WMSException( Globals.ERR_FeatureNotSupported );
    }
    
    /**
     * Answer a ExportInfo request.
     **/
    protected void answerExportReq(WmsRequest request, HttpServletRequest requ, HttpServletResponse resp) throws WMSException
    {
        //		log.info( "Delivered to " + requ.getRemoteAddr() + "/" + requ.getRemoteHost() + " ExportInfo: " + request);
        
        
        RequestConverter converter = new RequestConverter(myCapability);
        
        String expoInfo = converter.buildExpoInfo(request);
        
        try
        {
            PrintWriter out = resp.getWriter();
            // generate XML output
            resp.setContentType("text/plain");
            
            out.println(expoInfo);
        }
        catch (IOException e)
        {
            log.error( "answerExportReq: cannot write to client " + e.getMessage());
        }
        
    }
    
    /**
     * Answer a map request.
     **/
    protected void answerMapReq(WmsRequest request, HttpServletRequest requ, HttpServletResponse resp) throws WMSException, Exception
    {
        log.info( "answerMapReq: Client=" + requ.getRemoteAddr() + "/" + requ.getRemoteHost() + " image request: " + request);
        
        RequestConverter converter = new RequestConverter(myCapability);
        String contentType = converter.getImageFormatName(request.getImageFormat());
        
        
        String rasQuery = converter.buildRasQLQuery(request);
        log.error("answerMapReq: rasQuery=" + rasQuery);
        
        // multi - version
        BufferedOutputStream sOut;
        DBag resultBag = null;
        RasGMArray result = null;

        log.info( "answerMapReq: now executing query...");
        resultBag = (DBag) RasUtil.executeRasqlQuery(rasQuery);
        log.info( "answerMapReq: query done");
        
        if (resultBag != null)
        {
            log.info( "answerMapReq: Content type=" + contentType );
            resp.setContentType( contentType );
            try
            {
                sOut = new BufferedOutputStream( resp.getOutputStream());
            }
            catch (IOException e)
            {
                log.error( "answerMapReq: cannot create output stream");
                throw new WMSException( Globals.ERR_OutputCreate, e.getMessage() );
            }
            try
            {
                for (Object o: resultBag)
                {
                    // if there is more than one entry: use multipart mime
                    log.debug( "answerMapReq: extracting MDD from query result");
                    result = (RasGMArray) o;
                    // extract and return image
                    sOut.write( result.getArray() );
                }
                sOut.flush();
                log.info( "answerMapReq: done writing " + result.getArray().length + " bytes of image stream, announced length =" + result.getArraySize() );
            }
            catch (IOException e)
            {
                log.error( "answerMapReq: writing to client: " + e.getMessage());
            }
        }
        
        log.info( "answerMapReq: done." );
    }
    
    
    /**
     * Send exception to the client.
     **/
    protected void writeErrorMsg(PrintWriter out, Exception e)
    {
        if (e instanceof WMSException)
        {
            out.println(e.toString());
        }
        else
        {
            WMSException p = new WMSException( Globals.ERR_InternalException, e.toString() );
            out.println(p.toString());
        }
    }
    
    /**
     * Send the exception to the client.
     **/
    protected void paintErrorMsg(HttpServletResponse resp, WmsRequest request, Exception e)
    {
        log.info( "paintErrorMsg start: width = " + request.getWidth() + ", height = " + request.getHeight() );
        
        resp.setContentType(WmsRequest.JPEGSTRING);
        
        try
        {
            BufferedOutputStream sOut = new BufferedOutputStream( resp.getOutputStream());
            
            if (e instanceof WMSException)
            {
                WMSException p = (WMSException)e;
                p.toJpeg( sOut, request.getWidth(), request.getHeight());
            }
            else
            {
                WMSException p = new WMSException( Globals.ERR_InternalException, e.toString() );
                
                p.toJpeg( sOut, request.getWidth(), request.getHeight());
            }
        }
        catch(IOException m)
        {
            log.error( "paintErrorMsg: " + m.getMessage() );
        }
        log.info( "paintErrorMsg done." );
    }
    
    /**
     * Respond with a blank image to the client.
     **/
    protected void paintBlank(HttpServletResponse resp, WmsRequest request)
    {
        log.info( "paintBlank: width = " + request.getWidth() + ", height = " + request.getHeight() );
        
        resp.setContentType(WmsRequest.JPEGSTRING);
        
        try
        {
            BufferedOutputStream sOut = new BufferedOutputStream( resp.getOutputStream());
            BufferedImage bImage = new BufferedImage( request.getWidth(), request.getHeight(), BufferedImage.TYPE_3BYTE_BGR );
            Graphics2D gr = bImage.createGraphics();
            
            // make it white
            Rectangle rect = new Rectangle(0,0,bImage.getWidth(),bImage.getHeight());
            gr.setColor(Color.white);
            gr.fill(rect);
            ImageIO.write(bImage,"jpeg",sOut);
        }
        catch (Exception m)
        {
            log.error( "paintErrorMsg: " + m.getMessage() );
        }
        
        log.info( "paintBlank done. " );
    }
    
    /**
     * answer a capabilities request
     **/
    protected void answerCapabilitiesReq(WmsRequest request, HttpServletRequest requ, HttpServletResponse resp) throws WMSException
    {
        /*
         * UPDATESEQUENCE
         * client	server
         * none	any	recent
         * any	none	recent
         * equal	equal	e:CurrentUpdateSequence
         * lower	higher	recent
         * higher	lower	e:InvalidUpdateSequence
         */
        int reqUpdS = request.getUpdateSequence();
        int capUpdS = myCapabilities.getUpdateSequence();
        if (reqUpdS != 0)
        {
            if (capUpdS != 0)
            {
                if (capUpdS == reqUpdS)
                    throw new WMSException( Globals.ERR_UpdatesequenceSame );
                if (capUpdS < reqUpdS)
                    throw new WMSException( Globals.ERR_UpdatesequenceHigher );
            }
        }
        try
        {
            PrintWriter out = resp.getWriter();
            // generate XML output
            resp.setContentType(WmsRequest.WMSCAPABILITIES);
            
            String answer = myCapabilities.toString();
            // XML output has to be checked in Browser with xml support
            log.info( "length=" + answer.length());
            //why is real content length = answer.length()+1?
            //resp.setContentLength(answer.length());
            out.println(answer);
        }
        catch (IOException e)
        {
            log.error( "answerCapabilities: could not write to client: " + e.getMessage());
        }
    } // answerCapabilitiesReq()
    
    
    /**
     * answer a reload capabilities request
     * - close database connection
     * - re-read config file (raswms.cfg; no re-read of web.xml!)
     * - re-establish connections to database
     * - no new thread is opened, current one is reused!
     * comment:
     * - see also uniqueInit()
     * - unclear: is changed configuration propagated to all threads?
     **/
    protected void answerReloadReq( WmsRequest request, HttpServletRequest requ, HttpServletResponse resp ) throws ServletException, WMSException, IOException
    {
        log.info( "WmsServlet reloading capabilities start" );
        
        closeConnection();
        
        // to re-initialize, simply call init() instead of this old code
        // configuration.initFromFile( getInitParameter("configFile") );
        // openConnection( Config.getInstance() );
        //isInitialised = true;
        init( srvConfig );
        
        // prepare answer as XML reply
        resp.setContentType( "text/html" );
        PrintWriter outXml = resp.getWriter();
        outXml.println( Globals.RELOAD_CAPABILITIES_RESPONSE );
        
        log.info( "WmsServlet reloading capabilities done" );
    }
    
    
    protected void answerFetchConfigurationFile(WmsRequest request, HttpServletRequest requ, HttpServletResponse resp) throws ServletException, WMSException, IOException
    {
        log.info( "WmsServlet fetching configuration file start");
        
        String settings = WmsConfig.getInstance().dumpSettings();
        resp.setContentType("text/plain");
        PrintWriter out = resp.getWriter();
        out.print(settings);
        
        log.info( "WmsServlet fetching configuration file done");
    }
    
    /**
     * open rasdaman connection
     * precondition:
     * - valid configuration parameters set
     */
    protected void openConnection( WmsConfig configuration ) throws ServletException, WMSException
    {
        log.info( "WmsServlet openConnection start" );
        
        //rasdaman specific stuff from the init file
        user         = ConfigManager.RASDAMAN_USER;
        passwd       = ConfigManager.RASDAMAN_PASS;
        server       = ConfigManager.RASDAMAN_SERVER;
        databaseName = ConfigManager.RASDAMAN_DATABASE;
        port         = ConfigManager.RASDAMAN_PORT;
        
        // init capabilities
        WmsCapabilities.WmsContact wmsContact = configuration.createContact();
        
        myCapability = configuration.createCapability();
        myCapabilities = configuration.createCapabilities(wmsContact,myCapability);
        
        log.info( "WmsServlet openConnection done." );
    } // openConnection()
    
    /**
     * close database connections
     */
    protected void closeConnection()
    {
        
    }
} // WmsServlet

