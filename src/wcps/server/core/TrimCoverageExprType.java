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
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.math.BigInteger;


public class TrimCoverageExprType implements IRasNode, ICoverageInfo {

    private class axisSpec {
	public String axisName;
	public String crs;
	public List<Integer> coords = new ArrayList<Integer> ();
    };

    private List <axisSpec> axisList;
    private CoverageExprType coverageExprType;
    private CoverageInfo coverageInfo;
    private int dims;
    private String [] dim;

    public CoverageInfo getCoverageInfo() {
	return coverageInfo;
    }

    public TrimCoverageExprType (Node node, ProcessCoverageRequest source) throws WCPSException{
	Node child, axisNode;
	String nodeName;
	axisList = new ArrayList<axisSpec> ();

	for (child = node.getFirstChild(); child != null; child = child.getNextSibling()) {
	    nodeName = child.getNodeName();
	    if (nodeName.equals("#text")) continue;
	    
	    if (nodeName.equals("trimAxisSpec")) {
		axisSpec axis = new axisSpec();
		String childName;
		for (axisNode = child.getFirstChild(); axisNode != null; axisNode = axisNode.getNextSibling()) {
		    childName = axisNode.getNodeName();
		    if (childName.equals("axis")) {
			axis.axisName = axisNode.getFirstChild().getNodeValue();
			continue;
		    }
		    if (childName.equals("crs")) {
			axis.crs = axisNode.getFirstChild().getNodeValue();
			continue;
		    }
		    if (childName.equals("cellCoord") || childName.equals("domainCoord")) {
			CoordinateType coord = new CoordinateType(axisNode);
			axis.coords.add(coord.getValue());
		    }
		}
		axisList.add(axis);
		continue;
	    }

	    // else is a coverage expression type
	    coverageExprType = new CoverageExprType(child, source);
	    coverageInfo = coverageExprType.getCoverageInfo();
	}
	
	dims = coverageInfo.getNumDimensions();
	dim = new String[dims];
	for (int j=0; j<dims; ++j)
	    dim[j]="*:*";
	

	Iterator <axisSpec> i = axisList.iterator();
    System.out.println("AxisList count:" + axisList.size());
	axisSpec axis;
	int axisId;
	int axisLo, axisHi;
	while (i.hasNext()) {
	    axis = i.next();
	    axisId = coverageInfo.getDomainIndexByName(axis.axisName);
        System.out.println("Axis ID: " + axisId);
        System.out.println("Axis name: " + axis.axisName);
        System.out.println("Axis coords size: " + axis.coords.size());
        System.out.print("Axis coords: ");
        for (int ii=0; ii < axis.coords.size(); ii++)
            System.out.print(axis.coords.get(ii) + ", ");

	    axisLo = axis.coords.get(0);
	    axisHi = axis.coords.get(1);
	    dim[axisId]=axisLo+":"+axisHi;
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
