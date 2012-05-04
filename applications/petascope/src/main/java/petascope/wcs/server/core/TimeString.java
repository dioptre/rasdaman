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
package petascope.wcs.server.core;

import org.joda.time.DateTime;
import org.joda.time.format.DateTimeFormat;
import org.joda.time.format.DateTimeFormatter;
import org.joda.time.format.ISODateTimeFormat;

/**
 * Represents a Time object, according to ISO 8601, and utility methods.
 *
 * @author Gulius Caesar
 */
public class TimeString {

    private static DateTimeFormatter DTF = ISODateTimeFormat.dateTime();
    private DateTime dateTime;
    private long milliseconds;
    private String strTime;

    /* Default constructor */
    public TimeString(String timeStr) throws IllegalArgumentException {
        strTime = timeStr;
        dateTime = parse(strTime);
        milliseconds = parseMillis(strTime);
    }

    public DateTime getDateTime() {
        return dateTime;
    }

    public long getMilliseconds() {
        return milliseconds;
    }

    public long subtract(String newString) throws IllegalArgumentException {
        long newD = parseMillis(newString);
        long res = milliseconds - newD;

        return res;
    }

    public double divide(String newString) throws IllegalArgumentException {
        long newD = parseMillis(newString);
        double result = milliseconds * new Double(1.0) / newD;

        return result;
    }

    /** ******************** Static methods ***************** */

    /*
     * Subtraction Method, calculates the difference in milliseconds between two dates
     */
    public static long difference(String bigDate, String smallDate) throws IllegalArgumentException {
        long smallD = parseMillis(smallDate);
        long bigD = parseMillis(bigDate);
        long result = bigD - smallD;

        return result;
    }

    /**
     * Division of one Date by the other.
     */
    public static double division(String IAmDivided, String IDivide) throws IllegalArgumentException {
        long iad = parseMillis(IAmDivided);
        long id = parseMillis(IDivide);
        double result = iad * new Double(1.0) / id;

        return result;
    }

    /*
     * Division of a millisecond-Date-respresentation by a number.
     */
    public static double division(String InputDate, long Divisor) throws IllegalArgumentException {
        long ID = parseMillis(InputDate);

        double result = ID * new Double(1.0) / Divisor;

        return result;

    }

    /**
     * Date to millisecond parser. Accepts the standard ISO 8601 + shorter codes.
     * @return long number of miliseconds
     */
    public static long parseMillis(String TString) throws IllegalArgumentException {
        long Millis;
        int NumberOfSymbols = TString.length();

        switch (NumberOfSymbols) {
            case 24:
                DTF = ISODateTimeFormat.dateTime();
                Millis = DTF.parseMillis(TString);

                break;
            case 22:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd'T'HH:mm:ss.SS");
                Millis = DTF.parseMillis(TString);

                break;
            case 19:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd'T'HH:mm:ss");
                Millis = DTF.parseMillis(TString);

                break;
            case 16:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd'T'HH:mm");
                Millis = DTF.parseMillis(TString);

                break;
            case 13:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd'T'HH");
                Millis = DTF.parseMillis(TString);

                break;
            case 10:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd");
                Millis = DTF.parseMillis(TString);

                break;
            case 7:
                DTF = DateTimeFormat.forPattern("yyyy-MM");
                Millis = DTF.parseMillis(TString);

                break;
            case 4:
                DTF = DateTimeFormat.forPattern("yyyy");
                Millis = DTF.parseMillis(TString);

                break;
            default:
                throw new IllegalArgumentException("Unknown DateTime format.");
        }

        return Millis;
    }

    /**
     *
     * @param TString string containing date
     * @return joda DateTime corresponding to the input string
     * @throws IllegalArgumentException
     */
    public static DateTime parse(String str) throws IllegalArgumentException {
        DateTime date;
        int NumberOfSymbols = str.length();

        System.out.println("Parsing date '" + str + "', with length " + str.length());

        switch (NumberOfSymbols) {
            case 24:
                DTF = ISODateTimeFormat.dateTime();
                date = DTF.parseDateTime(str);

                break;
            case 22:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd'T'HH:mm:ss.SS");
                date = DTF.parseDateTime(str);

                break;
            case 19:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd'T'HH:mm:ss");
                date = DTF.parseDateTime(str);

                break;
            case 16:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd'T'HH:mm");
                date = DTF.parseDateTime(str);

                break;
            case 13:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd'T'HH");
                date = DTF.parseDateTime(str);

                break;
            case 10:
                DTF = DateTimeFormat.forPattern("yyyy-MM-dd");
                date = DTF.parseDateTime(str);

                break;
            case 7:
                DTF = DateTimeFormat.forPattern("yyyy-MM");
                date = DTF.parseDateTime(str);

                break;
            case 4:
                DTF = DateTimeFormat.forPattern("yyyy");
                date = DTF.parseDateTime(str);

                break;
            default:
                throw new IllegalArgumentException("Unknown DateTime format.");
        }

        return date;
    }
}
