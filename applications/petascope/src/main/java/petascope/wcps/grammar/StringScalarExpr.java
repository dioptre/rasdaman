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
 * @author: Andrei Aiordachioaie
 */
public class StringScalarExpr implements IParseTreeNode {

    CoverageExpr cov;
    String function;
    String op;
    String val;

    public StringScalarExpr(String val) {
        this.val = val;
        this.function = WCPSConstants.MSG_STING_CONSTANT;
    }

    public StringScalarExpr(String op, CoverageExpr cov) {
        this.cov = cov;
        function = WCPSConstants.MSG_STING_IDENTIFIER;
        this.op = op;
    }

    public String toXML() {
        String result = "<" + function + ">";

        if (function.equals(WCPSConstants.MSG_STING_CONSTANT)) {
            result += val;
        } else if (function.equals(WCPSConstants.MSG_STING_IDENTIFIER)) {
            result += cov.toXML();
        }

        result += "</" + function + ">";

        return result;
    }
}
