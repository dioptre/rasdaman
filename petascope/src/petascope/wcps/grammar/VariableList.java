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
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.grammar;

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
        String result = "<axisIterator><axisType>" + axisType + "</axisType>"
                + "<iteratorVar>" + iteratorName + "</iteratorVar>" + "<coord>"
                + lo + "</coord>" + "<coord>" + hi + "</coord></axisIterator>";

        if (next != null) {
            result += next.toXML();
        }

        return result;
    }
}
