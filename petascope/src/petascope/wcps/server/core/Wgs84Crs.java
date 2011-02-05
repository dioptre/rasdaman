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

import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCPSException;

/**
 * Represents a WGS Coordinate Reference System (CRS) and its bounding box.
 *
 *
 * @version        10.Sep 2009
 * @author         Andrei Aiordachioaie
 */
public class Wgs84Crs implements Cloneable {

    private String name = "WGS84";
    private Double low1, high1, low2, high2;
    private Double offset1, offset2;        // precomputed world-to-crs offsets

    public Wgs84Crs(Double l1, Double h1, Double l2, Double h2, Double o1, Double o2) throws WCPSException {
        if ((l1 == null) || (h1 == null) || (l2 == null) || (h2 == null)) {
            throw new WCPSException(ExceptionCode.InvalidMetadata,
                    "Invalid WGS 84 bounding box: null element encountered.");
        }
        if ((o1 == null) || (o2 == null)) {
            throw new WCPSException(ExceptionCode.InvalidMetadata, "Invalid WGS 84 offsets: null element encountered.");
        }

        low1 = l1;
        low2 = l2;
        high1 = h1;
        high2 = h2;

        offset1 = o1;
        offset2 = o2;
    }

    public Wgs84Crs clone() {
        try {
            return new Wgs84Crs(new Double(getLow1()), new Double(getHigh1()),
                    new Double(getLow2()), new Double(getHigh2()),
                    new Double(getOffset1()), new Double(getOffset2()));
        } catch (WCPSException e) {
            return null;
        }
    }

    public String toString() {
        String d = "CRS '" + getName() + "' { Bounding Box [X(" + getLow1() + ", " + getHigh1() + "), "
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
     * @return the name
     */
    public String getName() {
        return name;
    }
}
