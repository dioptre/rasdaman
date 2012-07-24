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
import petascope.util.WCPSConstants;

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
        log.trace(WCPSConstants.NUMERIC_SCALAR_EXPR_CONDENSE);
        condense = c;
        function = WCPSConstants.CONDENSE;
    }

    public NumericScalarExpr(String val) {
        log.trace(WCPSConstants.NUMERIC_SCALAR_EXPR + " " + val);
        if (val.contains(WCPSConstants.PLUS_I)) {
            ComplexConst cc = new ComplexConst(val);

            constValue = cc.toXML();
            function = WCPSConstants.COMPLEX_CONSTANT;
        } else {
            constValue = val;
            function = WCPSConstants.NUMERIC_CONSTANT;
        }
    }

    public NumericScalarExpr(String op, NumericScalarExpr expr) {
        log.trace(WCPSConstants.NUMERIC_SCALAR_EXPR + " " + op + " "+ WCPSConstants.NUM);
        leftNumericScalarExpr = expr;

        if (op.equals("-")) {
            function = WCPSConstants.NUMERIC_UNARY_MINUS;
        } else
        if (op.equals(WCPSConstants.SQRT)) {
            function = WCPSConstants.NUMERIC_SQRT;
        } else
        if (op.equals(WCPSConstants.ABS)) {
            function = WCPSConstants.NUMERIC_ABS;
        } else {
            log.error(WCPSConstants.UNIARY_OPERATOR + " " + op + " " + WCPSConstants.IS_NOT_RECOGNIZED);
        }
    }

    public NumericScalarExpr(String varOp, String varName) {
        log.trace(WCPSConstants.NUMERIC_SCALAR_EXPR + " " + varOp + ", " + varName);
        if (varOp.equals(WCPSConstants.VAR)) {
            function = WCPSConstants.VARIABLE_REF;
            constValue = varName;
        } else {
            log.error(WCPSConstants.INTERNAL_ERROR_THIS + ":" + varName);
        }
    }

    public NumericScalarExpr(String op, NumericScalarExpr lbe, NumericScalarExpr rbe) {
        log.trace(WCPSConstants.NUMERIC_SCALAR_EXPR + " " + WCPSConstants.A + " " + op + " " + WCPSConstants.B);
        leftNumericScalarExpr = lbe;
        rightNumericScalarExpr = rbe;

        if (op.equals(WCPSConstants.PLUS)) {
            function = WCPSConstants.NUMERIC_ADD;
        } else if (op.equals(WCPSConstants.MINUS)) {
            function = WCPSConstants.NUMERIC_MINUS;
        } else if (op.equals(WCPSConstants.MULT)) {
            function = WCPSConstants.NUMERIC_MULT;
        } else if (op.equals(WCPSConstants.DIV)) {
            function = WCPSConstants.NUMERIC_DIV;
        } else {
            log.error(WCPSConstants.OPERATOR + " " + op + " " + WCPSConstants.IS_NOT_RECOGNIZED);
        }
    }

    public String toXML() {
        String result;

        if (function.equals(WCPSConstants.COMPLEX_CONSTANT)) {
            return constValue;
        }
        if (function.equals(WCPSConstants.CONDENSE)) {
            return condense.toXML();
        }

        result = "<" + function + ">";

        if (function.equals(WCPSConstants.NUMERIC_CONSTANT) || function.equals(WCPSConstants.VARIABLE_REF)) {
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
