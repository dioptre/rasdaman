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
package petascope.wcps.grammar;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.WCPSConstants;

/**
 * MetaDataExpr
 * Creation date: (3/3/2003 2:28:43 AM)
 * @author: mattia parigiani, Sorin Stancu-Mara, Andrei Aiordachioaie
 */
public class MetaDataExpr implements IParseTreeNode {
    
    private static Logger log = LoggerFactory.getLogger(MetaDataExpr.class);

    CoverageExpr expr;
    String field;
    String function;
    IParseTreeNode param;

    public MetaDataExpr(DomainExpr dom) {
        function = WCPSConstants.MSG_DOMAIN;
        param = dom;
    }

    // Identifier, ImageCRS, ImageCRSDomain, CrsSet, NullSet
    public MetaDataExpr(String op, CoverageExpr expr) {
        function = op;
        this.expr = expr;
    }

    // ImageCRSDomain, interpolationDefault, interpolationSet
    public MetaDataExpr(String op, CoverageExpr expr, String str) {
        function = op;
        this.expr = expr;
        this.field = str;
    }

    public String toXML() {
        String result = "";

        if (function.equalsIgnoreCase(WCPSConstants.MSG_IMAGE_CRSDOMAIN)) {
            result += "<" + WCPSConstants.MSG_IMAGE_CRSDOMAIN + ">";
            result += expr.toXML();

            if (field != null) {
                result += "<" + WCPSConstants.MSG_AXIS + ">" + field + "</" + 
                        WCPSConstants.MSG_AXIS + ">";
            }

            result += "</" + WCPSConstants.MSG_IMAGE_CRSDOMAIN + ">";
        } else if (function.equalsIgnoreCase(WCPSConstants.MSG_DOMAIN)) {
            result = "<" + WCPSConstants.MSG_DOMAIN_METADATA_CAMEL + ">" + param.toXML() + "</" + 
                    WCPSConstants.MSG_DOMAIN_METADATA_CAMEL + ">";
        } else if (function.equalsIgnoreCase(WCPSConstants.MSG_INTERPOLATION_DEFAULT)) {
            result += "<" + WCPSConstants.MSG_INTERPOLATION_DEFAULT + ">";
            result += expr.toXML();
            result += "<" + WCPSConstants.MSG_NAME + ">" + param + "</" + WCPSConstants.MSG_PARAM + ">";
            result += "</" + WCPSConstants.MSG_INTERPOLATION_DEFAULT + ">";
        } else if (function.equalsIgnoreCase(WCPSConstants.MSG_INTERPOLATION_SET)) {
            result += "<" + WCPSConstants.MSG_INTERPOLATION_SET + ">";
            result += expr.toXML();
            result += "<" + WCPSConstants.MSG_NAME + ">" + param + "</" + WCPSConstants.MSG_PARAM + ">";
            result += "</" + WCPSConstants.MSG_INTERPOLATION_SET + ">";
        } else if (function.equalsIgnoreCase(WCPSConstants.MSG_IDENTIFIER)
                || function.equalsIgnoreCase(WCPSConstants.MSG_IMAGE_CRS) || function.equalsIgnoreCase(WCPSConstants.MSG_CRS_SET)
                || function.equalsIgnoreCase(WCPSConstants.MSG_NULL_SET)) {
            result += "<" + function + ">";
            result += expr.toXML();
            result += "</" + function + ">";
        } else {
            log.error(WCPSConstants.ERRTXT_UNKNOWN_METADATAEXPR + ": " + function);
        }

        return result;
    }
}
