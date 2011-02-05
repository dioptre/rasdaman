/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.server.core;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import petascope.exceptions.WCPSException;
import org.w3c.dom.*;
import petascope.exceptions.ExceptionCode;

public class ScaleCoverageExpr implements IRasNode, ICoverageInfo {

    private List<DimensionIntervalElement> axisList;
    private CoverageExpr coverageExprType;
    private CoverageInfo coverageInfo;
    private String[] dim;
    private int dims;
    private DimensionIntervalElement elem;
    private FieldInterpolationElement fieldInterp;

    public ScaleCoverageExpr(Node node, XmlQuery xq) throws WCPSException {
        Node child;
        String nodeName;

        axisList = new ArrayList<DimensionIntervalElement>();

        child = node.getFirstChild();
        while (child != null) {
            nodeName = child.getNodeName();

            if (nodeName.equals("#text")) {
                child = child.getNextSibling();
                continue;
            }

            try {
                System.err.println("Trying out an CoverageExprType group...");
                coverageExprType = new CoverageExpr(child, xq);
                coverageInfo = coverageExprType.getCoverageInfo();
                child = child.getNextSibling();
                continue;
            } catch (WCPSException e) {
                System.err.println("This was no CoverageExprType: " + nodeName);
            }

            try {
                // Start a new axis and save it
                elem = new DimensionIntervalElement(child, xq, coverageInfo);
                axisList.add(elem);
                child = elem.getNextNode();
                continue;
            } catch (WCPSException e) {
                System.err.println("This was no Dimension Interval ELement: " + child.getNodeName());
            }

            try {
                fieldInterp = new FieldInterpolationElement(child, xq);
                child = fieldInterp.getNextNode();
                continue;
            } catch (WCPSException e) {
                System.err.println("This was no Field Interpolation Element: " + child.getNodeName());
            }

            // else unknown element
            throw new WCPSException(ExceptionCode.InvalidMetadata, "Unknown node for ScaleCoverageExpr expression:" + child.getNodeName());
        }

        dims = coverageInfo.getNumDimensions();
        dim = new String[dims];

        for (int j = 0; j < dims; ++j) {
            dim[j] = "*:*";
        }


        Iterator<DimensionIntervalElement> i = axisList.iterator();

        System.out.println("Axis List count:" + axisList.size());
        DimensionIntervalElement axis;
        int axisId;
        int axisLo, axisHi;

        while (i.hasNext()) {
            axis = i.next();
            axisId = coverageInfo.getDomainIndexByName(axis.getAxisName());
            System.out.println("Axis ID: " + axisId);
            System.out.println("Axis name: " + axis.getAxisName());
            System.out.print("Axis coords: ");

            axisLo = Integer.parseInt(axis.getLowCoord());
            axisHi = Integer.parseInt(axis.getHighCoord());
            dim[axisId] = axisLo + ":" + axisHi;
            coverageInfo.setCellDimension(
                    axisId,
                    new CellDomainElement(
                    BigInteger.valueOf(axisLo), BigInteger.valueOf(axisHi)));
        }

    }

    public String toRasQL() {
        String result = "scale( " + coverageExprType.toRasQL() + ", [";

        for (int j = 0; j < dims; ++j) {
            if (j > 0) {
                result += ",";
            }

            result += dim[j];
        }

        result += "] )";
        return result;
    }

    public CoverageInfo getCoverageInfo() {
        return coverageInfo;
    }
}
