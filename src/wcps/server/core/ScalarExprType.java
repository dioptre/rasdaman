/*
 * This file is part of %PACKAGENAME%.
 *
 * %PACKAGENAME% is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * %PACKAGENAME% is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with %PACKAGENAME%. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.%PACKAGENAME%.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package wcps.server.core;

import org.w3c.dom.*;

public class ScalarExprType implements IRasNode {
    private String type; // can be simple (just a value), unary, binary, function or defered
    private String value;
    private String op;
    private IRasNode first,second;
    
    public ScalarExprType(Node node, ProcessCoverageRequest pcr) throws WCPSException {
	while (node.getNodeName().equals("#text")) node = node.getNextSibling();	
	String nodeName = node.getNodeName();
	
	if (nodeName.equals("scalar")) {
	    type = "simple";	    
	    value = node.getFirstChild().getNodeValue();	    
	} else if (nodeName.equals("scalarUnaryPlus") || nodeName.equals("scalarUnaryMinus")) {
	    type = "unary";
	    op = (nodeName.equals("scalarUnaryPlus") ? "+" : "-");
	    first = new ScalarExprType(node.getFirstChild(), pcr);
	} else if (nodeName.equals("scalarPlus") || nodeName.equals("scalarMinus") || nodeName.equals("scalarMult") || nodeName.equals("scalarDiv")) {
	    type = "binary";
	    if (nodeName.equals("scalarPlus")) op = "+";
	    if (nodeName.equals("scalarMinus")) op = "-";
	    if (nodeName.equals("scalarMult")) op = "*";
	    if (nodeName.equals("scalarDiv")) op = "/";
	    Node child = node.getFirstChild();
	    first = new ScalarExprType(child, pcr);
	    child = child.getNextSibling();
	    second = new ScalarExprType(child, pcr);
	} else if (nodeName.equals("scalarAbs")) {
	    type= "function";
	    op = "abs";
	    first = new ScalarExprType(node.getFirstChild(), pcr);
	} else {
	    type = "defered";
	    try {
		first = new BooleanScalarExprType(node, pcr);
	    } catch (WCPSException e) {}

	    if (first == null) {
		try {
		    first = new ReduceScalarExprType(node, pcr);
	        } catch (WCPSException e) {}
	    }
// TODO(smsorin): This should be added, eventually...	    
/*	    if (first == null) {
		try {
		    first = new CondenseScalarExprType(Node, pcr);
	        } catch (WCPSException e) {}
	    }
 	    		
	    if (first == null) {
		try {
		    first = new GetMetadataScalarExprType(Node, pcr);
	        } catch (WCPSException e) {}
	    }*/
	    
	    if (first == null) 
		throw new WCPSException("Unexpected Scalar Expression node : " + node.getNodeName());
	}
    }
    
    public String toRasQL() {
	if (type.equals("simple"))
	    return value;
	if (type.equals("unary"))
	    return op + "(" + first.toRasQL() + ")";
	if (type.equals("binary")) 
	    return "(" + first.toRasQL() + ")" + op + "(" + second.toRasQL() + ")";
	if (type.equals("function"))
	    return op + "(" + first.toRasQL() + ")";
	if (type.equals("defered"))
	    return first.toRasQL();
	return "";
    }
}
