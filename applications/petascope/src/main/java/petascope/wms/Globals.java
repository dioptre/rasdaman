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
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package petascope.wms;

/**
 * This interface contains a list of global exception messages.
 * Provide the list of all raswms error messages, together
 * with int tags identifying them.
 * By convention, error texts ending with a colon ":" are
 * expected to get appended detailing information.
 */
public interface Globals
{
    
    // --- constants ------------------------------------------------------
    /**
     * web.xml parameter names
     **/
    /// - parameter name for message property file
    public static final String MESSAGE_FILE = "messageFile";
    public static final String DEBUG_LEVEL  = "debug";
    public static final String CONFIGFILE   = "configFile";
    public static final String OPTION_JDBC_DATABASE_URL = "databaseUrl";
    public static final String OPTION_JDBC_USER = "user";
    public static final String OPTION_JDBC_PASSWORD = "password";
    public static final String OPTION_JDBC_DRIVER = "driver";

    /**
     * ReloadCapabilities request HTML response
     **/
    public static final String RELOAD_CAPABILITIES_RESPONSE = "<html><body><h1>Capabilities reload done.</h1></body></html>";
    
    /**
     * wait interval for seek of free connections to rasdaman server
     **/
    public static final int FREECONNECTION_TIMEOUT = 100000; // msec (was 1000) ?
    
    /**
     * default number of rasserver connections claimed
     **/
    public static final String DEFAULT_SERVER_CONNECTIONS = "1";
    
    /**
     * default thresholds for server connection balancing
     **/
    // This setting should be a little under the time-out setting on the rasserver.
    // Otherwise idle connections might get severed.
    public static final String SERVER_CONNECTION_TIMEOUT = "30";		// secs
    // This is the timeout for one query. If the server is under load or the query if very complex the response time will be long
    // and clients might get time-out's from their browsers saying that the server is not responding.
    // This way at least we can inform the user of the situation.
    public static final String REQUEST_TIMEOUT           = "30000";		// msecs (was 5000)
    // This is the sleep period of the monitor thread.
    // The monitor will adjust the number of connections and sleep for this period.
    // Too long and the server will not be able to cope with sudden spikes.
    // Too short and the server will behave eratically, opening and closing connections with no apparent reason.
    public static final String MONITOR_INTERVAL          = "20000";		// msecs (was 2000)
    // This value is the number of queries we are confident we can solve on one thread within one REQUEST_TIMEOUT.
    // When the number of queries per thread exceeds this value the monitor will start a new thread (if possible).
    public static final String HIGH_THRESHOLD            = "4";		// number of queries per thread
    // This value represents the number of queries per thread when we are confident that we can close one thread
    // and that the rest of the threads can handle the additional load.
    // When the number of queries per thread is below this value the monitor will kill one of the threads (if possible).
    public static final String LOW_THRESHOLD             = "1";		// -"-
    
    
    /**
     * list of all error codes
     * ATTENTION:
     * - keep consistent with error text list!
     * - be careful about numbering: identical to number in corresponding text string!
     **/
    
    // this is a placeholder for followup exceptions, saying "use last exception"
    // when evaluated, the last remembered message is reported.
    public static final int ERR_Followup                        =   -1;
    
    // WMS standard errors:
    public static final int ERR_OperationNotSupported           =   1;
    public static final int ERR_InvalidRequest                  =   2;
    public static final int ERR_SRSInvalid                      =   3;
    public static final int ERR_LayerUndefined                  =   4;
    public static final int ERR_LayerNotQueryable               =   5;
    public static final int ERR_ImageFormatNotSupported         =   6;
    public static final int ERR_ExceptionNotSupported           =   7;
    public static final int ERR_StyleNotDefined                 =   8;
    public static final int ERR_OutOfBounds                     =   9;
    public static final int ERR_ParsingError                    =  10;
    public static final int ERR_CurrentUpdateSequence           =  11;
    public static final int ERR_InvalidUpdateSequence           =  12;
    public static final int ERR_ConfigurationError              =  13;
    public static final int ERR_AuthorisationError              =  14;
    // rasdaman specific errors:
    public static final int ERR_QueryEval                       = 100;
    public static final int ERR_NoConnection                    = 101;
    // raswms specific errors:
    public static final int ERR_InternalException               = 200;
    @Deprecated public static final int ERR_UnsupportedExceptionXml         = 201;	// obsolete
    @Deprecated public static final int ERR_UnsupportedExceptionBlank       = 202;	// obsolete
    @Deprecated public static final int ERR_UnsupportedExceptionInimage     = 203;	// obsolete
    public static final int ERR_FormatLost                      = 204;
    public static final int ERR_SrsNotSupportedByLayer          = 205;
    public static final int ERR_WidthNotPositive                = 206;
    public static final int ERR_HeightNotPositive               = 207;
    public static final int ERR_NoLayerDef                      = 208;
    public static final int ERR_BackgroundNotSupported          = 209;
    public static final int ERR_TransparentNotSupported         = 210;
    public static final int ERR_NoPyramidLayer                  = 211;
    public static final int ERR_NoSuchStyle                     = 212;
    public static final int ERR_LayerNotDefined                 = 213;
    public static final int ERR_LayerOutOfBBox                  = 214;
    public static final int ERR_ServletNotInit                  = 215;
    public static final int ERR_IllegalRequest                  = 216;
    public static final int ERR_InvalidRequestObject            = 217;
    public static final int ERR_FeatureNotSupported             = 218;
    public static final int ERR_OutputCreate                    = 219;
    public static final int ERR_RequestMissing                  = 220;
    public static final int ERR_UnknownRequest                  = 221;
    public static final int ERR_VersionMissing                  = 222;
    public static final int ERR_VersionMismatch                 = 223;
    public static final int ERR_WidthMissing                    = 224;
    public static final int ERR_HeightMissing                   = 225;
    public static final int ERR_IllegalException                = 226;
    public static final int ERR_LayersMissing                   = 227;
    public static final int ERR_StylesMissing                   = 228;
    public static final int ERR_SrsMissing                      = 229;
    public static final int ERR_BboxMissing                     = 230;
    public static final int ERR_BboxIllegal                     = 231;
    public static final int ERR_WidthIllegal                    = 232;
    public static final int ERR_HeightIllegal                   = 233;
    public static final int ERR_LayersStylesMismatch            = 234;
    public static final int ERR_ServiceIllegal                  = 235;
    public static final int ERR_UpdatesequenceIllegal           = 236;
    public static final int ERR_FormatIllegal                   = 237;
    public static final int ERR_FormatMissing                   = 238;
    public static final int ERR_TimeIllegal                     = 239;
    public static final int ERR_ElevationIllegal                = 240;
    public static final int ERR_AuthParamIllegal                = 241;
    public static final int ERR_AuthFailure                     = 242;
    public static final int ERR_UpdatesequenceSame              = 243;
    public static final int ERR_UpdatesequenceHigher            = 244;
    public static final int ERR_RequestEmpty                    = 245;
    public static final int ERR_LayersEmpty                     = 246;
    public static final int ERR_StylesEmpty                     = 247;
    public static final int ERR_SrsEmpty                        = 248;
    public static final int ERR_BboxEmpty                       = 249;
    public static final int ERR_FormatEmpty                     = 250;
    public static final int ERR_TransparencyIllegal             = 251;
    public static final int ERR_BgcolorIllegal                  = 252;
    public static final int ERR_Bbox_X                          = 253;
    public static final int ERR_Bbox_Y                          = 254;
    public static final int ERR_Bbox_Resolution                 = 255;
    
    // --- variables ------------------------------------------------------

} // Globals

