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
package petascope.wcs2.extensions;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.core.DbMetadataSource;
import petascope.wcs2.handlers.Response;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.PetascopeException;
import petascope.exceptions.WCSException;
import petascope.util.Pair;
import petascope.util.WcsUtil;
import petascope.util.XMLUtil;
import petascope.wcs2.templates.Templates;

/**
 * An abstract superclass for SOAP protocol binding extensions, which provides some
 * convenience methods to concrete implementations.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class SOAPProtocolExtension extends AbstractProtocolExtension {
    
    private static final Logger log = LoggerFactory.getLogger(SOAPProtocolExtension.class);
    
    private static final Pattern REGEX = Pattern.compile(".+<([^:]+:)?Body>(.+)</([^:]+:)?Body>.+", Pattern.DOTALL);
    
    @Override
    public boolean canHandle(String input) {
        return input != null && input.startsWith("<?xml") && XMLUtil.isFirstTag(input, "Envelope");
    }

    @Override
    public Response handle(String request, DbMetadataSource meta) throws WCSException {
        try {
            Matcher matcher = REGEX.matcher(request);
            if (matcher.matches()) {
                request = matcher.group(2);
                if (request == null) {
                    throw new WCSException(ExceptionCode.InvalidRequest, "No WCS 2.0 operation"
                            + " found in the body of the SOAP request message.");
                }
                request = request.trim();
            } else {
                throw new WCSException(ExceptionCode.InvalidRequest, "No WCS 2.0 operation"
                        + " found in the body of the SOAP request message.");
            }
            Response ret = super.handle(request, meta);
            if (ret.getXml() != null) {
                ret = new Response(ret.getData(), Templates.getTemplate(Templates.SOAP_MESSAGE,
                        Pair.of("\\{body\\}", XMLUtil.removeXmlDecl(ret.getXml()))), ret.getMimeType());
            }
            return ret;
        } catch (Exception ex) {
            log.error("Error", ex);
            return new Response(Templates.getTemplate(Templates.SOAP_FAULT,
                    Pair.of("\\{exceptionReport\\}", XMLUtil.removeXmlDecl(WcsUtil.exceptionToXml((PetascopeException) ex)))));
        }
    }

    @Override
    public String getExtensionIdentifier() {
        return ExtensionsRegistry.SOAP_IDENTIFIER;
    }
}
