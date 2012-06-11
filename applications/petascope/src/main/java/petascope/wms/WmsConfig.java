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
/************************************************************
 * <pre>
 * This class is used to get the actual configuration options.
 *
 * To retrieve the value of a specific option, the method getOption()
 * is used. This method checks on entry if the disk resident
 * configuration file was changed since its last loading. If this
 * is the case, it is reloaded.
 *
 * In either case, the corresponding value to the given option is
 * returned. If the option was not found the returned string is an
 * empty string, <em>not</em> a <tt>null</tt> value!
 * This makes it possible to use it without checking against
 * <tt>null</tt>.
 * 
 * COMMENTS:
 *
 * </pre>
 *************************************************************/
package petascope.wms;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.TreeMap;
import javax.servlet.http.HttpServlet;
import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WMSException;


public class WmsConfig
{
    private static final Logger log = LoggerFactory.getLogger(WmsConfig.class);
    
    /** WMS specific constants **/
    public static final String CONFIG_PARAMETER_WMS_PREFIX = "WMS";
    // Database
    public static final String PARAMETER_USER = "user";
    public static final String PARAMETER_PASSWD = "passwd";
    public static final String PARAMETER_SERVER = "server";
    public static final String PARAMETER_PORT = "port";
    public static final String PARAMETER_DATABASE = "database";
    // Capabilities
    public static final String PARAMETER_WMS_LAYERS = "layers";
    public static final String PARAMETER_WMS_HOST_NAME = CONFIG_PARAMETER_WMS_PREFIX + "HostName";
    public static final String PARAMETER_WMS_PORT = CONFIG_PARAMETER_WMS_PREFIX + "Port";
    public static final String PARAMETER_WMS_PATH = CONFIG_PARAMETER_WMS_PREFIX + "Path";
    public static final String PARAMETER_WMS_FORMATS = CONFIG_PARAMETER_WMS_PREFIX + "Formats";
    @Deprecated public static final String PARAMETER_SUPPORT_XML_EXCEPTION = CONFIG_PARAMETER_WMS_PREFIX + "SupportXMLException";
    @Deprecated public static final String PARAMETER_SUPPORT_INIMAGE_EXCEPTION = CONFIG_PARAMETER_WMS_PREFIX + "SupportINIMAGEException";
    @Deprecated public static final String PARAMETER_SUPPORT_BLANK_EXCEPTION = CONFIG_PARAMETER_WMS_PREFIX + "SupportBLANKException";
    public static final String PARAMETER_WMS_VENDOR_CAPABILITIES = CONFIG_PARAMETER_WMS_PREFIX + "VendorCapabilities";
    public static final String PARAMETER_WMS_BASE_LAYER_NAME = CONFIG_PARAMETER_WMS_PREFIX + "BaseLayerName";
    //
    public static final String PARAMETER_WMS_UPDATE_SEQUENCE = CONFIG_PARAMETER_WMS_PREFIX + "UpdateSequence";
    public static final String PARAMETER_WMS_TITLE = CONFIG_PARAMETER_WMS_PREFIX + "Title";
    public static final String PARAMETER_WMS_ABSTRACT = CONFIG_PARAMETER_WMS_PREFIX + "Abstract";
    public static final String PARAMETER_WMS_KEYWORDS = CONFIG_PARAMETER_WMS_PREFIX + "Keywords";
    public static final String PARAMETER_WMS_FEES = CONFIG_PARAMETER_WMS_PREFIX + "Fees";
    public static final String PARAMETER_WMS_ACCESS_CONSTRAINTS = CONFIG_PARAMETER_WMS_PREFIX + "AccessConstraints";
    // Contact
    public static final String PARAMETER_WMS_CONTACT_PERSON_NAME = CONFIG_PARAMETER_WMS_PREFIX + "ContactPersonName";
    public static final String PARAMETER_WMS_CONTACT_ORGANIZATION_NAME = CONFIG_PARAMETER_WMS_PREFIX + "ContactOrganizationName";
    public static final String PARAMETER_WMS_CONTACT_POSITION = CONFIG_PARAMETER_WMS_PREFIX + "ContactPosition";
    public static final String PARAMETER_WMS_CONTACT_ADDRESS_TYPE = CONFIG_PARAMETER_WMS_PREFIX + "ContactAddressType";
    public static final String PARAMETER_WMS_CONTACT_ADDRESS = CONFIG_PARAMETER_WMS_PREFIX + "ContactAddress";
    public static final String PARAMETER_WMS_CONTACT_CITY = CONFIG_PARAMETER_WMS_PREFIX + "ContactCity";
    public static final String PARAMETER_WMS_CONTACT_STATE_OR_PROVINCE = CONFIG_PARAMETER_WMS_PREFIX + "ContactStateOrProvince";
    public static final String PARAMETER_WMS_CONTACT_POST_CODE = CONFIG_PARAMETER_WMS_PREFIX + "ContactPostCode";
    public static final String PARAMETER_WMS_CONTACT_COUNTRY = CONFIG_PARAMETER_WMS_PREFIX + "ContactCountry";
    public static final String PARAMETER_WMS_CONTACT_VOICE_TELEPHONE = CONFIG_PARAMETER_WMS_PREFIX + "ContactVoiceTelephone";
    public static final String PARAMETER_WMS_CONTACT_FACSIMILE_TELEPHONE = CONFIG_PARAMETER_WMS_PREFIX + "ContactFacsimileTelephone";
    public static final String PARAMETER_WMS_CONTACT_ELECTRONIC_MAIL_ADDRESS = CONFIG_PARAMETER_WMS_PREFIX + "ContactElectronicMailAddress";
    // Layer
    public static final String PARAMETER_WMS_SUFFIX_TITLE = "Title";
    public static final String PARAMETER_WMS_SUFFIX_SRS = "SRS";
    public static final String PARAMETER_WMS_SUFFIX_AUTHORITY = "Authority";
    public static final String PARAMETER_WMS_SUFFIX_BBOX  = "BBox";
    public static final String PARAMETER_WMS_SUFFIX_LATLON_BOX = "LatLonBox";
    public static final String PARAMETER_WMS_SUFFIX_ATTRIBUTION_URL = "AttributionURL";
    public static final String PARAMETER_WMS_SUFFIX_ATTRIBUTION_TITLE = "AttributionTitle";
    public static final String PARAMETER_WMS_SUFFIX_LOGO_WIDTH = "LogoWidth";
    public static final String PARAMETER_WMS_SUFFIX_LOGO_HEIGHT = "LogoHeight";
    public static final String PARAMETER_WMS_SUFFIX_LOGO_URL = "LogoURL";
    public static final String PARAMETER_WMS_SUFFIX_LOGO_FORMAT = "LogoFormat";
    public static final String PARAMETER_WMS_SUFFIX_FEATURE_URL = "FeatureURL";
    public static final String PARAMETER_WMS_SUFFIX_STYLES = "Styles";
    public static final String PARAMETER_WMS_SUFFIX_MAPTYPE = "MapType";
    // Style
    public static final String PARAMETER_WMS_SUFFIX_STYLE_TITLE = "StyleTitle";
    public static final String PARAMETER_WMS_SUFFIX_STYLE_ABSTRACT = "StyleAbstract";
    public static final String PARAMETER_WMS_SUFFIX_STYLE_LEGEND_WIDTH = "StyleLegendWidth";
    public static final String PARAMETER_WMS_SUFFIX_STYLE_LEGEND_HEIGHT = "StyleLegendHeight";
    public static final String PARAMETER_WMS_SUFFIX_STYLE_LEGEND_URL = "StyleLegendURL";
    public static final String PARAMETER_WMS_SUFFIX_STYLE_SHEET_URL = "StyleSheetURL";
    public static final String PARAMETER_WMS_SUFFIX_RASQL_OP = "RasQLOP";
    
