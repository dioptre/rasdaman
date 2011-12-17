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
package petascope.wcs2.legacy;

import java.util.Iterator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.core.DbMetadataSource;
import petascope.core.Metadata;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import petascope.util.Pair;
import petascope.util.StringUtil;
import petascope.wcps.server.core.CellDomainElement;
import petascope.wcps.server.core.DomainElement;
import petascope.wcps.server.core.RangeElement;
import petascope.wcs2.handlers.AbstractRequestHandler;
import petascope.wcs2.parsers.GetCoverageRequest;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSlice;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSubset;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionTrim;
import petascope.wcs2.handlers.Response;
import petascope.wcs2.templates.Templates;

/**
 * GetCapabilities operation for The Web Coverage Service 2.0
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class GetCoverageOld2 extends AbstractRequestHandler<GetCoverageRequest> {

    private static Logger log = LoggerFactory.getLogger(GetCoverageOld2.class);
    private String lowPoint, highPoint, newAxesLabels;

    public GetCoverageOld2(DbMetadataSource meta) {
        super(meta);
    }

    @Override
    public Response handle(GetCoverageRequest req) throws WCSException {
        Pair<Metadata, String> p = null; //coverageDescription(req.getCoverageId(),                Templates.getTemplate(Templates.GRID_COVERAGE), true);
        Metadata cov = p.fst;
        String descr = p.snd;

        String subsetting = computeRequestSubsettingLimits(req, cov);
        String coverageData = buildCoverageData(req, cov, subsetting);
        return new Response(descr
                .replaceAll("\\{low\\}", lowPoint)
                .replaceAll("\\{high\\}", highPoint)
                .replaceAll("\\{axisLabels\\}", newAxesLabels)
                .replaceAll("\\{coverageData\\}", coverageData));
    }

    /**
     * Computes the domain of the new coverage, and returns a string that can be
     * used to do subsetting on the original coverage. Also computes the low, high
     * and the axis labels for the new coverage.
     *
     * @param coverage
     * @return
     * @throws WCSException
     */
    private String computeRequestSubsettingLimits(GetCoverageRequest req, Metadata coverage)
            throws WCSException {
        int dims = coverage.getDimension(), i = 0;
        String[] limits = new String[dims];
        Double[] high = new Double[dims];
        Double[] low = new Double[dims];
        String[] axesLabels = new String[dims];
        boolean[] sliced = new boolean[dims];
        boolean[] trimmed = new boolean[dims];
        
        int axisIndex;
        String axis;

        Iterator<CellDomainElement> cdit = coverage.getCellDomainIterator();
        Iterator<DomainElement> dit = coverage.getDomainIterator();
        i = 0;
        while (cdit.hasNext() && dit.hasNext()) {
            CellDomainElement cell = cdit.next();
            DomainElement dom = dit.next();
            high[i] = cell.getHi().doubleValue();
            low[i] = cell.getLo().doubleValue();
            axesLabels[i] = dom.getName();
            limits[i] = low[i] + ":" + high[i];
            sliced[i] = false;
            trimmed[i] = false;
            i++;
        }

        for (DimensionSubset subset : req.getSubsets()) {
            axis = subset.getDimension();
            axisIndex = coverage.getDomainIndexByName(axis);
            if (axisIndex == -1) {
                throw new WCSException(ExceptionCode.InvalidParameterValue, "Unknown axis: " + axis);
            }
            if (trimmed[axisIndex] || sliced[axisIndex]) {
                throw new WCSException(ExceptionCode.NoApplicableCode,
                        "Already performed one subsetting operation on axis: " + axis);
            }
            if (subset instanceof DimensionTrim) {
                DimensionTrim trim = (DimensionTrim) subset;
//                low[axisIndex] = d2s(trim.getTrimLow());
//                high[axisIndex] = trim.getTrimHigh();
                limits[axisIndex] = low[axisIndex] + ":" + high[axisIndex];
                trimmed[axisIndex] = true;
            } else if (subset instanceof DimensionSlice) {
                DimensionSlice slice = (DimensionSlice) subset;
//                low[axisIndex] = slice.getSlicePoint();
//                high[axisIndex] = slice.getSlicePoint();
                limits[axisIndex] = slice.getSlicePoint().toString();
                sliced[axisIndex] = true;
            }
            log.debug("New limits for axis {}: {}", axis, limits[axisIndex]);
        }

        // Compute the lowest, highest point and the labels
        lowPoint = "";
        highPoint = "";
        newAxesLabels = "";
        for (i = 0; i < dims; i++) {
            if (!sliced[i]) {
                lowPoint += low[i] + " ";
                highPoint += high[i] + " ";
                newAxesLabels += axesLabels[i] + " ";
            }
        }
        lowPoint = lowPoint.trim();
        highPoint = highPoint.trim();
        newAxesLabels = newAxesLabels.trim();

        return StringUtil.combine(",", limits);
    }

    /**
     * Retrieve the coverage data for a multi-band coverage,
     * with particular subsetting parameters.
     * @param coverage metadata for the coverage we want
     * @param subsetting subsetting string, to be used in the RasQL query
     * @return a string of space-separated values, where various bands of one
     * pixel are comma-separated. For example, the string "1,2 3,4 5,6" can
     * be the coverage data of a 1-by-3 coverage, with two bands
     */
    private String buildCoverageData(GetCoverageRequest req, Metadata coverage, String subsetting)
            throws WCSException {
        String coverageId = req.getCoverageId();
        
        // count bands
        Iterator<RangeElement> rit = coverage.getRangeIterator();
        int bandcount = 0;
        while (rit.hasNext()) {
            rit.next();
            bandcount++;
        }
        log.debug("Coverage {} has {} bands", coverageId, bandcount);
        
        // for all bands of the coverage, execute a rasql query
        String[][] pixels = new String[bandcount][];
        String currentBand = "";
        for (int band = 0; band < bandcount; band++) {
            log.trace("Processing band {}", band);
            currentBand = "." + band;
            // If this is a one-band image, then band-subsetting would result in an error
            if (bandcount == 1) {
                currentBand = "";
            }
            
            String output = "";
//            try {
//                output = "RasUtil.executeRasqlQuery("select csv(cov[" + subsetting + "]" + 
//                 currentBand + ") "  + "from " + coverageId + " as cov");
//            } catch (RasdamanException ex) {
//                throw new WCSException(ExceptionCode.RasdamanError, ex);
//            }
            output = output.replaceAll("[\\{\\}]", "");
            // Tokenize the input to get the pixel values of the current band
            pixels[band] = output.split(",");
            log.trace("Done processing band {}.", band);
        }

        /* Combine all bands into one single string */
        int pixelcount = pixels[0].length;
        StringBuilder ret = new StringBuilder(pixelcount * bandcount);
        log.debug("Combining {} pixels with {} bands...", pixelcount, bandcount);
        for (int pix = 0; pix < pixelcount; pix++) {
            if (pixelcount > 20 && pix % (pixelcount / 20) == 0) {
                log.debug("Processing Pixel {} of " + pixelcount + " - {}%", pix, pix * 100 / (pixelcount - 1));
            }
            for (int b = 0; b < bandcount - 1; b++) {
                ret.append(pixels[b][pix]);
                ret.append(',');
            }
            ret.append(pixels[bandcount - 1][pix]);
            ret.append(" ");
        }
        return ret.substring(0, ret.length() - 2);
    }
}
