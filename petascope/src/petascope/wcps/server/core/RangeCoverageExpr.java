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
package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.WCPSException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.w3c.dom.*;

public class RangeCoverageExpr implements IRasNode, ICoverageInfo {

    private IRasNode child;
    private CoverageInfo info = null;
    List<IRasNode> components;

    public RangeCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException {

        components = new ArrayList<IRasNode>();

        if (node.getNodeName().equals("rangeConstructor"))
            node = node.getFirstChild();

        if (node.getNodeName().equals("#text"))
                node = node.getNextSibling();

        String nodeName = node.getNodeName();
        System.err.println("Trying to parse a range coverage expression... Starting at node "
                + nodeName);

        while (node != null) {
            if (node.getNodeName().equals("#text")) {
                node = node.getNextSibling();
                continue;
            }
            if (node.getNodeName().equals("component")) {
                RangeComponent elem = new RangeComponent(node, xq);
                info = elem.getCoverageInfo();
                components.add(elem);
            }

            node = node.getNextSibling();
        }

    }

    public CoverageInfo getCoverageInfo() {
        // FIXME: Returns currently only the info for the last range component
        return info;
    }

    public String toRasQL() {
        String result = "";
        int length = components.size();
        for (int i = 0; i < length - 1; i++)
            result += components.get(i).toRasQL() + ",";
        result += components.get(length - 1).toRasQL();

        result = "{ " + result + " }";

        return result;
    }
}
