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


package wcst.transaction;

//~--- non-JDK imports --------------------------------------------------------

import net.opengis.ows.v_1_0_0.ExceptionReport;
import net.opengis.ows.v_1_0_0.ExceptionType;

//~--- JDK imports ------------------------------------------------------------

import java.util.Arrays;

/**
 * Exception class for the WCS server.
 * This class can return an error report, than can be marshalled into a
 * WCS-standard compliant XML structure describing the error that has happened.
 *
 * @author Andrei Aiordachioaie
 */
public class WCSException extends Exception
{
	private static final long serialVersionUID = 847843429L;
	private String[] errorCodes =
	{
		/* The following codes are taken from the WCS specification (OGC 07-067r5) */
		"MissingParameterValue", "InvalidParameterValue", "NoApplicableCode", "UnsupportedCombination", "NotEnoughStorage",
		/* The following codes are defined by our implementation (PetaScope) */
		"MalitiousQuery", "ActionNotSupported"
	};
	private ExceptionType item;
	private ExceptionReport report;

	/**
	 * Default (minimal) constructor
	 * @param error Error Code
	 * @param detail Detailed message about the error
	 */
	public WCSException(String error, String detail)
	{
		report = new ExceptionReport();
		report.setLanguage("en");
		report.setVersion("1.0.0");
		Arrays.sort(errorCodes);
		if ( Arrays.binarySearch(errorCodes, error) >= 0 )
		{
			item = new ExceptionType();
			item.setExceptionCode(error);
			item.setLocator(detail);
			report.getException().add(item);
		}
		else
		{
			/*
			 *  This WCS Exception code is not recognized. But we should still be
			 * able to provide an XML ErrorReport
			 */
			item = new ExceptionType();
			item.setExceptionCode("NoApplicableCode");
			item.setLocator("Internal Error Code: " + error + " ... Internal Error Message: " + detail);
			report.getException().add(item);
		}
	}

	/**
	 * Convenience Constructor
	 * @param error Error Code
	 * @param detail Detailed message about the error
	 * @param msg A message that will be output on *System.err
	 */
	public WCSException(String error, String detail, String msg)
	{
		this(error, detail);

		System.err.println(msg);
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
}
