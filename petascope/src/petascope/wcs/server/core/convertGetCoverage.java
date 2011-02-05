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
package petascope.wcs.server.core;

import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;
import net.opengis.gml.v_3_1_1.TimePositionType;
import net.opengis.ows.v_1_0_0.BoundingBoxType;
import net.opengis.wcs.v_1_1_0.DomainSubsetType;
import net.opengis.wcs.v_1_1_0.GetCoverage;
import net.opengis.wcs.v_1_1_0.GridCrsType;
import net.opengis.wcs.v_1_1_0.RangeSubsetType;
import net.opengis.wcs.v_1_1_0.TimePeriodType;
import petascope.core.DbMetadataSource;
import petascope.wcps.server.core.ProcessCoveragesRequest;
import petascope.exceptions.WCPSException;
import petascope.wcps.server.core.DomainElement;
import petascope.ConfigManager;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.antlr.runtime.RecognitionException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.core.Metadata;

/**
 * This class takes a WCS GetCoverage XML request and converts this request into
 * a WCPS ProcessCoverages XML request. It is the user's responsibility to pass
 * the WCPS request to a WCPS server in order to view to result.
 *
 * @author Andrei Aiordachioaie
 */
public class convertGetCoverage {

    private static Logger log = LoggerFactory.getLogger(convertGetCoverage.class);
    // Trimming
    private boolean xAxisTrim = false;
    private boolean yAxisTrim = false;
    // Time Trimming
    private boolean timeTrim = false;
    // Time Slicing
    private boolean timeSlice = false;

    /* WCPS requests */
    private String abstractRequest;
    // Range Composition (field combination)
    private boolean combineFields;

    /* Value variables, used for building the WCPS abstract syntax query */
    private String coverageName;
    private List<String> fields;
    private boolean finished;
    // Output
    private String format;

    /* The Configuration Manager */
    private DbMetadataSource meta;
    private boolean store;
    private String time1, time2;
    private String timePos;
    private GetCoverage wcs;
    private long px0, px1, py0, py1;    // Bounding box subsetting
    private String crsName;             // for bounding box
    private String xmlRequest;
    private Metadata covMeta;

    /**
     * Default constructor
     * @param cov GetCoverage object, the input WCS request for retrieving a coverage
     * @param path Path to the "settings.properties" file
     */
    public convertGetCoverage(GetCoverage cov, DbMetadataSource source)
            throws WCSException, WCPSException {
        /* Init Config Manager and global */
        ConfigManager globals = ConfigManager.getInstance();
        /* Setup objects */
        wcs = cov;
        finished = false;

        meta = source;
    }

    /**
     * * Retrieves the WCPS request object that is
     * equivalent to the WCS request. If it is not yet converted, it will
     * perform the conversion.
     *
     * @return a WCPS abstract syntax query as a string
     * @throws wcs.server.core.WCSException
     */
    public String get() throws WCSException {
        try {
            if (!finished) {
                process();
            }

            /* Convert the abstract syntax query to a ProcessCoveragesRequest */
            log.debug("Converting WCPS abstract query '{}' to xml", abstractRequest);
            xmlRequest = ProcessCoveragesRequest.abstractQueryToXmlQuery(abstractRequest);
            log.debug("Resulting XML query is: \n{}", xmlRequest);
        } catch (RecognitionException re) {
            throw new WCSException(ExceptionCode.NoApplicableCode, "Internal error: Generated abstract syntax query was not valid.", re);
        }

        return xmlRequest;
    }

    /**
     * Converts WCS node 1 (Coverage Identifier) to WCPS info
     * @throws wcs_web_service.WCSException
     */
    private void readField1() throws WCSException {
        coverageName = wcs.getIdentifier().getValue();
    }

