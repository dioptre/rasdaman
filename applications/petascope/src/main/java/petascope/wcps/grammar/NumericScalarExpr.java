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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * NumericScalarExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class NumericScalarExpr implements IParseTreeNode {
    
    private static Logger log = LoggerFactory.getLogger(NumericScalarExpr.class);

    CondenseExpr condense;
    String constValue;
    String function;
    NumericScalarExpr leftNumericScalarExpr, rightNumericScalarExpr;

    public NumericScalarExpr(CondenseExpr c) {
        log.trace("NumericScalarExpr condense");
        condense = c;
        function = "condense";
    }

    public NumericScalarExpr(String val) {
        log.trace("NumericScalarExpr " + val);
        if (val.contains("+i")) {
            ComplexConst cc = new ComplexConst(val);

            constValue = cc.toXML();
            function = "complexConstant";
        } else {
            constValue = val;
            function = "numericConstant";
        }
    }

    public NumericScalarExpr(String op, NumericScalarExpr expr) {
        log.trace("NumericScalarExpr " + op + " num");
        leftNumericScalarExpr = expr;

        if (op.equals("-")) {
            function = "numericUnaryMinus";
        } else
        if (op.equals("sqrt")) {
            function = "numericSqrt";
        } else
        if (op.equals("abs")) {
            function = "numericAbs";
        } else {
            log.error("Unary Operator " + op + " is not recognized!");
        }
    }

    public NumericScalarExpr(String varOp, String varName) {
        log.trace("NumericScalarExpr " + varOp + ", " + varName);
        if (varOp.equals("var")) {
            function = "variableRef";
            constValue = varName;
        } else {
            log.error("Internal error: this should have been a variable name:" + varName);
        }
    }

    public NumericScalarExpr(String op, NumericScalarExpr lbe, NumericScalarExpr rbe) {
        log.trace("NumericScalarExpr a " + op + " b");
        leftNumericScalarExpr = lbe;
        rightNumericScalarExpr = rbe;

        if (op.equals("+")) {
            function = "numericAdd";
        } else if (op.equals("-")) {
            function = "numericMinus";
        } else if (op.equals("*")) {
            function = "numericMult";
        } else if (op.equals("/")) {
            function = "numericDiv";
        } else {
            log.error("Operator " + op + " is not recognized!");
        }
    }

    public String toXML() {
        String result;

        if (function.equals("complexConstant")) {
            return constValue;
        }
        if (function.equals("condense")) {
            return condense.toXML();
        }

        result = "<" + function + ">";

        if (function.equals("numericConstant") || function.equals("variableRef")) {
            result += constValue;
        } else {
            result += leftNumericScalarExpr.toXML();

            if (rightNumericScalarExpr != null) {
                result += rightNumericScalarExpr.toXML();
            }
        }

        result += "</" + function + ">";

        return result;
    }
}
