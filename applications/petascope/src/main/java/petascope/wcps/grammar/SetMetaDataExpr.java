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
 * SetMetaDataExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class SetMetaDataExpr implements IParseTreeNode {
    
    private static Logger log = LoggerFactory.getLogger(SetMetaDataExpr.class);

    CoverageExpr expr;
    String field;
    String function;
    IParseTreeNode param;

    // Set Crs Set
    public SetMetaDataExpr(String op, CoverageExpr expr, CrsList clist) {
        function = op;
        this.expr = expr;
        this.param = clist;
    }

    // Set Null Set
    public SetMetaDataExpr(String op, CoverageExpr expr, RangeExprList param) {
        function = op;
        this.expr = expr;
        this.param = param;
    }

    // Set Identifier
    public SetMetaDataExpr(String op, CoverageExpr expr, String constant) {
        function = op;
        this.expr = expr;
        this.field = constant;
    }

    // Set Interpolation Default
    public SetMetaDataExpr(String op, CoverageExpr expr, InterpolationMethod param,
            String field) {
        function = op;
        this.expr = expr;
        this.param = param;
        this.field = field;
    }

    // Set Interpolation Set
    public SetMetaDataExpr(String op, CoverageExpr expr, InterpolationMethodList param,
            String field) {
        function = op;
        this.expr = expr;
        this.param = param;
        this.field = field;
    }

    public String toXML() {
        String result = "";

        if (function.equalsIgnoreCase("setIdentifier")) {
            result += "<identifier>" + field + "</identifier>";
            result += expr.toXML();

            result = "<setIdentifier>" + result + "</setIdentifier>";
        } else if (function.equalsIgnoreCase("setCrsSet")) {
            result += expr.toXML();

            if (param != null) {
                result += param.toXML();
            }

            result = "<setCrsSet>" + result + "</setCrsSet>";
        } else if (function.equalsIgnoreCase("setNullSet")) {
            result += expr.toXML();

            if (param != null) {
                result += param.toXML();
            }

            result = "<setNullSet>" + result + "</setNullSet>";
        } else if (function.equalsIgnoreCase("setInterpolationDefault")) {
            result += expr.toXML();
            result += "<field>" + field + "</field>";
            result += param.toXML();

            result = "<setInterpolationDefault>" + result
                    + "</setInterpolationDefault>";
        } else if (function.equalsIgnoreCase("setInterpolationSet")) {
            result += expr.toXML();
            result += "<field>" + field + "</field>";
            result += param.toXML();

            result = "<setInterpolationSet>" + result + "</setInterpolationSet>";
        } else {
            log.error("Unknown SetMetadataExpr operation: " + function);
        }

        return result;
    }
}
