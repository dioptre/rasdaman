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
package petascope.wcs2.templates;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import nu.xom.Element;
import org.apache.commons.io.IOUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.Pair;
import petascope.util.XMLUtil;

/**
 * Load templates, etc.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 * @author <a href="mailto:v.bajpai@jacobs-university.de">Vaibhav Bajpai</a>
 */
public class Templates {

    private static final Logger log = LoggerFactory.getLogger(Templates.class);
    public static final String TEMPLATES_DIR = "petascope/wcs2/templates";
    public static final String TEMPLATES_EXT = ".templ";
    public static final String SERVICE_METADATA = "ServiceMetadata";
    public static final String OPERATIONS_METADATA = "OperationsMetadata";
    public static final String SERVICE_IDENTIFICATION = "ServiceIdentification";
    public static final String SERVICE_PROVIDER = "ServiceProvider";
    public static final String COVERAGE_DESCRIPTION = "CoverageDescription";
    public static final String RANGE_FIELD = "RangeField";
    public static final String GRID_COVERAGE = "GridCoverage";
    public static final String RECTIFIED_GRID_COVERAGE = "RectifiedGridCoverage";
    public static final String MULTIPOINT_COVERAGE = "MultiPointCoverage";
    public static final String MULTIPOINT_POINTMEMBERS = "PointMembers";
    public static final String SOAP_MESSAGE = "SOAPMessage";
    public static final String SOAP_FAULT = "SOAPFault";
    public static final String EXCEPTION_REPORT = "ExceptionReport";
    
    /**
     * template name -> (template as string, template as XOM element)
     */
    private static final Map<String, String> templates = new HashMap<String, String>();

    // load templates
    static {
        loadTemplate(SERVICE_METADATA);
        loadTemplate(OPERATIONS_METADATA);
        loadTemplate(SERVICE_IDENTIFICATION);
        loadTemplate(SERVICE_PROVIDER);
        loadTemplate(COVERAGE_DESCRIPTION);
        loadTemplate(RANGE_FIELD);
        loadTemplate(GRID_COVERAGE);
        loadTemplate(RECTIFIED_GRID_COVERAGE);
        loadTemplate(MULTIPOINT_COVERAGE);
        loadTemplate(MULTIPOINT_POINTMEMBERS);
        loadTemplate(SOAP_MESSAGE);
        loadTemplate(SOAP_FAULT);
        loadTemplate(EXCEPTION_REPORT);
    }

    public static void loadTemplate(String template) {
        try {
            String contents = IOUtils.toString(Templates.class.getResourceAsStream(template + TEMPLATES_EXT));
            templates.put(template, contents);
        } catch (IOException ex) {
            log.warn("Error loading template " + template, ex);
        }
    }

    /**
     * @return in case parsing of contents fails, this returns null
     */
    private static Element parseSafe(String contents) {
        try {
            return XMLUtil.buildDocument(null, contents).getRootElement();
        } catch (Exception ex) {
            // nop - we might have non-XML documents
            return null;
        }
    }

    /**
     * Load a template file from this package.
     *
     * @param template name of template
     * @return the template
     */
    public static String getTemplate(String template) {
        return templates.get(template);
    }

    /**
     * Load a template file from this package.
     *
     * @param template name of template
     * @return the template
     */
    public static String getTemplate(String template, Pair<String, String>... replacements) {
        String ret = getTemplate(template);
        if (ret != null) {
            for (Pair<String, String> p : replacements) {
                ret = ret.replaceAll(p.fst, p.snd);
            }
        }
        return ret;
    }

    /**
     * Load a template file from this package.
     *
     * @param templateName name of template
     * @return the template
     */
    public static Element getXmlTemplate(String template, Pair<String, String>... replacements) {
        return parseSafe(getTemplate(template, replacements));
    }

    public static void updateTemplate(String template, String contents) {
        templates.put(template, contents);
    }
}
