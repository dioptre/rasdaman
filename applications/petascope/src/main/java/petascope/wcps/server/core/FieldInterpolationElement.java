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
import org.w3c.dom.*;
import petascope.util.WCPSConstants;

public class FieldInterpolationElement implements IRasNode {

    private FieldName name;
    private InterpolationMethod interp;
    private Node nextNode;

    public FieldInterpolationElement(Node node, XmlQuery xq) throws WCPSException {

        throw new WCPSException(WCPSConstants.ERRTXT_NO_INTERPOLATION);

        /*
        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        if (node == null) {
            throw new WCPSException("FieldInterpolationElementType parsing error!");
        }

        while (node != null)
        {
            String nodeName = node.getNodeName();
            while (node.getNodeName().equals("#text"))
                node = node.getNextSibling();

            if (nodeName.equals("name")) {
                this.name = new FieldName(node, xq);
                node = node.getNextSibling();
                continue;
            }
            else
            {
                interp = new InterpolationMethod(node, xq);
                node = interp.getNextNode();
            }
        }
        nextNode = node;

         */
    }

    public Node getNextNode()
    {
        return nextNode;
    }

    public String toRasQL() {
        // Since we only accept the default rasdaman interpolation methods, we do not need to specify it further
        return "";
    }
};
