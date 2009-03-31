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