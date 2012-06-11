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
 * class for parsing the parameters passed to the WmsServlet.  it supports version 1.1.0 and 1.0.0 requests.
 * @version $Revision: 1.3 $ 
 *
 * COMMENTS:
 * - FIXME: ReloadCapabilities only supports XML response
 *
 * </pre>
 *********************************************************** */
package petascope.wms;

import java.util.Hashtable;
import java.util.LinkedList;
import java.util.List;
import java.util.StringTokenizer;
import javax.servlet.http.HttpServletRequest;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WMSException;


public class WmsRequest
{
    private static final Logger log = LoggerFactory.getLogger(WmsRequest.class);

    // WMS requests
    public static final String WMSREQ_GET_CAPABILITIES = "GetCapabilities";
    public static final String WMSREQ_GET_MAP = "GetMap";
    public static final String WMSREQ_GET_FEATURE_INFO = "GetFeatureInfo";
    public static final String WMSREQ_GET_EXPORT_INFO = "GetExportInfo";
    public static final String WMSREQ_RELOAD_CAPABILITIES = "ReloadCapabilities";
    public static final String WMSREQ_FETCH_CONFIGURATION_FILE = "FetchConfigurationFile";
    @Deprecated public static final String WMSREQ_CAPABILITIES = "capabilities";
    @Deprecated public static final String WMSREQ_MAP = "map";
    @Deprecated public static final String WMSREQ_FEATURE_INFO = "feature_info";
    // Custom DEM
    public static final String WMSREQ_CUSTOMDEM_T = "T";
    public static final String WMSREQ_CUSTOMDEM_MINLEVEL = "minLevel";
    public static final String WMSREQ_CUSTOMDEM_MAXLEVEL = "maxLevel";
    // WMS formats
    public static final String WMSREQ_FORMAT_JPEG = "JPEG";
    public static final String WMSREQ_FORMAT_PNG = "PNG";
    public static final String WMSREQ_FORMAT_TIFF = "TIFF";
    // WMS Request keys
    public static final String WMSREQ_KEY_FEES = "FEES";
    public static final String WMSREQ_KEY_VERSION = "VERSION";
    public static final String WMSREQ_KEY_WMTVER = "WMTVER";
    public static final String WMSREQ_KEY_REQUEST = "REQUEST";
    // +Export request
    public static final String WMSREQ_KEY_LAYERS = "LAYERS";
    public static final String WMSREQ_KEY_STYLES = "STYLES";
    public static final String WMSREQ_KEY_SRS = "SRS";
    public static final String WMSREQ_KEY_BBOX = "BBOX";
    public static final String WMSREQ_KEY_WIDTH = "WIDTH";
    public static final String WMSREQ_KEY_HEIGHT = "HEIGHT";
    public static final String WMSREQ_KEY_EXCEPTIONS = "EXCEPTIONS";
    public static final String WMSREQ_KEY_CUSTOMDEM = "CUSTOMDEM";
    // +Capabilities request
    public static final String WMSREQ_KEY_SERVICE = "SERVICE";
    public static final String WMSREQ_KEY_UPDATESEQUENCE = "UPDATESEQUENCE";
    // +GetMap
    public static final String WMSREQ_KEY_TRANSPARENT = "TRANSPARENT";
    public static final String WMSREQ_KEY_FORMAT = "FORMAT";
    public static final String WMSREQ_KEY_BGCOLOR = "BGCOLOR";
    public static final String WMSREQ_KEY_TIME = "TIME";
    public static final String WMSREQ_KEY_ELEVATION = "ELEVATION";
    
    ///////////////////////////////////////
    
    /**
     * ----- enum constant definitions; "0" is never allowed as real value, that's UNDEF! -----
     **/
    
    /**
     * numeric value to represent "undefined"
     **/
    public static final int UNDEF = 0;
    
    /**
     * Request type is: get capabilities
     **/
    public static final int CAPABILITIES = 1;
    
    /**
     * Request type is: get map
     **/
    public static final int MAP = 2;
    
    /**
     * Request type is: get feature infor
     **/
    public static final int FEATURE = 3;
    
    /**
     * Request type is: get export info
     **/
    public static final int EXPINFO = 4;
    
    /**
     * Request type is: reload capabilities
     **/
    public static final int RELOAD_CAPABILITIES = 5;
    
    /**
     * Request type is: Fetch configuration file
     **/
    public static final int FETCH_CONFIGURATION_FILE = 6;
    
    /**
     * Requested image type is: jpeg
     **/
    public static final int JPEG = 1;
    
    /**
     * Requested image type is: png
     **/
    public static final int PNG = 2;
    
    /**
     * Requested image type is: tiff
     **/
    public static final int TIFF = 3;
    
    /**
     * Requested exception output type is: in the image
     **/
    public static final int INIMAGE = 1;
    
    /**
     * Requested exception output type is: as xml text
     **/
    public static final int XML = 2;
    
    /**
     * Requested exception output type is: send a blank image
     **/
    public static final int BLANK = 3;
    
