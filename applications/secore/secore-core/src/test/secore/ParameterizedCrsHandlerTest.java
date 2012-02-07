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

import secore.ResolveRequest;
import secore.GmlResponse;
import secore.Resolver;
import secore.util.StringUtil;
import org.junit.Test;

/**
 *
 * @author Dimitar Misev
 */
public class ParameterizedCrsHandlerTest {

  /**
   * Test of handle method, of class ParameterizedCrsHandler.
   */
  @Test
  public void testHandle() throws Exception {
    System.out.println("handle");
    ResolveRequest request = StringUtil
        .buildRequest("/def/crs/AUTO/1.3/42001?lon=-100");
    GmlResponse res = Resolver.resolve(request);
    System.out.println(res.getData());
  }
}
