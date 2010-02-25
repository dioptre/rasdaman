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
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.w3c.dom.Node;

public class SliceCoverageExpr implements IRasNode, ICoverageInfo
{
    
	private List<DimensionPointElement> axisList;
	private CoverageExpr coverageExprType;
	private CoverageInfo coverageInfo = null;
	private String[] dim;
    private DimensionPointElement elem;
	private int dims;

	public SliceCoverageExpr(Node node, XmlQuery xq) throws WCPSException
	{
		Node child = node.getFirstChild();
		String nodeName;

		axisList = new ArrayList<DimensionPointElement>();

		while (child != null)
		{
			nodeName = child.getNodeName();

			if (nodeName.equals("#text"))
			{
                child = child.getNextSibling();
				continue;
			}

            try
            {
                System.err.println("Trying out an CoverageExprType group...");
                coverageExprType = new CoverageExpr(child, xq);
                coverageInfo = coverageExprType.getCoverageInfo();
                child = child.getNextSibling();
                continue;
            }
            catch (WCPSException e)
            {
                System.err.println("This was no CoverageExprType: " + nodeName);
            }

            try
			{
                // Start a new axis and save it
				elem = new DimensionPointElement(child, xq);
                axisList.add(elem);
                child = elem.getNextNode();
                continue;
			}
            catch (WCPSException e)
            {
                System.err.println("This was no Dimension Point ELement: " + child.getNodeName());
            }

            // else unknown element
            throw new WCPSException("Unknown node for TrimCoverage expression:" + child.getNodeName());
		}

		dims = coverageInfo.getNumDimensions();
		dim  = new String[dims];

		for (int j = 0; j < dims; ++j)
		{
			dim[j] = "*:*";
		}

		Iterator<DimensionPointElement> i = axisList.iterator();
		DimensionPointElement axis;
		int axisId;
		int slicingPosInt;
        String slicingPosStr;

		while (i.hasNext())
		{
			axis        = i.next();
            /* TODO: BUG: This searches the axis types list using name, not type */
			axisId      = coverageInfo.getDomainIndexByName(axis.getAxisName());
            slicingPosStr = axis.getSlicingPosition();
            dim[axisId] = slicingPosStr;
            // Slicing position can be a constant number or a variable reference
            try
            {
                slicingPosInt  = Integer.parseInt(slicingPosStr);
            }
            catch (NumberFormatException e)
            {
                slicingPosInt = 1;
            }
            coverageInfo.setCellDimension(
                axisId,
                new CellDomainElement(
                BigInteger.valueOf(slicingPosInt), BigInteger.valueOf(slicingPosInt)));

        }

	}

	public CoverageInfo getCoverageInfo()
	{
		return coverageInfo;
	}

	public String toRasQL()
	{
		String result = "(" + coverageExprType.toRasQL() + ") [";

		for (int j = 0; j < dims; ++j)
		{
			if (j > 0)
			{
				result += ",";
			}

			result += dim[j];
		}

		result += "]";
		return result;
	}
}