    /**
     * the string which represents a version 1.1.0 png
     **/
    public static final String PNGSTRING = "image/png";
    
    /**
     * the string which represents a version 1.0.0 png
     **/
    public static final String PNGSTRING10 = "PNG";
    
    /**
     * the string which represents a version 1.1.0 jpeg
     **/
    public static final String JPEGSTRING = "image/jpeg";
    
    /**
     * the string which represents a version 1.0.0 jpeg
     **/
    public static final String JPEGSTRING10 = "JPEG";
    
    /**
     * the string which represents a version 1.1.0 tiff
     **/
    public static final String TIFFSTRING = "image/tiff";
    
    /**
     * the string which represents a version 1.0.0 tiff
     **/
    public static final String TIFFSTRING10 = "TIFF";
    
    /**
     * The string which represents a version 1.1.0 xml exception
     **/
    public static final String XMLSTRING = "application/vnd.ogc.se_xml";
    
    /**
     * The string which represents a version 1.0.0 xml exception
     **/
    public static final String XMLSTRING10 = "WMS_XML";
    
    /**
     * The string which represents a version 1.1.0 blank image exception
     **/
    public static final String BLANKSTRING = "application/vnd.ogc.se_blank";
    
    /**
     * The string which represents a version 1.0.0 blank image exception
     **/
    public static final String BLANKSTRING10 = "BLANK";
    
    /**
     * The string which represents a version 1.1.0 in image exception
     **/
    public static final String INIMAGESTRING = "application/vnd.ogc.se_inimage";
    
    /**
     * The string which represents a version 1.0.0 in image exception
     **/
    public static final String INIMAGESTRING10 = "INIMAGE";
    
    /**
     * ???
     **/
    public static final String WMSCAPABILITIES = "application/vnd.ogc.wms_xml";
    
    /**
     * The string which must be present in an capabilities request
     **/
    public static final String WMSSERVICE = "WMS";
    
    /**
     * The string which identifies the version as 1.0.0
     **/
    public static final String VERSION10 = "1.0.0";
    
    /**
     * The string which identifies the version as 1.1.0
     **/
    public static final String VERSION11 = "1.1.0";
    
    /**
     * constants and static parameters for WMS parameter request table
     */
    private static final String WMS_DELIMITER_STRING = "&";	// separator between parameters
    private static final String WMS_EQUALS_STRING    = "=";	// for "name=value"
    
    /**
     * ----- static variables -----
     **/
    
    /**
     * instance counter; incremented with every instance creation
     **/
    protected static int counter = 0;
    
    /**
     * /**
     * ----- instance variables -----
     **/
    
    /**
     * instance id
     **/
    protected int myId = 0;
    
    /**
     * /**
     * is request object valid?
     **/
    protected boolean valid = false;
    
    /**
     * if (! valid): store error msg here
     **/
    protected String errorMsg = null;
    
    /**
     * do we have enough valid parameter values that allow to return an error message to the client?
     **/
    protected boolean answerPossible = false;
    
    /**
     * VERSION part of the wms request
     **/
    protected String version = null;
    
    /**
     * REQUEST part of the wms request
     **/
    protected int requestType = 0;
    
    /**
     * SRS part of the wms request
     **/
    protected String srs = null;
    
    /**
     * BBOX part of the wms request
     **/
    protected BoundingBox bbox;
    
    /**
     * WIDTH part of the wms request
     **/
    protected int width = 0;
    
    /**
     * HEIGHT part of the wms request
     **/
    protected int height = 0;
    
    /**
     * LAYER+STYLE part of the wms request
     **/
    protected List layerList;
    
    /**
     * FORMAT part of the wms request
     **/
    protected int imageFormat = 0;
    
    /**
     * BACKGROUND part of the wms request: red, green, and blue value, resp.
     **/
    protected int bgColorRed   = 255;
    protected int bgColorGreen = 255;
    protected int bgColorBlue  = 255;
    
    /**
     * TRANSPARENT part of the wms request
     **/
    protected boolean transparent;
    
    /**
     * EXCEPTIONS part of the wms request
     **/
    protected int exceptions = 0;
    
    /**
     * TIME part of the wms request
     **/
    protected int time = 0;
    
    /**
     * ELEVATION part of the wms request
     **/
    protected double elevation = 0.0;
    
    /**
     * SERVICE part of the wms request
     **/
    protected String service = null;
    
    /**
     * UPDATESEQUENCE part of the wms request
     **/
    protected int updateSequence = 0;
    
    /**
     * FEES part of the wms request
     **/
    protected String fees = null;
    
    /**
     * Client of the http request
     **/
    protected String client = null;
    
    /**
     * Custom DEM
     **/
    protected String customDem = null;
    
    // was static, thereby violating thread-safety -- PB 2004-apr-24
    private Hashtable requestParams = null;		// WMS parameter table with normalized (upper case) names
    
    /**
     * constructor
     */
    public WmsRequest()
    {
        myId = ++counter;
        log.debug( "WmsRequest::WmsRequest()" );
        valid = false;				// no params yet
    }
    
