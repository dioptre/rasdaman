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
 * IntervalExpr
 *
 * @author Andrei Aiordachioaie
 */
public class IntervalExpr implements IParseTreeNode
{
	IParseTreeNode e1, e2;
	String function;

	public IntervalExpr(IndexExpr n1, IndexExpr n2)
	{
		this.e1  = n1;
		this.e2  = n2;
		function = "two indexes";
	}

	public IntervalExpr(String coverage, String axis)
	{
		function = "crs metadata";
		this.e1  = new ImageCrsDomainMetadataExpr(coverage, axis);
	}

	public String toXML()
	{
		String result = "";

		if (function.equals("two indexes"))
		{
			result = e1.toXML() + e2.toXML();
		}
		else if (function.equals("crs metadata"))
		{
			result = e1.toXML();
		}

		return result;
	}
}
