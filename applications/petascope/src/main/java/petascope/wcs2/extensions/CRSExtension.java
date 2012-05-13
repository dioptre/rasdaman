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
package petascope.wcs2.extensions;

import java.util.List;
import petascope.core.DbMetadataSource;
import petascope.exceptions.WCSException;
import petascope.wcs2.parsers.GetCoverageRequest;
import petascope.util.CrsUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.PetascopeException;
import petascope.util.AxisTypes;
import petascope.wcps.server.core.Bbox;
import petascope.wcs2.parsers.GetCoverageMetadata;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSlice;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSubset;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionTrim;

/**
 * Manage CRS Extension (OGC 11-053).
 * 
 * @author <a href="mailto:cmppri@unife.it">Piero Campalani</a>
 */
public class CRSExtension implements Extension {
    
    private static final Logger log = LoggerFactory.getLogger(CRSExtension.class);
    
    @Override
    public String getExtensionIdentifier() {
        return ExtensionsRegistry.CRS_IDENTIFIER;
    }
    
    /**
     * Method for the handling of possible subsets in case of subsettingCrs not 
     * corrispondent to the one which the desired collection is natively stored.
     * 
     * @param request The WCS request, which is directly modified.
     * @param meta    Metadata of the underlying petascopedb.
     * 
     */
    protected void handle (GetCoverageRequest request, GetCoverageMetadata m, DbMetadataSource meta) throws WCSException {
        /**
         * NOTE1 (campalani): CRS transform cannot be done when only easting or
         *  only northing is known, so it must be done *outside* DimensionIntervalElement.
         *  Translation to pixel indices is moved outside DimensionInterval as well
         *  since it must be computed a posteriori of (possible) CRS transforms.
         * NOTE2 (campalani): at the same time, CRS is read only inside DimensionInterval
         *  due to the syntax of WCPS language: trim(x:<crs>(xMin,xMax),y:<crs>(yMin,yMax).
         * TODO: need to enable 3D spatial transform (x,y,elev): *Bbox* now is fixed to 2D but need to
         *  include elevation in case of 3D collections.
         */
        // Check if a CRS transform is needed for X and Y axis.
        if (request.getCRS().size() == 1 && !m.getBbox().getCrsName().equals(CrsUtil.IMAGE_CRS)) {
            if (request.getSubset(AxisTypes.X_AXIS) == null && request.getSubset(AxisTypes.Y_AXIS) == null) {  // No subsetting at all was specified
                log.warn("A subsettingCrs is stated but no subsettings were found: ignore it.");
            } else {                
                DimensionSubset xSubset = request.getSubset(AxisTypes.X_AXIS);
                DimensionSubset ySubset = request.getSubset(AxisTypes.Y_AXIS);
                DimensionSubset zSubset = request.getSubset(AxisTypes.ELEV_AXIS);
                
                if ((xSubset != null && !xSubset.isCrsTransformed())            // isCrsTransformed: avoid transform(transform)
                        || (ySubset != null && !ySubset.isCrsTransformed())) {     //
                        //|| (zSubset != null && !zSubset.isCrsTransformed())) {  // 
                        try {
                            if (request.getCRS().get(0).getSubsettingCrs() != null &&
                                    request.getCRS().get(0).getSubsettingCrs().equals(CrsUtil.IMAGE_CRS)) {
                                log.info("Subset(s) defined in pixel coordinates: no CRS reprojection need to be made.");
                                
                            } else if (request.getCRS().get(0).getSubsettingCrs() == null ||
                                    request.getCRS().get(0).getSubsettingCrs().equals(m.getBbox().getCrsName())) {
                                //if(!crs.getName().equals(DomainElement.WGS84_CRS)) {
                                log.info("Requested CRS (" + request.getCRS().get(0).getSubsettingCrs() + ") and native CRS coincide: no tranform needed.");
                                // Req7: /req/crs/getCoverage-subsettingCrs-default
                                if (request.getCRS().get(0).getSubsettingCrs()==null)
                                    request.getCRS().get(0).setSubsettingCrs(m.getBbox().getCrsName());
                                // Fill in crs of subsets, in case they were not embedded in the trimming expression of the request (es. KVP)
                                if (xSubset != null) xSubset.setCrs(m.getBbox().getCrsName());
                                if (ySubset != null) ySubset.setCrs(m.getBbox().getCrsName());
                            } else {
                                log.info("Requested CRS (" + request.getCRS().get(0).getSubsettingCrs() + ") and native CRS (" + m.getBbox().getCrsName() + ") differ: tranform needed.");

                                // If elevation is required to be transformed then throw exception: currently only 2D
                                if (zSubset != null)
                                    throw new WCSException(ExceptionCode.OperationNotSupported, "3D spatial transforms are currently not supported.");
                                
                                // Extrapolate values independently of Trim/Slice request
                                String[] subX, subY;
                                CrsUtil crsTool;
                                List<Double> temp;
                                if (xSubset == null) subX = new String[2];
                                else subX = (xSubset instanceof DimensionTrim)
                                        ? new String[] {((DimensionTrim)xSubset).getTrimLow(), ((DimensionTrim)xSubset).getTrimHigh()}
                                        : new String[] {((DimensionSlice)xSubset).getSlicePoint(), ((DimensionSlice)xSubset).getSlicePoint()};
                                if (ySubset == null) subY = new String[2];
                                else subY = (ySubset instanceof DimensionTrim)
                                        ? new String[] {((DimensionTrim)ySubset).getTrimLow(), ((DimensionTrim)ySubset).getTrimHigh()}
                                        : new String[] {((DimensionSlice)ySubset).getSlicePoint(), ((DimensionSlice)ySubset).getSlicePoint()};

                                // If only one subsetting was set in the request, fill the other dimension with bbox values (transformed in subsettingCrs)
                                if (subX[0] == null || subY[0] == null) {
                                    crsTool = new CrsUtil(m.getBbox().getCrsName(), request.getCRS().get(0).getSubsettingCrs());
                                    temp = crsTool.transform(m.getBbox().getLow1(), m.getBbox().getLow2(), m.getBbox().getHigh1(), m.getBbox().getHigh2());
                                    subX = (subX[0]==null) ? new String[] {"" + temp.get(0), "" + temp.get(2)} : subX;
                                    subY = (subY[0]==null) ? new String[] {"" + temp.get(1), "" + temp.get(3)} : subY;
                                }
                                
                                log.debug("Interval values: [" + subX[0] + ":" + subX[1] + "," + subY[0] + ":" + subY[1] + "]");
                                
                                // Now all values are filled: transform
                                crsTool = new CrsUtil(request.getCRS().get(0).getSubsettingCrs(), m.getBbox().getCrsName());
                                // crsTool.transform(xMin,yMin,xMax,yMax):
                                temp = crsTool.transform(subX[0], subY[0], subX[1], subY[1]);

                                // Update dimension intervals nodes: one dimension might not be trimmed/sliced!
                                if (xSubset != null) {
                                    if (xSubset instanceof DimensionTrim) {
                                        ((DimensionTrim)xSubset).setTrimLow(temp.get(0));
                                        ((DimensionTrim)xSubset).setTrimHigh(temp.get(2));
                                    } else ((DimensionSlice)xSubset).setSlicePoint(temp.get(0)); //~temp.get(2)
                                } 
                                if (ySubset != null) {
                                    if (ySubset instanceof DimensionTrim) {
                                        ((DimensionTrim)ySubset).setTrimLow(temp.get(1));
                                        ((DimensionTrim)ySubset).setTrimHigh(temp.get(3));
                                    } else ((DimensionSlice)ySubset).setSlicePoint(temp.get(1)); //~temp.get(3)
                                }

                                // Set transformed to true
                                if (xSubset != null) xSubset.isCrsTransformed(true);
                                if (ySubset != null) ySubset.isCrsTransformed(true);
                                // Change crs
                                if (xSubset != null) xSubset.setCrs(m.getBbox().getCrsName());
                                if (ySubset != null) ySubset.setCrs(m.getBbox().getCrsName());

                                log.debug("Transformed Interval values: [" + temp.get(0) + ":" + temp.get(2) + "," + temp.get(1) + ":" + temp.get(3) + "]");
                            }  
                        } catch (WCSException e) {
                            if (((PetascopeException)e).getExceptionCode().getExceptionCode().equalsIgnoreCase(ExceptionCode.InvalidMetadata.getExceptionCode()))
                                throw new WCSException(ExceptionCode.SubsettingCrsNotSupported, 
                                        "subsetting Crs " + request.getCRS().get(0).getSubsettingCrs() + " is not supported by the server.");
                            else throw new WCSException("Error while comparing requested subsettingCRS and native CRS of coverage " + m.getCoverageId());
                        } catch (Exception e) {
                            log.error(e.getMessage());
                            throw new WCSException("Error while comparing requested subsettingCRS and native CRS of coverage " + m.getCoverageId());
                        }
                }
            } // else: CRS transform already done (if necessary) by setBounds call.
        } else {
            // Req7: /req/crs/getCoverage-subsettingCrs-default
            // NOTE: if no CRS instance is presence, hence both subsettingCRS and outputCRS were not specified.
            if (request.getCRS().isEmpty()) 
                request.getCRS().add(new GetCoverageRequest.CRS(m.getBbox().getCrsName(), null));
            else request.getCRS().get(0).setSubsettingCrs(m.getBbox().getCrsName());
        }

        // Req10: /req/crs/getCoverage-outputCrs-default
        if (request.getCRS().get(0).getOutputCrs() == null)
            request.getCRS().get(0).setOutputCrs(request.getCRS().get(0).getSubsettingCrs());  
        
        // Check intersection with coverage Bbox:
        // X axis
        if (!axisDomainIntersection(request.getSubset(AxisTypes.X_AXIS), m, request.getCRS().get(0).getSubsettingCrs())) {
            throw new WCSException("Requested subset \"" + request.getSubset(AxisTypes.X_AXIS) + "\" is out of coverage bounds \"" + 
                    m.getBbox().getLow1() + ":" + m.getBbox().getHigh1() + "\".");
        }
        // Y axis
        if (!axisDomainIntersection(request.getSubset(AxisTypes.Y_AXIS), m, request.getCRS().get(0).getSubsettingCrs())) {
            throw new WCSException("Requested subset \"" + request.getSubset(AxisTypes.Y_AXIS) + "\" is out of coverage bounds \"" + 
                    m.getBbox().getLow2() + ":" + m.getBbox().getHigh2() + "\".");
        }

        // Request object modified by reference: no need to return a new Request.
        return;
    }
    
