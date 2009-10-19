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

package wcs.server;

//~--- non-JDK imports --------------------------------------------------------

import net.opengis.ows.v_1_0_0.ExceptionReport;
import net.opengis.wcs.v_1_1_0.Capabilities;
import net.opengis.wcs.v_1_1_0.CoverageDescriptions;
import net.opengis.wcs.v_1_1_0.DescribeCoverage;
import net.opengis.wcs.v_1_1_0.GetCapabilities;
import net.opengis.wcs.v_1_1_0.GetCoverage;

import wcps.server.core.DbMetadataSource;
import wcps.server.core.WCPS;
import wcps.server.core.WCPSException;
import wcs.server.core.WCSException;
import wcs.server.core.convertGetCoverage;
import wcs.server.core.executeDescribeCoverage;
import wcs.server.core.executeGetCapabilities;

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

/**
 * The Web Coverage Service (WcsServer)
 * This class provides methods for each of the WCS operations:
 * - GetCapabilities
 * - DescribeCoverage
 * - GetCoverage
 *
 * @author Andrei Aiordachioaie
 */
public class WcsServer
{
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
	public WcsServer(String settingsPath, DbMetadataSource source)
	{
		ConfigManager.getInstance(settingsPath);
		printLog = ConfigManager.PRINT_LOG;
		printOutput = ConfigManager.PRINT_OUT;
		meta = source;
		ok = true;
	}

	/**
	 * Record a log message
	 * @param msg Text of the message
	 */
	public static void log(String msg)
	{
		if ( printLog )
			System.err.println("WCS: " + msg);
	}

	/**
	 * WcsServer GetCapabilities operation
	 */
	public String GetCapabilities(String stringXml) throws WCSException
	{
		String output = "Default output. ";

		try
		{
			// read the input XML
			log("Reading the input XML file ... ");
			JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
			Unmarshaller unmarshaller = context.createUnmarshaller();
			Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

			// Find out which class to execute ...
			log("It is a " + xml.getClass().getSimpleName() + " request.");

			log("Compiling data into XML format for GetCapabilities ... ");
			GetCapabilities input = (GetCapabilities) xml;
			Capabilities cap = new executeGetCapabilities(input, meta).get();


            // Write the output
            log("MArshalling with context: " + cap.getClass().getPackage().getName());
            final StringWriter writer = new StringWriter();
            try {
                context = JAXBContext.newInstance(cap.getClass());
                final XMLStreamWriter xmlStreamWriter = XMLOutputFactory
                        .newInstance().createXMLStreamWriter(writer);
                
                final Marshaller marshaller = context.createMarshaller();
                marshaller.setProperty("com.sun.xml.bind.namespacePrefixMapper", new PetascopeXmlNamespaceMapper());
                marshaller.setProperty("jaxb.formatted.output", true);
                marshaller.setProperty("jaxb.schemaLocation", "http://www.opengis.net/wcs/1.1 http://schemas.opengis.net/wcs/1.1.0/wcsGetCapabilities.xsd " +
                        "http://www.opengis.net/wcs/1.1/ows http://schemas.opengis.net/wcs/1.1.0/owsGetCapabilities.xsd");

                marshaller.marshal(cap, xmlStreamWriter);
            } catch (final Exception e) {
                throw new RuntimeException(e.getMessage(), e);
            }
            output = writer.toString();
			log("Done! User has the GetCapabilities result !");

		}
		catch (javax.xml.bind.JAXBException ex)
		{
			throw new WCSException("XmlStructuresError",
                        "Could not marshall/unmarshall XML structures.", ex);
		}

		if ( printOutput )
			System.out.println("*****************\nOUTPUT IS \n" + output);

		return output;
	}

	/**
	 * WcsServer GetCoverage operation
	 */
	public String GetCoverage(String stringXml, WCPS wcps) throws WCSException
	{
		String output = "Default output. ";

		try
		{
			// read the input XML
			log("Reading the input XML file ... ");
			JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
			Unmarshaller unmarshaller = context.createUnmarshaller();
			Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

			log("It is a " + xml.getClass().getSimpleName() + " request.");

			// Convert the WcsServer into WCPS
			log("Converting to WCPS request ... please wait");
			GetCoverage wcs = (GetCoverage) xml;

			output = new convertGetCoverage(wcs, meta).get();
			log("Done! User has his WCPS request !");
		}
		catch (JAXBException ex)
		{
			throw new WCSException("XmlStructuresError",
								   "Could not marshall/unmarshall XML structures.", ex);
		}
		catch (WCPSException e)
		{
			throw new WCSException("InternalWcpsError", e.getMessage(), e);
		}

		if ( printOutput )
			System.out.println("*****************\nOUTPUT IS \n" + output);

		return output;
	}

	/**
	 * WcsServer DescribeCoverage operation
	 */
	public String DescribeCoverage(String stringXml) throws WCSException
	{
		String output = "Default output. ";

		try
		{
			// read the input XML
			log("Reading the input XML file ... ");
			JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
			Unmarshaller unmarshaller = context.createUnmarshaller();
			Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

			log("It is a " + xml.getClass().getSimpleName() + " request.");

			log("Querying the WCPS architecture for the details on this coverage ...");
			DescribeCoverage input = (DescribeCoverage) xml;
			CoverageDescriptions covs = new executeDescribeCoverage(input, meta).get();

			// Write the output file
            log("MArshalling with context: " + covs.getClass().getPackage().getName());
            final StringWriter writer = new StringWriter();
            try {
                context = JAXBContext.newInstance(covs.getClass());
                final XMLStreamWriter xmlStreamWriter = XMLOutputFactory
                        .newInstance().createXMLStreamWriter(writer);

                final Marshaller marshaller = context.createMarshaller();
                marshaller.setProperty("com.sun.xml.bind.namespacePrefixMapper", new PetascopeXmlNamespaceMapper());
                marshaller.setProperty("jaxb.formatted.output", true);
                marshaller.setProperty("jaxb.schemaLocation", "http://www.opengis.net/wcs/1.1 http://schemas.opengis.net/wcs/1.1.0/wcsDescribeCoverage.xsd " +
                        "http://www.opengis.net/wcs/1.1/ows http://schemas.opengis.net/wcs/1.1.0/owcsAll.xsd");

                marshaller.marshal(covs, xmlStreamWriter);
            } catch (final Exception e) {
                throw new RuntimeException(e.getMessage(), e);
            }
            output = writer.toString();
			log("Done! User has the DescribeCoverage result !");
		}
		catch (JAXBException ex)
		{
			throw new WCSException("XmlStructuresError",
								   "Could not marshall/unmarshall XML structures.", ex);
		}

		if ( printOutput )
			System.out.println("*****************\nOUTPUT IS \n" + output);

		return output;
	}

	private String exceptionToXml(WCSException e)
	{
		return exceptionReportToXml(e.getReport());
	}

	private String exceptionReportToXml(ExceptionReport report)
	{
		String output = null;

		try
		{
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
			log("Done marshalling Error Report.");
		}
		catch (Exception e2)
		{
			e2.printStackTrace();
			log("Error marshalling Exception Report.");
		}

		return output;
	}
}
