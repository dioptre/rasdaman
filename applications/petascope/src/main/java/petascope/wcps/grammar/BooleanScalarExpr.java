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
 * BooleanScalarExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class BooleanScalarExpr implements IParseTreeNode {

    String booleanConstant;
    IParseTreeNode left, right;
    BooleanScalarExpr leftBooleanScalarExpr, rightBooleanScalarExpr;
    NumericScalarExpr leftNumericScalar, rightNumericScalar;
    String node1, node2;
    String op;

    public BooleanScalarExpr(String bc) {
        op = null;
        booleanConstant = bc;
    }

    public BooleanScalarExpr(String op, BooleanScalarExpr be) {
        this.op = op;
        left = be;
    }

    public BooleanScalarExpr(String op, BooleanScalarExpr lbe, BooleanScalarExpr rbe) {
        this.op = op;
        left = lbe;
        right = rbe;
    }

    public BooleanScalarExpr(String op, NumericScalarExpr left, NumericScalarExpr right) {
        this.op = op;
        this.left = left;
        this.right = right;
    }

    public BooleanScalarExpr(String op, StringScalarExpr e1, StringScalarExpr e2) {
        this.op = op;
        left = e1;
        right = e2;
    }

    public String toXML() {
        if (op == null) {
            return "<" + WCPSConstants.MSG_BOOLEAN_CONSTANT + ">" + booleanConstant + "</" +
                    WCPSConstants.MSG_BOOLEAN_CONSTANT + ">";
        } else if (op.equals(WCPSConstants.MSG_NOT)) {
            return "<" + WCPSConstants.MSG_BOOLEAN_NOT + ">" + left.toXML() + "</" + 
                    WCPSConstants.MSG_BOOLEAN_NOT + ">";
        } else if (op.equals(WCPSConstants.MSG_BIT)) {
            return "<" + WCPSConstants.MSG_BIT + ">" + left.toXML() + right.toXML() + 
                    "</" + WCPSConstants.MSG_BIT + ">";
        } else {
            if (this.left != null) {
                node1 = this.left.toXML();
            }

            if (this.right != null) {
                node2 = this.right.toXML();
            }

            if (op.equals(WCPSConstants.MSG_AND)) {
                op = WCPSConstants.MSG_BOOLEAN_AND;
            } else if (op.equals(WCPSConstants.MSG_OR)) {
                op = WCPSConstants.MSG_BOOLEAN_OR;
            } else if (op.equals(WCPSConstants.MSG_XOR)) {
                op = WCPSConstants.MSG_BOOLEAN_XOR;

            } else if (op.equals(WCPSConstants.MSG_EQUALS)) {
                op = WCPSConstants.MSG_BOOLEAN_EQUALNUMERIC;
            } else if (op.equals(WCPSConstants.MSG_NOT_EQUALS)) {
                op = WCPSConstants.MSG_BOOLEAN_NOTEQUALNUMERIC;
            } else if (op.equals(WCPSConstants.MSG_LESS_THAN)) {
                op = WCPSConstants.MSG_BOOLEAN_LESSTHAN;
            } else if (op.equals(WCPSConstants.MSG_GREATER_THAN)) {
                op = WCPSConstants.MSG_BOOLEAN_GREATERTHAN;
            } else if (op.equals(WCPSConstants.MSG_LESS_OR_EQUAL)) {
                op = WCPSConstants.MSG_BOOLEAN_LESSOREQUAL;
            } else if (op.equals(WCPSConstants.MSG_GREATER_OR_EQUAL)) {
                op = WCPSConstants.MSG_BOOLEAN_GREATEROREQUAL;
            }

            return "<" + op + ">" + node1 + node2 + "</" + op + ">";
        }

    }
}
