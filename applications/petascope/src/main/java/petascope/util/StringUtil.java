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
 * GNU  General Public License for more details.
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

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;

/**
 * String utilities.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class StringUtil {

    /**
     * Remove leading/trailing spaces and any newlines.
     *
     * @param data String to be trimmed
     * @return the trimmed result
     */
    public static String trim(String data) {
        if (data == null) {
            return null;
        }

        String ret = data.trim();
        ret = ret.replaceAll("\n", "");
        return ret;
    }

    /**
     * Wrap a long string in multiple lines
     *
     * @param s the string
     * @param lineLength the preferred line length
     * @return the wrapped string
     */
    public static String wrap(String s, int lineLength) {
        return wrap(s, ' ', lineLength, 10, 10);
    }

    /**
     * Wrap a long string in multiple lines
     *
     * @param s the string
     * @param c the preferred char on which to split the string (usually ' ')
     * @param lineLength the preferred line length
     * @param leps tolerance to the left of <code>lineLength</code>
     * @param reps tolerance to the right of <code>lineLength</code>
     * @return the wrapped string
     */
    public static String wrap(String s, char c, int lineLength, int leps, int reps) {
        if (s.length() < lineLength) {
            return s;
        }
        int eps = Math.max(leps, reps);
        for (int i = 0; i < eps; i++) {
            int l = lineLength - i;
            int r = lineLength + i;
            if (i < leps && l >= 0 && l < s.length() && s.charAt(l) == c) {
                return s.substring(0, l) + "\n" + wrap(s.substring(l), c, lineLength, leps, reps);
            }
            if (i < reps && r >= 0 && r < s.length() && s.charAt(r) == c) {
                return s.substring(0, r) + "\n" + wrap(s.substring(l), c, lineLength, leps, reps);
            }
        }
        return s;
    }

    public static String quote(String s) {
        return "\"" + s + "\"";
    }

    public static String unquote(String s) {
        if (s == null || s.length() < 2) {
            return s;
        }
        if ((s.charAt(0) == '\"' && s.charAt(s.length() - 1) == '\"')
                || (s.charAt(0) == '\'' && s.charAt(s.length() - 1) == '\'')) {
            return s.substring(1, s.length() - 1);
        }
        return s;
    }

    public static String combine(String sep, String... s) {
        String ret = "";
        if (s.length > 0) {
            ret = s[0];
        }
        for (int i = 1; i < s.length; i++) {
            ret += sep + s[i];
        }
        return ret;
    }

    public static String join(String... s) {
        return combine("\n", s);
    }

    /**
     * Converts a collection to a string, separating the elements by ","
     *
     * @param l The StringList
     * @param c The delimiter
     * @return A String of the ListElements separated by c.
     */
    public static <T> String ltos(Collection<T> l, Character c) {
        String s = "";
        for (Iterator<T> iter = l.iterator(); iter.hasNext();) {
            if (!s.equalsIgnoreCase("")) {
                s = s + c + iter.next().toString();
            } else {
                s = s + iter.next().toString();
            }
        }
        return s;
    }

    /**
     * Converts a string to a list
     *
     * @param s The StringList
     * @return A String of the ListElements separated by c.
     */
    public static List<String> stol(String s) {
        List<String> l = new LinkedList<String>();
        if (s == null) {
            return l;
        }
        String[] sl = s.split(",");
        for (int i = 0; i < sl.length; i++) {
            l.add(sl[i]);
        }
        return l;
    }

    public static <T> T[] array(T... elements) {
        return elements;
    }

    public static String d2s(Double d) {
        Double tmp = (double) d.intValue();
        if (Math.abs(d - tmp) < 0.0000001) {
            return d.intValue() + "";
        } else {
            return d.toString();
        }
    }

    /** URL-decode a string, if needed */
    public static String urldecode(String encodedText, String contentType) {
        if (encodedText == null) {
            return null;
        }
        String decoded = encodedText;
        if (contentType != null && contentType.equals("application/x-www-form-urlencoded") && encodedText.
                indexOf(" ") == -1) {
            try {
                decoded = URLDecoder.decode(encodedText, "UTF-8");
            } catch (UnsupportedEncodingException ex) {
                decoded = URLDecoder.decode(encodedText);
            }
        }
        return decoded;
    }

    public static String urlencode(String text) {
        try {
            return URLEncoder.encode(text, "UTF-8");
        } catch (UnsupportedEncodingException ex) {
            return URLEncoder.encode(text);
        }
    }

    /**
     * <b>Taken from HttpUtils.java and modified</b>
     *
     * Parses a query string passed from the client to the
     * server and builds a <code>HashTable</code> object
     * with key-value pairs.
     * The query string should be in the form of a string
     * packaged by the GET or POST method, that is, it
     * should have key-value pairs in the form <i>key=value</i>,
     * with each pair separated from the next by a &amp; character.
     *
     * <p>A key can appear more than once in the query string
     * with different values. However, the key appears only once in
     * the hashtable, with its value being
     * an array of strings containing the multiple values sent
     * by the query string.
     *
     * <p>The keys and values in the hashtable are stored in their
     * decoded form, so
     * any + characters are converted to spaces, and characters
     * sent in hexadecimal notation (like <i>%xx</i>) are
     * converted to ASCII characters.
     *
     * @param s		a string containing the query to be parsed
     *
     * @return		a <code>HashTable</code> object built
     * 			from the parsed key-value pairs
     *
     * @exception IllegalArgumentException	if the query string
     *						is invalid
     *
     */
    public static Map<String, List<String>> parseQuery(String s) {
        if (s == null) {
            throw new IllegalArgumentException();
        }
        Map<String, List<String>> ret = new LinkedHashMap<String, List<String>>();
        StringBuffer sb = new StringBuffer();
        StringTokenizer st = new StringTokenizer(s, "&");
        while (st.hasMoreTokens()) {
            String pair = (String) st.nextToken();
            int pos = pair.indexOf('=');
            if (pos == -1) {
                // XXX
                // should give more detail about the illegal argument
                throw new IllegalArgumentException();
            }
            String key = parseName(pair.substring(0, pos), sb).toLowerCase();
            List<String> val = stol(parseName(pair.substring(pos + 1, pair.
                    length()), sb));
            if (ret.containsKey(key)) {
                ret.get(key).addAll(val);
            } else {
                ret.put(key, val);
            }
        }
        return ret;
    }

    /*
     * Parse a name in the query string.
     */
    private static String parseName(String s, StringBuffer sb) {
        sb.setLength(0);
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            switch (c) {
                case '+':
                    sb.append(' ');
                    break;
                case '%':
                    try {
                        sb.append((char) Integer.parseInt(s.substring(i + 1, i + 3),
                                16));
                        i += 2;
                    } catch (NumberFormatException e) {
                        // XXX
                        // need to be more specific about illegal arg
                        throw new IllegalArgumentException();
                    } catch (StringIndexOutOfBoundsException e) {
                        String rest = s.substring(i);
                        sb.append(rest);
                        if (rest.length() == 2) {
                            i++;
                        }
                    }

                    break;
                default:
                    sb.append(c);
                    break;
            }
        }
        return sb.toString();
    }
}
