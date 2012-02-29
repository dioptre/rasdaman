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
package petascope.wcps.grammar;


/**
 * CoverageExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class CoverageExpr implements IParseTreeNode {

    String coverageName;
    IParseTreeNode expr, e1, e2;
    String function;
    String op;

    public CoverageExpr(IParseTreeNode n) {
        expr = n;
        function = "child";
    }

    public CoverageExpr(String n) {
        coverageName = n;
        function = "coverage";
    }

    /* Unary Induced Expressions */
    public CoverageExpr(String op, CoverageExpr ce) {
        expr = ce;
        function = "unaryOp";
        this.op = op;
    }

    public CoverageExpr(String op, CoverageExpr e1, CoverageExpr e2) {
        function = "binaryOp";
        this.op = op;
        this.e1 = e1;
        this.e2 = e2;
    }

    @Override
    public String toXML() {
        String result = "";

        if (function.equals("coverage")) {
            result = "<coverage>" + coverageName + "</coverage>";
        } else if (function.equals("binaryOp")) {
            formatOperation();
            result = "<" + op + ">" + e1.toXML() + e2.toXML() + "</" + op + ">";
        } else if (function.equals("unaryOp")) {
            formatOperation();
            if (op.equals("plus")) {
                op = "unaryPlus";
            }
            if (op.equals("minus")) {
                op = "unaryMinus";
            }
            result = "<" + op + ">" + expr.toXML() + "</" + op + ">";
        } else if (function.equals("child")) {
            result = expr.toXML();
        }

        return result;
    }

    private void formatOperation() {
        if (op.equals("+")) {
            op = "plus";
        }
        if (op.equals("-")) {
            op = "minus";
        }
        if (op.equals("*")) {
            op = "mult";
        }
        if (op.equals("/")) {
            op = "div";
        }

        // AND, OR, XOR stay the same

        if (op.equals("=")) {
            op = "equals";
        }
        if (op.equals("<")) {
            op = "lessThan";
        }
        if (op.equals(">")) {
            op = "greaterThan";
        }
        if (op.equals("<=")) {
            op = "lessOrEqual";
        }
        if (op.equals(">=")) {
            op = "greaterOrEqual";
        }
        if (op.equals("!=")) {
            op = "notEqual";
        }

        // OVERLAY stays the same
    }
}
