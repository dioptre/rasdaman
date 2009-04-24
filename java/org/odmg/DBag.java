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
* This interface defines the operations associated with an ODMG bag collection.
* All of the operations defined by the JavaSoft <code>Collection</code>
* interface are supported by an ODMG implementation of <code>DBag</code>,
* the exception <code>UnsupportedOperationException</code> is not thrown when a
* call is made to any of the <code>Collection</code> methods.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/
// * @see java.lang.UnsupportedOperationException

public interface DBag extends DCollection
{
/**
* A new <code>DBag</code> instance is created that is the union of this object
* and <code>otherBag</code>.
* This method is similar to the <code>addAll</code> method in <code>Collection</code>,
* except that this method creates a new collection and <code>addAll</code>
* modifies the object to contain the result.
* @param	otherBag	The other bag to use in the union operation.
* @return A <code>DBag</code> instance that contains the union of this object
* and <code>otherBag</code>.
*/
// * @see	com.sun.java.util.collections.Collection#addAll
	public	DBag	union(DBag otherBag);

/**
* A new <code>DBag</code> instance is created that contains the intersection of
* this object and the <code>DBag</code> referenced by <code>otherBag</code>.
* This method is similar to the <code>retainAll</code> method in <code>Collection</code>,
* except that this method creates a new collection and <code>retainAll</code>
* modifies the object to contain the result.
* @param	otherBag The other bag to use in creating the intersection.
* @return A <code>DBag</code> instance that contains the intersection of this
* object and <code>otherBag</code>.
*/
// @see com.sun.java.util.collections.Collection#retainAll
	public	DBag	intersection(DBag otherBag);

/**
* A new <code>DBag</code> instance is created that contains the difference of
* this object and the <code>DBag</code> instance referenced by <code>otherBag</code>.
* This method is similar to the <code>removeAll</code> method in <code>Collection</code>,
* except that this method creates a new collection and <code>removeAll</code>
* modifies the object to contain the result.
* @param	otherBag The other bag to use in creating the difference.
* @return A <code>DBag</code> instance that contains the elements of this object
* minus the elements in <code>otherBag</code>.
*/
// * @see com.sun.java.util.collections.Collection#removeAll
	public	DBag	difference(DBag otherBag);

/**
* This method returns the number of occurrences of the object <code>obj</code>
* in the <code>DBag</code> collection.
* @param obj The value that may have elements in the collection.
* @return The number of occurrences of <code>obj</code> in this collection.
*/
	public	int		occurrences(Object obj);
}

