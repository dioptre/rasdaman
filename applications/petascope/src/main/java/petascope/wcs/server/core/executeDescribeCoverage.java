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

import petascope.core.Metadata;
import petascope.core.DbMetadataSource;
import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;
import net.opengis.ows.v_1_0_0.BoundingBoxType;
import net.opengis.wcs.ows.v_1_1_0.DomainMetadataType;
import net.opengis.wcs.ows.v_1_1_0.InterpolationMethodType;
import net.opengis.wcs.ows.v_1_1_0.InterpolationMethods;
import net.opengis.wcs.ows.v_1_1_0.UnNamedDomainType;
import net.opengis.wcs.v_1_1_0.CoverageDescriptionType;
import net.opengis.wcs.v_1_1_0.CoverageDescriptions;
import net.opengis.wcs.v_1_1_0.CoverageDomainType;
import net.opengis.wcs.v_1_1_0.DescribeCoverage;
import net.opengis.wcs.v_1_1_0.FieldType;
import net.opengis.wcs.v_1_1_0.RangeType;
import net.opengis.wcs.v_1_1_0.SpatialDomainType;
import petascope.wcps.server.core.*;
import java.util.Iterator;
import javax.xml.XMLConstants;
import javax.xml.bind.JAXBElement;
import javax.xml.namespace.QName;
import net.opengis.ows.v_1_0_0.KeywordsType;
import net.opengis.wcs.ows.v_1_1_0.AnyValue;
import net.opengis.wcs.v_1_1_0.TimeSequenceType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.CrsUtil;

/**
 * This class takes a WCS DescribeCoverage XML request and executes request,
 * building the corresponding XML respose.
 *
 * @author Andrei Aiordachioaie
 */
public class executeDescribeCoverage {

    private static Logger log = LoggerFactory.getLogger(executeDescribeCoverage.class);
    private boolean finished;
    private DescribeCoverage input;
    private DbMetadataSource meta;
    private CoverageDescriptions output;

    /**
     * Default constructor
     * @param cap DescribeCoverage object, a WCS (or WCPS) request
     * @param metadataDbPath Path to the "dbparams.properties" file
     */
    public executeDescribeCoverage(DescribeCoverage cap, DbMetadataSource source)
            throws WCSException {
        input = cap;
        output = new CoverageDescriptions();
        finished = false;
        meta = source;
    }

    /**
     * Main method of this class. Retrieves the response to the DescribeCoverage
     * request given to the constructor. If needed, it also calls <b>process()</b>
     * @return a CoverageDescriptions object.
     * @throws wcs_web_service.WCSException
     */
    public CoverageDescriptions get() throws WCSException {
        if (finished == false) {
            process();
        }
        if (finished == false) {
            throw new WCSException(ExceptionCode.NoApplicableCode, "Could not execute the GetCapabilities request! "
                    + "Please see the other errors...");
        }

        return output;
    }

    /**
     * Computes the response to the DescribeCoverage request given to the constructor.
     */
    public void process() throws WCSException {
        String name;

        for (int i = 0; i < input.getIdentifier().size(); i++) {
            name = input.getIdentifier().get(i);
            output.getCoverageDescription().add(getCoverageDescription(name));
        }
        finished = true;
    }

