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


package wcst.server;

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
	public static boolean PRINT_LOG;
	public static String SERVLET_INFO;
	public static String VERSION;
    public static String RASDAMAN_URL;
    public static String RASDAMAN_DATABASE;
    
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
		PRINT_LOG = Boolean.parseBoolean(get("wcst_print_log"));
		SERVLET_INFO = get("wcst_servlet_info");
        RASDAMAN_DATABASE = get("rasdaman_database");
        RASDAMAN_URL = get("rasdaman_url");

        log("---------------------------");
        log("WCS-T Language: " + LANGUAGE);
        log("WCS-T Version: " + VERSION);
        log("WCS-T Print Log: " + PRINT_LOG);
        log("WCS-T Servlet Info: " + SERVLET_INFO);
        log("Rasdaman URL: " + RASDAMAN_URL);
        log("Rasdaman DB: " + RASDAMAN_DATABASE);
        log("---------------------------");
	}

    private void log(String msg)
    {
//        if (PRINT_LOG)
            System.out.println(msg);
    }
}
