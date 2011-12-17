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
package petascope.wcs2.handlers;

import petascope.wcs2.parsers.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.core.DbMetadataSource;

/**
 * Parsers are managed in this class.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class HandlersRegistry {

    private static final Logger log = LoggerFactory.getLogger(HandlersRegistry.class);

    /**
     * @return a binding for the specified operation, that can parse the specified input, or null otherwise
     */
    public static RequestHandler getHandler(Request request, DbMetadataSource meta) {
        if (request instanceof GetCapabilitiesRequest) {
            return new GetCapabilitiesHandler(meta);
        } else if (request instanceof DescribeCoverageRequest) {
            return new DescribeCoverageHandler(meta);
        } else if (request instanceof GetCoverageRequest) {
            return new GetCoverageHandler(meta);
        }
        return null;
    }
}
