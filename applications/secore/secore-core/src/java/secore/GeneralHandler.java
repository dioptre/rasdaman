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

import secore.util.SecoreException;
import secore.util.ExceptionCode;
import secore.util.Pair;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import static secore.util.Constants.*;

/**
 * This handler should be invoked as a fallback to the more specific handler,
 * {@link CrsCompoundHandler}, {@link AxisHandler}, etc.
 *
 * @author Dimitar Misev
 */
public class GeneralHandler extends AbstractHandler {

  private static Logger log = LoggerFactory.getLogger(GeneralHandler.class);
  
  public static final String AUTHORITY_KEY = "authority";
  public static final String VERSION_KEY = "version";
  public static final String DEFAULT_VERSION = "0";
  public static final String CODE_KEY = "code";

  @Override
  public boolean canHandle(ResolveRequest request) {
    return request.getOperation() != null && request.getParams().size() == 3;
  }

  public GmlResponse handle(ResolveRequest request) throws SecoreException {
    log.debug("Handling resolve request...");
    return resolveRequest(request, 2);
  }
  
  public GmlResponse resolveRequest(ResolveRequest request, int depth) throws SecoreException {
    Pair<String, String> p = parseRequest(request);

    // try to resolve a URN first
    String res = null;
    try {
      res = resolve(IDENTIFIER_LABEL, p.fst, depth);
    } catch (SecoreException ex) {
      if (ExceptionCode.NoSuchDefinition.equals(ex.getExceptionCode())) {
        // URN resolution failed, try with a REST now
        res = resolve(IDENTIFIER_LABEL, p.snd, depth);
      } else {
        throw ex;
      }
    }

    // TODO check for the result, e.g. if the operation is datum than the result can only be GeodeticDatum
    log.debug("Done, returning response.");
    return new GmlResponse(res);
  }
  
  /**
   * Checks the request and returns a pair of URN/URL IDENTIFIER_LABELs to be looked
   * up in the database.
   * 
   * @param request resolving request
   * @return  pair of URN/URL IDENTIFIER_LABELs to be looked up in the database.
   * @throws SecoreException in case of an invalid request
   */
  Pair<String, String> parseRequest(ResolveRequest request) throws SecoreException {
    List<Pair<String, String>> params = request.getParams();

    if (request.getOperation() != null && params.size() == 3) {
      String authority = EMPTY;
      String version = EMPTY;
      String code = EMPTY;
      String res = null;
      for (int i = 0; i < params.size(); i++) {
        String key = params.get(i).fst;
        String val = params.get(i).snd;
        if (val == null) {
          if (authority.equals(EMPTY)) {
            authority = key;
          } else if (version.equals(EMPTY)) {
            version = key;
          } else if (code.equals(EMPTY)) {
            code = key;
          }
        } else if (key.equalsIgnoreCase(AUTHORITY_KEY)) {
          authority = val;
        } else if (key.equalsIgnoreCase(VERSION_KEY)) {
          version = val;
        } else if (key.equalsIgnoreCase(CODE_KEY)) {
          code = val;
        }
      }
      
      // check for empty parameters
      if (authority.equals(EMPTY)) {
        log.error("No authority specified.");
        throw new SecoreException(ExceptionCode.MissingParameterValue
            .locator(AUTHORITY_KEY), "Insufficient parameters provided");
      }
      if (version.equals(EMPTY)) {
        log.error("No version specified.");
        throw new SecoreException(ExceptionCode.MissingParameterValue
            .locator(VERSION_KEY), "Insufficient parameters provided");
      }
      if (code.equals(EMPTY)) {
        log.error("No code specified.");
        throw new SecoreException(ExceptionCode.MissingParameterValue
            .locator(CODE_KEY), "Insufficient parameters provided");
      }
      
      // try to resolve a URN first
      String urn = URN_PREFIX + URN_SEPARATOR + request.getOperation() + URN_SEPARATOR + authority
          + URN_SEPARATOR + (version.equals(DEFAULT_VERSION) ? EMPTY : version) + URN_SEPARATOR + code;
      String url = request.getService() + request.getOperation() + REST_SEPARATOR
          + authority + REST_SEPARATOR + version + REST_SEPARATOR + code;
      return Pair.of(urn, url);
    } else {
      log.error("Can't handle the given parameters, exiting with error.");
      throw new SecoreException(ExceptionCode.MissingParameterValue, "Insufficient parameters provided");
    }
  }

  public String getOperation() {
    return EMPTY;
  }
}
