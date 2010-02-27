/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */
package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.WCPSException;
import org.w3c.dom.*;

public class UnaryOperationCoverageExpr implements IRasNode, ICoverageInfo {

    private CoverageExpr child;
    private CoverageInfo info;
    private String operation;
    private String params;

    public UnaryOperationCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException, InvalidCrsException {
        String nodeName = node.getNodeName();

        System.err.println("Trying to parse unary operation: " + nodeName);

        if (nodeName.equals("unaryPlus")) {
            operation = "+";
            child = new CoverageExpr(node.getFirstChild(), xq);
        } else if (nodeName.equals("unaryMinus")) {
            operation = "-";
            child = new CoverageExpr(node.getFirstChild(), xq);
        } else if (nodeName.equals("sqrt") || nodeName.equals("abs")
                || nodeName.equals("exp") || nodeName.equals("log") || nodeName.equals("ln")
                || nodeName.equals("sin") || nodeName.equals("cos") || nodeName.equals("tan")
                || nodeName.equals("sinh") || nodeName.equals("cosh")
                || nodeName.equals("tanh") || nodeName.equals("arcsin")
                || nodeName.equals("arccos") || nodeName.equals("arctan")
                || nodeName.equals("not") || nodeName.equals("re") || nodeName.equals("im")) {
            operation = nodeName;
            child = new CoverageExpr(node.getFirstChild(), xq);
        } else if (nodeName.equals("bit")) {
            operation = "bit";
            Node c = node.getFirstChild();

            while (c != null) {
                if (c.getNodeName().equals("#text")) {
                    c = c.getNextSibling();
                    continue;
                }

                if (c.getNodeName().equals("bitIndex")) {
                    try {
                        params = c.getFirstChild().getNodeValue();
                        int i = Integer.parseInt(params);
                        System.err.println("Found bitIndex = " + params);
                    } catch (NumberFormatException e) {
                        throw new WCPSException("Invalid Number as bitIndex: " + params);
                    }
                } else {
                    child = new CoverageExpr(c, xq);
                }

                c = c.getNextSibling();
            }
        } else if (nodeName.equals("cast")) {
            operation = "cast";
            Node c = node.getFirstChild();

            while (c != null) {
                if (c.getNodeName().equals("#text")) {
                    c = c.getNextSibling();
                    continue;
                }

                if (c.getNodeName().equals("type")) {
                    RangeField typeNode = new RangeField(c, xq);
                    params = typeNode.toRasQL();
                } else {
                    child = new CoverageExpr(c, xq);
                }

                c = c.getNextSibling();
            }
        } else if (nodeName.equals("fieldSelect")) {
            operation = "select";
            Node c = node.getFirstChild();

            while (c != null) {
                if (c.getNodeName().equals("#text")) {
                    c = c.getNextSibling();
                    continue;
                }

                if (c.getNodeName().equals("field")) {
                    FieldName nameNode = new FieldName(c.getFirstChild(), xq);
                    params = nameNode.toRasQL();
                } else {
                    child = new CoverageExpr(c, xq);
                }

                c = c.getNextSibling();
            }
        } else {
            throw new WCPSException("Unknown unary operation: " + nodeName);
        }

        info = new CoverageInfo(child.getCoverageInfo());
    }

    public CoverageInfo getCoverageInfo() {
        return info;
    }

    public String toRasQL() {
        if (operation.equals("sqrt") || operation.equals("abs") || operation.equals("exp")
                || operation.equals("log") || operation.equals("ln") || operation.equals("sin")
                || operation.equals("cos") || operation.equals("tan")
                || operation.equals("sinh") || operation.equals("cosh")
                || operation.equals("tanh") || operation.equals("arcsin")
                || operation.equals("arccos") || operation.equals("arctan")
                || operation.equals("not") || operation.equals("+") || operation.equals("-")) {
            return operation + "(" + child.toRasQL() + ")";
        } else if (operation.equals("cast")) {
            return "(" + params + ")(" + child.toRasQL() + ")";
        } else if (operation.equals("select")) {
            return "(" + child.toRasQL() + ")." + params;
        } else if (operation.equals("bit")) {
            return "bit(" + child.toRasQL() + "," + params + ")";
        }

        return " error ";
    }
}
