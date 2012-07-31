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
 * IntervalExpr
 *
 * @author Andrei Aiordachioaie
 */
public class IntervalExpr implements IParseTreeNode {
    private static Logger log = LoggerFactory.getLogger(IntervalExpr.class);

    IParseTreeNode e1, e2;
    String function;

    public IntervalExpr(IndexExpr n1, IndexExpr n2) {
        log.trace(WCPSConstants.MSG_CREATING_INTERVALEXPR);
        this.e1 = n1;
        this.e2 = n2;
        function = WCPSConstants.MSG_TWO_INDEXES;
    }

    public IntervalExpr(String coverage, String axis) {
        log.trace(WCPSConstants.MSG_CREATING_INTERVALEXPR + ": " + coverage + " " + WCPSConstants.MSG_AND + 
                " " + WCPSConstants.MSG_AXIS + ": " + axis);
        function = WCPSConstants.MSG_CRS_METADATA;
        this.e1 = new ImageCrsDomainMetadataExpr(coverage, axis);
    }

    public String toXML() {
        String result = "";

        if (function.equals(WCPSConstants.MSG_TWO_INDEXES)) {
            result = e1.toXML() + e2.toXML();
        } else if (function.equals(WCPSConstants.MSG_CRS_METADATA)) {
            result = e1.toXML();
        }

        return result;
    }
}
