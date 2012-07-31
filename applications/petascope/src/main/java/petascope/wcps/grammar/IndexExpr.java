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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.WCPSConstants;

/**
 * IndexExpr
 *
 * @author Andrei Aiordachioaie
 */
public class IndexExpr implements IParseTreeNode {
    
    private static Logger log = LoggerFactory.getLogger(IndexExpr.class);

    String constant;
    IParseTreeNode e1, e2;
    String function;
    String op;

    public IndexExpr(String constant) {
        log.trace(WCPSConstants.MSG_INDEX_EXPR + ": " + constant);
        function = WCPSConstants.MSG_CONSTANT;
        this.constant = constant;
    }

    public IndexExpr(String op, NumericScalarExpr e1) {
        log.trace(WCPSConstants.MSG_INDEX_EXPR + ": " + op + " " + WCPSConstants.MSG_NUM);
        this.op = op;
        this.e1 = e1;
        function = WCPSConstants.MSG_OP1;
    }

    public IndexExpr(String op, IndexExpr e1, IndexExpr e2) {
        log.trace(WCPSConstants.MSG_INDEX_EXPR + ": " + WCPSConstants.MSG_A + " " + op + " " + WCPSConstants.MSG_B);
        this.op = op;
        this.e1 = e1;
        this.e2 = e2;
        function = WCPSConstants.MSG_OP2;
    }

    public String toXML() {
        String result = "";
        String tag1 = "<" + op + ">",
                tag2 = "</" + op + ">";

        if (function.equals(WCPSConstants.MSG_CONSTANT)) {
            result = "<" + WCPSConstants.MSG_NUMERIC_CONSTANT + ">" + constant + "</" + 
                    WCPSConstants.MSG_NUMERIC_CONSTANT + ">";
        } else if (function.equals(WCPSConstants.MSG_OP1)) {
            result = tag1 + e1.toXML() + tag2;
        } else if (function.equals(WCPSConstants.MSG_OP2)) {
            result = tag1 + e1.toXML() + e2.toXML() + tag2;
        }

        return result;
    }
}
