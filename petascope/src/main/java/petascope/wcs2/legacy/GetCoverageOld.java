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

import org.apache.commons.io.IOUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import petascope.ConfigManager;
import petascope.core.DbMetadataSource;
import petascope.core.Metadata;
import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;
import java.io.IOException;
import java.math.BigInteger;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import javax.servlet.http.HttpServletResponse;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import org.odmg.DBag;
import org.odmg.Database;
import org.odmg.Implementation;
import org.odmg.ODMGException;
import org.odmg.OQLQuery;
import org.odmg.QueryException;
import org.odmg.Transaction;
import rasj.RasGMArray;
import rasj.RasImplementation;
import petascope.wcps.server.core.CellDomainElement;
import petascope.wcps.server.core.DomainElement;
import petascope.wcps.server.core.RangeElement;
import petascope.wcps.server.core.SDU;
import petascope.wcs2.parsers.GetCoverageMetadata.RangeField;
import petascope.wcs2.templates.Templates;
import petascope.wcs2.templates.WcsNamespaceContext;

/**
 * Get Coverage operation for WCS 2.0
 *
 * @author Andrei Aiordachioaie
 */
public class GetCoverageOld {

    private static Logger log = LoggerFactory.getLogger(GetCoverageOld.class);
    private DocumentBuilder builder = null;
    private XPathFactory xpathFactory = XPathFactory.newInstance();
    /* Template XMLs for response types */
    private String GetCoverageResponse;
    private String rangeComponentTemplate;
    /* Xml request */
    private Document doc;
    /* for Metadata */
    private DbMetadataSource meta;
    /* The new coverage domain */
    private String lowPoint, highPoint, newAxesLabels;

    public GetCoverageOld(DbMetadataSource metadata) throws WCSException {
        meta = metadata;
        GetCoverageResponse = Templates.getTemplate(Templates.GRID_COVERAGE);
        if (GetCoverageResponse == null) {
            throw new WCSException(ExceptionCode.InvalidServiceConfiguration, "Could not find template file.");
        }

        /* Find the RangeField template string */
        String starttag = "<gmlwcs:rangeField", endtag = "</gmlwcs:rangeField>";
        int start = GetCoverageResponse.indexOf(starttag);
        int end = GetCoverageResponse.indexOf(endtag);
        rangeComponentTemplate = GetCoverageResponse.substring(start, end + endtag.length());
        String newTemplate = GetCoverageResponse.substring(0, start - 1) + GetCoverageResponse.substring(end + endtag.length());
        GetCoverageResponse = newTemplate;

        /* init XML parser */
        DocumentBuilderFactory domFactory = DocumentBuilderFactory.newInstance();
        domFactory.setNamespaceAware(true);		// never forget this!
        try {
            builder = domFactory.newDocumentBuilder();
        } catch (Exception e) {
            throw new WCSException(ExceptionCode.NoApplicableCode, "Error initializing XML parser", e);
        }
    }

    public String handle(String stringXml, HttpServletResponse response) throws WCSException {
        String output;
        Metadata cov;

        try {
            doc = builder.parse(IOUtils.toInputStream(stringXml));
            cov = readCoverageMetadata();
        } catch (SAXException e) {
            throw new WCSException(ExceptionCode.XmlNotValid, "Could not parse input request.", e);
        } catch (IOException e) {
            throw new WCSException(ExceptionCode.IOConnectionError, "Could not read input request", e);
        } catch (XPathExpressionException e) {
            throw new WCSException(ExceptionCode.InternalComponentError, "Invalid XPath expression", e);
        }

        // Analyze input and build trim/slice info structures
        String subsetting = computeRequestSubsettingLimits(cov);

        // Evaluate all bands and then perform string processing to get coverage data
        String coverageData = buildCoverageData(cov, subsetting);

        // Build response xml document
        output = buildOutputXml(coverageData, cov);

        return output;
    }

    private NodeList evalXPathList(String query, Object context) throws WCSException {
        try {
            log.debug("Evaluating XPath: {}", query);
            XPath xpath = xpathFactory.newXPath();

            xpath.setNamespaceContext(new WcsNamespaceContext());
            NodeList result = (NodeList) xpath.evaluate(query, context, XPathConstants.NODESET);

            for (int i = 0; i < result.getLength(); i++) {
                log.trace("Result {}: {}", i, result.item(i).getNodeValue());
            }

            return (NodeList) result;
        } catch (XPathExpressionException e) {
            log.error("Could not execute XPath expression '{}'", query);

            throw new WCSException("Could not execute XPath expression", e);
        }
    }

