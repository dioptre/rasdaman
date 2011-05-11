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

import java.util.Map;
import petascope.util.StringUtil;
import petascope.wcs2.handlers.RequestHandler;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import petascope.util.ListUtil;
import petascope.wcs2.extensions.FormatExtension;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSlice;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionTrim;

/**
 * Parse a GetCapabilities KVP request.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class KVPGetCoverageParser extends KVPParser<GetCoverageRequest> {

    //                                                          dim=$1  crs=$2      low=$4  high=$5
    private static final Pattern PATTERN = Pattern.compile("([^,\\(]+)(,([^\\(]+))?\\(([^,\\)]+)(,([^\\)]+))?\\)");

    @Override
    public GetCoverageRequest parse(String input) throws WCSException {
        Map<String, List<String>> p = StringUtil.parseQuery(input);
        checkEncodingSyntax(p, "coverageid", "version");
        List<String> coverageIds = p.get("coverageid");
        if (coverageIds.size() != 1) {
            throw new WCSException(ExceptionCode.InvalidRequest,
                    "A GetCoverage request can specify only one CoverageId");
        }
        String mediaType = ListUtil.head(p.get("mediatype"));
        String format = ListUtil.head(p.get("format"));
        if (FormatExtension.MIME_MULTIPART.equals(mediaType) && FormatExtension.MIME_GML.equals(format)) {
            throw new WCSException(ExceptionCode.InvalidRequest,
                    "The 'MEDIATYPE=multipart/mixed & FORMAT=application/gml+xml' combination is not applicable");
        }
        GetCoverageRequest ret = new GetCoverageRequest(coverageIds.get(0), format,
                FormatExtension.MIME_MULTIPART.equals(mediaType));

        for (Map.Entry<String, List<String>> e : p.entrySet()) {
            String k = e.getKey();
            List<String> v = e.getValue();

            if (k.startsWith("subset")) {
                String s = ListUtil.ltos(v, ",");
                Matcher matcher = PATTERN.matcher(s);
                if (matcher.find()) {
                    String dim = matcher.group(1);
                    String crs = matcher.group(3);
                    String low = matcher.group(4);
                    String high = matcher.group(6);
                    if (high == null) {
                        ret.getSubsets().add(new DimensionSlice(dim, crs, low));
                    } else if (dim != null) {
                        ret.getSubsets().add(new DimensionTrim(dim, crs, low, high));
                    } else {
                        throw new WCSException(ExceptionCode.InvalidEncodingSyntax.locator(k));
                    }
                } else {
                    throw new WCSException(ExceptionCode.InvalidEncodingSyntax.locator(k));
                }
            }
        }

        return ret;
    }

    @Override
    public String getOperationName() {
        return RequestHandler.GET_COVERAGE;
    }
}
