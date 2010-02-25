/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */
package petascope.wcs2.server.ops;

//~--- non-JDK imports --------------------------------------------------------
import java.util.HashSet;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import petascope.ConfigManager;

import petascope.wcps.server.core.CellDomainElement;
import petascope.wcps.server.core.DbMetadataSource;
import petascope.wcps.server.core.DomainElement;
import petascope.wcps.server.core.Metadata;
import petascope.wcps.server.core.RangeElement;

import petascope.wcs.server.exceptions.WCSException;

//~--- JDK imports ------------------------------------------------------------

import java.util.Iterator;
import java.util.List;
import java.util.Set;
import petascope.wcps.server.core.SDU;
import petascope.wcs.server.exceptions.InvalidParameterValueException;
import petascope.wcs.server.exceptions.InvalidServiceConfigurationException;
import petascope.wcs.server.exceptions.NoApplicableCodeException;

/**
 * Describe Coverage operation for WCS 2.0
 *
 * @author Andrei Aiordachioaie
 */
public class DescribeCoverage implements WcsOperation {

    public static String DATATYPE_URN_PREFIX = "urn:ogc:def:dataType:OGC:1.1:";
    
    private static Logger LOG = LoggerFactory.getLogger(DescribeCoverage.class);
    /* Template XMLs for response types */
    private String DescribeCoverageResponse;
    /* Other useful stuff */
    private String coverageDescriptionTemplate;
    private String listOfFormats;
    private String low, high, axisLabels;
    private DbMetadataSource meta;
    private String rangeComponentTemplate;
    private String rangeComponents, gridDimension, gridId;

    public DescribeCoverage(DbMetadataSource metadata) throws InvalidServiceConfigurationException {
        meta = metadata;

        DescribeCoverageResponse = ConfigManager.WCS2_DESCRIBE_COVERAGE_TEMPLATE;
        if (DescribeCoverageResponse == null) {
            throw new InvalidServiceConfigurationException("Could not find template file.");
        }
        listOfFormats = "application/xml";

        /* Find the CoverageDescription template string */
        String starttag = "<wcs:CoverageDescription ", endtag = "</wcs:CoverageDescription>";
        int start = DescribeCoverageResponse.indexOf(starttag);
        int end = DescribeCoverageResponse.indexOf(endtag);

        coverageDescriptionTemplate = DescribeCoverageResponse.substring(start, end + endtag.length());
        String newResponse = DescribeCoverageResponse.substring(0, start - 1) + DescribeCoverageResponse.substring(end + endtag.length());

        DescribeCoverageResponse = newResponse;

        /* Find the RangeComponent template string */
        starttag = "<gmlwcs:rangeField";
        endtag = "</gmlwcs:rangeField>";
        start = coverageDescriptionTemplate.indexOf(starttag);
        end = coverageDescriptionTemplate.indexOf(endtag);
        rangeComponentTemplate = coverageDescriptionTemplate.substring(start, end + endtag.length());
        String newTemplate = coverageDescriptionTemplate.substring(0, start - 1) + coverageDescriptionTemplate.substring(end + endtag.length());

        coverageDescriptionTemplate = newTemplate;
    }

    @Override
    public String execute(String stringXml) throws WCSException {
        String output;
        String desc = "";

        String listOfCoverageDescriptions = "";
        int start, end;

        start = stringXml.indexOf("gml:id=\"");
        while (start != -1) {
            end = stringXml.substring(start + 8).indexOf("\"");
            String coverageId = stringXml.substring(start + 8, start + 8 + end);

            stringXml = stringXml.substring(start + 8 + end + 1);
            start = stringXml.indexOf("gml:id=\"");

            // Describe "coverageId"
            if (meta.existsCoverageName(coverageId)) {
                Metadata cov = null;

                // Read in coverage metadata
                try {
                    cov = meta.read(coverageId);
                } catch (Exception e) {
                    e.printStackTrace();

                    throw new NoApplicableCodeException("Metadata for coverage '" + coverageId + "' is not valid.");
                }

                // Analyze the grid components and their values
                Iterator<DomainElement> it = cov.getDomainIterator();
                Iterator<CellDomainElement> it2 = cov.getCellDomainIterator();

                low = "";
                high = "";
                axisLabels = "";
                while (it.hasNext() && it2.hasNext()) {
                    DomainElement dom = it.next();
                    CellDomainElement cell = it2.next();

                    axisLabels += dom.getName() + " ";
                    low += cell.getLo() + " ";
                    high += cell.getHi() + " ";
                }

                // Build the range components data structure
                Iterator<RangeElement> it3 = cov.getRangeIterator();
                Iterator<String> nilIterator = cov.getNullSetIterator();


                rangeComponents = "";
                int i = -1;
                while (it3.hasNext()) {
                    String component = rangeComponentTemplate;
                    RangeElement range = it3.next();
                    i++;
                    String rangeId = "range-" + cov.getCoverageId() + "-" + range.getName();

                    component = component.replaceAll("\\{rangeFieldId\\}", rangeId);
                    component = component.replaceAll("\\{fieldName\\}", range.getName());
                    component = component.replaceAll("\\{datatype\\}", DATATYPE_URN_PREFIX + range.getType());

                    // Compute the null values for this range field
                    Set<String> nullVals = new HashSet<String>();
                    Iterator<String> it4 = cov.getNullSetIterator();
                    while (it4.hasNext())
                    {
                        List<String> nilVal = SDU.str2string(it4.next());
                        nullVals.add(nilVal.get(i));
                    }
                    StringBuffer nullValsString = new StringBuffer();
                    it4 = nullVals.iterator();
                    while (it4.hasNext())
                        nullValsString.append(" " + it4.next());
                    component = component.replaceAll("\\{nilValues\\}", nullValsString.toString().substring(1));

                    // And add this range field to the range structure
                    rangeComponents += component;
                }

                desc = coverageDescriptionTemplate.replaceAll("\\{coverageId\\}", coverageId);
                gridId = "grid" + coverageId;
                desc = desc.replaceAll("\\{gridId\\}", gridId);
                gridDimension = String.valueOf(cov.getDimension());
                desc = desc.replaceAll("\\{gridDimension\\}", gridDimension);
                desc = desc.replaceAll("\\{low\\}", low);
                desc = desc.replaceAll("\\{high\\}", high);
                desc = desc.replaceAll("\\{axisLabels\\}", axisLabels);
                desc = desc.replaceAll("\\{rangeFields\\}", rangeComponents);
                desc = desc.replaceAll("\\{rangeStructureId\\}", "rangeStructure-" + coverageId);
                desc = desc.replaceAll("\\{supportedFormats\\}", listOfFormats);

                listOfCoverageDescriptions += desc;
            } else {
                throw new InvalidParameterValueException("gml:id");
            }
        }

        output = DescribeCoverageResponse.replaceAll("\\{listOfCoverageDescriptions\\}",
                listOfCoverageDescriptions);

        output = output.replaceAll("\\{wcsSchemaUrl\\}", ConfigManager.WCS2_SCHEMA_URL);

        return output;
    }
}
