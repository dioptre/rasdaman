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
package petascope.exceptions;


/**
 * Represents an exception code, as defined in [OGC 06-121r9] Clause 8
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class ExceptionCode {

    /**
     * Code representing type of this exception
     */
    private final String exceptionCode;

    /**
     * Exception code description
     */
    private final String description;

    /**
     * Indicator of location in the client's operation request where this exception was encountered
     */
    private String locator;

    /**
     * HTTP response code, when the exception is returned over HTTP
     */
    private int httpErrorCode;


    /**
     * Create new ExceptionCode
     *
     * @param exceptionCode Code representing type of this exception
     */
    public ExceptionCode(String exceptionCode) {
        this(exceptionCode, null, 0);
    }

    /**
     * Create new ExceptionCode
     *
     * @param exceptionCode Code representing type of this exception
     * @param description Exception code description
     * @param locator Indicator of location in the client's operation request where this exception was encountered
     * @param httpErrorCode HTTP response code, when the exception is returned over HTTP
     */
    public ExceptionCode(String exceptionCode, String description, int httpErrorCode) {
        this.exceptionCode = exceptionCode;
        this.description = description;
        this.httpErrorCode = httpErrorCode;
    }

    /**
     * Use this to set the locator on the fly, while passing the exception code
     *
     * @param locator Indicator of location in the client's operation request where this exception was encountered
     * @return a copy of the original exception code
     */
    public ExceptionCode locator(String locator) {
        ExceptionCode ret = new ExceptionCode(exceptionCode, description, httpErrorCode);
        ret.setLocator(locator);
        return ret;
    }

    /**
     * @return Exception code description
     */
    public String getDescription() {
        return description;
    }

    /**
     * @return Code representing type of this exception
     */
    public String getExceptionCode() {
        return exceptionCode;
    }

    /**
     * @return HTTP response code, when the exception is returned over HTTP
     */
    public int getHttpErrorCode() {
        return httpErrorCode;
    }

    /**
     * @return Indicator of location in the client's operation request where this exception was encountered
     */
    public String getLocator() {
        return locator;
    }

    /**
     * @param locator Indicator of location in the client's operation request where this exception was encountered
     */
    public void setLocator(String locator) {
        this.locator = locator;
    }

    @Override
    public String toString() {
        return "Exception Code {"
                + "\n  exceptionCode = " + exceptionCode
                + "\n  description = " + description
                + "\n  locator = " + locator
                + "\n  httpErrorCode = " + httpErrorCode
                + "\n}";
    }

    public static final ExceptionCode BadResponseHandler = new ExceptionCode("BadResponseHandler", null, 0);
    public static final ExceptionCode InternalComponentError = new ExceptionCode("InternalComponentError", null, 0);
    public static final ExceptionCode InternalSqlError = new ExceptionCode("InternalSqlError", null, 0);
    public static final ExceptionCode InvalidEncodingSyntax = new ExceptionCode("InvalidEncodingSyntax",
            "Document received does not conform with protocol syntax", 400);
    public static final ExceptionCode InvalidMetadata = new ExceptionCode("InvalidMetadata", null, 0);
    public static final ExceptionCode InvalidParameterValue = new ExceptionCode("InvalidParameterValue",
            "Operation request contains an invalid parameter value", 400);
    public static final ExceptionCode InvalidPropertyValue = new ExceptionCode("InvalidPropertyValue", null, 0);
    public static final ExceptionCode InvalidRequest = new ExceptionCode("InvalidRequest", null, 0);
    public static final ExceptionCode InvalidServiceConfiguration = new ExceptionCode("InvalidServiceConfiguration", null, 0);
    public static final ExceptionCode InvalidTemporalMetadata = new ExceptionCode("InvalidTemporalMetadata", null, 0);
    public static final ExceptionCode InvalidUpdateSequence = new ExceptionCode("InvalidUpdateSequence",
            "Value of (optional) updateSequence parameter in GetCapabilities operation request "
            + "is greater than current value of service metadata updateSequence number", 400);
    public static final ExceptionCode IOConnectionError = new ExceptionCode("IOConnectionError", null, 0);
    public static final ExceptionCode MaliciousQuery = new ExceptionCode("MaliciousQuery", null, 0);
    public static final ExceptionCode MissingParameterValue = new ExceptionCode("MissingParameterValue",
            "Operation request does not include a parameter value, and this server did "
            + "not declare a default value for that parameter. Locator: Name of missing parameter", 400);
    public static final ExceptionCode MultiBandImagesNotSupported = new ExceptionCode("MultiBandImagesNotSupported", null, 0);
    public static final ExceptionCode NoApplicableCode = new ExceptionCode("NoApplicableCode",
            "No other exceptionCode specified by this service and server applies to this exception", 501);
    public static final ExceptionCode NodeParsingNotImplemented = new ExceptionCode("NodeParsingNotImplemented", null, 0);
    public static final ExceptionCode NoSuchCoverage = new ExceptionCode("NoSuchCoverage",
            "One of the identifiers passed does not match with any of the coverages offered by this server. Locator: List of violating coverage identifiers", 404);
    public static final ExceptionCode NotEnoughStorage = new ExceptionCode("NotEnoughStorage", null, 0);
    public static final ExceptionCode OperationNotSupported = new ExceptionCode("OperationNotSupported",
            "Request is for an operation that is not supported by this server. Locator: Name of operation not supported", 501);
    public static final ExceptionCode OptionNotSupported = new ExceptionCode("OptionNotSupported",
            "Request is for an option that is not supported by this server. Locator: Identifier of option not supported", 501);
    public static final ExceptionCode RasdamanRequestFailed = new ExceptionCode("RasdamanRequestFailed", null, 0);
    public static final ExceptionCode RasdamanUnavailable = new ExceptionCode("RasdamanUnavailable", null, 0);
    public static final ExceptionCode RasdamanError = new ExceptionCode("RasdamanError", null, 0);
    public static final ExceptionCode ResourceError = new ExceptionCode("ResourceError", null, 0);
    public static final ExceptionCode RuntimeError = new ExceptionCode("RuntimeError", null, 0);
    public static final ExceptionCode ServletConnectionError = new ExceptionCode("ServletConnectionError", null, 0);
    public static final ExceptionCode UnknownError = new ExceptionCode("UnknownError", null, 0);
    public static final ExceptionCode UnsupportedCombination = new ExceptionCode("UnsupportedCombination", null, 0);
    public static final ExceptionCode VersionNegotiationFailed = new ExceptionCode("VersionNegotiationFailed",
            "List of versions in AcceptVersions parameter value in GetCapabilities operation "
            + "request did not include any version supported by this server", 400);
    public static final ExceptionCode XmlNotValid = new ExceptionCode("XmlNotValid", null, 0);
    public static final ExceptionCode XmlStructuresError = new ExceptionCode("XmlStructuresError", null, 0);
    public static final ExceptionCode WcpsError = new ExceptionCode("WcpsError", null, 0);
    public static final ExceptionCode WcsError = new ExceptionCode("WcsError", null, 0);
    public static final ExceptionCode WcstError = new ExceptionCode("WcstError", null, 0);
    public static final ExceptionCode WpsError = new ExceptionCode("WpsError", null, 0);
    // WCS CRS-extension
    public static final ExceptionCode NotASubsettingCrs = new ExceptionCode("NotASubsettingCrs",
            "Parameter indicated in the subsettingCrs parameter is not a valid CRS", 404);
    public static final ExceptionCode SubsettingCrsNotSupported = new ExceptionCode("SubsettingCrsNotSupported",
            "CRS indicated in the subsettingCrs parameter is not supported by this server", 404);
    public static final ExceptionCode NotAnOutputCrs = new ExceptionCode("NotAnOutputCrs",
            "Parameter indicated in the outputCrs parameter is not a valid CRS", 404);
    public static final ExceptionCode OutputCrsNotSupported = new ExceptionCode("OutputCrsNotSupported",
            "CRS indicated in the outputCrs parameter is not supported by this server", 404);
}
