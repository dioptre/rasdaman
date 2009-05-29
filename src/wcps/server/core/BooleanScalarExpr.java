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

public class BooleanScalarExpr implements IRasNode
{
	private BooleanScalarExpr first, second;
	private String op;
	private boolean simple;    // true if the expression is just a value
	private String value;

	public BooleanScalarExpr(Node node, ProcessCoveragesRequest pcr) throws WCPSException
	{
        if (node == null)
            throw new WCPSException("Unexpected null node !");

		String nodeName = node.getNodeName();

		simple = false;

		System.out.println("Parsing boolean scalar operation ...");

		if (nodeName.equals("booleanConstant"))
		{
			simple = true;
			value  = node.getFirstChild().getNodeValue();
		}
		else if (nodeName.equals("booleanAnd")
            || nodeName.equals("booleanOr")
		    || nodeName.equals("booleanXor")
            || nodeName.equals("booleanLessThan")
            || nodeName.equals("booleanLessOrEqual")
		    || nodeName.equals("booleanGreaterThan")
		    || nodeName.equals("booleanGreaterOrEqual")
		    || nodeName.equals("booleanEqualNumeric")
		    || nodeName.equals("booleanNotEqualNumeric")
		    || nodeName.equals("booleanEqualString")
		    || nodeName.equals("booleanNotEqualString"))
		{
			// Logical operations
			if (nodeName.equals("booleanAnd")
                || nodeName.equals("booleanOr")
			    || nodeName.equals("booleanXor")) 
            {
                // Remove the "boolean" in front
                op = nodeName.substring(7).toLowerCase();
            }
            else
            // Boolean Comparison operations
            {
                if (nodeName.equals("booleanLessThan"))
                    op = "<";
                if (nodeName.equals("booleanLessOrEqual"))
                    op = "<=";
                if (nodeName.equals("booleanGreaterThan"))
                    op = ">";
                if (nodeName.equals("booleanGreaterOrEqual"))
                    op = ">=";
                if (nodeName.equals("booleanEqualNumeric") || nodeName.equals("booleanEqualString"))
                    op = "=";
                if (nodeName.equals("booleanNotEqualNumeric") || nodeName.equals("booleanNotEqualString"))
                    op = "!=";
            }

			Node child = node.getFirstChild();

			first  = new BooleanScalarExpr(child, pcr);
			child  = child.getNextSibling();
			second = new BooleanScalarExpr(child, pcr);
		}
		else if (nodeName.equals("booleanNot"))
		{
			op    = "not";
			first = new BooleanScalarExpr(node.getFirstChild(), pcr);
		}
		else
		{
			throw new WCPSException("Unexpected Binary Expression node : "
						+ node.getNodeName());
		}
        System.err.println("*** Boolean Scalar Expr SUCCESS: " + node.getNodeName());
	}

	public String toRasQL()
	{
		if (simple)
		{
			return value;
		}

		if (op.equals("not"))
		{
			return "not(" + first.toRasQL() + ")";
		}

		return "(" + first.toRasQL() + ")" + op + "(" + second.toRasQL() + ")";
	}
}
