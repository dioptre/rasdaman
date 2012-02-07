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
 * Copyright 2003 - 2012 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package secore.db;

import secore.util.IOUtil;
import secore.util.SecoreException;
import secore.util.ExceptionCode;
import java.io.File;
import java.io.FilenameFilter;
import org.basex.core.BaseXException;
import org.basex.core.Context;
import org.basex.core.cmd.Add;
import org.basex.core.cmd.Close;
import org.basex.core.cmd.CreateDB;
import org.basex.core.cmd.CreateIndex;
import org.basex.core.cmd.Open;
import org.basex.core.cmd.Set;
import org.basex.core.cmd.XQuery;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * BaseX backend.
 *
 * @author Dimitar Misev
 */
public class BaseX implements Database {
  
  private static Logger log = LoggerFactory.getLogger(BaseX.class);
  
  protected static final String GML_DB = "gml";
  private static final String XML_EXTENSION = ".xml";
  
  private static final String TEXT_INDEX_TYPE = "text";

  private Context context;
  private boolean closed;

  public boolean open() {
    context = new Context();
    try {
      new Set("CREATEFILTER", "*" + XML_EXTENSION).execute(context);
      new Open(GML_DB).execute(context);
      new CreateIndex(TEXT_INDEX_TYPE).execute(context);
    } catch (Exception ex) {
      log.debug("The database doesn't seem to exist");
      try {
        log.info("Initializing database " + GML_DB);
        new CreateDB(GML_DB).execute(context);
        log.info("Database successfully initialized.");
      } catch (Exception e) {
        log.error("Failed initializing the " + GML_DB + " database", e);
        return false;
      }
    }
    log.info("Database connection successfully opened.");
    closed = false;
    return true;
  }

  public boolean closed() {
    return closed;
  }

  public boolean close() {
    try {
      new Close().execute(context);
      closed = true;
      log.debug("Database connection successfully closed.");
      return true;
    } catch (Exception ex) {
      log.error("Failed closing the database connection.", ex);
    }
    return false;
  }

  public String query(String query) throws SecoreException {
    log.trace("Executing query: " + query);
    try {
      String ret = new XQuery(query).execute(context);
      log.trace("Query successfully executed.");
      return ret;
    } catch (Exception e) {
      throw new SecoreException(ExceptionCode.InternalComponentError, "Failed at querying the database", e);
    }

  }

  public boolean add(String path) {
    try {
      String name = IOUtil.getFilename(path);
      // The XQuery base-uri() function returns a file path
      String docs = new XQuery("for $doc in collection('" + GML_DB + "')"
          + "return base-uri($doc)").execute(context);
      if (docs.contains(name)) {
        return false;
      }
      new Add(path, name).execute(context);
    } catch (BaseXException ex) {
      log.error("Failed adding document " + path + " to the " + GML_DB + " database.", ex);
      return false;
    }
    log.debug("Loaded GML file to database: " + path);
    return true;
  }

  public void addAll(String dir) {
    try {
      File fdir = new File(dir);
      File[] files = fdir.listFiles(new FilenameFilter() {
        public boolean accept(File file, String string) {
          return string.endsWith(XML_EXTENSION);
        }
      });
      for (File file : files) {
        add(file.toURI().toString());
      }
    } catch (Exception ex) {
      log.error("Failed loading GML definitions from " + dir, ex);
      throw new RuntimeException("Failed loading GML definitions from " + dir, ex);
    }
  }
}
