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

import petascope.util.WCPSConstants;

/**
 * ComplexConst
 * @author: Andrei Aiordachioaie
 */
public class ComplexConst {

    String re, im;

    /**
     * ComplexLit constructor comment.
     */
    public ComplexConst() {
        super();
    }

    public ComplexConst(String val) {
        int pos = val.indexOf(WCPSConstants.MSG_PLUS_I, 0);

        if (pos != -1) {
            re = val.substring(0, pos - 1);
            im = val.substring(pos + 2, val.length());
        }
    }

    public String toXML() {
        return "<" + WCPSConstants.MSG_COMPLEX_CONSTANT + "><" + WCPSConstants.MSG_RE + ">" + re + 
                "</" + WCPSConstants.MSG_RE + "><" + WCPSConstants.MSG_IM + ">" + im + "</" + 
                WCPSConstants.MSG_IM + "></" + WCPSConstants.MSG_COMPLEX_CONSTANT + ">";
    }
}
