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


package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.WCPSException;
import org.w3c.dom.*;

// This is the equivalent of the "ProcessingExprType" complex XML type.
public class EncodeDataExpr implements IRasNode
{
	private IRasNode coverageExprType;
	private String extraParams;
	private String format;
	private String mime;
	private Boolean store;

	public EncodeDataExpr(Node node, XmlQuery request) throws WCPSException
	{
		Node child;
		String nodeName;

		for (child = node.getFirstChild(); child != null; child = child.getNextSibling())
		{
			nodeName = child.getNodeName();

			if (nodeName.equals("#text"))
			{
				continue;
			}

			System.err.println("Encode : node : " + child.getNodeName());

			if (nodeName.equals("format"))
			{
				format = child.getFirstChild().getNodeValue();
				mime   = request.getMetadataSource().formatToMimetype(format);
				continue;
			}

			if (nodeName.equals("extraParameters"))
			{
				extraParams = child.getFirstChild().getNodeValue();
				continue;
			}

			coverageExprType = new CoverageExpr(child, request);
		}

		Node _store = node.getAttributes().getNamedItem("store");

		if (_store != null)
		{
			store = _store.getNodeValue().equals("true");
		}
	}

	public String getMime()
	{
		return mime;
	}

	public String toRasQL()
	{
		// TODO: cjucovschi - implement store

		String result;

		if (format.equals("raw"))
		{
			result = coverageExprType.toRasQL();
		}
		else
		{
			result = format + "(" + coverageExprType.toRasQL();

			if (extraParams != null)
			{
				result = result + ", " + extraParams;
			}

			result = result + ")";
		}

        System.err.println("Returning EncodeExpression:" + result); 
		return result;
	}
}