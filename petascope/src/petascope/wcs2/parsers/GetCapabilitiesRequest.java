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

/**
 * A GetCapabilities request object, populated by a parser.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class GetCapabilitiesRequest extends Request {

    /**
     * Service name, fixed to WCS
     */
    public static final String SERVICE = "WCS";

    private final String acceptVersion;
    private final String acceptFormats;
    private final String acceptLanguages;

    public GetCapabilitiesRequest(String acceptVersion, String acceptFormats, String acceptLanguages) {
        this.acceptVersion = (acceptVersion == null) ? "2.0" : acceptVersion;
        this.acceptFormats = (acceptFormats == null) ? "text/xml" : acceptFormats;
        this.acceptLanguages = (acceptLanguages == null) ? "en" : acceptLanguages;
    }

    public String getAcceptVersion() {
        return acceptVersion;
    }

    public String getAcceptFormats() {
        return acceptFormats;
    }

    public String getAcceptLanguages() {
        return acceptLanguages;
    }

    @Override
    public String toString() {
        return "GetCapabilitiesRequest: " + "acceptVersion=" + acceptVersion + ", acceptFormats=" + 
                acceptFormats + ", acceptLanguages=" + acceptLanguages;
    }
}
