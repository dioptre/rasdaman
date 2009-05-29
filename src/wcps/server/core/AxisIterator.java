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

public class AxisIterator implements IRasNode
{
	private String var;
    private AxisName axis;
	private NumericScalarExpr hi,lo;

	public AxisIterator(Node node, ProcessCoveragesRequest pcr) throws WCPSException
	{

        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}
        System.err.println("Trying to parse AxisIterator ");

        while (node != null)
        {
            String nodeName = node.getNodeName();

            if (nodeName.equals("iteratorVar"))
            {
                var = node.getTextContent();
            }
            else if (nodeName.equals("axis"))
            {
                axis = new AxisName(node, pcr);
            }
            else
            {
                // Should contain the limits
                // TODO: Implement ImageCrsDomainMetadataType class
                if (lo == null)
                    lo = new NumericScalarExpr(node, pcr);
                else if (hi == null)
                    hi = new NumericScalarExpr(node, pcr);
                else
                    throw new WCPSException("Unknown node in AxisIterator: " + nodeName);
            }

            node = node.getNextSibling();
            while ((node != null) && node.getNodeName().equals("#text"))
            {
                node = node.getNextSibling();
            }
        }
	}

	public String toRasQL()
	{
		String result = var + " in (" + lo.toRasQL() + "):(" + hi.toRasQL() + ")";
        return result;
	}
}
