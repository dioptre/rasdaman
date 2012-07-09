/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */

import org.apache.commons.io.IOUtils;
import petascope.ConfigManager;
import petascope.core.DbMetadataSource;
import petascope.util.ras.RasUtil;
import petascope.wcps.server.core.ProcessCoveragesRequest;
import petascope.wcps.server.core.Wcps;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Dimitar Misev
 */
public class Test {

    public static void main(String[] args) {
        try {
            System.out.println("WCPS: initializing metadata database");
            DbMetadataSource meta =
                    new DbMetadataSource(ConfigManager.METADATA_DRIVER,
                    ConfigManager.METADATA_URL,
                    ConfigManager.METADATA_USER,
                    ConfigManager.METADATA_PASS, false);
            System.out.println("WCPS: initializing WCPS core");
            Wcps wcps = new Wcps(meta);
            
            String query = "";
            String xmlRequest = RasUtil.abstractWCPStoXML(query);
            ProcessCoveragesRequest pc = wcps.pcPrepare(
                    ConfigManager.RASDAMAN_URL, ConfigManager.RASDAMAN_DATABASE,
                    IOUtils.toInputStream(xmlRequest));
            String rasql = pc.getRasqlQuery();
            System.out.println("DONE");
            System.out.println("rasql: " + rasql);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
