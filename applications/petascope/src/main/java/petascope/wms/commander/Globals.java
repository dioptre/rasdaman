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
/** ******************************************************************
 * 
 * PURPOSE:
 * This interface contains the global definitions and values.
 * They are either used cross-Java-source, or even in the
 * JavaScript code of commander.js etc.
 * Hence, BEWARE that all changes here must be in sync with all places affected!
 *
 * COMMENTS:
 *
 ****************************************************************** */

package petascope.wms.commander;

public interface Globals
{
    /// Field COMMANDER_VERSION
    public static final String COMMANDER_VERSION        = "1.0";
    
    /// shell used for executing the cmdr_* scripts
    public static final String MY_SHELL                 = "bash";
    
    /**
     * --- web deployment file ---------------------
     * conformance requirement: WEB-INF/web.xml
     */
    
    /// - Field OPTION_JDBC_DATABASE_URL
    public static final String OPTION_JDBC_DATABASE_URL = "url";
    
    /// - Field OPTION_JDBC_USER
    public static final String OPTION_JDBC_USER         = "user";
    
    /// - Field OPTION_JDBC_PASSWD
    public static final String OPTION_JDBC_PASSWD       = "password";
    
    /// - Field OPTION_JDBC_DRIVER
    public static final String OPTION_JDBC_DRIVER       = "driver";
    
    /// - Field OPTION_DEBUG
    public static final String OPTION_DEBUG             = "debug";
    
    /// - Field OPTION_RASDAMAN_USER
    public static final String OPTION_RASDAMAN_USER         = "rasuser";
    
    /// - Field OPTION_RASDAMAN_PASSWD
    public static final String OPTION_RASDAMAN_PASSWD       = "raspassword";
    
    /// - Field OPTION_MSGFILE
    public static final String OPTION_MSGFILE           = "messageFile";
    
    /// - Field OPTION_SCRIPTDIR
    public static final String OPTION_SCRIPTDIR         = "scriptDirectory";
    
    /// - Field OPTION_MGRDIR
    public static final String OPTION_MGRDIR         = "RasMgrDirectory";
    
    /// - Field OPTION_RASAUTH
    public static final String OPTION_RASLOGIN         = "RasLogin";
    
    /**
     * default values for web.xml
     **/
    public static final String DEFAULT_URL              = "jdbc:postgresql:RASBASE";
    public static final String DEFAULT_USER             = "rasdaman";
    public static final String DEFAULT_PASSWD           = "noPass";
    public static final String DEFAULT_DRIVER           = "org.postgresql.Driver";
    public static final String DEFAULT_DEBUG            = "0";
    public static final String DEFAULT_MSGFILE          = "./classes/rasgeo/commander/commander_msgs_en";
    public static final String DEFAULT_RASDAMAN_USER    = "rasadmin";
    public static final String DEFAULT_RASDAMAN_PASSWD  = "rasadmin";
    public static final String DEFAULT_SCRIPTDIR        = "WEB-INF/bin";	// this path is relative to web deployment directory
    public static final String DEFAULT_RASLOGIN         = "rasadmin:d293a15562d3e70b6fdc5ee452eaed40";	// FIXME: not really a good default
    
    // --- JDBC DriverManager login properties keywords ---------------------
    
    public final static String PROPERTY_USER            = "user";
    public final static String PROPERTY_PASSWORD        = "password";
    
    /**
     * scripts used for rasdaman access
     */
    public static final String SCRIPT_COLLCREATE = "cmdr_createpyramidlevel.sh";
    public static final String SCRIPT_COLLDROP   = "cmdr_droppyramidlevel.sh";
    public static final String SCRIPT_COLLGET    = "cmdr_collectionnames.sh";
    public static final String SCRIPT_COLLNEWNAV = "cmdr_newnavigator.sh";
    public static final String SCRIPT_COLLDELNAV = "cmdr_delnavigator.sh";
    
    // --- value of the availability parameter ------------------------------
    
    /// - in case service is not sufficiently complete for operation yet
    public static final int NO_SERVICE = 0;
    /// - in case service is not available (also initial value of the param when service entered in the database)
    public static final int NOT_AVAILABLE = 1;
    /// - in case service is available
    public static final int YES_AVAILABLE = 2;
    
    // --- input fields default values --------------------------------------
    
    // all defaults that boil down to an empty string / null int
    public final static String DEFAULT_EMPTY = "";
    
    // - service
    public final static String DEFAULT_SERVICE_UPDATESEQUENCE 	= "0";
    public final static int    DEFAULT_SERVICE_AVAILABILITY		= NO_SERVICE;
    public final static String DEFAULT_SERVICE_NAME 		= "new service - please give me a name!";
    public final static String DEFAULT_SERVICE_TITLE 		= "give me a title!";
    public final static String DEFAULT_SERVICE_ABSTRACTTEXT 	= DEFAULT_EMPTY;
    public final static String DEFAULT_SERVICE_KEYWORDS 		= DEFAULT_EMPTY;
    public final static String DEFAULT_SERVICE_ACCESSCONSTRAINTS 	= "none";
    public final static String DEFAULT_SERVICE_FEES 		= "none";
    public final static String DEFAULT_SERVICE_HOSTNAME 		= "localhost";
    public final static int    DEFAULT_SERVICE_PORT 		= 8000;
    public final static String DEFAULT_SERVICE_PATH 		= "/rasgeo/servlet/rasogc/";
    public final static String DEFAULT_SERVICE_FORMATS 		= "image/jpeg;image/tiff;image/png";
    public final static String DEFAULT_SERVICE_BASELAYER 		= "-CHANGE-TO-BASE-LAYER-";
    public final static String DEFAULT_SERVICE_VENDORCAPABILITIES 	= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_CONTACTPERSON	= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_CONTACTORGANIZATION	= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_ADDRESSTYPE		= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_ADDRESS		= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_CITY			= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_STATE		= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_POSTCODE		= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_COUNTRY		= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_TEL			= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_FAX			= DEFAULT_EMPTY;
    public static final String DEFAULT_SERVICE_EMAIL		= DEFAULT_EMPTY;
    
