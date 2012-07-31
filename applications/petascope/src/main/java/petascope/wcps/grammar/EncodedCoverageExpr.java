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
 * EncodedCoverageExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class EncodedCoverageExpr implements IParseTreeNode {

    CoverageExpr expr;
    String extraParams;
    String format;
    boolean store;

    public EncodedCoverageExpr(CoverageExpr ce, String fn) {
        expr = ce;
        // remove double quotes
        if (fn.getBytes()[0] == '"' && fn.getBytes()[fn.length() - 1] == '"') {
            format = fn.substring(1, fn.length() - 1);
        } else {
            format = fn;
        }

        extraParams = null;
        store = false;
    }

    public void setParams(String params) {
        extraParams = params;
    }

    public void setStoreFlag() {
        store = true;
    }

    @Override
    public String toXML() {
        String result = "";

        if (store) {
            result = "<" + WCPSConstants.MSG_ENCODE + " " + WCPSConstants.MSG_STORE + 
                    "=\"" + WCPSConstants.MSG_TRUE + "\">";
        } else {
            result = "<" + WCPSConstants.MSG_ENCODE + " " + WCPSConstants.MSG_STORE + 
                    "=\"" + WCPSConstants.MSG_FALSE + "\">";
        }

        result += expr.toXML();
        result += "<" + WCPSConstants.MSG_FORMAT + ">" + format + "</" + WCPSConstants.MSG_FORMAT + ">";

        if (extraParams != null) {
            result += "<" + WCPSConstants.MSG_EXTRA_PARAMETERS + ">" + extraParams + "</" + 
                    WCPSConstants.MSG_EXTRA_PARAMETERS + ">";
        }

        result += "</" + WCPSConstants.MSG_ENCODE + ">";

        return result;
    }
}
