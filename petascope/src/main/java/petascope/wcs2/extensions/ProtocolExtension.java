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
package petascope.wcs2.extensions;

import petascope.core.DbMetadataSource;
import petascope.wcs2.handlers.Response;
import petascope.exceptions.WCSException;
import petascope.util.Pair;
import petascope.wcs2.parsers.Request;

/**
 * WCS 2.0 protocol binding extension.
 * 
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public interface ProtocolExtension extends  Extension {
    
    /**
     * @return true if this protocol extension can handle the request, or false otherwise
     */
    boolean canHandle(String request);

    /**
     * Handle request. This involves parsing the raw request into a {@link Request}, then handling
     * this request object, and finally writing the result back.
     *
     * @param request request to execute
     * @return result from executing the requested operation
     * @throws WCSException
     */
    Response handle(String request, DbMetadataSource meta) throws WCSException;
}
