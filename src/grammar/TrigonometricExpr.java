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

package grammar;

public class TrigonometricExpr implements IParseTreeNode {
	String trigOperator;
	CoverageExpr coverageExpr;
	
	public TrigonometricExpr(String op, CoverageExpr ce){
		trigOperator = op;
		coverageExpr = ce;		
	}

	public String toXML(){
		String result=  ""  ;
		result += "<" + trigOperator + ">";
		result += coverageExpr.toXML() ;
		result += "</" + trigOperator + ">";
		return result;
	}
}
