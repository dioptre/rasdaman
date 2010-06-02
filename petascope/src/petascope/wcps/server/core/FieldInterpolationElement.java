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
import org.w3c.dom.*;
import petascope.wcps.server.exceptions.InvalidWcpsRequestException;

public class FieldInterpolationElement implements IRasNode {

    private FieldName name;
    private InterpolationMethod interp;
    private Node nextNode;

    public FieldInterpolationElement(Node node, XmlQuery xq) throws WCPSException {

        throw new InvalidWcpsRequestException("Currently, our implementation does not allow specifying " +
                "interpolation methods or null resistances for the scale operation.");

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
