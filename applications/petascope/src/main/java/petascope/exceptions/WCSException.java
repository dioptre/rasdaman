/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
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

import petascope.ConfigManager;

/**
 * This exception can return an error report, than can be marshalled into a
 * WCS-standard compliant XML structure describing the error that has happened.
 *
 * @author Andrei Aiordachioaie
 * @author Dimitar Misev
 */
public class WCSException extends PetascopeException {

    public WCSException(String exceptionText) {
        this(ExceptionCode.WcsError, exceptionText);
    }

    public WCSException(String exceptionText, Exception ex) {
        this(ExceptionCode.WcsError, exceptionText, ex);
    }

    public WCSException(ExceptionCode exceptionCode) {
        this(exceptionCode, null, null);
    }

    public WCSException(ExceptionCode exceptionCode, Exception ex) {
        this(exceptionCode, ex.getLocalizedMessage(), ex);
    }

    public WCSException(ExceptionCode exceptionCode, String exceptionText) {
        this(exceptionCode, exceptionText, null);
    }

    public WCSException(ExceptionCode exceptionCode, String exceptionText, Exception ex) {
        super(exceptionCode, exceptionText, ex,
                ConfigManager.WCS_DEFAULT_VERSION, ConfigManager.WCS_DEFAULT_LANGUAGE);
    }
}
