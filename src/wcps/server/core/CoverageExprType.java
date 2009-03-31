package wcps.server.core;

import org.w3c.dom.*;
import java.util.Iterator;

public class CoverageExprType implements IRasNode, ICoverageInfo {

    private boolean simpleCoverage; // True is the coverage is just a string
    private IRasNode child;
    private String childInfo;
    private CoverageInfo info;
    
    public CoverageInfo getCoverageInfo() {
	return info;
    }
    
    public CoverageExprType (Node node, ProcessCoverageRequest pcr) throws WCPSException {
        while (node != null && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        if (node == null) throw new WCPSException("CoverageExprType parsing error!");	
        String nodeName = node.getNodeName();
        simpleCoverage = false;
        System.err.println("CoverageExprType: node " + nodeName);
        
        if (nodeName.equals("coverage")) {
            simpleCoverage = true;
            childInfo = node.getFirstChild().getNodeValue();
            if (!pcr.isIteratorDefined(childInfo)) 
            throw new WCPSException("Iterator " + childInfo + " not defined!");                        

            Iterator<String> coverages = pcr.getCoverages(childInfo);
            
            info = new CoverageInfo(pcr.getMetadataSource().read(coverages.next()));
            
            while (coverages.hasNext()) { // Check if all the coverages are compatible
                CoverageInfo tmp = new CoverageInfo(pcr.getMetadataSource().read(coverages.next()));
                if (!tmp.isCompatible(info))
                    throw new WCPSException("Incompatible coverages within the same iterator");
            }
            System.err.println("Found simple coverage definition: " + childInfo + ", " + info.toString());
        } else if (nodeName.equals("trim")) {
            child = new TrimCoverageExprType(node, pcr);	    
        } else if (nodeName.equals("extend")) {
            //child = new ExtendCoverageExprType(node, pcr); 
            // TODO: add the class and remove this	    
            throw new WCPSException("Feature not yet implemented");
        } else if (nodeName.equals("slice")) {
            child = new SliceCoverageExprType(node, pcr);	    
        } else if (nodeName.equals("scale")) {
            //child = new ScaleCoverageExprType(node, pcr);
            // TODO: add the class and remove this	    
            throw new WCPSException("Feature not yet implemented");
        } else if (nodeName.equals("combine")) {
            child = new CombineRangeCoverageExprType(node, pcr); 	    
        } else if (nodeName.equals("crsTransform")) {
            child = new CrsTransformCoverageExprType(node, pcr);
        } else if (nodeName.equals("construct")) {
            //child = new ConstructcoverageExprType(node, pcr);
            // TODO: add the class and remove this	    
            throw new WCPSException("Feature not yet implemented");

        } else { // Try one of the groups
            try {
                child = new UnaryOperationCoverageExprType(node, pcr);
                System.err.println("Matched unary operation.");
            } catch (WCPSException e) {
                System.err.println("Failed to match Unary operation: " + e.toString() + "\nRetrying");
                child = null;
            }
            
            if (child == null) {
                try {
                    child = new BinaryOperationCoverageExprType(node, pcr);
                    System.err.println("Matched binary operation.");
                } catch (WCPSException e) {
                    System.err.println("Failed to match binary operation: " + e.toString() + "\nRetrying");
                    child = null;
                }
            }
            
            // TODO: implement the class and remove this
    /*	    if (child == null) {
            try {
                child = new SetMetadataCoverageExprType(node, pcr);
            } catch (WCPSException e) {
                child = null;
            }
            }*/
            
        }
        if (!simpleCoverage && child == null) {
            throw new WCPSException("Invalid coverage Expression, next node: " + node.getNodeName());
        }    
        if (info == null) {
            System.err.println(node.getNodeName());
            info = new CoverageInfo(((ICoverageInfo)child).getCoverageInfo());
        }
    }

    public String toRasQL() {
        if (simpleCoverage) 
            return childInfo;
        else
            return child.toRasQL();
    }
    
};
