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
package petascope.util;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
import hirondelle.date4j.DateTime;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 *
 * @author <a href="mailto:cmppri@unife.it">Piero Campalani</a>
 */
public class TimeUtil {
    
    private static final int DELTA_ANSI_MJD = 94188; // =(ANSI Day)-(Modified Julian Day)
    private static Logger log = LoggerFactory.getLogger(TimeUtil.class);

    /**
     * @param   String timestamp    Timestamp string requested by the client
     * @return  boolean             True if valid ISO timestamp (YYYY-MM-DD).
     */
    public static boolean isValidTimestamp (String timestamp) {
        Pattern p = Pattern.compile("^([0-9]{4}-[0-9]{2}-[0-9]{2})$");
        Matcher m = p.matcher(timestamp);
        while (m.find()) {
            return true;
        }
        return false;
    }
    
    /**
     * @param   String timestamp    YYYY-MM-DD timestamp
     * @return  int                 ANSI day number.
     */
    public static int convert2AnsiDay (String timestamp) throws Exception {        
        try {
            DateTime dt = new DateTime(timestamp);
            return dt.getModifiedJulianDayNumber() + DELTA_ANSI_MJD;
        } catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }
}
