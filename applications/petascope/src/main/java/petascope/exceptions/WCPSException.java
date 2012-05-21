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
 * GNU  General Public License for more details.
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
 * This is the superclass of all WCPS exceptions.
 */
public class WCPSException extends PetascopeException {

    public WCPSException(String exceptionText) {
        this(ExceptionCode.WcpsError, exceptionText);
    }

    public WCPSException(String exceptionText, Exception ex) {
        this(ExceptionCode.WcpsError, exceptionText, ex);
    }

    public WCPSException(ExceptionCode exceptionCode) {
        this(exceptionCode, null, null);
    }

    public WCPSException(ExceptionCode exceptionCode, Exception ex) {
        this(exceptionCode, ex.getLocalizedMessage(), ex);
    }

    public WCPSException(ExceptionCode exceptionCode, String exceptionText) {
        this(exceptionCode, exceptionText, null);
    }

    public WCPSException(ExceptionCode exceptionCode, String exceptionText, Exception ex) {
        super(exceptionCode, exceptionText, ex, ConfigManager.WCPS_VERSION, ConfigManager.WCPS_LANGUAGE);
    }
}