    /**
     * returns id of this instance
     */
    public int getId()
    {
        return myId;
    }
    
    /**
     * wrappers for debug output to add myId identifier
     */
    public void talkVerbose( String s )   { log.debug(   "{" + myId + "} " + s ); }
    public void enterVerbose( String s )  { log.info(  "{" + myId + "} " + s ); }
    public void leaveVerbose( String s )  { log.info(  "{" + myId + "} " + s ); }
    public void talkSparse( String s )    { log.info(    "{" + myId + "} " + s ); }
    public void enterSparse( String s )   { log.info(   "{" + myId + "} " + s ); }
    public void leaveSparse( String s )   { log.info(   "{" + myId + "} " + s ); }
    public void talkWarning( String s )   { log.warn(   "{" + myId + "} " + s ); }
    public void talkCritical( String s )  { log.error(  "{" + myId + "} " + s ); }
    public void enterCritical( String s ) { log.info( "{" + myId + "} " + s ); }
    public void leaveCritical( String s ) { log.info( "{" + myId + "} " + s ); }
    
    /**
     * per WMS request, prepare table of name=value pairs with all upper case name
     */
    private void prepareRequestParamTable( HttpServletRequest req )
    {
        enterVerbose( "prepareRequestParamTable start" );
        
        // these variables which are exclusively used in this method have made local,
        // have been global and static 8-() thereby violating thread safety -- PB 2004-apr-24
        String keywd = null;			// name of a name/value pair
        String value = null;			// value of a name/value pair
        
        // allocate parameter table for this request
        requestParams = new Hashtable();
        
        String encodedQueryString = req.getQueryString();
        String queryString = java.net.URLDecoder.decode( encodedQueryString );
        
        talkSparse( "WmsRequest::prepareRequestParamTable: raw request    : " + encodedQueryString );
        talkSparse( "WmsRequest::prepareRequestParamTable: decoded request: " + queryString );
        
        StringTokenizer listTok = new StringTokenizer( queryString, WMS_DELIMITER_STRING );
        while ( listTok.hasMoreTokens() )
        {
            StringTokenizer itemTok = new StringTokenizer( listTok.nextToken() , WMS_EQUALS_STRING );
            if ( itemTok.hasMoreTokens() )
                // make name upper case for later comparison:
                keywd = itemTok.nextToken().toUpperCase();
            if ( itemTok.hasMoreTokens() )
                value = itemTok.nextToken();
            else
                value = "";	// was: =null, crashed hash table -- PB 2004-feb-10
            if ( keywd != null )
            {
                requestParams.put( keywd, value );
                talkVerbose( "added request keyword/value pair: " + keywd + " = " + (value==null?"null":value)  );
            }
        }
        leaveVerbose( "prepareRequestParamTable done" );
    } // prepareRequestParamTable()
    
    /**
     * getparameterCI - "case Insensitive"
     * returns value component addressed by (case insensitive) name parameter
     */
    private String getParameterCI(HttpServletRequest req, String paramName)
    {
        /* old implementation, can only handle all upper case or all lower case, but not mixed:
         * String retVal = req.getParameter(paramName.toUpperCase());
         *
         * if(retVal == null)
         * { retVal = req.getParameter(paramName.toLowerCase());
         * }
         * return retVal;
         * new implementation, reads from normalized parameter table and is _really_ case insensitive:
         */
        String retval = (String) requestParams.get( paramName.toUpperCase() );
        talkVerbose( "getParameterCI: " + paramName + " -> " + retval );
        return retval;
    }
    
    /**
     * parseRequest(HttpServletRequest req): build parameter table from request parameters, check validity
     * @throws nothing (was: WMSException if there are errors during the parsing process (missing required parameters, unknown/illegal arguments to parameters and mismatches between version number and argument/parameter names.) but we need to return a valid object in any case
     */
    
