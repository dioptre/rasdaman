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
import java.util.Vector;
import org.w3c.dom.*;

public class CondenseScalarExpr implements IRasNode {

    private CondenseOperation op;
    private Vector<AxisIterator> iterators;
    private IRasNode using;
    private IRasNode where;
    private String axisIteratorString;
    private String newIteratorName;

    public CondenseScalarExpr(Node node, XmlQuery xq) throws WCPSException {
        if (node.getNodeName().equals("condense")) {
            node = node.getFirstChild();
        }
        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        iterators = new Vector();
        newIteratorName = xq.registerNewExpressionWithVariables();

        while (node != null) {
            String name = node.getNodeName();
            if (op == null) {
                op = new CondenseOperation(node, xq);
            } else if (name.equals("iterator")) {
                AxisIterator it = new AxisIterator(node.getFirstChild(), xq, newIteratorName);
                iterators.add(it);
            } else if (name.equals("where")) {
                where = new BooleanScalarExpr(node.getFirstChild(), xq);
            } else {
                using = new CoverageExpr(node, xq);
            }

            node = node.getNextSibling();
            while ((node != null) && node.getNodeName().equals("#text")) {
                node = node.getNextSibling();
            }
        }

        buildAxisIteratorDomain();
    }

    public String toRasQL() {
        String result = "condense " + op.toRasQL() + " over ";
        result += axisIteratorString;
        if (where != null) {
            result += where.toRasQL();
        }
        result += " using " + using.toRasQL();
        return result;
    }

    /* Concatenates all the AxisIterators into one large multi-dimensional object,
     * that will be used to build to RasQL query */
    private void buildAxisIteratorDomain() {
        axisIteratorString = "";
        axisIteratorString += newIteratorName + " in [";

        for (int i = 0; i < iterators.size(); i++) {
            if (i > 0) {
                axisIteratorString += ", ";
            }
            AxisIterator ai = iterators.elementAt(i);
            axisIteratorString += ai.getInterval();
        }

        axisIteratorString += "]";
    }
}
