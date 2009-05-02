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

class GetMetaDataExpr implements IParseTreeNode {
	CoverageExpr coverageExpr;
	String function;
	String field;
	
	/**
	 * GetMetaDataExpr constructor comment.
	 */
	public GetMetaDataExpr() {
		super();
	}
	
	public GetMetaDataExpr( String f, CoverageExpr ce){
		coverageExpr = ce ;
		function =f;
		field = null;
	}
	public GetMetaDataExpr( String f, CoverageExpr exp, String field) {
		coverageExpr = exp;
		function = f;
		this.field = field;
	}
	
	public String toXML() {
		String op = "operationNotSet";
		if (function.equals("identifier")) op = "getIdentifier";
		if (function.equals("imageCrs")) op = "getImageCrs";
		if (function.equals("imageCrsDomain")) op = "getImageCrsDomain"; //TODO(smsorin): The schema requires an extra parameter axis not documented in the protocol specfication
		if (function.equals("crsSet")) op = "crsSet";
		if (function.equals("generalDomain")) op = "getGeneralDomain"; //TODO(smsorin): The schema requires an extra parameter axis and optional* parameter crs
		if (function.equals("nullDefault")) op = "getNullDefault";
		if (function.equals("nullSet")) op = "getNullSet";
		if (function.equals("interpolationDefault")) op = "getInterpolationDefault";
		if (function.equals("interpolationSet")) op = "getInterpolationSet";
		String result = "<" + op + ">";
		result += coverageExpr.toXML();
		if (field != null) {
		    result += "<field>" + field + "</field>";		    		    
		}
		result += "</" + op + ">";
		return result;
	}
	
}