    // According to the specification the parameter names should be case insensitive, now they are!
    // no need to return object now that we have a separate constructor -- PB 2004-apr-24
    // TODO: define String constants.
    public void parseRequest(HttpServletRequest req) throws WMSException
    {
        enterVerbose( "parseRequest start" );
        
        valid = false;		// until now not a request object with valid, complete parameters
        answerPossible = false;	// uninitialised object doesn't allow answer to client
        
        try
        {
            // prepare parameter name/value table
            prepareRequestParamTable( req );
            
            // initialize so that in case of error situation is clear
            // (see WmsServlet, doGet() and PaintMsg())
            width = UNDEF;
            height = UNDEF;
            exceptions = UNDEF;
            
            // now start reading the parameters
            fees = getParameterCI(req, WMSREQ_KEY_FEES);
            client = req.getRemoteAddr() + "/" + req.getRemoteHost();
            // feature info not supported for both versions -> parsing finished!!!
            
            version = null;
            String tempVersion = getParameterCI(req, WMSREQ_KEY_VERSION);
            if (tempVersion == null)
            {
                talkWarning( "WmsRequest parseRequest: WMS 1.1.0 Parameter VERSION is missing. Trying with deprecated WMTVER.");
                tempVersion = getParameterCI(req, WMSREQ_KEY_WMTVER);
            }
            
            String request = getParameterCI(req, WMSREQ_KEY_REQUEST);
            requestType = UNDEF;
            if (request == null)
            {
                leaveVerbose( "parseRequest end with exception: null request" );
                throw new WMSException( Globals.ERR_RequestMissing );
            }
            if (request.equalsIgnoreCase(WMSREQ_GET_CAPABILITIES))
            {
                requestType = CAPABILITIES;
                version = VERSION11;
                if (tempVersion == null)
                {
                    talkWarning( "WmsRequest parseRequest: WMS 1.0.0 Parameter WMTVER is missing.");
                    tempVersion = version;
                }
            }
            else if (request.equalsIgnoreCase(WMSREQ_CAPABILITIES))
            {//1.0
                requestType = CAPABILITIES;
                version = VERSION10;
                if (tempVersion == null)
                    tempVersion = version;
            }
            else if (request.equalsIgnoreCase(WMSREQ_GET_MAP))
            {
                requestType = MAP;
                version = VERSION11;
            }
            else if (request.equalsIgnoreCase(WMSREQ_MAP))
            {
                requestType = MAP;
                version = VERSION10;
            }
            else if (request.equalsIgnoreCase(WMSREQ_FEATURE_INFO))
            {
                requestType = FEATURE;
                version = VERSION10;
            }
            else if (request.equalsIgnoreCase(WMSREQ_GET_FEATURE_INFO))
            {
                requestType = FEATURE;
                version = VERSION11;
            }
            else if (request.equalsIgnoreCase(WMSREQ_GET_EXPORT_INFO))
            {
                requestType = EXPINFO;
                version = VERSION11;
            }
            else if (request.equalsIgnoreCase(WMSREQ_RELOAD_CAPABILITIES))
            {
                requestType = RELOAD_CAPABILITIES;
                version = VERSION11;
            }
            else if (request.equalsIgnoreCase(WMSREQ_FETCH_CONFIGURATION_FILE)) {
                requestType = FETCH_CONFIGURATION_FILE;
                version = VERSION11;
            }
            else
            {
                leaveVerbose( "parseRequest end with exception: unknown request: " + request );
                throw new WMSException( Globals.ERR_UnknownRequest, request );
            }
            
            switch (requestType)
            {
                case EXPINFO:
                    parseExportRequest(req, this);
                    break;
                case FEATURE:
                    parseFeatureRequest(req, this);
                    break;
                case MAP:
                    // parseMapRequest(req, this);
                    parseMapRequest(req);
                    break;
                case CAPABILITIES:
                    parseCapabilitiesRequest(req, this);
                    break;
                case RELOAD_CAPABILITIES:
                    // no further parameters as for now
                    // error message return format is always XML, others unsupported now
                    exceptions = XML;
                    break;
                case FETCH_CONFIGURATION_FILE:
                    // no further parameters for now
                    exceptions = XML;
                    break;
                default:
                    talkCritical( "internal error: illegal request type " + requestType );
                    break;
            }
            
            // throw possible exceptions as far down as possible, to achieve "full" request object
            
            // VERSION parameter optional for GetCapabilities, not suported for ReloadCapabilities -> check only other cases
            if (requestType != CAPABILITIES && requestType != RELOAD_CAPABILITIES)
            {
                if (tempVersion == null)
                {
                    leaveVerbose( "parseRequest end with exception: version missing" );
                    throw new WMSException( Globals.ERR_VersionMissing );
                }
                if (!tempVersion.equalsIgnoreCase(version))
                {
                    leaveVerbose( "parseRequest end with exception: version mismatch" );
                    throw new WMSException( Globals.ERR_VersionMismatch );
                }
            }
            
            // if we come here we have a valid request object
            valid = true;
        }
        
        // we MUST make sure that a valid request object is returned, so we catch exceptions here
        // & don't rethrow
        catch (WMSException e) 	// that should only be re-catching, we focus on returning an object
        {
            talkCritical( "request parsing error: " + e.getMessage() );
            valid = false;
            errorMsg = e.getMessage();
        }
        
        // is information sufficient to return - just in case - an in-image error message?
        answerPossible =
                ( exceptions != UNDEF )
                &&
                ( (exceptions != INIMAGE) || (width != UNDEF && height != UNDEF) );
        // as GetCapabilities and ReloadCapabilities do not need image response, answer is always possible
        answerPossible |= (requestType == CAPABILITIES) || (requestType == RELOAD_CAPABILITIES);
        
        leaveSparse( "parseRequest done; request valid = " + valid + ", response possible = " + answerPossible + ", request = " + this );
    } // parseRequest()
    
