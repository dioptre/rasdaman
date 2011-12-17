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
import petascope.exceptions.WCSException;
import petascope.wcs2.parsers.GetCoverageRequest;
import petascope.wcs2.handlers.Response;

/**
 * Applies specific formatting to the result.
 * 
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public interface FormatExtension extends  Extension {
    
    String MIME_TEXT = "text/plain";
    String MIME_MULTIPART = "multipart/mixed";
    String MIME_GML = "application/gml+xml";
    String MIME_TIFF = "image/tiff";
    
    String CSV_ENCODING = "csv";
    String TIFF_ENCODING = "tiff";
    String PNG_ENCODING = "png";
    
    /**
     * Check if this format extension is the right one for req
     * 
     * @param req GetCoverage request
     * @return true if this extension can encode the response for req according to the format, or false otherwise
     */
    boolean canHandle(GetCoverageRequest req);
    
    /**
     * Return a properly encoded response, given the request req.
     * 
     * @param req GetCoverage request
     * @param meta database metadata
     * @return response encoded in the respective format
     */
    Response handle(GetCoverageRequest req, DbMetadataSource meta) throws WCSException;
}
