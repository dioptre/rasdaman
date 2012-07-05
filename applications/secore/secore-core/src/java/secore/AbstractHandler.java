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

import secore.db.DbManager;
import secore.util.SecoreException;
import java.util.ArrayList;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import static secore.util.Constants.*;
import secore.util.ExceptionCode;
import secore.util.Pair;

/**
 * An abstract implementation of {@link Handler}, which provides some
 * convenience methods to concrete implementations.
 *
 * @author Dimitar Misev
 */
public abstract class AbstractHandler implements Handler {
  
  private static Logger log = LoggerFactory.getLogger(AbstractHandler.class);

  public boolean canHandle(ResolveRequest request) {
    return getOperation().equals(request.getOperation());
  }

  /**
   * Returns the parent of element <code>el</code> which text content equals
   * <code>id</code>.
   * 
   * @param el element name
   * @param id text content to match
   * @param depth set the depth to which to dereference xlinks
   * @return the definition (parent element of el)
   * @throws SecoreException usually if the text content of el hasn't been matched with id
   */
  public String resolve(String el, String id, int depth) throws SecoreException {
    return resolve(el, id, depth, new ArrayList<Parameter>());
  }

  /**
   * Returns the parent of element <code>el</code> which text content equals
   * <code>id</code>.
   * 
   * @param el element name
   * @param id text content to match
   * @param depth set the depth to which to dereference xlinks
   * @param parameters parameters to substitute for target elements
   * @return the definition (parent element of el)
   * @throws SecoreException usually if the text content of el hasn't been matched with id
   */
  public String resolve(String el, String id, int depth, List<Parameter> parameters) throws SecoreException {
    String work = null;
    
    if (!parameters.isEmpty()) {
      String targets = EMPTY;
      for (Parameter parameter : parameters) {
        if (!targets.equals(EMPTY)) {
          targets += COMMA + NEW_LINE;
        }
        targets += "  replace value of node $res" + parameter.getTarget() +
            " with '" + parameter.getValue() + "'";
      }
      work =
        "	copy $res := local:flatten(doc('gml'), $id, 0)\n" +
        " modify (\n" +
        targets + 
        " )\n" +
        "	return $res\n";
    } else {
      work =
        "	let $res := local:flatten(doc('gml'), $id, 0)\n" +
        "	return $res\n";
    }
    
    // construct query
    String query =
        "declare namespace gml = \"" + NAMESPACE_GML + "\";\n" +
        "declare namespace xlink = \"" + NAMESPACE_XLINK + "\";\n" +
        "\n" +
        "declare function local:getid($d as document-node(), $id as xs:string) as element() {\n" +
        "	let $ret := $d//gml:" + el + "[text() = $id]/..\n" +
        "	return  if (empty($ret)) then\n" +
        "					 <empty/>\n" +
        "	       else\n" +
        "	       	$ret[last()]\n" +
        "};\n" +
        "\n" +
        "declare function local:flatten($d as document-node(), $id as xs:string, $depth as xs:integer) as element()* {\n" +
        "  copy $el := local:getid($d, $id)\n" +
        "  modify\n" +
        "  (\n" +
        "  for $c in $el/*[@xlink:href]\n" +
        "  return if ($depth < " + depth + ") then\n" +
        "  	replace node $c with local:flatten($d, $c/@xlink:href, $depth + 1)\n" +
        "	  else replace node $c with $c\n" +
        "  )\n" +
        "  return $el\n" +
        "};\n" +
        "\n" +
        "declare function local:work($id as xs:string) as element() {\n" +
        work +
        "};\n" +
        "local:work('" + id + "')";
    
    return DbManager.getInstance().getDb().query(query);
  }
  
  /**
   * Returns the value of the attribute with name "identifier" 
   * of the element <code>el</code> which text content equals to <code>id</code>.
   * 
   * @param el element name
   * @param id text content to match
   * @return the data value of the attribute 
   * @throws SecoreException usually if the text content of el hasn't been matched with id
   */
  public String resolveAttribute(String el, String id) throws SecoreException {
    String query = "declare namespace gml = \"" + NAMESPACE_GML + "\";\n"
      + "let $d := doc('gml')\n"
      + "return data($d//gml:" + el + "[text() = '" + id + "']/../@identifier)";
    return DbManager.getInstance().getDb().query(query);
  }
  
  public List<String> getComponentCRSs(ResolveRequest request, int componentNo) throws SecoreException {
    
    List<Pair<String, String>> params = request.getParams();

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
            log.error("Invalid " + getOperation() + " request, expected number "
                + (components.size() + 1) + " but got " + ind);
            throw new SecoreException(ExceptionCode.InvalidParameterValue,
                "Invalid " + getOperation() + " request, expected number "
                + (components.size() + 1) + " but got " + ind);
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
    if (components.size() < componentNo) {
      log.error("Expected at least " + componentNo + " CRSs, got " + components.size());
      throw new SecoreException(ExceptionCode.MissingParameterValue,
          "Expected at least " + componentNo + " CRSs, got " + components.size());
    }

    return components;
  }
  
  private void checkCrsRef(String crsRef) throws SecoreException {
    if (!crsRef.contains("/def/crs")) {
       log.error("Invalid " + getOperation() + " request, expected a CRS reference, but got " + crsRef);
       throw new SecoreException(ExceptionCode.InvalidParameterValue, 
           "Invalid " + getOperation() + " request, expected a CRS reference, but got " + crsRef);
    }
  }
}