    /**
     * parses the parameters for a map request
     **/
    protected void parseExportRequest(HttpServletRequest req, WmsRequest retval) throws WMSException
    {
        /*
         * GetExportInfo:
         * Required
         * VERSION done
         * REQUEST done
         * LAYERS
         * STYLES
         * SRS
         * BBOX (xmin,ymin,xmax,ymax)
         */
        String slayers = getParameterCI(req, WMSREQ_KEY_LAYERS);
        String sstyles = getParameterCI(req, WMSREQ_KEY_STYLES);
        retval.srs = getParameterCI(req, WMSREQ_KEY_SRS);// no check done here
        String sbbox = getParameterCI(req, WMSREQ_KEY_BBOX);
        String swidth = getParameterCI(req, WMSREQ_KEY_WIDTH);
        String sheight = getParameterCI(req, WMSREQ_KEY_HEIGHT);
        String sexception = getParameterCI(req, WMSREQ_KEY_EXCEPTIONS);
        retval.customDem         = getParameterCI(req, WMSREQ_KEY_CUSTOMDEM); //###
        
        // process WIDTH/HEIGHT/EXCEPTION first because they are important for error feedback
        if (swidth == null)
            throw new WMSException( Globals.ERR_WidthMissing );
        if (sheight == null)
            throw new WMSException( Globals.ERR_HeightMissing );
        if (sexception == null)
            retval.exceptions = XML;
        else if (sexception.equalsIgnoreCase(XMLSTRING))
            retval.exceptions = XML;
        else if (sexception.equalsIgnoreCase(INIMAGESTRING))
            retval.exceptions = INIMAGE;
        else if (sexception.equalsIgnoreCase(BLANKSTRING))
            retval.exceptions = BLANK;
        else
            throw new WMSException( Globals.ERR_IllegalException, "exception=" + sexception );
        
        StringTokenizer stBBox = new StringTokenizer(sbbox, ",");
        
        if (slayers == null)
            throw new WMSException( Globals.ERR_LayersMissing );
        if (sstyles == null)
            throw new WMSException( Globals.ERR_StylesMissing );
        if (retval.srs == null)
            throw new WMSException( Globals.ERR_SrsMissing );
        if (sbbox == null)
            throw new WMSException( Globals.ERR_BboxMissing );
        
        try
        {
            retval.bbox = new BoundingBox(Double.parseDouble(stBBox.nextToken()), Double.parseDouble(stBBox.nextToken()), Double.parseDouble(stBBox.nextToken()), Double.parseDouble(stBBox.nextToken()));
        }
        catch (NumberFormatException e)
        {
            throw new WMSException( Globals.ERR_BboxIllegal, "bbox=" + sbbox );
        }
        
        try
        {
            retval.width = Integer.parseInt(swidth);
        }
        catch (NumberFormatException e)
        {
            throw new WMSException( Globals.ERR_WidthIllegal );
        }
        try
        {
            retval.height = Integer.parseInt(sheight);
        }
        catch (NumberFormatException e)
        {
            throw new WMSException( Globals.ERR_HeightIllegal );
        }
        
        StringTokenizer strTok1 = new StringTokenizer(slayers, ",");
        StringTokenizer strTok2 = new StringTokenizer(sstyles, ",");
        if (strTok1.countTokens() != strTok2.countTokens())
        {
            throw new WMSException( Globals.ERR_LayersStylesMismatch, "styles=" + strTok2.countTokens() + ", layers=" + strTok1.countTokens() );
        }
        retval.layerList = new LinkedList();
        LayerStyle ls = null;
        String style = null;
        while(strTok1.hasMoreTokens())
        {
            style = strTok2.nextToken();
            if (style == null)
                style = "default";
            ls = new LayerStyle(strTok1.nextToken(), style);
            retval.layerList.add(ls);
        }
    }
    
    /**
     * parses the parameters for a feature request
     **/
    protected void parseFeatureRequest(HttpServletRequest req, WmsRequest retval) throws WMSException
    {
        //		parseMapRequest(req, retval);
        /*
         * feature_info: (optional)
         * GetFeatureInfo: (optional)
         * Required
         * VERSION done
         * REQUEST done
         * map request copy done
         * QUERY_LAYERS
         * X
         * Y
         * Optional
         * INFO_FORMAT
         * FEATURE_COUNT
         * EXCEPTIONS
         */
    }
    
    /**
     * parses the parameters for a capabilities request
     **/
    protected void parseCapabilitiesRequest(HttpServletRequest req, WmsRequest retval) throws WMSException
    {
        /*
         * GetCapabilities/capabilities:
         * Required
         * SERVICE = WMS
         * REQUEST
         * Optional
         * UPDATESEQUENCE
         * VERSION done
         */
        String service = getParameterCI(req, WMSREQ_KEY_SERVICE);
        if(service == null)
            service = WMSSERVICE;
        else	{
            if (!service.equalsIgnoreCase(WMSSERVICE))
                throw new WMSException( Globals.ERR_ServiceIllegal, "service=" + service );
        }
        retval.service = service;
        String updateSequence = getParameterCI(req, WMSREQ_KEY_UPDATESEQUENCE);
        if (updateSequence == null)
            retval.updateSequence = 0;
        else	{
            try	{
                retval.updateSequence = Integer.parseInt(updateSequence);
            }
            catch (NumberFormatException e)
            {
                throw new WMSException( Globals.ERR_UpdatesequenceIllegal, "updatesequence=" + updateSequence );
            }
        }
    }
    
