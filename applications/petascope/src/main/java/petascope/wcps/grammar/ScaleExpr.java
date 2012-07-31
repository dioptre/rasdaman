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
 * ScaleExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class ScaleExpr implements IParseTreeNode {

    CoverageExpr coverageExpr;
    FieldInterpolationList interpMethods;
    DimensionIntervalList intervalList;

    public ScaleExpr(CoverageExpr ce, DimensionIntervalList ail)
    {
        coverageExpr = ce;
        intervalList = ail;
    }

    public void addInterpolationList(FieldInterpolationList iml)
    {
        interpMethods = iml;
    }

    public String toXML() {
        String result = "";

        result += coverageExpr.toXML() + intervalList.toXML();
        if (interpMethods != null)
            result += interpMethods.toXML();
        
        result = "<" + WCPSConstants.MSG_SCALE + ">" + result + "</" + WCPSConstants.MSG_SCALE + ">";

        return result;
    }
}
