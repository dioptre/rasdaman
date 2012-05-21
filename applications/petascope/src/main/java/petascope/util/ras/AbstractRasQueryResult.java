/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */

package petascope.util.ras;

import org.odmg.DBag;
import rasj.RasGMArray;

/**
 * Provides a convenient implementation of {@link #inspect()}.
 *
 * @author Dimitar Misev
 */
public abstract class AbstractRasQueryResult implements IRasQueryResult {
    
    protected final Object result;
    /**
     * Prevent multiple inspections
     */
    private boolean inspected;

    public AbstractRasQueryResult(Object result) {
        this.result = result;
        this.inspected = false;
    }

    public Object getResult() {
        return result;
    }

    public void inspect() {
        if (inspected) {
            return;
        }
        if (result instanceof DBag) {
            DBag bag = (DBag) result;
            for (Object e : bag) {
                if (e instanceof RasGMArray) {
                    mdd((RasGMArray) e);
                } else {
                    scalar(e);
                }
            }
        } else if (result instanceof Integer) {
            scalar(result);
        }
        inspected = true;
    }
}
