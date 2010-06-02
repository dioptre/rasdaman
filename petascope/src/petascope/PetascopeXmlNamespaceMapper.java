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
package petascope;

//~--- JDK imports ------------------------------------------------------------
import com.sun.xml.bind.marshaller.NamespacePrefixMapper;

public class PetascopeXmlNamespaceMapper extends NamespacePrefixMapper {

    @Override
    public String getPreferredPrefix(String uri, String id, boolean req) {
        if (uri.equals("") || uri.equals("http://www.opengis.net/wcs/1.1")) {
            return "wcs";
        }
        if (uri.equals("http://www.w3.org/2001/SMIL20/Language")) {
            return "smil1";
        }
        if (uri.equals("http://www.w3.org/2001/SMIL20/")) {
            return "smil20";
        }
        if (uri.equals("http://www.opengis.net/gml")) {
            return "gml";
        }
        if (uri.equals("http://www.w3.org/1999/xlink")) {
            return "xlink";
        }
        if (uri.equals("http://www.opengis.net/wcs/1.1/ows")) {
            return "owcs";
        }
        if (uri.equals("http://www.opengis.net/ows")
                || uri.equals("http://www.opengis.net/ows/1.1")) {
            return "ows";
        }
        if (uri.equals("http://www.w3.org/2001/XMLSchema-instance")) {
            return "xsd";
        }
        if (uri.equals("http://www.opengis.net/wcs/1.1/wcst")) {
            return "wcst";
        }

        return null;

    }
}
