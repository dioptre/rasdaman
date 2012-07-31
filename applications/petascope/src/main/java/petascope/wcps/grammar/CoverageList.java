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
 * CoverageList class represents a CoverageList.
 * Creation date: (3/3/2003 2:52:55 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class CoverageList implements IParseTreeNode {

    private String coverageName;
    private CoverageList next;

    public CoverageList(String c) {
        coverageName = c;
        next = null;
    }

    public CoverageList(String c, CoverageList l) {
        coverageName = c;
        next = l;
    }

    public String toXML() {
        String result = "<" + WCPSConstants.MSG_COVERAGE_NAME + ">" + coverageName + "</" 
                + WCPSConstants.MSG_COVERAGE_NAME +">";

        if (next != null) {
            result += next.toXML();
        }

        return result;
    }

    public String toString() {
        String result = coverageName;
        if (next != null) {
            result += next.toString();
        }

        return result;
    }
}
