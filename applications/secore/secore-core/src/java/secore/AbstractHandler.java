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

import secore.db.DbManager;
import secore.util.SecoreException;
import java.util.ArrayList;
import java.util.List;
import static secore.util.Constants.*;

/**
 * An abstract implementation of {@link Handler}, which provides some
 * convenience methods to concrete implementations.
 *
 * @author Dimitar Misev
 */
public abstract class AbstractHandler implements Handler {

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
}
