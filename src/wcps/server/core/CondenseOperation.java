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

public class CondenseOperation implements IRasNode
{
    private String name;

	public CondenseOperation(Node node, XmlQuery xq) throws WCPSException
	{
        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}

        System.err.println("Parsing condense Operation: " + node.getNodeName());

        String text = node.getNodeName();
        this.name = formatOperation(text);
        
        if (name == null)
            throw new WCPSException("Unknown condense operation: " + text);
	}

    private String formatOperation(String name)
    {
        String shortOp = null;
        if (name.equals("opPlus"))
            shortOp = "+";
        if (name.equals("opMult"))
            shortOp = "*";
        if (name.equals("opMin"))
            shortOp = "min";
        if (name.equals("opMax"))
            shortOp = "max";
        if (name.equals("opAnd"))
            shortOp = "and";
        if (name.equals("opOr"))
            shortOp = "or";
        
        return shortOp;
    }

	public String toRasQL()
	{
        return name;
	}
}
