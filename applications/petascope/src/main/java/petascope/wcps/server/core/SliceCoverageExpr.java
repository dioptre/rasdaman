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
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Node;
import petascope.util.Pair;
import petascope.util.WcsUtil;

public class SliceCoverageExpr implements IRasNode, ICoverageInfo {
    
    private static Logger log = LoggerFactory.getLogger(SliceCoverageExpr.class);

    private List<DimensionPointElement> axisList;
    private CoverageExpr coverageExprType;
    private CoverageInfo coverageInfo = null;
    private String[] dim;
    private DimensionPointElement elem;
    private int dims;

    public SliceCoverageExpr(Node node, XmlQuery xq) throws WCPSException {
        log.trace(node.getNodeName());
        
        Node child = node.getFirstChild();
        String nodeName;

        axisList = new ArrayList<DimensionPointElement>();

        while (child != null) {
            nodeName = child.getNodeName();

            if (nodeName.equals("#text")) {
                child = child.getNextSibling();
                continue;
            }

            if (nodeName.equals("axis")) {
                // Start a new axis and save it
                log.trace("  axis");
                elem = new DimensionPointElement(child, xq, coverageInfo);
                axisList.add(elem);
                child = elem.getNextNode();
            } else {
                try {
                    log.trace("  coverage");
                    coverageExprType = new CoverageExpr(child, xq);
                    coverageInfo = coverageExprType.getCoverageInfo();
                    child = child.getNextSibling();
                    continue;
                } catch (WCPSException e) {
                    log.error("  expected coverage node, got " + nodeName);
                    throw new WCPSException("Unknown node for SliceCoverage expression:" + child.getNodeName());
                }
            }
        }

        dims = coverageInfo.getNumDimensions();
        log.trace("  number of dimensions: " + dims);
        dim = new String[dims];

        for (int j = 0; j < dims; ++j) {
            dim[j] = "*:*";
        }

        Iterator<DimensionPointElement> i = axisList.iterator();
        DimensionPointElement axis;
        int axisId;
        int slicingPosInt;
        String slicingPosStr;

        while (i.hasNext()) {
            axis = i.next();
            /* TODO: BUG: This searches the axis types list using name, not type */
            axisId = coverageInfo.getDomainIndexByName(axis.getAxisName());
            slicingPosStr = axis.getSlicingPosition();
            dim[axisId] = slicingPosStr;
            // Slicing position can be a constant number or a variable reference
            try {
                slicingPosInt = Integer.parseInt(slicingPosStr);
            } catch (NumberFormatException e) {
                slicingPosInt = 1;
            }
            log.trace("  slice at axis id: " + axisId + ", axis name: " + axis.getAxisName() + ", slicing position: " + slicingPosInt);
            coverageInfo.setCellDimension(
                    axisId,
                    new CellDomainElement(
                    BigInteger.valueOf(slicingPosInt), BigInteger.valueOf(slicingPosInt), axis.getAxisName()));

        }

    }

    @Override
    public CoverageInfo getCoverageInfo() {
        return coverageInfo;
    }

    @Override
    public String toRasQL() {
        Pair<String[], String> res = computeRasQL();
        
        String ret = "(" + res.snd + ") [";
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
