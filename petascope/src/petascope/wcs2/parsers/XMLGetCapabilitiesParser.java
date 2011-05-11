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
import nu.xom.Node;
import nu.xom.Element;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WCSException;
import static petascope.util.XMLUtil.*;
import static petascope.util.XMLSymbols.*;

/**
 * Parse a GetCapabilities XML request.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class XMLGetCapabilitiesParser extends XMLParser<GetCapabilitiesRequest> {
    
    @Override
    public GetCapabilitiesRequest parse(String input) throws WCSException {
        Element root = parseInput(input);

        return new GetCapabilitiesRequest(
                childrenToString(firstChildRecursive(root, LABEL_ACCEPT_VERSIONS)),
                childrenToString(firstChildRecursive(root, LABEL_ACCEPT_FORMATS)),
                childrenToString(firstChildRecursive(root, LABEL_ACCEPT_LANGUAGES)));
    }

    private String childrenToString(Element e) {
        if (e == null) {
            return null;
        }
        String ret = "";
        for (int i = 0; i < e.getChildCount(); i++) {
            Node n = e.getChild(i);
            if (n instanceof Element) {
                ret += getText((Element)n) + ",";
            }
        }
        return ret.substring(0, ret.length() - 1);
    }

    @Override
    public String getOperationName() {
        return RequestHandler.GET_CAPABILITIES;
    }
}
