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

import secore.util.Config;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Dimitar Misev
 */
public class ConfigTest {

  @BeforeClass
  public static void setUpClass() throws Exception {
    assertFalse(Config.getInstance().getUsername().equals(""));
  }

  /**
   * Test of getPassword method, of class Config.
   */
  @Test
  public void testGetPassword() {
    assertFalse(Config.getInstance().getPassword().equals(""));
  }

  /**
   * Test of getUsername method, of class Config.
   */
  @Test
  public void testGetUsername() {
    assertFalse(Config.getInstance().getUsername().equals(""));
  }
}
