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
package secore;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;

/**
 * Stuff commonly needed by JUnit tests.
 *
 * @author Dimitar Misev
 */
public abstract class BaseTest {
  
  public static final String TESTDATA_PATH = "etc/testdata/";
  
  public String getData(String file) {
    String path = TESTDATA_PATH + file;
    try {
      BufferedReader reader = new BufferedReader(
          new InputStreamReader(new DataInputStream(new FileInputStream(path))));
      StringBuilder ret = new StringBuilder();
      String line = null;
      while ((line = reader.readLine()) != null) {
        ret.append(line);
        ret.append('\n');
      }
      return ret.toString().trim();
    } catch (Exception ex) {
    }
    return null;
  }
}
