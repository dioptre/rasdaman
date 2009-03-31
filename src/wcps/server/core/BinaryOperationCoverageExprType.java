package wcps.server.core;

import org.w3c.dom.*;

public class  BinaryOperationCoverageExprType implements IRasNode, ICoverageInfo {
    
    private IRasNode first, second;
    private String operation;
    private CoverageInfo info;
    
    public BinaryOperationCoverageExprType(Node node, ProcessCoverageRequest pcr) throws WCPSException {
        String nodeName = node.getNodeName();
        System.err.println("Trying to parse binary operation: " + nodeName);
        boolean okay = false; // will be true if the node is recognized
        if (nodeName.equals("plus")) {
            operation = "+";
            okay = true;
        }
        if (nodeName.equals("minus")) {
            operation = "-";
            okay = true;
        }
        if (nodeName.equals("mult")) {
            operation = "*";
            okay = true;
        }
        if (nodeName.equals("div")) {
            operation = "/";
            okay = true;
        }

        if (nodeName.equals("and") || nodeName.equals("or") || nodeName.equals("xor")) {
            operation = nodeName;
            okay = true;
        }
        
        if (nodeName.equals("equals")) {
            operation = "=";
            okay = true;
        }
        if (nodeName.equals("lessThan")) {
            operation = "<";
            okay = true;
        }
        if (nodeName.equals("greaterThan")) {
            operation = ">";
            okay = true;
        }
        if (nodeName.equals("lessOrEqual")) {
            operation = "<=";
            okay = true;
        }
        if (nodeName.equals("greaterOrEqual")) {
            operation = ">=";
            okay = true;
        }	
        if (nodeName.equals("notEqual")) {
            operation = "!=";
            okay = true;
        }
        if (nodeName.equals("overlay")) {
            operation = "overlay";
            okay = true;
        }	    
        
        if (!okay) 
            throw new WCPSException("Unexpected binary operation : " + nodeName);
            
        Node operand = node.getFirstChild();
        while (operand.getNodeName().equals("#text")) operand = operand.getNextSibling();
        try {
            first = new ScalarExprType(operand, pcr);
            System.err.println("Matched scalar expression in binary operation " + nodeName + ": " + operand.getNodeName());
            
        } catch (WCPSException e) {
            System.err.println("Failed to match scalar expression in binary operation " + nodeName + ": " + operand.getNodeName());
        }
        if (first == null) {
            first = new CoverageExprType(operand, pcr);
            info = new CoverageInfo(((ICoverageInfo)first).getCoverageInfo());
            operand = operand.getNextSibling();
            while (operand.getNodeName().equals("#text")) 
                operand = operand.getNextSibling();
            try {
                second = new ScalarExprType(operand, pcr);
                System.err.println("Matched scalar expression in binary operation " + nodeName + ": " + operand.getNodeName());
            } catch (WCPSException e) {
                System.err.println("Failed to match scalar expression in binary operation " + nodeName + ": " + operand.getNodeName());
            }
            if (second == null) {
                second = new CoverageExprType(operand, pcr);
                if (!((ICoverageInfo)first).getCoverageInfo().isCompatible(((ICoverageInfo)second).getCoverageInfo()))
                    throw new WCPSException("Coverages are not compatible");
            }
        } else {
            operand = operand.getNextSibling();
            while (operand.getNodeName().equals("#text")) operand = operand.getNextSibling();	
            second = new CoverageExprType(operand, pcr);
            info = new CoverageInfo(((ICoverageInfo)second).getCoverageInfo());
        }	
    }

    public CoverageInfo getCoverageInfo() {
	return info;
    }

    public String toRasQL() {
	return "(" + first.toRasQL() + ")" + operation + "(" + second.toRasQL() + ")";
    }

}
