/*
 * This file is part of Petascope.
 *
 * Petascope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Petascope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Petascope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.Petascope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package grammar;

public class SetMetaDataExpr implements IParseTreeNode {

	String function;
	CoverageExpr expr;
	IParseTreeNode param;
	String field;

    // Set Identifier
    public SetMetaDataExpr(String op, CoverageExpr expr, String constant)
    {
        function = op;
        this.expr = expr;
        this.field = constant;
    }

    // Set Crs Set
    public SetMetaDataExpr(String op, CoverageExpr expr, CrsList clist)
    {
        function = op;
        this.expr = expr;
        this.param = clist;
    }

    // Set Null Set
	public SetMetaDataExpr(String op, CoverageExpr expr, RangeExprList param)
    {
		function = op;
		this.expr = expr;
		this.param = param;
	}

    // Set Interpolation Default
	public SetMetaDataExpr(String op, CoverageExpr expr, InterpolationMethod param, String field)
    {
		function = op;
		this.expr = expr;
		this.param = param;
        this.field = field;
	}

    // Set Interpolation Set
    public SetMetaDataExpr(String op, CoverageExpr expr, InterpolationMethodList param, String field)
    {
		function = op;
		this.expr = expr;
		this.param = param;
        this.field = field;
	}
    
	
	public String toXML()
    {
		String result = "";

        if (function.equalsIgnoreCase("setIdentifier"))
        {
            result += "<identifier>" + field + "</identifier>";
            result += expr.toXML();

            result = "<setIdentifier>" + result + "</setIdentifier>";
        }
        else if (function.equalsIgnoreCase("setCrsSet"))
        {
            result += expr.toXML();
            if (param != null)
                result += param.toXML();

            result = "<setCrsSet>" + result + "</setCrsSet>";
        }
        else if (function.equalsIgnoreCase("setNullSet"))
        {
            result += expr.toXML();
            if (param != null)
                result += param.toXML();

            result = "<setNullSet>" + result + "</setNullSet>";
        }
        else if (function.equalsIgnoreCase("setInterpolationDefault"))
        {
            result += expr.toXML();
            result += "<field>" + field + "</field>";
            result += param.toXML();

            result = "<setInterpolationDefault>" + result + "</setInterpolationDefault>";
        }
        else if (function.equalsIgnoreCase("setInterpolationSet"))
        {
            result += expr.toXML();
            result += "<field>" + field + "</field>";
            result += param.toXML();

            result = "<setInterpolationSet>" + result + "</setInterpolationSet>";
        }
        else
        {
            System.err.println("Unknown SetMetadataExpr operation: " + function);
        }

		return result;
	}
	
}
