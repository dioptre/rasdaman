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
 * CondenseOperationType
 *
 * @author Andrei Aiordachioaie
 */
public class CondenseOperation implements IParseTreeNode {

    String op;

    public CondenseOperation(String op) {
        this.op = op;
    }

    public String toXML() {
        String result = "";

        if (op.equalsIgnoreCase(WCPSConstants.MSG_PLUS)) {
            result = WCPSConstants.MSG_OP_PLUS;
        } else if (op.equalsIgnoreCase(WCPSConstants.MSG_STAR)) {
            result = WCPSConstants.MSG_OP_MULT;
        } else if (op.equalsIgnoreCase(WCPSConstants.MSG_MAX)) {
            result = WCPSConstants.MSG_OP_MAX;
        } else if (op.equalsIgnoreCase(WCPSConstants.MSG_MIN)) {
            result = WCPSConstants.MSG_OP_MIN;
        } else if (op.equalsIgnoreCase(WCPSConstants.MSG_AND)) {
            result = WCPSConstants.MSG_OP_AND;
        } else if (op.equalsIgnoreCase(WCPSConstants.MSG_OR)) {
            result = WCPSConstants.MSG_OP_OR;
        }

        result = "<" + result + "/>";

        return result;
    }
}
