/*
 * This file is part of Petascope.
 *
 * Petascope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Petascope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Petascope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.Petascope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package wcs.server.core;

import net.opengis.ows.v_1_0_0.ExceptionReport;
import net.opengis.ows.v_1_0_0.ExceptionType;

/**
 * Exception class for the WCS server. 
 * This class can return an error report, than can be marshalled into a
 * WCS-standard compliant XML structure describing the error that has happened.
 */

public class WCSException extends Exception {
    
    private static final long serialVersionUID = 847843429L;

    private ExceptionReport report;
    private ExceptionType item;

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
        if (error.equalsIgnoreCase("MissingParameterValue") ||
                error.equalsIgnoreCase("InvalidParameterValue") ||
                error.equalsIgnoreCase("NoApplicableCode") ||
                error.equalsIgnoreCase("UnsupportedCombination") ||
                error.equalsIgnoreCase("NotEnoughStorage"))
        {
            item = new ExceptionType();
            item.setExceptionCode(error);
            item.setLocator(detail);
            report.getException().add(item);
        }
        else
        {
            /* This WCS Exception code is not recognized. But should still be
             able to provide an XML ErrorReport */
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
     * @param msg A message that will be output on *System.err*
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
