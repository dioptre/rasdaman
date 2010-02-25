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




package petascope.wcps.grammar;

/**
 * IndexExpr
 *
 * @author Andrei Aiordachioaie
 */
public class IndexExpr implements IParseTreeNode
{
	String constant;
	IParseTreeNode e1, e2;
	String function;
	String op;

	public IndexExpr(String constant)
	{
		function      = "constant";
		this.constant = constant;
	}

	public IndexExpr(String op, NumericScalarExpr e1)
	{
		this.op  = op;
		this.e1  = e1;
		function = "op1";
	}

	public IndexExpr(String op, IndexExpr e1, IndexExpr e2)
	{
		this.op  = op;
		this.e1  = e1;
		this.e2  = e2;
		function = "op2";
	}

	public String toXML()
	{
		String result = "";
		String tag1   = "<" + op + ">",
		       tag2   = "</" + op + ">";

		if (function.equals("constant"))
		{
			result = "<numericConstant>" + constant + "</numericConstant>";
		}
		else if (function.equals("op1"))
		{
			result = tag1 + e1.toXML() + tag2;
		}
		else if (function.equals("op2"))
		{
			result = tag1 + e1.toXML() + e2.toXML() + tag2;
		}

		return result;
	}
}
