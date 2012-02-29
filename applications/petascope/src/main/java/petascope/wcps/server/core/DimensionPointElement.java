/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcps.server.core;

import petascope.core.Metadata;
import petascope.exceptions.WCPSException;
import petascope.exceptions.WCSException;
import org.w3c.dom.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.CrsUtil;

public class DimensionPointElement implements IRasNode {

    Logger log = LoggerFactory.getLogger(DimensionPointElement.class);
    
    private IRasNode child;
    private ScalarExpr domain;
    private AxisName axis;
    private Crs crs;
    private boolean finished = false;
    private Node nextNode;
    private Metadata meta = null;       // metadata about the current coverage
    private boolean transformedCoordinates = false;
    private long coord;
    
    public DimensionPointElement(Node node, XmlQuery xq, CoverageInfo covInfo)
            throws WCPSException {
        
        if (covInfo.getCoverageName() != null) {
            // Add Bbox information from coverage metadata, may be useful
            // for converting geo-coordinates to pixel-coordinates
            String coverageName = covInfo.getCoverageName();
            try {
                meta = xq.getMetadataSource().read(coverageName);
            } catch (Exception ex) {
                throw new WCPSException(ex.getMessage(), ex);
            }
        }
        
        String name;

        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        while (node != null && finished == false) {
            if (node.getNodeName().equals("#text")) {
                node = node.getNextSibling();
                continue;
            }

            name = node.getNodeName();
            System.err.println("Current node is " + name);

            // Try Axis
            try {
                axis = new AxisName(node, xq);
                node = node.getNextSibling();
                continue;
            } catch (WCPSException e) {
                System.err.println("Failed to parse an axis!");
            }

            // Try CRS name
            try {
                crs = new Crs(node, xq);
                node = node.getNextSibling();
                if (axis == null) {
                    throw new WCPSException("Expected Axis node before CRS !");
                }
                continue;
            } catch (WCPSException e) {
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

            // Then it must be a "slicingPosition"
            if (node.getNodeName().equals("slicingPosition")) {
                domain = new ScalarExpr(node.getFirstChild(), xq);
                if (axis == null) {
                    throw new WCPSException("Expected <axis> node before <slicingPosition> !");
                }
            } else {
                throw new WCPSException("Unexpected node: " + node.getFirstChild().getNodeName());
            }

            if (axis != null && domain != null) {
                finished = true;
            }

            if (finished == true) {
                nextNode = node.getNextSibling();
            }

            node = node.getNextSibling();
        }
        
        // Pixel indices are retrieved from bbox, which is stored for XY plane only.
        if (finished == true && !crs.getName().equals(CrsUtil.IMAGE_CRS)) {
           convertToPixelCoordinate(); 
        }
    }

    /* If input coordinates are geo-, convert them to pixel coordinates. */
    private void convertToPixelCoordinate() {
        //if (meta.getCrs() == null && crs != null && crs.getName().equals(DomainElement.WGS84_CRS)) {
        if (meta.getBbox() == null && crs != null) {
            throw new RuntimeException("Coverage '" + meta.getCoverageName()
                    //+ "' is not georeferenced with 'EPSG:4326' coordinate system.");
                    + "' is not georeferenced.");
        }        
        if (crs != null && domain.isSingleValue()) {
            //if (crs.getName().equals(DomainElement.WGS84_CRS)) {
                //log.debug("CRS is '{}' and should be equal to '{}'", crs.getName(), DomainElement.WGS84_CRS);
                log.debug("[Transformed] requested subsettingCrs is '{}', should match now native CRS is '{}'", crs.getName(), meta.getBbox().getCrsName());
                try {
                    this.transformedCoordinates = true;
                    // Convert to pixel coordinates
                    Double val = domain.getSingleValue();
                    String axisName = axis.toRasQL(); //.toUpperCase();
                    coord = crs.convertToPixelIndices(meta, axisName, val);
                } catch (WCSException e) {
                    this.transformedCoordinates = false;
                    log.error("Error while transforming geo-coordinates to pixel coordinates."
                            + "The metadata is probably not valid.");
                }
            //}
        } // else no crs was embedded in the slice expression 
    }

    @Override
    public String toRasQL() {
        return child.toRasQL();
    }

    public Node getNextNode() {
        return nextNode;
    }

    public String getAxisName() {
        return this.axis.toRasQL();
    }

    public String getCrsName() {
        return this.crs.toRasQL();
    }

    public String getSlicingPosition() {
        if (transformedCoordinates) {
            return String.valueOf(coord);
        } else {
            return this.domain.toRasQL();
        }
    }
}
