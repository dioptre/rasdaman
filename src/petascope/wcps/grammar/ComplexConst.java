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
package petascope.wcps.grammar;

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
        int pos = val.indexOf("+i", 0);

        if (pos != -1) {
            re = val.substring(0, pos - 1);
            im = val.substring(pos + 2, val.length());
        }
    }

    public String toXML() {
        return "<complexConstant><re>" + re + "</re><im>" + im + "</im></complexConstant";
    }
}
