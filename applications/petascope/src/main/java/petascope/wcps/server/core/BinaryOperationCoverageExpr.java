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
import petascope.util.WCPSConstants;

public class BinaryOperationCoverageExpr implements IRasNode, ICoverageInfo {
    
    private static Logger log = LoggerFactory.getLogger(BinaryOperationCoverageExpr.class);

    private IRasNode first, second;
    private CoverageExprPairType pair;
    private CoverageInfo info;
    private String operation;

    public BinaryOperationCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException {
        String nodeName = node.getNodeName();
        log.trace(nodeName);

        boolean okay = false;    // will be true if the node is recognized

        if (nodeName.equals(WCPSConstants.PLUS_S)) {
            operation = WCPSConstants.PLUS;
            okay = true;
        } else if (nodeName.equals(WCPSConstants.MINUS_S)) {
            operation = WCPSConstants.MINUS;
            okay = true;
        } else if (nodeName.equals(WCPSConstants.MULT)) {
            operation = WCPSConstants.STAR;
            okay = true;
        } else if (nodeName.equals(WCPSConstants.DIV_S)) {
            operation = WCPSConstants.DIV;
            okay = true;
        } else if (nodeName.equals(WCPSConstants.AND) || nodeName.equals(WCPSConstants.OR) || nodeName.equals("xor")) {
            operation = nodeName;
            okay = true;
        } else if (nodeName.equals(WCPSConstants.EQUALS)) {
            operation = WCPSConstants.EQUAL;
            okay = true;
        } else if (nodeName.equals(WCPSConstants.LESS_THAN)) {
            operation = "<";
            okay = true;
        } else if (nodeName.equals(WCPSConstants.GREATER_THAN)) {
            operation = ">";
            okay = true;
        } else if (nodeName.equals(WCPSConstants.LESS_OR_EQUAL)) {
            operation = "<=";
            okay = true;
        } else if (nodeName.equals(WCPSConstants.GREATER_OR_EQUAL)) {
            operation = ">=";
            okay = true;
        } else if (nodeName.equals(WCPSConstants.NOT_EQUALS)) {
            operation = "!=";
            okay = true;
        } else if (nodeName.equals(WCPSConstants.OVERLAY)) {
            operation = WCPSConstants.OVERLAY;
            okay = true;
        }

        if (!okay) {
            throw new WCPSException(WCPSConstants.UNEXPECTED_BINARY + " : " + nodeName);
        }
        log.trace("  " + WCPSConstants.OPERATION + ": " + operation);
        
        Node operand = node.getFirstChild();
        while (operand.getNodeName().equals("#" + WCPSConstants.TEXT)) {
            operand = operand.getNextSibling();
        }

        pair = new CoverageExprPairType(operand, xq);
        info = new CoverageInfo(((ICoverageInfo) pair).getCoverageInfo());
        first = pair.getFirst();
        second = pair.getSecond();
    }

    public CoverageInfo getCoverageInfo() {
        return info;
    }

    public String toRasQL() {
        return "((" + first.toRasQL() + ")" + operation + "(" + second.toRasQL() + "))";
    }
}