    public final static String DEFAULT_SERVICE_RASSERVER 		= "localhost";
    public final static int    DEFAULT_SERVICE_RASPORT 		= 7001;
    public final static String DEFAULT_SERVICE_RASUSER 		= "rasguest";
    public final static String DEFAULT_SERVICE_RASPASS 		= "rasguest";
    public final static String DEFAULT_SERVICE_RASBASE 		= "RASBASE";
    public final static int    DEFAULT_SERVICE_TIMEOUT 		= 60;
    public final static int    DEFAULT_SERVICE_PRECLAIMED 		= 0;
    public final static int    DEFAULT_SERVICE_ONDEMAND 		= 2;
    
    // - layers
    public static final String DEFAULT_LAYER_NAME 			= "new layer - please give me a name!";
    public static final String DEFAULT_LAYER_TITLE 			= "new layer title";
    public static final String DEFAULT_LAYER_SRS 			= "EPSG:";
    public static final String DEFAULT_LAYER_AUTHORITY 		= DEFAULT_EMPTY;
    public static final String DEFAULT_LAYER_LATLONBOXXMIN 		= "-999999.999";
    public static final String DEFAULT_LAYER_LATLONBOXXMAX 		=  "999999.999";
    public static final String DEFAULT_LAYER_LATLONBOXYMIN 		= "-999999.999";
    public static final String DEFAULT_LAYER_LATLONBOXYMAX 		=  "999999.999";
    public static final String DEFAULT_LAYER_BBOXXMIN 		= "-999999.999";
    public static final String DEFAULT_LAYER_BBOXXMAX 		=  "999999.999";
    public static final String DEFAULT_LAYER_BBOXYMIN 		= "-999999.999";
    public static final String DEFAULT_LAYER_BBOXYMAX 		=  "999999.999";
    public static final String DEFAULT_LAYER_ATTRIBUTIONURL 	= DEFAULT_EMPTY;
    public static final String DEFAULT_LAYER_ATTRIBUTIONTITLE 	= DEFAULT_EMPTY;
    public static final int    DEFAULT_LAYER_LOGOWIDTH 		= 1;
    public static final int    DEFAULT_LAYER_LOGOHEIGHT 		= 1;
    public static final String DEFAULT_LAYER_LOGOFORMAT 		= DEFAULT_EMPTY;
    public static final String DEFAULT_LAYER_LOGOURL 		= DEFAULT_EMPTY;
    public static final String DEFAULT_LAYER_FEATUREURL 		= DEFAULT_EMPTY;
    public static final double DEFAULT_LAYER_RESOLUTION 		= 1.0;
    public static final MapType DEFAULT_LAYER_MAPTYPE 		= MapType.RGB;
    
    // - styles
    public static final String DEFAULT_STYLE_NAME 			= "new style - please give me a name!";
    public static final String DEFAULT_STYLE_TITLE 			= "new style title";
    public static final String DEFAULT_STYLE_ABSTRACT 		= "new style abstract";
    public static final int    DEFAULT_STYLE_LEGENDWIDTH 		= 0;
    public static final int    DEFAULT_STYLE_LEGENDHEIGHT 		= 0;
    public static final String DEFAULT_STYLE_LEGENDURL		= DEFAULT_EMPTY;
    public static final String DEFAULT_STYLE_SHEETURL 		= DEFAULT_EMPTY;
    public static final String DEFAULT_STYLE_RASQLOP 		= DEFAULT_EMPTY;
    
    // - pyramids
    public static final String DEFAULT_LEVEL_COLLNAME 		= "changeMe";
    public static final double DEFAULT_LEVEL_SCALEFACTOR 		= 1.0;
    
    // --- KVP request parameter keys ---------------------------------------
    // conformance requirement: Javascript
    
    /**
     * message tag names
     */
    public static final String REQPARAM_RASDAMANMSG  = "rasdamanMsg";
    public static final String REQPARAM_OPMSG        = "opMsg";
    public static final String REQPARAM_STATUSMSG    = "statusText";	// FIXME: inconsistent; change here & in commander.js!
    
    /**
     * KVP command tags and values
     */
    public static final String REQPARAM_ENTITY  = "entity";
    public static final String REQPARAM_COMMAND = "command";
    
    public static final String ENT_SERVICE      = "service";
    public static final String ENT_LAYER        = "layer";
    public static final String ENT_STYLE        = "style";
    public static final String ENT_LEVEL        = "level";
    
    public static final String CMD_DELETE       = "del";
    public static final String CMD_ADD          = "add";
    public static final String CMD_UPDATE       = "upd";
    public static final String CMD_UPDATE_AVAILABILITY  = "upd_availability";
    public static final String CMD_SWAP         = "swap";
    public static final String CMD_CREATE       = "create";
    public static final String CMD_SEQ          = "seq";
    public static final String CMD_SETLATLON    = "setlatlon";
    public static final String CMD_SETCRS       = "setcrs";
    public static final String CMD_START_SERVER       = "srvstart";
    public static final String CMD_STOP_SERVER       = "srvstop";
    public static final String CMD_UPDATE_STATUS       = "updatestatus";
    
    /**
     * KVP request parameter keys
     */
    public static final String REQPARAM_SERVICE_SERVICEID = "serviceId";
    public static final String REQPARAM_SERVICE_UPDATESEQUENCE = "updateSequence";
    public static final String REQPARAM_SERVICE_AVAILABILITY = "availability";
    public static final String REQPARAM_SERVICE_NAME = "name";
    public static final String REQPARAM_SERVICE_TITLE = "title";
    public static final String REQPARAM_SERVICE_ABSTRACT = "abstractText";
    public static final String REQPARAM_SERVICE_KEYWORDS = "keywords";
    public static final String REQPARAM_SERVICE_FEES = "fees";
    public static final String REQPARAM_SERVICE_ACCESSCONSTRAINTS = "accessConstraints";
    public static final String REQPARAM_SERVICE_HOSTNAME = "hostName";
    public static final String REQPARAM_SERVICE_PORT = "port";
    public static final String REQPARAM_SERVICE_PATH = "path";
    public static final String REQPARAM_SERVICE_FORMATS = "formats";
    public static final String REQPARAM_SERVICE_BASELAYER = "baseLayer";
    public static final String REQPARAM_SERVICE_VENDORCAPABILITIES = "vendorCapabilities";
    public static final String REQPARAM_SERVICE_CONTACTPERSON	= "contactPerson";
    public static final String REQPARAM_SERVICE_CONTACTORGANIZATION	= "contactOrganization";
    public static final String REQPARAM_SERVICE_ADDRESSTYPE		= "addressType";
    public static final String REQPARAM_SERVICE_ADDRESS		= "address";
    public static final String REQPARAM_SERVICE_CITY		= "city";
    public static final String REQPARAM_SERVICE_STATE		= "stateOrProvince";
    public static final String REQPARAM_SERVICE_POSTCODE		= "postCode";
    public static final String REQPARAM_SERVICE_COUNTRY		= "country";
    public static final String REQPARAM_SERVICE_TEL			= "contactVoiceTelephone";
    public static final String REQPARAM_SERVICE_FAX			= "contactFacsimileTelephone";
    public static final String REQPARAM_SERVICE_EMAIL		= "contactElectronicMailAddress";
    
