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

public class BooleanScalarExprType implements IRasNode {
    private boolean simple; // true if the expression is just a value
    private String value;
    private String op;
    private BooleanScalarExprType first, second;
    
    public BooleanScalarExprType(Node node, ProcessCoverageRequest pcr) throws WCPSException {
	String nodeName = node.getNodeName();
	simple = false;
	if (nodeName.equals("boolean")) {
	    simple = true;
	    value = node.getFirstChild().getNodeValue();
	} else if (nodeName.equals("scalarAnd") || nodeName.equals("scalarOr") || nodeName.equals("scalarXor")) {
	    if (nodeName.equals("scalarAnd")) op = "and";
	    if (nodeName.equals("scalarOr")) op = "or";
	    if (nodeName.equals("scalarXor")) op = "xor";
	    Node child = node.getFirstChild();
	    first = new BooleanScalarExprType(child, pcr);
	    child = child.getNextSibling();
	    second = new BooleanScalarExprType(child, pcr);
	} else if (nodeName.equals("scalarNot")) {
	    op = "not";
	    first = new BooleanScalarExprType(node.getFirstChild(), pcr);
	} else throw new WCPSException("Unexpected Binary Expression node : " + node.getNodeName());    	    
    }
    
    public String toRasQL() {
	if (simple) return value;
	if (op.equals("not")) return "not(" + first.toRasQL() + ")";
	return "(" + first.toRasQL() + ")" + op + "(" + second.toRasQL() + ")";
    }
}
