package wcps.server.core;

import org.w3c.dom.*;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.math.BigInteger;

public class SliceCoverageExprType implements IRasNode, ICoverageInfo {

    private class axisSpec {
	public String axisName;
	public String crs;
	public int sliceId;
    };

    private List <axisSpec> axisList;
    private CoverageExprType coverageExprType;
    private CoverageInfo coverageInfo;
    private int dims;
    private String [] dim;

    public CoverageInfo getCoverageInfo() {
	return coverageInfo;
    }

    public SliceCoverageExprType (Node node, ProcessCoverageRequest source) throws WCPSException{
	Node child, axisNode;
	String nodeName;
	axisList = new ArrayList<axisSpec> ();

	for (child = node.getFirstChild(); child != null; child = child.getNextSibling()) {
	    nodeName = child.getNodeName();
	    if (nodeName.equals("#text"))
		continue;
	    
	    if (nodeName.equals("sliceAxisSpec")) {
		axisSpec axis = new axisSpec();
		String childName;
		for (axisNode = child.getFirstChild(); axisNode != null; axisNode = axisNode.getNextSibling()) {
		    childName = axisNode.getNodeName();
		    if (childName.equals("#text")) continue;
		    
		    if (childName.equals("axis")) {
			axis.axisName = axisNode.getFirstChild().getNodeValue();
			continue;
		    }
		    if (childName.equals("crs")) {
			axis.crs = axisNode.getFirstChild().getNodeValue();
			continue;
		    }
		    if (childName.equals("coord")) {
			CoordinateType coord = new CoordinateType(axisNode);
			axis.sliceId = coord.getValue();
		    }
		}
		axisList.add(axis);
		continue;
	    }

	    // else is a coverage expression type
	    coverageExprType = new CoverageExprType(child, source);
	    coverageInfo = new CoverageInfo(coverageExprType.getCoverageInfo());
	}
	dims = coverageInfo.getNumDimensions();
	dim = new String[dims];
	for (int j=0; j<dims; ++j)
	    dim[j]="*:*";
	
	Iterator <axisSpec> i = axisList.iterator();
	axisSpec axis;
	int axisId;
	int axisLo, axisHi;
	while (i.hasNext()) {
	    axis = i.next();
	    axisId = coverageInfo.getDomainIndexByName(axis.axisName);
	    axisLo = axis.sliceId;
	    axisHi = axis.sliceId;
	    dim[axisId]=""+axisLo;
	    coverageInfo.setCellDimension(axisId, new CellDomainElement(BigInteger.valueOf(axisLo), BigInteger.valueOf(axisHi)));	    
	}
	
    }

    public String toRasQL() {
	String result = coverageExprType.toRasQL()+"[";
	for (int j=0; j<dims; ++j) {
	    if (j>0)
		result += ",";
	    result += dim[j];
	}
	result += "]";
	return result;
    }    
};