    //////////////////////////////////////////////////
    
    /**
     * The unique instance of this class
     */
    private static WmsConfig myInstance = null;
    
    /**
     * Stores the configuration keyword/value pairs.
     */
    private static Hashtable options = null;
    
    /**
     * Offers access to the unique instance of this class.
     */
    static WmsConfig getInstance()
    {
        if (myInstance == null)
            myInstance = new WmsConfig();
        
        // Add defaults
        
        return myInstance;
    }
    
    /**
     * drop old config object (if any), instantiate new
     */
    static WmsConfig resetInstance()
    {
        myInstance = new WmsConfig();		// unlink old object if there was one
        return getInstance();
    }
    
    /**
     * /**
     * Private constructor
     */
    private WmsConfig()
    {
        if(options == null)
            options  = new Hashtable();
    }
    
    void initFromDatabase(HttpServlet servlet) throws ServletException {
        log.info("Config.initFromDatabase");
        if (options == null)
            options = new Hashtable();
        try {
            DatabaseConfigAdapter.translate(servlet, options);
            log.debug(dumpSettings());
        }
        catch (WMSException e) {
            throw new ServletException(e.toString());
        }
        log.info("Config.initFromDatabase");
    }
    
    String dumpSettings() throws WMSException {
        if (options == null)
            throw new WMSException(309);
        Enumeration e = options.keys();
        String result = "";
        while (e.hasMoreElements())
        {
            String key = (String)e.nextElement();
            result += key + "=" + options.get(key) + "\n";
        }
        return result;
    }
    /**
     * Initialisation from config file.
     */
    void initFromFile(String fileName) throws ServletException
    {
        log.info( "Config.initFromFile start: file name = " + fileName);
        log.info( "reading configuration file " + fileName);
        
        if( fileName == null )
        {
            log.error( "Config.initFromFile: No configFile provided." );
            throw new ServletException( "Parameter configFile missing." );
        }
        
        File             configFile;
        RandomAccessFile file;
        String           line;
        
        try {
            configFile = new File( fileName );
            file = new RandomAccessFile(configFile, "r");
            while( (line = file.readLine()) != null )
            {
                line = line.trim();
                if( (line == null)
                        || (line.equals(""))		// empty lines
                        || (line.startsWith("#")) )	// lines starting with '#' are comments
                    ;					// ignore them
                else
                {
                    int pos;
                    pos = line.indexOf("=");
                    if( pos > 0 )
                        options.put(line.substring(0, pos).trim(), line.substring(pos+1).trim());
                }
            }
            file.close();
            printHashTable();
        }
        catch(IOException e1)
        {
            log.error( "Error reading configuration file: " + e1.getMessage() );
            log.error( "Config.initFromFile: cannot evaluate config file." );
            throw new ServletException(e1.getMessage());
        }
        
        log.info( "Config.initFromFile done." );
    } // initFromFile()
    
