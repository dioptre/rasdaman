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

import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.WCPSException;
import org.w3c.dom.*;

public class DimensionPointElement implements IRasNode {

    private IRasNode domain, child;
    private AxisName axis;
    private Crs crs;
    private boolean finished = false;
    private Node nextNode;

    public DimensionPointElement(Node node, XmlQuery xq)
            throws WCPSException, InvalidCrsException {
        System.err.println("Trying to parse DimensionPointElement expression...");
        String name;

        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        while (node != null && finished == false) {
            if (node.getNodeName().equals("#text")) {
                node = node.getNextSibling();
                continue;
            }

            name = node.getNodeName();
            System.err.println("Current node is " + name);

            // Try Axis
            try {
                axis = new AxisName(node, xq);
                node = node.getNextSibling();
                continue;
            } catch (WCPSException e) {
                System.err.println("Failed to parse an axis!");
            }

            // Try CRS name
            try {
                crs = new Crs(node, xq);
                node = node.getNextSibling();
                if (axis == null) {
                    throw new WCPSException("Expected Axis node before CRS !");
                }
                continue;
            } catch (WCPSException e) {
                System.err.println("Failed to parse a crs!");
            }

            // TODO: how to implement DomainMetadataExpr ?

//            // Try last thing
//            try
//            {
//                domain1 = new DomainMetadataExprType(node, xq);
//                counter = 1;
//                continue;
//            }
//            catch (WCPSException e)
//            {
//                System.err.println("Failed to parse domain metadata!");
//            }

            // Then it must be a "slicingPosition"
            if (node.getNodeName().equals("slicingPosition")) {
                domain = new ScalarExpr(node.getFirstChild(), xq);
                if (axis == null) {
                    throw new WCPSException("Expected <axis> node before <slicingPosition> !");
                }
            } else {
                throw new WCPSException("Unexpected node: " + node.getFirstChild().getNodeName());
            }

            if (axis != null && domain != null) {
                finished = true;
            }

            if (finished == true) {
                nextNode = node.getNextSibling();
            }

            node = node.getNextSibling();
        }
    }

    public String toRasQL() {
        return child.toRasQL();
    }

    public Node getNextNode() {
        return nextNode;
    }

    public String getAxisName() {
        return this.axis.toRasQL();
    }

    public String getCrsName() {
        return this.crs.toRasQL();
    }

    public String getSlicingPosition() {
        return this.domain.toRasQL();
    }
}
