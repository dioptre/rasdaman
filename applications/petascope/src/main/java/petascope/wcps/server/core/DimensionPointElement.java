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

import java.util.Iterator;
import petascope.core.Metadata;
import petascope.exceptions.WCPSException;
import petascope.exceptions.WCSException;
import org.w3c.dom.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.CrsUtil;
import petascope.util.WCPSConstants;

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

        while ((node != null) && node.getNodeName().equals("#" + WCPSConstants.MSG_TEXT)) {
            node = node.getNextSibling();
        }
        
        log.trace(node.getNodeName());
        
        if (covInfo.getCoverageName() != null) {
            // Add Bbox information from coverage metadata, may be useful
            // for converting geo-coordinates to pixel-coordinates
            String coverageName = covInfo.getCoverageName();
            try {
                meta = xq.getMetadataSource().read(coverageName);
            } catch (Exception ex) {
                log.error(ex.getMessage());
                throw new WCPSException(ex.getMessage(), ex);
            }
        }
        
        String name;

        while (node != null && finished == false) {
            if (node.getNodeName().equals("#" + WCPSConstants.MSG_TEXT)) {
                node = node.getNextSibling();
                continue;
            }

            // Try Axis
            try {
                log.trace("  " + WCPSConstants.MSG_MATCHING_AXIS_NAME);
                axis = new AxisName(node, xq);
                node = node.getNextSibling();
                continue;
            } catch (WCPSException e) {
            }

            // Try CRS name
            try {
                log.trace("  " + WCPSConstants.MSG_MATCHING_CRS);
                crs = new Crs(node, xq);
                node = node.getNextSibling();
                if (axis == null) {
                    throw new WCPSException(WCPSConstants.ERRTXT_EXPECTED_AXIS_NODE);
                }
                continue;
            } catch (WCPSException e) {
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
//                log.error("Failed to parse domain metadata!");
//            }

            // Then it must be a "slicingPosition"
            if (node.getNodeName().equals(WCPSConstants.MSG_SLICING_POSITION)) {
                log.trace("  " + WCPSConstants.MSG_SLICE_POSITION);
                domain = new ScalarExpr(node.getFirstChild(), xq);
                if (axis == null) {
                    throw new WCPSException(WCPSConstants.ERRTXT_EXPECTED_AXIS_NODE_SLICINGP);
                }
            } else {
                throw new WCPSException(WCPSConstants.ERRTXT_UNEXPETCTED_NODE + ": " + node.getFirstChild().getNodeName());
            }

            if (axis != null && domain != null) {
                finished = true;
            }

            if (finished == true) {
                nextNode = node.getNextSibling();
            }

            node = node.getNextSibling();
        }
        
        if (crs == null) {
            // if no CRS is specified assume native CRS -- DM 2012-mar-05
            String axisName = axis.toRasQL();

            DomainElement axisDomain = meta.getDomainByName(axisName);
            if (axisDomain != null) {
              Iterator<String> crsIt = axisDomain.getCrsSet().iterator();
              if (crsIt.hasNext()) {
                String crsname = crsIt.next();
                log.info(WCPSConstants.MSG_USING_NATIVE_CRS + ": " + crsname);
                crs = new Crs(crsname);
              } else {
                log.warn(WCPSConstants.WARNTXT_NO_NATIVE_CRS_P1 + " " + axisName + ", " + WCPSConstants.WARNTXT_NO_NATIVE_CRS_P2);
                crs = new Crs(CrsUtil.IMAGE_CRS);
              }
            }
        }
        
        // Pixel indices are retrieved from bbox, which is stored for XY plane only.
        if (finished == true && crs != null && !crs.getName().equals(CrsUtil.IMAGE_CRS)) {
           convertToPixelCoordinate(); 
        }
    }

    /* If input coordinates are geo-, convert them to pixel coordinates. */
    private void convertToPixelCoordinate() {
        //if (meta.getCrs() == null && crs != null && crs.getName().equals(DomainElement.WGS84_CRS)) {
        if (meta.getBbox() == null && crs != null) {
            throw new RuntimeException(WCPSConstants.MSG_COVERAGE + " '" + meta.getCoverageName()
                    //+ "' is not georeferenced with 'EPSG:4326' coordinate system.");
                    + "' " + WCPSConstants.ERRTXT_IS_NOT_GEOREFERENCED);
        }        
        if (crs != null && domain.isSingleValue()) {
            //if (crs.getName().equals(DomainElement.WGS84_CRS)) {
                //log.debug("CRS is '{}' and should be equal to '{}'", crs.getName(), DomainElement.WGS84_CRS);
                log.debug(WCPSConstants.DEBUGTXT_REQUESTED_SUBSETTING, crs.getName(), meta.getBbox().getCrsName());
                try {
                    this.transformedCoordinates = true;
                    // Convert to pixel coordinates
                    Double val = domain.getSingleValue();
                    String axisName = axis.toRasQL(); //.toUpperCase();
                    coord = crs.convertToPixelIndices(meta, axisName, val);
                } catch (WCSException e) {
                    this.transformedCoordinates = false;
                    log.error(WCPSConstants.ERRTXT_ERROR_WHILE_TRANSFORMING);
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