    void initFromInitArgs(ServletConfig config) //throws ServletException
    {
        Enumeration keys = config.getInitParameterNames();
        
        while(keys.hasMoreElements())
        {
            String key = (String)keys.nextElement();
            
            String value=config.getInitParameter(key);
            value = value.replace('\\', ',');
            
            options.put(key,value);
            log.info( "+++ reading from init args: " + key + "=" + value);
        }
    }
    
    String getRequiredParameter(String parameterName) throws NullPointerException
    {
        log.debug("getRequiredParameter(" + parameterName + ")" );
        String value = (String)options.get(parameterName);
        // changed to allow "" parameters in particular for not so relevant attributes -- PB 2009-jan-26
        // if ((value == null) || (value.length() == 0))
        if (value == null)
        {
            throw new NullPointerException("Error: mandatory raswms parameter '" + parameterName + "' is missing.");
        }
        return value;
    }
    
    /* 
     * NOTE(pcampalani): Hashtable (`options' class) does not accept NULL values 
     * (nor in keys), so it has no point to search for NULL values into it: 
     * a NullPointerException would be raised when database is loaded.
     * An HashMap might be used for the `options' object but:
     *     1. Hashtable is synchronized, whereas HashMap is not. This makes HashMap 
     *        better for non-threaded applications, as unsynchronized Objects 
     *        typically perform better than synchronized ones.
     *     2. Hashtable does not allow null keys or values. HashMap allows one null
     *        key and any number of null values.
     * Meanwhile I'll force default /undefined/ String for optional fields in the 
     * database definition, like `initgeo' does.
     */
    String getOptionalParameter(String parameterName)
    {        
        log.debug("getOptionalParameter(" + parameterName + ")" );
        String value = (String)options.get(parameterName);
        if ((value == null) || (value.length() == 0) )
            value = null;
        
        return value;
    }
    
