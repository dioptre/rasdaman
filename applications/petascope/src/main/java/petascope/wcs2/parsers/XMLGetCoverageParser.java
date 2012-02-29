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

import petascope.wcs2.handlers.RequestHandler;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSlice;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionTrim;
import java.util.List;
import nu.xom.Element;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import petascope.exceptions.PetascopeException;
import static petascope.util.XMLUtil.*;
import static petascope.util.XMLSymbols.*;
import petascope.util.CrsUtil;
import petascope.util.TimeUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Parse a GetCapabilities XML request.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class XMLGetCoverageParser extends XMLParser<GetCoverageRequest> {

    private static final Logger log = LoggerFactory.getLogger(XMLGetCoverageParser.class);
    
    @Override
    public GetCoverageRequest parse(String input) throws WCSException {
        Element root = parseInput(input);
        List<Element> coverageIds = collectAll(root, PREFIX_WCS,
                LABEL_COVERAGE_ID, CTX_WCS);
        if (coverageIds.size() != 1) {
            throw new WCSException(ExceptionCode.InvalidRequest,
                    "A GetCoverage request can specify only one CoverageId");
        }
        GetCoverageRequest ret = new GetCoverageRequest(getText(coverageIds.get(0)));
        List<Element> children = ch(root);
        for (Element e : children) {
            String name = e.getLocalName();
            List<Element> c = ch(e);
            try {
                if (name.equals(LABEL_DIMENSION_TRIM)) {
                    ret.getSubsets().add(new DimensionTrim(getText(c.get(0)), getText(c.get(1)), getText(c.get(2))));
                    // Check timestamps validity
                    if (getText(c.get(0)).equalsIgnoreCase("T") || getText(c.get(0)).equalsIgnoreCase("TEMPORAL")) {
                        if (getText(c.get(1)) != null && !TimeUtil.isValidTimestamp(getText(c.get(1)))) {
                            throw new WCSException(ExceptionCode.InvalidParameterValue, "Timestamp \"" + getText(c.get(1)) + "\" is not valid (pattern is YYYY-MM-DD).");
                        }
                        if (getText(c.get(2)) != null && !TimeUtil.isValidTimestamp(getText(c.get(2)))) {
                            throw new WCSException(ExceptionCode.InvalidParameterValue, "Timestamp \"" + getText(c.get(2)) + "\" is not valid (pattern is YYYY-MM-DD).");
                        }
                    }
                } else if (name.equals(LABEL_DIMENSION_SLICE)) {
                    ret.getSubsets().add(new DimensionSlice(getText(c.get(0)), getText(c.get(1))));
                    // Check timestamps validity
                    if (getText(c.get(0)).equalsIgnoreCase("T") || getText(c.get(0)).equalsIgnoreCase("TEMPORAL")) {
                        if (getText(c.get(1)) != null && !TimeUtil.isValidTimestamp(getText(c.get(1)))) {
                            throw new WCSException(ExceptionCode.InvalidParameterValue, "Timestamp \"" + getText(c.get(1)) + "\" is not valid (pattern is YYYY-MM-DD).");
                        }
                    }
                } else if (name.equals(LABEL_CRS)) {
                    String subCrs=null, outCrs=null;
                    for (Element attr : c) {
                        if (attr.getLocalName().equals(ATT_SUBSET_CRS)) {
                            if (subCrs == null) subCrs = getText(attr);
                            else throw new WCSException(ExceptionCode.InvalidRequest, "Multiple \"subsettingCrs\" parameters in the request: must be unique.");
                            // check validity of CRS specification
                            if (!CrsUtil.isValidCrsCode(subCrs)) throw new WCSException(ExceptionCode.NotASubsettingCrs, "subsettingCrs \"" + subCrs + "\" is not valid.");
                            if (!CrsUtil.isSupportedCrsCode(subCrs)) throw new WCSException(ExceptionCode.SubsettingCrsNotSupported, "subsettingCrs " + subCrs + " is not supported.");
                        }
                        else if (attr.getLocalName().equals(ATT_OUTPUT_CRS)) {
                            if (outCrs == null) outCrs = getText(attr);
                            else throw new WCSException(ExceptionCode.InvalidRequest, "Multiple \"outputCrs\" parameters in the request: must be unique.");
                            // check validity of CRS specification
                            if (!CrsUtil.isValidCrsCode(outCrs)) throw new WCSException(ExceptionCode.NotAnOutputCrs, "outputCrs \"" + outCrs + "\" is not valid.");
                            if (!CrsUtil.isSupportedCrsCode(outCrs)) throw new WCSException(ExceptionCode.SubsettingCrsNotSupported, "outputCrs " + outCrs + " is not supported.");
                        }
                        else log.warn("\"" + attr.getLocalName() + "\" unknown attribute of CRS element while parsing XML GetCoverage request");
                    }
                    if (ret.getCRS().size() == 1) log.warn("Repeated CRS item inside XML GetCoverage request: discard it.");
                    else ret.getCRS().add(new GetCoverageRequest.CRS(subCrs, outCrs));
                }
                
            } catch (Exception ex) {
                if (((PetascopeException)ex).getExceptionCode().getExceptionCode().equalsIgnoreCase(ExceptionCode.NotASubsettingCrs.getExceptionCode()))
                    throw (WCSException)ex;
                else if (((PetascopeException)ex).getExceptionCode().getExceptionCode().equalsIgnoreCase(ExceptionCode.NotAnOutputCrs.getExceptionCode()))
                    throw (WCSException)ex;
                else 
                    throw new WCSException(ExceptionCode.InvalidRequest, "Error parsing dimension subset:\n\n" + e.toXML(), ex);
            }
        }
        return ret;
    }

    @Override
    public String getOperationName() {
        return RequestHandler.GET_COVERAGE;
    }
}
