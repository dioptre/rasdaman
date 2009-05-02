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

package syntaxParser;

class CoverageExpr implements IParseTreeNode {

	String coverageName;
	IParseTreeNode expr;
	String function;
	
	public CoverageExpr( String n ){	
		coverageName = n;
		function = "name";
	}
	
	public CoverageExpr( SetMetaDataExpr smde ){	
		expr = smde;
		function = "setMetaDataExpr";
	}	
	
	public CoverageExpr( InducedExpr ie ){	
		expr = ie;
		function = "inducedExpr";
	}
	
	public CoverageExpr( SubsetExpr se ){	
		expr = se;
		function = "subsetExpr";
	}		
	
	public CoverageExpr( CrsTransformExpr cte ){	
		expr = cte;
		function = "crsTransformExpr";
	}		

	public CoverageExpr( ScaleExpr se ){	
		expr =  se;
		function = "scaleExpr";
	}	
	
	public CoverageExpr( CoverageExpr ce ){	
		expr = ce;
		function = "coverageExpr";
	}	

	public CoverageExpr( CoverageConstructorExpr ce ){	
		expr = ce;
		function = "coverageConstructorExpr";
	}

	public String toXML(){
		String result=  ""  ;
		if (function.equals("name")) {
				result = "<coverage>" + coverageName + "</coverage>";
		} else 
			result = expr.toXML();		
		return result;
	}
}
