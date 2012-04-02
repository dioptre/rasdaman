/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package petascope.wcps.server.core;

import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCPSException;
import petascope.exceptions.WCSException;
import petascope.util.CrsUtil;
import java.util.List;

/**
 *  Generalization of Wgs84Crs class (the source is almost cloned).
 * 
 *  NOTES: 
 *  In order to get it working, the coverage axis need to be linked to a CRS != CRS:1.
 *  This means that petascopedb must be altered.
 *  For each coverage: 
 *      · INSERT INTO ps_crs the CRS name° of the coverage (if not yet present);
 *      · UPDATE ps_crsset and fill 'crs' field with the ID of the proper CRS (from ps_crs table).
 * 
 *  °The name should be registered with OGC-NA [www.epsg.org].
 * 
 * TODO: allow 3D bboxes for x/y/elev collections.
 *       Is Bbox to me meant as spatial? Add temporal dimension as well?
 * 
 * @author <a href="mailto:cmppri@unife.it">Piero Campalani</a>
 */
public class Bbox implements Cloneable {

    private String crsName;
    private Double low1, high1, low2, high2;
    private Double WGS84low1, WGS84high1, WGS84low2, WGS84high2;
    private Double offset1, offset2;        // precomputed world-to-crs offsets
    
    public Bbox(String crs, Double l1, Double h1, Double l2, Double h2, Double o1, Double o2) throws WCPSException, WCSException {
        if ((l1 == null) || (h1 == null) || (l2 == null) || (h2 == null)) {
            throw new WCPSException(ExceptionCode.InvalidMetadata,
                    "Invalid bounding box: null element encountered.");
        }
        if ((o1 == null) || (o2 == null)) {
            throw new WCPSException(ExceptionCode.InvalidMetadata, "Invalid offsets: null element encountered.");
        }
        if (crs == null) {
            throw new WCPSException(ExceptionCode.InvalidMetadata, "Invalid CRS name: null element encountered");
        }

        crsName = crs;
        
        low1 = l1;
        low2 = l2;
        high1 = h1;
        high2 = h2;

        offset1 = o1;
        offset2 = o2;
        
        // Get WGS84 bounding box (GetCapabilites WCS 2.0)
        if (crsName.equals(CrsUtil.WGS84_CRS)) {
            WGS84low1 = low1;   WGS84high1 = high1;
            WGS84low2 = low2;   WGS84high2 = high2;
        } else {
            try {
                CrsUtil crsTool = new CrsUtil(crsName, CrsUtil.WGS84_CRS);
                List<Double> temp = crsTool.transform(low1, low2, high1, high2);
                WGS84low1 = temp.get(0);    WGS84high1 = temp.get(2);
                WGS84low2 = temp.get(1);    WGS84high2 = temp.get(3);
            } catch (WCSException e) {
                //throw (WCSException)e;
            }            
        }
    }

    @Override
    public Bbox clone() {
        try {
            return new Bbox(crsName, new Double(getLow1()), new Double(getHigh1()),
                    new Double(getLow2()), new Double(getHigh2()),
                    new Double(getOffset1()), new Double(getOffset2()));
        } catch (WCPSException e) {
            return null;
        } catch (WCSException e) {
            return null;
        }
    }

    @Override
    public String toString() {
        String d = "CRS '" + getCrsName() + "' { Bounding Box [X(" + getLow1() + ", " + getHigh1() + "), "
                + "Y(" + getLow2() + ", " + getHigh2() + ")], "
                + "Offsets [X(" + getOffset1() + "), Y(" + getOffset2() + ")]}";
        return d;
    }

    /**
     * @return the low1
     */
    public Double getLow1() {
        return low1;
    }

    /**
     * @return the high1
     */
    public Double getHigh1() {
        return high1;
    }

    /**
     * @return the low2
     */
    public Double getLow2() {
        return low2;
    }

    /**
     * @return the high2
     */
    public Double getHigh2() {
        return high2;
    }

    /**
     * @return the offset1
     */
    public Double getOffset1() {
        return offset1;
    }

    /**
     * @return the offset2
     */
    public Double getOffset2() {
        return offset2;
    }

    /**
     * @return the CRS name
     */
    public String getCrsName() {
        return crsName;
    }   
    
    /**
     * @return the WGS84Low1
     */
    public Double getWgs84Low1() {
        return WGS84low1;
    }

    /**
     * @return the WGS84high1
     */
    public Double getWgs84High1() {
        return WGS84high1;
    }

    /**
     * @return the WGS84low2
     */
    public Double getWgs84Low2() {
        return WGS84low2;
    }

    /**
     * @return the WGS84high2
     */
    public Double getWgs84High2() {
        return WGS84high2;
    }

}