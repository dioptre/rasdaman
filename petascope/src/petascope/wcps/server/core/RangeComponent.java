/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */
package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.WCPSException;
import org.w3c.dom.*;
import petascope.wcps.server.exceptions.InvalidCrsException;


/**
 * This is a component of a RangeStructure. 
 * @author Andrei Aiordachioaie
 */
public class RangeComponent implements IRasNode, ICoverageInfo {

    private String field = null;
    private CoverageInfo info = null;
    private CoverageExpr expr = null;

    public RangeComponent(Node node, XmlQuery xq)
            throws WCPSException {

        String nodeName = node.getNodeName();
        if (nodeName.equals("component"))
            node = node.getFirstChild();
            
        while (node != null)
        {
            nodeName = node.getNodeName();
            if (nodeName.equals("#text"))
            {
                node = node.getNextSibling();
                continue;
            }

            if (nodeName.equals("field"))
                this.field = node.getTextContent();
            else
                try
                {
                    this.expr = new CoverageExpr(node, xq);
                    this.info = expr.getCoverageInfo();
                }
                catch (InvalidCrsException e2)
                {}
                catch (WCPSException e)
                {
                    System.err.println("Could not match CoverageExpr inside RangeExpr. Next node: " + nodeName);
                    throw e;
                }

            node = node.getNextSibling();
        }
    }

    public CoverageInfo getCoverageInfo() {
        return info;
    }

    public String toRasQL() {
        String result = "";

        if (this.expr != null)
            result = expr.toRasQL();

        return result;
    }
}
