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

public class RangeElementType implements IRasNode, ICoverageInfo
{
	private IRasNode child;
	private CoverageInfo info;
    private String field;

	public RangeElementType(Node node, ProcessCoveragesRequest pcr)
	    throws WCPSException
	{
        
        // TODO: Implement RangeCoverageExpr
        throw new WCPSException("Feature not yet implemented !");


        /*
        String nodeName = node.getNodeName();
		System.err.println("Trying to parse a range coverage expression... Starting at node "
                + nodeName);

		Node it = node.getFirstChild();

		while (it != null)
		{
			if (it.getNodeName().equals("#text"))
			{
				it = it.getNextSibling();
				continue;
			}
			if (it.getNodeName().equals("field"))
			{
                field = it.getNodeValue();
                continue;
			}

            // Try to read a CoverageExprType
            try
            {
                child = new CoverageExprType(node, pcr);
                info = new CoverageInfo(((ICoverageInfo) child).getCoverageInfo());
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to parse a CoverageExprType !");
                child = null;
            }
            
			it = it.getNextSibling();
		}

        if (child == null)
            throw new WCPSException("Could not parse a CoverageExpr !");

         */
	}

	public CoverageInfo getCoverageInfo()
	{
		return info;
	}

	public String toRasQL()
	{
        String result = "";

        return result;
	}
}
