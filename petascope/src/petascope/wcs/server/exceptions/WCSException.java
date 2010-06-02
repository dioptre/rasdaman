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
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */


package petascope.wcs.server.exceptions;

//~--- non-JDK imports --------------------------------------------------------

import net.opengis.ows.v_1_0_0.ExceptionReport;
import net.opengis.ows.v_1_0_0.ExceptionType;

//~--- JDK imports ------------------------------------------------------------

import java.util.Arrays;
import petascope.ConfigManager;

/**
 * Private Exception superclass for the WCS server.
 * This class can return an error report, than can be marshalled into a
 * WCS-standard compliant XML structure describing the error that has happened.
 *
 * @author Andrei Aiordachioaie
 */
public class WCSException extends Exception
{
//    private static Logger LOG = LoggerFactory.getLogger(WCSException.class);

    /*
	private String[] errorCodes =
	{
		"MissingParameterValue", "InvalidParameterValue", "NoApplicableCode",
        "UnsupportedCombination", "NotEnoughStorage",
		"MaliciousQuery", "ActionNotSupported", "XmlStructuresError", 
        "BadResponseHandler", "MultiBandImagesNotSupported", "RasdamanUnavailable",
        "ServletConnectionError", "XmlInputNotValid", "InvalidPropertyValue", 
        "InternalWcpsError", "InternalSqlError", "RasdamanRequestFailed",
        "NodeParsingNotImplemented", "IOConnectionError", "InvalidTemporalMetadata",
        "InvalidRequestString"
	};
    */
	private ExceptionType item;
	private ExceptionReport report;
    private String errorCode;
    private String errorDetail = "";

	/**
	 * Default (minimal) constructor
	 * @param error Error Code
	 * @param detail Detailed message about the error
	 */
	protected WCSException(String error, String detail)
	{
        super();
        errorCode = error;
        errorDetail = detail;

		createReport();
	}

    /**
	 * Convenience Constructor
	 * @param error Error Code
	 * @param detail Detailed message about the error
	 * @param e The cause of the current exception.
	 */
	protected WCSException(String error, String detail, Exception e)
	{
        super(e);
        errorCode = error;
        errorDetail = detail;
        createReport();
	}

    private void createReport()
    {
        report = new ExceptionReport();
		report.setLanguage(ConfigManager.WCST_LANGUAGE);
		report.setVersion(ConfigManager.WCST_VERSION);

        item = new ExceptionType();
        item.setExceptionCode(errorCode);
        item.setLocator(errorDetail);
        report.getException().add(item);
    }

    /** Return the error code.
     *
     * @return
     */
    public String getErrorCode()
    {
        return errorCode;
    }

    /** Return the detailed error message.
     *
     * @return
     */
    public String getErrorDetail()
    {
        return errorDetail;
    }

	/**
	 * Retrieves a data structure that can be later marshalled into a XML
	 * "ExceptionReport" document.
	 * @return ExceptionReport object
	 */
	public ExceptionReport getReport()
	{
		return report;
	}

    /**
     * Adds text to this exception's detail message. 
     * @param msg 
     */
    public void appendErrorDetail(String msg)
    {
        this.errorDetail += msg;
        createReport();
    }

    @Override
    public String getMessage()
    {
        return this.errorDetail;
    }
}
