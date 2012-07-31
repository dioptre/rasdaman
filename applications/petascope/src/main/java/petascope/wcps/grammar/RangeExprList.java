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

import java.util.*;
import petascope.util.WCPSConstants;

/**
 * RangeExprList
 *
 * @author Andrei Aiordachioaie
 */
public class RangeExprList implements IParseTreeNode {

    LinkedList<RangeExpr> list;

    public RangeExprList() {
        list = new LinkedList();
    }

    public void add(RangeExpr expr) {
        list.add(expr);
    }

    public String toXML() {
        String result = "";

        Iterator<RangeExpr> it = list.iterator();

        while (it.hasNext()) {
            result += "<" + WCPSConstants.MSG_NULL + ">" + it.next().toXML() + "</" + WCPSConstants.MSG_NULL + ">";
        }

        return result;
    }
}
