/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package petascope.wcs2.server.templates;

import java.util.Iterator;
import javax.xml.XMLConstants;
import javax.xml.namespace.NamespaceContext;

/**
 *
 * @author Andrei Aiordachioaie
 */
public class WcsNamespaceContext implements NamespaceContext
{
    @Override
	public String getNamespaceURI(String prefix)
	{
		if ( prefix == null )
		{
			throw new NullPointerException("Null prefix");
		}
		else if ( "wcs".equals(prefix) )
		{
			return "http://www.opengis.net/wcs/2.0";
		}
		else if ( "gml".equals(prefix) )
		{
			return "http://www.opengis.net/gml/3.2";
		}

		return XMLConstants.NULL_NS_URI;
	}

	// This method isn't necessary for XPath processing.
    @Override
	public String getPrefix(String uri)
	{
		throw new UnsupportedOperationException();
	}

	// This method isn't necessary for XPath processing either.
    @Override
	public Iterator getPrefixes(String uri)
	{
		throw new UnsupportedOperationException();
	}
}
