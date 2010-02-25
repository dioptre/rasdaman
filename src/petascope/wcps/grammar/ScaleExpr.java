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
 * ScaleExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class ScaleExpr implements IParseTreeNode
{
	CoverageExpr coverageExpr;
	FieldInterpolationList interpMethods;
	DimensionIntervalList intervalList;

	public ScaleExpr(CoverageExpr ce, DimensionIntervalList ail, FieldInterpolationList iml)
	{
		coverageExpr  = ce;
		intervalList  = ail;
		interpMethods = iml;
	}

	public String toXML()
	{
		String result = "";

		result += coverageExpr.toXML() + intervalList.toXML() + interpMethods.toXML();
		result = "<scale>" + result + "</scale>";

		return result;
	}
}