    int getRequiredIntegerParameter(String parameterName)
    {
        String value = getRequiredParameter(parameterName);
        
        int intValue = 0;
        try {
            intValue = Integer.parseInt(value);
        }
        catch (NumberFormatException e)
        {
            throw new NullPointerException("Error: bad integer value '" + value + "' for parameter '" + parameterName );
        }
        return intValue;
    }
    
    int getOptionalIntegerParameter(String parameterName, int defaultValue)
    {
        String value = getOptionalParameter(parameterName);
        
        if(value == null)
            return defaultValue;
        
        int intValue = 0;
        try {
            intValue = Integer.parseInt(value);
        }
        catch (NumberFormatException e)
        {
            throw new NullPointerException("Error: bad integer value '" + value + "' for parameter '" + parameterName );
        }
        return intValue;
    }
    
    //########### Compound initialisation stuff ##########################
    
    WmsCapabilities.WmsContact createContact()
    {
        String WMSContactPersonName            = getOptionalParameter(PARAMETER_WMS_CONTACT_PERSON_NAME);
        String WMSContactOrganizationName      = getOptionalParameter(PARAMETER_WMS_CONTACT_ORGANIZATION_NAME);
        String WMSContactPosition              = getOptionalParameter(PARAMETER_WMS_CONTACT_POSITION);
        String WMSContactAddressType           = getOptionalParameter(PARAMETER_WMS_CONTACT_ADDRESS_TYPE);
        String WMSContactAddress               = getOptionalParameter(PARAMETER_WMS_CONTACT_ADDRESS);
        String WMSContactCity                  = getOptionalParameter(PARAMETER_WMS_CONTACT_CITY);
        String WMSContactStateOrProvince       = getOptionalParameter(PARAMETER_WMS_CONTACT_STATE_OR_PROVINCE);
        String WMSContactPostCode              = getOptionalParameter(PARAMETER_WMS_CONTACT_POST_CODE);
        String WMSContactCountry               = getOptionalParameter(PARAMETER_WMS_CONTACT_COUNTRY);
        String WMSContactVoiceTelephone        = getOptionalParameter(PARAMETER_WMS_CONTACT_VOICE_TELEPHONE);
        String WMSContactFacsimileTelephone    = getOptionalParameter(PARAMETER_WMS_CONTACT_FACSIMILE_TELEPHONE);
        String WMSContactElectronicMailAddress = getOptionalParameter(PARAMETER_WMS_CONTACT_ELECTRONIC_MAIL_ADDRESS);
        
        if (WMSContactElectronicMailAddress != null || WMSContactFacsimileTelephone != null || WMSContactVoiceTelephone != null || (WMSContactCountry != null && WMSContactPostCode != null && WMSContactStateOrProvince != null && WMSContactCity != null && WMSContactAddress != null && WMSContactAddressType != null) || WMSContactPosition !=null || (WMSContactOrganizationName != null && WMSContactPersonName != null))
            return new WmsCapabilities.WmsContact(WMSContactPersonName, WMSContactOrganizationName, WMSContactPosition, WMSContactAddressType, WMSContactAddress, WMSContactCity, WMSContactStateOrProvince, WMSContactPostCode, WMSContactCountry, WMSContactVoiceTelephone, WMSContactFacsimileTelephone, WMSContactElectronicMailAddress);
        else
            return null;
    }
    
