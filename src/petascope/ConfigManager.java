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

//~--- JDK imports ------------------------------------------------------------
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
import petascope.wcs2.server.templates.WcsNamespaceContext;

/**
 * Configuration Manager class: a single entry point for all server settings.
 * Implements the singleton design pattern.
 *
 * @author Andrei Aiordachioaie
 */
public class ConfigManager {

    private static Logger LOG = LoggerFactory.getLogger(ConfigManager.class);

    /* Major version number. This is the first release (1). */
    private final static String MAJOR = "1";
    /* Minor version number. v2 adds the reference implementation of WCS 2.0.
    v3 adds WGS84 handling in WCPS requests. */
    private final static String MINOR = "3";
    /* Bug-fix count. We have a hack: every WCPS response is written to disk. */
    private final static String BUGFIX = "5-hack";

    /* Petascope 1.2.0 contains WCS 1.1.0, WCS 2.0, WCS-T 1.0.0 and WCPS 1.0.0 */
    public final static String PETASCOPE_VERSION = MAJOR + "." + MINOR + "." + BUGFIX;
    /* Settings variables */
    public static String WCST_LANGUAGE;
    public static String WCST_VERSION;
    public static String RASDAMAN_URL;
    public static String RASDAMAN_DATABASE;
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
            LOG.info("Loading settings from file: " + settingsPath);
            props.load(new FileInputStream(settingsPath));
            initSettings(servletRoot);
        } catch (IOException e) {
            LOG.error("Failed to load settings. Stack trace: " + e);
            throw new ServletException("Failed to load settings file.");
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
            throw new RuntimeException("Could not initialize the ConfigManager "
                    + "because no settings file path was provided.");
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
            URI desc = WcsNamespaceContext.class.getResource("DescribeCoverageTemplate.xml").toURI();
            URI getcov = WcsNamespaceContext.class.getResource("GetCoverageTemplate.xml").toURI();
            URI getcap = WcsNamespaceContext.class.getResource("GetCapabilitiesTemplate.xml").toURI();
            WCS2_GET_CAPABILITIES_TEMPLATE = loadFile(getcap);
            WCS2_DESCRIBE_COVERAGE_TEMPLATE = loadFile(desc);
            WCS2_GET_COVERAGE_TEMPLATE = loadFile(getcov);
            WCS2_SCHEMA_URL = get("wcs2_schema_url");
        } catch (Exception e) {
            LOG.warn("Could not read XML template files for WCS 2.0. Therefore, WCS 2.0 will be unable to start.");
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

        LOG.info("---------------------------");
        if (CCIP_HACK) {
            LOG.info("-----------CCIP------------");
        }
        LOG.info("---------------------------");

//        log("Print Log: " + PRINT_LOG);
        LOG.info("       *** PETASCOPE ***      ");
        LOG.info("Rasdaman URL: " + RASDAMAN_URL);
        LOG.info("Rasdaman DB: " + RASDAMAN_DATABASE);
        LOG.info("Metadata Driver: " + METADATA_DRIVER);
        LOG.info("Metadata URL: " + METADATA_URL);
        LOG.info("Metadata Username: " + METADATA_USER);
//        LOG.info("Metadata Password: " + METADATA_PASS);
        LOG.info("       *** WCS-T ***       ");
        LOG.info("WCS-T Language: " + WCST_LANGUAGE);
        LOG.info("WCS-T Version: " + WCST_VERSION);
        LOG.info("WCS-T Default Interpolation: " + WCST_DEFAULT_INTERPOLATION);
        LOG.info("WCS-T Default Null Resistance: " + WCST_DEFAULT_NULL_RESISTANCE);
        LOG.info("WCS-T Default Datatype: " + WCST_DEFAULT_DATATYPE);
        LOG.info("       *** WCS 2.0 ***     ");
        LOG.trace("Get Capabilities Template: " + WCS2_GET_CAPABILITIES_TEMPLATE.substring(0, 100));
        LOG.trace("Describe Coverage Template: " + WCS2_DESCRIBE_COVERAGE_TEMPLATE.substring(0, 100));
        LOG.trace("Get Capabilities Template: " + WCS2_GET_COVERAGE_TEMPLATE.substring(0, 100));
        LOG.info("---------------------------");
    }

    private String loadFile(URI fileUri) throws IOException {
        InputStream is = null;
        String contents = null;
        try {
            LOG.debug("Loading file: " + fileUri);
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
