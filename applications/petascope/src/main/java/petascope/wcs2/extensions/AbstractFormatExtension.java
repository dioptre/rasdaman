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
import petascope.util.CrsUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.TimeUtil;
import petascope.util.AxisTypes;
import petascope.wcps.server.core.CellDomainElement;

/**
 *  An abstract implementation of {@link FormatExtension}, which provides some
 * convenience methods to concrete implementations.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public abstract class AbstractFormatExtension implements FormatExtension {

    protected static WCPS wcps;
    private static final Logger log = LoggerFactory.getLogger(AbstractFormatExtension.class);

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
     * Request subsets values are pre-transformed if necessary (e.g. CRS reprojection, timestamps).
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

        // Convert human-readable **timestamp** to ANSI day number
        // NOTE: solution is not definitive!
        DimensionSubset tSubset = request.getSubset(AxisTypes.T_AXIS); // or TEMPORAL_AXIS?
        if (tSubset != null && !tSubset.isCrsTransformed()) {
                //&& m.getAxisLabels().contains(AxisTypes.T_AXIS)) {
            try {
                if (tSubset instanceof DimensionTrim) {
                    int dayLo = TimeUtil.convert2AnsiDay(((DimensionTrim)tSubset).getTrimLow());
                    int dayHi = TimeUtil.convert2AnsiDay(((DimensionTrim)tSubset).getTrimHigh());
                    // Check order:
                    if (dayLo > dayHi) {
                        log.error("Temporal subset order is wrong: lower bound is greater than upper-bound.");
                        throw new WCSException(ExceptionCode.InvalidParameterValue);
                    } else {
                        // Check intersection with coverage temporal domain
                        CellDomainElement tRange = m.getMetadata().getCellDomainByName(AxisTypes.T_AXIS);
                        if ((dayLo < tRange.getLo().intValue() && dayHi < tRange.getHi().intValue())          // t1,t2 < Tmin
                                || (dayLo > tRange.getHi().intValue() && dayHi > tRange.getHi().intValue())) {// t1,t2 > Tmax
                            log.error("Temporal subset does not intersect with coverage domain.");
                            throw new WCSException(ExceptionCode.InvalidParameterValue, "Temporal subset does not intersect with coverage temporal domain.");
                        } else {
                            ((DimensionTrim)tSubset).setTrimLow((double)dayLo);
                            ((DimensionTrim)tSubset).setTrimHigh((double)dayHi);
                        }
                    }
                } else if (request.getSubset(AxisTypes.TEMPORAL_AXIS) instanceof DimensionSlice) {
                    int day = TimeUtil.convert2AnsiDay(((DimensionSlice)tSubset).getSlicePoint());
                    // Check intersection with coverage temporal domain
                    CellDomainElement tRange = m.getMetadata().getCellDomainByName(AxisTypes.T_AXIS);
                    if (day < tRange.getLo().intValue() || day > tRange.getHi().intValue())
                        throw new WCSException(ExceptionCode.InvalidParameterValue, "Temporal subset does not intersect with coverage temporal domain.");
                    else ((DimensionSlice)tSubset).setSlicePoint((double)day);
                }
                
                // Set transformed to true
                if (tSubset != null) tSubset.isCrsTransformed(true);
                
            } catch (WCSException e) {
               throw e; 
            } catch (Exception e) {
                log.error("Error while converting WCS time subsetting to equivalent ANSI day numbers.\n" + e.getMessage());
                throw new WCSException(ExceptionCode.InternalComponentError);
            }
        }
        // Possible required CRS subsetting transforms have been done now, proceed to WCPS:
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
        /** NOTE: trims and slices are nested in each dimension: this inhibits WCPS 
         * subsetExpr to actually accept CRS != Native CRS of Image, since both 
         * X and Y coordinates need to be known at time of reprojection: CRS reprojection
         * is hence done a priori, but this does not hurt that much actually.
         */
        for (DimensionSubset subset : req.getSubsets()) {
            String dim = subset.getDimension();
            DomainElement de = cov.getDomainByName(dim);

            String crs = CrsUtil.IMAGE_CRS;
            // Subset-CRS might be embedded in a trimmig spec (~WCPS, e.g. KVP req) or with subsettingCrs attribute:
            if (subset.getCrs() != null || (req.getCRS().size() == 1 && req.getCRS().get(0).getSubsettingCrs() != null))
                crs = (subset.getCrs() != null) ? subset.getCrs() : req.getCRS().get(0).getSubsettingCrs();

            if (subset instanceof DimensionTrim) {
                DimensionTrim trim = (DimensionTrim) subset;
                proc = "trim(" + proc + ",{" + dim + ":\"" + crs + "\" (" + 
                        trim.getTrimLow() + ":" + trim.getTrimHigh() + ")})";
            } else if (subset instanceof DimensionSlice) {
                DimensionSlice slice = (DimensionSlice) subset;
                proc = "slice(" + proc + ",{" + dim + ":\"" + crs + "\" (" + slice.getSlicePoint() + ")})";
                axes = axes.replaceFirst(dim + " ?", ""); // remove axis
            }
        }
        if (params != null) {
            format += ", \"" + params + "\"";
        }
        
        // If outputCrs != Native CRS then add crsTrasform WCPS expression
        if (!format.equalsIgnoreCase("sdom") && !req.getCRS().get(0).getOutputCrs().equals(cov.getBbox().getCrsName())) {
            proc = "crsTransform(" + proc + ", { x:\"" + req.getCRS().get(0).getOutputCrs() + "\","
                    + "y:\"" + req.getCRS().get(0).getOutputCrs() + "\"}, { })";
            // TODO: manage interpolation formats list.
        }
        String query = "for c in (" + req.getCoverageId() + ") return encode(" + proc + ", \"" + format + "\")";
        return Pair.of(query, axes.trim());
    }
}