    public static final String REQPARAM_SERVERID		= "serverID";
    
    // - layer parameters
    public static final String REQPARAM_LAYER_LAYERID = "layerId";
    public static final String REQPARAM_LAYER_LAYERDETAILS = "layerDetailsId";
    public static final String REQPARAM_LAYER_SERVICEID = "serviceId";
    public static final String REQPARAM_LAYER_LAYERIDSWAP = "layerSwapId";
    public static final String REQPARAM_LAYER_NAME = "layerName";
    public static final String REQPARAM_LAYER_TITLE = "layerTitle";
    public static final String REQPARAM_LAYER_SRS = "srs";
    public static final String REQPARAM_LAYER_AUTHORITY = "authority";
    public static final String REQPARAM_LAYER_LATLONBOXXMIN = "lBoxXmin";
    public static final String REQPARAM_LAYER_LATLONBOXXMAX = "lBoxXmax";
    public static final String REQPARAM_LAYER_LATLONBOXYMIN = "lBoxYmin";
    public static final String REQPARAM_LAYER_LATLONBOXYMAX = "lBoxYmax";
    public static final String REQPARAM_LAYER_BBOXXMIN = "bBoxXmin";
    public static final String REQPARAM_LAYER_BBOXXMAX = "bBoxXmax";
    public static final String REQPARAM_LAYER_BBOXYMIN = "bBoxYmin";
    public static final String REQPARAM_LAYER_BBOXYMAX = "bBoxYmax";
    public static final String REQPARAM_LAYER_ATTRIBUTIONURL = "attrUrl";
    public static final String REQPARAM_LAYER_ATTRIBUTIONTITLE = "attrTitle";
    public static final String REQPARAM_LAYER_LOGOWIDTH = "lWidth";
    public static final String REQPARAM_LAYER_LOGOHEIGHT = "lHeight";
    public static final String REQPARAM_LAYER_LOGOFORMAT = "lFormat";
    public static final String REQPARAM_LAYER_LOGOURL = "lUrl";
    public static final String REQPARAM_LAYER_FEATUREURL = "fUrl";
    public static final String REQPARAM_LAYER_RESOLUTION = "resolution";
    public static final String REQPARAM_LAYER_MAPTYPE = "mapType";
    
    // - servicelayer param
    public static final String REQPARAM_SERVICELAYER_SERVICEID = "serviceid";
    public static final String REQPARAM_SERVICELAYER_LAYERID = "layerid";
    public static final String REQPARAM_SERVICELAYER_LAYERSEQ = "layerseq";
    
    // - style
    public static final String REQPARAM_STYLE_LAYERID = "layerId";
    public static final String REQPARAM_STYLE_STYLEID = "styleId";
    public static final String REQPARAM_STYLE_STYLEIDSWAP = "styleIdSwap";
    public static final String REQPARAM_STYLE_NAME = "styleName";
    public static final String REQPARAM_STYLE_TITLE = "styleTitle";
    public static final String REQPARAM_STYLE_ABSTRACT = "styleAbstract";
    public static final String REQPARAM_STYLE_URL = "sheetUrl";
    public static final String REQPARAM_LEGEND_WIDTH = "lWidth";
    public static final String REQPARAM_LEGEND_HEIGHT = "lHeight";
    public static final String REQPARAM_LEGEND_URL = "lUrl";
    public static final String REQPARAM_RASQL_OPERATION_FRAGMENT = "rasqlOp";
    
    // - pyramid level param
    public static final String REQPARAM_LEVEL_LEVELSID = "levelsId";
    public static final String REQPARAM_LEVEL_LEVELID = "levelId";
    public static final String REQPARAM_LEVEL_LAYERID = "layerId";
    public static final String REQPARAM_LEVEL_COLLNAME = "collName";
    public static final String REQPARAM_LEVEL_SCALEFACTOR = "scaleFactor";
    public static final String REQPARAM_LEVEL_NUM = "levelNum";
    
    /**
     * message components used for AJAX result communication
     */
    public static final String REQPARAM_MSG_SUFFIX   = "Error";
    public static final String REQPARAM_INDEX  ="index";
    
    
    // --- table and attribute names ------------------------------
    // conformance requirement: database schema
    
    public static final String COMMON_TABLE_PREFIX            = "PS_";
    
