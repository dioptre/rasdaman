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

import java.util.LinkedList;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.WCPSConstants;

/**
 * Subset containing mixed trim/slicing
 * 
 * @author Dimitar Misev
 */
public class TrimSliceExpr implements IParseTreeNode {
    
    private static Logger log = LoggerFactory.getLogger(TrimSliceExpr.class);

    CoverageExpr expr;
    List<IParseTreeNode> subsets;

    public TrimSliceExpr(CoverageExpr ce) {
        log.trace(WCPSConstants.MSG_CREATE);
        expr = ce;
        subsets = new LinkedList<IParseTreeNode>();
    }
    
    public void add(IParseTreeNode n) {
        log.trace(WCPSConstants.MSG_ADD_SUBSET);
        subsets.add(n);
    }

    public String toXML() {
        String ret = expr.toXML();
        String type = null;
        for (IParseTreeNode n : subsets) {
            if (n instanceof DimensionPointElement) {
                type = WCPSConstants.MSG_SLICE;
            } else if (n instanceof DimensionIntervalElement) {
                type = WCPSConstants.MSG_TRIM;
            }
            ret = "<" + type + ">" + ret + n.toXML() + "</" + type + ">";
        }
        log.trace(ret);
        return ret;
    }
}
