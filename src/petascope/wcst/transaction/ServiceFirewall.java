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

package petascope.wcst.transaction;

/** Firewall class for WCS-T. It's job is to detect malitious queries and reject them.
 * Currently, it only checks that queries do not occur too often. 
 * 
 * @author Andrei Aiordachioaie
 */
public class ServiceFirewall
{
    private static long lastQueryTime = 0;
    /* Queries should not occur more often than this period (in seconds) */
    private static final double MIN_QUERY_PERIOD = 10;

    /** Do we reject a certain query through the firewall?
     *
     * @param query Query string
     * @return boolean 
     */
    public static boolean reject(String query)
    {
        boolean result = true;
        
        /* Check query period */
        long currentTime = System.currentTimeMillis();
        if (((currentTime - lastQueryTime) / 1000) >= MIN_QUERY_PERIOD)
            result = false;
        lastQueryTime = currentTime;
        
        return result;
    }
}