    /**
     * parses the parameters for a map request
     * since this raswms version (May 2003): no more support for WMS 1.0.0 (allows to disregard VERSION)
     **/
    protected void parseMapRequest(HttpServletRequest req) throws WMSException
    {
        /*
         * GetMap:
         * Required
         * VERSION done
         * REQUEST done
         * LAYERS
         * STYLES
         * SRS
         * BBOX (xmin,ymin,xmax,ymax)
         * WIDTH
         * HEIGHT
         * FORMAT
         * Optional
         * TRANSPARENT
         * BGCOLOR
         * EXCEPTIONS
         * TIME
         * ELEVATION
         * other dimensions
         */
        
        enterVerbose( "parseMapRequest start, request = "  + this );
        
        service = WMSSERVICE;	// to have well-defined value -- PB 2005-oct-07
        // TODO: use String constants!
        String slayers = getParameterCI(req, WMSREQ_KEY_LAYERS);
        String sstyles = getParameterCI(req, WMSREQ_KEY_STYLES);
        srs = getParameterCI(req, WMSREQ_KEY_SRS);// no check done here
        String sbbox = getParameterCI(req, WMSREQ_KEY_BBOX);
        String sformat = getParameterCI(req, WMSREQ_KEY_FORMAT);
        String swidth = getParameterCI(req, WMSREQ_KEY_WIDTH);
        String sheight = getParameterCI(req, WMSREQ_KEY_HEIGHT);
        String sexception = getParameterCI(req, WMSREQ_KEY_EXCEPTIONS);
        String stransparent = getParameterCI(req, WMSREQ_KEY_TRANSPARENT);
        String sbgcolor = getParameterCI(req, WMSREQ_KEY_BGCOLOR);
        String stime = getParameterCI(req, WMSREQ_KEY_TIME);
        String selevation = getParameterCI(req, WMSREQ_KEY_ELEVATION);
        customDem         = getParameterCI(req, WMSREQ_KEY_CUSTOMDEM); //###
        
        // address WIDTH/HEIGHT/EXCEPTIONS first to ensure proper error handling if possible at all
        
        if (sexception == null)
            exceptions = XML;
        else if (sexception.equalsIgnoreCase(XMLSTRING))
            exceptions = XML;
        else if (sexception.equalsIgnoreCase(INIMAGESTRING))
            exceptions = INIMAGE;
        else if (sexception.equalsIgnoreCase(BLANKSTRING))
            exceptions = BLANK;
        else
        {
            leaveCritical( "parseMapRequest: done. EXCEPTIONS illegal." );
            throw new WMSException( Globals.ERR_IllegalException, "exceptions=" + sexception );
        }
        
        talkVerbose( "parseMapRequest: EXCEPTIONS=" + exceptions );
        
        try
        {
            width = Integer.parseInt(swidth);
        }
        catch (NullPointerException e)
        {
            leaveCritical( "parseMapRequest: done. WIDTH missing." );
            throw new WMSException( Globals.ERR_WidthMissing );
        }
        catch (NumberFormatException e)
        {
            leaveCritical( "parseMapRequest: done. WIDTH illegal." );
            throw new WMSException( Globals.ERR_WidthIllegal, "width=" + swidth );
        }
        if (width <= 0)
        {
            leaveCritical( "parseMapRequest: done. WIDTH non-positive." );
            throw new WMSException( Globals.ERR_WidthIllegal, "width=" + swidth );
        }
        talkVerbose( "parseMapRequest: WIDTH=" + width );
        
        try
        {
            height = Integer.parseInt(sheight);
        }
        catch (NullPointerException e)
        {
            leaveCritical( "parseMapRequest: done. HEIGHT missing." );
            throw new WMSException( Globals.ERR_HeightMissing );
        }
        catch (NumberFormatException e)
        {
            leaveCritical( "parseMapRequest: done. HEIGHT illegal." );
            throw new WMSException( Globals.ERR_HeightIllegal, "height=" + sheight );
        }
        if (height <= 0)
        {
            leaveCritical( "parseMapRequest: done. HEIGHT non-positive." );
            throw new WMSException( Globals.ERR_HeightIllegal, "height=" + sheight );
        }
        
        talkVerbose( "parseMapRequest: HEIGHT=" + height );
        
        if (sformat == null)
        {
            leaveCritical( "parseMapRequest: done. FORMAT missing." );
            throw new WMSException( Globals.ERR_FormatMissing );
        }
        else if (sformat.equalsIgnoreCase(PNGSTRING))
            imageFormat = PNG;
        else if (sformat.equalsIgnoreCase(JPEGSTRING))
            imageFormat = JPEG;
        else if (sformat.equalsIgnoreCase(TIFFSTRING))
            imageFormat = TIFF;
        else
        {
            leaveCritical( "parseMapRequest: done. FORMAT illegal." );
            throw new WMSException( Globals.ERR_FormatIllegal, "format=" + sformat );
        }
        talkVerbose( "parseMapRequest: FORMAT=" + imageFormat );
        
        // now for the less important parameters wrt error handling:
        if (slayers == null)
        {
            leaveCritical( "parseMapRequest: done. LAYERS missing." );
            throw new WMSException( Globals.ERR_LayersMissing );
        }
        if (sstyles == null)
        {
            leaveCritical( "parseMapRequest: done. STYLES missing." );
            throw new WMSException( Globals.ERR_StylesMissing );
        }
        if (srs == null)
        {
            leaveCritical( "parseMapRequest: done. SRS missing." );
            throw new WMSException( Globals.ERR_SrsMissing );
        }
        if (sbbox == null)
        {
            leaveCritical( "parseMapRequest: done. BBOX missing." );
            throw new WMSException( Globals.ERR_BboxMissing );
        }
        if (sformat == null)
        {
            leaveCritical( "parseMapRequest: done. FORMAT missing." );
            throw new WMSException( Globals.ERR_FormatMissing );
        }
        
        if (stime != null)
        {
            try
            {
                time = Integer.parseInt(stime);
            }
            catch (NumberFormatException e)
            {
                leaveCritical( "parseMapRequest: done. TIME illegal." );
                throw new WMSException( Globals.ERR_TimeIllegal, "time=" + stime );
            }
        }
        
        if (selevation != null)
        {
            try
            {
                elevation = Integer.parseInt(selevation);
            }
            catch (NumberFormatException e)
            {
                leaveCritical( "parseMapRequest: done. ELEVATION illegal." );
                throw new WMSException( Globals.ERR_ElevationIllegal, "elevation=" + selevation );
            }
        }
        
        
        StringTokenizer stBBox = new StringTokenizer(sbbox, ",");
        try
        {
            bbox = new BoundingBox(Double.parseDouble(stBBox.nextToken()), Double.parseDouble(stBBox.nextToken()), Double.parseDouble(stBBox.nextToken()), Double.parseDouble(stBBox.nextToken()));
        }
        catch (NumberFormatException e)
        {
            leaveCritical( "parseMapRequest: done. BBOX illegal." );
            throw new WMSException( Globals.ERR_BboxIllegal, "bbox=" + sbbox );
        }
        
        if (sbgcolor == null)					// missing parameter means 0xffffff
        {
            bgColorRed   = 255;
            bgColorGreen = 255;
            bgColorBlue  = 255;
        }
        else
        {
            if ( sbgcolor.length() != 8)			// format 0xDDDDDD
            {
                leaveCritical( "parseMapRequest: done. BGCOLOR illegal (prefix):" + sbgcolor );
                throw new WMSException( Globals.ERR_BgcolorIllegal, "bgcolor=" + sbgcolor );
            }
            
            if ( ! sbgcolor.startsWith( "0X" )
                    && ! sbgcolor.startsWith( "0x" ) )
            {
                leaveCritical( "parseMapRequest: done. BGCOLOR illegal (hex value):" + sbgcolor );
                throw new WMSException( Globals.ERR_BgcolorIllegal, "bgcolor=" + sbgcolor );
            }
            // now we've checked all, we can convert safely
            try
            {
                bgColorRed   = Integer.parseInt( sbgcolor.substring(2,4), 16);
                bgColorGreen = Integer.parseInt( sbgcolor.substring(4,6), 16);
                bgColorBlue  = Integer.parseInt( sbgcolor.substring(6,8), 16);
            }
            catch (NumberFormatException e)
            {
                leaveCritical( "parseMapRequest: done. BGCOLOR illegal (hex value):" + sbgcolor );
                throw new WMSException( Globals.ERR_BgcolorIllegal, "bgcolor=" + sbgcolor );
            }
        }
        
        if (stransparent == null)
            transparent = false;				// default is false
        else
        {
            if (stransparent.equalsIgnoreCase("false"))
                transparent = false;
            else if (stransparent.equalsIgnoreCase("true"))
                transparent = true;
            else
            {
                leaveCritical( "parseMapRequest: done. TRANSPARENT value illegal: " + stransparent );
                throw new WMSException( Globals.ERR_TransparencyIllegal, "transparent=" + stransparent );
            }
        }
        
        
        StringTokenizer strTok1 = new StringTokenizer(slayers, ",");
        StringTokenizer strTok2 = new StringTokenizer(sstyles, ",");
        
        /* some clients need empty layer lists, so we can't deny service -- PB 2004-feb-10
         * if (strTok1.countTokens() != strTok2.countTokens())
         * {
         * leaveCritical( "parseMapRequest: done. number of LAYERS and STYLES do not match." );
         * throw new WMSException( Globals.ERR_LayersStylesMismatch,
         * "styles=" + strTok2.countTokens() + ",  layers=" + strTok1.countTokens() );
         * }
         */
        layerList = new LinkedList();
        LayerStyle ls = null;
        String layer = null;
        String style = null;
        String defaultStyle = "";
        while(strTok1.hasMoreTokens())
        {
            layer = strTok1.nextToken();
            if (strTok2.hasMoreTokens())
            {
                style = strTok2.nextToken();
                if (style == null)
                {
                    style = defaultStyle;
                    talkWarning( "parseMapRequest: replacing empty STYLE name with \'" + style + "\'." );
                }
            }
            else
            {
                style = defaultStyle;
                talkWarning( "parseMapRequest: running out of styles for layer '" + layer + "', using default style '" + style + "'" );
            }
            talkVerbose( "parseMapRequest: associating style '" + style + "' with layer'" + layer + "'" );
            ls = new LayerStyle(layer, style);
            layerList.add(ls);
        }
        leaveVerbose( "parseMapRequest done. request=" + this );
    } // parseMapRequest()
    