    // - table Rasgeo_Services
    public static final String TABLE_SERVICE                  = COMMON_TABLE_PREFIX + "Services";
    public static final String TABLE_SERVICE_SERVICEID        = "serviceId";
    public static final String TABLE_SERVICE_AVAILABILITY	  = "availability";
    public static final String TABLE_SERVICE_UPDATESEQUENCE   = "updateSequence";
    public static final String TABLE_SERVICE_NAME             = "name";
    public static final String TABLE_SERVICE_TITLE            = "title";
    public static final String TABLE_SERVICE_ABSTRACT         = "abstract";
    public static final String TABLE_SERVICE_KEYWORDS         = "keywords";
    public static final String TABLE_SERVICE_FEES             = "fees";
    public static final String TABLE_SERVICE_ACCESSCONSTRAINTS = "accessConstraints";
    public static final String TABLE_SERVICE_HOSTNAME         = "hostName";
    public static final String TABLE_SERVICE_PORT             = "port";
    public static final String TABLE_SERVICE_PATH             = "path";
    public static final String TABLE_SERVICE_FORMATS          = "formats";
    public static final String TABLE_SERVICE_BASELAYERNAME    = "baseLayerName";
    public static final String TABLE_SERVICE_VENDORCAPABILITIES = "vendorCapabilities";
    public static final String TABLE_SERVICE_CONTACTPERSON		= "contactPerson";
    public static final String TABLE_SERVICE_CONTACTORGANIZATION	= "contactOrganization";
    public static final String TABLE_SERVICE_ADDRESSTYPE		= "addressType";
    public static final String TABLE_SERVICE_ADDRESS		= "address";
    public static final String TABLE_SERVICE_CITY			= "city";
    public static final String TABLE_SERVICE_STATE			= "stateOrProvince";
    public static final String TABLE_SERVICE_POSTCODE		= "postCode";
    public static final String TABLE_SERVICE_COUNTRY		= "country";
    public static final String TABLE_SERVICE_TEL			= "contactVoiceTelephone";
    public static final String TABLE_SERVICE_FAX			= "contactFacsimileTelephone";
    public static final String TABLE_SERVICE_EMAIL			= "contactElectronicMailAddress";
    public static final String TABLE_SERVICE_DBCONNECTION     = "dbConnection";
    
    // - table Rasgeo_Layers
    public static final String TABLE_LAYER                    = COMMON_TABLE_PREFIX + "Layers";
    public static final String TABLE_LAYER_LAYERID = "layerId";
    public static final String TABLE_LAYER_NAME = "name";
    public static final String TABLE_LAYER_TITLE = "title";
    public static final String TABLE_LAYER_SRS = "srs";
    public static final String TABLE_LAYER_AUTHORITY = "authority";
    public static final String TABLE_LAYER_LATLONXMIN = "latLonBoxXmin";
    public static final String TABLE_LAYER_LATLONXMAX = "latLonBoxXmax";
    public static final String TABLE_LAYER_LATLONYMIN = "latLonBoxYmin";
    public static final String TABLE_LAYER_LATLONYMAX = "latLonBoxYmax";
    public static final String TABLE_LAYER_BBOXXMIN = "bBoxXmin";
    public static final String TABLE_LAYER_BBOXXMAX = "bBoxXmax";
    public static final String TABLE_LAYER_BBOXYMIN = "bBoxYmin";
    public static final String TABLE_LAYER_BBOXYMAX = "bBoxYmax";
    public static final String TABLE_LAYER_ATTRIBUTIONURL = "attributionURL";
    public static final String TABLE_LAYER_ATTRIBUTIONTITLE = "attributionTitle";
    public static final String TABLE_LAYER_LOGOWIDTH = "logoWidth";
    public static final String TABLE_LAYER_LOGOHEIGHT = "logoHeight";
    public static final String TABLE_LAYER_LOGOFORMAT = "logoFormat";
    public static final String TABLE_LAYER_LOGOURL = "logoURL";
    public static final String TABLE_LAYER_FEATUREURL = "featureURL";
    public static final String TABLE_LAYER_RESOLUTION = "resolution";
    public static final String TABLE_LAYER_MAPTYPE = "maptype";
    
    // - table Rasgeo_ServiceLayer
    public static final String TABLE_SERVICELAYER = COMMON_TABLE_PREFIX + "ServiceLayer";
    public static final String TABLE_SERVICELAYER_SERVICEID = "serviceId";
    public static final String TABLE_SERVICELAYER_LAYERID = "layerId";
    public static final String TABLE_SERVICELAYER_LAYERSEQ = "layerSeq";
    
    // - table Rasgeo_DatabaseConnections
    public static final String TABLE_DATABASECONNECTION = COMMON_TABLE_PREFIX + "DatabaseConnections";
    public static final String TABLE_DATABASECONNECTION_CONNECTIONID = "connectionId";
    public static final String TABLE_DATABASECONNECTION_SERVER = "server";
    public static final String TABLE_DATABASECONNECTION_PORT = "port";
    public static final String TABLE_DATABASECONNECTION_USER = "userId";
    public static final String TABLE_DATABASECONNECTION_PASSWD = "passwd";
    public static final String TABLE_DATABASECONNECTION_DATABASE = "database";
    public static final String TABLE_DATABASECONNECTION_SERVERTIMEOUT = "serverTimeOut";
    public static final String TABLE_DATABASECONNECTION_SERVERCONNECTIONSPRECLAIMED = "serverConnectionsPreclaimed";
    public static final String TABLE_DATABASECONNECTION_SERVERCONNECTIONSONDEMAND = "serverConnectionsOnDemand";
    
    // - table Rasgeo_Styles
    public static final String TABLE_STYLES             = COMMON_TABLE_PREFIX + "Styles";
    public static final String TABLE_STYLES_STYLEID = "styleId";
    public static final String TABLE_STYLES_LAYERID = "layerId";
    public static final String TABLE_STYLES_STYLESEQ = "styleSeq";
    public static final String TABLE_STYLES_NAME = "name";
    public static final String TABLE_STYLES_TITLE = "title";
    public static final String TABLE_STYLES_ABSTRACT = "abstract";
    public static final String TABLE_STYLES_LEGENDWIDTH = "legendWidth";
    public static final String TABLE_STYLES_LEGENDHEIGHT = "legendHeight";
    public static final String TABLE_STYLES_LEGENDURL = "legendURL";
    public static final String TABLE_STYLES_SHEETURL = "sheetURL";
    public static final String TABLE_STYLES_RASQLOP = "rasqlOp";
    
    // - table Rasgeo_PyramidLevels
    public static final String TABLE_PYRAMIDLEVELS = COMMON_TABLE_PREFIX + "PyramidLevels";
    public static final String TABLE_PYRAMIDLEVELS_LEVELID = "levelId";
    public static final String TABLE_PYRAMIDLEVELS_LAYERID = "layerId";
    public static final String TABLE_PYRAMIDLEVELS_COLLNAME = "collectionName";
    public static final String TABLE_PYRAMIDLEVELS_SCALEFACTOR = "scaleFactor";
    
    // text constants in HTML file
    public static final String HTML_TITLE           = "rasgeo commander";
    public static final String HTML_SERVICE_DETAILS = "service details";
    public static final String HTML_LAYER_DETAILS   = "layer details";
    public static final String HTML_LEVEL_TITLE     = "pyramid level";
    public static final String HTML_COLON           = ":";
    
