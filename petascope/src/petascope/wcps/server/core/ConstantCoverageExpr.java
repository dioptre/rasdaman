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

import petascope.core.Metadata;
import petascope.exceptions.PetascopeException;
import petascope.exceptions.WCPSException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Vector;
import org.w3c.dom.*;

public class ConstantCoverageExpr implements IRasNode, ICoverageInfo {

    private String covName;
    private Vector<AxisIterator> iterators;
    private ConstantList valueList;
    private CoverageInfo info;
    private String axisIteratorString;
    private int requiredListSize = 1;

    public ConstantCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException {
        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        iterators = new Vector();
        System.err.println("Parsing Constant Coverage Expr: " + node.getNodeName());

        while (node != null) {
            String name = node.getNodeName();
            if (name.equals("name")) {
                covName = node.getTextContent();
            } else if (name.equals("axisIterator")) {
                AxisIterator it = new AxisIterator(node.getFirstChild(), xq, "temp");
                iterators.add(it);
            } else {
                valueList = new ConstantList(node, xq);
                node = valueList.getLastNode();
            }

            node = node.getNextSibling();
            while ((node != null) && node.getNodeName().equals("#text")) {
                node = node.getNextSibling();
            }
        }

        try {
            buildMetadata(xq);
        } catch (PetascopeException ex) {
            throw new WCPSException("Cannot build coverage metadata !!!");
        }
        buildAxisIteratorDomain();

        // Sanity check: dimensions should match number of constants in the list
        if (valueList.getSize() != requiredListSize) {
            throw new WCPSException("The number of constants in the list do "
                    + "not match the dimensions specified !");
        }
        // Sanity check: metadata should have already been build
        if (info == null) {
            throw new WCPSException("Could not build constant coverage metadata !!!");
        }
    }

    public String toRasQL() {
        String result = "< ";
        result += axisIteratorString + " ";
        result += valueList.toRasQL();
        result += ">";

        return result;
    }

    public CoverageInfo getCoverageInfo() {
        return info;
    }

    /* Concatenates all the AxisIterators into one large multi-dimensional object,
     * that will be used to build to RasQL query. Also counts how many elements
     * fit in the specified dimensions. */
    private void buildAxisIteratorDomain() {
        requiredListSize = 1;
        axisIteratorString = "";
        axisIteratorString += "[";

        for (int i = 0; i < iterators.size(); i++) {
            if (i > 0) {
                axisIteratorString += ", ";
            }
            AxisIterator ai = iterators.elementAt(i);
            axisIteratorString += ai.getInterval();
            requiredListSize *= (ai.getHigh().intValue() - ai.getLow().intValue() + 1);
        }

        axisIteratorString += "]";

        System.err.println("Axes for ConstantCoverage tell us that the constant"
                + "list should have exactly " + requiredListSize + " elements !");
    }

    /** Builds full metadata for the newly constructed coverage **/
    private void buildMetadata(XmlQuery xq) throws WCPSException, PetascopeException {
        List<CellDomainElement> cellDomainList = new LinkedList<CellDomainElement>();
        List<RangeElement> rangeList = new LinkedList<RangeElement>();
        HashSet<String> nullSet = new HashSet<String>();
        String nullDefault = "0";
        nullSet.add(nullDefault);
        HashSet<InterpolationMethod> interpolationSet = new HashSet<InterpolationMethod>();
        InterpolationMethod interpolationDefault = new InterpolationMethod("none", "none");
        interpolationSet.add(interpolationDefault);
        String coverageName = covName;
        List<DomainElement> domainList = new LinkedList<DomainElement>();

        Iterator<AxisIterator> i = iterators.iterator();
        while (i.hasNext()) {
            // Build domain metadata
            AxisIterator ai = i.next();
            cellDomainList.add(new CellDomainElement(ai.getLow(), ai.getHigh()));
            String axisName = ai.getVar();
            String axisType = ai.getAxisType();
            String crs = DomainElement.WGS84_CRS;
            HashSet<String> crsset = new HashSet<String>();
            crsset.add(crs);
            DomainElement domain = new DomainElement(axisName, axisType,
                    ai.getLow().doubleValue(), ai.getHigh().doubleValue(),
                    null, null, crsset, xq.getMetadataSource().getAxisNames(), null); // FIXME uom = null
            domainList.add(domain);
        }

        // TODO: check element datatypes and their consistency
        // "unsigned int" is default datatype
        rangeList.add(new RangeElement("dynamic_type", "unsigned int", null));
        Metadata metadata = new Metadata(cellDomainList, rangeList, nullSet,
                nullDefault, interpolationSet, interpolationDefault,
                coverageName, "GridCoverage", domainList, null); // FIXME
        // Let the top-level query know the full metadata about us
        xq.getMetadataSource().addDynamicMetadata(covName, metadata);
        info = new CoverageInfo(metadata);
    }
}
