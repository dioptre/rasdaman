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
package org.odmg;

/**
* This exception is thrown when the database asynchronously and explicitly
* aborts the user's transaction due to some failure, the user's data is reset
* just as if the user had directly called <code>Transaction.abort</code>.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/

public class TransactionAbortedException extends ODMGRuntimeException
{
/**
* Constructs an instance of the exception.
*/
	public TransactionAbortedException()
  {
  	super();
  }

/**
* Constructs an instance of the exception with the provided message.
* @param	msg	The message that describes the exception.
*/
	public TransactionAbortedException(String msg)
  {
  	super(msg);
  }
}