    /**
     * names used by JavaScript code (commander.js etc.) - KEEP IN SYNC!
     */
    
    // file names used for HTML: JS, CSS
    public static final String COMMANDER_JS = "commander.js";
    public static final String MGRCOMMANDER_JS = "rasmgrcommander.js";
    public static final String DEBUG_JS     = "debug.js";
    public static final String PROTOTYPE_JS = "prototype.js";
    public static final String JSON_JS      = "json.js";
    public static final String GLOBALS_JS   = "globals.js";
    public static final String DTREE_JS     = "dtree.js";
    public static final String TOOLTIPS_JS  = "wz_tooltip.js";
    
    public static final String CSS_FILE     = "commander.css";
    
    // CSS names
    public static final String CSS_DTREE = "dtree";
    public static final String CSS_ITEMTABLE = "itemTable";
    public static final String CSS_ITEMNAME = "itemName";
    public static final String CSS_ITEMINPUT = "itemInput";
    public static final String CSS_ITEMERROR = "itemError";
    public static final String CSS_ITEMOK = "itemOk";
    public static final String CSS_NAME = "name";
    
    
    // CSS stypes
    public static final String CSS_SRV_TABLE = "srv_table";
    public static final String CSS_SRV_ODD_ROW = "srv_odd_row";
    public static final String CSS_SRV_EVEN_ROW = "srv_even_row";
    public static final String CSS_SRV_IDLE = "srv_idle";
    public static final String CSS_SRV_STOPPED = "srv_stopped";
    public static final String CSS_SRV_BUSY = "srv_busy";
    public static final String CSS_SRV_ERROR = "srv_error";
    
    /// - node id of tree root in HTML page
    public static final String JS_SERVICE_TREE_ROOT = "root";
    public static final int JS_SERVICE_TREE_ROOT_ID = -1;
    
    /// name of JS variable holding foldable tree (keep in sync with commander.js!)
    public static final String NODE_VARNAME = "d";
    
    // HTML constants for text and POST variables, etc.
    public static final String HTML_SERVICES = "list of services defined";
    public static final String HTML_SERVICE  = "service";
    public static final String FORM_SERVICE_ID = "serviceId";
    public static final String HTML_SERVICE_UPDATESEQUENCE_TEXT = "WMS update sequence";
    public static final String HTML_SERVICE_NAME_TEXT = "service name";
    public static final String HTML_SERVICE_NAME_EXPLAIN = "Name of the service, used for machine communication; typically a single word (mandatory).";
    public static final String FORM_SERVICE_NAME = "OGCServiceName";
    public static final String HTML_SERVICE_AVAILABILITY_TEXT = "service availability";
    public static final String HTML_SERVICE_AVAILABILITY_EXPLAIN = "(internal, hidden)";
    public static final String FORM_SERVICE_AVAILABILITY = "Availability";
    public static final String HTML_SERVICE_TITLE_TEXT = "service title";
    public static final String HTML_SERVICE_TITLE_EXPLAIN = "Title of the service, used for human communication; typically a a brief term.";
    public static final String FORM_SERVICE_TITLE = "OGCServiceTitle";
    public static final String HTML_SERVICE_ABSTRACT_TEXT = "service abstract";
    public static final String HTML_SERVICE_ABSTRACT_EXPLAIN = "Descriptive narrative providing more information about the service (mandatory).";
    public static final String FORM_SERVICE_ABSTRACT = "OGCAbstract";
    public static final String HTML_SERVICE_KEYWORDS_TEXT = "service keywords";
    public static final String HTML_SERVICE_KEYWORDS_EXPLAIN = "keywords to help catalog searching (optional).";
    public static final String FORM_SERVICE_KEYWORDS = "OGCKeywords";
    public static final String HTML_SERVICE_FEES_TEXT = "service fees";
    public static final String HTML_SERVICE_FEES_EXPLAIN = "Fees for accessing the service, for human communication (optional).";
    public static final String FORM_SERVICE_FEES = "OGCFees";
    public static final String HTML_SERVICE_ACCCONSTR_TEXT = "service access constraints";
    public static final String HTML_SERVICE_ACCCONSTR_EXPLAIN = "Access constraints on this service, for human communication (optional).";
    public static final String FORM_SERVICE_ACCCONSTR = "OGCAccessConstraints";
    public static final String HTML_SERVICE_HOSTNAME_TEXT = "service host name";
    public static final String HTML_SERVICE_HOSTNAME_EXPLAIN = "Host name of local machine, as visible to clients (mandatory).";
    public static final String FORM_SERVICE_HOSTNAME = "OGCHostname";
    public static final String HTML_SERVICE_PORT_TEXT = "service port";
    public static final String HTML_SERVICE_PORT_EXPLAIN = "Port on local machine at which the service is accessible (mandatory).";
    public static final String FORM_SERVICE_PORT = "OGCPort";
    public static final String HTML_SERVICE_PATH_TEXT = "service path";
    public static final String HTML_SERVICE_PATH_EXPLAIN = "Service path on local machine (mandatory).";
    public static final String FORM_SERVICE_PATH = "OGCPath";
    public static final String HTML_SERVICE_FORMATS_TEXT = "formats supported";
    public static final String HTML_SERVICE_FORMATS_EXPLAIN = "Data formats supported for GetMap, specified as whitespace-separated list of MIME types (mandatory).";
    public static final String FORM_SERVICE_FORMATS = "OGCFormats";
    public static final String HTML_SERVICE_BASELAYER_TEXT = "base layer";
    public static final String HTML_SERVICE_BASELAYER_EXPLAIN = "Base layer (currently unused; optional).";
    public static final String FORM_SERVICE_BASELAYER = "OGCBaseLayer";
    public static final String HTML_SERVICE_VENDOR_TEXT = "vendor capabilities";
    public static final String HTML_SERVICE_VENDOR_EXPLAIN = "Human-readable description of additional vendor-specific functionality (optional).";
    public static final String FORM_SERVICE_VENDOR = "OGCVendorCapabilities";
    
