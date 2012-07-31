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

/*Author: Sorin Stancu-Mara, Andrei Aiordachioaie
 * Date: 8 Feb 2008
 */
public class VariableList implements IParseTreeNode {

    String axisType, iteratorName;
    String lo, hi;
    VariableList next;

    public VariableList(String type, String name, String lo, String hi) {
        axisType = type;
        iteratorName = name;
        this.lo = lo;
        this.hi = hi;
        next = null;
    }

    public VariableList(String type, String name, String lo, String hi, VariableList next) {
        axisType = type;
        iteratorName = name;
        this.lo = lo;
        this.hi = hi;
        this.next = next;
    }

    public String toXML() {
        String result = "<" + WCPSConstants.MSG_AXIS_ITERATOR + "><" + WCPSConstants.MSG_AXIS_TYPE 
                + ">" + axisType + "</" + WCPSConstants.MSG_AXIS_TYPE + ">"
                + "<" + WCPSConstants.MSG_ITERATORVAR + ">" + iteratorName + "</" + 
                WCPSConstants.MSG_ITERATORVAR + ">" + "<" + WCPSConstants.MSG_COORD + ">"
                + lo + "</" + WCPSConstants.MSG_COORD + ">" + "<" + WCPSConstants.MSG_COORD 
                + ">" + hi + "</" + WCPSConstants.MSG_COORD + "></" + WCPSConstants.MSG_AXIS_ITERATOR + ">";

        if (next != null) {
            result += next.toXML();
        }

        return result;
    }
}
