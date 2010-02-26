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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.WCPSException;
import org.w3c.dom.*;
import petascope.wcs.server.exceptions.NoApplicableCodeException;

public class DimensionIntervalElement implements IRasNode, ICoverageInfo
{
    Logger LOG = LoggerFactory.getLogger(DimensionIntervalElement.class);

    private IRasNode child;
	private CoverageInfo info = null;
    private AxisName axis;
    private Crs crs;
    private ScalarExpr domain1, domain2;  // lower and upper bound, or "DomainMetadataExprType" and null
    private long coord1, coord2;
    private int counter = 0;            // counter for the domain vars
    private Metadata meta = null;       // metadata about the current coverage
    private boolean finished = false;
    private Node nextNode;
    private boolean transformedCoordinates = false;

    /**
     * Constructs an element of a dimension interval.
     * @param node XML Node
     * @param xq WCPS Xml Query object
     * @param covInfo CoverageInfo object about the Trim parent object
     * @throws WCPSException
     */
	public DimensionIntervalElement(Node node, XmlQuery xq, CoverageInfo covInfo)
	    throws WCPSException, InvalidCrsException
	{

        System.err.println("Trying to parse DimensionIntervalElement expression...");
        String name;

        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}

        while (node != null && finished == false)
        {
            if (node.getNodeName().equals("#text"))
			{
				node = node.getNextSibling();
				continue;
			}

            name = node.getNodeName();
            System.err.println("Current node is " + name);

            // Try Axis
            try
            {
                axis = new AxisName(node, xq);
                node = node.getNextSibling();
                continue;
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to parse an axis!");
            }

            // Try CRS name
            try
            {
                crs = new Crs(node, xq);
                node = node.getNextSibling();
                if (axis == null)
                    throw new WCPSException("Expected Axis node before CRS !");

                if (covInfo.getCoverageName() != null)
                {
                    // Add WGS84 CRS information from coverage metadata, may be useful
                    // for converting geo-coordinates to pixel-coordinates
                    String coverageName = covInfo.getCoverageName();
                    meta = xq.getMetadataSource().read(coverageName);
                }
                continue;
            }
            catch (WCPSException e)
            {
                System.err.println("Failed to parse a crs!");
            }

            // TODO: how to implement DomainMetadataExpr ?
            
//            // Try last thing
//            try
//            {
//                domain1 = new DomainMetadataExprType(node, xq);
//                counter = 1;
//                continue;
//            }
//            catch (WCPSException e)
//            {
//                System.err.println("Failed to parse domain metadata!");
//            }

            // Then it must be a pair of nodes "lowerBound" + "upperBound"
            if (node.getNodeName().equals("lowerBound"))
            {
                counter = 2;
                domain1 = new ScalarExpr(node.getFirstChild(), xq);
                if (axis == null)
                    throw new WCPSException("Expected <axis> node before <lowerBound> !");
            }
            else
            if (node.getNodeName().equals("upperBound"))
            {
                counter = 2;
                domain2 = new ScalarExpr(node.getFirstChild(), xq);
                if (axis == null)
                    throw new WCPSException("Expected <lowerBound> node before <upperBound> !");
            }
            else
                throw new WCPSException("Unexpected node: " + node.getFirstChild().getNodeName());

            if (axis != null && counter == 1 && domain1 != null)
                finished = true;
            if (axis != null && counter == 2 && domain1 != null && domain2 != null)
                finished = true;

            if (finished == true)
                nextNode = node.getNextSibling();

            node = node.getNextSibling();
        }

        if (finished == true)
            convertToPixelCoordinates();
	}


    /* If input coordinates are geo-, convert them to pixel coordinates. */
    private void convertToPixelCoordinates()
    {
        if (meta.getCrs() == null && crs != null && crs.getName().equals(DomainElement.WGS84_CRS))
        {
            throw new RuntimeException("Coverage '" + meta.getCoverageName() +
                    "' is not georeferenced with 'EPSG:4326' coordinate system.");
        }
        if (counter == 2 && crs != null && domain1.isSingleValue() && domain2.isSingleValue())
        {
            if (crs.getName().equals(DomainElement.WGS84_CRS))
            {
                LOG.debug("CRS is '{}' and should be equal to '{}'", crs.getName(), DomainElement.WGS84_CRS);
                try
                {
                    this.transformedCoordinates = true;
                    // Convert to pixel coordinates
                    Double val1 = domain1.getSingleValue();
                    Double val2 = domain2.getSingleValue();
                    String axisName = axis.toRasQL().toUpperCase();
                    if (axisName.equals("X"))
                    {
                        long[] pCoord = crs.convertToPixelCoordinates(meta, "X", val1, val2, null, null);
                        coord1 = pCoord[0];
                        coord2 = pCoord[1];
                    }
                    if (axisName.equals("Y"))
                    {
                        long[] pCoord = crs.convertToPixelCoordinates(meta, "Y", null, null, val1, val2);
                        coord1 = pCoord[2];
                        coord2 = pCoord[3];
                    }
                }
                catch (NoApplicableCodeException e)
                {
                    this.transformedCoordinates = false;
                    LOG.error("Error while transforming geo-coordinates to pixel coordinates." +
                            "The metadata is probably not valid.");
                }
            }
        }
    }

    /* Not used */
	public String toRasQL()
	{
		return "<DimensionIntervalElement Not Converted to RasQL>";
	}

	public CoverageInfo getCoverageInfo()
	{
		return info;
	}

    public Node getNextNode()
    {
        return nextNode;
    }

    public String getAxisName()
    {
        return this.axis.toRasQL();
    }

    public String getAxisCoords()
    {
        return this.domain1.toRasQL() + " : " + this.domain2.toRasQL();
    }

    public String getLowCoord()
    {
        if (transformedCoordinates)
            return String.valueOf(coord1);
        else
            return this.domain1.toRasQL();
    }

    public String getHighCoord()
    {
        if (transformedCoordinates)
            return String.valueOf(coord2);
        else
            return this.domain2.toRasQL();
    }
}
