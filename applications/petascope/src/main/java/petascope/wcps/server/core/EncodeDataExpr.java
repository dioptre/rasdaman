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
import petascope.exceptions.WCPSException;
import org.w3c.dom.*;

// This is the equivalent of the "ProcessingExprType" complex XML type.
public class EncodeDataExpr implements IRasNode {
    
    private static Logger log = LoggerFactory.getLogger(EncodeDataExpr.class);

    private IRasNode coverageExprType;
    private String extraParams;
    private String format;
    private String mime;
    private Boolean store;

    public EncodeDataExpr(Node node, XmlQuery request) throws WCPSException {
        Node child;
        String nodeName;
        log.trace(node.getNodeName());

        for (child = node.getFirstChild(); child != null; child = child.getNextSibling()) {
            nodeName = child.getNodeName();

            if (nodeName.equals("#text")) {
                continue;
            }

            if (nodeName.equals("format")) {
                format = child.getFirstChild().getNodeValue();
                mime = request.getMetadataSource().formatToMimetype(format);
                log.trace("  format: " + format + ", mime: " + mime);
                continue;
            }

            if (nodeName.equals("extraParameters")) {
                extraParams = child.getFirstChild().getNodeValue();
                log.trace("  extra params: " + extraParams);
                continue;
            }

            coverageExprType = new CoverageExpr(child, request);
        }

        Node _store = node.getAttributes().getNamedItem("store");
        if (_store != null) {
            store = _store.getNodeValue().equals("true");
        }
    }

    public String getMime() {
        return mime;
    }

    public String toRasQL() {
        // TODO: cjucovschi - implement store

        String result;

        if (format.equals("raw")) {
            result = coverageExprType.toRasQL();
        } else {
            result = format + "(" + coverageExprType.toRasQL();

            if (extraParams != null) {
                extraParams = '"' + extraParams + '"';
                result = result + ", " + extraParams;
            }

            result = result + ")";
        }

        log.trace("Returning EncodeExpression:" + result);
        return result;
    }
}
