/*
 * This file is part of Petascope.
 *
 * Petascope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Petascope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Petascope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.Petascope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package wcps.server.core;

import org.w3c.dom.*;

public class EncodeDataExprType implements IRasNode{

    private String format;
    private String extraParams;
    private IRasNode coverageExprType; 
    private Boolean store;
    private String mime;

    public EncodeDataExprType (Node node, ProcessCoverageRequest request) throws WCPSException {
	Node child;
	String nodeName;

	for (child = node.getFirstChild(); child != null; child = child.getNextSibling()) {
	    nodeName = child.getNodeName();

	    if (nodeName.equals("#text")) continue;
	    System.err.println("Encode : node : " + child.getNodeName());	    
	    
	    if (nodeName.equals("format")) {
		format = child.getFirstChild().getNodeValue();
		mime = request.getMetadataSource().mimetype(format);
		continue;
	    }

	    if (nodeName.equals("extraParameters")) {
		extraParams = child.getFirstChild().getNodeValue();
		continue;
	    }

	    coverageExprType = new CoverageExprType(child, request);
	}
	
	Node _store = node.getAttributes().getNamedItem("store");
	if (_store != null)
	    store = _store.getNodeValue().equals("true");
    }

    public String getMime() {
	return mime;
    }

    public String toRasQL() {
	// TODO: cjucovschi - implement store
	
	String result;
	if (format.equals("raw")) 
	    result = coverageExprType.toRasQL();
	else {
	    result = format + "(" + coverageExprType.toRasQL();
	    if (extraParams != null)
		result = result + ", " + extraParams;
	    result = result + ")";
	}
	return result;
    }    
};
