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
 * GeneralCondenseExpr
 *
 * @author Andrei Aiordachioaie
 */
public class GeneralCondenseExpr implements IParseTreeNode
{
	AxisIteratorList alist;
	CondenseOperationType op;
	ScalarExpr using;
	BooleanScalarExpr where;

	public GeneralCondenseExpr(CondenseOperationType op, AxisIteratorList al)
	{
		this.op = op;
		alist   = al;
		alist.setTag("iterator");
		where = null;
		using = null;
	}

	public void setWhere(BooleanScalarExpr bse)
	{
		where = bse;
	}

	public void setUsing(ScalarExpr se)
	{
		using = se;
	}

	public String toXML()
	{
		String result = "";

		result += op.toXML();
		result += alist.toXML();

		if (where != null)
		{
			result += "<where>" + where.toXML() + "</where>";
		}

		result += using.toXML();

		return result;
	}
}
