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

import petascope.exceptions.WCPSException;
import org.w3c.dom.*;

public class StringScalarExpr implements IRasNode {

    private String op, string;
    private CoverageExpr cov;

    public StringScalarExpr(Node node, XmlQuery xq) throws WCPSException {
        while ((node != null) && (node.getNodeName().equals("#text"))) {
            node = node.getNextSibling();
        }

        if (node.getNodeName().equals("stringIdentifier")) {
            Node child = node.getFirstChild();
            cov = new CoverageExpr(child, xq);
            op = "id";
        } else if (node.getNodeName().equals("stringConstant")) {
            op = "constant";
            string = node.getNodeValue();
        } else {
            throw new WCPSException("Unknown String expr node: " + node.getNodeName());
        }
    }

    public String toRasQL() {
        String result = "";
        if (op.equals("constant")) {
            result = string;
        }
        if (op.equals("id")) {
            result = cov.toRasQL();
        }

        return result;
    }
}
