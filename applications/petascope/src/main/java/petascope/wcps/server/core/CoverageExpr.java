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

import petascope.exceptions.WCPSException;
import org.w3c.dom.*;
import java.util.Iterator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.WCPSConstants;

public class CoverageExpr implements IRasNode, ICoverageInfo {
    
    private static Logger log = LoggerFactory.getLogger(CoverageExpr.class);

    private IRasNode child;
    private String childInfo;
    private CoverageInfo info;
    private boolean scalarExpr = false;
//    private String var;
    private boolean simpleCoverage;    // True if the coverage is just a string

    public CoverageExpr(Node node, XmlQuery xq) throws WCPSException {
        while ((node != null) && node.getNodeName().equals("#" + WCPSConstants.MSG_TEXT)) {
            node = node.getNextSibling();
        }

        if (node == null) {
            throw new WCPSException(WCPSConstants.ERRTXT_COVERAGEEXPRTYPE_PASING_ERR);
        }

        String nodeName = node.getNodeName();
        log.trace(nodeName);

        simpleCoverage = false;

        if (nodeName.equals(WCPSConstants.MSG_COVERAGE)) {
            simpleCoverage = true;
            childInfo = node.getFirstChild().getNodeValue();

            if (!xq.isIteratorDefined(childInfo)) {
                throw new WCPSException(WCPSConstants.MSG_ITERATOR + " " + childInfo + " " + WCPSConstants.ERRTXT_NOT_DEFINED);
            }

            Iterator<String> coverages = xq.getCoverages(childInfo);

            try {
                info = new CoverageInfo(xq.getMetadataSource().read(coverages.next()));

                while (coverages.hasNext()) {    // Check if all the coverages are compatible
                    CoverageInfo tmp = new CoverageInfo(
                            xq.getMetadataSource().read(
                            coverages.next()));

                    if (!tmp.isCompatible(info)) {
                        throw new WCPSException(
                                WCPSConstants.ERRTXT_INCOMPATIBLE_COVERAGES);
                    }
                }
            } catch (Exception ex) {
                throw new WCPSException(ex.getMessage(), ex);
            }

            log.trace(WCPSConstants.MSG_FOUND_SIMPLE_COVERAGE_DEF + ": " + childInfo + ", "
                    + info.toString());
        } else if (nodeName.equals(WCPSConstants.MSG_CRS_TRANSFORM)) {
            // TODO: implement CrsTransform class
            child = new CrsTransformCoverageExpr(node, xq);
        } else if (nodeName.equals(WCPSConstants.MSG_SCALE)) {
            child = new ScaleCoverageExpr(node, xq);
        } else if (nodeName.equals(WCPSConstants.MSG_CONSTRUCT)) {
            child = new ConstructCoverageExpr(node.getFirstChild(), xq);
        } else if (nodeName.equals(WCPSConstants.MSG_CONST)) {
            child = new ConstantCoverageExpr(node.getFirstChild(), xq);
        } //        else if (nodeName.equals("variableRef"))
        //        {
        //            child = new VariableReference(node, xq);
        //        }
        else {    // Try one of the groups
            child = null;

            if (child == null) {
                try {
                    child = new SetMetadataCoverageExpr(node, xq);
                    log.trace("  " + WCPSConstants.MSG_MATCHED_SET_METADATA);
                } catch (WCPSException e) {
                    child = null;
                }
            }

            if (child == null) {
                try {
                    child = new InducedOperationCoverageExpr(node, xq);
                    log.trace("  " + WCPSConstants.MSG_MATCHED_INDUCED_COVERAGE);
                } catch (WCPSException e) {
                    child = null;
                    if (e.getMessage().equals(WCPSConstants.MSG_METHOD_NOT_IMPL)) {
                        throw e;
                    }
                }
            }

            if (child == null) {
                try {
                    child = new SubsetOperationCoverageExpr(node, xq);
                    log.trace("  " + WCPSConstants.MSG_MATCHED_SUBSET_OP);
                } catch (WCPSException e) {
                    child = null;
                }
            }

            if (child == null) {
                try {
                    child = new ScalarExpr(node, xq);
                    this.scalarExpr = true;
                    log.trace(WCPSConstants.MSG_MATCHED_SCALAR_EXPR);
                } catch (WCPSException e) {
                    child = null;
                }
            }
        }

        if (!simpleCoverage && (child == null)) {
            throw new WCPSException(WCPSConstants.ERRTXT_INVALID_COVERAGE_EXPR + ": "
                    + node.getNodeName());
        }

        if (info == null) {
            info = new CoverageInfo(((ICoverageInfo) child).getCoverageInfo());
        }
    }

    @Override
    public CoverageInfo getCoverageInfo() {
        return info;
    }

    public boolean isScalarExpr()
    {
        return scalarExpr;
    }

    public ScalarExpr getScalarExpr()
    {
        ScalarExpr r = null;
        if (scalarExpr)
            r = (ScalarExpr) child;
        return r;
    }

    public IRasNode getChild() {
        return child;
    }

    @Override
    public String toRasQL() {
        if (simpleCoverage) {
            return childInfo;
        } else {
            return child.toRasQL();
        }
    }
};
