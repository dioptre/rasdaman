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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WCPSException;
import org.w3c.dom.*;
import petascope.exceptions.ExceptionCode;
import petascope.util.CrsUtil;

// TODO: implement class MetadataScalarExprType
public class MetadataScalarExpr implements IRasNode {
    
    private static Logger log = LoggerFactory.getLogger(MetadataScalarExpr.class);
    
    private IRasNode child;
    private CoverageExpr coverageExprType;
    private CoverageInfo coverageInfo;
    private String op;
    private String lo, hi;

    public MetadataScalarExpr(Node node, XmlQuery xq) throws WCPSException {
        String nodeName = node.getNodeName();
        log.trace(nodeName);
        
        Node child = node.getFirstChild();
        while (child.getNodeName().equals("#text")) {
            child = child.getNextSibling();
        }
        
        // the first argument is always a coverage expression
        try {
            coverageExprType = new CoverageExpr(child, xq);
            coverageInfo = coverageExprType.getCoverageInfo();
            child = child.getNextSibling();
        } catch (WCPSException ex) {
            log.error("  expected a valid coverage expression as the first argument of a metadata expression", ex);
            throw ex;
        }
        
        op = nodeName;
        if (nodeName.equals("DomainMetadata")) {
            AxisName axis = new AxisName(child, xq);
            int axisIndex = coverageInfo.getDomainIndexByName(axis.toRasQL());
            DomainElement domainElement = coverageInfo.getDomainElement(axisIndex);
            if (domainElement.getNumLo() == null) {
                lo = domainElement.getStrLo();
                hi = domainElement.getStrHi();
            } else {
                lo = domainElement.getNumLo().toString();
                hi = domainElement.getNumHi().toString();
            }
        } else if (nodeName.equals("imageCrsDomain")) {
            AxisName axis = new AxisName(child, xq);
            int axisIndex = coverageInfo.getDomainIndexByName(axis.toRasQL());
            CellDomainElement cellDomain = coverageInfo.getCellDomainElement(axisIndex);
            lo = cellDomain.getLo().toString();
            hi = cellDomain.getHi().toString();
        }
    }

    public String toRasQL() {
        String ret = "";
        if (op.equals("identifier")) {
            ret = coverageInfo.getCoverageName();
        } else if (op.equals("imageCrs")) {
            ret = CrsUtil.IMAGE_CRS;
        } else if (op.equals("DomainMetadata") || op.equals("imageCrsDomain")) {
            ret = "(" + lo + "," + hi + ")";
        }
        return ret;
    }
}