    /**
     * @param   DimensionSubset subset  A subset from the WCS GetCoverage request
     * @param   Bbox bbox    Bbox of the requested coverage
     * @return  boolean      Does request subset intersect with coverage Bbox?.
     */
    public boolean axisDomainIntersection (DimensionSubset subset, GetCoverageMetadata meta, String subsettingCrs) {
        // If subsettingCrs is CRS:1, need to check cellDomains instead of geo-Bbox.
        boolean imageSubset = subsettingCrs.equals(CrsUtil.IMAGE_CRS);
        
        // X axis
        if (subset.getDimension().equals(AxisTypes.X_AXIS)) {
            if (subset != null) {
                // Set bounds
                double xMin = imageSubset ? meta.getMetadata().getCellDomainByName(AxisTypes.X_AXIS).getLo().doubleValue() : meta.getBbox().getLow1();
                double xMax = imageSubset ? meta.getMetadata().getCellDomainByName(AxisTypes.X_AXIS).getHi().doubleValue() : meta.getBbox().getHigh1();

                if (subset instanceof DimensionTrim &&
                        new Double(((DimensionTrim)subset).getTrimLow()) < xMin && 
                        new Double(((DimensionTrim)subset).getTrimHigh()) < xMax) {
                    return false;
                } else if (subset instanceof DimensionSlice &&
                        (new Double(((DimensionSlice)subset).getSlicePoint()) < xMin ||
                        new Double(((DimensionSlice)subset).getSlicePoint()) > xMax))
                    return false;
            }
        }
        // Y axis
        if (subset.getDimension().equals(AxisTypes.Y_AXIS)) {
            if (subset != null) {
                // Set bounds
                double yMin = imageSubset ? meta.getMetadata().getCellDomainByName(AxisTypes.Y_AXIS).getLo().doubleValue() : meta.getBbox().getLow2();
                double yMax = imageSubset ? meta.getMetadata().getCellDomainByName(AxisTypes.Y_AXIS).getHi().doubleValue() : meta.getBbox().getHigh2();
                
                if (subset instanceof DimensionTrim &&
                        new Double(((DimensionTrim)subset).getTrimLow()) < yMin && 
                        new Double(((DimensionTrim)subset).getTrimHigh()) < yMax) {
                    return false;
                } else if (subset instanceof DimensionSlice &&
                        (new Double(((DimensionSlice)subset).getSlicePoint()) < yMin ||
                        new Double(((DimensionSlice)subset).getSlicePoint()) > yMax))
                    return false;
            }
        }
        
        return true;
    }
}