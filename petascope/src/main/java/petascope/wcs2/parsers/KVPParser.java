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
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcs2.parsers;

import java.util.List;
import java.util.Map;
import java.util.Set;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import petascope.util.ListUtil;
import petascope.wcs2.handlers.RequestHandler;

/**
 * An abstract superclass for XML/POST protocol binding extensions, which provides some
 * convenience methods to concrete implementations.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public abstract class KVPParser<T extends Request> extends AbstractRequestParser<T> {

    @Override
    public boolean canParse(String input) {
        return input != null && !input.startsWith("<") && input.contains(getOperationName());
    }

    protected String get(String key, Map<String, List<String>> m) {
        if (m.containsKey(key)) {
            return m.get(key).get(0);
        } else {
            return null;
        }
    }

    /**
     * Checks for requirement 9 in OGC 09-147r1
     *
     * @param m
     * @param keys KVP keys that the operation supports
     * @throws WCSException thrown when the request doesn't comply with the
     * KVP syntax
     */
    protected void checkEncodingSyntax(Map<String, List<String>> m, String... keys) throws WCSException {
        List<String> possibleKeys = ListUtil.toList(keys);
        Set<String> requestKeys = m.keySet();
        
        String version = get("version", m);
        if (version == null || !version.matches(BaseRequest.VERSION)) {
            throw new WCSException(ExceptionCode.InvalidEncodingSyntax.locator("version"));
        }
        for (String k : requestKeys) {
            if (k.equals("request") || k.equals("service") || k.equals("version")) {
                if (m.get(k).size() > 1) {
                    throw new WCSException(ExceptionCode.InvalidEncodingSyntax.locator(k));
                }
            }
            if (k.equals("request") || k.equals("service") || (k.startsWith("subset")
                    && getOperationName().equals(RequestHandler.GET_COVERAGE))) {
                continue;
            }
            if (!possibleKeys.contains(k)) {
                throw new WCSException(ExceptionCode.InvalidEncodingSyntax.locator(k));
            }
        }

        checkValue(m, "request", getOperationName());
        checkValue(m, "service", BaseRequest.SERVICE);
    }

    private void checkValue(Map<String, List<String>> m, String k, String... vals) throws WCSException {
        String v = get(k, m);
        if (v == null || (!ListUtil.toList(vals).contains(v))) {
            throw new WCSException(ExceptionCode.InvalidEncodingSyntax.locator(k));
        }
    }
}