    public static final String HTML_SERVICE_CONTACTPERSON_TEXT = "contact person";
    public static final String HTML_SERVICE_CONTACTPERSON_EXPLAIN = "Name of contact person for this service.";
    public static final String FORM_SERVICE_CONTACTPERSON = "contactPerson";
    public static final String HTML_SERVICE_CONTACTORGANIZATION_TEXT = "contact organization";
    public static final String HTML_SERVICE_CONTACTORGANIZATION_EXPLAIN = "Name of organization responsible for this service.";
    public static final String FORM_SERVICE_CONTACTORGANIZATION = "contactOrganization";
    public static final String HTML_SERVICE_ADDRESSTYPE_TEXT = "address type";
    public static final String HTML_SERVICE_ADDRESSTYPE_EXPLAIN = "Type of address (usually: postal) of service responsible.";
    public static final String FORM_SERVICE_ADDRESSTYPE = "addressType";
    public static final String HTML_SERVICE_ADDRESS_TEXT = "address";
    public static final String HTML_SERVICE_ADDRESS_EXPLAIN = "Address of service responsible.";
    public static final String FORM_SERVICE_ADDRESS = "address";
    public static final String HTML_SERVICE_CITY_TEXT = "city";
    public static final String HTML_SERVICE_CITY_EXPLAIN = "City of service responsible.";
    public static final String FORM_SERVICE_CITY = "city";
    public static final String HTML_SERVICE_STATE_TEXT = "state or province";
    public static final String HTML_SERVICE_STATE_EXPLAIN = "State or province of service responsible.";
    public static final String FORM_SERVICE_STATE = "stateOrProvince";
    public static final String HTML_SERVICE_POSTCODE_TEXT = "post code";
    public static final String HTML_SERVICE_POSTCODE_EXPLAIN = "Post code of service responsible.";
    public static final String FORM_SERVICE_POSTCODE = "postCode";
    public static final String HTML_SERVICE_COUNTRY_TEXT = "country";
    public static final String HTML_SERVICE_COUNTRY_EXPLAIN = "Country of service responsible.";
    public static final String FORM_SERVICE_COUNTRY = "country";
    public static final String HTML_SERVICE_TEL_TEXT = "contact telephone";
    public static final String HTML_SERVICE_TEL_EXPLAIN = "Contact tel. number of service responsible.";
    public static final String FORM_SERVICE_TEL = "contactVoiceTelephone";
    public static final String HTML_SERVICE_FAX_TEXT = "contact fax";
    public static final String HTML_SERVICE_FAX_EXPLAIN = "Contact fax number of service responsible.";
    public static final String FORM_SERVICE_FAX = "contactFacsimileTelephone";
    public static final String HTML_SERVICE_EMAIL_TEXT = "contact email";
    public static final String HTML_SERVICE_EMAIL_EXPLAIN = "Contact email of service responsible.";
    public static final String FORM_SERVICE_EMAIL = "contactElectronicMailAddress";
    
    public static final String HTML_SERVICE_DBCONN_TEXT = "database connection";
    public static final String HTML_SERVICE_DBCONN_EXPLAIN = "(internal)";
    public static final String FORM_SERVICE_DBCONN = "RasConnection";
    public static final String HTML_SERVICE_RASSERVER_TEXT = "rasdaman server";
    public static final String HTML_SERVICE_RASSERVER_EXPLAIN = "Web service url at which rasdaman database is accessible (mandatory).";
    public static final String FORM_SERVICE_RASSERVER = "RasServer";
    public static final String HTML_SERVICE_RASPORT_TEXT = "rasdaman port";
    public static final String HTML_SERVICE_RASPORT_EXPLAIN = "Web service port at which rasdaman server listens (usually: 7001; mandatory).";
    public static final String FORM_SERVICE_RASPORT = "RasPort";
    public static final String HTML_SERVICE_RASUSERID_TEXT = "rasdaman user id";
    public static final String HTML_SERVICE_RASUSERID_EXPLAIN = "Rasdaman database user login (mandatory).";
    public static final String FORM_SERVICE_RASUSERID = "RasUser";
    public static final String HTML_SERVICE_RASPASSWD_TEXT = "rasdaman password";
    public static final String HTML_SERVICE_RASPASSWD_EXPLAIN = "Rasdaman database login password (mandatory).";
    public static final String FORM_SERVICE_RASPASSWD = "RasPasswd";
    public static final String HTML_SERVICE_RASDB_TEXT = "rasdaman database";
    public static final String HTML_SERVICE_RASDB_EXPLAIN = "Rasdaman database name (usually: RASBASE).";
    public static final String FORM_SERVICE_RASDB = "RasDb";
    public static final String HTML_SERVICE_RASTIMEOUT_TEXT = "rasdaman server timeout";
    public static final String HTML_SERVICE_RASTIMEOUT_EXPLAIN = "Timeout after which rasdaman server force-closes connection (check rasdaman setup; deprecated).";
    public static final String FORM_SERVICE_RASTIMEOUT = "RasTimeout";
    public static final String HTML_SERVICE_CONNPRE_TEXT = "rasdaman preclaimed connections";
    public static final String HTML_SERVICE_CONNPRE_EXPLAIN = "Number of rasdaman server connections claimed during startup and held over service runtime (mandatory).";
    public static final String FORM_SERVICE_CONNPRE = "RasConnPre";
    public static final String HTML_SERVICE_CONNDMD_TEXT = "rasdaman ad-hoc connections";
    public static final String HTML_SERVICE_CONNDMD_EXPLAIN = "Rasdaman connections acquired on demand, beyond the preclaimed pool (mandatory).";
    public static final String FORM_SERVICE_CONNDMD = "RasConnDemand";
    
