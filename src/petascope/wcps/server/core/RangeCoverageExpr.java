/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
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

        // TODO: Implement RangeCoverageExpr
        throw new WCPSException("Feature not yet implemented !");

        /*
        components = new ArrayList<IRasNode>();

        String nodeName = node.getNodeName();
        System.err.println("Trying to parse a range coverage expression... Starting at node "
        + nodeName);

        Node it = node.getFirstChild();

        while (it != null)
        {
        if (it.getNodeName().equals("#text"))
        {
        it = it.getNextSibling();
        continue;
        }
        if (it.getNodeName().equals("component"))
        {
        RangeElementType elem = new RangeElementType(it, pcr);
        components.add(elem);
        }

        it = it.getNextSibling();
        }
         */

    }

    public CoverageInfo getCoverageInfo() {
        return info;
    }

    public String toRasQL() {
        String result = "";
        /*
        Iterator<IRasNode> i = components.iterator();
        while (i.hasNext())
        {
        result += i.next().toRasQL();
        }
         */

        return result;
    }
}
