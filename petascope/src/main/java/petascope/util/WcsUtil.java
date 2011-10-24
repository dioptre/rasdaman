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
package petascope.util;

import java.io.StringWriter;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import javax.xml.bind.JAXBException;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.PetascopeXmlNamespaceMapper;
import petascope.core.DbMetadataSource;
import petascope.core.Metadata;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.PetascopeException;
import petascope.exceptions.WCSException;
import petascope.wcps.server.core.Wgs84Crs;
import petascope.wcs2.parsers.GetCoverageMetadata;
import petascope.wcs2.parsers.GetCoverageMetadata.RangeField;
import petascope.wcs2.parsers.GetCoverageRequest;
import petascope.wcs2.templates.Templates;

/**
 * WCS utility methods.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class WcsUtil {

    private static final Logger log = LoggerFactory.getLogger(WcsUtil.class);

    private static final Map<String, Metadata> metadataCache = new HashMap<String, Metadata>();

    /**
     * Utility method to read coverage's metadata, using caching for performance.
     */
    public static Metadata getMetadata(DbMetadataSource meta, String coverageId) throws WCSException {
        if (metadataCache.containsKey(coverageId)) {
            return metadataCache.get(coverageId);
        } else {
            try {
                Metadata ret = meta.read(coverageId);
                metadataCache.put(coverageId, ret);
                return ret;
            } catch (Exception e) {
                throw new WCSException(ExceptionCode.NoApplicableCode.locator(coverageId),
                        "Metadata for coverage '" + coverageId + "' is not valid.");
            }
        }
    }

    public static void clearMetadataCache() {
        metadataCache.clear();
    }

    /**
     * Convert csv format from rasdaman into a tupleList format, for including in a gml:DataBlock
     *
     * @param csv coverage in csv format
     * @return tupleList representation
     */
    public static String csv2tupleList(String csv) {
        return csv; // FIXME
    }

    /**
     * Convert spatial domain of the form [band1][band2]..., where band1 is of the form [low:high, low:high,...]
     *
     * @param sdom spatial domain as retreived from rasdaman with sdom(coverage)
     * @return (low, high) bound
     */
    public static Pair<String, String> sdom2bounds(String sdom) {
        sdom = sdom.replaceAll("\\[", "");
        sdom = sdom.substring(0, sdom.length() - 1);
        String[] bands = sdom.split("\\]");

        int n = bands[0].split(",").length;
        Double[] low = new Double[n];
        Arrays.fill(low, Double.MAX_VALUE);
        Double[] high = new Double[n];
        Arrays.fill(high, Double.MIN_VALUE);

        for (String band : bands) {
            String[] dims = band.split(",");
            for (int i = 0; i < dims.length; i++) {
                String[] bounds = dims[i].split(":");
                try {
                    Double l = Double.parseDouble(bounds[0]);
                    low[i] = Math.min(low[i], l);
                } catch (NumberFormatException ex) {
                    log.warn("Error parsing " + bounds[0], ex);
                }
                try {
                    Double h = Double.parseDouble(bounds[1]);
                    high[i] = Math.max(high[i], h);
                } catch (NumberFormatException ex) {
                    log.warn("Error parsing " + bounds[1], ex);
                }
            }
        }
        String l = "", h = "";
        for (Double t : low) {
            l += StringUtil.d2s(t) + " ";
        }
        for (Double t : high) {
            h += StringUtil.d2s(t) + " ";
        }
        return Pair.of(l.trim(), h.trim());
    }

    public static String exceptionReportToXml(ExceptionReport report) {
        log.info(report.getException().get(0).getLocator());
        log.info(report.getException().get(0).getExceptionCode());
        String output = null;
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.
                    newInstance(report.getClass().getPackage().getName());
            javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
            marshaller.setProperty("jaxb.formatted.output", true);
            marshaller.setProperty("jaxb.schemaLocation",
                    "http://www.opengis.net/ows http://schemas.opengis.net/ows/2.0/owsExceptionReport.xsd");
            marshaller.setProperty("com.sun.xml.bind.namespacePrefixMapper", new PetascopeXmlNamespaceMapper());
            StringWriter strWriter = new StringWriter();
            marshaller.marshal(report, strWriter);
            output = strWriter.toString();
            String sub = output.substring(output.indexOf("<ows:Exception "), output.
                    indexOf("</ows:ExceptionReport>"));
            log.debug(output);
            log.debug(sub);
            try {
                output = Templates.getTemplate(Templates.EXCEPTION_REPORT, Pair.
                        of("\\{exception\\}", sub));
            } catch (Exception ex) {
                log.warn("Error handling exception report template");
            }
            log.debug("Done marshalling Error Report.");
            log.debug(output);
        } catch (JAXBException e2) {
            log.error("Stack trace: {}", e2);
            log.error("Error stack trace: " + e2);
        }
        return output;
    }

    public static String exceptionToXml(PetascopeException e) {
        return exceptionReportToXml(e.getReport());
    }

    public static String getGML(GetCoverageMetadata m, String template, boolean replaceBounds) {
        String rangeFields = "";
        for (RangeField range : m.getRangeFields()) {
            rangeFields += Templates.getTemplate(Templates.RANGE_FIELD,
                    Pair.of("\\{fieldName\\}", range.getFieldName()),
                    Pair.of("\\{componentName\\}", range.getComponentName()),
                    Pair.of("\\{datatype\\}", range.getDatatype()),
                    Pair.of("\\{nilValues\\}", range.getNilValues()),
                    Pair.of("\\{fieldDescr\\}", range.getDescription()),
                    Pair.of("\\{allowedValues\\}", range.getAllowedValues()),
                    Pair.of("\\{code\\}", range.getUomCode()));
        }

        String ret = Templates.getTemplate(template,
                Pair.of("\\{coverageId\\}", m.getCoverageId()),
                Pair.of("\\{coverageType\\}", m.getCoverageType()),
                Pair.of("\\{gridId\\}", m.getGridId()),
                Pair.of("\\{mpId\\}", "multipoint_" + m.getGridId()),
                Pair.of("\\{gridDimension\\}", String.valueOf(m.getGridDimension())),
                Pair.of("\\{uomLabels\\}", m.getUomLabels()),
                Pair.of("\\{rangeFields\\}", rangeFields),
                Pair.of("\\{coverageSubtype\\}", m.getCoverageType()),
                Pair.of("\\{axisLabels\\}", m.getAxisLabels()),
                Pair.of("\\{gridType\\}", m.getGridType()),
                Pair.of("\\{srsgroup\\}", getSrsGroup(m)),
                Pair.of("\\{additions\\}", getAdditions(m)));

        if (replaceBounds) {
            ret = ret.replaceAll("\\{low\\}", m.getLow())
                    .replaceAll("\\{high\\}", m.getHigh())
                    .replaceAll("\\{axisLabels\\}", m.getAxisLabels());
        }
        return ret;
    }

    private static String getSrsGroup(GetCoverageMetadata m) {
        Wgs84Crs crs = m.getCrs();
        if (crs != null) {
            return " srsName=\"" + crs.getName() + "\" " +
                    "srsDimension=\"" + m.getGridDimension() + "\"";
        } else {
            return "";
        }
    }

    private static String getAdditions(GetCoverageMetadata m) {
        String ret = "";
        Wgs84Crs crs;
        if (m.getCoverageType().equals(GetCoverageRequest.RECTIFIED_GRID_COVERAGE)) {
            if (m.getCrs() != null) {
                crs = m.getCrs();
                return Templates.getTemplate(Templates.RECTIFIED_GRID_COVERAGE,
                        Pair.of("\\{pointId\\}", m.getCoverageId() + "-origin"),
                        Pair.of("\\{srsName\\}", crs.getName()),
                        Pair.of("\\{originPos\\}", crs.getLow1() + " " + crs.getLow2()),
                        Pair.of("\\{offset1\\}", crs.getOffset1() + ""),
                        Pair.of("\\{offset2\\}", crs.getOffset2() + ""));
            }
        }
        return ret;
    }

    public static Pair<String, String> toInterval(String type) {
        if (type.equals("char")) {
            return Pair.of("-128", "128");
        } else if (type.equals("unsigned char")) {
            return Pair.of("0", "255");
        } else if (type.equals("short")) {
            return Pair.of("-32768", "32767");
        } else if (type.equals("unsigned short")) {
            return Pair.of("0", "65535");
        } else if (type.equals("int")) {
            return Pair.of("-2147483648", "2147483647");
        } else if (type.equals("unsigned int")) {
            return Pair.of("0", "4294967295");
        } else if (type.equals("long")) {
            return Pair.of("-9223372036854775808", "9223372036854775807");
        } else if (type.equals("usnigned long")) {
            return Pair.of("0", "18446744073709551615");
        } else if (type.equals("float")) {
            return Pair.of("+/-3.4e-38", "+/-3.4e+38");
        } else if (type.equals("double")) {
            return Pair.of("+/-1.7e- 308", "+/-1.7e+308");
        }
        return Pair.of("", "");
    }
    
    /**
     * @return the minimum interval from a and b
     */
    public static String min(String a, String b) {
        String[] as = a.split(":");
        String[] bs = b.split(":");
        if (as.length < bs.length) {
            return a;
        } else if (as.length > bs.length) {
            return b;
        }
        Integer al = toInt(as, 0);
        Integer bl = toInt(bs, 0);
        if (as.length == 1) {
            if (al < bl) {
                return a;
            } else {
                return b;
            }
        }
        Integer ah = toInt(as, 1);
        Integer bh = toInt(bs, 1);
        
        Integer rl = al;
        if (rl > bl) {
            rl = bl;
        }
        Integer rh = ah;
        if (rh > bh) {
            rh = bh;
        }
        
        return toStr(rl) + ":" + toStr(rh);
    }
    
    private static String toStr(Integer i) {
        if (i == Integer.MAX_VALUE) {
            return "*";
        } else {
            return i.toString();
        }
    }
    
    private static Integer toInt(String[] s, int i) {
        if (s[i].equals("*")) {
            return Integer.MAX_VALUE;
        } else {
            return Integer.parseInt(s[i]);
        }
    }
}
