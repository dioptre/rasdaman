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

public class DomainMetadataExpr implements IRasNode, ICoverageInfo {

    private IRasNode child;
    private CoverageInfo info = null;
    private AxisName axis;
    private Crs crs;
    private IRasNode domain1, domain2;  // lower and upper bound, or "DomainMetadataExprType" and null
    private int counter = 0;             // counter for the domain vars

    public DomainMetadataExpr(Node node, XmlQuery xq)
            throws WCPSException {
        /*
        while ((node != null) && node.getNodeName().equals("#text"))
        {
        node = node.getNextSibling();
        }

        if (node == null)
        {
        throw new WCPSException("SubsetOperationCoverageExpr parsing error!");
        }

        String nodeName = node.getNodeName();

        System.err.println("SubsetOperationCoverageExpr: node " + nodeName);

        if (nodeName.equals("trim"))
        {
        child = new TrimCoverageExprType(node, pcr);
        }
        else if (nodeName.equals("extend"))
        child = new ExtendCoverageExprType(node, pcr);
        else if (nodeName.equals("slice"))
        child = new SliceCoverageExprType(node, pcr);
         * */

        System.err.println("Trying to parse DimensionIntervalElement expression...");

        throw new WCPSException("Method not implemented");

//        while ((node != null) && node.getNodeName().equals("#text"))
//		{
//			node = node.getNextSibling();
//		}
//
//        while (node != null)
//        {
//            if (node.getNodeName().equals("#text"))
//			{
//				node = node.getNextSibling();
//				continue;
//			}
//
//            String name = node.getNodeName();
//            System.err.println("Current node is " + name);
//
//            // Try Axis
//            try
//            {
//                axis = new AxisNameType(node, pcr);
//                continue;
//            }
//            catch (WCPSException e)
//            {
//                System.err.println("Failed to parse an axis!");
//            }
//
//            // Try CRS name
//            try
//            {
//                crs = new CrsNameType(node, pcr);
//                continue;
//            }
//            catch (WCPSException e)
//            {
//                System.err.println("Failed to parse a crs!");
//            }
//
//            // Try last thing
//            try
//            {
//                domain1 = new DomainMetadataExprType(node, pcr);
//                counter = 1;
//                continue;
//            }
//            catch (WCPSException e)
//            {
//                System.err.println("Failed to parse domain metadata!");
//            }
//
//            // Then it must be a pair of nodes "lowerBound" + "upperBound"
//            if (node.getNodeName().equals("lowerBound"))
//            {
//                counter ++;
//                if (counter == 1)
//                    domain1 = new ScalarExprType(node, pcr);
//                else
//                    throw new WCPSException("Too many *Bounds nodes !!!");
//            }
//            else
//            if (node.getNodeName().equals("upperBound"))
//            {
//                counter ++;
//                if (counter == 2)
//                    domain2 = new ScalarExprType(node, pcr);
//                else
//                    throw new WCPSException("Too many *Bounds nodes !!!");
//            }
//            else
//                throw new WCPSException("Unexpected node: " + node.getNodeName());
//        }
    }

    public String toRasQL() {
        return child.toRasQL();
    }

    public CoverageInfo getCoverageInfo() {
        return info;
    }
}
