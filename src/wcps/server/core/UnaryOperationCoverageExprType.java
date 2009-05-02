/*
 * This file is part of Petascope.
 *
 * Petascope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Petascope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Petascope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.Petascope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package wcps.server.core;

import org.w3c.dom.*;

public class UnaryOperationCoverageExprType implements IRasNode, ICoverageInfo {

    private String operation;
    private CoverageExprType child;
    private String params;
    private CoverageInfo info;
    
    public UnaryOperationCoverageExprType(Node node, ProcessCoverageRequest pcr) throws WCPSException {
        String nodeName = node.getNodeName();
        System.err.println("Trying to parse unary operation: " + nodeName);
        if (nodeName.equals("unaryPlus")) {
            operation = "+";
            child = new CoverageExprType(node.getFirstChild(), pcr);
        } else if (nodeName.equals("unaryMinus")) {
            operation = "-";            
            child = new CoverageExprType(node.getFirstChild(), pcr);
        } else if (nodeName.equals("sqrt") || nodeName.equals("abs") || nodeName.equals("exp") || nodeName.equals("log") ||
               nodeName.equals("ln") || nodeName.equals("sin") || nodeName.equals("cos") || nodeName.equals("tan") ||
               nodeName.equals("sinh") || nodeName.equals("cosh") || nodeName.equals("tanh") || nodeName.equals("arcsin") ||
               nodeName.equals("arccos") || nodeName.equals("arctan") || nodeName.equals("not")) {
            operation = nodeName;
            child = new CoverageExprType(node.getFirstChild(), pcr);        
        } else if (nodeName.equals("bit")) {
            operation = "bit";
            Node c = node.getFirstChild();
            while (c != null) {
                if (c.getNodeName().equals("#text")) {
                    c = c.getNextSibling();
                    continue;       
                }
                if (c.getNodeName().equals("position")) {
                    params = c.getFirstChild().getNodeValue();
                } else {
                    child = new CoverageExprType(c, pcr);
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
                    params = c.getFirstChild().getNodeValue();
                } else {
                    child = new CoverageExprType(c, pcr);
                }       
                c = c.getNextSibling();
            }
        } else if (nodeName.equals("select")) {
            operation = "select";
            Node c = node.getFirstChild();
            while (c != null) {
                if (c.getNodeName().equals("#text")) {
                    c = c.getNextSibling();
                    continue;
                }
                if (c.getNodeName().equals("field")) {
                    params = c.getFirstChild().getNodeValue();
                } else {
                    child = new CoverageExprType(c, pcr);
                }       
                c = c.getNextSibling();
            }
        } else throw new WCPSException("Unknown unary operation: " + nodeName);
        info = new CoverageInfo(child.getCoverageInfo());
    }

    public CoverageInfo getCoverageInfo() {
    return info;
    }

    public String toRasQL() {
        if (operation.equals("sqrt") || operation.equals("abs") || operation.equals("exp") || operation.equals("log") ||
            operation.equals("ln") || operation.equals("sin") || operation.equals("cos") || operation.equals("tan") ||
            operation.equals("sinh") || operation.equals("cosh") || operation.equals("tanh") || operation.equals("arcsin") ||
            operation.equals("arccos") || operation.equals("arctan") || operation.equals("not") || operation.equals("+") || operation.equals("-")) {
            return operation + "(" + child.toRasQL() + ")";
        } else if (operation.equals("cast")) {
            return "(" + params + ")(" + child.toRasQL() + ")";
        } else if (operation.equals("select")) {
            return "(" + child.toRasQL() + ")." + params;
        } else if (operation.equals("bit")) {
            return "bit(" + child.toRasQL() + "," + params +")";
        }
        return  " error ";
    }

}
