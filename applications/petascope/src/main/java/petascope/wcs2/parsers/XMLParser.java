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
package petascope.wcs2.parsers;

import nu.xom.ParsingException;
import nu.xom.Document;
import nu.xom.Element;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import petascope.util.XMLUtil;
import static petascope.util.XMLSymbols.*;

/**
 * An abstract superclass for XML/POST protocol binding extensions, which provides some
 * convenience methods to concrete implementations.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public abstract class XMLParser<T extends Request> extends AbstractRequestParser<T> {

    @Override
    public boolean canParse(String input) {
        return input != null && input.startsWith("<") && XMLUtil.isFirstTag(input, getOperationName());
    }

    protected Element parseInput(String input) throws WCSException {
        try {
            Document doc = XMLUtil.buildDocument(null, input);
            Element root = doc.getRootElement();

            String service = root.getAttributeValue(ATT_SERVICE);
            String version = root.getAttributeValue(ATT_VERSION);
            if ((service != null && !service.equals(BaseRequest.SERVICE)) ||
                    (version != null && !version.matches(BaseRequest.VERSION))) {
                throw new WCSException(ExceptionCode.VersionNegotiationFailed, "Service/Version not supported.");
            }

            return root;
        } catch (ParsingException ex) {
            throw new WCSException(ExceptionCode.XmlNotValid.locator(
                    "line: " + ex.getLineNumber() + ", column:" + ex.getColumnNumber()),
                    ex.getMessage(), ex);
        } catch (Exception ex) {
            throw new WCSException(ExceptionCode.XmlNotValid, ex.getMessage(), ex);
        }
    }
}
