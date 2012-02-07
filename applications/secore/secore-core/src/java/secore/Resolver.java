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
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import static secore.util.Constants.*;

/**
 * Resolves a given request.
 *
 * @author Dimitar Misev
 */
public class Resolver {
  
  private static Logger log = LoggerFactory.getLogger(Resolver.class);

  private static List<Handler> handlers;

  static {
    handlers = new ArrayList<Handler>();
    registerHandler(new CrsCompoundHandler());
    registerHandler(new ParameterizedCrsHandler());
    registerHandler(new GeneralHandler());
    registerHandler(new AxisHandler());
  }

  /**
   * Add new handler to the registry.
   */
  public static void registerHandler(Handler handler) {
    handlers.add(handler);
  }

  public static List<Handler> getHandlers() {
    return handlers;
  }

  /**
   * Handle a request given a list of key-value pairs.
   * 
   * @param request contains the arguments as pairs
   * @return the response for the given request
   * @throws SecoreException when the resolver can not handle the given request, or in
   *  case of a mall-formed request.
   */
  public static GmlResponse resolve(ResolveRequest request) throws SecoreException {
    List<Pair<String, String>> args = request.getParams();
    if (args == null || args.isEmpty()) {
      throw new SecoreException(ExceptionCode.MissingParameterValue, "No arguments provided");
    }
    for (Pair<String, String> arg : args) {
      if (arg.fst == null) {
        throw new SecoreException(ExceptionCode.InvalidRequest, "Null key encountered");
      }
    }
    for (Handler handler : handlers) {
      if (handler.canHandle(request)) {
        log.debug("Selected " + handler.getClass().getSimpleName() + " to handle the request.");
        return handler.handle(request);
      }
    }
    throw new SecoreException(ExceptionCode.OperationNotSupported.locator(request.getOperation()), "Operation not defined");
  }

  /**
   * Resolve the given URI.
   * 
   * @param url the URI to resolve
   * @return the response
   * @throws SecoreException when the resolver can not handle the given request, or in
   *  case of a mall-formed request.
   */
  public static GmlResponse resolve(URL url) throws SecoreException {
    BufferedReader in = null;
    StringBuilder data = new StringBuilder(1000);
    
    try {
      in = new BufferedReader(new InputStreamReader(url.openStream()));
      String line;
      while ((line = in.readLine()) != null) {
        data.append(line);
        data.append(NEW_LINE);
      }
    } catch (Exception e) {  
      throw new SecoreException(ExceptionCode.IOConnectionError, "The request could not be retrieved", e);
    } finally {
      if (in != null) {
        try {
          in.close();
        } catch (Exception e) {
          throw new SecoreException(ExceptionCode.IOConnectionError, "The request could not be retrieved");
        }
      }
    }
    String ret = data.toString();
    if (ret.endsWith(NEW_LINE)) {
      ret = ret.substring(0, ret.length() - 1);
    }
    return new GmlResponse(ret);
  }
}
