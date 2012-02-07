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

import secore.util.SecoreException;

/**
 * Handles a given request.
 *
 * @author Dimitar Misev
 */
public interface Handler {
  
  String OP_AXIS = "axis";
  String OP_CRS = "crs";
  String OP_CRS_COMPOUND = "crs-compound";
  
  /**
   * @param request a list of key-value pairs. The value can be null, but a null
   *  key will result in an exception.
   * @return true if this handler can handle the given request, false otherwise
   */
  boolean canHandle(ResolveRequest request);
  
  /**
   * In this method a specific handler tries to handle a request represented
   * by a list of arguments. The arguments are key-value pairs, as they have
   * been parsed from the URL that the front-end receives.
   * <p>
   * For example, a request for resolving the CRS with code 4326 by the EPSG
   * authority would be represented by this list:
   * [(crs, null), (authority, EPSG), (code, 4326)].
   * 
   * @param request a list of key-value pairs. The value can be null, but a null
   *  key will result in an exception.
   * @return the response for the given request
   * @throws SecoreException if a problem arises while the request is being handled,
   *  an exception is thrown
   */
  GmlResponse handle(ResolveRequest request) throws SecoreException;
  
  /**
   * @return this handler's operation id, e.g. crs, axis, crs-compound
   */
  String getOperation();
}
