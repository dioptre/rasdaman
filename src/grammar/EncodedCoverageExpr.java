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
 * EncodedCoverageExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class EncodedCoverageExpr implements IParseTreeNode
{
	CoverageExpr expr;
	String extraParams;
	String format;
	boolean store;

	public EncodedCoverageExpr(CoverageExpr ce, String fn)
	{
		expr        = ce;
        // remove double quotes
        if (fn.getBytes()[0] == '"' && fn.getBytes()[fn.length()-1] == '"')
            format = fn.substring(1, fn.length()-1);
        else
            format      = fn;

		extraParams = null;
		store       = false;
	}

	public void setParams(String params)
	{
		extraParams = params;
	}

	public void setStoreFlag()
	{
		store = true;
	}

	public String toXML()
	{
		String result = "";

		if (store)
		{
			result = "<encode store=\"true\">";
		}
		else
		{
			result = "<encode store=\"false\">";
		}

		result += expr.toXML();
		result += "<format>" + format + "</format>";

		if (extraParams != null)
		{
			result += "<extraParams>" + extraParams + "</extraParams>";
		}

		result += "</encode>";

		return result;
	}
}
