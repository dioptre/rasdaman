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


package wcps.server.core;

import org.w3c.dom.*;

public class DimensionPointElement implements IRasNode
{
    private IRasNode domain, child;
    private AxisNameType axis;
    private CrsNameType crs;
    private boolean finished = false;
    private Node nextNode;

	public DimensionPointElement(Node node, ProcessCoveragesRequest pcr)
	    throws WCPSException
	{
        System.err.println("Trying to parse DimensionPointElement expression...");
        String name;

        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}

        while (node != null && finished == false)
        {
            if (node.getNodeName().equals("#text"))
			{
				node = node.getNextSibling();
				continue;
			}

            name = node.getNodeName();
            System.err.println("Current node is " + name);

            // Try Axis
            try
            {
                axis = new AxisNameType(node, pcr);
                node = node.getNextSibling();
                continue;
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to parse an axis!");
            }

            // Try CRS name
            try
            {
                crs = new CrsNameType(node, pcr);
                node = node.getNextSibling();
                if (axis == null)
                    throw new WCPSException("Expected Axis node before CRS !");
                continue;
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to parse a crs!");
            }

            // TODO: how to implement DomainMetadataExpr ?

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

            // Then it must be a pair of nodes "lowerBound" + "upperBound"
            if (node.getNodeName().equals("slicingPosition"))
            {
                domain = new ScalarExprType(node.getFirstChild(), pcr);
                if (axis == null)
                    throw new WCPSException("Expected <axis> node before <slicingPosition> !");
            }
            else
                throw new WCPSException("Unexpected node: " + node.getFirstChild().getNodeName());

            if (axis != null && domain != null)
                finished = true;

            if (finished == true)
                nextNode = node.getNextSibling();

            node = node.getNextSibling();
        }
	}

	public String toRasQL()
	{
		return child.toRasQL();
	}
    
    public Node getNextNode()
    {
        return nextNode;
    }

    public String getAxisName()
    {
        return this.axis.toRasQL();
    }

    public String getCrsName()
    {
        return this.crs.toRasQL();
    }

    public String getSlicingPosition()
    {
        return this.domain.toRasQL();
    }
}
