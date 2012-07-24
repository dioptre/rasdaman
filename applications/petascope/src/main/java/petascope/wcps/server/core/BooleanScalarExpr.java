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

public class BooleanScalarExpr implements IRasNode {
    
    private static Logger log = LoggerFactory.getLogger(BooleanScalarExpr.class);

    private IRasNode first, second;
    private String op;
    private boolean simple;    // true if the expression is just a value
    private String value;

    public BooleanScalarExpr(Node node, XmlQuery xq) throws WCPSException {
        if (node == null) {
            throw new WCPSException(WCPSConstants.UNEXPECTED_NULL_NODE + " !");
        }

        String nodeName = node.getNodeName();

        simple = false;

        if (nodeName.equals(WCPSConstants.BOOLEAN_CONSTANT)) {
            simple = true;
            value = node.getFirstChild().getNodeValue();
        } else if (nodeName.equals(WCPSConstants.BOOLEAN_AND)
                || nodeName.equals(WCPSConstants.BOOLEAN_OR)
                || nodeName.equals(WCPSConstants.BOOLEAN_XOR)
                || nodeName.equals(WCPSConstants.BOOLEAN_LESSTHAN)
                || nodeName.equals(WCPSConstants.BOOLEAN_LESSOREQUAL)
                || nodeName.equals(WCPSConstants.BOOLEAN_GREATERTHAN)
                || nodeName.equals(WCPSConstants.BOOLEAN_GREATEROREQUAL)
                || nodeName.equals(WCPSConstants.BOOLEAN_EQUALNUMERIC)
                || nodeName.equals(WCPSConstants.BOOLEAN_NOTEQUALNUMERIC)
                || nodeName.equals(WCPSConstants.BOOLEAN_EQUAL_STRING)
                || nodeName.equals(WCPSConstants.BOOLEAN_NOT_EQUAL_STRING)) {
            // Logical operations
            if (nodeName.equals(WCPSConstants.BOOLEAN_AND)
                    || nodeName.equals(WCPSConstants.BOOLEAN_OR)
                    || nodeName.equals(WCPSConstants.BOOLEAN_XOR)) {
                // Remove the "boolean" in front
                op = nodeName.substring(7).toLowerCase();

                Node child = node.getFirstChild();

                first = new BooleanScalarExpr(child, xq);
                child = child.getNextSibling();
                second = new BooleanScalarExpr(child, xq);
            } else // Boolean Comparison operations between numbers or strings
            {
                if (nodeName.equals(WCPSConstants.BOOLEAN_LESSTHAN) || nodeName.equals(WCPSConstants.BOOLEAN_LESSOREQUAL)
                        || nodeName.equals(WCPSConstants.BOOLEAN_GREATERTHAN) || nodeName.equals(WCPSConstants.BOOLEAN_GREATEROREQUAL)
                        || nodeName.equals(WCPSConstants.BOOLEAN_EQUALNUMERIC) || nodeName.equals(WCPSConstants.BOOLEAN_NOTEQUALNUMERIC)) {
                    if (nodeName.equals(WCPSConstants.BOOLEAN_LESSTHAN)) {
                        op = "<";
                    }
                    if (nodeName.equals(WCPSConstants.BOOLEAN_LESSOREQUAL)) {
                        op = "<=";
                    }
                    if (nodeName.equals(WCPSConstants.BOOLEAN_GREATERTHAN)) {
                        op = ">";
                    }
                    if (nodeName.equals(WCPSConstants.BOOLEAN_GREATEROREQUAL)) {
                        op = ">=";
                    }
                    if (nodeName.equals(WCPSConstants.BOOLEAN_EQUALNUMERIC)) {
                        op = "=";
                    }
                    if (nodeName.equals(WCPSConstants.BOOLEAN_NOTEQUALNUMERIC)) {
                        op = "!=";
                    }

                    Node child = node.getFirstChild();

                    first = new NumericScalarExpr(child, xq);
                    child = child.getNextSibling();
                    second = new NumericScalarExpr(child, xq);
                } else {
                    if (nodeName.equals(WCPSConstants.BOOLEAN_EQUAL_STRING)) {
                        op = "=";
                    }
                    if (nodeName.equals(WCPSConstants.BOOLEAN_NOT_EQUAL_STRING)) {
                        op = "!=";
                    }

                    Node child = node.getFirstChild();

                    first = new StringScalarExpr(child, xq);
                    child = child.getNextSibling();
                    second = new StringScalarExpr(child, xq);
                }
            }

        } else if (nodeName.equals(WCPSConstants.BOOLEAN_NOT)) {
            op = WCPSConstants.NOT;
            first = new BooleanScalarExpr(node.getFirstChild(), xq);
        } else if (nodeName.equals(WCPSConstants.BIT)) {
            op = WCPSConstants.BIT;
            first = new CoverageExpr(node.getFirstChild(), xq);
            second = new NumericScalarExpr(node.getFirstChild().getNextSibling(), xq);
        } else {
            throw new WCPSException(WCPSConstants.UNEXPECTED_BINARY_EXPR_NODE + " : "
                    + node.getNodeName());
        }
        log.trace(WCPSConstants.BOOLEAN_SCALAR_EXPR + ": " + node.getNodeName());
    }

    public String toRasQL() {
        if (simple) {
            return value;
        }

        if (op.equals(WCPSConstants.NOT)) {
            return WCPSConstants.NOT + "(" + first.toRasQL() + ")";
        } else if (op.equals(WCPSConstants.BIT)) {
            return WCPSConstants.BIT + "(" + first.toRasQL() + "," + second.toRasQL() + ")";
        }

        return "(" + first.toRasQL() + ")" + op + "(" + second.toRasQL() + ")";
    }
}
