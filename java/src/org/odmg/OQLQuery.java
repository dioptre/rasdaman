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
* The interface to an OQL query object.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
*/

public interface OQLQuery {

/**
* Create an OQL query from the string parameter.
* In order to execute a query, an <code>OQLQuery</code> object must be created
* by calling <code>Implementation.newOQLQuery</code>, then calling the
* <code>create</code> method with the query string.
* The <code>create</code> method might throw <code>QueryInvalidException</code>
* if the query could not be compiled properly. Some implementations may not want
* to compile the query before <code>execute</code> is called. In this case
* <code>QueryInvalidException</code> is thrown when <code>execute</code> is called.
* @param	query	An OQL query.
* @exception	QueryInvalidException	The query syntax is invalid.
*/
	public	void		create(String query) throws QueryInvalidException;

/**
* Bind a parameter to the query.
* A parameter is denoted in the query string passed to <code>create</code> by <i>$i</i>,
* where <i>i</i> is the rank of the parameter, beginning with 1.
* The parameters are set consecutively by calling this method <code>bind</code>.
* The <i>ith</i> variable is set by the <i>ith</i> call to the <code>bind</code> method.
* If any of the <i>$i</i> are not set by a call to <code>bind</code> at the point
* <code>execute</code> is called, <code>QueryParameterCountInvalidException</code> is thrown.
* The parameters must be objects, and the result is an <code>Object</code>.
* Objects must be used instead of primitive types (<code>Integer</code> instead
* of <code>int</code>) for passing the parameters.
* <P>
* If the parameter is of the wrong type,
* <code>QueryParameterTypeInvalidException</code> is thrown.
* After executing a query, the parameter list is reset.
* @param parameter A value to be substituted for a query parameter.
* @exception QueryParameterCountInvalidException The number of calls to
* <code>bind</code> has exceeded the number of parameters in the query.
* @exception QueryParameterTypeInvalidException The type of the parameter does
* not correspond with the type of the parameter in the query.
*/
	public	void		bind(Object parameter) throws QueryParameterCountInvalidException,
					      					QueryParameterTypeInvalidException;

/**
* Execute the query.
* After executing a query, the parameter list is reset.
* Some implementations may throw additional exceptions that are also derived
* from <code>ODMGException</code>.
* @return	The object that represents the result of the query.
* The returned data, whatever its OQL type, is encapsulated into an object.
* For instance, when OQL returns an integer, the result is put into an
* <code>Integer</code> object. When OQL returns a collection (literal or object),
* the result is always a Java collection object of the same kind
* (for instance, a <code>DList</code>).
* @exception	QueryException	An exception has occurred while executing the query.
*/
	public	Object	execute() throws QueryException;
}

