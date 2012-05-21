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
package petascope.util;

/**
 * A simple class to represent a triple.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class Triple<A, B, C> {

    /**
     * The first component.
     */
    public final A fst;
    /**
     * The second component.
     */
    public final B snd;
    /**
     * The third component.
     */
    public final C trd;

    /**
     * Creates a new pair.
     *
     * @param <A> type variable for the type of the first component
     * @param <B> type variable for the type of the second component
     * @param a a first component
     * @param b a second component
     * @return a new pair consisting of <code>a</code> and <code>b</code>
     */
    public static <A, B, C> Triple<A, B, C> of(A a, B b, C c) {
        return new Triple<A, B, C>(a, b, c);
    }

    public Triple(A fst, B snd, C trd) {
        this.fst = fst;
        this.snd = snd;
        this.trd = trd;
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#hashCode()
     */
    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((fst == null) ? 0 : fst.hashCode());
        result = prime * result + ((snd == null) ? 0 : snd.hashCode());
        result = prime * result + ((trd == null) ? 0 : trd.hashCode());
        return result;
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#equals(java.lang.Object)
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean equals(Object other) {
        return other instanceof Triple && equals(fst, ((Triple) other).fst) && equals(snd, ((Triple) other).snd) && equals(trd, ((Triple) other).trd);
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
        return "(" + fst + "," + snd + "," + trd + ")";
    }
}
