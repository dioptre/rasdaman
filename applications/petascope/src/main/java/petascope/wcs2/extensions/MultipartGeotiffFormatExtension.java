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
package petascope.wcs2.extensions;

import petascope.core.DbMetadataSource;
import petascope.exceptions.WCSException;
import petascope.wcs2.handlers.Response;
import petascope.wcs2.parsers.GetCoverageMetadata;
import petascope.wcs2.parsers.GetCoverageRequest;

/**
 * Return coverage as GML + GeoTIFF.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class MultipartGeotiffFormatExtension extends  GmlFormatExtension {
    
    @Override
    public boolean canHandle(GetCoverageRequest req) {
        return req.isMultipart() && MIME_TIFF.equals(req.getFormat());
    }

    @Override
    public Response handle(GetCoverageRequest req, DbMetadataSource meta) throws WCSException {
        // get gml response, but without the {coverageData} replaced
        Response gml = super.handle(req, meta);
        // get the GeoTIFF file
        Response tiff = ExtensionsRegistry.getFormatExtension(false, req.getFormat()).handle(req, meta);
        // return multipart response
        String xml = gml.getXml().replace("{coverageData}",
                   "<File>"
                + "<fileName>" + "file" + "</fileName>"
                + "<fileStructure>Record Interleaved</fileStructure>"
                + "<mimeType>" + MIME_TIFF + "</mimeType>"
                + "</File>");
        return new Response(tiff.getData(), xml, MIME_TIFF);
    }

    @Override
    protected String addCoverageData(String gml, GetCoverageRequest request, DbMetadataSource meta, GetCoverageMetadata m)
            throws WCSException {
        return gml;
    }

    @Override
    public String getExtensionIdentifier() {
        return ExtensionsRegistry.GEOTIFF_IDENTIFIER;
    }
}
