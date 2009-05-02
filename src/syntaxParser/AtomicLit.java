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

class AtomicLit {


	BooleanExpr booleanExpr;
	int integerExpr;
	float floatExpr;
	String stringExpr;
	
	String flag;

	/**
	 * AtomicLit constructor comment.
	 */
	public AtomicLit() {
		super();
	}
	public AtomicLit( BooleanExpr be){	
		booleanExpr = be;
		flag = "boolean";
	}
	
	public AtomicLit( int ie){	
		stringExpr = ie.toString();
		flag = "int";
	}		
	
	public AtomicLit( float fe){	
		stringExpr = fe.toString();
		flag = "float";
	}

	public AtomicLit( String se){	
		stringExpr = se;
		flag = "string";
	}	
	
	String toXML(){
		String result="" ;
		try {
			if (flag.equals("boolean")) {
				result += "<boolean>"  + booleanExpr.toXML() + "</boolean>";
			} else if (flag.equals("int") || flag.equals("float") || flag.equals("string")) {
                result += "<scalar>" + stringExpr + "</scalar>
			}
		} catch (Exception e) {
			e.printStackTrace();	
		}
		return result;
	}
}
