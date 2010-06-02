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
package petascope.wcs.server;

//~--- non-JDK imports --------------------------------------------------------
import javax.servlet.ServletException;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import net.opengis.wcs.v_1_1_0.Capabilities;
import net.opengis.wcs.v_1_1_0.CoverageDescriptions;
import net.opengis.wcs.v_1_1_0.DescribeCoverage;
import net.opengis.wcs.v_1_1_0.GetCapabilities;
import net.opengis.wcs.v_1_1_0.GetCoverage;

import petascope.wcps.server.core.DbMetadataSource;
import petascope.wcps.server.core.WCPS;
import petascope.wcps.server.exceptions.WCPSException;
import petascope.wcs.server.exceptions.WCSException;
import petascope.wcs.server.core.convertGetCoverage;
import petascope.wcs.server.core.executeDescribeCoverage;
import petascope.wcs.server.core.executeGetCapabilities;

import petascope.ConfigManager;
import petascope.PetascopeXmlNamespaceMapper;

//~--- JDK imports ------------------------------------------------------------

import java.io.StringReader;
import java.io.StringWriter;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.stream.XMLOutputFactory;
import javax.xml.stream.XMLStreamWriter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.wcs.server.exceptions.InternalComponentException;
import petascope.wcs.server.exceptions.XmlStructuresException;

/**
 * The Web Coverage Service (WcsServer)
 * This class provides methods for each of the WCS operations:
 * - GetCapabilities
 * - DescribeCoverage
 * - GetCoverage
 *
 * @author Andrei Aiordachioaie
 */
public class WcsServer {

    private static Logger LOG = LoggerFactory.getLogger(WcsServer.class);

    /* Status variable */
    private static Boolean ok;
    private static Boolean printLog;
    /* Globals */
    private static Boolean printOutput;
    private DbMetadataSource meta;

    /**
     * Public minimal constructor
     * @param settingsPath path to the "settings.properties" file
     */
    public WcsServer(String settingsPath, DbMetadataSource source) throws ServletException {
        ConfigManager.getInstance(settingsPath, null);
        meta = source;
        ok = true;
    }

    /**
     * WcsServer GetCapabilities operation
     */
    public String GetCapabilities(String stringXml) throws WCSException {
        String output = "Default output. ";

        try {
            // read the input XML
            LOG.trace("Reading the input XML file ... ");
            JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
            Unmarshaller unmarshaller = context.createUnmarshaller();
            Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

            // Find out which class to execute ...
            LOG.trace("It is a " + xml.getClass().getSimpleName() + " request.");

            LOG.trace("Compiling data into XML format for GetCapabilities ... ");
            GetCapabilities input = (GetCapabilities) xml;
            Capabilities cap = new executeGetCapabilities(input, meta).get();


            // Write the output
            LOG.trace("MArshalling with context: " + cap.getClass().getPackage().getName());
            final StringWriter writer = new StringWriter();
            try {
                context = JAXBContext.newInstance(cap.getClass());
                final XMLStreamWriter xmlStreamWriter = XMLOutputFactory.newInstance().createXMLStreamWriter(writer);

                final Marshaller marshaller = context.createMarshaller();
                marshaller.setProperty("com.sun.xml.bind.namespacePrefixMapper", new PetascopeXmlNamespaceMapper());
                marshaller.setProperty("jaxb.formatted.output", true);
                marshaller.setProperty("jaxb.schemaLocation", "http://www.opengis.net/wcs/1.1 http://schemas.opengis.net/wcs/1.1.0/wcsGetCapabilities.xsd "
                        + "http://www.opengis.net/wcs/1.1/ows http://schemas.opengis.net/wcs/1.1.0/owsGetCapabilities.xsd");

                marshaller.marshal(cap, xmlStreamWriter);
            } catch (final Exception e) {
                throw new RuntimeException(e.getMessage(), e);
            }
            output = writer.toString();
            LOG.trace("Done! User has the GetCapabilities result !");

        } catch (javax.xml.bind.JAXBException ex) {
            throw new XmlStructuresException("Could not marshall/unmarshall XML structures.", ex);
        }

        LOG.trace("---------------------OUTPUT--------------------------");
        LOG.trace(output);
        LOG.trace("-----------------------------------------------------");

        return output;
    }

