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
* This exception is thrown when attempting to bind a name to an object
* when the name is already bound to another object.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
* @see org.odmg.Database#bind
*/

public class ObjectNameNotUniqueException extends ODMGException
{
/**
* Construct an instance of the exception.
*/
	public ObjectNameNotUniqueException()
  {
  	super();
  }

/**
* Construct an instance of the exception with a descriptive message.
* @param	msg	A message containing a description of the exception.
*/
	public ObjectNameNotUniqueException(String msg)
  {
  	super(msg);
  }
/*
	private	Object o;
  private String n;
	public ObjectNameNotUniqueException(Object obj, String name)
  {
  	super();
		o = obj;
    n = name;
  }


* Get the object that was passed to Database.bind.
* @return	The object that was being bound to a name.

	public	Object	getObject()
  {
		return o;
  }


* Get the name that is not unique.
* @return The name that is already associated with another object.

  public String getName()
  {
  	return n;
  }
*/
}

