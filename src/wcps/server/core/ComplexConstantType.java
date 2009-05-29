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

public class ComplexConstantType implements IRasNode
{
    private String re, im;

	public ComplexConstantType(Node node, ProcessCoveragesRequest pcr) throws WCPSException
	{
        System.err.println("Parsing complex constant: " + node.getNodeName());

        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}

        while (node != null)
        {
            String name = node.getNodeName();
            if (name.equals("re"))
                re = node.getNodeValue();
            else
            if (name.equals("im"))
                im = node.getNodeValue();
            else
                throw new WCPSException("Unknown node while processing complex constant: " + name);

            node = node.getNextSibling();
        }

        // parse the real part
        try
        {
            Integer real = Integer.parseInt(re);
        }
        catch (NumberFormatException e)
        {
            try
            {
                Float real = Float.parseFloat(re);
            }
            catch (NumberFormatException e2)
            {
                throw new WCPSException("Could not parse float or integer " +
                        "number for real part of complex number:" + re);
            }
        }
        // parse the imaginary part
        try
        {
            Integer imag = Integer.parseInt(im);
        }
        catch (NumberFormatException e)
        {
            try
            {
                Float imag = Float.parseFloat(im);
            }
            catch (NumberFormatException e2)
            {
                throw new WCPSException("Could not parse float or integer " +
                        "number for imaginary part of complex number" + im);
            }
        }
	}

	public String toRasQL()
	{
        return "complex ( " + re + ", " + im + " ) ";
	}
}
