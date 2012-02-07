/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2012 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package secore.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.ConsoleAppender;
import org.apache.log4j.PatternLayout;
import org.apache.log4j.PropertyConfigurator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Reads a configuration file, secore.conf
 *
 * @author Dimitar Misev
 */
public class Config {
  
  private static Logger log = LoggerFactory.getLogger(Config.class);
  
  // configuration files
  public static final String CONF_FILE = "etc/secore.conf";
  public static final String LOG4J_PROPERTIES = "etc/log4j.properties";
  
  // version
  public final static String VERSION_MAJOR = "0";
  public final static String VERSION_MINOR = "1";
  public final static String VERSION_MICRO = "0";
  public final static String VERSION =
      VERSION_MAJOR + "." + VERSION_MINOR + "." + VERSION_MICRO;
  public final static String LANGUAGE = "en";
  
  // configuration keys
  private final static String USERNAME_KEY = "username";
  private final static String PASSWORD_KEY = "password";
  private final static String GML_DEF_KEY = "gml.def.path";
  private final static String SERVICE_URL = "service.url";
  private final static String CODESPACE_KEY = "codespace";
  
  // singleton
  private static Config instance;
  private static Properties props;

  private Config() {
    // setup logging
    try {
      File file = IOUtil.findFile(LOG4J_PROPERTIES);
      PropertyConfigurator.configure(file.toURI().toURL());
      log.info("Logging configured from configuration file " + LOG4J_PROPERTIES);
    } catch (Exception ex) {
      BasicConfigurator.configure(new ConsoleAppender(
          new PatternLayout("[%d{HH:mm:ss}]%6p %c{1}@%L: %m%n")));
      log.warn("No log4j.properties found on the classpath. Logging to standard output configured in code");
    }
    
    // read configuration
    props = new Properties();
    InputStream is = null;
    try {
      is = new FileInputStream(IOUtil.findFile(CONF_FILE));
      props.load(is);
    } catch (IOException ex) {
      log.error("Failed loading settings", ex);
      throw new RuntimeException("Failed loading settings", ex);
    } finally {
      if (is != null) {
        try {
          is.close();
        } catch (IOException ex) {
          log.warn("Failed closing stream settings input stream" , ex);
        }
      }
    }
  }
  
  public static Config getInstance() {
    if (instance == null) {
      instance = new Config();
    }
    return instance;
  }

  /**
   * Return a setting value from the settings file
   *
   * @param key setting key
   * @return setting value, or the empty string in case the key does not exist
   */
  private String get(String key) {
    if (props.containsKey(key)) {
      return props.getProperty(key);
    }
    log.warn("Unknown setting: " + key);
    return "";
  }

  public String getPassword() {
    return get(USERNAME_KEY);
  }

  public String getUsername() {
    return get(PASSWORD_KEY);
  }

  public String getGmlDefPath() {
    return get(GML_DEF_KEY);
  }
  
  public String getServiceUrl() {
    return get(SERVICE_URL);
  }
  
  public String getCodespace() {
    return get(CODESPACE_KEY);
  }
}
