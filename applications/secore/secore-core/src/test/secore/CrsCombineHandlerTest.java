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
package secore;

import secore.ResolveRequest;
import secore.GmlResponse;
import secore.CrsCompoundHandler;
import secore.Resolver;
import java.net.URL;
import secore.db.DbManager;
import secore.db.Database;
import secore.util.Config;
import secore.util.StringUtil;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Dimitar Misev
 */
public class CrsCombineHandlerTest extends BaseTest {
  
  private static Database db;
  private static CrsCompoundHandler handler;

  @BeforeClass
  public static void setUpClass() throws Exception {
    Config.getInstance();
    handler = new CrsCompoundHandler();
  }

  @AfterClass
  public static void tearDownClass() throws Exception {
    DbManager.getInstance().getDb().close();
  }

  /**
   * Test of handle method, of class CrsCombineHandler.
   */
  @Test
  public void testHandle() throws Exception {
    String uri = "/def/crs-combine?"
        + "1=http://localhost:8081/def/crs/EPSG/0/4326&"
        + "2=http://localhost:8081/def/crs/EPSG/0/4440";
    ResolveRequest req = StringUtil.buildRequest(uri);
    GmlResponse res = handler.handle(req);
    String expResult = getData("4326+4440.exp");
    assertEquals(expResult, res.getData());
  }

  /**
   * Test of handle method, of class CrsCombineHandler.
   */
  @Test
  public void testHandle2() throws Exception {
    String uri = "http://localhost:8081/def/crs-combine?"
        + "1=http://localhost:8081/def/crs/EPSG/0/4326&"
        + "2=http://localhost:8081/def/crs/EPSG/0/4440";
    GmlResponse res = Resolver.resolve(new URL(uri));
    String expResult = getData("4326+4440.exp");
    assertEquals(expResult, res.getData());
  }

  /**
   * Test of getOperation method, of class CrsCombineHandler.
   */
  @Test
  public void testGetOperation() {
    String expResult = "crs-combine";
    String result = handler.getOperation();
    assertEquals(expResult, result);
  }
}
