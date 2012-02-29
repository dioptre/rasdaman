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
package petascope;

import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.util.Properties;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.XMLUtil;
import petascope.wps.server.WpsServer;

/**
 * Configuration Manager class: a single entry point for all server settings.
 * Implements the singleton design pattern.
 *
 * @author Andrei Aiordachioaie
 * @author Dimitar Misev
 */
public class ConfigManager {

    private static Logger log = LoggerFactory.getLogger(ConfigManager.class);

    /* Major version number. This is the first release (1). */
    private final static String MAJOR = "1";
    /* 
     * Minor version number.
     * v2 adds the reference implementation of WCS 2.0.
     * v3 adds WGS84 handling in WCPS requests
     * v4 adds a WPS implementation.
     * v5 adds integration to n52 WPS framework
     */
    private final static String MINOR = "5";
    /* Bug-fix count. We have a hack: every WCPS response is written to disk. */
    private final static String BUGFIX = "0";

    public final static String PETASCOPE_VERSION = MAJOR + "." + MINOR + "." + BUGFIX;
    public final static String PETASCOPE_LANGUAGE = "en";
    /* This URL gets initialized automatically when the first request is received.
     * Its value is used in the Capabilities response */
    public static String PETASCOPE_SERVLET_URL;
    
    /*
     * settings.properties
     */
    
    // petascope metadata (stored in postgres)
    public static String METADATA_DRIVER = "org.postgresql.Driver";
    public static String METADATA_URL = "jdbc:postgresql://localhost:5432/petascopedb";
    public static String METADATA_USER = "petauser";
    public static String METADATA_PASS = "petapasswd";
    
    // rasdaman connection settings
    public static String RASDAMAN_URL = "http://localhost:7001";
    public static String RASDAMAN_DATABASE = "RASBASE";
    public static String RASDAMAN_USER = "rasguest";
    public static String RASDAMAN_PASS = "rasguest";
    public static String RASDAMAN_VERSION = "8";
    
    public static String WCST_LANGUAGE  = "en";
    public static String WCST_VERSION = "1.1.4";
    public static String WCPS_LANGUAGE = "en";
    public static String WCPS_VERSION = "1.0.0";
    public static String WPS_LANGUAGE = "en";
    public static String WPS_VERSION = "1.0.0";
    public static String WCS_DEFAULT_LANGUAGE = "en";
    public static String WCS_DEFAULT_VERSION = "2.0.0";
    public static String WCS_LANGUAGES = "en";
    public static String WCS_VERSIONS = "1.1.2,2.0.0";
    public static String RASDAMAN_LANGUAGE = "en";
    
    // depends on ccip_version in the petascope settings, ccip_version=true
    // will make this flag true.
    public static boolean CCIP_HACK = false;

    /* WPS variables*/
    public static URI WPS_GET_CAPABILITIES_URI;
    public static URI WPS_DESCRIBE_PROCESS_URI;

    /* WCS-T Settings. Overridden by user-preferences in <code>settings.properties</code> */
    public static String WCST_DEFAULT_INTERPOLATION = "none";
    public static String WCST_DEFAULT_NULL_RESISTANCE = "none";
    public static String WCST_DEFAULT_DATATYPE = "unsigned char";
    
    /* Singleton instance */
    private static ConfigManager instance;
    private static Properties props;
    
    private static final String SETTINGS_FILE = "/settings.properties";
    private static final String LOG_PROPERTIES_FILE = "/log4j.properties";
    
    // path to the default HTML response of the interface servlet

    /**
     * Private constructor. Use <i>getInstance()</i>.
     *
     * @param settingsPath Path to the settings properties file
     * @param servletRoot Path to the root folder where the servlet is deployed
     */
    private ConfigManager() {
//        try {
//            PropertyConfigurator.configure(LOG_PROPERTIES_FILE);
//        } catch (Exception ex) {
//            log.warn("Error loading logger configuration " + LOG_PROPERTIES_FILE);
//        }
        
        XMLUtil.init();
        props = new Properties();
        try {
            InputStream is = ConfigManager.class.getResourceAsStream(SETTINGS_FILE);
            if (is != null) {
                props.load(is);
            }
            initSettings();
        } catch (IOException e) {
            log.error("Failed to load settings. Stack trace: " + e);
            throw new RuntimeException("Failed loading settings");
        }
    }

