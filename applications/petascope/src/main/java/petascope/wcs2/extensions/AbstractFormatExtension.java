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

import java.util.Iterator;
import petascope.core.DbMetadataSource;
import petascope.core.Metadata;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCPSException;
import petascope.exceptions.WCSException;
import petascope.util.Pair;
import petascope.util.ras.RasUtil;
import petascope.util.WcsUtil;
import petascope.util.ras.RasQueryResult;
import petascope.wcps.server.core.DomainElement;
import petascope.wcps.server.core.WCPS;
import petascope.wcs2.parsers.GetCoverageMetadata;
import petascope.wcs2.parsers.GetCoverageRequest;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSlice;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSubset;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionTrim;

/**
 *  An abstract implementation of {@link FormatExtension}, which provides some
 * convenience methods to concrete implementations.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public abstract class AbstractFormatExtension implements FormatExtension {

    protected static WCPS wcps;

    /**
     * Update m with the correct bounds and axes (mostly useful when there's slicing/trimming in the request)
     */
    protected void setBounds(GetCoverageRequest request, GetCoverageMetadata m, DbMetadataSource meta) throws WCSException {
        Pair<Object, String> pair = executeRasqlQuery(request, m, meta, "sdom", null);
        if (pair.fst != null) {
            RasQueryResult res = new RasQueryResult(pair.fst);
            if (!res.getScalars().isEmpty()) {
                // TODO: can be done better with Minterval instead of sdom2bounds
                Pair<String, String> bounds = WcsUtil.sdom2bounds(res.getScalars().get(0));
                m.setAxisLabels(pair.snd);
                m.setLow(bounds.fst);
                m.setHigh(bounds.snd);
            }
        }
    }
    
    /**
     * Execute rasql query, given GetCoverage request, request metadata, and format of result.
     * 
     * @return (result of executing the query, axes)
     * @throws WCSException 
     */
    protected Pair<Object, String> executeRasqlQuery(GetCoverageRequest request, 
            GetCoverageMetadata m, DbMetadataSource meta, String format, String params) throws WCSException {
        if (wcps == null) {
            try {
                wcps = new WCPS(meta);
            } catch (Exception ex) {
                throw new WCSException(ExceptionCode.InternalComponentError, "Error initializing WCPS engine", ex);
            }
        }

        Pair<String, String> pair = constructWcpsQuery(request, m.getMetadata(), format, params);
        String rquery = null;
        try {
            rquery = RasUtil.abstractWCPSToRasql(pair.fst, wcps);
        } catch (WCPSException ex) {
            throw new WCSException(ExceptionCode.WcpsError, "Error converting WCPS query to rasql query", ex);
        }
        Object res = null;
        try {
            if ("sdom".equals(format) && !rquery.contains(":")) {
                res = null;
            } else {
                res = RasUtil.executeRasqlQuery(rquery);
            }
        } catch (RasdamanException ex) {
            throw new WCSException(ExceptionCode.RasdamanRequestFailed, "Error executing rasql query", ex);
        }
        return Pair.of(res, pair.snd);
    }

    /**
     * Given a GetCoverage request, construct an abstract WCPS query.
     * @param req GetCoverage request
     * @param cov coverage metadata
     * @return (WCPS query in abstract syntax, axes)
     */
    protected Pair<String, String> constructWcpsQuery(GetCoverageRequest req, Metadata cov, String format, String params) {
        String axes = "";
        Iterator<DomainElement> dit = cov.getDomainIterator();
        while (dit.hasNext()) {
            axes += dit.next().getName() + " ";
        }
        String proc = "c";

        // process subsetting operations
        for (DimensionSubset subset : req.getSubsets()) {
            String dim = subset.getDimension();
            DomainElement de = cov.getDomainByName(dim);
            String crs = "CRS:1";
            if (subset.getCrs() != null) {
                crs = subset.getCrs();
            }

            if (subset instanceof DimensionTrim) {
                DimensionTrim trim = (DimensionTrim) subset;
                proc = "trim(" + proc + ",{" + dim + ":\"" + crs + "\" (" + trim.
                        getTrimLow() + ":" + trim.getTrimHigh() + ")})";
            } else if (subset instanceof DimensionSlice) {
                DimensionSlice slice = (DimensionSlice) subset;
                proc = "slice(" + proc + ",{" + dim + "(" + slice.getSlicePoint() + ")})";
                axes = axes.replaceFirst(dim + " ?", ""); // remove axis
            }
        }
        if (params != null) {
            format += ", \"" + params + "\"";
        }
        String query = "for c in (" + req.getCoverageId() + ") return encode(" + proc + ", \"" + format + "\")";
        return Pair.of(query, axes.trim());
    }
}
