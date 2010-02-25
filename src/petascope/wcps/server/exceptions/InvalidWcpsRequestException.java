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


package petascope.wcps.server.exceptions;

/**
 * This exception is throws whenever a user submits a request that is not valid in some way.
 */

public class InvalidWcpsRequestException extends WCPSException
{
	private static final long serialVersionUID = 65492846L;

	public InvalidWcpsRequestException(String message)
	{
		super(message, null);
	}

	public InvalidWcpsRequestException(String message, Throwable cause)
	{
		super(message, cause);
	}
}
