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

public class InducedOperationCoverageExpr implements IRasNode, ICoverageInfo {

    private IRasNode child;
    private CoverageInfo info;
    private String operation = "";

    public InducedOperationCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException {
        String nodeName = node.getNodeName();

        boolean okay = false;    // will be true if the node is recognized

        if (nodeName.equals("rangeConstructor")) {
            operation = nodeName;
            child = new RangeCoverageExpr(node, xq);
            info = new CoverageInfo((((ICoverageInfo) child).getCoverageInfo()));
        } else {    // Try one of the groups
            child = null;

            if (child == null) {
                try {
                    child = new UnaryOperationCoverageExpr(node, xq);
                    info = new CoverageInfo((((ICoverageInfo) child).getCoverageInfo()));
                    System.err.println("*** Induced Operation SUCCESS: " + node.getNodeName());
                } catch (WCPSException e) {
                    child = null;
                    if (e.getMessage().equals("Method not implemented")) {
                        throw e;
                    }
                }
            }

            if (child == null) {
                try {
                    child = new BinaryOperationCoverageExpr(node, xq);
                    info = new CoverageInfo((((ICoverageInfo) child).getCoverageInfo()));
                    System.err.println("*** Binary Operation SUCCESS: " + node.getNodeName());
                } catch (WCPSException e) {
                    child = null;
                }
            }

            if (child == null) {
                throw new WCPSException("Invalid induced coverage expression, next node: "
                        + node.getNodeName());
            }
        }

    }

    public CoverageInfo getCoverageInfo() {
        return info;
    }

    public String toRasQL() {
        return child.toRasQL();
    }
}
