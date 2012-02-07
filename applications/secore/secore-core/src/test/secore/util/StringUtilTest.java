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

import secore.util.SecoreException;
import secore.util.StringUtil;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Dimitar Misev
 */
public class StringUtilTest {

  /**
   * Test of urldecode method, of class StringUtil.
   */
  @Test
  public void testUrldecode() {
    String encodedText = "hello%20world";
    String expResult = "hello world";
    String result = StringUtil.urldecode(encodedText);
    assertEquals(expResult, result);
  }

  /**
   * Test of getElementValue method, of class StringUtil.
   */
  @Test
  public void testGetElementValue() {
    String xml = "<hello:world>HELLO WORLD</hello:world>";
    String elname = "world";
    String expResult = "HELLO WORLD";
    String result = StringUtil.getElementValue(xml, elname);
    assertEquals(expResult, result);
  }

  @Test
  public void testBuildRequest() throws SecoreException {
    String request = "/def/crs/EPSG/0/4326";
    String expResult = "ResolveRequest {\n"
        + "	params=[(EPSG,null), (0,null), (4326,null)]\n"
        + "	operation=crs\n"
        + "	service=/def/\n"
        + "	fullUri=/def/crs/EPSG/0/4326\n"
        + "}";
    String result = StringUtil.buildRequest(request).toString();
    assertEquals(expResult, result);
    
    request = "/def/crs?authority=EPSG/0/4326";
    expResult = "ResolveRequest {\n"
        + "	params=[(authority,EPSG), (0,null), (4326,null)]\n"
        + "	operation=crs\n"
        + "	service=/def/\n"
        + "	fullUri=/def/crs?authority=EPSG/0/4326\n"
        + "}";
    result = StringUtil.buildRequest(request).toString();
    assertEquals(expResult, result);
    
    request = "/def/crs?authority=EPSG&version=0&code=4326";
    expResult = "ResolveRequest {\n"
        + "	params=[(authority,EPSG), (version,0), (code,4326)]\n"
        + "	operation=crs\n"
        + "	service=/def/\n"
        + "	fullUri=/def/crs?authority=EPSG&version=0&code=4326\n"
        + "}";
    result = StringUtil.buildRequest(request).toString();
    assertEquals(expResult, result);
  }

  @Test
  public void testBuildRequest1() throws SecoreException {
    String request = "/def/crs-combine?"
        + "1=http://localhost:8080/def/crs/EPSG/0/4326&"
        + "2=http://localhost:8080/def/crs/EPSG/0&code=4440";
    String expResult = "ResolveRequest {\n"
        + "	params=[(1,http://localhost:8080/def/crs/EPSG/0/4326), (2,http://localhost:8080/def/crs/EPSG/0&code)]\n"
        + "	operation=crs-combine\n"
        + "	service=/def/\n"
        + "	fullUri=/def/crs-combine?1=http://localhost:8080/def/crs/EPSG/0/4326&2=http://localhost:8080/def/crs/EPSG/0&code=4440\n"
        + "}";
    String result = StringUtil.buildRequest(request).toString();
    assertEquals(expResult, result);
  }
}
