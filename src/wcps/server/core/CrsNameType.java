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


package wcps.server.core;

import org.w3c.dom.*;

public class CrsNameType implements IRasNode
{
    private String crs;

	public CrsNameType(Node node, ProcessCoveragesRequest pcr) throws WCPSException
	{
        System.err.println("Parsing crs name ...");

        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}

        if (node != null && node.getNodeName().equals("srsName"))
        {
            String val = node.getNodeValue();
            this.crs = val;
        }
        else
            throw new WCPSException("Could not a 'srsName' node !");
	}

	public String toRasQL()
	{
        return crs;
	}
}
