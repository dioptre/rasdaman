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
package petascope.wcs2.handlers;

import petascope.wcs2.parsers.BaseRequest;
import petascope.wcs2.parsers.GetCapabilitiesRequest;
import nu.xom.Attribute;
import petascope.util.Pair;
import petascope.wcs2.templates.Templates;
import java.io.IOException;
import java.util.Iterator;
import petascope.exceptions.PetascopeException;
import nu.xom.Document;
import nu.xom.Element;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.ConfigManager;
import petascope.core.DbMetadataSource;
import petascope.exceptions.ExceptionCode;
import petascope.exceptions.WCSException;
import petascope.util.XMLUtil;
import petascope.wcs2.Wcs2Servlet;
import petascope.wcs2.extensions.ExtensionsRegistry;
import static petascope.util.XMLSymbols.*;

/**
 * GetCapabilities operation for The Web Coverage Service 2.0
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class GetCapabilitiesHandler extends AbstractRequestHandler<GetCapabilitiesRequest> {

    private static Logger log = LoggerFactory.getLogger(GetCapabilitiesHandler.class);

    public GetCapabilitiesHandler(DbMetadataSource meta) {
        super(meta);
    }

    @Override
    public Response handle(GetCapabilitiesRequest request) throws WCSException {
        Document ret = constructDocument(LABEL_CAPABILITIES, NAMESPACE_WCS);

        Element root = ret.getRootElement();
        root.addAttribute(new Attribute(ATT_VERSION, BaseRequest.VERSION_STRING));

        // Service Identification
        Element serviceIdentification = Templates.getXmlTemplate(Templates.SERVICE_IDENTIFICATION,
                Pair.of("\\{URL\\}", ConfigManager.PETASCOPE_SERVLET_URL != null
                ? ConfigManager.PETASCOPE_SERVLET_URL
                : Wcs2Servlet.LOCAL_SERVLET_ADDRESS));
        if (serviceIdentification != null) {
            for (String id : ExtensionsRegistry.getExtensionIds()) {
                Element profile = new Element(PREFIX_OWS + ":" + LABEL_PROFILE, NAMESPACE_OWS);
                profile.appendChild(id);
                serviceIdentification.appendChild(profile);
            }
            root.appendChild(serviceIdentification.copy());
        }

        // Service Provider
        Element serviceProvider = Templates.getXmlTemplate(Templates.SERVICE_PROVIDER,
                Pair.of("\\{URL\\}", ConfigManager.PETASCOPE_SERVLET_URL != null
                ? ConfigManager.PETASCOPE_SERVLET_URL
                : Wcs2Servlet.LOCAL_SERVLET_ADDRESS));
        if (serviceProvider != null) {
            root.appendChild(serviceProvider.copy());
        }

        // Operations Metadata
        Element operationsMetadata = Templates.getXmlTemplate(Templates.OPERATIONS_METADATA,
                Pair.of("\\{URL\\}", ConfigManager.PETASCOPE_SERVLET_URL != null
                ? ConfigManager.PETASCOPE_SERVLET_URL
                : Wcs2Servlet.LOCAL_SERVLET_ADDRESS));
        if (operationsMetadata != null) {
            root.appendChild(operationsMetadata.copy());
        }

        // ServiceMetadata
        Element serviceMetadata = Templates.getXmlTemplate(Templates.SERVICE_METADATA);
        if (serviceMetadata != null) {
            root.appendChild(serviceMetadata.copy());
        }

        // Contents
        Element contents = new Element(LABEL_CONTENTS, NAMESPACE_WCS);
        Iterator<String> it;
        try {
            it = meta.coverages().iterator();
            while (it.hasNext()) {
                Element cs = new Element(LABEL_COVERAGE_SUMMARY, NAMESPACE_WCS);
                Element c = null;
                c = new Element(LABEL_COVERAGE_ID, NAMESPACE_WCS);
                String coverageId = it.next();
                c.appendChild(coverageId);
                cs.appendChild(c);
                c = new Element(LABEL_COVERAGE_SUBTYPE, NAMESPACE_WCS);
                c.appendChild(meta.coverageType(coverageId));
                cs.appendChild(c);
                contents.appendChild(cs);
            }
        } catch (PetascopeException ex) {
            log.error("Error", ex);
            throw new WCSException(ex.getExceptionCode(), ex.getExceptionText());
        }
        root.appendChild(contents);

        try {
            return new Response(XMLUtil.serialize(ret));
        } catch (IOException ex) {
            throw new WCSException(ExceptionCode.IOConnectionError,
                    "Error serializing constructed document", ex);
        }
    }
}
