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

public class NumericScalarExpr implements IRasNode
{
    private IRasNode first, second;
	private String op, value;
    private boolean twoChildren;

	public NumericScalarExpr(Node node, XmlQuery xq) throws WCPSException
	{
        twoChildren = false;
        String nodeName = node.getNodeName();

		op = "";

		System.out.println("Trying to parse numeric scalar expression ...");

		if (nodeName.equals("numericConstant"))
		{
            twoChildren = false;
			op = code(nodeName);
			value  = node.getFirstChild().getNodeValue();
            try
            {
                double dval = Double.parseDouble(value);
            }
            catch (NumberFormatException e)
            {
                throw new WCPSException("Could not understand constant: " + value);
            }
		}
		else if (nodeName.equals("complexConstant")
            || nodeName.equals("condense")
		    || nodeName.equals("reduce"))
		{
            op = code(nodeName);
            twoChildren = false;
            if (nodeName.equals("complexConstant"))
                first = new ComplexConstant(node, xq);
            if (nodeName.equals("condense"))
                first = new CondenseScalarExpr(node, xq);
            if (nodeName.equals("reduce"))
                first = new ReduceScalarExpr(node, xq);
		}
		else if (nodeName.equals("numericUnaryMinus"))
		{
            op = code(nodeName);
            twoChildren = false;
            first = new NumericScalarExpr(node.getFirstChild(), xq);
		}
        else if (nodeName.equals("numericAdd")
                || nodeName.equals("numericMinus")
                || nodeName.equals("numericMult")
                || nodeName.equals("numericDiv"))
        {
            try
            {
                op = code(nodeName);
                twoChildren = true;
                Node child = node.getFirstChild();
                first = new NumericScalarExpr(child, xq);
                second = new NumericScalarExpr(child.getNextSibling(), xq);
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to parse a numeric expression pair !");
            }
        }

		else
		{
			throw new WCPSException("Unexpected Numeric Scalar Expression node : "
						+ node.getNodeName());
		}
	}

	public String toRasQL()
	{
        String result = "";
        if (op.equals("value"))
            result = value;
        else if ((twoChildren==false) && (op.equals("-")))
            result = "-" + first.toRasQL();
        else if (op.equals("child"))
            result = first.toRasQL();
        else if (twoChildren == true)
            result = "(" + first.toRasQL() + ")" + op +
                     "(" + second.toRasQL() + ")";
        else
            return " error ";

        return result;
	}

    public String code(String name)
    {
        String op = "";
        if (name.equals("numericConstant"))
            op = "value";
        if (name.equals("numericUnaryMinus") || name.equals("numericMinus"))
            op = "-";
        if (name.equals("numericAdd"))
            op = "+";
        if (name.equals("numericMult"))
            op = "*";
        if (name.equals("numericDiv"))
            op = "/";
        if (name.equals("condense") || name.equals("reduce")
                || name.equals("complexConstant"))
            op = "child";

        return op;
    }
}