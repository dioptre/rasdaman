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

import secore.util.Pair;
import secore.util.SecoreException;
import secore.util.ExceptionCode;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Resolves axis identifiers.
 * In case a single parameter is provided in a RESTful request it is defaulted to a name search
 *
 */
public class AxisHandler extends AbstractHandler {

  private static Logger log = LoggerFactory.getLogger(AxisHandler.class);
  
  private static final String ELEMENT = "synonym";
  private static final String IDENTIFIER = "identifier";

  public GmlResponse handle(ResolveRequest request) throws SecoreException {
    log.debug("Handling resolve request...");
    List<Pair<String, String>> params = request.getParams();
    if (request.getOperation().equals(getOperation()) && params.size() == 1) {
      String name = params.get(0).fst;
      String id = resolveAttribute(ELEMENT, name);
      log.debug("Retrieved the identifier '" + id + "' of the synonym for " + name);
      String res = resolve(IDENTIFIER, id, 0);
      log.debug("Done, returning response.");
      return new GmlResponse(res);
    } else {
      log.error("Can't handle the given parameters, exiting with error.");
      throw new SecoreException(ExceptionCode.MissingParameterValue, "Insufficient parameters provided");
    }
  }

  public String getOperation() {
    return OP_AXIS;
  }
}
