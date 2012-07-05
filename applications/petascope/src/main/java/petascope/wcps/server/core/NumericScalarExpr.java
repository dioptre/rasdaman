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

public class NumericScalarExpr implements IRasNode {
    
    private static Logger log = LoggerFactory.getLogger(NumericScalarExpr.class);

    private IRasNode first, second;
    private String op, value;
    private boolean twoChildren;
    private double dvalue;

    public NumericScalarExpr(Node node, XmlQuery xq) throws WCPSException {
        twoChildren = false;
        String nodeName = node.getNodeName();

        op = "";

        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        if (nodeName.equals("numericConstant")) {
            twoChildren = false;
            op = code(nodeName);
            value = node.getFirstChild().getNodeValue();
            try {
                dvalue = Double.parseDouble(value);
            } catch (NumberFormatException e) {
                throw new WCPSException("Could not understand constant: " + value);
            }
        } else if (nodeName.equals("complexConstant")
                || nodeName.equals("condense")
                || nodeName.equals("reduce")) {
            op = code(nodeName);
            twoChildren = false;
            if (nodeName.equals("complexConstant")) {
                first = new ComplexConstant(node, xq);
            }
            if (nodeName.equals("condense")) {
                first = new CondenseScalarExpr(node, xq);
            }
            if (nodeName.equals("reduce")) {
                first = new ReduceScalarExpr(node, xq);
            }
        } else if (nodeName.equals("numericUnaryMinus") 
                || nodeName.equals("numericSqrt")
                || nodeName.equals("numericAbs")) {
            op = code(nodeName);
            twoChildren = false;
            first = new NumericScalarExpr(node.getFirstChild(), xq);
        } else if (nodeName.equals("numericAdd")
                || nodeName.equals("numericMinus")
                || nodeName.equals("numericMult")
                || nodeName.equals("numericDiv")) {
            try {
                op = code(nodeName);
                twoChildren = true;
                Node child = node.getFirstChild();
                first = new NumericScalarExpr(child, xq);
                second = new NumericScalarExpr(child.getNextSibling(), xq);
            } catch (WCPSException e) {
                log.error("Failed to parse a numeric expression pair !");
            }
        } else if (nodeName.equals("variableRef")) {
            try {
                op = code(nodeName);
                twoChildren = false;
                first = new VariableReference(node, xq);
                log.trace("Matched variable reference: " + first.toRasQL());
            } catch (WCPSException e) {
                log.error("Failed to match variable reference: "
                        + e.toString());
            }
        } else {
            throw new WCPSException("Unexpected Numeric Scalar Expression node : "
                    + node.getNodeName());
        }
    }

    public String toRasQL() {
        String result = "";
        if (twoChildren == false)
        {
            if (op.equals("variable")) {
                result = first.toRasQL();
            } else if (op.equals("value")) {
                result = value;
            } else if (op.equals("-")) {
                    result = "-" + first.toRasQL();
            } else if (op.equals("sqrt")) {
                    result = "sqrt(" + first.toRasQL() + ")";
            } else if (op.equals("child")) {
                result = first.toRasQL();
            } else if (op.equals("abs")) {
                result = "abs(" + first.toRasQL() + ")";
            }
        }else if (twoChildren == true) {
            result = "(" + first.toRasQL() + ")" + op
                    + "(" + second.toRasQL() + ")";
        } else {
            return " error ";
        }

        return result;
    }

    private String code(String name) {
        String op = "";
        if (name.equals("numericConstant")) {
            op = "value";
        }
        if (name.equals("numericUnaryMinus") || name.equals("numericMinus")) {
            op = "-";
        }
        if (name.equals("numericAdd")) {
            op = "+";
        }
        if (name.equals("numericMult")) {
            op = "*";
        }
        if (name.equals("numericDiv")) {
            op = "/";
        }
        if (name.equals("numericSqrt")) {
            op = "sqrt";
        }
        if (name.equals("numericAbs")) {
            op  = "abs";
        }
        if (name.equals("condense") || name.equals("reduce")
                || name.equals("complexConstant")) {
            op = "child";
        }
        if (name.equals("variableRef")) {
            op = "variable";
        }

        return op;
    }

    public boolean isSingleValue() {
        return op.equals("value");
    }

    public double getSingleValue() {
        return dvalue;
    }
}
