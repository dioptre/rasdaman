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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCPSException;
import org.w3c.dom.*;

//TODO(smsorin): how do we do combine this ?

/** NOTE(campalani): rasdaman enterprise already implements reprojection of coverage
 *  through 'project' function.
 */
public class CrsTransformCoverageExpr implements IRasNode, ICoverageInfo {
    
    private static Logger log = LoggerFactory.getLogger(CrsTransformCoverageExpr.class);
    
    public CrsTransformCoverageExpr(Node node, XmlQuery xq)
            throws WCPSException {
        log.trace(node.getNodeName());
        // Call function to reproject the bounding box:

        //  foreach XYplane slice (=spatialcoverage)
            //      extract xy pixels 
            //      project(arr, bbox, s_crs, t_crs);
            //      replace it in the ...DOVE? data is returned in executeRasqlQuery..
            // end
        throw new WCPSException(ExceptionCode.NoApplicableCode, "crsTransform expression not implemented.");
    }

    @Override
    public String toRasQL() {
        return "";
    }

    @Override
    public CoverageInfo getCoverageInfo() {
        return null;
    }
}
