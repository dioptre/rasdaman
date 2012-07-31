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
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.*;
import petascope.util.CrsUtil;
import petascope.util.WCPSConstants;

public class ConstantCoverageExpr implements IRasNode, ICoverageInfo {

    private static Logger log = LoggerFactory.getLogger(ConstantCoverageExpr.class);

    private String covName;
    private Vector<AxisIterator> iterators;
    private ConstantList valueList;
    private CoverageInfo info;
    private String axisIteratorString;
    private int requiredListSize = 1;

    public ConstantCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException {
        while ((node != null) && node.getNodeName().equals("#" + WCPSConstants.MSG_TEXT)) {
            node = node.getNextSibling();
        }
        log.trace(node.getNodeName());

        iterators = new Vector();

        while (node != null) {
            String name = node.getNodeName();
            if (name.equals(WCPSConstants.MSG_NAME)) {
                covName = node.getTextContent();
                log.trace("  " + WCPSConstants.MSG_COVERAGE + " " + covName);
            } else if (name.equals(WCPSConstants.MSG_AXIS_ITERATOR )) {
                log.trace("  " + WCPSConstants.MSG_ADD_AXIS_ITERATOR);
                AxisIterator it = new AxisIterator(node.getFirstChild(), xq, WCPSConstants.MSG_TEMP);
                iterators.add(it);
            } else {
                log.trace("  " + WCPSConstants.MSG_VALUE_LISt);
                valueList = new ConstantList(node, xq);
                node = valueList.getLastNode();
            }

            node = node.getNextSibling();
            while ((node != null) && node.getNodeName().equals("#" + WCPSConstants.MSG_TEXT)) {
                node = node.getNextSibling();
            }
        }

        try {
            buildMetadata(xq);
        } catch (PetascopeException ex) {
            throw new WCPSException(WCPSConstants.ERRTXT_CANNOT_BUILD_COVERAGE+ " !!!");
        }
        buildAxisIteratorDomain();

        // Sanity check: dimensions should match number of constants in the list
        if (valueList.getSize() != requiredListSize) {
            throw new WCPSException(WCPSConstants.ERRTXT_CONST_DIMS_DOESNOT_MATCH);
        }
        // Sanity check: metadata should have already been build
        if (info == null) {
            throw new WCPSException(WCPSConstants.ERRTXT_COULD_BUILD_CONST_COVERAGE);
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
    }

    /** Builds full metadata for the newly constructed coverage **/
    private void buildMetadata(XmlQuery xq) throws WCPSException, PetascopeException {
        log.trace("  " + WCPSConstants.MSG_BUILDING_METADATA);
        List<CellDomainElement> cellDomainList = new LinkedList<CellDomainElement>();
        List<RangeElement> rangeList = new LinkedList<RangeElement>();
        HashSet<String> nullSet = new HashSet<String>();
        String nullDefault = "0";
        nullSet.add(nullDefault);
        HashSet<InterpolationMethod> interpolationSet = new HashSet<InterpolationMethod>();
        InterpolationMethod interpolationDefault = new InterpolationMethod(WCPSConstants.MSG_NONE, WCPSConstants.MSG_NONE);
        interpolationSet.add(interpolationDefault);
        String coverageName = covName;
        List<DomainElement> domainList = new LinkedList<DomainElement>();

        Iterator<AxisIterator> i = iterators.iterator();
        while (i.hasNext()) {
            // Build domain metadata
            AxisIterator ai = i.next();
            String axisName = ai.getVar();
            String axisType = ai.getAxisType();
            cellDomainList.add(new CellDomainElement(ai.getLow(), ai.getHigh(), axisType));
            String crs = CrsUtil.IMAGE_CRS;
            HashSet<String> crsset = new HashSet<String>();
            crsset.add(crs);
            DomainElement domain = new DomainElement(axisName, axisType,
                    ai.getLow().doubleValue(), ai.getHigh().doubleValue(),
                    null, null, crsset, xq.getMetadataSource().getAxisNames(), null); // FIXME uom = null
            domainList.add(domain);
        }

        // TODO: check element datatypes and their consistency
        // "unsigned int" is default datatype
        rangeList.add(new RangeElement(WCPSConstants.MSG_DYNAMIC_TYPE, WCPSConstants.MSG_UNSIGNED_INT, null));
        Metadata metadata = new Metadata(cellDomainList, rangeList, nullSet,
                nullDefault, interpolationSet, interpolationDefault,
                coverageName, WCPSConstants.MSG_GRID_COVERAGE, domainList, null); // FIXME
        // Let the top-level query know the full metadata about us
        xq.getMetadataSource().addDynamicMetadata(covName, metadata);
        info = new CoverageInfo(metadata);
    }
}
