/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */
package petascope.wcps.grammar;

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

    public String toXML() {
        String result = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";

        result +=
                "<ProcessCoveragesRequest xmlns=\"http://www.opengis.net/wcps/1.0\" service=\"WCPS\" "
                + "version=\"1.0.0\">\n";
        result += "<query><xmlSyntax>";

        result += forClause.toXML();

        if (whereClause != null) {
            result += whereClause.toXML();
        }

        if (returnClause != null) {
            result += returnClause.toXML();
        }

        result += "</xmlSyntax></query>";
        result += "</ProcessCoveragesRequest>";

        return result;
    }
}