    /**
     * Retrieve details for one coverage.
     * @param name Name of the coverage
     * @return CoverageDescriptionType object, that can just be plugged in the respose object
     */
    private CoverageDescriptionType getCoverageDescription(String name) throws WCSException {
        log.trace("Building coverage description for coverage '" + name + "' ...");
        CoverageDescriptionType desc = new CoverageDescriptionType();

        // Error checking: is the coverage available?
        if (meta.existsCoverageName(name) == false) {
            throw new WCSException(ExceptionCode.InvalidParameterValue, "Identifier. Explanation: Coverage "
                    + name + " is not served by this server !");
        }

        // Read all coverage metadata
        Metadata cov = null;

        try {
            cov = meta.read(name);
        } catch (Exception e) {
            throw new WCSException(ExceptionCode.NoApplicableCode, "Metadata for coverage " + name + " is not valid.");
        }

        desc.setIdentifier(name);
        desc.setTitle(cov.getTitle());
        desc.setAbstract(cov.getAbstract());

        KeywordsType keyword = new KeywordsType();
        Iterator<String> keys = SDU.str2string(cov.getKeywords()).iterator();
        while (keys.hasNext()) {
            String k = keys.next();
            keyword.getKeyword().add(k);
        }
        desc.getKeywords().add(keyword);

        // Coverage Domain
        CoverageDomainType domain = null;
        Double lo1 = 0.0, lo2 = 0.0, hi1 = 0.0, hi2 = 0.0;

        /* Default Bounding Box (uses IMAGE_CRS): use image size */
        BoundingBoxType bbox = new BoundingBoxType();
        CellDomainElement X = cov.getXCellDomain();
        CellDomainElement Y = cov.getYCellDomain();
        if (X != null && Y != null) {
            lo1 = X.getLo().doubleValue();
            hi1 = X.getHi().doubleValue();
            lo2 = Y.getLo().doubleValue();
            hi2 = Y.getHi().doubleValue();

            bbox.setCrs(CrsUtil.IMAGE_CRS);

            bbox.getLowerCorner().add(lo1);
            bbox.getLowerCorner().add(lo2);
            bbox.getUpperCorner().add(hi1);
            bbox.getUpperCorner().add(hi2);
        } else {
            throw new WCSException(ExceptionCode.NoApplicableCode, "Internal error: Could "
                    + "not find X and Y cell domain extents.");
        }

        /* Try to use bounding box, if available */
        //Wgs84Crs crs = cov.getCrs();
        Bbox boundbox = cov.getBbox();
        BoundingBoxType bboxType = new BoundingBoxType();
        //bbox84.setCrs(DomainElement.WGS84_CRS);
        bboxType.setCrs(boundbox.getCrsName());
        if (boundbox != null) {
            lo1 = boundbox.getLow1().doubleValue();
            hi1 = boundbox.getHigh1().doubleValue();
            lo2 = boundbox.getLow2().doubleValue();
            hi2 = boundbox.getHigh2().doubleValue();

            bboxType.getLowerCorner().add(lo1);
            bboxType.getLowerCorner().add(lo2);
            bboxType.getUpperCorner().add(hi1);
            bboxType.getUpperCorner().add(hi2);

            bbox = bboxType;
        }

        domain = new CoverageDomainType();
        SpatialDomainType spatial = new SpatialDomainType();
        spatial.getBoundingBox().add(new JAXBElement<BoundingBoxType>(
                new QName("http://www.opengis.net/ows", "BoundingBox", XMLConstants.DEFAULT_NS_PREFIX),
                BoundingBoxType.class, bbox));
        domain.setSpatialDomain(spatial);


        /* Find a time-axis if exists */
        CellDomainElement T = cov.getTCellDomain();
        if (T != null) {
            log.trace("Found time-axis for coverage: [" + T.getLo() + ", " + T.getHi() + "]");
            TimeSequenceType temporal = new TimeSequenceType();
            temporal.getTimePositionOrTimePeriod().add(T.getLo().intValue());
            temporal.getTimePositionOrTimePeriod().add(T.getHi().intValue());
            domain.setTemporalDomain(temporal);
        }

        desc.setDomain(domain);

        // The coverage Range
        RangeType wcsRange = new RangeType();
        Iterator<RangeElement> rangeIt = cov.getRangeIterator();

        while (rangeIt.hasNext()) {
            RangeElement range = rangeIt.next();
            FieldType field = new FieldType();

            field.setIdentifier(range.getName());
            UnNamedDomainType domtype = new UnNamedDomainType();
            AnyValue anyVal = new AnyValue();
            domtype.setAnyValue(anyVal);
            DomainMetadataType dommeta = new DomainMetadataType();

            dommeta.setValue(range.getType());
            domtype.setDataType(dommeta);
            field.setDefinition(domtype);

            InterpolationMethods interp = new InterpolationMethods();

            InterpolationMethodType meth = new InterpolationMethodType();

            meth.setValue(cov.getInterpolationDefault());
            meth.setNullResistance(cov.getNullResistanceDefault());
            interp.setDefaultMethod(meth);

            Iterator<InterpolationMethod> interpIt = cov.getInterpolationMethodIterator();

            while (interpIt.hasNext()) {
                InterpolationMethod wcpsInterp = interpIt.next();

                meth = new InterpolationMethodType();
                meth.setValue(wcpsInterp.getInterpolationType());
                meth.setNullResistance(wcpsInterp.getNullResistance());
                if ((wcpsInterp.getInterpolationType().equals(interp.getDefaultMethod().getValue()) == false) || (wcpsInterp.getNullResistance().equals(interp.getDefaultMethod().getNullResistance()) == false)) {
                    interp.getOtherMethod().add(meth);
                }
            }

            field.setInterpolationMethods(interp);
            wcsRange.getField().add(field);
        }
        desc.setRange(wcsRange);

        // Supported formats for GetCoverage: known rasdaman encoders
        String[] mimetypes = meta.getMimetypesList();

        for (int i = 0; i < mimetypes.length; i++) {
            String format = mimetypes[i];
            desc.getSupportedFormat().add(format);
        }

        // Available CRSs for current coverage
        desc.getSupportedCRS().add(CrsUtil.IMAGE_CRS);
        if (cov.getBbox() != null) {
            //desc.getSupportedCRS().add(DomainElement.WGS84_CRS);
            desc.getSupportedCRS().add(cov.getBbox().getCrsName());
        }

        log.trace("Done building the Coverage Description for coverage '" + name + "'.");

        return desc;
    }
}
