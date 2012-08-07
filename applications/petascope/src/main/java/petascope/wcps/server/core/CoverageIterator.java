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

import petascope.core.IDynamicMetadataSource;
import petascope.exceptions.PetascopeException;
import petascope.exceptions.WCPSException;
import org.w3c.dom.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.WCPSConstants;

public class CoverageIterator implements IRasNode {

    private static Logger log = LoggerFactory.getLogger(CoverageIterator.class);

    private List<String> coverageNames;
    private String iteratorName;
    private boolean dynamic = false;    // created from a Construct Coverage expr?

    public CoverageIterator(Node x, XmlQuery xq) throws WCPSException, PetascopeException {
        log.trace(x.getNodeName());
        coverageNames = new ArrayList<String>();
        if (!x.getNodeName().equals(WCPSConstants.MSG_COVERAGE_ITERATOR)) {
            log.error(WCPSConstants.ERRTXT_INVALID_CAST_FROM_Part1 + " " + x.getNodeName() + " " + WCPSConstants.ERRTXT_INVALID_CAST_FROM_PART2);
            throw new WCPSException(WCPSConstants.ERRTXT_INVALID_CAST_FROM_Part1 + " " + x.getNodeName()
                    + " " + WCPSConstants.ERRTXT_INVALID_CAST_FROM_PART2);
        }

        Node it = x.getFirstChild();
        while (it != null) {
            if (it.getNodeName().equals("#" + WCPSConstants.MSG_TEXT)) {
                it = it.getNextSibling();
                continue;
            }

            if (it.getNodeName().equals(WCPSConstants.MSG_ITERATORVAR)) {
                iteratorName = it.getFirstChild().getNodeValue();
                log.trace("  " + WCPSConstants.MSG_ITERATOR_VARIABLE + " : " + iteratorName);
            } else if (it.getNodeName().equals(WCPSConstants.MSG_COVERAGE_NAME)) {
                String cn = it.getFirstChild().getNodeValue();
                log.trace("  " + WCPSConstants.MSG_COVERAGE_REFERENCE + " : " + cn);
                if (!xq.getMetadataSource().coverages().contains(cn)) {
                    log.error("  " + WCPSConstants.ERRTXT_UNKNOWN_COVERAGE + " " + cn);
                    throw new WCPSException(WCPSConstants.ERRTXT_UNKNOWN_COVERAGE + " " + cn);
                }

                coverageNames.add(cn);
            }

            it = it.getNextSibling();
        }
    }

    public CoverageIterator(String iterator, String coverage) throws WCPSException {
        log.trace(WCPSConstants.MSG_ITERATOR + ": " + iterator + ", " + WCPSConstants.MSG_FOR_COVERAGE + ": " + coverage);
        coverageNames = new ArrayList<String>();
        iteratorName = iterator;
        coverageNames.add(coverage);
        this.dynamic = true;
    }

    public Iterator<String> getCoverages() {
        return coverageNames.iterator();
    }

    public String getIteratorName() {
        return iteratorName;
    }

    public String toRasQL() {
        // TODO(andreia) : How to translate multiple coverages?
        return coverageNames.get(0) + " " + WCPSConstants.MSG_AS + " " + iteratorName;
    }
}
