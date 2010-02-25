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
 * DimensionIntervalExpr
 *
 * @author Andrei Aiordachioaie
 */
public class DimensionIntervalExpr implements IParseTreeNode
{
	IParseTreeNode e1, e2;
	String fun;
	String str;

	public DimensionIntervalExpr(ScalarExpr exp1, ScalarExpr exp2)
	{
		fun = "scalars";
		e1  = exp1;
		e2  = exp2;
	}

	public DimensionIntervalExpr(String cov, String axis, String crs)
	{
		fun = "domain metadata";
		str = "<coverage>" + cov + "</coverage>";
		str += "<axis>" + axis + "</axis>";
		str += "<crs>" + crs + "</crs>";
	}

	public String toXML()
	{
		String result = "";

		if (fun.equals("scalars"))
		{
			result += "<lowerBound>" + e1.toXML() + "</lowerBound>";
			result += "<upperBound>" + e2.toXML() + "</upperBound>";
		}
		else if (fun.equals("domain metadata"))
		{
			result = str;
		}

		return result;
	}
}
