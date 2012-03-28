/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 *
 * Original author    Normen Müller <n.mueller@jacobs-university.de>
 * Web                http://kwarc.info/nmueller/
 * Created            Oct 18, 2007
 * Filename           $Id: XMLSymbols.java 2188 2010-11-03 09:29:09Z dmisev $
 * Revision           $Revision: 2188 $
 *
 * Last modified on   $Date:2007-10-25 18:50:01 +0200 (Thu, 25 Oct 2007) $
 *               by   $Author:nmueller $
 * 
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */
package petascope.util;


import nu.xom.XPathContext;

/**
 * All recognized XML symbols, including prefixes, namespaces, element labels,
 * attribute names, XPath contexts, and other XML-related constants.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public interface XMLSymbols {

    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Prefixes
    //
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String PREFIX_XML = "xml";
    String PREFIX_XMLNS = "xmlns";
    String PREFIX_GML = "gml";
    String PREFIX_GMLCOV = "gmlcov";
    String PREFIX_SWE = "swe";
    String PREFIX_OWS = "ows";
    String PREFIX_WCS = "wcs";
    String PREFIX_WSDL = "wsdl";
    String PREFIX_XSI = "xsi";
    String PREFIX_XLINK = "xlink";
    String PREFIX_CRS = "crs";
    
    
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Namespaces
    //
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String NAMESPACE_XML = "http://www.w3.org/XML/1998/namespace";
    String NAMESPACE_GML = "http://www.opengis.net/gml/3.2";
    String NAMESPACE_GMLCOV = "http://www.opengis.net/gmlcov/1.0";
    String NAMESPACE_SWE = "http://www.opengis.net/swe/2.0";
    String NAMESPACE_OWS = "http://www.opengis.net/ows/2.0";
    String NAMESPACE_WCS_OLD = "http://www.opengis.net/wcs/1.1";
    String NAMESPACE_WCS = "http://www.opengis.net/wcs/2.0";
    String NAMESPACE_WSDL = "http://schemas.xmlsoap.org/wsdl/";
    String NAMESPACE_XSI = "http://www.w3.org/2001/XMLSchema-instance";
    String NAMESPACE_XLINK = "http://www.w3.org/1999/xlink";
    String NAMESPACE_CRS = "http://www.opengis.net/wcs/service-extension/crs/1.0";
    
    
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // XML element labels
    //
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // wcs
    String LABEL_SERVICE_METADATA = "ServiceMetadata";
    String LABEL_CAPABILITIES = "Capabilities";
    String LABEL_CONTENTS = "Contents";
    String LABEL_COVERAGE_SUMMARY = "CoverageSummary";
    String LABEL_COVERAGE_ID = "CoverageId";
    String LABEL_COVERAGE_SUBTYPE = "CoverageSubtype";
    String LABEL_COVERAGE_DESCRIPTIONS = "CoverageDescriptions";
    String LABEL_COVERAGE_DESCRIPTION = "CoverageDescription";
    String LABEL_DIMENSION = "Dimension";
    String LABEL_DIMENSION_TRIM = "DimensionTrim";
    String LABEL_DIMENSION_SLICE = "DimensionSlice";
    String LABEL_TRIM_LOW = "TrimLow";
    String LABEL_TRIM_HIGH = "TrimHigh";
    String LABEL_SLICE_POINT = "SlicePoint";
    String LABEL_WGS84_BBOX = "Wgs84BoundingBox";
    String LABEL_BBOX = "BoundingBox";
    String LABEL_BUONDEDBY = "BoundedBy";
    // wcs crs-extension
    String LABEL_CRS_METADATA = "CrsMetadata";
    String LABEL_CRS = "CRS";

    // ows
    String LABEL_SERVICE_IDENTIFICATION = "ServiceIdentification";
    String LABEL_TITLE = "ServiceIdentification";
    String LABEL_ABSTRACT = "ServiceIdentification";
    String LABEL_SERVICE_TYPE = "ServiceType";
    String LABEL_SERVICE_TYPE_VERSION = "ServiceTypeVersion";
    String LABEL_PROFILE = "Profile";
    String LABEL_SERVICE_PROVIDER = "ServiceProvider";
    String LABEL_PROVIDER_NAME = "ProviderName";
    String LABEL_PROVIDER_SITE = "ProviderSite";
    String LABEL_SERVICE_CONTACT = "ServiceContact";
    String LABEL_POSITION_NAME = "PositionName";
    String LABEL_OPERATIONS_METADATA = "OperationsMetadata";
    String LABEL_OPERATION = "Operation";
    String LABEL_DCP = "DCP";
    String LABEL_HTTP = "HTTP";
    String LABEL_GET = "Get";
    String LABEL_VERSION = "Version";
    String LABEL_ACCEPT_VERSIONS = "AcceptVersions";
    String LABEL_ACCEPT_FORMATS = "AcceptFormats";
    String LABEL_ACCEPT_LANGUAGES = "AcceptLanguages";
    
    // soap
    String LABEL_BODY = "Body";
    
    
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Attributes
    //
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String ATT_SERVICE = "service";
    String ATT_VERSION = "version";
    String ATT_SCHEMA_LOCATION = "schemaLocation";
    
    // crs-extension
    String ATT_SUPPORTED_CRS = "supportedCrs";
    String ATT_SUBSET_CRS = "subsettingCrs";
    String ATT_OUTPUT_CRS = "outputCrs";
    // bbox
    String ATT_LOWERCORNER = "LowerCorner";
    String ATT_UPPERCORNER = "UpperCorner";
    String ATT_CRS = "crs";
    String ATT_DIMENSIONS = "dimensions";

    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // XPath Contexts
    //
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    XPathContext CTX_XML = new XPathContext(PREFIX_XML, NAMESPACE_XML);
    XPathContext CTX_WCS = new XPathContext(PREFIX_WCS, NAMESPACE_WCS);
    XPathContext CTX_CRS = new XPathContext(PREFIX_CRS, NAMESPACE_CRS);
}
