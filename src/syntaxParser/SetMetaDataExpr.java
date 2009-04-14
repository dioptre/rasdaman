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

class SetMetaDataExpr implements IParseTreeNode {

	String function;
	CoverageExpr expr;
	IParseTreeNode param;
	String field;

	public SetMetaDataExpr(String op, CoverageExpr expr,IParseTreeNode param) {
		function = op;
		this.expr = expr;
		this.param = param;
	}
	public SetMetaDataExpr(String op, CoverageExpr expr, IParseTreeNode param, String field) {
		function = op;
		this.expr = expr;
		this.param = param;
	}
	
	public String toXML(){
		String result=  ""  ;

		if (function.equals("setNullDefault")) {
			result = "<setNullDefault>" + "<null>" +  param.toXML()  + "</null>"
				+ expr.toXML() 
				+ "</setNullDefault>";
		} else if (function.equals("setNullSet")) {
			result = "<setNullSet>";
			RangeValueList list = (RangeValueList) param;
			list.setTag("null");
			result += list.toXML();
			result += expr.toXML() + "</setNullSet>";
		} else if (function.equals("setInterpolationDefault")) {
			result = "<setInterpolationDefault><field>" + field + "</field>" 
				+ param.toXML()	+ expr.toXML() + "</setInterpolationDefault>";
		} else if (function.equals("setInterpolationSet")) {
			result = "<setInterpolationSet><field>" + field + "</field>" + param.toXML() +
					 expr.toXML() + "</setInterpolationSet>";
		} else if (function.equals("setCrsSet")) {
			// TODO(smsorin): schema/grammar inconsistencies
			result += "<setCrsSet>" + expr.toXML()  
				+ "</setCrsSet>";
		}	

		result+=  "";
		return result;
	}
	
}
