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
import org.w3c.dom.*;
import petascope.util.WCPSConstants;

public class ExtendCoverageExpr implements IRasNode, ICoverageInfo {
    
    private static Logger log = LoggerFactory.getLogger(ExtendCoverageExpr.class);

    private List<DimensionIntervalElement> axisList;
    private CoverageExpr coverageExprType;
    private CoverageInfo coverageInfo;
    private String[] dim;
    private int dims;
    private DimensionIntervalElement elem;

    public ExtendCoverageExpr(Node node, XmlQuery xq) throws WCPSException {
        
        log.trace(node.getNodeName());

        Node child;
        String nodeName;

        axisList = new ArrayList<DimensionIntervalElement>();

        child = node.getFirstChild();
        while (child != null) {
            nodeName = child.getNodeName();

            if (nodeName.equals("#" + WCPSConstants.MSG_TEXT)) {
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
                log.trace("  " + WCPSConstants.MSG_ADD_NEW_AXIS + ": " + elem.getAxisName());
                axisList.add(elem);
                child = elem.getNextNode();
                continue;
            } catch (WCPSException e) {
                log.error(WCPSConstants.ERRTXT_THIS_WAS_NO_DIM + ": " + child.getNodeName());
            }

            // else unknown element
            throw new WCPSException(WCPSConstants.ERRTXT_UNKNOWN_NODE_EXTENDCOVERAGE + child.getNodeName());
        }

        dims = coverageInfo.getNumDimensions();
        log.trace("  " + WCPSConstants.MSG_NUMBER_OF_DIMENSIONS + ": " + dims);
        dim = new String[dims];

        for (int j = 0; j < dims; ++j) {
            dim[j] = "*:*";
        }


        Iterator<DimensionIntervalElement> i = axisList.iterator();
        DimensionIntervalElement axis;
        int axisId;
        int axisLo, axisHi;

        while (i.hasNext()) {
            axis = i.next();
            axisId = coverageInfo.getDomainIndexByName(axis.getAxisName());
            log.trace("  " + WCPSConstants.MSG_AXIS + " " + WCPSConstants.MSG_ID + ": " + axisId);
            log.trace("  " + WCPSConstants.MSG_AXIS + " " + WCPSConstants.MSG_NAME + ": " + axis.getAxisName());
            log.trace("  " + WCPSConstants.MSG_AXIS + " " + WCPSConstants.MSG_COORDS + ": ");

            axisLo = Integer.parseInt(axis.getLowCoord());
            axisHi = Integer.parseInt(axis.getHighCoord());
            dim[axisId] = axisLo + ":" + axisHi;
            coverageInfo.setCellDimension(
                    axisId,
                    new CellDomainElement(
                    BigInteger.valueOf(axisLo), BigInteger.valueOf(axisHi), axis.getAxisName()));
        }


    }

    public CoverageInfo getCoverageInfo() {
        return coverageInfo;
    }

    public String toRasQL() {
        String result = WCPSConstants.MSG_EXTEND + "(" + coverageExprType.toRasQL() + ",[";

        for (int j = 0; j < dims; ++j) {
            if (j > 0) {
                result += ",";
            }

            result += dim[j];
        }

        result += "])";

        return result;
    }
}
