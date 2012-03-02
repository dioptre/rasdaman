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
import java.util.List;
import petascope.core.DbMetadataSource;
import petascope.core.Metadata;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCPSException;
import petascope.exceptions.WCSException;
import petascope.exceptions.PetascopeException;
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
        /** NOTE1 (campalani): CRS transform cannot be done when only easting or
         *  only northing is known, so it must be done *outside* DimensionIntervalElement.
         *  Translation to pixel indices is moved outside DimensionInterval as well
         *  since it must be computed a posteriori of (possible) CRS transforms.
         *  NOTE2 (campalani): at the same time, CRS is read only inside DimensionInterval
         *  due to the syntax of WCPS language: trim(x:<crs>(xMin,xMax),y:<crs>(yMin,yMax).
         */
        // Check if a CRS transform is needed for X and Y axis.
        // TODO: embed this code into a serparate function (inside Crs/CrsUtil?).
        if (request.getCRS().size() == 1 && !m.getBbox().getCrsName().equals(CrsUtil.IMAGE_CRS)) {
            if (request.getSubset(AxisTypes.X_AXIS) == null && request.getSubset(AxisTypes.Y_AXIS) == null) {          // No subsetting at all was specified
                log.warn("A subsettingCrs is stated but no subsettings were found: ignore it.");
            } else {                
                DimensionSubset xSubset = request.getSubset(AxisTypes.X_AXIS);
                DimensionSubset ySubset = request.getSubset(AxisTypes.Y_AXIS);
                
                if (xSubset != null && !xSubset.isCrsTransformed()                  // SetBounds not called yet
                            || ySubset != null && !ySubset.isCrsTransformed()) {    // SetBounds not called yet (if no X subsetting was specified)
                        try {
                            if (request.getCRS().get(0).getSubsettingCrs() != null &&
                                    request.getCRS().get(0).getSubsettingCrs().equals(CrsUtil.IMAGE_CRS)) {
                                log.info("Subset(s) defined in pixel coordinates: no CRS reprojection need to be made.");
                                
                            } else if (request.getCRS().get(0).getSubsettingCrs() == null ||
                                    request.getCRS().get(0).getSubsettingCrs().equals(m.getBbox().getCrsName())) {
                                //if(!crs.getName().equals(DomainElement.WGS84_CRS)) {
                                log.info("Requested CRS (" + request.getCRS().get(0).getSubsettingCrs() + ") and native CRS coincide: no tranform needed.");
                                // Req7: /req/crs/getCoverage-subsettingCrs-default
                                if (request.getCRS().get(0).getSubsettingCrs()==null)
                                    request.getCRS().get(0).setSubsettingCrs(m.getBbox().getCrsName());
                                // Fill in crs of subsets, in case they were not embedded in the trimming expression of the request (es. KVP)
                                if (xSubset != null) xSubset.setCrs(m.getBbox().getCrsName());
                                if (ySubset != null) ySubset.setCrs(m.getBbox().getCrsName());
                            } else {
                                log.info("Requested CRS (" + request.getCRS().get(0).getSubsettingCrs() + ") and native CRS (" + m.getBbox().getCrsName() + ") differ: tranform needed.");

                                // Extrapolate values independently of Trim/Slice request
                                String[] subX, subY;
                                CrsUtil crsTool;
                                List<Double> temp;
                                if (xSubset == null) subX = new String[2];
                                else subX = (xSubset instanceof DimensionTrim)
                                        ? new String[] {((DimensionTrim)xSubset).getTrimLow(), ((DimensionTrim)xSubset).getTrimHigh()}
                                        : new String[] {((DimensionSlice)xSubset).getSlicePoint(), ((DimensionSlice)xSubset).getSlicePoint()};
                                if (ySubset == null) subY = new String[2];
                                else subY = (ySubset instanceof DimensionTrim)
                                        ? new String[] {((DimensionTrim)ySubset).getTrimLow(), ((DimensionTrim)ySubset).getTrimHigh()}
                                        : new String[] {((DimensionSlice)ySubset).getSlicePoint(), ((DimensionSlice)ySubset).getSlicePoint()};

                                // If only one subsetting was set in the request, fill the other dimension with bbox values (transformed in subsettingCrs)
                                if (subX[0] == null || subY[0] == null) {
                                    crsTool = new CrsUtil(m.getBbox().getCrsName(), request.getCRS().get(0).getSubsettingCrs());
                                    temp = crsTool.transform(m.getBbox().getLow1(), m.getBbox().getLow2(), m.getBbox().getHigh1(), m.getBbox().getHigh2());
                                    subX = (subX[0]==null) ? new String[] {"" + temp.get(0), "" + temp.get(2)} : subX;
                                    subY = (subY[0]==null) ? new String[] {"" + temp.get(1), "" + temp.get(3)} : subY;
                                }
                                
                                log.debug("Interval values: [" + subX[0] + ":" + subX[1] + "," + subY[0] + ":" + subY[1] + "]");
                                
                                // Now all values are filled: transform
                                crsTool = new CrsUtil(request.getCRS().get(0).getSubsettingCrs(), m.getBbox().getCrsName());
                                // crsTool.transform(xMin,yMin,xMax,yMax):
                                temp = crsTool.transform(subX[0], subY[0], subX[1], subY[1]);

                                // Update dimension intervals nodes: one dimension might not be trimmed/sliced!
                                if (xSubset != null) {
                                    if (xSubset instanceof DimensionTrim) {
                                        ((DimensionTrim)xSubset).setTrimLow(temp.get(0));
                                        ((DimensionTrim)xSubset).setTrimHigh(temp.get(2));
                                    } else ((DimensionSlice)xSubset).setSlicePoint(temp.get(0)); //~temp.get(2)
                                } 
                                if (ySubset != null) {
                                    if (ySubset instanceof DimensionTrim) {
                                        ((DimensionTrim)ySubset).setTrimLow(temp.get(1));
                                        ((DimensionTrim)ySubset).setTrimHigh(temp.get(3));
                                    } else ((DimensionSlice)ySubset).setSlicePoint(temp.get(1)); //~temp.get(3)
                                }

                                // Set transformed to true
                                if (xSubset != null) xSubset.isCrsTransformed(true);
                                if (ySubset != null) ySubset.isCrsTransformed(true);
                                // Change crs
                                if (xSubset != null) xSubset.setCrs(m.getBbox().getCrsName());
                                if (ySubset != null) ySubset.setCrs(m.getBbox().getCrsName());

                                log.debug("Transformed Interval values: [" + temp.get(0) + ":" + temp.get(2) + "," + temp.get(1) + ":" + temp.get(3) + "]");
                            }  
                        } catch (WCSException e) {
                            if (((PetascopeException)e).getExceptionCode().getExceptionCode().equalsIgnoreCase(ExceptionCode.InvalidMetadata.getExceptionCode()))
                                throw new WCSException(ExceptionCode.SubsettingCrsNotSupported, 
                                        "subsetting Crs " + request.getCRS().get(0).getSubsettingCrs() + " is not supported by the server.");
                            else throw new WCSException("Error while comparing requested subsettingCRS and native CRS of coverage " + m.getCoverageId());
                        } catch (Exception e) {
                            log.error(e.getMessage());
                            throw new WCSException("Error while comparing requested subsettingCRS and native CRS of coverage " + m.getCoverageId());
                        }
                }
            } // else: CRS transform already done (if necessary) by setBounds call.
        } else {
            // Req7: /req/crs/getCoverage-subsettingCrs-default
            // NOTE: if no CRS instance is presence, hence both subsettingCRS and outputCRS were not specified.
            if (request.getCRS().isEmpty()) 
                request.getCRS().add(new GetCoverageRequest.CRS(m.getBbox().getCrsName(), null));
            else request.getCRS().get(0).setSubsettingCrs(m.getBbox().getCrsName());
        }

        // Req10: /req/crs/getCoverage-outputCrs-default
        if (request.getCRS().get(0).getOutputCrs() == null)
            request.getCRS().get(0).setOutputCrs(request.getCRS().get(0).getSubsettingCrs());

        // Convert human-readable **timestamp** to ANSI day number
        // NOTE: solution is not definitive!
        DimensionSubset tSubset = request.getSubset(AxisTypes.T_AXIS); // or TEMPORAL_AXIS?
        if (tSubset != null && !tSubset.isCrsTransformed()) {
            try {
                if (tSubset instanceof DimensionTrim) {
                    int dayLo = TimeUtil.convert2AnsiDay(((DimensionTrim)tSubset).getTrimLow());
                    int dayHi = TimeUtil.convert2AnsiDay(((DimensionTrim)tSubset).getTrimHigh());
                    // Check order:
                    if (dayLo > dayHi) {
                        log.error("Temporal subset order is wrong: lower bound is greater than upper-bound.");
                        throw new WCSException(ExceptionCode.InvalidParameterValue);
                    } else {
                        ((DimensionTrim)tSubset).setTrimLow((double)dayLo);
                        ((DimensionTrim)tSubset).setTrimHigh((double)dayHi);
                    }
                } else if (request.getSubset(AxisTypes.TEMPORAL_AXIS) instanceof DimensionSlice) {
                    int day = TimeUtil.convert2AnsiDay(((DimensionSlice)tSubset).getSlicePoint());
                    ((DimensionSlice)tSubset).setSlicePoint((double)day);
                }
                
                // Set transformed to true
                if (tSubset != null) tSubset.isCrsTransformed(true);
                
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
            // Correct syntax for crsTransform???
            proc = "crsTransform(" + proc + ", { x:\"" + req.getCRS().get(0).getOutputCrs() + "\","
                    + "y:\"" + req.getCRS().get(0).getOutputCrs() + "\"}, { })";
            // TODO: manage interpolation formats list.
        }
        String query = "for c in (" + req.getCoverageId() + ") return encode(" + proc + ", \"" + format + "\")";
        return Pair.of(query, axes.trim());
    }
}