    private String getCoverageName(Document doc) throws WCSException {
        NodeList nodes = evalXPathList("//wcs:id/@gml:id", doc);

        if (nodes.getLength() != 1) {
            log.error("Could not retrieve coverage name from XML document.");

            return null;
        }

        return nodes.item(0).getNodeValue();
    }

    private Metadata readCoverageMetadata() throws WCSException, XPathExpressionException {
        String coverageId = getCoverageName(doc);

        if (meta.existsCoverageName(coverageId)) {
            try {
                Metadata cov = meta.read(coverageId);

                return cov;
            } catch (Exception e) {
                e.printStackTrace();

                throw new WCSException(ExceptionCode.NoApplicableCode, "Metadata for coverage '"
                        + coverageId + "' is not valid.");
            }
        } else {
            throw new WCSException(ExceptionCode.InvalidParameterValue, "gml:id");
        }
    }

    /** Computes the domain of the new coverage, and returns a string that can be
     * used to do subsetting on the original coverage. Also computes the low, high
     * and the axis labels for the new coverage.
     *
     * @param coverage
     * @return
     * @throws WCSException
     */
    private String computeRequestSubsettingLimits(Metadata coverage) throws WCSException {
        int dims = coverage.getDimension(), i = 0;
        String[] limits = new String[dims];
        BigInteger[] high = new BigInteger[dims];
        BigInteger[] low = new BigInteger[dims];
        String[] axesLabels = new String[dims];
        boolean[] sliced = new boolean[dims];
        boolean[] trimmed = new boolean[dims];

        Iterator<CellDomainElement> it = coverage.getCellDomainIterator();
        Iterator<DomainElement> it2 = coverage.getDomainIterator();
        i = 0;
        while (it.hasNext() && it2.hasNext()) {
            CellDomainElement cell = it.next();
            DomainElement dom = it2.next();
            log.trace(cell.toString());
            log.trace(dom.toString());
            high[i] = cell.getHi();
            low[i] = cell.getLo();
            axesLabels[i] = dom.getName();
            limits[i] = low[i] + ":" + high[i];
            sliced[i] = false;
            trimmed[i] = false;
            i++;
        }

        NodeList list = null;
        int axisIndex;
        String axis, root;
        NodeList trims, slices;

        trims = evalXPathList("//wcs:trimDimension", doc);
        for (i = 0; i < trims.getLength(); i++) {
            Node trim = trims.item(i);

            list = evalXPathList("wcs:dimension/text()", trim);
            axis = list.item(0).getNodeValue();
            axisIndex = coverage.getDomainIndexByName(axis);
            if (axisIndex == -1) {
                throw new WCSException(ExceptionCode.InvalidParameterValue, "dimension. Explanation: Unknown axis: " + axis);
            }
            if (trimmed[axisIndex] || sliced[axisIndex]) {
                throw new WCSException(ExceptionCode.NoApplicableCode, "Already performed one subsetting operation on axis: " + axis);
            }

            list = evalXPathList("wcs:trimLow/text()", trim);
            if (list.getLength() > 0) {
                try {
                    long val = Long.parseLong(list.item(0).getNodeValue());
                    low[axisIndex] = BigInteger.valueOf(val);
                } catch (NumberFormatException e) {
                    throw new WCSException(ExceptionCode.InvalidParameterValue, "trimLow. Explanation: invalid integer number: " + list.item(0).
                            getNodeValue());
                }
            }
            list = evalXPathList("wcs:trimHigh/text()", trim);
            if (list.getLength() > 0) {
                try {
                    long val = Long.parseLong(list.item(0).getNodeValue());
                    high[axisIndex] = BigInteger.valueOf(val);
                } catch (NumberFormatException e) {
                    throw new WCSException(ExceptionCode.InvalidParameterValue, "trimHigh. Explanation: invalid integer number: " + list.item(0).
                            getNodeValue());
                }
            }

            trimmed[axisIndex] = true;
            limits[axisIndex] = low[axisIndex] + ":" + high[axisIndex];
            log.debug("New limits for axis {}: {}", axis, limits[axisIndex]);
        }

        slices = evalXPathList("//wcs:sliceDimension", doc);
        for (i = 0; i < slices.getLength(); i++) {
            Node slice = slices.item(i);

            list = evalXPathList("wcs:dimension/text()", slice);
            axis = list.item(0).getNodeValue();
            axisIndex = coverage.getDomainIndexByName(axis);
            if (axisIndex == -1) {
                throw new WCSException(ExceptionCode.InvalidParameterValue, "dimension. Explanation: Unknown axis name: " + axis);
            }
            if (trimmed[axisIndex] || sliced[axisIndex]) {
                throw new WCSException(ExceptionCode.NoApplicableCode, "Already performed one subsetting operation on axis: " + axis);
            }

            list = evalXPathList("wcs:slicePoint/text()", slice);
            if (list.getLength() > 0) {
                try {
                    long point = Long.parseLong(list.item(0).getNodeValue());
                    low[axisIndex] = BigInteger.valueOf(point);
                } catch (NumberFormatException e) {
                    throw new WCSException(ExceptionCode.InvalidParameterValue, "slicePoint. Explanation: invalid integer number: " + list.item(0).
                            getNodeValue());
                }
                high[axisIndex] = low[axisIndex];
                limits[axisIndex] = list.item(0).getNodeValue();
            }

            sliced[axisIndex] = true;
            log.debug("New limits for axis {}: {}", axis, limits[axisIndex]);
        }

        // Compute the lowest, highest point and the labels
        lowPoint = "";
        highPoint = "";
        newAxesLabels = "";
        int first = 0;
        for (i = 0; i < dims; i++) {
            if (sliced[i] == false) {
                if (first == 0) {
                    lowPoint = low[i].toString();
                    highPoint = high[i].toString();
                    newAxesLabels = axesLabels[i];
                    first++;
                } else {
                    lowPoint += " " + low[i];
                    highPoint += " " + high[i];
                    newAxesLabels += " " + axesLabels[i];
                }
            }
        }

        // Concatenate all limits into a single string
        String result = limits[0];

        for (i = 1; i < dims; i++) {
            result += ", " + limits[i];
        }

        return result;
    }

