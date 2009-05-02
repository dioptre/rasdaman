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

public class ReduceScalarExprType implements IRasNode {

    CoverageExprType expr;
    String op;
    
    public ReduceScalarExprType(Node node, ProcessCoverageRequest pcr) throws WCPSException {
	String nodeName = node.getNodeName();
	if (nodeName.equals("all") || nodeName.equals("some") || nodeName.equals("count") || nodeName.equals("add") || nodeName.equals("avg") ||
	    nodeName.equals("min") || nodeName.equals("max")) {
	    op = nodeName;
	    if (!op.equals("all") && !op.equals("some"))
		op = op + "_cells";
	    node = node.getFirstChild();
	    while (node != null && node.getNodeName() == "#text")
		node = node.getNextSibling();
	    expr = new CoverageExprType(node, pcr);
	} else
	    throw new WCPSException("invalid ReduceScalarExprType node : " + nodeName);
    }    

    public String toRasQL() {
    
	return op + "(" + expr.toRasQL() +")"; 
    }
}