    // layer
    public static final String HTML_LAYERS = "list of layers";
    public static final String HTML_LAYER  = "layer";
    public static final String FORM_LAYER_ID = "layerId";
    public static final String HTML_LAYER_NAME_TEXT = "layer name";
    public static final String HTML_LAYER_NAME_EXPLAIN = "Externally visible name of this layer, usually one word (mandatory).";
    public static final String FORM_LAYER_NAME = "Name";
    public static final String HTML_LAYER_TITLE_TEXT = "layer title";
    public static final String HTML_LAYER_TITLE_EXPLAIN = "Title of this layer, used for human communication; typically a a brief term.";
    public static final String FORM_LAYER_TITLE = "Title";
    public static final String HTML_LAYER_AUTH_TEXT = "authority";
    public static final String HTML_LAYER_AUTH_EXPLAIN = "Narrative of the instance in charge of this service (optional).";
    public static final String FORM_LAYER_AUTH = "Authority";
    public static final String HTML_LAYER_SRS_TEXT = "SRS";
    public static final String HTML_LAYER_SRS_EXPLAIN = "Coordinate reference system, in EPSG:nnnnn notation (currently only one supported; optional).";
    public static final String FORM_LAYER_SRS = "SRS";
    public static final String HTML_LAYER_BBOXXMIN_TEXT = "bounding box xmin";
    public static final String HTML_LAYER_BBOXXMIN_EXPLAIN = "Eastern limit of map layer, expressed in the above SRS (optional).";
    public static final String FORM_LAYER_BBOXXMIN = "BBOXXMIN";
    public static final String HTML_LAYER_BBOXXMAX_TEXT = "bounding box xmax";
    public static final String HTML_LAYER_BBOXXMAX_EXPLAIN = "Western limit of map layer, expressed in the above SRS (optional).";
    public static final String FORM_LAYER_BBOXXMAX = "BBOXXMAX";
    public static final String HTML_LAYER_BBOXYMIN_TEXT = "bounding box ymin";
    public static final String HTML_LAYER_BBOXYMIN_EXPLAIN = "Northern limit of map layer, expressed in the above SRS (optional).";
    public static final String FORM_LAYER_BBOXYMIN = "BBOXYMIN";
    public static final String HTML_LAYER_BBOXYMAX_TEXT = "bounding box ymax";
    public static final String HTML_LAYER_BBOXYMAX_EXPLAIN = "Southern limit of map layer, expressed in the above SRS (optional).";
    public static final String FORM_LAYER_BBOXYMAX = "BBOXYMAX";
    public static final String HTML_LAYER_LLBOXXMIN_TEXT = "lat/lon box xmin";
    public static final String HTML_LAYER_LLBOXXMIN_EXPLAIN = "Western limit of map layer, expressed in WGS1984 lat/lon (mandatory).";
    public static final String FORM_LAYER_LLBOXXMIN = "LBBOXXMIN";
    public static final String HTML_LAYER_LLBOXXMAX_TEXT = "lat/lon box xmax";
    public static final String HTML_LAYER_LLBOXXMAX_EXPLAIN = "Eastern limit of map layer, expressed in WGS1984 lat/lon (mandatory).";
    public static final String FORM_LAYER_LLBOXXMAX = "LBBOXXMAX";
    public static final String HTML_LAYER_LLBOXYMIN_TEXT = "lat/lon box ymin";
    public static final String HTML_LAYER_LLBOXYMIN_EXPLAIN = "Northern limit of map layer, expressed in WGS1984 lat/lon (mandatory).";
    public static final String FORM_LAYER_LLBOXYMIN = "LBBOXYMIN";
    public static final String HTML_LAYER_LLBOXYMAX_TEXT = "lat/lon box ymax";
    public static final String HTML_LAYER_LLBOXYMAX_EXPLAIN = "Southern limit of map layer, expressed in WGS1984 lat/lon (mandatory).";
    public static final String FORM_LAYER_LLBOXYMAX = "LBBOXYMAX";
    public static final String HTML_LAYER_ATTRURL_TEXT = "attribution URL";
    public static final String HTML_LAYER_ATTRURL_EXPLAIN = "URL of provider of this Layer (optional).";
    public static final String FORM_LAYER_ATTRURL = "AttributionUrl";
    public static final String HTML_LAYER_ATTRTITLE_TEXT = "attribution title";
    public static final String HTML_LAYER_ATTRTITLE_EXPLAIN = "Descriptive title string for the provider of this layer (optional).";
    public static final String FORM_LAYER_ATTRTITLE = "AttributionTitle";
    public static final String HTML_LAYER_LWIDTH_TEXT = "logo width";
    public static final String HTML_LAYER_LWIDTH_EXPLAIN = "Width (in pixels) of this layer&quot;s provider logo (optional).";
    public static final String FORM_LAYER_LWIDTH = "LogoWidth";
    public static final String HTML_LAYER_LHEIGHT_TEXT = "logo height";
    public static final String HTML_LAYER_LHEIGHT_EXPLAIN = "Height (in pixels) of this layer&quot;s provider logo (optional).";
    public static final String FORM_LAYER_LHEIGHT = "LogoHeight";
    public static final String HTML_LAYER_LFORMAT_TEXT = "logo format";
    public static final String HTML_LAYER_LFORMAT_EXPLAIN = "Format (as MIME type) of this layer&quot;s provider logo (optional).";
    public static final String FORM_LAYER_LFORMAT = "LogoFormat";
    public static final String HTML_LAYER_LURL_TEXT = "logo url";
    public static final String HTML_LAYER_LURL_EXPLAIN = "URL of this layer&quot;s provider logo (optional).";
    public static final String FORM_LAYER_LURL = "LogoUrl";
    public static final String HTML_LAYER_FURL_TEXT = "feature url";
    public static final String HTML_LAYER_FURL_EXPLAIN = "URL to a list of the features represented in a Layer (optional).";
    public static final String FORM_LAYER_FURL = "FeatureUrl";
    public static final String HTML_LAYER_RESOLUTION_TEXT = "resolution";
    public static final String HTML_LAYER_RESOLUTION_EXPLAIN = "Pixel resolution of the layer (any unit possible, but must be used consistently across this service; mandatory).";
    public static final String FORM_LAYER_RESOLUTION = "Resolution";
    public static final String HTML_LAYER_MAPTYPE_TEXT = "map type";
    // thematic currently not supported:
    // public static final String HTML_LAYER_MAPTYPE_EXPLAIN = "Map type, one of: grey (for greyscale maps), RGB (for color maps), DEM (for Digital Elevation Data), or thematic (for thematic maps) - (mandatory).";
    public static final String HTML_LAYER_MAPTYPE_EXPLAIN = "Map type, one of: grey (for greyscale maps), RGB (for color maps), DEM (for Digital Elevation Data) - (mandatory).";
    public static final String FORM_LAYER_MAPTYPE = "MapType";
    public static final String HTML_LAYER_MAPTYPE_OPTIONS = "HTML_LAYER_MAPTYPE_OPTIONS";	// name of JS variable holding list
    // public static final String[] HTML_LAYER_MAPTYPE_OPTIONS = { "RGB", "grey", "DEM" };
    
