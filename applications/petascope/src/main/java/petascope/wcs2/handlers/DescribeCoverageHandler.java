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

import petascope.wcs2.parsers.GetCoverageMetadata;
import petascope.wcs2.parsers.DescribeCoverageRequest;
import nu.xom.Document;
import petascope.core.Metadata;
import petascope.util.Pair;
import petascope.util.XMLUtil;
import petascope.wcs2.templates.Templates;
import java.io.IOException;
import petascope.exceptions.ExceptionCode;
import nu.xom.Element;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.core.DbMetadataSource;
import petascope.exceptions.WCSException;
import petascope.util.WcsUtil;
import petascope.wcs2.parsers.GetCoverageRequest;
import static petascope.util.XMLSymbols.*;
import static petascope.util.XMLUtil.*;

/**
 * GetCapabilities operation for The Web Coverage Service 2.0
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class DescribeCoverageHandler extends AbstractRequestHandler<DescribeCoverageRequest> {

    private static Logger log = LoggerFactory.getLogger(DescribeCoverageHandler.class);

    public DescribeCoverageHandler(DbMetadataSource meta) {
        super(meta);
    }

    @Override
    public Response handle(DescribeCoverageRequest request) throws WCSException {
        
        Document ret = constructDocument(LABEL_COVERAGE_DESCRIPTIONS, NAMESPACE_WCS);
        Element root = ret.getRootElement();

        WCSException exc = null;
        ExceptionCode code;
        for (String coverageId : request.getCoverageIds()) {
            String descr = null;
            try {
                GetCoverageRequest tmp = new GetCoverageRequest(coverageId);
                GetCoverageMetadata m = new GetCoverageMetadata(tmp, meta);
                descr = WcsUtil.getGML(m, Templates.COVERAGE_DESCRIPTION, true);
            } catch (WCSException ex) {
                if (ex.getExceptionCode().getExceptionCode().equals(ExceptionCode.NoSuchCoverage.getExceptionCode())) {
                    if (exc == null) {
                        exc = ex;
                    } else {
                        code = exc.getExceptionCode();
                        code.setLocator(code.getLocator() + " " + ex.getExceptionCode().getLocator());
                    }
                }
            }
            if (exc != null) {
                continue;
            }
            try {
                root.appendChild(XMLUtil.buildDocument(null, descr).getRootElement().copy());
            } catch (Exception ex) {
                log.warn("Error parsing template file:\n\n" + descr, ex);
            }
        }
        if (exc != null) {
            throw exc;
        }

        try {
            return new Response(serialize(ret));
        } catch (IOException ex) {
            throw new WCSException(ExceptionCode.IOConnectionError,
                    "Error serializing constructed document", ex);
        }
    }
}
