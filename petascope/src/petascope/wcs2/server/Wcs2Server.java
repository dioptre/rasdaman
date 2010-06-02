/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */
package petascope.wcs2.server;

//~--- non-JDK imports --------------------------------------------------------
import javax.servlet.ServletException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import petascope.ConfigManager;

import petascope.wcps.server.core.DbMetadataSource;

import petascope.wcs.server.exceptions.WCSException;

import petascope.wcs2.server.ops.DescribeCoverage;
import petascope.wcs2.server.ops.GetCapabilities;
import petascope.wcs2.server.ops.GetCoverage;

//~--- JDK imports ------------------------------------------------------------
/**
 * The Web Coverage Service (WcsServer)
 * This class provides methods for each of the WCS operations:
 * - GetCapabilities
 * - DescribeCoverage
 * - GetCoverage
 *
 * @author Andrei Aiordachioaie
 */
public class Wcs2Server {

    private static Logger LOG = LoggerFactory.getLogger(Wcs2Server.class);

    /* The Operations to delegate to */
    private GetCapabilities capa;
    private DescribeCoverage desc;
    private GetCoverage getcov;

    /* Globals */
    private DbMetadataSource meta;

    /**
     * constructor
     * @param settingsPath path to the "settings.properties" file
     */
    public Wcs2Server(String settingsPath, DbMetadataSource source) throws ServletException {
        ConfigManager.getInstance(settingsPath, null);
        meta = source;

        try {
            capa = new GetCapabilities(meta);
            desc = new DescribeCoverage(meta);
            getcov = new GetCoverage(meta);
        } catch (Exception e) {
            LOG.error("Could not init WCS 2.0");

            throw new RuntimeException("Could not initialize WCS 2.0", e);
        }
    }

    /**
     * WcsServer GetCapabilities operation
     */
    public String GetCapabilities(String stringXml) throws WCSException {
        LOG.trace("Received input:\n{}", stringXml);
        LOG.info("Executing operation GetCapabilities...");
        String result = capa.execute(stringXml);

        LOG.info("GetCapabilities finished");

        return result;
    }

    /**
     * WcsServer DescribeCoverage operation
     */
    public String DescribeCoverage(String stringXml) throws WCSException {
        LOG.trace("Received input:\n{}", stringXml);
        LOG.info("Executing operation DescribeCoverage...");
        String result = desc.execute(stringXml);

        LOG.info("DescribeCoverage finished");

        return result;
    }

    /**
     * WcsServer GetCoverage operation
     */
    public String GetCoverage(String stringXml) throws WCSException {
        LOG.trace("Received input:\n{}", stringXml);
        LOG.info("Executing operation GetCoverage...");
        String result = getcov.execute(stringXml);

        LOG.info("GetCoverage finished");

        return result;
    }
}
