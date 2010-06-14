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
* The base interface for all ODMG collections.
* The ODMG collections are based on JavaSoft's collection interfaces.
* All of the operations defined by the JavaSoft <code>Collection</code>
* interface are supported by an ODMG implementation of <code>DCollection</code>;
* the exception <code>UnsupportedOperationException</code> is not thrown when a
* call is made to any of the <code>Collection</code> methods.
* <p>
* <code>DCollection</code> contains methods used to perform queries on the collection.
* The OQL query predicate is given as a string with the syntax of the
* <code>where</code> clause of OQL. The predefined OQL variable <code>this</code>
* is used inside the predicate to denote the current element of the collection.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/
// * @see com.sun.java.util.collections.UnsupportedOperationException

public interface DCollection extends java.util.Collection
{
/**
* Selects the single element of the collection for which the provided OQL query
* predicate is true,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	predicate	An OQL boolean query predicate.
* @return The element that evaluates to true for the predicate. If no element
* evaluates to true, null is returned.
* @exception	QueryInvalidException	The query predicate is invalid.
*/
	public Object selectElement(String predicate) throws QueryInvalidException;

/**
* Access all of the elements of the collection that evaluate to true for the
* provided query predicate,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	predicate	An OQL boolean query predicate.
* @return	An iterator used to iterate over the elements that evaluated true for the predicate.
* @exception	QueryInvalidException	The query predicate is invalid.
*/
	public java.util.Iterator select(String predicate) throws QueryInvalidException;

/**
* Evaluate the boolean query predicate for each element of the collection and
* return a new collection that contains each element that evaluated to true,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	predicate	An OQL boolean query predicate.
* @return	A new collection containing the elements that evaluated true for the predicate.
* @exception	QueryInvalidException	The query predicate is invalid.
*/
	public DCollection query(String predicate) throws QueryInvalidException;

/**
* Determines whether there is an element of the collection that evaluates to true
* for the predicate,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param	predicate	An OQL boolean query predicate.
* @return	True if there is an element of the collection that evaluates to true
* for the predicate, otherwise false.
* @exception	QueryInvalidException	The query predicate is invalid.
*/
	public boolean existsElement(String predicate) throws QueryInvalidException;
}

