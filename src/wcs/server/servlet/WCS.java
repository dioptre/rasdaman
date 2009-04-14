/*
 * This file is part of %PACKAGENAME%.
 *
 * %PACKAGENAME% is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * %PACKAGENAME% is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with %PACKAGENAME%. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.%PACKAGENAME%.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package wcs.server.servlet;

import wcs.server.core.*;
import java.io.StringReader;
import java.io.StringWriter;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.Unmarshaller;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import net.opengis.wcs.v_1_1_0.Capabilities;
import net.opengis.wcs.v_1_1_0.CoverageDescriptions;
import net.opengis.wcs.v_1_1_0.DescribeCoverage;
import net.opengis.wcs.v_1_1_0.GetCapabilities;
import net.opengis.wcs.v_1_1_0.GetCoverage;

/**
 * The Web Coverage Service (WCS)
 * This class provides methods for each of the WCS operations:
 * - GetCapabilities
 * - DescribeCoverage
 * - GetCoverage
 */

public class WCS {

    private static Boolean ok;
    private static Boolean printOutput;
    private static Boolean printLog;
	private static String metaDbPath;

    /**
     * Public minimal constructor
	 * @param metadataDbPath path to the "dbparams.properties" file
     */
    public WCS(String metadataDbPath)
    {
        printLog = true;
        printOutput = true;
        ok = true;
		metaDbPath = metadataDbPath;
    }

    /**
     * Record a log message
     * @param msg Text of the message
     */
    public static void log(String msg)
    {
        if (printLog)
            System.err.println(msg);
    }

    /**
     * WCS GetCapabilities operation
     */
    public String GetCapabilities(String stringXml)
    {
        String output = "Default output. ";
        String errmsg = "No error. ";
        ok = false;

        try
        {
            // read the input XML
             log(">>> Reading the input XML file ... ");
             JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
             Unmarshaller unmarshaller = context.createUnmarshaller() ;
             Object xml = unmarshaller.unmarshal(new StringReader(stringXml));
             // Find out which class to execute ...
             log(">>> It is a " + xml.getClass().getSimpleName() + " request.");

             log(">>> Compiling data into XML format for GetCapabilities ... ");
             GetCapabilities input = (GetCapabilities) xml;
             Capabilities cap = new executeGetCapabilities(input,metaDbPath).get();

            // Write the output
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(cap.getClass().getPackage().getName());
            javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
            StringWriter strWriter = new StringWriter();
            marshaller.marshal(cap, strWriter);
            output = strWriter.toString();
            log(">>> Done! User has the GetCapabilities result !");
            ok = true;


        }
        catch (javax.xml.bind.JAXBException ex) {
                 java.util.logging.Logger.getLogger("global").log(java.util.logging.Level.SEVERE, null, ex); //NOI18N
                 ex.printStackTrace();
                 errmsg = ex.toString();
                 log("ERROR 1 ");
             }
        catch (WCSException e)
        {
            log("*** GOT WCS EXCEPTION !");
            ExceptionReport report = e.getReport();
            try
            {
                javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(report.getClass().getPackage().getName());
                javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
                marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
                marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
                marshaller.marshal(report, System.err);
                StringWriter strWriter = new StringWriter();
                marshaller.marshal(report, strWriter);
                output = strWriter.toString();
                ok = true;
                log("Done with the Error Report !");
            }
            catch (Exception e2)
            {
                    e2.printStackTrace();
                    errmsg = e2.toString();
                    log("ERROR 2");
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
            errmsg = e.toString();
            log("ERROR 3");
        }

        if (ok == false)
            output = errmsg;
        if (printOutput)
            System.out.println("*****************\nOUTPUT IS \n" + output);
        
        return output;
    }

    /**
     * WCS GetCoverage operation
     */
    public String GetCoverage(String stringXml)
    {
        String output = "Default output. ";
        String errmsg = "No error. ";
        ok = false;

        try
        {
            // read the input XML
             log(">>> Reading the input XML file ... ");
             JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
             Unmarshaller unmarshaller = context.createUnmarshaller() ;
             Object xml = unmarshaller.unmarshal(new StringReader(stringXml));
             log(">>> It is a " + xml.getClass().getSimpleName() + " request.");

             //Convert the WCS into WCPS
             log(">>> Converting to WCPS ... please wait");
             GetCoverage wcs = (GetCoverage) xml;
             ProcessCoverage wcps = new convertGetCoverage(wcs, metaDbPath).get();
             // Write the output WCPS request
            log(">>> Writing the WCPS XML request ...");
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(wcps.getClass().getPackage().getName());
            javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
            marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
            StringWriter strWriter = new StringWriter();
            marshaller.marshal(wcps, strWriter);
            output = strWriter.toString();
            log(">>> Done! User has his WCPS request !");
            ok = true;
        }
        catch (WCSException e)
        {
            ExceptionReport report = e.getReport();
            try
            {
                javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(report.getClass().getPackage().getName());
                javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
                marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
                marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
                marshaller.marshal(report, System.err);
                StringWriter strWriter = new StringWriter();
                marshaller.marshal(report, strWriter);
                output = strWriter.toString();
                ok = true;
                log("Done with the Error Report !");
            }
            catch (Exception e2)
            {
                    e2.printStackTrace();
                    errmsg = e2.toString();
                    log("ERROR 4");
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
            errmsg = e.toString();
            log("ERROR 5");
        }

        if (ok == false)
            output = errmsg;
        if (printOutput)
            System.out.println("*****************\nOUTPUT IS \n" + output);

        return output;
    }

    /**
     * WCS DescribeCoverage operation
     */
    public String DescribeCoverage(String stringXml)
    {
        String output = "Default output. ";
        String errmsg = "No error. ";
        ok = false;

        try
        {
            // read the input XML
             log(">>> Reading the input XML file ... ");
             JAXBContext context = JAXBContext.newInstance("net.opengis.wcs.v_1_1_0");
             Unmarshaller unmarshaller = context.createUnmarshaller() ;
             Object xml = unmarshaller.unmarshal(new StringReader(stringXml));
             log(">>> It is a " + xml.getClass().getSimpleName() + " request.");

             log(">>> Querying the WCPS architecture for the details on this coverage ...");
             DescribeCoverage input = (DescribeCoverage) xml;
             CoverageDescriptions covs = new executeDescribeCoverage(input, metaDbPath).get();

             // Write the output file
             javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(covs.getClass().getPackage().getName());
             javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
             marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
             marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
             StringWriter strWriter = new StringWriter();
             marshaller.marshal(covs, strWriter);
             output = strWriter.toString();
             log(">>> Done! User has the DescribeCoverage result !");
             ok = true;
        }
        catch (WCSException e)
        {
            ExceptionReport report = e.getReport();
            try {
                javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(report.getClass().getPackage().getName());
                javax.xml.bind.Marshaller marshaller = jaxbCtx.createMarshaller();
                marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
                marshaller.setProperty(javax.xml.bind.Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
                marshaller.marshal(report, System.err);
                StringWriter strWriter = new StringWriter();
                marshaller.marshal(report, strWriter);
                output = strWriter.toString();
                ok = true;
                log("Done with the Error Report !");
            }
            catch (Exception e2)
            {
                    e2.printStackTrace();
                    errmsg = e2.toString();
                    log("ERROR 6");
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
            errmsg = e.toString();
            log("ERROR 7");
        }


        if (ok == false)
            output = errmsg;
        if (printOutput)
            System.out.println("*****************\nOUTPUT IS \n" + output);

        return output;
    }

}
