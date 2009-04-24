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
import java.util.List;

/**
* The interface that defines the operations of an ODMG array,
* <BR><I> not yet available in RasDaMan. </BR></I>
* Nearly all of its operations are defined by the JavaSoft <code>List</code> interface.
* All of the operations defined by the JavaSoft <code>List</code>
* interface are supported by an ODMG implementation of <code>DArray</code>,
* the exception <code>UnsupportedOperationException</code> is not thrown when a
* call is made to any of the <code>List</code> methods.
* An instance of a class implementing this interface can be obtained
* by calling the method <code>Implementation.newDArray</code>.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/
// @see java.lang.UnsupportedOperationException

public interface DArray extends org.odmg.DCollection, java.util.List
{
/**
* Resize the array to have <code>newSize</code> elements.
* @param	newSize	The new size of the array.
*/
	public void resize(int newSize);
}

