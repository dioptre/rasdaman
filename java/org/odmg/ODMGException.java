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
* This is the base class for all exceptions thrown by an ODMG implementation.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/

public class ODMGException extends Exception {
/**
* Construct an <code>ODMGException</code> object without an error message.
*/
	public ODMGException()
  {
  	super();
  }
/**
* Construct an <code>ODMGException</code> object with an error message.
* @param msg	The error message associated with this exception.
*/

  public ODMGException(String msg)
  {
  	super(msg);
  }
}

