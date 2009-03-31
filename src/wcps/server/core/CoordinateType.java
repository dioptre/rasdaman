package wcps.server.core;

import org.w3c.dom.*;
import java.util.List;
import java.util.Iterator;

public class CoordinateType implements IRasNode {
    private int value;
    
    public CoordinateType(Node node) throws WCPSException {
	if (!node.getNodeName().equals("cellCoord") &&
            !node.getNodeName().equals("domainCoord"))
	    throw new WCPSException("Expected cellCoord or domainCoord, found " + node.getNodeName());

    System.out.println("CoordinateType: node name: " + node.getNodeName());
    System.out.println("CoordinateType: node child 1: " + node.getFirstChild().getNodeName());
    value = Integer.parseInt(node.getFirstChild().getNodeValue());
    }

    public int getValue() {
	return value;
    }
    
    public String toRasQL() {
	return ""+value;
    }
};