    /**
     * Returns the instance of the ConfigManager. If no such instance exists,
     * it creates one with the specified settings file.
     *
     * @param settingsPath Path to the settings file
     * @param servletRoot Path to the deployed servlet root
     * @return instance of the ConfigManager class
     */
    public static ConfigManager getInstance() {
        if (instance == null) {
            instance = new ConfigManager();
        }
        return instance;
    }

    /**
     * Return a setting value from the settings file
     *
     * @param key Key of the setting
     * @return String value, or the empty string in case the key does not exist
     */
    private String get(String key) {
        String result = "";
        if (props.containsKey(key)) {
            result = props.getProperty(key);
        }
        return result;
    }

    private void initSettings() {
        RASDAMAN_DATABASE = get("rasdaman_database");
        RASDAMAN_URL = get("rasdaman_url");
        RASDAMAN_USER = get("rasdaman_user");
        RASDAMAN_PASS = get("rasdaman_pass");
        RASDAMAN_VERSION = get("rasdaman_version");
        METADATA_DRIVER = get("metadata_driver");
        METADATA_URL = get("metadata_url");
        METADATA_USER = get("metadata_user");
        METADATA_PASS = get("metadata_pass");

        CCIP_HACK = Boolean.parseBoolean(get("ccip_version"));

        //WPS 1.0.0 describeprocess and getcapabilities documents
        try {
            WPS_GET_CAPABILITIES_URI = WpsServer.class.getResource("../templates/GetCapabilities.xml").toURI();
            WPS_DESCRIBE_PROCESS_URI = WpsServer.class.getResource("../templates/DescribeProcess.xml").toURI();
        } catch (Exception e) {
            log.warn("Could not find WPS GetCapabilities and DescribeProcess Documents");
        }

        /* User preferences override default values for WCS-T */
        String tmp = get("default_interpolation");
        if (tmp.length() > 0) {
            WCST_DEFAULT_INTERPOLATION = tmp;
        }
        tmp = get("default_null_resistance");
        if (tmp.length() > 0) {
            WCST_DEFAULT_NULL_RESISTANCE = tmp;
        }
        tmp = get("default_datatype");
        if (tmp.length() > 0) {
            WCST_DEFAULT_DATATYPE = tmp;
        }

        log.info("------------------------------------");
        log.info("       *** PETASCOPE ***      ");
        log.info("Petascope Version: " + PETASCOPE_VERSION);
        log.info("Metadata Driver  : " + METADATA_DRIVER);
        log.info("Metadata URL     : " + METADATA_URL);
        log.info("Metadata Username: " + METADATA_USER);
        log.info("");
        log.info("       *** RASDAMAN ***       ");
        log.info("Rasdaman URL     : " + RASDAMAN_URL);
        log.info("Rasdaman DB      : " + RASDAMAN_DATABASE);
        log.info("Rasdaman user    : " + RASDAMAN_USER);
        log.info("Rasdaman version : " + RASDAMAN_VERSION);
        log.info("");
        log.info("       *** WCS-T ***       ");
        log.info("WCS-T Language: " + WCST_LANGUAGE);
        log.info("WCS-T Version : " + WCST_VERSION);
        log.info("WCS-T Default Interpolation: " + WCST_DEFAULT_INTERPOLATION);
        log.info("WCS-T Default Null Resistance: " + WCST_DEFAULT_NULL_RESISTANCE);
        log.info("WCS-T Default Datatype: " + WCST_DEFAULT_DATATYPE);
        log.info("");
        log.info("       *** WCPS ***       ");
        log.info("WCPS Language : " + WCPS_LANGUAGE);
        log.info("WCPS Version  : " + WCPS_VERSION);
        log.info("");
        log.info("       *** WCS ***       ");
        log.info("WCS Languages : " + WCS_LANGUAGES);
        log.info("WCS Versions  : " + WCS_VERSIONS);
        log.info("");
        log.info("       *** WPS ***       ");
        log.info("WPS Language  : " + WPS_LANGUAGE);
        log.info("WPS Version   : " + WPS_VERSION);
        log.info("WPS GetCapabilities template: " + WPS_GET_CAPABILITIES_URI);
        log.info("WPS DescribeProcess template: " + WPS_DESCRIBE_PROCESS_URI);
        log.info("------------------------------------");
    }
}