    /**
     * WcsServer GetCoverage operation
     */
    public String GetCoverage(String stringXml, WCPS wcps) throws WCSException {
        String output = "Default output. ";

        try {
            // read the input XML
            LOG.trace("Reading the input XML file ... ");
            JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
            Unmarshaller unmarshaller = context.createUnmarshaller();
            Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

            LOG.trace("It is a " + xml.getClass().getSimpleName() + " request.");

            // Convert the WcsServer into WCPS
            LOG.trace("Converting to WCPS request ... please wait");
            GetCoverage wcs = (GetCoverage) xml;

            output = new convertGetCoverage(wcs, meta).get();
            LOG.trace("Done! User has his WCPS request !");
        } catch (JAXBException ex) {
            throw new XmlStructuresException("Could not marshall/unmarshall XML structures.", ex);
        } catch (WCPSException e) {
            throw new InternalComponentException(e.getMessage(), e);
        }

        LOG.trace("---------------------OUTPUT--------------------------");
        LOG.trace(output);
        LOG.trace("-----------------------------------------------------");

        return output;
    }

    /**
     * WcsServer DescribeCoverage operation
     */
    public String DescribeCoverage(String stringXml) throws WCSException {
        String output = "Default output. ";

        try {
            // read the input XML
            LOG.trace("Reading the input XML file ... ");
            JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
            Unmarshaller unmarshaller = context.createUnmarshaller();
            Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

            LOG.trace("It is a " + xml.getClass().getSimpleName() + " request.");

            LOG.trace("Querying the WCPS architecture for the details on this coverage ...");
            DescribeCoverage input = (DescribeCoverage) xml;
            CoverageDescriptions covs = new executeDescribeCoverage(input, meta).get();

            // Write the output file
            LOG.trace("Marshalling with context: " + covs.getClass().getPackage().getName());
            final StringWriter writer = new StringWriter();
            try {
                context = JAXBContext.newInstance(covs.getClass());
                final XMLStreamWriter xmlStreamWriter = XMLOutputFactory.newInstance().createXMLStreamWriter(writer);

                final Marshaller marshaller = context.createMarshaller();
                marshaller.setProperty("com.sun.xml.bind.namespacePrefixMapper", new PetascopeXmlNamespaceMapper());
                marshaller.setProperty("jaxb.formatted.output", true);
                marshaller.setProperty("jaxb.schemaLocation", "http://www.opengis.net/wcs/1.1 http://schemas.opengis.net/wcs/1.1.0/wcsDescribeCoverage.xsd "
                        + "http://www.opengis.net/wcs/1.1/ows http://schemas.opengis.net/wcs/1.1.0/owcsAll.xsd");

                marshaller.marshal(covs, xmlStreamWriter);
            } catch (final Exception e) {
                throw new RuntimeException(e.getMessage(), e);
            }
            output = writer.toString();
            LOG.trace("Done! User has the DescribeCoverage result !");
        } catch (JAXBException ex) {
            throw new XmlStructuresException("Could not marshall/unmarshall XML structures.", ex);
        }

        LOG.trace("---------------------OUTPUT--------------------------");
        LOG.trace(output);
        LOG.trace("-----------------------------------------------------");

        return output;
    }

    private String exceptionReportToXml(ExceptionReport report) {
        String output = null;

        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(
                    report.getClass().getPackage().getName());
            javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();

            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8");
            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
            marshaller.marshal(report, System.err);
            StringWriter strWriter = new StringWriter();

            marshaller.marshal(report, strWriter);
            output = strWriter.toString();
            ok = true;
            LOG.trace("Done marshalling Error Report.");
        } catch (Exception e2) {
            LOG.error("Error marshalling Exception Report.");
            LOG.error("Stack trace: " + e2);
        }

        return output;
    }
}
