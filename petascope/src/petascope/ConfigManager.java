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

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.util.Properties;
import javax.servlet.ServletException;
import org.apache.commons.io.IOUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.wcs2.templates.WcsNamespaceContext;
import static petascope.util.MsgUtil.*;

/**
 * Configuration Manager class: a single entry point for all server settings.
 * Implements the singleton design pattern.
 *
 * @author Andrei Aiordachioaie
 */
public class ConfigManager {

    private static Logger log = LoggerFactory.getLogger(ConfigManager.class);

    /* Major version number. This is the first release (1). */
    private final static String MAJOR = "1";
    /* Minor version number. v2 adds the reference implementation of WCS 2.0.
    v3 adds WGS84 handling in WCPS requests, and v4 adds a WPS implementation. */
    private final static String MINOR = "4";
    /* Bug-fix count. We have a hack: every WCPS response is written to disk. */
    private final static String BUGFIX = "11-hack";

    /* Petascope 1.2.0 contains WCS 1.1.0, WCS 2.0, WCS-T 1.0.0 and WCPS 1.0.0 */
    public final static String PETASCOPE_VERSION = MAJOR + "." + MINOR + "." + BUGFIX;
    public final static String PETASCOPE_LANGUAGE = "en";
    /* Settings variables */
    public static String WCST_LANGUAGE;
    public static String WCST_VERSION;
    public static String WCPS_LANGUAGE = "en";
    public static String WCPS_VERSION = "1.0.0";
    public static String WPS_LANGUAGE = "en";
    public static String WPS_VERSION = "1.0.0";
    public static String WCS_LANGUAGE = "en";
    public static String WCS_VERSION = "1.1.0";
    public static String WCS2_LANGUAGE = "en";
    public static String WCS2_VERSION = "2.0.0";
    public static String RASDAMAN_URL;
    public static String RASDAMAN_DATABASE;
    public static String RASDAMAN_LANGUAGE = "en";
    public static String RASDAMAN_VERSION = "8";
    public static String METADATA_DRIVER;
    public static String METADATA_URL;
    public static String METADATA_USER;
    public static String METADATA_PASS;
    public static boolean CCIP_HACK = false;

    /* WCS 2.0 variables */
    public static String WCS2_GET_CAPABILITIES_TEMPLATE;
    public static String WCS2_DESCRIBE_COVERAGE_TEMPLATE;
    public static String WCS2_GET_COVERAGE_TEMPLATE;
    public static String WCS2_SCHEMA_URL;

    /* This URL gets initialized automatically when the first request is received.
     * Its value is used in the Capabilities response */
    public static String PETASCOPE_SERVLET_URL;
    public static String WCS2_SERVLET_URL;

    /* WCS-T Settings. Overridden by user-preferences in <code>settings.properties</code> */
    public static String WCST_DEFAULT_INTERPOLATION = "none";
    public static String WCST_DEFAULT_NULL_RESISTANCE = "none";
    public static String WCST_DEFAULT_DATATYPE = "unsigned char";
    /* Singleton instance */
    private static ConfigManager instance;
    private static Properties props;

    /**
     * Private constructor. Use <i>getInstance()</i>.
     *
     * @param settingsPath Path to the settings properties file
     * @param servletRoot Path to the root folder where the servlet is deployed
     */
    private ConfigManager(String settingsPath, String servletRoot) throws ServletException {
        props = new Properties();
        try {
            log.info("Loading settings from file: " + settingsPath);
            props.load(new FileInputStream(settingsPath));
            initSettings(servletRoot);
        } catch (IOException e) {
            log.error("Failed to load settings. Stack trace: " + e);
            throw new ServletException(msg(SETTINGS_FAILED_LOADING));
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
    public static ConfigManager getInstance(String settingsPath, String servletRoot) throws ServletException {
        if (instance == null) {
            instance = new ConfigManager(settingsPath, servletRoot);
        }

        return instance;
    }

    public static ConfigManager getInstance() {
        if (instance == null) {
            throw new RuntimeException(msg(SETTINGS_INITERROR));
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

    private void initSettings(String servletRoot) {
        WCST_LANGUAGE = get("wcst_language");
        WCST_VERSION = get("wcst_version");
        RASDAMAN_DATABASE = get("rasdaman_database");
        RASDAMAN_URL = get("rasdaman_url");
        METADATA_DRIVER = get("metadata_driver");
        METADATA_URL = get("metadata_url");
        METADATA_USER = get("metadata_user");
        METADATA_PASS = get("metadata_pass");

        CCIP_HACK = Boolean.parseBoolean(get("ccip_version"));

        try {
            WCS2_SCHEMA_URL = get("wcs2_schema_url");

        } catch (Exception e) {
            log.warn("Could not read XML template files for WCS 2.0. Therefore, WCS 2.0 will be unable to start.");
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

        log.info("---------------------------");
        if (CCIP_HACK) {
            log.info("-----------CCIP------------");
        }
        log.info("---------------------------");

//        log("Print Log: " + PRINT_log);
        log.info("       *** PETASCOPE ***      ");
        log.info("Rasdaman URL: " + RASDAMAN_URL);
        log.info("Rasdaman DB: " + RASDAMAN_DATABASE);
        log.info("Metadata Driver: " + METADATA_DRIVER);
        log.info("Metadata URL: " + METADATA_URL);
        log.info("Metadata Username: " + METADATA_USER);
//        log.info("Metadata Password: " + METADATA_PASS);
        log.info("       *** WCS-T ***       ");
        log.info("WCS-T Language: " + WCST_LANGUAGE);
        log.info("WCS-T Version: " + WCST_VERSION);
        log.info("WCS-T Default Interpolation: " + WCST_DEFAULT_INTERPOLATION);
        log.info("WCS-T Default Null Resistance: " + WCST_DEFAULT_NULL_RESISTANCE);
        log.info("WCS-T Default Datatype: " + WCST_DEFAULT_DATATYPE);
        log.info("       *** WCS 2.0 ***     ");
        log.info("---------------------------");
    }

    private String loadFile(URI fileUri) throws IOException {
        InputStream is = null;
        String contents = null;
        try {
            log.debug("Loading file: " + fileUri);
            File f = new File(fileUri);
            is = new FileInputStream(f);
            contents = IOUtils.toString(is);
        } finally {
            try {
                is.close();
            } catch (IOException ex) {
            }
        }
        return contents;
    }
}
