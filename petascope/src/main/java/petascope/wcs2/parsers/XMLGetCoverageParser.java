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
package petascope.wcs2.parsers;

import petascope.wcs2.handlers.RequestHandler;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSlice;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionTrim;
import java.util.List;
import nu.xom.Element;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import static petascope.util.XMLUtil.*;
import static petascope.util.XMLSymbols.*;

/**
 * Parse a GetCapabilities XML request.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class XMLGetCoverageParser extends XMLParser<GetCoverageRequest> {

    @Override
    public GetCoverageRequest parse(String input) throws WCSException {
        Element root = parseInput(input);
        List<Element> coverageIds = collectAll(root, PREFIX_WCS,
                LABEL_COVERAGE_ID, CTX_WCS);
        if (coverageIds.size() != 1) {
            throw new WCSException(ExceptionCode.InvalidRequest,
                    "A GetCoverage request can specify only one CoverageId");
        }
        GetCoverageRequest ret = new GetCoverageRequest(getText(coverageIds.get(0)));
        List<Element> children = ch(root);
        for (Element e : children) {
            String name = e.getLocalName();
            List<Element> c = ch(e);
            try {
                if (name.equals(LABEL_DIMENSION_TRIM)) {
                    ret.getSubsets().add(new DimensionTrim(getText(c.get(0)), getText(c.get(1)), getText(c.get(2))));
                } else if (name.equals(LABEL_DIMENSION_SLICE)) {
                    ret.getSubsets().add(new DimensionSlice(getText(c.get(0)), getText(c.get(1))));
                }
            } catch (Exception ex) {
                throw new WCSException(ExceptionCode.InvalidRequest,
                        "Error parsing dimension subset:\n\n" + e.toXML(), ex);
            }
        }
        return ret;
    }

    @Override
    public String getOperationName() {
        return RequestHandler.GET_COVERAGE;
    }
}
