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

import secore.util.Config;
import secore.util.IOUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Holds an instance to the database.
 *
 * @author Dimitar Misev
 */
public class DbManager {
  
  private static Logger log = LoggerFactory.getLogger(DbManager.class);
  
  private Database db;
  private static DbManager instance;
  
  private DbManager() {
    db = new BaseX();
    if (!db.open()) {
      throw new RuntimeException("Failed connecting to the database");
    }
    try {
      db.addAll(IOUtil.findFile(Config.getInstance().getGmlDefPath()).toString());
    } catch (Exception ex) {
      throw new RuntimeException("Failed loading GML definitions to the database", ex);
    }
  }
  
  public static DbManager getInstance() {
    if (instance == null) {
      instance = new DbManager();
    }
    return instance;
  }
  
  public Database getDb() {
    return db;
  }
  
  /**
   * @param db the new database instance
   * @throws IllegalArgumentException if db is null
   */
  public void setDb(Database db) {
    if (db == null) {
      throw new IllegalArgumentException();
    }
    this.db.close();
    this.db = db;
  }
}
