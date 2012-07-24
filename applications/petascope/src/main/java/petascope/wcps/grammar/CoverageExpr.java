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

import petascope.util.WCPSConstants;


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
        function = WCPSConstants.CHILD;
    }

    public CoverageExpr(String n) {
        coverageName = n;
        function = WCPSConstants.COVERAGE;
    }

    /* Unary Induced Expressions */
    public CoverageExpr(String op, CoverageExpr ce) {
        expr = ce;
        function = WCPSConstants.UNIARY_OP;
        this.op = op;
    }

    public CoverageExpr(String op, CoverageExpr e1, CoverageExpr e2) {
        function = WCPSConstants.BINARY_OP;
        this.op = op;
        this.e1 = e1;
        this.e2 = e2;
    }

    @Override
    public String toXML() {
        String result = "";

        if (function.equals(WCPSConstants.COVERAGE)) {
            result = "<" + WCPSConstants.COVERAGE + ">" + coverageName + "</" + 
                    WCPSConstants.COVERAGE + ">";
        } else if (function.equals(WCPSConstants.BINARY_OP)) {
            formatOperation();
            result = "<" + op + ">" + e1.toXML() + e2.toXML() + "</" + op + ">";
        } else if (function.equals(WCPSConstants.UNIARY_OP)) {
            formatOperation();
            if (op.equals(WCPSConstants.PLUS_S)) {
                op = WCPSConstants.UNIARY_PLUS;
            }
            if (op.equals(WCPSConstants.MINUS)) {
                op = WCPSConstants.UNIARY_MINUS;
            }
            result = "<" + op + ">" + expr.toXML() + "</" + op + ">";
        } else if (function.equals(WCPSConstants.CHILD)) {
            result = expr.toXML();
        }

        return result;
    }

    private void formatOperation() {
        if (op.equals(WCPSConstants.PLUS)) {
            op = WCPSConstants.PLUS_S;
        }
        if (op.equals(WCPSConstants.MINUS)) {
            op = WCPSConstants.MINUS_S;
        }
        if (op.equals(WCPSConstants.STAR)) {
            op = WCPSConstants.MULT;
        }
        if (op.equals(WCPSConstants.DIV)) {
            op = WCPSConstants.DIV_S;
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
