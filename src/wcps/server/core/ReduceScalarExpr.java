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

public class ReduceScalarExpr implements IRasNode
{
	CoverageExpr expr;
	String op;

	public ReduceScalarExpr(Node node, ProcessCoveragesRequest pcr) throws WCPSException
	{
        System.err.println("Trying to parse ReduceScalarExpr ");
		String nodeName = node.getNodeName();

		if (nodeName.equals("all") || nodeName.equals("some") || nodeName.equals("count")
		    || nodeName.equals("add") || nodeName.equals("avg") || nodeName.equals("min")
		    || nodeName.equals("max"))
		{
			op = nodeName;

			if (!op.equals("all") && !op.equals("some"))
			{
				op = op + "_cells";
			}

			node = node.getFirstChild();

			while ((node != null) && (node.getNodeName().equals("#text")))
			{
				node = node.getNextSibling();
			}

			expr = new CoverageExpr(node, pcr);
		}
		else
		{
			throw new WCPSException("invalid ReduceScalarExprType node : " + nodeName);
		}
	}

	public String toRasQL()
	{
		return op + "(" + expr.toRasQL() + ")";
	}
}
