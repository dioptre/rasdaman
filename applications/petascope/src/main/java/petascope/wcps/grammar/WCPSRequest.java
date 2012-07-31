/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.grammar;

import petascope.util.WCPSConstants;


/**
 * WCPSRequest class represents a WCPSRequest.
 * Creation date: (3/3/2003 2:52:55 AM)
 * @author: *, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class WCPSRequest implements IParseTreeNode {

    private ForClause forClause;
    private ReturnClause returnClause;
    private WhereClause whereClause;

    public WCPSRequest() {
        super();
    }

    public WCPSRequest(ForClauseElements f) {
        forClause = new ForClause(f);
        whereClause = null;
        returnClause = null;
    }

    public void setWhere(WhereClause w) {
        whereClause = w;
    }

    public void setReturn(ReturnClause r) {
        returnClause = r;
    }

    @Override
    public String toXML() {
        String result = WCPSConstants.MSG_XML_HEADER + "\n";

        result +=
                "<" + WCPSConstants.MSG_PROCESS_COVERAGE_REQUEST + " " + WCPSConstants.MSG_XMLNS + ">\n";
        result += "<" + WCPSConstants.MSG_QUERY + "><" + WCPSConstants.MSG_XML_SYNTAX + ">";

        result += forClause.toXML();

        if (whereClause != null) {
            result += whereClause.toXML();
        }

        if (returnClause != null) {
            result += returnClause.toXML();
        }

        result += "</" + WCPSConstants.MSG_XML_SYNTAX + "></" + WCPSConstants.MSG_QUERY + ">";
        result += "</" + WCPSConstants.MSG_PROCESS_COVERAGE_REQUEST + ">";

        return result;
    }
}
