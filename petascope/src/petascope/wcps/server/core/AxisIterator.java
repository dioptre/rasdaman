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
import java.math.BigInteger;
import org.w3c.dom.*;
import petascope.exceptions.ExceptionCode;

public class AxisIterator implements IRasNode {

    private String var, varTranslation;
    private AxisName axis;
    private NumericScalarExpr hi, lo;

    public AxisIterator(Node node, XmlQuery xq, String newIteratorName) throws WCPSException {
        while ((node != null) && node.getNodeName().equals("#text")) {
            node = node.getNextSibling();
        }

        while (node != null) {
            String nodeName = node.getNodeName();

            if (nodeName.equals("iteratorVar")) {
                var = node.getTextContent();
                // This variable will be referenced later on. Translate it.
                xq.addReferenceVariable(var, newIteratorName);
                varTranslation = xq.getReferenceVariableName(var);
            } else if (nodeName.equals("axis")) {
                axis = new AxisName(node, xq);
            } else {
                // Should contain the limits
                // TODO: Implement ImageCrsDomainMetadataType class
                if (lo == null) {
                    lo = new NumericScalarExpr(node, xq);
                } else if (hi == null) {
                    hi = new NumericScalarExpr(node, xq);
                } else {
                    throw new WCPSException(ExceptionCode.UnsupportedCombination,
                            "Unknown node in AxisIterator: " + nodeName);
                }
            }

            node = node.getNextSibling();
            while ((node != null) && node.getNodeName().equals("#text")) {
                node = node.getNextSibling();
            }
        }
    }

    public String toRasQL() {
        String result = varTranslation + " in [" + lo.toRasQL() + ":" + hi.toRasQL() + "]";
        return result;
    }

    /** Sets a new name for the iterator variable, to be used in the rasql query**/
    public void setVariableTranslation(String newName) {
        varTranslation = newName;
    }

    /** Return the Higher bound for the axis iterator.
     * This only works for constant expressions.
     * TODO: implement arbitrary expressions.
     * @return BigInteger
     */
    public BigInteger getHigh() {
        return SDU.str2integer(hi.toRasQL()).get(0);
    }

    /** Return the Lower bound for the axis iterator.
     * This only works for constant expressions.
     * TODO: implement arbitrary expressions.
     * @return BIgInteger
     */
    public BigInteger getLow() {
        return SDU.str2integer(lo.toRasQL()).get(0);
    }

    /* Return the variable name used in this axis */
    public String getVar() {
        return var;
    }

    public String getAxisType() {
        return axis.toRasQL();
    }

    /* Returns the m-interval that this axis iterates over in a string form */
    public String getInterval() {
        return lo.toRasQL() + ":" + hi.toRasQL();
    }
}
