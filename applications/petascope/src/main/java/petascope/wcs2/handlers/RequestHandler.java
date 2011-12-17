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

import petascope.exceptions.WCSException;

/**
 * Handles a {@link Request}
 * 
 * @author Dimitar Misev
 */
public interface RequestHandler<T> {
    
    String GET_CAPABILITIES = "GetCapabilities";
    String DESCRIBE_COVERAGE = "DescribeCoverage";
    String GET_COVERAGE = "GetCoverage";

    /**
     * Execute the operation given the request, and return the result.
     */
    public Response handle(T request) throws WCSException;
}