    /**
     * accessor method
     * @return a reference to the version string
     **/
    public String getVersion()
    {
        return version;
    }
    
    /**
     * accessor method
     * @return the type of the request
     **/
    public int getRequestType()
    {
        return requestType;
    }
    
    /**
     * accessor method
     * @return the type of the image format
     **/
    public int getImageFormat()
    {
        return imageFormat;
    }
    
    /**
     * accessor method
     * @return the type of exception reporting
     **/
    public int getExceptionType()
    {
        return exceptions;
    }
    
    /**
     * accessor method
     * @return a reference to the SRS string
     **/
    public String getSRS()
    {
        return srs;
    }
    
    /**
     * accessor method
     * @return a reference to the bounding box of the requested area
     **/
    public BoundingBox getBoundingBox()
    {
        return bbox;
    }
    
    /**
     * accessor method
     * @return the requested image width
     **/
    public int getWidth()
    {
        return width;
    }
    
    /**
     * accessor method
     * @return the requested image height
     **/
    public int getHeight()
    {
        return height;
    }
    
    /**
     * accessor method
     * @return a reference to the linked list of LayerStyles objects
     **/
    public List getLayerList()
    {
        return layerList;
    }
    
    /**
     * accessor method
     * @return the BGCOLOR colors red, green, blue, resp. as integers
     **/
    public int getBackgroundColorRed()
    {
        return bgColorRed;
    }
    public int getBackgroundColorGreen()
    {
        return bgColorGreen;
    }
    public int getBackgroundColorBlue()
    {
        return bgColorBlue;
    }
    