    /**
     * Converts WCS node 2 (Domain subsetting) to WCPS info
     * @throws wcs_web_service.WCSException
     */
    private void readField2() throws WCSException {
        if (!wcs.isSetDomainSubset()) {
            throw new WCSException(ExceptionCode.MissingParameterValue, "DomainSubset");
        }
        if (!wcs.getDomainSubset().isSetBoundingBox()) {
            throw new WCSException(ExceptionCode.MissingParameterValue, "BoundingBox");
        }

        DomainSubsetType domain = wcs.getDomainSubset();

        // DomainSubset->BoundingBox
        if (domain.isSetBoundingBox()) {
            BoundingBoxType bbox = (BoundingBoxType) domain.getBoundingBox().getValue();

            /* We only understand two CRSs: WGS84 and IMAGE_CRS */
            /* TODO: Implement CRS transformations */
            crsName = bbox.getCrs();
            if (crsName != null) {
                if (crsName.equals(DomainElement.IMAGE_CRS)) {
                    log.trace("CRS: NATIVE_IMAGE_CRS");
                } else if (crsName.equals(DomainElement.WGS84_CRS)) {
                    log.trace("CRS: WGS84");
                } else {
                    throw new WCSException(ExceptionCode.InvalidParameterValue, "BoundingBox.crs. Explanation: "
                            + "CRS '" + crsName + "' not available on this server.");
                }
            } else {
                log.debug("CRS: None specified for bounding box");
                log.debug("CRS: Using default IMAGE_CRS");
                crsName = DomainElement.IMAGE_CRS;
            }

            /* BBox declarations */
            if (bbox.getLowerCorner().size() != 2) {
                throw new WCSException(ExceptionCode.InvalidParameterValue, "LowerCorner. Explanation: "
                        + "BoundingBox -> LowerCorner should have exactly two "
                        + "values, not " + bbox.getLowerCorner().size());
            }
            if (bbox.getUpperCorner().size() != 2) {
                throw new WCSException(ExceptionCode.InvalidParameterValue, "UpperCorner. Explanation: "
                        + "BoundingBox -> UpperCorner should have exactly two "
                        + "values, not " + bbox.getUpperCorner().size());
            }

            xAxisTrim = true;
            int u2 = bbox.getLowerCorner().get(0).intValue();
            int u3 = bbox.getUpperCorner().get(0).intValue();
            log.trace("Added X-axis trimming ! (DomainSubset->BoundingBox): " + u2 + " ... "
                    + u3);

            yAxisTrim = true;
            int v2 = bbox.getLowerCorner().get(1).intValue();
            int v3 = bbox.getUpperCorner().get(1).intValue();
            log.trace("Added Y-axis trimming ! (DomainSubset->BoundingBox): " + v2 + " ... "
                    + v3);

            /* Use bounding-box values as they are given */
            px0 = u2;
            py0 = v2;
            px1 = u3;
            py1 = v3;

            if (crsName.equals(DomainElement.IMAGE_CRS) == false
                    && crsName.equals(DomainElement.WGS84_CRS) == false) {
                throw new WCSException(ExceptionCode.NoApplicableCode, "Unknown CRS: " + crsName);
            }
        }

        // DomainSubset->TemporalSubset
        if (domain.isSetTemporalSubset()) {
            // TemporalSubset is of type TimeSequenceType = choice(gml:TimePosition, wcs:TimePeriodType)
            Object one = domain.getTemporalSubset().getTimePositionOrTimePeriod().get(0);

            log.trace("Inside TemporalSubset there is " + one.getClass());
            if (one instanceof net.opengis.gml.v_3_1_1.TimePositionType) {
                // TemporalSubset = gml:TimePosition
                // use WCPS:slice
                timeSlice = true;

                TimePositionType pos = (TimePositionType) one;


                /* Default syntax is ISO 8601. 
                However, we also accept direct time-axis coordinates, as a fail-back solution. */
                timePos = parseTimePosition(pos);
                log.trace("Added time-axis slicing ! ( DomainSubset->TemporalSubset->gml:TimePositionType): position "
                        + timePos);
            } else if (one instanceof net.opengis.wcs.v_1_1_0.TimePeriodType) {
                // TemporalSubset = wcs:TimePeriodType
                timeTrim = true;

                TimePeriodType period = (TimePeriodType) one;

                TimePositionType pos1 = period.getBeginPosition();
                TimePositionType pos2 = period.getEndPosition();

                /* Default syntax is ISO 8601.
                However, we also accept direct time-axis coordinates, as a fail-back solution. */
                time1 = parseTimePosition(pos1);
                time2 = parseTimePosition(pos2);

                log.trace("Added time-axis trimming ! ( DomainSubset->TemporalSubset->wcs:TimePeriodType): "
                        + time1 + " ... " + time2);
            }
        }
    }

    /**
     * Converts WCS node 3 (Range subsetting) to WCPS info
     */
    private void readField3() throws WCSException {
        if (wcs.isSetRangeSubset()) {
            RangeSubsetType range = wcs.getRangeSubset();

            combineFields = true;
            fields = new ArrayList<String>();

            Iterator<RangeSubsetType.FieldSubset> it = range.getFieldSubset().iterator();

            while (it.hasNext()) {
                RangeSubsetType.FieldSubset field = it.next();

                fields.add(field.getIdentifier().getValue());
                log.trace("RangeSubsetType->FieldSubset->Identifier is "
                        + field.getIdentifier().getValue());
                log.trace("RangeSubsetType->FieldSubset->Interpolation is "
                        + field.getInterpolationType());
                /* NOTE:  We ignore interpolation instructions (optional) */
                /* NOTE:  We ignore axis subset lists (optional) */
            }

        }
    }

