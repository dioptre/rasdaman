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
 * CrsTransformExpr
 *
 * @author Andrei Aiordachioaie
 */
public class CrsTransformExpr implements IParseTreeNode {

    IParseTreeNode e1, e2, e3;

    public CrsTransformExpr(CoverageExpr expr, DimensionIntervalList list1,
            FieldInterpolationList list2) {
        e1 = expr;
        e2 = list1;
        e3 = list2;
    }

    public String toXML() {
        String result = "";

        result += e1.toXML();
        result += e2.toXML();
        result += e3.toXML();

        result = "<crsTransform>" + result + "</crsTransform>";
        return result;
    }
}
