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

public class BooleanScalarExpr implements IRasNode {

    private IRasNode first, second;
    private String op;
    private boolean simple;    // true if the expression is just a value
    private String value;

    public BooleanScalarExpr(Node node, XmlQuery xq) throws WCPSException {
        if (node == null) {
            throw new WCPSException("Unexpected null node !");
        }

        String nodeName = node.getNodeName();

        simple = false;

        System.err.println("Parsing boolean scalar operation ...");

        if (nodeName.equals("booleanConstant")) {
            simple = true;
            value = node.getFirstChild().getNodeValue();
        } else if (nodeName.equals("booleanAnd")
                || nodeName.equals("booleanOr")
                || nodeName.equals("booleanXor")
                || nodeName.equals("booleanLessThan")
                || nodeName.equals("booleanLessOrEqual")
                || nodeName.equals("booleanGreaterThan")
                || nodeName.equals("booleanGreaterOrEqual")
                || nodeName.equals("booleanEqualNumeric")
                || nodeName.equals("booleanNotEqualNumeric")
                || nodeName.equals("booleanEqualString")
                || nodeName.equals("booleanNotEqualString")) {
            // Logical operations
            if (nodeName.equals("booleanAnd")
                    || nodeName.equals("booleanOr")
                    || nodeName.equals("booleanXor")) {
                // Remove the "boolean" in front
                op = nodeName.substring(7).toLowerCase();

                Node child = node.getFirstChild();

                first = new BooleanScalarExpr(child, xq);
                child = child.getNextSibling();
                second = new BooleanScalarExpr(child, xq);
            } else // Boolean Comparison operations between numbers or strings
            {
                if (nodeName.equals("booleanLessThan") || nodeName.equals("booleanLessOrEqual")
                        || nodeName.equals("booleanGreaterThan") || nodeName.equals("booleanGreaterOrEqual")
                        || nodeName.equals("booleanEqualNumeric") || nodeName.equals("booleanNotEqualNumeric")) {
                    if (nodeName.equals("booleanLessThan")) {
                        op = "<";
                    }
                    if (nodeName.equals("booleanLessOrEqual")) {
                        op = "<=";
                    }
                    if (nodeName.equals("booleanGreaterThan")) {
                        op = ">";
                    }
                    if (nodeName.equals("booleanGreaterOrEqual")) {
                        op = ">=";
                    }
                    if (nodeName.equals("booleanEqualNumeric")) {
                        op = "=";
                    }
                    if (nodeName.equals("booleanNotEqualNumeric")) {
                        op = "!=";
                    }

                    Node child = node.getFirstChild();

                    first = new NumericScalarExpr(child, xq);
                    child = child.getNextSibling();
                    second = new NumericScalarExpr(child, xq);
                } else {
                    if (nodeName.equals("booleanEqualString")) {
                        op = "=";
                    }
                    if (nodeName.equals("booleanNotEqualString")) {
                        op = "!=";
                    }

                    Node child = node.getFirstChild();

                    first = new StringScalarExpr(child, xq);
                    child = child.getNextSibling();
                    second = new StringScalarExpr(child, xq);
                }
            }

        } else if (nodeName.equals("booleanNot")) {
            op = "not";
            first = new BooleanScalarExpr(node.getFirstChild(), xq);
        } else {
            throw new WCPSException("Unexpected Binary Expression node : "
                    + node.getNodeName());
        }
        System.err.println("*** Boolean Scalar Expr SUCCESS: " + node.getNodeName());
    }

    public String toRasQL() {
        if (simple) {
            return value;
        }

        if (op.equals("not")) {
            return "not(" + first.toRasQL() + ")";
        }

        return "(" + first.toRasQL() + ")" + op + "(" + second.toRasQL() + ")";
    }
}
