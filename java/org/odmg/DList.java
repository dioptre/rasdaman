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
* The ODMG List collection.
* A <code>DList</code> collection is an ordered collection that provides
* efficient insertion and removal of elements at arbitrary positions in the
* list, but it also supports indexed access. The beginning index value is 0.
* When an element is added at a given position in the list, the index of all
* subsequent elements is increased by 1. Similarly, when an element is removed
* from the list, the index of all subsequent elements is decreased by 1.
* <p>
* All of the operations defined by the JavaSoft <code>List</code>
* interface are supported by an ODMG implementation of <code>DList</code>,
* the exception <code>UnsupportedOperationException</code> is not thrown when a
* call is made to any of the <code>List</code> methods.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/
// * @see com.sun.java.util.collections.UnsupportedOperationException

public interface DList extends DCollection, java.util.List {
/**
* Creates a new <code>DList</code> object that contains the contents of this
* <code>DList</code> object concatenated
* with the contents of the <code>otherList</code> object.
* @param	otherList	The list whose elements are placed at the end of the list
* returned by this method.
* @return	A new <code>DList</code> that is the concatenation of this list and
* the list referenced by <code>otherList</code>.
*/
	public DList concat(DList otherList);
}

