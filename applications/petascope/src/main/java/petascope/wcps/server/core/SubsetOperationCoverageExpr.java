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
package petascope.wcps.server.core;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WCPSException;
import org.w3c.dom.*;
import petascope.util.Pair;

// TODO: Implement class SubsetOperation
public class SubsetOperationCoverageExpr implements IRasNode, ICoverageInfo {
    
    private static Logger log = LoggerFactory.getLogger(SubsetOperationCoverageExpr.class);

    private IRasNode child;
    private CoverageInfo info = null;

    public SubsetOperationCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException {

        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        if (node == null) {
            throw new WCPSException("SubsetOperationCoverageExpr parsing error!");
        }

        String nodeName = node.getNodeName();
        log.trace(nodeName);

        if (nodeName.equals("trim")) {
            log.trace("  trim child");
            child = new TrimCoverageExpr(node, xq);
            info = ((TrimCoverageExpr) child).getCoverageInfo();
        } else if (nodeName.equals("extend")) {
            log.trace("  extend child");
            child = new ExtendCoverageExpr(node, xq);
            info = ((ExtendCoverageExpr) child).getCoverageInfo();
        } else if (nodeName.equals("slice")) {
            log.trace("  slice child");
            child = new SliceCoverageExpr(node, xq);
            info = ((SliceCoverageExpr) child).getCoverageInfo();
        } else {
            log.error("  failed to match SubsetOperation: " + nodeName);
            throw new WCPSException("Failed to match SubsetOperation: " + nodeName);
        }
    }

    @Override
    public String toRasQL() {
        return child.toRasQL();
    }
    
    Pair<String[], String> computeRasQL() {
        if (child instanceof TrimCoverageExpr) {
            return ((TrimCoverageExpr) child).computeRasQL();
        } else if (child instanceof SliceCoverageExpr) {
            return ((SliceCoverageExpr) child).computeRasQL();
        } else {
            return Pair.of(null, child.toRasQL());
        }
    }

    @Override
    public CoverageInfo getCoverageInfo() {
        return info;
    }
}
