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
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.WCPSException;
import org.w3c.dom.*;

public class BinaryOperationCoverageExpr implements IRasNode, ICoverageInfo {

    private IRasNode first, second;
    private CoverageExprPairType pair;
    private CoverageInfo info;
    private String operation;

    public BinaryOperationCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException, InvalidCrsException {
        String nodeName = node.getNodeName();

        System.err.println("Trying to parse binary operation: " + nodeName);
        boolean okay = false;    // will be true if the node is recognized

        if (nodeName.equals("plus")) {
            operation = "+";
            okay = true;
        }

        if (nodeName.equals("minus")) {
            operation = "-";
            okay = true;
        }

        if (nodeName.equals("mult")) {
            operation = "*";
            okay = true;
        }

        if (nodeName.equals("div")) {
            operation = "/";
            okay = true;
        }

        if (nodeName.equals("and") || nodeName.equals("or") || nodeName.equals("xor")) {
            operation = nodeName;
            okay = true;
        }

        if (nodeName.equals("equals")) {
            operation = "=";
            okay = true;
        }

        if (nodeName.equals("lessThan")) {
            operation = "<";
            okay = true;
        }

        if (nodeName.equals("greaterThan")) {
            operation = ">";
            okay = true;
        }

        if (nodeName.equals("lessOrEqual")) {
            operation = "<=";
            okay = true;
        }

        if (nodeName.equals("greaterOrEqual")) {
            operation = ">=";
            okay = true;
        }

        if (nodeName.equals("notEqual")) {
            operation = "!=";
            okay = true;
        }

        if (nodeName.equals("overlay")) {
            operation = "overlay";
            okay = true;
        }

        if (!okay) {
            throw new WCPSException("Unexpected binary operation : " + nodeName);
        }

        Node operand = node.getFirstChild();

        while (operand.getNodeName().equals("#text")) {
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
