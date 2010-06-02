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
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.WCPSException;
import petascope.wcps.server.exceptions.InvalidMetadataException;
import java.math.BigInteger;
import java.util.Collection;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import org.w3c.dom.*;

public class ScalarExpr implements IRasNode, ICoverageInfo {

    private IRasNode child;
    private CoverageInfo info;
    private boolean singleNumericValue = false;
    private double dvalue;

    public ScalarExpr(Node node, XmlQuery xq) throws WCPSException, InvalidCrsException {
        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        // Try one of the groups
        child = null;

//      TODO: Implement class MetadataScalarExprType
//      MetadataScalarExprType
        if (child == null) {
            try {
                child = new MetadataScalarExpr(node, xq);
                System.err.println("Matched metadata scalar expression.");
            } catch (WCPSException e) {
                System.err.println("Failed to match metadata scalar expression: "
                        + e.toString() + "\nRetrying");
                child = null;
            }
        }

//            BooleanScalarExprType
        if (child == null) {
            try {
                child = new BooleanScalarExpr(node, xq);
                System.err.println("Matched boolean scalar expression.");
            } catch (WCPSException e) {
                System.err.println("Failed to match boolean scalar expression : "
                        + e.toString() + "\nRetrying");
                child = null;
            }
        }

//            NumericScalarExprType
        if (child == null) {
            try {
                child = new NumericScalarExpr(node, xq);
                singleNumericValue = ((NumericScalarExpr) child).isSingleValue();
                dvalue = ((NumericScalarExpr) child).getSingleValue();
                System.err.println("Matched numeric scalar expression.");
            } catch (WCPSException e) {
                System.err.println("Failed to match numeric scalar expression : "
                        + e.toString() + "\nRetrying");
                child = null;
            }
        }

//            ReduceScalarExprType
        if (child == null) {
            try {
                child = new ReduceScalarExpr(node, xq);
                System.err.println("Matched reduce scalar expression.");
            } catch (WCPSException e) {
                System.err.println("Failed to match reduce scalar expression: "
                        + e.toString() + "\nRetrying");
                child = null;
            }
        }

//            StringScalarExprType
        if (child == null) {
            try {
                child = new StringScalarExpr(node, xq);
                System.err.println("Matched string scalar expression.");
            } catch (WCPSException e) {
                System.err.println("Failed to match string scalar expression: "
                        + e.toString() + "\nRetrying");
                child = null;
            }
        }

        // Error check
        if (child == null) {
            throw new WCPSException("Invalid coverage Expression, next node: "
                    + node.getNodeName());
        }

        Metadata meta = createScalarExprMetadata(xq);
        info = new CoverageInfo(meta);
    }

    public String toRasQL() {
        return child.toRasQL();
    }

    public CoverageInfo getCoverageInfo() {
        return info;
    }

    /** Builds full metadata for the newly constructed coverage **/
    private Metadata createScalarExprMetadata(XmlQuery xq) throws WCPSException {
        List<CellDomainElement> cellDomainList = new LinkedList<CellDomainElement>();
        List<RangeElement> rangeList = new LinkedList<RangeElement>();
        HashSet<String> nullSet = new HashSet<String>();
        String nullDefault = "0";
        nullSet.add(nullDefault);
        HashSet<InterpolationMethod> interpolationSet = new HashSet<InterpolationMethod>();
        InterpolationMethod interpolationDefault = new InterpolationMethod("none", "none");
        interpolationSet.add(interpolationDefault);
        String coverageName = "scalarExpr";
        List<DomainElement> domainList = new LinkedList<DomainElement>();

        // Build domain metadata
        cellDomainList.add(new CellDomainElement(new BigInteger("1"), new BigInteger("1")));
        String crs = DomainElement.IMAGE_CRS;
        HashSet<String> crsset = new HashSet<String>();
        crsset.add(crs);
        Collection<String> allowedAxes = xq.getMetadataSource().getAxisNames();
        DomainElement domain = new DomainElement("x", "x", 1.0, 1.0, null, null, crsset, allowedAxes);
        domainList.add(domain);
        // "unsigned int" is default datatype
        rangeList.add(new RangeElement("dynamic_type", "unsigned int"));

        try {
            Metadata metadata = new Metadata(cellDomainList, rangeList, nullSet,
                    nullDefault, interpolationSet, interpolationDefault,
                    coverageName, domainList, null);
            return metadata;
        } catch (InvalidMetadataException e) {
            throw new WCPSException("Could not build metadata for scalar expression !", e);
        }
    }

    public boolean isSingleValue() {
        return singleNumericValue;
    }

    public double getSingleValue() {
        return dvalue;
    }
}
