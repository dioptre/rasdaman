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

import java.math.BigInteger;
import java.util.ArrayList;
import org.w3c.dom.*;

public class ConstantList implements IRasNode
{
    private ArrayList<String> list;
	private String val;
    private Node lastNode;

	public ConstantList(Node node, XmlQuery xq) throws WCPSException
	{
        list = new ArrayList<String>();

        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}
        System.err.println("Trying to parse ConstantList ");

        while (node != null)
        {
            String nodeName = node.getNodeName();

            if (nodeName.equals("value"))
            {
                val = node.getTextContent();
                checkConstant(val);
                list.add(val);
                lastNode = node;
            }
            else
                throw new WCPSException("Unknown node in ConstantList: " + nodeName);

            node = node.getNextSibling();
            while ((node != null) && node.getNodeName().equals("#text"))
            {
                node = node.getNextSibling();
            }
        }

        System.err.println("Parsed constant list with " + list.size() + " elements !");
	}

    private void checkConstant(String val) throws WCPSException
    {
        boolean ok = false;
        try
        {
            Integer.parseInt(val);
            ok = true;
        }
        catch (NumberFormatException e1){}
        try
        {
            Float.parseFloat(val);
            ok = true;
        }
        catch (NumberFormatException e2){}
        try
        {
            new ComplexConstant(val);
            ok = true;
        }
        catch (WCPSException e1){}

        if (ok == false)
            throw new WCPSException("'" + val + "' is not an integer, float, or complex constant !");
    }

	public String toRasQL()
	{
		String result = list.get(0);
        for (int i = 1; i < list.size(); i++)
            result += ", " + list.get(i);
        return result;
	}

    /** Size of all elements in the constant list **/
    public int getSize()
    {
        return list.size();
    }

    /** Return the last node of the constant list. **/
    public Node getLastNode()
    {
        return lastNode;
    }
}