    WmsCapabilities.WmsCapability  createCapability() throws WMSException
    {
        LayerContainer wmslayermap   = buildWMSLayerMapAndLayers( getRequiredParameter(PARAMETER_WMS_LAYERS) );
        String WMSHostName = getRequiredParameter(PARAMETER_WMS_HOST_NAME);
        int wmsport = getRequiredIntegerParameter(PARAMETER_WMS_PORT);
        String WMSPath = getRequiredParameter(PARAMETER_WMS_PATH);
        List wmsformatslist = buildWMSFormatsList( getRequiredParameter(PARAMETER_WMS_FORMATS) );
        
        // ---- these parameters are deprecated & ignored, raswms supports all exception types -- PB 2003-jun-09
        String wmssupportxmlexceptionString = getOptionalParameter(PARAMETER_SUPPORT_XML_EXCEPTION);
        if ( wmssupportxmlexceptionString != null)
            log.warn( "WmsCapabilities.WmsCapability:createCapability: ignoring deprecated parameter WMSSupportXMLException." );
        boolean wmssupportxmlexception = true;
        
        String wmssupportinimageexceptionString = getOptionalParameter(PARAMETER_SUPPORT_INIMAGE_EXCEPTION);
        if ( wmssupportinimageexceptionString != null)
            log.warn( "WmsCapabilities.WmsCapability:createCapability: ignoring deprecated parameter WMSSupportINIMAGEException." );
        boolean wmssupportinimageexception = true;
        
        String wmssupportblankexceptionString = getOptionalParameter(PARAMETER_SUPPORT_BLANK_EXCEPTION);
        if ( wmssupportblankexceptionString != null)
            log.warn( "WmsCapabilities.WmsCapability:createCapability: ignoring deprecated parameter WMSSupportBLANKException." );
        boolean wmssupportblankexception = true;
        // ------------------------------------------------------------------------------------------------------
        
        String WMSVendorCapabilities = getOptionalParameter(PARAMETER_WMS_VENDOR_CAPABILITIES);
        String WMSBaseLayerName = getRequiredParameter(PARAMETER_WMS_BASE_LAYER_NAME);
        
        log.info( "WmsCapabilities.WmsCapability:createCapability: done." );
        
        return new WmsCapabilities.WmsCapability(WMSHostName, wmsport, WMSPath, wmsformatslist, wmssupportxmlexception, wmssupportinimageexception, wmssupportblankexception, WMSVendorCapabilities, wmslayermap, WMSBaseLayerName);
    }
    
    protected LayerContainer buildWMSLayerMapAndLayers(String layerParam) throws WMSException
    {
        log.info( "WmsCapabilities.LayerContainer::buildWMSLayerMapAndLayers( " + layerParam + " )" );
        //Map wmslayermap = new TreeMap();
        
        LayerContainer wmslayermap = new LayerContainer();
        
        StringTokenizer stLayer = new StringTokenizer(layerParam, ";");
        while (stLayer.hasMoreElements())
        { //go through the layers
            String layer = stLayer.nextElement().toString();
            WmsCapabilities.WmsLayer wmslayer = createLayer(layer);
            // log.debug( "WmsCapabilities.LayerContainer::buildWMSLayerMapAndLayers(): adding layer " + wmslayer );
            wmslayermap.add(layer, wmslayer);
        }
        
        log.info( "WmsCapabilities.LayerContainer::buildWMSLayerMapAndLayers() -> " + wmslayermap );
        return wmslayermap;
    }
    