    /**
     * Converts WCS node 4 (Output) to WCPS info
     * @throws wcs_web_service.WCSException
     */
    private void readField4() throws WCSException {
        if (!wcs.isSetOutput()) {
            throw new WCSException(ExceptionCode.MissingParameterValue, "Output");
        }
        log.trace("Format: " + wcs.getOutput().getFormat());

        if (wcs.getOutput().isSetGridCRS()) {
            GridCrsType crs = wcs.getOutput().getGridCRS();

            throw new WCSException(ExceptionCode.NoApplicableCode, "Currently, the Output->GridCRS node is not supported !");
        }

        String wcsMimeFormat = wcs.getOutput().getFormat();

        format = meta.mimetypeToFormat(wcsMimeFormat);
        log.trace("New format: " + format);
        if ((format == null) || format.equals("")) {
            throw new WCSException(ExceptionCode.InvalidParameterValue, "Output format");
        }

        log.trace("issetstore = " + wcs.getOutput().isSetStore());
        log.trace("isstore = " + wcs.getOutput().isStore());
        store = false;
        if (wcs.getOutput().isSetStore() && wcs.getOutput().isStore()) {
            store = true;
        }

        /* WCPS does not support "store=true" */
        if (store) {
            throw new WCSException(ExceptionCode.InvalidParameterValue, "Output Store. Explanation: "
                    + "Cannot store result image on server.");
        }
    }

    /**
     * Performs the conversion of the WCS request into a WCPS abstract syntax request.
     * @throws wcs_web_service.WCSException
     */
    @SuppressWarnings("static-access")
    public void process() throws WCSException {
        /** * Processing starts here ... with the nodes of the WCS * */
        // Service Description
        log.debug("WCS service: \"" + wcs.SERVICE + "\"");
        log.debug("WCS version: \"" + wcs.VERSION + "\"");
        if (!wcs.SERVICE.equalsIgnoreCase("WCS")) {
            throw new WCSException(ExceptionCode.InvalidParameterValue, "Service. Explanation: "
                    + "Only the WCS service is supported.");
        }
        if (!wcs.VERSION.equals("1.1.0")) {
            throw new WCSException(ExceptionCode.InvalidParameterValue, "Version. Explanation: "
                    + "Only WCS Version 1.1.0 is currently supported.");
        }

        // First of all, error checking: is the coverage offered by the server?
        if (!wcs.isSetIdentifier()) {
            throw new WCSException(ExceptionCode.MissingParameterValue, "Identifier");
        }
        try {
            if (!meta.existsCoverageName(wcs.getIdentifier().getValue())) {
                throw new WCSException(ExceptionCode.InvalidParameterValue, "Identifier. Explanation: "
                        + "Coverage " + wcs.getIdentifier().getValue()
                        + " is not served by this server.");
            }
            covMeta = meta.read(wcs.getIdentifier().getValue());
        } catch (Exception e) {
            throw new WCSException(ExceptionCode.InvalidParameterValue, "Identifier. Explanation: "
                    + "Coverage " + wcs.getIdentifier().getValue()
                    + " is not served by this server.");
        }


        // Convert all the child nodes of WCS GetCoverage XML node
        log.trace("*** Reading WCS node 1 ... ");
        readField1();
        log.trace("*** Reading WCS node 2 ...");
        readField2();
        log.trace("*** Reading WCS node 3 ...");
        readField3();
        log.trace("*** Reading WCS node 4 ...");
        readField4();
        log.trace("*** Assembling WCPS abstract syntax query ...");
        assembleFinalWcpsQuery();

        /* Done building WCPS abstract query. */
        finished = true;
        log.trace("Done Converting WCS GetCoverage into WCPS ProcessCoverage.");
    }

