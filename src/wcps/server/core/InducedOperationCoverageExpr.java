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

public class InducedOperationCoverageExpr implements IRasNode, ICoverageInfo
{
	private IRasNode child;
	private CoverageInfo info;
	private String operation = "";

	public InducedOperationCoverageExpr(Node node, XmlQuery xq)
	    throws WCPSException
	{
		String nodeName = node.getNodeName();

		System.err.println("Trying induced operation: " + nodeName);
		boolean okay = false;    // will be true if the node is recognized

        if (nodeName.equals("rangeConstructor"))
		{
            operation = nodeName;
			child = new RangeCoverageExpr(node, xq);
            info = new CoverageInfo((((ICoverageInfo)child).getCoverageInfo()));
		}
		else
		{    // Try one of the groups
            child = null;

            if (child == null)
			{
				try
				{
					child = new UnaryOperationCoverageExpr(node, xq);
                    info = new CoverageInfo((((ICoverageInfo)child).getCoverageInfo()));
					System.err.println("*** Induced Operation SUCCESS: " + node.getNodeName());
				}
				catch (WCPSException e)
				{
					System.err.println("InducedOperation failed: " + node.getNodeName());
					child = null;
                    if (e.getMessage().equals("Method not implemented"))
                        throw e;
				}
			}

            if (child == null)
            {
                try
                {
                    child = new BinaryOperationCoverageExpr(node, xq);
                    info = new CoverageInfo((((ICoverageInfo)child).getCoverageInfo()));
                    System.err.println("*** Binary Operation SUCCESS: " + node.getNodeName());
                }
                catch (WCPSException e)
                {
                    System.err.println("Binary operation failed: " + node.getNodeName()) ;
                    child = null;
                }
            }
            
            if (child == null)
            {
                throw new WCPSException("Invalid induced coverage expression, next node: "
                            + node.getNodeName());
            }
		}

	}

	public CoverageInfo getCoverageInfo()
	{
		return info;
	}

	public String toRasQL()
	{
        if (operation.equals("rangeConstructor"))
            return "<rangeConstructor>" + child.toRasQL() + "</rangeConstructor>";
        // else: groups
		return child.toRasQL();
	}
}
