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

import java.util.ArrayList;
import java.util.List;
import petascope.util.ListUtil;
import petascope.wcs2.extensions.FormatExtension;

/**
 * A GetCoverage request object, populated by a parser.
 *
   <element name="GetCoverage" type="wcs:GetCoverageType">
        <annotation>
            <documentation>Request to a WCS to perform the GetCoverage operation. This operation allows a client to retrieve a subset of one coverage. </documentation>
        </annotation>
    </element>
    <complexType name="GetCoverageType">
        <complexContent>
            <extension base="wcs:RequestBaseType">
                <sequence>
                    <element ref="wcs:CoverageId">
                        <annotation>
                            <documentation>Identifier of the coverage that this GetCoverage operation request shall draw from. </documentation>
                        </annotation>
                    </element>
                    <element ref="wcs:DimensionSubset" minOccurs="0" maxOccurs="unbounded"/>
                </sequence>
            </extension>
        </complexContent>
    </complexType>

    <!-- ======================================================= -->
    <!-- Domain subset types and elements                        -->
    <!-- ======================================================= -->
    <element name="DimensionSubset" type="wcs:DimensionSubsetType" abstract="true">
        <annotation>
            <documentation>Definition of the desired subset of the domain of the coverage. This is either a Trim operation, or a Slice operation.</documentation>
        </annotation>
    </element>
    <complexType name="DimensionSubsetType" abstract="true">
        <sequence>
            <element name="Dimension" type="NCName"/>
        </sequence>
    </complexType>
    <!-- ======================================================= -->
    <element name="DimensionTrim" type="wcs:DimensionTrimType" substitutionGroup="wcs:DimensionSubset">
        <annotation>
            <documentation>Describes the trimming of a coverage's domain axis, between two values.</documentation>
        </annotation>
    </element>
    <complexType name="DimensionTrimType">
        <complexContent>
            <extension base="wcs:DimensionSubsetType">
                <sequence>
                    <element name="TrimLow" type="double" minOccurs="0"/>
                    <element name="TrimHigh" type="double" minOccurs="0"/>
                </sequence>
            </extension>
        </complexContent>
    </complexType>
    <!-- ======================================================= -->
    <element name="DimensionSlice" type="wcs:DimensionSliceType" substitutionGroup="wcs:DimensionSubset">
        <annotation>
            <documentation>Describes the slicing of a coverage's domain axis at a particular point.</documentation>
        </annotation>
    </element>
    <complexType name="DimensionSliceType">
        <complexContent>
            <extension base="wcs:DimensionSubsetType">
                <sequence>
                    <element name="SlicePoint" type="double"/>
                </sequence>
            </extension>
        </complexContent>
    </complexType>
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class GetCoverageRequest extends BaseRequest {
    
    public static final String GRID_COVERAGE = "GridCoverage";
    public static final String RECTIFIED_GRID_COVERAGE = "RectifiedGridCoverage";
    public static final String REFERENCEABLE_GRID_COVERAGE = "ReferenceableGridCoverage";
    public static final String MULTIPOINT_COVERAGE = "MultiPointCoverage";

    private final String coverageId;
    private final String format;
    private final boolean multipart;
    private final List<DimensionSubset> subsets;

    public GetCoverageRequest(String coverageId) {
        this(coverageId, FormatExtension.MIME_GML, false);
    }

    public GetCoverageRequest(String coverageId, String format, boolean multipart) {
        this.coverageId = coverageId;
        this.format = format;
        this.multipart = multipart;
        this.subsets = new ArrayList<DimensionSubset>();
    }

    public String getCoverageId() {
        return coverageId;
    }

    public List<DimensionSubset> getSubsets() {
        return subsets;
    }

    public String getFormat() {
        return format;
    }

    public boolean isMultipart() {
        return multipart;
    }

    @Override
    public String toString() {
        return "Coverage: " + coverageId + ", Subsets: " + ListUtil.ltos(subsets, ", ");
    }

    public static class DimensionSubset {

        protected final String dimension;
        protected final String crs;

        public DimensionSubset(String dimension) {
            this(dimension, null);
        }

        public DimensionSubset(String dimension, String crs) {
            this.dimension = dimension;
            this.crs = crs;
        }

        public String getDimension() {
            return dimension;
        }

        public String getCrs() {
            return crs;
        }

        @Override
        public String toString() {
            return dimension + ((crs != null) ? "," + crs : "");
        }
    }

    public static class DimensionTrim extends DimensionSubset {

        private final String trimLow;
        private final String trimHigh;

        public DimensionTrim(String dimension, String trimLow, String trimHigh) {
            this(dimension, null, trimLow, trimHigh);
        }

        public DimensionTrim(String dimension, String crs, String trimLow, String trimHigh) {
            super(dimension, crs);
            this.trimLow = trimLow;
            this.trimHigh = trimHigh;
        }

        public String getTrimHigh() {
            return trimHigh;
        }

        public String getTrimLow() {
            return trimLow;
        }

        @Override
        public String toString() {
            return super.toString() + "(" + trimLow + "," + trimHigh + ")";
        }
    }

    public static class DimensionSlice extends DimensionSubset {

        private final String slicePoint;

        public DimensionSlice(String dimension, String slicePoint) {
            this(dimension, null, slicePoint);
        }

        public DimensionSlice(String dimension, String crs, String slicePoint) {
            super(dimension, crs);
            this.slicePoint = slicePoint;
        }

        public String getSlicePoint() {
            return slicePoint;
        }

        @Override
        public String toString() {
            return super.toString() + "(" + slicePoint + ")";
        }
    }
}
