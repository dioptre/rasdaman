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

class AxisIntervalElement implements IParseTreeNode {
	String axisName;
	String lo, hi;
	String crs;

	public AxisIntervalElement(String name, String lo, String hi) {
		axisName = name;
		this.lo = lo;
		this.hi = hi;
	}

	public AxisIntervalElement(String name,String crs, String lo, String hi) {
		axisName = name;
		this.crs = crs;
		this.lo = lo;
		this.hi = hi;
	}

	public String toXML() {
		String result = "<axis>" + axisName + "</axis>";
		if (crs != null) 
			result += "<crs>" + crs + "</crs>";
		result += "<coord>" + lo + "</coord><coord>" + hi + 
			      "</coord>";
		return result;
	}
}
