/*
 * This file is part of %PACKAGENAME%.
 *
 * %PACKAGENAME% is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * %PACKAGENAME% is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with %PACKAGENAME%. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.%PACKAGENAME%.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package syntaxParser;

class NumericScalarExpr implements IParseTreeNode {
	NumericScalarExpr leftNumericScalarExpr, rightNumericScalarExpr;
	String function;
	String constValue;

	public NumericScalarExpr(String scalar){
		constValue = scalar;
		function = "scalar";
	}
	
	
	public NumericScalarExpr(String op, NumericScalarExpr expr) throws Exception{
		leftNumericScalarExpr = expr;
		if (op.equals("+")) function = "scalarUnaryPlus";
		else if (op.equals("-")) function = "scalarUnaryMinus";
		else if (op.equals("abs")) function = "scalarAbs";
		else throw new Exception("Operator " + op + " is not recognized!");
	}

	public NumericScalarExpr(String op, NumericScalarExpr lbe, NumericScalarExpr rbe) throws Exception {
		leftNumericScalarExpr = lbe ;
		rightNumericScalarExpr = rbe ;
		if (op.equals("+")) function = "scalarPlus";
		else if (op.equals("-")) function = "scalarMinus";
		else if (op.equals("*")) function = "scalarMult";
		else if (op.equals("/")) function = "scalarDiv";
		else throw new Exception("Operator " + op + " is not recognized!");
	}	
	
	public String toXML() {
		String result;
		result = "<" + function + ">";
		if (function.equals("scalar")) {
			result += constValue;
		} else {
			result += leftNumericScalarExpr.toXML();
			if (rightNumericScalarExpr != null) {
				result += rightNumericScalarExpr.toXML();
			}
		}
		result += "</" + function + ">";
		return result;
	}
	
}