    /**
     * accessor method
     * @return request for a transparent image
     **/
    public boolean isTransparent()
    {
        return transparent;
    }
    
    /**
     * accessor method
     * @return the requested time coordinate
     **/
    public int getTime()
    {
        return time;
    }
    
    /**
     * accessor method
     * @return the requested elevation coordinate
     **/
    public double getElevation()
    {
        return elevation;
    }
    
    /**
     * accessor method
     * @return a reference to the SERVICE string
     **/
    public String getService()
    {
        return service;
    }
    
    /**
     * accessor method
     * @return the requested update sequence count
     **/
    public int getUpdateSequence()
    {
        return updateSequence;
    }
    
    /**
     * accessor method
     * @return the fees string
     **/
    public String getFees()
    {
        return fees;
    }
    
    /**
     * accessor method.  put the values for the fees.
     **/
    public void setFees(String theFees)
    {
        fees = theFees;
    }
    
    /**
     * accessor method
     * @return the client of the request
     **/
    public String getClient()
    {
        return client;
    }
    
    public String getCustomDem()
    {
        return customDem;
    }
    
    public String toString()
    {
        return WMSREQ_KEY_VERSION + "=" + version + "; "
                + WMSREQ_KEY_REQUEST + "=" + requestType + "; "
                + WMSREQ_KEY_SRS + "=" + srs + "; "
                + WMSREQ_KEY_BBOX + "=" + bbox + "; "
                + WMSREQ_KEY_WIDTH + "=" + width + "; "
                + WMSREQ_KEY_HEIGHT + "=" + height + "; "
                + WMSREQ_KEY_LAYERS + "=" + layerList + "; "
                + WMSREQ_KEY_FORMAT + "=" + imageFormat + "; "
                + WMSREQ_KEY_BGCOLOR + "={" + bgColorRed + "," + bgColorGreen + "," + bgColorBlue + "}; "
                + WMSREQ_KEY_TRANSPARENT + "=" + transparent + "; "
                + WMSREQ_KEY_EXCEPTIONS + "=" + exceptions + "; "
                + WMSREQ_KEY_TIME + "=" + time + "; "
                + WMSREQ_KEY_ELEVATION + "=" + elevation + "; "
                + WMSREQ_KEY_SERVICE + "=" + service + "; "
                + WMSREQ_KEY_UPDATESEQUENCE + "=" + updateSequence;
    }

} // WmsRequest


