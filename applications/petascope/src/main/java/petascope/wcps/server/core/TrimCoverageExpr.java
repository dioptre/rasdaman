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

import petascope.exceptions.WCPSException;
import org.w3c.dom.*;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import petascope.util.Pair;
import petascope.util.WcsUtil;

public class TrimCoverageExpr implements IRasNode, ICoverageInfo {
    
    private List<DimensionIntervalElement> axisList;
    private CoverageExpr coverageExprType;
    private CoverageInfo coverageInfo;
    private String[] dim;
    private int dims;
    private DimensionIntervalElement elem;

    public TrimCoverageExpr(Node node, XmlQuery xq) throws WCPSException {
        Node child, axisNode;
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
                coverageExprType = new CoverageExpr(child, xq);
                coverageInfo = coverageExprType.getCoverageInfo();
                child = child.getNextSibling();
                continue;
            } catch (WCPSException e) {
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

            // else unknown element
            throw new WCPSException("Unknown node for TrimCoverage expression:" + child.getNodeName());
        }
        
        // Afterward
        dims = coverageInfo.getNumDimensions();
        dim = new String[dims];

        for (int j = 0; j < dims; ++j) {
            dim[j] = "*:*";
        }


        Iterator<DimensionIntervalElement> i = axisList.iterator();

        System.out.println("Axis List count: " + axisList.size());
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
            System.out.println(dim[axisId]);
            coverageInfo.setCellDimension(
                    axisId,
                    new CellDomainElement(
                    BigInteger.valueOf(axisLo), BigInteger.valueOf(axisHi), axis.getAxisName()));
        }

    }

    @Override
    public CoverageInfo getCoverageInfo() {
        return coverageInfo;
    }

    @Override
    public String toRasQL() {
        Pair<String[], String> res = computeRasQL();
        
        String ret = res.snd + "[";
        for (int j = 0; j < res.fst.length; ++j) {
            if (j > 0) {
                ret += ",";
            }

            ret += res.fst[j];
        }

        ret += "]";
        return ret;
    }
    
    Pair<String[], String> computeRasQL() {
        Pair<String[], String> res = null;
        IRasNode c = coverageExprType.getChild();
        if (c instanceof SubsetOperationCoverageExpr) {
            res = ((SubsetOperationCoverageExpr) c).computeRasQL();
        } else if (c instanceof TrimCoverageExpr) {
            res = ((TrimCoverageExpr) c).computeRasQL();
        } else if (c instanceof SliceCoverageExpr) {
            res = ((SliceCoverageExpr) c).computeRasQL();
        } else {
            return Pair.of(dim, coverageExprType.toRasQL());
        }
        String[] a = res.fst;
        String[] b = new String[dims];
        for (int i = 0; i < dims; i++) {
            b[i] = WcsUtil.min(a[i], dim[i]);
        }
        
        return Pair.of(b, res.snd);
    }
}
