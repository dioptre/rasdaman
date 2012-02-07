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

import static secore.util.Constants.*;

/**
 * Wrapper for GML responses.
 *
 * @author Dimitar Misev
 */
public class GmlResponse {
  
  private final String data;

  public GmlResponse(String data) {
    // Add copyright
    int ind = data.indexOf(METADATA_LABEL);
    if (ind != -1) {
      int end = data.indexOf(">", ind);
      int nextStart = data.indexOf("<", end);
      
      String firstPart = data.substring(0, end + 1);
      String secondPart = data.substring(end + 1);
      String indent = data.substring(end + 1, nextStart);
      
      // finally recompose data
      data = firstPart + indent + 
          "<credits xmlns=\"http://rasdaman.org/ns\">" + COPYRIGHT + "</credits>" +
          secondPart;
    }
    
    this.data = data;
  }
  
  /**
   * Gets the GML data stored in the object
   * Surrounds it by XML tags defining the document and declaring the required namespaces
   * 
   * @return the GML definition of the resource requested 
   */
  public String getData() {
    return XML_DECL + data;
  }
}
