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
 * This class contains can read the configuration saved with the commander
 * in the database and create the options needed to instantiate the service.
 *
 * COMMENTS:
 * The class only holds the translate method, therefore it cannot be instantiated.
 *
 * </pre>
 *************************************************************/

package petascope.wms;

import java.util.Enumeration;
import java.util.Hashtable;
import javax.servlet.http.HttpServlet;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.ConfigManager;
import petascope.wms.commander.CommanderConnection;
import petascope.wms.commander.Table_Layer;
import petascope.wms.commander.Table_PyramidLevels;
import petascope.wms.commander.Table_Service;
import petascope.wms.commander.Table_Style;
import petascope.exceptions.WMSException;

public class DatabaseConfigAdapter {
    
    private static final Logger log = LoggerFactory.getLogger(DatabaseConfigAdapter.class);
    private DatabaseConfigAdapter() {
    }
    
    public static void translate(HttpServlet servlet, Hashtable options) throws WMSException {
        log.info("DatabaseConfigAdapter.translate()");
        
        String jdbcUrl, jdbcUser, jdbcPassword, jdbcDriver;
        jdbcUrl = ConfigManager.METADATA_URL;
        jdbcUser = ConfigManager.METADATA_USER;
        jdbcPassword = ConfigManager.METADATA_PASS;
        jdbcDriver = ConfigManager.METADATA_DRIVER;
        DatabaseConfigAdapter.translate(jdbcUrl, jdbcUser, jdbcPassword, jdbcDriver, options);
        
        log.info("DatabaseConfigAdapter.translate()");
    }
    
