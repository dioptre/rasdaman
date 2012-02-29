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
import java.util.Iterator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WCPSException;
import org.w3c.dom.*;
import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;

public class Crs implements IRasNode {

    Logger log = LoggerFactory.getLogger(Crs.class);
    private String crsName;

    public Crs(Node node, XmlQuery xq) throws WCPSException {
             while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        if (node != null && node.getNodeName().equals("srsName")) {
            String val = node.getTextContent();
            this.crsName = val;
            //if (crsName.equals(DomainElement.IMAGE_CRS) || crsName.equals(DomainElement.WGS84_CRS)) {
                log.trace("Found CRS: " + crsName);
            //} else {
            //    throw new WCPSException("Invalid CRS: '" + crsName + "'");
            //}
        } else {
            throw new WCPSException("Could not find a 'srsName' node !");
        }
    }

    /***
     * Converts an array of 4 coordinates (bounding box) expressed in the
     * current CRS to pixel coordinates.
     * NOTE: origin of pixel grid is UPPER-LEFT corner (!!)
     * @param u2 Left-most  X point (CRS coordinate)
     * @param u3 Right-most X point (CRS coordinate)
     * @param v2 Lower-most Y point (CRS coordinate)
     * @param v3 Upper-most Y point (CRS coordinate)
     * @return array of integers, pixel coordinates that represent the given CRS
     * coordinates.
     */
    @Deprecated
    public long[] convertToPixelCoordinates(Metadata meta, String axisName, Double u2, Double u3, Double v2, Double v3) throws WCSException {
        //Wgs84Crs crs = meta.getCrs();
        Bbox bbox = meta.getBbox();
        long px0 = -1, px1 = -1, py0 = -1, py1 = -1;
        // Convert bounding box values to pixel coordinates
        //if (crsName.equals(DomainElement.WGS84_CRS)) {
            //log.trace("Converting WGS84 axis {} interval to pixel coordinates ...", axisName);
            log.trace("Converting {} axis {} interval to pixel coordinates ...", bbox.getCrsName(), axisName);
            /* Image coordinates */
            Iterator<CellDomainElement> it = meta.getCellDomainIterator();
            CellDomainElement X = it.next();
            CellDomainElement Y = it.next();
            if (X == null || Y == null) {
                log.error("Could not find the X or Y axis for coverage: " + meta.getCoverageName());
                throw new WCSException(ExceptionCode.NoApplicableCode, "Could not find the X or Y axis for coverage: " + meta.getCoverageName());
            }
            int x0 = X.getLo().intValue();
            int x1 = X.getHi().intValue();
            int y0 = Y.getLo().intValue();
            int y1 = Y.getHi().intValue();
            int W = x1-x0;
            int H = y1-y0;

            log.trace("Pixel Coordinates: X01 (" + x0 + "," + x1 + ") + Y01 (" + y0 + "," + y1 + ")");
            /* BBOX span */
            double x2 = bbox.getLow1();
            double y2 = bbox.getLow2();
            double x3 = bbox.getHigh1();
            double y3 = bbox.getHigh2();
            double bboxW = x3-x2;
            double bboxH = y3-y2;
            log.trace("BBOX Coordinates: X23 (" + x2 + "," + x3 + ") + Y23 (" + y2 + "," + y3 + ")");
            /* For WGS84, the offset = (# pixels)/(CRS span) */
            double oX = bbox.getOffset1();
            double oY = bbox.getOffset2();

            /* The actual conversion is below: */
            if (axisName.equals("X")) {
                px0 = Math.round((u2 - x2) / oX) + x0;
                //px0 = Math.round(W*(u2-x2)/bboxW);
                px1 = Math.round((u3 - u2) / oX) + px0;
                //px1 = Math.round(W*(u3-x2)/bboxW);
                // Check outside bounds:
                px0 = (px0<x0) ? (x0) : ((px0>x1)?x1:px0);
                px1 = (px1<x0) ? (x0) : ((px1>x1)?x1:px1);
                log.debug("CRS Coordinates on axis X: U23 (" + u2 + "," + u3 + ")");
                log.debug("Pixel Coordinates on axis X: U01 (" + px0 + "," + px1 + ") ");
            }
            if (axisName.equals("Y")) {
                py0 = Math.round((y3 - v3) / oY) + y0;
                //py0 = Math.round(H*(v2-y3)/(-bboxH));
                py1 = Math.round((v3 - v2) / oY) + py0;
                //py1 = Math.round(H*(v3-y3)/(-bboxH));
                // Check outside bounds:
                py0 = (py0<y0) ? (y0) : ((py0>y1)?y1:py0);
                py1 = (py1<y0) ? (y0) : ((py1>y1)?y1:py1);
                log.debug("CRS Coordinates on axis Y:  V23 (" + v2 + "," + v3 + ")");
                log.debug("Pixel Coordinates on axis Y: V01 (" + py0 + "," + py1 + ")");
            }

        //}
            
        long[] longCoord = {(px0<px1)?px0:px1, (px0<px1)?px1:px0, (py0<py1)?py0:py1, (py0<py1)?py1:py0};        
        
        return longCoord;
    }
    
