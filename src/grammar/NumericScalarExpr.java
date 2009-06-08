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




package grammar;

/**
 * NumericScalarExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class NumericScalarExpr implements IParseTreeNode
{
	CondenseExpr condense;
	String constValue;
	String function;
	NumericScalarExpr leftNumericScalarExpr, rightNumericScalarExpr;

	public NumericScalarExpr(CondenseExpr c)
	{
		condense = c;
		function = "condense";
	}

	public NumericScalarExpr(String val)
	{
		if (val.contains("+i"))
		{
			ComplexConst cc = new ComplexConst(val);

			constValue = cc.toXML();
			function   = "complexConstant";
		}
		else
		{
			constValue = val;
			function   = "numericConstant";
		}
	}

	public NumericScalarExpr(String op, NumericScalarExpr expr)
	{
		leftNumericScalarExpr = expr;

		if (op.equals("-"))
		{
			function = "numericUnaryMinus";
		}
		else
		{
			System.err.println("Unary Operator " + op + " is not recognized!");
		}
	}

    public NumericScalarExpr(String varOp, String varName)
    {
        if (varOp.equals("var"))
        {
            function = "variableRef";
            constValue = varName;
        }
        else
            System.err.println("Internal error: this should have been a variable name:" + varName);
    }

	public NumericScalarExpr(String op, NumericScalarExpr lbe, NumericScalarExpr rbe)
	{
		leftNumericScalarExpr  = lbe;
		rightNumericScalarExpr = rbe;

		if (op.equals("+"))
		{
			function = "numericAdd";
		}
		else if (op.equals("-"))
		{
			function = "numericMinus";
		}
		else if (op.equals("*"))
		{
			function = "numericMult";
		}
		else if (op.equals("/"))
		{
			function = "numericDiv";
		}
		else
		{
			System.err.println("Operator " + op + " is not recognized!");
		}
	}

	public String toXML()
	{
		String result;

		if (function.equals("complexConstant"))
		{
			return constValue;
		}

		result = "<" + function + ">";

		if (function.equals("numericConstant") || function.equals("variableRef"))
		{
			result += constValue;
		}
		else
		{
			result += leftNumericScalarExpr.toXML();

			if (rightNumericScalarExpr != null)
			{
				result += rightNumericScalarExpr.toXML();
			}
		}

		result += "</" + function + ">";

		return result;
	}
}