    public String executeRasqlQuery(String query) throws WCSException {
        byte[] result = null;

        Implementation impl = new RasImplementation(ConfigManager.RASDAMAN_URL);
        Database db = impl.newDatabase();

        try {
            db.open(ConfigManager.RASDAMAN_DATABASE, Database.OPEN_READ_ONLY);
        } catch (ODMGException odmge) {
            try {
                db.close();
            } catch (ODMGException e) {
            }

            throw new WCSException(ExceptionCode.InternalComponentError,
                    "Could not connect to rasdaman server at " + ConfigManager.RASDAMAN_URL
                    + ", database " + ConfigManager.RASDAMAN_DATABASE, odmge);
        }

        Transaction tr = impl.newTransaction();

        tr.begin();
        OQLQuery q = impl.newOQLQuery();
        DBag resultSet;

        try {
            q.create(query);
            resultSet = (DBag) q.execute();

            if (resultSet != null) {
                Iterator resultIterator = resultSet.iterator();

                if (resultIterator.hasNext()) {
                    Object current = resultIterator.next();

                    try {
                        RasGMArray resultArray = (RasGMArray) current;
                        result = resultArray.getArray();
                    } catch (ClassCastException e) {
                        log.error("result=" + current.toString());
                        result = current.toString().getBytes();
                    }
                }
            }
        } catch (QueryException qe) {
            tr.commit();

            try {
                db.close();
            } catch (ODMGException odmge) {
            }

            throw new WCSException(ExceptionCode.InternalSqlError,
                    "Could not evaluate rasdaman query: '" + query + "'. Cause: " + qe.getMessage(), qe);
        }

        tr.commit();

        try {
            db.close();
        } catch (ODMGException odmge) {
        }

        return new String(result);
    }

