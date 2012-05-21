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
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.util;

/**
 * A simple class to represent a pair.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class Pair<A, B> {

    /**
     * The fst component.
     */
    public final A fst;
    /**
     * The snd component.
     */
    public final B snd;

    /**
     * Creates a new pair.
     *
     * @param <A> type variable for the type of the fst component
     * @param <B> type variable for the type of the snd component
     * @param a a fst component
     * @param b a snd component
     * @return a new pair consisting of <code>a</code> and <code>b</code>
     */
    public static <A, B> Pair<A, B> of(A a, B b) {
        return new Pair<A, B>(a, b);
    }

    public Pair(A fst, B snd) {
        this.fst = fst;
        this.snd = snd;
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#equals(java.lang.Object)
     */
    @Override
    public int hashCode() {
        if (fst == null) {
            return (snd == null) ? 0 : snd.hashCode() + 1;
        } else if (snd == null) {
            return fst.hashCode() + 2;
        } else {
            return fst.hashCode() * 17 + snd.hashCode();
        }
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#equals(java.lang.Object)
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean equals(Object other) {
        return other instanceof Pair && equals(fst, ((Pair) other).fst) && equals(snd, ((Pair) other).snd);
    }

    private static boolean equals(Object x, Object y) {
        return (x == null && y == null) || (x != null && x.equals(y));
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#equals(java.lang.Object)
     */
    public String toString() {
        return "(" + fst + "," + snd + ")";
    }
}
