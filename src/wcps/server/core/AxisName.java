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

public class AxisName implements IRasNode
{
    private String name;

	public AxisName(Node node, XmlQuery xq) throws WCPSException
	{
        System.err.println("Trying axis node: " + node.getNodeName());

        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}

        if (node != null && node.getNodeName().equals("axis"))
        {
            String axis = node.getTextContent();
            if (axis.length() == 1 && "xyzt".contains(axis))
                this.name = axis;
            else
                throw new WCPSException("Unknown axis name " + axis);
        }
        else
            throw new WCPSException("Could not find an axis node !");
	}

	public String toRasQL()
	{
        return name;
	}
}
