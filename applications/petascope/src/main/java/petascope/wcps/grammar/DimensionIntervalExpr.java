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
 * DimensionIntervalExpr
 *
 * @author Andrei Aiordachioaie
 */
public class DimensionIntervalExpr implements IParseTreeNode {

    IParseTreeNode e1, e2;
    String fun;
    String str;

    public DimensionIntervalExpr(ScalarExpr exp1, ScalarExpr exp2) {
        fun = WCPSConstants.SCALARS;
        e1 = exp1;
        e2 = exp2;
    }

    public DimensionIntervalExpr(String cov, String axis, String crs) {
        fun = WCPSConstants.DOMAIN_METADATA;
        str = "<" + WCPSConstants.COVERAGE + ">" + cov + "</" + WCPSConstants.COVERAGE + ">";
        str += "<" + WCPSConstants.AXIS + ">" + axis + "</" + WCPSConstants.AXIS + ">";
        str += "<" + WCPSConstants.CRS + ">" + crs + "</" + WCPSConstants.CRS + ">";
    }

    public String toXML() {
        String result = "";

        if (fun.equals(WCPSConstants.SCALARS)) {
            result += "<" + WCPSConstants.LOWER_BOUND + ">" + e1.toXML() + "</" + 
                    WCPSConstants.LOWER_BOUND + ">";
            result += "<" + WCPSConstants.UPPER_BOUND + ">" + e2.toXML() + "</" + 
                    WCPSConstants.UPPER_BOUND + ">";
        } else if (fun.equals(WCPSConstants.DOMAIN_METADATA)) {
            result = str;
        }

        return result;
    }
}
