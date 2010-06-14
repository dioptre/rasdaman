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
* The ODMG Set collection interface.
* A <code>DSet</code> object is an unordered collection that does not support
* multiple elements with the same value. An implementation typically is very
* efficient at determining whether the collection contains a particular value.
* <p>
* All of the operations defined by the JavaSoft <code>Set</code>
* interface are supported by an ODMG implementation of <code>DSet</code>,
* the exception <code>UnsupportedOperationException</code> is not thrown when a
* call is made to any of the <code>Set</code> methods.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/
// * @see java.lang.UnsupportedOperationException

public interface DSet extends DCollection, java.util.Set
{

/**
* Create a new <code>DSet</code> object that is the set union of this
* <code>DSet</code> object and the set referenced by <code>otherSet</code>,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	otherSet	The other set to be used in the union operation.
* @return	A newly created <code>DSet</code> instance that contains the union of the two sets.
*/
	public DSet			union(DSet otherSet);

/**
* Create a new <code>DSet</code> object that is the set intersection of this
* <code>DSet</code> object and the set referenced by <code>otherSet</code>,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	otherSet	The other set to be used in the intersection operation.
* @return	A newly created <code>DSet</code> instance that contains the
* intersection of the two sets.
*/
	public DSet			intersection(DSet otherSet);

/**
* Create a new <code>DSet</code> object that contains the elements of this
* collection minus the elements in <code>otherSet</code>,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	otherSet	A set containing elements that should not be in the result set.
* @return	A newly created <code>DSet</code> instance that contains the elements
* of this set minus those elements in <code>otherSet</code>.
*/
	public DSet			difference(DSet otherSet);

/**
* Determine whether this set is a subset of the set referenced by <code>otherSet</code>,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	otherSet	Another set.
* @return True if this set is a subset of the set referenced by <code>otherSet</code>,
* otherwise false.
*/
	public boolean	subsetOf(DSet otherSet);

/**
* Determine whether this set is a proper subset of the set referenced by
* <code>otherSet</code>,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	otherSet	Another set.
* @return True if this set is a proper subset of the set referenced by
* <code>otherSet</code>, otherwise false.
*/
	public boolean	properSubsetOf(DSet otherSet);

/**
* Determine whether this set is a superset of the set referenced by <code>otherSet</code>,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	otherSet	Another set.
* @return True if this set is a superset of the set referenced by <code>otherSet</code>,
* otherwise false.
*/
	public boolean	supersetOf(DSet otherSet);

/**
* Determine whether this set is a proper superset of the set referenced by
* <code>otherSet</code>,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	otherSet	Another set.
* @return True if this set is a proper superset of the set referenced by
* <code>otherSet</code>, otherwise false.
*/
	public boolean	properSupersetOf(DSet otherSet);
}

