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


public class StringScalarExprType implements IRasNode
{

    private String op, string;
    private CoverageExprType cov;
	public StringScalarExprType(Node node, ProcessCoveragesRequest pcr) throws WCPSException
	{
        while ((node != null) && (node.getNodeName().equals("#text")))
        {
            node = node.getNextSibling();
        }

        System.err.println("Parsing String Scalar expr : " + node.getNodeName());

        if (node.getNodeName().equals("stringIdentifier"))
        {
            Node child = node.getFirstChild();
            cov = new CoverageExprType(child, pcr);
            op = "id";
        }
        else
        if (node.getNodeName().equals("stringConstant"))
        {
            op = "constant";
            string = node.getNodeValue();
        }
        else
            throw new WCPSException("Unknown String expr node: " + node.getNodeName());
	}

	public String toRasQL()
	{
        String result = "";
        if (op.equals("constant"))
            result = string;
        if (op.equals("id"))
            result = cov.toRasQL();

        return result;
	}
}
