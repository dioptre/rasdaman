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

import secore.GmlResponse;
import secore.util.Constants;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Dimitar Misev
 */
public class GmlResponseTest {

  /**
   * Test of getData method, of class GmlResponse.
   */
  @Test
  public void testGetData() {
    System.out.println("getData");
    String data = "<hello>\n" +
        "   <metaDataProperty>\n" +
        "      <adsf/>\n" +
        "   </metaDataProperty>\n" +
        "</hello>";
    GmlResponse instance = new GmlResponse(data);
    String expResult = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<hello>\n" +
        "   <metaDataProperty>\n" +
        "      <credits xmlns=\"http://rasdaman.org/ns\">" + Constants.COPYRIGHT + "</credits>\n" +
        "      <adsf/>\n" +
        "   </metaDataProperty>\n" +
        "</hello>";
    String result = instance.getData();
    
    assertEquals(expResult, result);
  }
}
