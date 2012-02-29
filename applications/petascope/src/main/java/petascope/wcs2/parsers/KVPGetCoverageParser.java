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


import java.util.HashMap;
import java.util.Map;
import petascope.util.StringUtil;
import petascope.wcs2.handlers.RequestHandler;
import java.util.List;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import petascope.util.ListUtil;
import petascope.util.CrsUtil;
import petascope.util.TimeUtil;
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

    /**
     * Parses any subset parameters defined as in OGC 09-147r1 standard(e.g. ...&subset=x(200,300)&subset=y(300,200))
     * and for backwards compatibility subsets defined as subsetD(where D is any distinct string)(e.g. &subsetA=x(200,300))
     * @param requestParams - the request parameters as a string
     * @return ret - a hashmap containing the subsets
     */
    public HashMap<String, String> parseSubsetParams(String request) {
        HashMap<String, String> ret = new HashMap<String, String>();
        StringTokenizer st = new StringTokenizer(request, "&");
        while (st.hasMoreTokens()) {
            String kvPair = (String) st.nextToken();
            int splitPos = kvPair.indexOf("=");
            if (splitPos != -1) {
                String key = kvPair.substring(0, splitPos);
                String value = kvPair.substring(splitPos + 1);
                if (key.equalsIgnoreCase("subset")) {
                    ret.put(key + value, value);
                }
                //Backward compatibility
                else if (key.toLowerCase().startsWith("subset") 
                        && !key.equalsIgnoreCase("subsettingCrs")) {
                    ret.put(key + value, value);
                }
            }
        }

        return ret;
    }

    @Override
    public GetCoverageRequest parse(String input) throws WCSException {
        Map<String, List<String>> p = StringUtil.parseQuery(input);
        checkEncodingSyntax(p, "coverageid", "version", "mediatype", "format", "subsettingcrs", "outputcrs");
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

        /* CRS-extension parameters: */
        // subsettingCrs
        String subCrs=null, outCrs=null;
        List<String> list = p.get("subsettingcrs");
        if (list != null && list.size() > 1)
            throw new WCSException(ExceptionCode.InvalidRequest, "Multiple \"subsettingCrs\" parameters in the request: must be unique.");
        else {
            subCrs = ListUtil.head(list);
            if (!(subCrs == null) && !CrsUtil.isValidCrsCode(subCrs))
                throw new WCSException(ExceptionCode.NotASubsettingCrs, "subsettingCrs " + subCrs + " is not valid.");
            if (!(subCrs == null) && !CrsUtil.isSupportedCrsCode(subCrs))
                throw new WCSException(ExceptionCode.SubsettingCrsNotSupported, "subsettingCrs " + subCrs + " is not supported.");
        }
        // outputCrs
        list = p.get("outputcrs");
        if (list != null && list.size() > 1)
            throw new WCSException(ExceptionCode.InvalidRequest, "Multiple \"outputCrs\" parameters in the request: must be unique.");
        else {
            outCrs = ListUtil.head(list);
            if (!(outCrs == null) && !CrsUtil.isValidCrsCode(outCrs))
                throw new WCSException(ExceptionCode.NotAnOutputCrs, "outputCrs " + outCrs + " is not valid.");
            if (!(outCrs == null) && !CrsUtil.isSupportedCrsCode(outCrs))
                throw new WCSException(ExceptionCode.SubsettingCrsNotSupported, "outputCrs " + outCrs + " is not supported.");
        }        
        if (!(subCrs==null) || !(outCrs==null)) 
            ret.getCRS().add(new GetCoverageRequest.CRS(subCrs, outCrs));
        
        HashMap<String, String> subsets = parseSubsetParams(input);
        for (Map.Entry<String, String> subset : subsets.entrySet()) {
                String subsetKey = (String)subset.getKey();
                String subsetValue = (String)subset.getValue();                
                Matcher matcher = PATTERN.matcher(subsetValue);
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
                        throw new WCSException(ExceptionCode.InvalidEncodingSyntax.locator(subsetKey));
                    }
                    
                    // Check time-subset validity (YYYY-MM-DD)
                    if (dim.equalsIgnoreCase("T") || dim.equalsIgnoreCase("TEMPORAL")) {
                        if (low != null && !TimeUtil.isValidTimestamp(low)) {
                            throw new WCSException(ExceptionCode.InvalidParameterValue, "Timestamp \"" + low + "\" is not valid (pattern is YYYY-MM-DD).");
                        }
                        if (high != null && !TimeUtil.isValidTimestamp(high)) {
                            throw new WCSException(ExceptionCode.InvalidParameterValue, "Timestamp \"" + high + "\" is not valid (pattern is YYYY-MM-DD).");
                        }
                    }
                } else {
                    throw new WCSException(ExceptionCode.InvalidEncodingSyntax.locator(subsetKey));
                }
        }

        return ret;
    }

    @Override
    public String getOperationName() {
        return RequestHandler.GET_COVERAGE;
    }
}
