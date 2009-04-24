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
* An attempt to get a object via its name using <code>Database.lookup</code>
* and the name is not associated with an object in the database.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
* @see org.odmg.Database#lookup
*/

public class ObjectNameNotFoundException extends ODMGException
{
/**
* Construct an instance of the exception.
*/
	public ObjectNameNotFoundException()
  {
  	super();
  }

/**
* Construct an instance of the exception with a descriptive message.
* @param msg	A message describing the exception.
*/
  public ObjectNameNotFoundException(String msg)
  {
  	super(msg);
  }
/*
	private	String	n;

* Construct an instance of the exception.
* @param	name	The name passed to Database.lookup that is not associated with
* any object in the database.

	public ObjectNameNotFoundException(String name)
  {
		super();
    n = name;
  }


*	Access the name that is not bound to any object in the database.
* @return	The name that was passed to Database.lookup.

  public String getName()
  {
  	return n;
  }
*/
}

