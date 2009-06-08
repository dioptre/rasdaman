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

public class ScalarExpr implements IRasNode
{
    private IRasNode child;

	public ScalarExpr(Node node, XmlQuery xq) throws WCPSException
	{
        while ((node != null) && node.getNodeName().equals("#text"))
        {
            node = node.getNextSibling();
        }

        // Try one of the groups
        child = null;

//      TODO: Implement class MetadataScalarExprType
//      MetadataScalarExprType
        if (child == null)
        {
            try
            {
                child = new MetadataScalarExpr(node, xq);
                System.err.println("Matched metadata scalar expression.");
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to match metadata scalar expression: "
                           + e.toString() + "\nRetrying");
                child = null;
            }
        }

//            BooleanScalarExprType
        if (child == null)
        {
            try
            {
                child = new BooleanScalarExpr(node, xq);
                System.err.println("Matched boolean scalar expression.");
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to match boolean scalar expression : "
                           + e.toString() + "\nRetrying");
                child = null;
            }
        }

//            NumericScalarExprType
        if (child == null)
        {
            try
            {
                child = new NumericScalarExpr(node, xq);
                System.err.println("Matched numeric scalar expression.");
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to match numeric scalar expression : "
                           + e.toString() + "\nRetrying");
                child = null;
            }
        }

//            ReduceScalarExprType
        if (child == null)
        {
            try
            {
                child = new ReduceScalarExpr(node, xq);
                System.err.println("Matched reduce scalar expression.");
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to match reduce scalar expression: "
                           + e.toString() + "\nRetrying");
                child = null;
            }
        }

//            StringScalarExprType
        if (child == null)
        {
            try
            {
                child = new StringScalarExpr(node, xq);
                System.err.println("Matched string scalar expression.");
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to match string scalar expression: "
                           + e.toString() + "\nRetrying");
                child = null;
            }
        }

        // Error check
		if (child == null)
		{
			throw new WCPSException("Invalid coverage Expression, next node: "
						+ node.getNodeName());
		}
	}

	public String toRasQL()
	{
        return child.toRasQL();
	}
}