    // Style
    public static final String HTML_STYLES = "list of styles";
    public static final String HTML_STYLE  = "style";
    public static final String FORM_STYLE_ID = "styleId";
    public static final String HTML_STYLE_NAME_TEXT = "style name";
    public static final String HTML_STYLE_NAME_EXPLAIN = "Externally visible name of this style, usually one word (mandatory).";
    public static final String FORM_STYLE_NAME = "StyleName";
    public static final String HTML_STYLE_TITLE_TEXT = "style title";
    public static final String HTML_STYLE_TITLE_EXPLAIN = "Title of this style, used for human communication; typically a a brief term.";
    public static final String FORM_STYLE_TITLE = "Title";
    public static final String HTML_STYLE_ABSTRACT_TEXT = "style abstract";
    public static final String HTML_STYLE_ABSTRACT_EXPLAIN = "Descriptive narrative providing more information about the style (mandatory).";
    public static final String FORM_STYLE_ABSTRACT = "Abstract";
    public static final String HTML_STYLE_SHEETURL_TEXT = "sheet URL";
    public static final String HTML_STYLE_SHEETURL_EXPLAIN = "URL leading to symbology information for this style (optional).";
    public static final String FORM_STYLE_SHEETURL = "SheetUrl";
    public static final String HTML_STYLE_LWIDTH_TEXT = "logo width";
    public static final String HTML_STYLE_LWIDTH_EXPLAIN = "URL width in pixels (optional).";
    public static final String FORM_STYLE_LWIDTH = "LogoWidth";
    public static final String HTML_STYLE_LHEIGHT_TEXT = "logo height";
    public static final String HTML_STYLE_LHEIGHT_EXPLAIN = "URL height in pixels (optional)";
    public static final String FORM_STYLE_LHEIGHT = "LogoHeight";
    public static final String HTML_STYLE_LURL_TEXT = "logo URL";
    public static final String HTML_STYLE_LURL_EXPLAIN = "URL of this style&quot;s provider logo (optional).";
    public static final String FORM_STYLE_LURL = "LogoUrl";
    public static final String HTML_STYLE_RASQLOP_TEXT = "rasql operation fragment";
    public static final String HTML_STYLE_RASQLOP_EXPLAIN = "Query fragment specifying how to obtain an RGB image out of the map. Use only induced operations, any bounding box selection and scaling will be added automatically; it is highly recommended to thoroughly test a query fragment manually before going operational (mandatory).";
    public static final String FORM_STYLE_RASQLOP = "RasqlOp";
    
    // level
    public static final String HTML_LEVELS = "list of pyramid levels";
    public static final String FORM_LEVEL_ID = "levelId";
    public static final String HTML_LEVEL_COLLNAME_TEXT = "rasdaman collection name";
    public static final String HTML_LEVEL_COLLNAME_EXPLAIN = "Name of the rasdaman collection  holding this pyramid level data (mandatory).";
    public static final String FORM_LEVEL_COLLNAME = "CollectionName";
    public static final String HTML_LEVEL_SCALEFACTOR_TEXT = "collection relative scale factor";
    public static final String HTML_LEVEL_SCALEFACTOR_EXPLAIN = "Scale factor relative to the original map. The original map has a scale factor of 1 by definition, upper pyramid layers have a factor greater than 1; no factor should appear more than once, otherwise WMS behavior is undefined (mandatory).";
    public static final String FORM_LEVEL_SCALEFACTOR = "ScaleFactor";
    
    // ------ file & service paths ---------------------------------------
    /**
     * Field SERVICE_ACTION_URL
     * Here the URL of the applet which handles POST/GET requests is stored
     * Afterwards, HTML forms will have their action parameter set to this
     */
    public static final String SERVICE_ACTION_URL     = "/commander";
    
    // URL path for aux files for apache (until I know how to load via Tomcat)
    public static final String HTTP_PATH = "";
    public static final String IMAGES_PATH = HTTP_PATH + "images/";		// image directory
    
    /**
     * light bulbs for service status indication
     * CAVEAT: keep consistent with lightbulb handling in commander.js!
     **/
    public static final String LIGHTBULB_RED    = IMAGES_PATH + "lightbulb_red.gif";	// red = not sane, not activated
    public static final String LIGHTBULB_YELLOW = IMAGES_PATH + "lightbulb_yellow.gif";	// yellow = sane, not activated
    public static final String LIGHTBULB_GREEN  = IMAGES_PATH + "lightbulb_green.gif";	// green = sane, activated
    
    public static final int IMG_WIDTH  = 12;	// width of lightbulb image
    public static final int IMG_HEIGHT = 12;	// height of lightbulb image
    
    /**
     * string templates for HTML id names
     */
    public static final String ID_INPUT_TEMPLATE = "_input";
    public static final String ID_FEEDBACK_TEMPLATE = "_feedback";
    public static final String ID_HIDDEN_TEMPLATE = "_hidden";
    public static final String ID_PARAM_TEMPLATE = "_param";
    public static final String ID_SERVICE_AVAIL_TEMPLATE = "available_";
    
    /**
     * id for top-level error message field
     */
    public static final String GLOBAL_MSG_ID = "globalMsgText";
    
    /**
     * key names for JSON response
     */
    public static final String JSON_ITEMLIST = "itemlist";
    public static final String JSON_ITEMCOUNT = "itemcount";
    
    /**
     * invalid database key result
     */
    public static final int NO_KEY = -1;
    
    /**
     * rasdaman max collection name length
     */
    public static final int RASDAMAN_MAXNAMELEN = 100;     // FIXME: actual value?
    
    /**
     * error flag
     */
    public static final String MSG_OK        = "Ok";
    public static final String MSG_NOT_OK    = "notOk";
    
    /**
     * settings for auto pyramid creation
     */
    /// min pyramid edge size in pixels
    public static final int MIN_SIZE = 512;
    /// scalefactor between two subsequent levels
    public static final double STEPPING = 2.0;

}