    /**
     * Constructs the WCPS abstract syntax query from the information gathered
     * by <code>readField*()</code> functions.
     */
    private void assembleFinalWcpsQuery() {
        // Bind variable "c" to the coverage name
        String forClause = "for c in (" + coverageName + ") ";
        // Base processing is done on the coverage variable
        String processing = "c";

        if (xAxisTrim && yAxisTrim) {
            // Bounding box subsetting
            String xAxis = "x:\"" + crsName + "\" (" + px0 + ":" + px1 + ")";
            String yAxis = "y:\"" + crsName + "\" (" + py0 + ":" + py1 + ")";
            String tAxis = "t(" + time1 + ":" + time2 + ")";

            if (timeTrim) {
                processing = "trim( " + processing + ", {" + xAxis + ", " + yAxis + ", " + tAxis
                        + "} )";
            } else {
                processing = "trim( " + processing + ", {" + xAxis + ", " + yAxis + "} )";
            }
        }
        if (timeSlice) {
            // Time slicing
            String tAxis = "t(" + timePos + ")";

            processing = "slice( " + processing + ", {" + tAxis + "} )";
        }
        if (combineFields) {
            // Only one field means we have a "field select" statement
            if (fields.size() == 1) {
                String field = fields.get(0);

                processing = "(" + processing + ")." + field;
            } else // Multiple fields translate into a "range constructor" statement
            {
                String newProc = "";
                Iterator<String> it = fields.iterator();

                while (it.hasNext()) {
                    String field = it.next();

                    if (newProc.length() > 0) {
                        newProc += "; ";
                    }
                    newProc += field + ":" + processing;
                }
                processing = "struct { " + newProc + " }";
            }
        }
        // Set the output format
        processing = "encode(" + processing + ", \"" + format + "\")";

        // Build the final query
        String returnClause = "return " + processing;

        abstractRequest = forClause + returnClause;
    }

    /** Convert a time-position JAXB object into a numerical index we can use for the
     * time subsetting. 
     * @param pos TimePositionType object
     * @return String representation of integer time
     * @throws WCSException if the contents of the time position is not an integer or an ISO8601 string
     */
    private String parseTimePosition(TimePositionType pos) throws WCSException {
        String result;

        log.trace("TimePosition has length " + pos.getValue().size());
        if (pos.getValue().size() != 1) {
            throw new WCSException(ExceptionCode.InvalidParameterValue, "TimePosition. Explanation: "
                    + "The TimePosition element should have exactly one item, and not "
                    + pos.getValue().size());
        }
        String timeStr = pos.getValue().get(0);

        log.debug("Parsing time position: " + timeStr);
        try {
            // ISO 8601 parsing
            TimeString ts = new TimeString(timeStr);
            log.debug("Found time position (ISO 8601): " + timeStr);
            if (ts.subtract(covMeta.getTimePeriodBeginning()) < 0
                    || TimeString.difference(covMeta.getTimePeriodEnd(), timeStr) < 0) {
                throw new WCSException(ExceptionCode.InvalidParameterValue, "TimePosition: value " + timeStr
                        + " is outside this coverage's time range.");
            }
            String begin = covMeta.getTimePeriodBeginning();
            if (begin == null) {
                throw new WCSException(ExceptionCode.InvalidTemporalMetadata, "Coverage '" + covMeta.getCoverageName()
                        + "' has no time axis beginning or end in table PS_Domain.");
            }
            long diff1 = ts.subtract(begin);
            log.trace("Selected time span (ISO 8601, in ms) : " + diff1);
            long diff2 = covMeta.getTimeSpan();
            if (diff2 == -1) {
                throw new WCSException(ExceptionCode.InvalidTemporalMetadata, "Coverage '" + covMeta.getCoverageName()
                        + "' has no time axis beginning or end in table PS_Domain.");
            }
            log.trace("Coverage " + covMeta.getCoverageName() + " has time span (ISO 8601, in ms) : " + diff2);
            log.trace("Coverage " + covMeta.getCoverageName() + " has time indexes span  : " + covMeta.getTimeIndexesSpan());
            Double dIndex = covMeta.getTimeIndexesSpan() * diff1 * new Double(1.0) / diff2;
            if (dIndex == -1) {
                throw new WCSException(ExceptionCode.InvalidTemporalMetadata, "Coverage '" + covMeta.getCoverageName()
                        + "' has no time axis.");
            }
            log.trace("Computed time axis index: " + dIndex);
            long timeIndex = dIndex.longValue();

            result = String.valueOf(timeIndex);
        } catch (IllegalArgumentException e) {
            log.warn("Time position '" + timeStr + "' was not in ISO 8601 format. Trying to parse integer...");
            try {
                Integer tPos = Integer.parseInt(timeStr);
                log.debug("Found time position in integer coordinates: " + tPos);
                result = tPos.toString();
            } catch (NumberFormatException e2) {
                throw new WCSException(ExceptionCode.InvalidParameterValue, "TimePosition: " + timeStr, e2);
            }
        }

        return result;
    }
}
