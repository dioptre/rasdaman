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

import java.io.FileInputStream;
import java.io.IOException;

import java.util.Properties;

/**
 * Configuration Manager class: a single entry point for all server settings.
 * Implements the singleton design pattern.
 *
 * @author Andrei Aiordachioaie
 */
public class ConfigManager
{
	/* Settings variables */
	public static String LANGUAGE;
	public static boolean PRINT_LOG;    // Print to log file
    public static boolean PRINT_OUT = true;    // Print to standard output
	public static String SERVLET_INFO;
	public static String VERSION;
    public static String RASDAMAN_URL;
    public static String RASDAMAN_DATABASE;
    public static String METADATA_DRIVER;
    public static String METADATA_URL;
    public static String METADATA_USER;
    public static String METADATA_PASS;

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
	 */
	private ConfigManager(String settingsPath)
	{
		props = new Properties();
		try
		{
            log("Loading settings from file: " + settingsPath);
			props.load(new FileInputStream(settingsPath));
			initSettings();
		}
		catch (IOException e)
		{
			e.printStackTrace();
            log("Failed to load settings.");
		}
	}

	/**
	 * Returns the instance of the ConfigManager. If no such instance exists,
	 * it creates one with the specified settings file.
	 *
	 * @param settingsPath Path to the settings file
	 * @return instance of the ConfigManager class
	 */
	public static ConfigManager getInstance(String settingsPath)
	{
		if ( instance == null )
			instance = new ConfigManager(settingsPath);

		return instance;
	}

    public static ConfigManager getInstance()
    {
        if (instance == null)
            throw new RuntimeException("Could not initialize the ConfigManager " +
                    "because no settings file path was provided.");
        return instance;
    }

	/**
	 * Return a setting value from the settings file
	 *
	 * @param key Key of the setting
	 * @return String value, or the empty string in case the key does not exist
	 */
	private String get(String key)
	{
		String result = "";

		if ( props.containsKey(key) )
			result = props.getProperty(key);

		return result;
	}

	private void initSettings()
	{
		LANGUAGE = get("wcst_language");
		VERSION = get("wcst_version");
		PRINT_LOG = Boolean.parseBoolean(get("print_log"));
        PRINT_OUT = Boolean.parseBoolean(get("print_output"));
		SERVLET_INFO = get("servlet_info");
        RASDAMAN_DATABASE = get("rasdaman_database");
        RASDAMAN_URL = get("rasdaman_url");
        METADATA_DRIVER = get("metadata_driver");
        METADATA_URL = get("metadata_url");
        METADATA_USER = get("metadata_user");
        METADATA_PASS = get("metadata_pass");

        /* User preferences override default values for WCS-T */
        String tmp = get("default_interpolation");
        if (tmp.length() > 0)
            WCST_DEFAULT_INTERPOLATION = tmp;
        tmp = get("default_null_resistance");
        if (tmp.length() > 0)
            WCST_DEFAULT_NULL_RESISTANCE = tmp;
        tmp = get("default_datatype");
        if (tmp.length() > 0)
            WCST_DEFAULT_DATATYPE = tmp;

        log("---------------------------");
        log("WCS-T Language: " + LANGUAGE);
        log("WCS-T Version: " + VERSION);
        log("Print Log: " + PRINT_LOG);
        log("PetaScope Servlet Info: " + SERVLET_INFO);
        log("Rasdaman URL: " + RASDAMAN_URL);
        log("Rasdaman DB: " + RASDAMAN_DATABASE);
        log("Metadata Driver: " + METADATA_DRIVER);
        log("Metadata URL: " + METADATA_URL);
        log("Metadata Username: " + METADATA_USER);
//        log("Metadata Password: " + METADATA_PASS);
        log("WCS-T Default Interpolation: " + WCST_DEFAULT_INTERPOLATION);
        log("WCS-T Default Null Resistance: " + WCST_DEFAULT_NULL_RESISTANCE);
        log("WCS-T Default Datatype: " + WCST_DEFAULT_DATATYPE);
        log("---------------------------");
	}

    private void log(String msg)
    {
        if (PRINT_LOG)
            System.out.println(msg);
    }
}
