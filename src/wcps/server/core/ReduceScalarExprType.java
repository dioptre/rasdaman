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