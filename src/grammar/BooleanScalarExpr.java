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
 * BooleanScalarExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */

public class BooleanScalarExpr implements IParseTreeNode
{
	String booleanConstant;
	IParseTreeNode left, right;
	BooleanScalarExpr leftBooleanScalarExpr, rightBooleanScalarExpr;
	NumericScalarExpr leftNumericScalar, rightNumericScalar;
	String node1, node2;
	String op;

	public BooleanScalarExpr(String bc)
	{
		op              = null;
		booleanConstant = bc;
	}

	public BooleanScalarExpr(String op, BooleanScalarExpr be)
	{
		this.op = op;
		left    = be;
	}

	public BooleanScalarExpr(String op, BooleanScalarExpr lbe, BooleanScalarExpr rbe)
	{
		this.op = op;
		left    = lbe;
		right   = rbe;
	}

	public BooleanScalarExpr(String op, NumericScalarExpr left, NumericScalarExpr right)
	{
		this.op    = op;
		this.left  = left;
		this.right = right;
	}

	public BooleanScalarExpr(String op, StringScalarExpr e1, StringScalarExpr e2)
	{
		this.op = op;
		left    = e1;
		right   = e2;
	}

	public String toXML()
	{
		if (op == null)
		{
			return "<booleanConstant>" + booleanConstant + "</booleanConstant>";
		}
		else if (op.equals("not"))
		{
			return "<booleanNot>" + left.toXML() + "</booleanNot>";
		}
		else
		{
			if (this.left != null)
			{
				node1 = this.left.toXML();
			}

			if (this.right != null)
			{
				node2 = this.right.toXML();
			}

			if (op.equals("and"))
			{
				op = "booleanAnd";
			}
			else if (op.equals("or"))
			{
				op = "booleanOr";
			}
			else if (op.equals("xor"))
			{
				op = "booleanXor";

			}
			else if (op.equals("="))
			{
				op = "booleanEqualNumeric";
			}
			else if (op.equals("!="))
			{
				op = "booleanNotEqualNumeric";
			}
			else if (op.equals("<"))
			{
				op = "booleanLessThan";
			}
			else if (op.equals(">"))
			{
				op = "booleanGreaterThan";
			}
			else if (op.equals("<="))
			{
				op = "booleanLessOrEqual";
			}
			else if (op.equals(">="))
			{
				op = "booleanGreaterOrEqual";
			}

			return "<" + op + ">" + node1 + node2 + "</" + op + ">";
		}

	}
}
