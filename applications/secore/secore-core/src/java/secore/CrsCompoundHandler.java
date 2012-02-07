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

import secore.util.Config;
import secore.util.Pair;
import secore.util.StringUtil;
import secore.util.SecoreException;
import secore.util.ExceptionCode;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import static secore.util.Constants.*;

/**
 * Performs CRS combining into a compound CRS.
 *
 * @author Dimitar Misev
 */
class CrsCompoundHandler extends AbstractHandler {
  
  private static Logger log = LoggerFactory.getLogger(CrsCompoundHandler.class);

  public GmlResponse handle(ResolveRequest request) throws SecoreException {
    log.debug("Handling resolve request...");
    
    List<Pair<String, String>> params = request.getParams();
    
    if (request.getOperation().equals(getOperation()) && params.size() >= 1) {
      
      // component CRS URIs
      List<String> components = new ArrayList<String>();
    
      // get the component CRSs
      for (int i = 0; i < params.size(); i++) {
        String key = params.get(i).fst;
        String val = params.get(i).snd;
        if (val != null) {
          try {
            int ind = Integer.parseInt(key);
            if (ind == components.size() + 1) {
              // the value is a CRS reference, e.g. 1=crs_ref
              checkCrsRef(val);
              components.add(val);
            } else {
              // error
              log.error("Invalid " + getOperation() + " request, expected number " + 
                  (components.size() + 1) + " but got " + ind);
              throw new SecoreException(ExceptionCode.InvalidParameterValue, 
                  "Invalid " + getOperation() + " request, expected number " + 
                  (components.size() + 1) + " but got " + ind);
            }
          } catch (NumberFormatException ex) {
            // this is a key=value pair that needs to be added to the last component
            int ind = components.size() - 1;
            if (ind < 0) {
              log.error("Invalid " + getOperation() + " request");
              throw new SecoreException(ExceptionCode.InvalidRequest, 
                  "Invalid " + getOperation() + " request");
            }
            // append to last component
            String component = components.get(ind);
            if (component.contains(FRAGMENT_SEPARATOR)) {
               component += PAIR_SEPARATOR;
            } else {
               component += FRAGMENT_SEPARATOR;
            }
            components.set(ind, component + key + KEY_VALUE_SEPARATOR + val);
          }
        }
      }
      
      // they both must be specified
      if (components.size() < 2) {
        log.error("Can not combine less than two CRSs");
        throw new SecoreException(ExceptionCode.MissingParameterValue, 
            "Can not combine less than two CRSs");
      }
      
      // do some checking first, whether they are existing references
      String name = EMPTY;
      String comp = EMPTY;
      String code = EMPTY;
      for (String component : components) {
        try {
          URL crsRef = new URL(component);
          String res = Resolver.resolve(crsRef).getData();
          if (res.equals(EMPTY)) {
            throw new SecoreException(ExceptionCode.NoSuchDefinition,
                "Invalid CRS definition received for " + component);
          }
          if (!name.equals(EMPTY)) {
            name += " / ";
          }
          name += StringUtil.getElementValue(res, NAME_LABEL);
          String id = StringUtil.getElementValue(res, IDENTIFIER_LABEL);
          if (id == null) {
            throw new SecoreException(ExceptionCode.XmlNotValid,
                "Invalid CRS definition received for " + component);
          }
          comp += "   <componentReferenceSystem xlink:href='" + id + "'/>\n";
          code += "-" + id.substring(id.lastIndexOf(':') + 1);
        } catch (Exception ex) {
          log.error("Failed resolving CRS definition: " + component, ex);
          throw new SecoreException(ExceptionCode.NoSuchDefinition,
              "Failed resolving CRS definition: " + component, ex);
        }
      }
      
      String res = 
          "<CompoundCRS xmlns:gml='" + NAMESPACE_GML + "'\n" +
          "   xmlns:epsg='" + NAMESPACE_EPSG + "'\n" +
          "   xmlns:xlink='" + NAMESPACE_XLINK + "'\n" +
          "   gml:id='crs'>\n" +
          "   <metaDataProperty>\n" +
          "      <epsg:CommonMetaData>\n" +
          "         <epsg:type>compound</epsg:type>\n" +
          "      </epsg:CommonMetaData>\n" +
          "   </metaDataProperty>\n" +
          "   <identifier codeSpace='" + Config.getInstance().getCodespace() +
          "'>" + request.getFullUri().replaceAll("&", "%26") + "</identifier>" +
          "   <name>" + name + "</name>\n" + comp +
          "</CompoundCRS>";
      log.debug("Done, returning response.");
      return new GmlResponse(res);
    } else {
      log.error("Can't handle the given parameters, exiting with error.");
      throw new SecoreException(ExceptionCode.MissingParameterValue, 
          "Insufficient parameters provided");
    }
  }
  
  private void checkCrsRef(String crsRef) throws SecoreException {
    if (!crsRef.contains("/def/crs")) {
       log.error("Invalid crs-compound request, expected a CRS reference, but got " + crsRef);
       throw new SecoreException(ExceptionCode.InvalidParameterValue, 
           "Invalid " + getOperation() + " request, expected a CRS reference, but got " + crsRef);
    }
  }

  public String getOperation() {
    return OP_CRS_COMPOUND;
  }
}
