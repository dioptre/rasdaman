/* 
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 *
 * Original author    Normen Müller <n.mueller@jacobs-university.de>
 * Web                http://kwarc.info/nmueller/
 * Created            Oct 17, 2007
 * Filename           $Id: ListUtil.java 1976 2010-07-31 12:07:20Z dmisev $
 * Revision           $Revision: 1976 $
 *
 * Last modified on   $Date:2007-10-25 18:50:01 +0200 (Thu, 25 Oct 2007) $
 *               by   $Author:nmueller $
 * 
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU  Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */
package petascope.util;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import nu.xom.Node;

/**
 * List utilities.
 * 
 * @author Normen M&#xFC;ller&nbsp;&#60;n.mueller@jacobs-university.de&#62;
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class ListUtil {

    // TODO comment
    public static final <T> List<T> reverse(List<T> l) {
        List<T> nl = new LinkedList<T>(l);
        Collections.reverse(nl);
        return nl;
    }

    /**
     * @return sublist(l, n, m) --> [x_n,...,x_m] if 0 <= n < m < size(l) else empty_list
     */
    public static <T> List<T> sublist(List<T> l, int fromIndex, int toIndex) {
        if (fromIndex > toIndex) {
            return Collections.<T>emptyList();
        }
        return l.subList(fromIndex, toIndex + 1);
    }

    /**
     * Converts a collection to a string, separating the elements by ","
     * 
     * @param l The StringList
     * @param c The delimiter
     * @return A String of the ListElements separated by c.
     */
    public static <T> String ltos(Collection<T> l, String c) {
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
     * Converts a collection  to a string
     * 
     * @param l the element list
     * @return A String of the ListElements separated by new lines
     */
    public static <T> String ltos(Collection<T> l) {
        String s = "";
        for (Iterator<T> iter = l.iterator(); iter.hasNext();) {
            Object o = iter.next();
            String tmp = null;
            if (o instanceof Node) {
                tmp = ((Node) o).toXML();
            } else if (o instanceof Collection) {
                tmp = ltos((Collection) o);
            } else {
                tmp = o.toString();
            }
            if (!s.equals("")) {
                s += "\n";
                s += tmp;
            } else {
                s += tmp;
            }
        }
        return s;
    }

    /**
     * Converts a string to a list
     * 
     * @param s The StringList
     * @param c The delimiter
     * @return A String of the ListElements separated by c.
     */
    public static List<String> stol(String s, String c) {
        List<String> l = new LinkedList<String>();
        if (s == null) {
            return l;
        }
        s = s.replaceAll(" *, *", c);
        String[] sl = s.split(c, s.length());
        for (int i = 0; i < sl.length; i++) {
            l.add(sl[i]);
        }
        return l;
    }

    /* head(e_1,...,e_n) --> e_1 */
    public static <T> T head(List<T> l) {
        if (l == null || l.size() == 0) {
            return null;
        } else {
            return l.get(0);
        }
    }

    /* head(e_1,...,e_n) --> (e_2,...,e_n) */
    public static <T> List<T> tail(List<T> l) {
        if (l == null) {
            return null;
        }
        if (l.size() == 0 || l.size() == 1) {
            return Collections.<T>emptyList();
        }
        return ListUtil.sublist(l, 1, l.size() - 1);
    }

    /**
     * con (e, [x_1,...x_n]) --> [e,x_1,...x_n] 
     */
    public static <T> List<T> cons(T e, List<T> l) {
        return concat(Collections.singletonList(e), l);
    }

    /**
     * [x_1,...x_n]@[y_1,...y_m] --> [x_1,...x_n,y_1,...,y_m] 
     */
    public static <T> List<T> concat(List<T> l, List<T> r) {
        List<T> res = new ArrayList<T>();
        for (T a : l) {
            res.add(a);
        }
        for (T a : r) {
            res.add(a);
        }
        return res;
    }

    public static <T> List<T> toList(T... e) {
         List<T> ret = new ArrayList<T>();
         if (e != null)
             for (T o : e) {
                ret.add(o);
            }
         return ret;
    }

    /**
     * New iteration [x_1,...x_m] --> [x_1,...x_m, n]
     */
    public static final List<Integer> add(List<Integer> l, int n) {
        List<Integer> nl = new LinkedList<Integer>(l);
        nl.add(n);
        return nl;
    }

    /**
     * Goto next iteration [x_1,...x_m] --> [x_1,...x_m + 1]
     */
    public static final List<Integer> inc(List<Integer> l) {
        List<Integer> nl = new LinkedList<Integer>(l);
        if (!nl.isEmpty()) {
            nl.add(nl.remove(nl.size() - 1).intValue() + 1);
        }
        return nl;
    }

    /**
     * @param <T> return type
     * @param <E> input type
     * @param l input list
     */
    public static <T, E> List<T> cast(List<E> l) {
        List<T> ret = new ArrayList<T>();
        for (Object object : l) {
            ret.add((T) object);
        }
        return ret;
    }

    public static <T> T[] append(T[] a, T... b) {
        T[] ret = (T[]) new Object[a.length + b.length];
        for (int i = 0; i < a.length; i++) {
            ret[i] = a[i];
        }
        for (int i = 0; i < b.length; i++) {
            ret[i + a.length] = b[i];
        }
        return ret;
    }
}
