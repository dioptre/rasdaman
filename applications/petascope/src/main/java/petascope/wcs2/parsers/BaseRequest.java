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

import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

/**
 * Contains the common stuff found in DescribeCoverage, GetCoverage.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class BaseRequest extends Request {

    /**
     * Service name, fixed to WCS
     */
    public static final String SERVICE = "WCS";

    /**
     * WCS version, fixed to 2.0
     */
    public static final String VERSION = "2\\.0\\.\\d+";
    public static final String VERSION_STRING = "2.0.0";

    /**
     * A placeholder for further request parameters defined by WCS extension standards
     */
    private final List<String> extension = new ArrayList<String>();

    public List<String> getExtension() {
        return extension;
    }
}
