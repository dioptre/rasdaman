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

import petascope.exceptions.WCSException;

/**
 * Parse a raw WCS 2.0 operation request into an appropriate request object.
 * 
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 * @param <T> type of parsed object
 */
public interface RequestParser<T extends Request> {
    
    /**
     * @return true if this parser can parse request, or false otherwise
     */
    boolean canParse(String request);

    /**
     * Parse request.
     *
     * @return bean containing parsed data
     * @throws WCSException
     */
    T parse(String request) throws WCSException;
    
    /**
     * @return this operations name, e.g. GetCapabilities
     */
    String getOperationName();
}
