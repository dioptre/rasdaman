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





package petascope.wcs.server.cli;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

/**
 * Command Line Interface class, for testing the WCS Server operation
 * Converts an WCS XML input file into a WCPS XML input file.<br />
 * Settings are read from the file "input-output.txt", in the current folder.
 *
 * @author Andrei Aiordachioaie
 */
public class CLI {

    private static final String settingsFile = "input-output.txt";
    private static String inputFile;
    private static String outputFile;
    private static String metadataSettingsFile;
    private static Boolean ok;
    private static Boolean printOutput;
    private static Boolean printLog = true;

    public static void log(String msg) {
        if (printLog) {
            System.out.println(msg);
        }
    }

    public static boolean init() {
        ok = true;
        printOutput = true;

        Properties set = new Properties();
        try {
            set.load(new FileInputStream(settingsFile));
        } catch (IOException ex) {
            ex.printStackTrace();
            ok = false;
        }

        inputFile = set.getProperty("inputFile");
        outputFile = set.getProperty("outputFile");
        metadataSettingsFile = set.getProperty("metadataSettingsFile");
        printOutput = Boolean.parseBoolean(set.getProperty("printOutputFile"));
        printLog = Boolean.parseBoolean(set.getProperty("printLogMessages"));

        return ok;
    }

    /**
     * Converts a WCS XML input file into a WCPS XML input file
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        if (!init()) {
            return;
        }
        /*
        try
        {
        // read the input XML
        log(">>> Reading the input XML file ... ");
        JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
        Unmarshaller unmarshaller = context.createUnmarshaller() ;
        Object xml = unmarshaller.unmarshal(new FileInputStream(inputFile));
        // Find out which class to execute ...
        log(">>> It is a " + xml.getClass().getSimpleName() + " request.");
        if (xml instanceof GetCoverage)
        {
        //Convert the WCS into WCPS
        log(">>> Converting to WCPS ... please wait");
        GetCoverage wcs = (GetCoverage) xml;
        ProcessCoverage wcps = new convertGetCoverage(wcs, metadataSettingsFile).get();
        // Write the output WCPS request
        try {
        log(">>> Writing the WCPS XML request");
        javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(wcps.getClass().getPackage().getName());
        javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
        marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
        marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
        if (printOutput)
        marshaller.marshal(wcps, System.out);
        marshaller.marshal(wcps, new FileOutputStream(outputFile));
        } catch (javax.xml.bind.JAXBException ex) {
        // XXXTODO Handle exception
        java.util.logging.Logger.getLogger("global").log(java.util.logging.Level.SEVERE, null, ex); //NOI18N
        ex.printStackTrace();
        }
        }
        else
        if (xml instanceof GetCapabilities)
        {
        log(">>> Compiling data into XML format for GetCapabilities ... ");
        GetCapabilities input = (GetCapabilities) xml;
        Capabilities cap = new executeGetCapabilities(input, metadataSettingsFile).get();
        // Write the output file (WCS)
        try {
        javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(cap.getClass().getPackage().getName());
        javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
        marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
        marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
        if (printOutput)
        marshaller.marshal(cap, System.out);
        marshaller.marshal(cap, new FileOutputStream(outputFile));

        } catch (javax.xml.bind.JAXBException ex) {
        // XXXTODO Handle exception
        java.util.logging.Logger.getLogger("global").log(java.util.logging.Level.SEVERE, null, ex); //NOI18N
        ex.printStackTrace();
        }
        }
        else
        if (xml instanceof DescribeCoverage)
        {
        log(">>> Querying the WCPS architecture for the details on this coverage ...");
        DescribeCoverage input = (DescribeCoverage) xml;
        CoverageDescriptions output = new executeDescribeCoverage(input, metadataSettingsFile).get();
        // Write the output file (WCS)
        try {
        javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(output.getClass().getPackage().getName());
        javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
        marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
        marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
        if (printOutput)
        marshaller.marshal(output, System.out);
        marshaller.marshal(output, new FileOutputStream(outputFile));
        } catch (javax.xml.bind.JAXBException ex) {
        // XXXTODO Handle exception
        java.util.logging.Logger.getLogger("global").log(java.util.logging.Level.SEVERE, null, ex); //NOI18N
        ex.printStackTrace();
        }
        }
        else
        throw new Exception("ERROR ! Unrecognized request type " + xml.getClass().getSimpleName());
        }
        catch (WCSException e)
        {
        ExceptionReport report = e.getReport();
        try {
        javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(report.getClass().getPackage().getName());
        javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
        marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
        marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
        if (printOutput)
        marshaller.marshal(report, System.err);
        marshaller.marshal(report, new FileOutputStream(outputFile));
        } catch (javax.xml.bind.JAXBException ex) {
        // XXXTODO Handle exception
        java.util.logging.Logger.getLogger("global").log(java.util.logging.Level.SEVERE, null, ex); //NOI18N
        ex.printStackTrace();
        }
        catch (Exception e2)
        {
        e2.printStackTrace();
        }
        }
        catch (Exception e)
        {
        e.printStackTrace();
        }
        log(">>> Done !");
         */
    }
}
