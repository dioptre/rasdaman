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

public class VariableReference implements IRasNode
{
    private String name;
    private String translatedName;

	public VariableReference(Node node, XmlQuery xq) throws WCPSException
	{
        System.err.println("Parsing variable reference: " + node.getNodeName());

        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}

        if (node != null && node.getNodeName().equals("variableRef"))
        {
            name = node.getTextContent();
            translatedName = xq.getReferenceVariableName(name);
            System.err.println("Variable " + name + " has been renamed into " +
                    translatedName);
        }
        else
            throw new WCPSException("Could not find any variable reference !");
	}

    public String getName()
    {
        return name;
    }

	public String toRasQL()
	{
        return translatedName;
	}
}
