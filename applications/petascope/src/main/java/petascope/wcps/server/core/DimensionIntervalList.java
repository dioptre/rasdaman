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
package petascope.wcps.server.core;

import petascope.exceptions.WCPSException;
import java.util.ArrayList;
import java.util.List;
import org.w3c.dom.*;

public class DimensionIntervalList {

    private ArrayList<IRasNode> list;

    public DimensionIntervalList(Node node, XmlQuery xq, CoverageInfo info)
            throws WCPSException {

        while (node != null) {
            list = new ArrayList<IRasNode>();
            try {
                DimensionIntervalElement elem = new DimensionIntervalElement(node, xq, info);
                node = elem.getNextNode();
                list.add(elem);
            } catch (WCPSException e) {
            }
        }
    }

    public List getAxisList() {
        return list;
    }
}
