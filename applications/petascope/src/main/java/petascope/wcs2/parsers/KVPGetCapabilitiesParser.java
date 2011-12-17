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

import java.util.List;
import java.util.Map;
import petascope.wcs2.handlers.RequestHandler;
import petascope.exceptions.WCSException;
import petascope.util.StringUtil;

/**
 * Parse a GetCapabilities XML request.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class KVPGetCapabilitiesParser extends KVPParser<GetCapabilitiesRequest> {

    @Override
    public GetCapabilitiesRequest parse(String input) throws WCSException {
        Map<String, List<String>> p = StringUtil.parseQuery(input);
        checkEncodingSyntax(p, "acceptversions", "acceptformats",
                "acceptlanguages", "version");
        return new GetCapabilitiesRequest(
                get("acceptversions", p),
                get("acceptformats", p),
                get("acceptlanguages", p));
    }

    @Override
    public String getOperationName() {
        return RequestHandler.GET_CAPABILITIES;
    }
}