    /** Creates a string with the contents of the GetCoverage response XML */
    private String buildOutputXml(String coverageData, Metadata coverage) {
        String xml = GetCoverageResponse;
        xml = xml.replaceAll("\\{coverageId\\}", coverage.getCoverageName() + Math.random());
        xml = xml.replaceAll("\\{gridDimension\\}", String.valueOf(coverage.getDimension()));
        xml = xml.replaceAll("\\{gridId\\}", "grid-" + coverage.getCoverageName());
        // low
        xml = xml.replaceAll("\\{low\\}", lowPoint);
        // high
        xml = xml.replaceAll("\\{high\\}", highPoint);
        // axisLabels
        xml = xml.replaceAll("\\{axisLabels\\}", newAxesLabels);
        // coverageData
        xml = xml.replaceAll("\\{coverageData\\}", coverageData);



        // Build the range structure
        Iterator<RangeElement> it3 = coverage.getRangeIterator();
        int i = -1;
        String rangeComponents = "";
        while (it3.hasNext()) {
            String component = rangeComponentTemplate;
            RangeElement range = it3.next();
            i++;
            String rangeId = "range-" + coverage.getCoverageId() + "-" + range.getName();
            log.trace(range.toString());

            component = component.replaceAll("\\{rangeFieldId\\}", rangeId);
            component = component.replaceAll("\\{fieldName\\}", range.getName());
            component = component.replaceAll("\\{datatype\\}", RangeField.DATATYPE_URN_PREFIX + range.getType());

            // Compute the null values for this range field
            Set<String> nullVals = new HashSet<String>();
            Iterator<String> it = coverage.getNullSetIterator();
            while (it.hasNext()) {
                List<String> nilVal = SDU.str2string(it.next());
                nullVals.add(nilVal.get(i));
            }
            StringBuffer nullValsString = new StringBuffer();
            it = nullVals.iterator();
            while (it.hasNext()) {
                nullValsString.append(" " + it.next());
            }
            component = component.replaceAll("\\{nilValues\\}", nullValsString.toString().substring(1));

            // And add this range field to the range structure
            rangeComponents += component;
        }

        xml = xml.replaceAll("\\{rangeFields\\}", rangeComponents);
        xml = xml.replaceAll("\\{rangeStructureId\\}", "rangeStructure-" + coverage.getCoverageId());

        return xml;
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
    private String buildCoverageData(Metadata coverage, String subsetting) throws WCSException {
        String coverageName = coverage.getCoverageName();
        Iterator<RangeElement> it = coverage.getRangeIterator();
        int bandcount = 0;
        while (it.hasNext()) {
            it.next();
            bandcount++;
        }
        log.debug("Coverage {} has {} bands", coverageName, bandcount);
        String[][] pixels = new String[bandcount][];
        String currentBand = "";
        /* For all bands of the coverage, execute a rasql query */
        for (int band = 0; band < bandcount; band++) {
            log.trace("Processing band {}", band);
            // Construct rasql query
            currentBand = "." + band;
            // If this is a one-band image, then band-subsetting would result in an error
            if (bandcount == 1) {
                currentBand = "";
            }
            String rasqlQuery = "select csv(cov[" + subsetting + "]" + currentBand + ") "
                    + "from " + coverageName + " as cov";

            // Execute RasQl query => coverage data
            log.trace("Executing query {}", rasqlQuery);
            String output = executeRasqlQuery(rasqlQuery);

            // Remove the curly braces from the rasql output
            log.trace("Removing curly braces...");
            output = output.replaceAll("\\{", "");
            output = output.replaceAll("\\}", "");

            // Tokenize the input to get the pixel values of the current band
            log.trace("Splitting values with comma...");
            pixels[band] = output.split(",");

            log.trace("Done processing band {}.", band);
        }

        /* Combine all bands into one single string */
        int pixelcount = pixels[0].length;
        StringBuilder data = new StringBuilder(pixelcount * bandcount);
        log.debug("Going to combine {} pixels with {} bands...", pixelcount, bandcount);
        for (int pix = 0; pix < pixelcount; pix++) {
            if (pixelcount > 20 && pix % (pixelcount / 20) == 0) {
                log.debug("Processing Pixel {} of " + pixelcount + " - {}%", pix, pix * 100 / (pixelcount - 1));
            }
            for (int b = 0; b < bandcount - 1; b++) {
                data.append(pixels[b][pix] + ",");
            }
            data.append(pixels[bandcount - 1][pix]);
            data.append(" ");
        }
        return data.substring(0, data.length() - 2);
    }
}
