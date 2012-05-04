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
 * AxisIteratorList
 *
 * @author Andrei Aiordachioaie
 */
public class AxisIteratorList implements IParseTreeNode {

    private AxisIterator it;
    private AxisIteratorList next;
    private String tag;

    public AxisIteratorList(AxisIterator it) {
        this.it = it;
        next = null;
        tag = "";
    }

    public AxisIteratorList(AxisIterator it, AxisIteratorList n) {
        this.it = it;
        next = n;
        tag = "";
    }

    public void setTag(String tag) {
        this.tag = tag;
    }

    public String toXML() {
        String result = "";
        String tag1 = "<" + tag + ">";
        String tag2 = "</" + tag + ">";

        if (tag.equals("")) {
            tag1 = tag2 = "";
        }

        if (next != null) {
            next.setTag(tag);
            result += next.toXML();
        }
        result += tag1 + it.toXML() + tag2;

        return result;
    }
}
