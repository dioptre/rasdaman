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
* The factory interface for a particular ODMG implementation.
* Each ODMG implementation will have a class that implements this interface.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/

public interface Implementation {
/**
* Create a <code>Transaction</code> object and associate it with the current thread.
* @return The newly created <code>Transaction</code> instance.
* @see org.odmg.Transaction
*/
	public		Transaction		newTransaction();

/**
* Get the current <code>Transaction</code> for the thread.
* @return The current <code>Transaction</code> object or null if there is none.
* @see org.odmg.Transaction
*/
	public		Transaction		currentTransaction();

/**
* Create a new <code>Database</code> object.
* @return The new <code>Database</code> object.
* @see org.odmg.Database
*/
	public		Database			newDatabase();

/**
* Create a new <code>OQLQuery</code> object.
* @return The new <code>OQLQuery</code> object.
* @see org.odmg.OQLQuery
*/
	public		OQLQuery			newOQLQuery();

/**
* Create a new <code>DList</code> object.
* @return The new <code>DList</code> object.
* @see org.odmg.DList
*/
	public		DList					newDList();

/**
* Create a new <code>DBag</code> object.
* @return The new <code>DBag</code> object.
* @see org.odmg.DBag
*/
	public		DBag					newDBag();

/**
* Create a new <code>DSet</code> object.
* @return The new <code>DSet</code> object.
* @see org.odmg.DSet
*/
	public		DSet					newDSet();

/**
* Create a new <code>DArray</code> object,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @return The new <code>DArray</code> object.
* @see org.odmg.DArray
*/
	public		DArray				newDArray();

/**
* Create a new <code>DMap</code> object,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @return	The new <code>DMap</code> object.
* @see org.odmg.DMap
*/
	public		DMap					newDMap();

/**
* Get a <code>String</code> representation of the object's identifier.
* @param obj The object whose identifier is being accessed.
* @return The object's identifier in the form of a String
*/
	public		String				getObjectId(Object obj);

/**
* Get the <code>Database</code> that contains the object <code>obj</code>,
* <BR><I> not yet available in RasDaMan. </BR></I>
* @param obj The object.
* @return The <code>Database</code> that contains the object.
*/
	public		Database			getDatabase(Object obj);
}

