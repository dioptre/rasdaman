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

/**
 * GeneralCondenseExpr
 *
 * @author Andrei Aiordachioaie
 */
public class GeneralCondenseExpr implements IParseTreeNode {

    AxisIteratorList alist;
    CondenseOperation op;
    CoverageExpr using;
    BooleanScalarExpr where;

    public GeneralCondenseExpr(CondenseOperation op, AxisIteratorList al) {
        this.op = op;
        alist = al;
        alist.setTag("iterator");
        where = null;
        using = null;
    }

    public void setWhere(BooleanScalarExpr bse) {
        where = bse;
    }

    public void setUsing(CoverageExpr se) {
        using = se;
    }

    public String toXML() {
        String result = "<condense>";

        result += op.toXML();
        result += alist.toXML();

        if (where != null) {
            result += "<where>" + where.toXML() + "</where>";
        }

        result += using.toXML();
        result += "</condense>";

        return result;
    }
}
