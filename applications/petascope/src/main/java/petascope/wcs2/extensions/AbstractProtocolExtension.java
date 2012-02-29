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
package petascope.wcs2.extensions;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.core.DbMetadataSource;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import petascope.wcs2.handlers.HandlersRegistry;
import petascope.wcs2.parsers.Request;
import petascope.wcs2.handlers.RequestHandler;
import petascope.wcs2.handlers.Response;
import petascope.wcs2.parsers.ParsersRegistry;
import petascope.wcs2.parsers.RequestParser;

/**
 * An abstract implementation of {@link ProtocolExtension}, which provides some
 * convenience methods to concrete implementations.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public abstract class AbstractProtocolExtension implements  ProtocolExtension {
    
    private static final Logger log = LoggerFactory.getLogger(AbstractProtocolExtension.class);

    @Override
    public Response handle(String request, DbMetadataSource meta) throws WCSException {
        RequestParser parser = ParsersRegistry.getParser(request);
        if (parser == null) {
            throw new WCSException(ExceptionCode.NoApplicableCode, "No suitable parser found.");
        } else {
            log.info("Found parser {}", parser.getClass().getSimpleName());
        }
        Request req = parser.parse(request);
            if (req == null) {
            throw new WCSException(ExceptionCode.InternalComponentError, "Error parsing the request.");
        } else {
            log.info("Request successfully parsed.");
        }
        RequestHandler handler = HandlersRegistry.getHandler(req, meta);
        if (handler == null) {
            throw new WCSException(ExceptionCode.NoApplicableCode, "No suitable handler found");
        } else {
            log.info("Found handler {}", handler.getClass().getSimpleName());
        }
        return handler.handle(req);
    }

    @Override
    public boolean equals(Object o) {
        return o instanceof ProtocolExtension && 
                ((ProtocolExtension)o).getExtensionIdentifier().equals(this.getExtensionIdentifier());
    }

    @Override
    public int hashCode() {
        return (getExtensionIdentifier()).hashCode();
    }

    @Override
    public String toString() {
        return getExtensionIdentifier();
    }
}
