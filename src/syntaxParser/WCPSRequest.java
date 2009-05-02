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

import java.util.*;

public class WCPSRequest implements IParseTreeNode {
	private ForClause forClause;
	private WhereClause whereClause;
	private ReturnClause returnClause;
	
	/**
	 * WCPSRequest constructor comment.
	 */
	public WCPSRequest() {
		super();
	}
	public WCPSRequest(ForClause f, ReturnClause r) {
		forClause = f;
		whereClause = null;
		returnClause = r;
	}
	public WCPSRequest(ForClause f, WhereClause w, ReturnClause r) {
		forClause = f;
		whereClause = w;
		returnClause = r;
	}

	public String toXML(){
		String result="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
		result += "<processCoverage xmlns=\"http://www.opengis.net/wcps/1.0.0\" service=\"WCPS\" version=\"1.0.0\">\n";
		result += forClause.toXML();
		if (whereClause != null) {
			result += whereClause.toXML();
		}
		result += returnClause.toXML();
		result += "</processCoverage>";
		return result;
	}
	
}