    protected LinkedList buildWMSFormatsList(String WMSFormats)
    {
        LinkedList wmsformatslist = new LinkedList();
        StringTokenizer stFormats = new StringTokenizer(WMSFormats, ";");
        while (stFormats.hasMoreElements())
        {
            String format = stFormats.nextElement().toString();
            if ((format == null) || (format.length() == 0))
            {
                throw new NullPointerException("Error: null or empty format.");
            }
            if      (format.equals(WmsRequest.PNGSTRING))
                wmsformatslist.add(new Integer(WmsRequest.PNG));
            else if (format.equals(WmsRequest.TIFFSTRING))
                wmsformatslist.add(new Integer(WmsRequest.TIFF));
            else if (format.equals(WmsRequest.JPEGSTRING))
                wmsformatslist.add(new Integer(WmsRequest.JPEG));
            else
            {
                throw new NullPointerException("Error: unknown WMSFormats parameter, only allowed "
                        + "'" + WmsRequest.JPEGSTRING + "', "
                        + "'" + WmsRequest.PNGSTRING  + "', or "
                        + "'" + WmsRequest.TIFFSTRING + "'");
            }
        }
        
        return wmsformatslist;
    }
    
    protected WmsCapabilities.WmsLayer createLayer(String layer) throws WMSException
    {
        log.info("createLayer(" + layer + ")");
        
        WmsCapabilities.WmsLayer wmslayer = null;
        StringTokenizer stSLayer = new StringTokenizer(getRequiredParameter(layer), ";");
        RasPersistentScaleLayer persScaleLayer = null;
        Map persScaleLayerMap = new TreeMap();
        while (stSLayer.hasMoreElements())
        { // go through the persistent layers
            String sLayer = stSLayer.nextElement().toString();
            log.debug( "now processing layer: " + sLayer );
            if ((sLayer == null) || (sLayer.length() == 0))
            {
                throw new NullPointerException("Error: argument to '" + layer + "' must not contain empty or null values (like 'foo,,bar')");
            }
            // get first persistent layer
            String sLayerArgs = getRequiredParameter( sLayer );
            
            // get ground resolution/coordinates string
            StringTokenizer stSLayerPars = new StringTokenizer(sLayerArgs, ";");
            
            if (stSLayerPars.countTokens() != 6)
            {
                throw new NullPointerException("Error: bad value for '" + sLayer + "'; must consist of 6 doubles separated by ';' (ground_resolution_x; ground_resolution_y; min_x; min_y; max_x; max_y), but is: " + sLayerArgs );
            }
            double minX = 0;
            double minY = 0;
            double maxX = 0;
            double maxY = 0;
            double groundResolutionX = 0;
            double groundResolutionY = 0;
            try
            {
                groundResolutionX = Double.parseDouble( stSLayerPars.nextElement().toString() );
                groundResolutionY = Double.parseDouble( stSLayerPars.nextElement().toString() );
                minX = Double.parseDouble( stSLayerPars.nextElement().toString() );
                minY = Double.parseDouble( stSLayerPars.nextElement().toString() );
                maxX = Double.parseDouble( stSLayerPars.nextElement().toString() );
                maxY = Double.parseDouble( stSLayerPars.nextElement().toString() );
            }
            catch (NumberFormatException e)
            {
                throw new NullPointerException("Error: bad float number for '" + sLayer + "'" );
            }
            persScaleLayer = new RasPersistentScaleLayer(sLayer, new BoundingBox(minX, minY, maxX, maxY, groundResolutionX, groundResolutionY));
            persScaleLayerMap.put(sLayer, persScaleLayer);
        }//now we have the persistent scale layers for the current layer
        //retrieve wms meta data for each layer
        //layers within layers are not supported
        //hence inheritance of attributes neither, you need to specify each
        
        String WMSLayerTitle = getRequiredParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_TITLE);
        String WMSLayerSRS = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_SRS);
        String WMSLayerAuthority = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_AUTHORITY);
        
        double wmslayerminx = 0;
        double wmslayerminy = 0;
        double wmslayermaxx = 0;
        double wmslayermaxy = 0;
        String WMSLayerBBox = getRequiredParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_BBOX);
        //get the coordinates string
        StringTokenizer stLayerBBoxPars = new StringTokenizer(WMSLayerBBox, ";");
        if (stLayerBBoxPars.countTokens() != 4)
        {
            throw new NullPointerException("Error: argument to '" + CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_BBOX
                    + "' must consist of 4 doubles separated by ';' (min_x; min_y; max_x; max_y)");
        }
        
        try
        {
            wmslayerminx = Double.parseDouble( stLayerBBoxPars.nextElement().toString() );
            wmslayerminy = Double.parseDouble( stLayerBBoxPars.nextElement().toString() );
            wmslayermaxx = Double.parseDouble( stLayerBBoxPars.nextElement().toString() );
            wmslayermaxy = Double.parseDouble( stLayerBBoxPars.nextElement().toString() );
        }
        catch (NumberFormatException e)
        {
            throw new NullPointerException("Error: bad float for '" + CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_BBOX + "': " + e.getMessage());
        }
        
        double wmslayerlatlonminx = 0;
        double wmslayerlatlonminy = 0;
        double wmslayerlatlonmaxx = 0;
        double wmslayerlatlonmaxy = 0;
        String WMSLayerLatLonBox = getRequiredParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_LATLON_BOX);
        //get the coordinates string
        StringTokenizer stLayerLatLonBoxPars = new StringTokenizer(WMSLayerLatLonBox, ";");
        if (stLayerLatLonBoxPars.countTokens() != 4)
        {
            throw new NullPointerException("Error: argument to '" + CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_LATLON_BOX
                    + "' must consist of 4 doubles separated by ';' (min_x; min_y; max_x; max_y)");
        }
        
        try
        {
            wmslayerlatlonminx = Double.parseDouble( stLayerLatLonBoxPars.nextElement().toString() );
            wmslayerlatlonminy = Double.parseDouble( stLayerLatLonBoxPars.nextElement().toString() );
            wmslayerlatlonmaxx = Double.parseDouble( stLayerLatLonBoxPars.nextElement().toString() );
            wmslayerlatlonmaxy = Double.parseDouble( stLayerLatLonBoxPars.nextElement().toString() );
        }
        catch (NumberFormatException e)
        {
            throw new NullPointerException("Error: bad float number for '" + CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_LATLON_BOX + "': " + e.getMessage());
        }
        
        String WMSLayerAttributionURL = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_ATTRIBUTION_URL);
        String WMSLayerAttributionTitle = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_ATTRIBUTION_TITLE);
        int wmslayerlogowidth = getOptionalIntegerParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_LOGO_WIDTH,0);
        int wmslayerlogoheight = getOptionalIntegerParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_LOGO_HEIGHT,0);
        String WMSLayerLogoURL = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_LOGO_URL);
        String WMSLayerLogoFormat = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_LOGO_FORMAT);
        String WMSLayerFeatureURL = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_FEATURE_URL);
        String WMSLayerStyles = getRequiredParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + PARAMETER_WMS_SUFFIX_STYLES);
        
        WmsCapabilities.WmsStyle wmsstyle = null;
        StringTokenizer stStyles = new StringTokenizer(WMSLayerStyles, ";");
        Map wmsstylelist = new HashMap();
        while (stStyles.hasMoreElements())
        {   //get all styles
            String Style = stStyles.nextElement().toString();
            String WMSLayerStyleTitle = getRequiredParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + Style + PARAMETER_WMS_SUFFIX_STYLE_TITLE);
            String WMSLayerStyleAbstract = getRequiredParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + Style + PARAMETER_WMS_SUFFIX_STYLE_ABSTRACT);
            int wmslayerstylelegendwidth = getRequiredIntegerParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + Style + PARAMETER_WMS_SUFFIX_STYLE_LEGEND_WIDTH);
            int wmslayerstylelegendheight = getRequiredIntegerParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + Style + PARAMETER_WMS_SUFFIX_STYLE_LEGEND_HEIGHT);
            String WMSLayerStyleLegendURL = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + Style + PARAMETER_WMS_SUFFIX_STYLE_LEGEND_URL);
            String WMSLayerStyleSheetURL = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + Style + PARAMETER_WMS_SUFFIX_STYLE_SHEET_URL);
            String WMSLayerStyleRasQLOP = getOptionalParameter(CONFIG_PARAMETER_WMS_PREFIX + layer + Style + PARAMETER_WMS_SUFFIX_RASQL_OP);
            
            if (WMSLayerStyleRasQLOP == null)	WMSLayerStyleRasQLOP = Style;
            wmsstyle = new WmsCapabilities.WmsStyle(Style, WMSLayerStyleTitle, WMSLayerStyleAbstract, wmslayerstylelegendwidth, wmslayerstylelegendheight, WMSLayerStyleLegendURL, WMSLayerStyleSheetURL, WMSLayerStyleRasQLOP);
            wmsstylelist.put(Style, wmsstyle);
        }
        wmslayer = new WmsCapabilities.WmsLayer(layer, WMSLayerTitle, WMSLayerSRS, WMSLayerAuthority, wmslayerminx, wmslayerminy, wmslayermaxx, wmslayermaxy, WMSLayerAttributionURL, WMSLayerAttributionTitle, wmslayerlogowidth, wmslayerlogoheight, WMSLayerLogoFormat, WMSLayerLogoURL, WMSLayerFeatureURL, wmsstylelist, persScaleLayerMap, wmslayerlatlonminx, wmslayerlatlonminy, wmslayerlatlonmaxx, wmslayerlatlonmaxy);
        log.info("createLayer(" + layer + ")");
        return wmslayer;
    }
    
    WmsCapabilities createCapabilities(WmsCapabilities.WmsContact wmsContact, WmsCapabilities.WmsCapability myCapability)
    {
        int wmsupdatesequence = getRequiredIntegerParameter(PARAMETER_WMS_UPDATE_SEQUENCE);
        String WMSTitle = getRequiredParameter(PARAMETER_WMS_TITLE);
        String WMSAbstract = getOptionalParameter(PARAMETER_WMS_ABSTRACT);
        List wmskeywordslist = buildOptionalWMSKeywordsList( getOptionalParameter(PARAMETER_WMS_KEYWORDS) );
        String WMSHostName = getRequiredParameter(PARAMETER_WMS_HOST_NAME);
        String WMSFees = getOptionalParameter(PARAMETER_WMS_FEES);
        String WMSAccessConstraints = getOptionalParameter(PARAMETER_WMS_ACCESS_CONSTRAINTS);
        
        return new WmsCapabilities(wmsupdatesequence, WMSTitle, WMSAbstract, wmskeywordslist, WMSHostName, wmsContact, WMSFees, WMSAccessConstraints, myCapability);
    }
    
    protected LinkedList buildOptionalWMSKeywordsList(String WMSKeywords)
    {
        if(WMSKeywords == null) return null;
        
        LinkedList wmskeywordslist = new LinkedList();
        
        StringTokenizer stKeywords = new StringTokenizer(WMSKeywords, ";");
        while (stKeywords.hasMoreElements())
        {
            String keyword = stKeywords.nextElement().toString();
            if ((keyword == null) || (keyword.length() == 0))
            {
                throw new NullPointerException("Error: WMSKeywords parameter must not contain null strings (like 'foo,,bar')");
            }
            wmskeywordslist.add(keyword);
        }
        
        return wmskeywordslist.size()>0 ? wmskeywordslist : null;
    }
    
    //########### Stuff for testing ######################################
    
    public static void main(String arg[])
    {
        System.out.println("Config - main - in");
        
        String configFile = arg[0];
        
        System.out.println("configFile=" + configFile);
        
        WmsConfig config = WmsConfig.getInstance();
        
        try{
            config.initFromFile(configFile);
        }
        catch(ServletException e)
        {
            System.out.println("Error=" + e.getMessage());
            
        }
        
        config.printHashTable();
        
        System.out.println("Config - main - out");
        
    }
    
    static final void printHashTable()
    {
        Enumeration keys = options.keys();
        
        log.info( "+++ current configuration table contents: " );
        
        while(keys.hasMoreElements())
        {
            String key = (String)keys.nextElement();
            String value=(String)options.get(key);
            log.debug( "+++    " + key + "=" + value);
        }
        log.info( "+++ end current configuration table contents." );
    }
}