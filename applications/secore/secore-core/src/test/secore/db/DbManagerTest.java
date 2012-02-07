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

import secore.db.DbManager;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Dimitar Misev
 */
public class DbManagerTest {
  
  /**
   * Test of getInstance method, of class DbManager.
   */
  @Test
  public void testGetInstance() {
    assertFalse(DbManager.getInstance().getDb().closed());
  }

  @Test
  public void testQuery() throws Exception {
    String query =
            "declare namespace gml = \"http://www.opengis.net/gml\";\n"
          + "let $d := doc('gml')\n"
          + "return $d//gml:identifier[text() = 'urn:ogc:def:crs:EPSG::4326']";
    String result = DbManager.getInstance().getDb().query(query);
    assertTrue(result.matches("<identifier [^>]+>urn:ogc:def:crs:EPSG::4326</identifier>"));
  }

  @Test
  public void testDocumentNames() throws Exception {
    String query =
           "for $doc in collection('gml') return base-uri($doc)";
    String result = DbManager.getInstance().getDb().query(query);
    System.out.println(result);
  }
  
  @Test
  public void testClose() {
    DbManager.getInstance().getDb().close();
    assertTrue(DbManager.getInstance().getDb().closed());
  }
}