    public static void translate(String jdbcUrl, String jdbcUser, String jdbcPassword, String jdbcDriver, Hashtable options) throws WMSException {
        log.info("DatabaseConfigAdapter.translate()");
        
        CommanderConnection dbc;
        log.debug( "Current Settings:\nURL: " + jdbcUrl + "\nUser: " +
                jdbcUser + "\nPassword: " + jdbcPassword + "\nDriver: "+
                jdbcDriver);
        try {
            dbc = new CommanderConnection(jdbcUrl, jdbcUser, jdbcPassword, jdbcDriver);
        }
        catch (Exception e) {
            log.error("Error: Failed to initialize database connection for reading configuration: " + e.getMessage());
            throw new WMSException(301);
        }
        
        log.warn("Finding service...");	// NB: Only 1 service supported currently!
        Table_Service serviceInfo = new Table_Service();
        try {
            serviceInfo.fetchAllTuples(dbc);
            serviceInfo.next();
            while (serviceInfo.isTupleValid() && serviceInfo.getAvailability() != petascope.wms.commander.Globals.YES_AVAILABLE)
                serviceInfo.next();
            if (!serviceInfo.isTupleValid())
            	throw new Exception("Error: Service not found.");
        }
        catch (Exception e) {
            log.error("Error: Failed to find service definition in database:" + e.getMessage());
            throw new WMSException(302);
        }
        
        if (serviceInfo.getAvailability() != petascope.wms.commander.Globals.YES_AVAILABLE) {
            log.error("Error: Service will not be available.");
            throw new WMSException(303);
        }
        
        log.warn("Service definition found: title '" + serviceInfo.getTitle() + "', id=" + serviceInfo.getId() );
        options.put(WmsConfig.PARAMETER_WMS_UPDATE_SEQUENCE, serviceInfo.getUpdateSequence());
        options.put(WmsConfig.PARAMETER_WMS_TITLE, serviceInfo.getTitle());
        options.put(WmsConfig.PARAMETER_WMS_ABSTRACT, serviceInfo.getAbstract());
        options.put(WmsConfig.PARAMETER_WMS_KEYWORDS, serviceInfo.getKeywords());
        options.put(WmsConfig.PARAMETER_WMS_FEES, serviceInfo.getFees());
        options.put(WmsConfig.PARAMETER_WMS_ACCESS_CONSTRAINTS, serviceInfo.getAccessConstraints());
        options.put(WmsConfig.PARAMETER_WMS_HOST_NAME, serviceInfo.getHostName());
        options.put(WmsConfig.PARAMETER_WMS_PORT, "" + serviceInfo.getPort());
        options.put(WmsConfig.PARAMETER_WMS_PATH, serviceInfo.getPath());
        options.put(WmsConfig.PARAMETER_WMS_FORMATS, serviceInfo.getFormats());
        options.put(WmsConfig.PARAMETER_WMS_BASE_LAYER_NAME, serviceInfo.getBaseLayerName());
        options.put(WmsConfig.PARAMETER_WMS_VENDOR_CAPABILITIES, serviceInfo.getVendorCapabilities());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_PERSON_NAME, serviceInfo.getContactPerson());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_ORGANIZATION_NAME, serviceInfo.getContactOrganization());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_ADDRESS_TYPE, serviceInfo.getAddressType());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_ADDRESS, serviceInfo.getAddress());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_CITY, serviceInfo.getCity());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_STATE_OR_PROVINCE, serviceInfo.getStateOrProvince());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_POST_CODE, serviceInfo.getPostCode());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_COUNTRY, serviceInfo.getCountry());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_VOICE_TELEPHONE, serviceInfo.getContactVoiceTelephone());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_FACSIMILE_TELEPHONE, serviceInfo.getContactFacsimileTelephone());
        options.put(WmsConfig.PARAMETER_WMS_CONTACT_ELECTRONIC_MAIL_ADDRESS, serviceInfo.getContactElectronicMailAddress());
        
        log.warn("Loading layers...");
        
        loadLayers(options, dbc, serviceInfo.getId());
        
        while (serviceInfo.isTupleValid() ) {
            if (serviceInfo.getAvailability() == petascope.wms.commander.Globals.YES_AVAILABLE)
                log.warn("Warning: More than one service available; current implementation can only handle one service. "+
                        "Service '" + serviceInfo.getTitle() + "' will not be operational.");
            try {
                serviceInfo.next();
            }
            catch (Exception e) {
                log.warn("Error fetching data from database: " + e.getMessage());
            }
        }
        
        log.info("DatabaseConfigAdapter.translate()");
    }
    
    private static void loadLayers(Hashtable options, CommanderConnection dbc, int serviceId) throws WMSException {
        log.info("DatabaseConfigAdapter.loadLayers()");
        
        Table_Layer layerInfo = new Table_Layer();
        String layers = "";
        try {
            layerInfo.fetchTuplesByServiceId(dbc, serviceId);
            layerInfo.next();
        }
        catch (Exception e) {
            log.error("Error: Failed to fetch layer information: " + e.getMessage() + "." );
            throw new WMSException(305);
        }
        while (layerInfo.isTupleValid()) {
            String layerName = layerInfo.getName();
            String bbox;
            layers += (layers.equals("") ? "" : ";") + layerName;
            
            log.debug("found layer '" + layerName + "', id=" + layerInfo.getLayerId() );
            
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_TITLE, layerInfo.getTitle());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_SRS, layerInfo.getSrs());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_AUTHORITY, layerInfo.getAuthority());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_LATLON_BOX, layerInfo.getLatLonBoxXmin() + ";" + layerInfo.getLatLonBoxYmin() + ";" +
                    layerInfo.getLatLonBoxXmax() + ";" + layerInfo.getLatLonBoxYmax() );
            bbox = layerInfo.getBBoxXmin() +";"+ layerInfo.getBBoxYmin() +";"+
                    layerInfo.getBBoxXmax() + ";" + layerInfo.getBBoxYmax();
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_BBOX, bbox);
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_ATTRIBUTION_URL, layerInfo.getAttributionURL());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_ATTRIBUTION_TITLE, layerInfo.getAttributionTitle());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_LOGO_WIDTH, "" + layerInfo.getLogoWidth());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_LOGO_HEIGHT, "" + layerInfo.getLogoHeight());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_LOGO_FORMAT, layerInfo.getLogoFormat());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_LOGO_URL, layerInfo.getLogoURL());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_FEATURE_URL, layerInfo.getFeatureURL());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_MAPTYPE, layerInfo.getMapType().toString() );
            
            
            double resolution = layerInfo.getResolution();
            loadStyles(layerInfo.getLayerId(), layerName, options, dbc);
            loadPyramidLevels(layerInfo.getLayerId(), layerName, options, dbc,
                    bbox, resolution);
            try {
                layerInfo.next();
            }
            catch (Exception e) {
                log.error("Error: Failed to fetch layer information: " + e.getMessage() + "." );
                throw new WMSException(305);
            }
        }
        log.debug("Layers found: " + layers);
        options.put("layers", layers);
        
        log.info("DatabaseConfigAdapter.loadLayers()");
    }
    
    private static void loadPyramidLevels(int layerId, String layerName, Hashtable options, CommanderConnection dbc, String bbox, double resolution) throws WMSException {
        log.info("DatabaseConfigAdapter.loadPyramidLevels(layerName=" + layerName + ")");
        
        Table_PyramidLevels tpl = new Table_PyramidLevels();
        try {
            tpl.fetchTuplesByLayerId(dbc, layerId);
            tpl.next();
        }
        catch (Exception e) {
            log.error("Failed to fetch pyramid levels from database: " + e.getMessage());
            throw new WMSException(308, layerName);
        }
        String pyramid = "";
        while (tpl.isTupleValid()) {
            String collName = tpl.getCollectionName();
            double factor = tpl.getScaleFactor();
            pyramid += (pyramid.equals("") ? "" : ";") + collName;
            options.put(collName, factor*resolution + ";" + factor*resolution + ";" + bbox);
            try {
                tpl.next();
            }
            catch (Exception e) {
                log.error("Error while loading pyramid levels for " + "layer " + layerName + ": " +  e.getMessage());
            }
        }
        options.put(layerName, pyramid);
        log.info("DatabaseConfigAdapter.loadPyramidLevels()");
    }
    
    private static void loadStyles(int layerId, String layerName, Hashtable options, CommanderConnection dbc) throws WMSException {
        log.info( "DatabaseConfigAdapter.loadStyles(layerName=" + layerName + ")");
        
        Table_Style styleInfo = new Table_Style();
        String styles = "";
        try
        {
            styleInfo.fetchTuplesByLayerId(dbc, layerId);
            styleInfo.next();
        }
        catch (Exception e)
        {
            log.error("Error: Failed to fetch style information at first try: " + e.toString());
            throw new WMSException(306);
        }
        if (!styleInfo.isTupleValid())
            throw new WMSException(307, layerName);
        while (styleInfo.isTupleValid()) {
            String styleName = styleInfo.getName();
            styles += (styles.equals("") ? "" : ";") + styleName;
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + styleName + WmsConfig.PARAMETER_WMS_SUFFIX_STYLE_TITLE, styleInfo.getTitle());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + styleName + WmsConfig.PARAMETER_WMS_SUFFIX_STYLE_ABSTRACT, styleInfo.getAbstract());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + styleName + WmsConfig.PARAMETER_WMS_SUFFIX_STYLE_LEGEND_WIDTH, "" + styleInfo.getLegendWidth());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + styleName + WmsConfig.PARAMETER_WMS_SUFFIX_STYLE_LEGEND_HEIGHT, "" + styleInfo.getLegendHeight());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + styleName + WmsConfig.PARAMETER_WMS_SUFFIX_STYLE_LEGEND_URL, styleInfo.getLegendURL());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + styleName + WmsConfig.PARAMETER_WMS_SUFFIX_STYLE_SHEET_URL, styleInfo.getSheetURL());
            options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + styleName + WmsConfig.PARAMETER_WMS_SUFFIX_RASQL_OP, styleInfo.getRasqlOp());
            
            try
            {
                styleInfo.next();
            }
            catch (Exception e)
            {
                log.error("Error: Failed to fetch style information: " + e.toString());
                throw new WMSException(306);
            }
        }
        options.put(WmsConfig.CONFIG_PARAMETER_WMS_PREFIX + layerName + WmsConfig.PARAMETER_WMS_SUFFIX_STYLES, styles);
        log.info("DatabaseConfigAdapter.loadStyles");
    }
    
    private static String getInitParameter(HttpServlet servlet, String option) throws WMSException {
        String result;
        try {
            result = servlet.getInitParameter(option);
        } catch (Exception e) {
            log.error("Error: Option " + option +" not defined.");
            throw new WMSException(300, option);
        }
        return result;
    }
    
    
    // In case other tools need to read the raswms.cfg file they can read the output generated here
    public static void main(String args[]) {
        if (args.length == 1) {
            System.err.println("Usage: java rasogc.raswms.DatabaseConfigAdatper --url url --user user --passwd password --driver driver");
            Runtime.getRuntime().exit (1);
        }
        String jdbcUrl, jdbcUser, jdbcPassword, jdbcDriver;
        jdbcUrl = jdbcUser = jdbcPassword = jdbcDriver = null;
        int i = 0;
        while (i < args.length) {
            if (args[i].equals("--url") && i+1 < args.length) {
                jdbcUrl = args[i+1];
                i+=2;
            } else
                if (args[i].equals("--user") && i+1 < args.length) {
                    jdbcUser = args[i+1];
                    i += 2;
                } else
                    if (args[i].equals("--passwd") && i+1 < args.length) {
                        jdbcPassword = args[i+1];
                        i += 2;
                    } else
                        if (args[i].equals("--driver") && i+1 < args.length) {
                            jdbcDriver = args[i+1];
                            i += 2;
                        } else {
                            System.err.println("Error: Paramerter " + args[i] + " was not recognized!");
                            Runtime.getRuntime().exit(1);
                        }
        }
        if (jdbcUrl == null) {
            System.err.println("Error: Missing mandatory parameter url.");
            Runtime.getRuntime().exit(1);
        }
        if (jdbcUser == null) {
            System.err.println("Error: Missing mandatory parameter user.");
            Runtime.getRuntime().exit(1);
        }
        if (jdbcPassword == null) {
            System.err.println("Error: Missing mandatory parameter password.");
            Runtime.getRuntime().exit(1);
        }
        if (jdbcDriver == null) {
            System.err.println("Error: Missing mandatory parameter driver.");
            Runtime.getRuntime().exit(1);
        }
        
        Hashtable options = new Hashtable();
        try {translate(jdbcUrl, jdbcUser, jdbcPassword, jdbcDriver, options);}
        catch ( WMSException e) {
            System.err.println(e.toString());
            Runtime.getRuntime().exit(1);
        }
        
        Enumeration it = options.keys();
        while(it.hasMoreElements()) {
            String v = (String)it.nextElement();
            System.out.println(v + "=" + options.get(v));
        }
        
        Runtime.getRuntime().exit(0);
    }

}  // DatabaseConfigAdapter			
