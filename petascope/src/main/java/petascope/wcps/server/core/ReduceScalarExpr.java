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

public class ReduceScalarExpr implements IRasNode {

    CoverageExpr expr;
    String op;

    public ReduceScalarExpr(Node node, XmlQuery xq) throws WCPSException {
        if (node.getNodeName().equals("reduce")) {
            node = node.getFirstChild();
        }
        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        System.err.println("Trying to parse ReduceScalarExpr ");
        String nodeName = node.getNodeName();

        if (nodeName.equals("all") || nodeName.equals("some") || nodeName.equals("count")
                || nodeName.equals("add") || nodeName.equals("avg") || nodeName.equals("min")
                || nodeName.equals("max")) {
            op = nodeName;

            if (!op.equals("all") && !op.equals("some")) {
                op = op + "_cells";
            }

            node = node.getFirstChild();

            while ((node != null) && (node.getNodeName().equals("#text"))) {
                node = node.getNextSibling();
            }

            expr = new CoverageExpr(node, xq);
        } else {
            throw new WCPSException("invalid ReduceScalarExprType node : " + nodeName);
        }
    }

    public String toRasQL() {
        return op + "(" + expr.toRasQL() + ")";
    }
}