    /***
     * Converts an interval subset to CRS:1 domain (grid indices).
     * NOTE1:   origin of pixel grid is UPPER-LEFT corner (!!)
     * NOTE2:   Currently it works for axis whose domain is numerical.
     *          To be updated when *strlo* and *strhi* extents will be effectively possible (e.g. temporal axis).
     * @param coordMin Min value of interval
     * @param coordMax Max value of interval
     * @return Interval transformed values.
     * coordinates.
     */
    public int[] convertToPixelIndices(Metadata meta, String axisName, Double coordLo, Double coordHi) throws WCSException {
       
        // Put in order to prevent call error
        if (coordHi < coordLo) {
            log.error("Argument \"high\" is lower than \"low\": " + coordHi + "<" + coordLo + " (rasql error 389 would be raised: \"in case of fixed bounds, the upper one can not be smaller than the lower one\")");
            throw new WCSException(ExceptionCode.NoApplicableCode, "Could not find the \"" + axisName + "\" axis for coverage:" + meta.getCoverageName());
        }
        
        // Convert domain-space values to cell-space indices
        log.trace("Converting axis {} interval to pixel indices ...", axisName);
        
        // Image coordinates
        DomainElement dom = meta.getDomainByName(axisName);
        CellDomainElement cdom = meta.getCellDomainByName(axisName);
        if (cdom == null || dom == null) {
            log.error("Could not find the \"" + axisName + "\" axis for coverage: " + meta.getCoverageName());
            throw new WCSException(ExceptionCode.NoApplicableCode, "Could not find the \"" + axisName + "\" axis for coverage:" + meta.getCoverageName());
        }
        
        // Get cellDomain extremes
        int pxLo = cdom.getLo().intValue();
        int pxHi = cdom.getHi().intValue();
        log.trace("CellDomain extremes values: LOW:" + pxLo + ", HIGH:" + pxHi);
        
        // Get Domain extremes (real sdom)
        double domLo = dom.getNumLo();
        double domHi = dom.getNumHi();
        log.trace("Domain extremes coordinates: (" + domLo + "," + domHi + ")");

        // Get cell dimension 
        double cellWidth = (domHi-domLo)/(double)(pxHi-pxLo);

        // Conversion to pixel domain
        int[] out = new int[] {
            (int)Math.round((coordLo - domLo) / cellWidth) + pxLo,
            (int)Math.round((coordHi - domLo) / cellWidth) + pxLo
        };
        log.debug("Transformed coords indices (" + out[0] + "," + out[1] + ")");
        
        // Check outside bounds:
        out[0] = (out[0]<pxLo) ? pxLo : ((out[0]>pxHi)?pxHi:out[0]);
        out[1] = (out[1]<pxLo) ? pxLo : ((out[1]>pxHi)?pxHi:out[1]);
        log.debug("Transformed rebounded coords indices (" + out[0] + "," + out[1] + ")");

        return out;
    }
    // Dummy overload (for DimensionPointElements)
    public int convertToPixelIndices(Metadata meta, String axisName, Double value) throws WCSException {
        return convertToPixelIndices(meta, axisName, value, value)[0];
    }

    @Override
    public String toRasQL() {
        return crsName;
    }

    public String getName() {
        return crsName;
    }
}
