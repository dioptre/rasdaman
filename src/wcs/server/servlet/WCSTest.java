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

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import org.apache.commons.io.FileUtils;
import org.junit.*;
import org.junit.Assert;

public class WCSTest {

    // Location of the input files, relative to the project folder root
    private String pathPrefix = "../testing/wcs_test/input/";
	// Path to "dbparams.properties" file
	private String metadataDbSettings = "/home/andrei/Job/WCPS Update/WCPS_Project/wcps/dbparams.properties";

    public WCSTest()
    {
    }

    /**
     * Test of GetCoverage method, of class WCS, with input file "GetCoverage1.xml"
     * Output should be an ProcessCoverage XML, not a ExceptionReport XML
     */
    @Test
// Uncomment the following line to skip this test
//    @Ignore
    public void testGetCoverage1() throws IOException {
        System.out.println("*** Testing GetCoverage1.xml");
        String stringXml = "";
        boolean gotWcsException = false;
        stringXml = FileUtils.readFileToString(new File(pathPrefix + "GetCoverage1.xml"));

        WCS instance = new WCS(metadataDbSettings);
        String result = instance.GetCoverage(stringXml);

        ExceptionReport report = new ExceptionReport();
        // Try reading the exception report
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(ExceptionReport.class.getPackage().getName());
            javax.xml.bind.Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
            report = (ExceptionReport) unmarshaller.unmarshal(new StringReader(result));
            gotWcsException = true;
        } catch (Exception e) {

        }

        Assert.assertEquals(false, gotWcsException);

    }


    /**
     * Test of GetCoverage method, of class WCS, with input file "GetCoverage2.xml"
     * Output should be ProcessCoverage XML, not a ExceptionReport XML
     */
    @Test
// Uncomment the following line to skip this test
//    @Ignore
    public void testGetCoverage2() throws IOException {
        System.out.println("*** Testing GetCoverage2.xml");
        String stringXml = "";
        boolean gotWcsException = false;
        stringXml = FileUtils.readFileToString(new File(pathPrefix + "GetCoverage2.xml"));

        WCS instance = new WCS(metadataDbSettings);
        String result = instance.GetCoverage(stringXml);

        ExceptionReport report = new ExceptionReport();
        // Try reading the exception report
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(ExceptionReport.class.getPackage().getName());
            javax.xml.bind.Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
            report = (ExceptionReport) unmarshaller.unmarshal(new StringReader(result));
            gotWcsException = true;
        } catch (Exception e) {

        }

        Assert.assertEquals(false, gotWcsException);
    }


    /**
     * Test of GetCoverage method, of class WCS, with input file "GetCoverage3.xml"
     * Output should be ProcessCoverage XML, not a ExceptionReport XML
     */
    @Test
// Uncomment the following line to skip this test
//    @Ignore
    public void testGetCoverage3() throws IOException {
        System.out.println("*** Testing GetCoverage3.xml");
        String stringXml = "";
        boolean gotWcsException = false;
        stringXml = FileUtils.readFileToString(new File(pathPrefix + "GetCoverage3.xml"));

        WCS instance = new WCS(metadataDbSettings);
        String result = instance.GetCoverage(stringXml);

        ExceptionReport report = new ExceptionReport();
        // Try reading the exception report
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(ExceptionReport.class.getPackage().getName());
            javax.xml.bind.Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
            report = (ExceptionReport) unmarshaller.unmarshal(new StringReader(result));
            gotWcsException = true;
        } catch (Exception e) {

        }

        Assert.assertEquals(false, gotWcsException);
    }

    /**
     * Test of GetCoverage method, of class WCS, with input file "GetCoverage4.xml"
     * Output should be an ExceptionReport XML, not a ProcessCoverage XML
     */
    @Test
// Uncomment the following line to skip this test
//    @Ignore
    public void testGetCoverage4() throws IOException {
        System.out.println("*** Testing GetCoverage4.xml");
        String stringXml = "";
        boolean gotWcsException = false;
        stringXml = FileUtils.readFileToString(new File(pathPrefix + "GetCoverage4.xml"));

        WCS instance = new WCS(metadataDbSettings);
        String result = instance.GetCoverage(stringXml);

        ExceptionReport report = new ExceptionReport();
        // Read the ExceptionReport from the "result"
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(ExceptionReport.class.getPackage().getName());
            javax.xml.bind.Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
            report = (ExceptionReport) unmarshaller.unmarshal(new StringReader(result));
            gotWcsException = true;
        } catch (Exception e) {

        }

        Assert.assertEquals(true, gotWcsException);
    }

    /**
     * Test of DescribeCoverage method, of class WCS, with file DescribeCoverage1.xml
     * Output should be an ExceptionReport XML, not a CoverageDescriptions XML
     */
    @Test
// Uncomment the following line to skip this test
//    @Ignore
    public void testDescribeCoverage1() throws IOException {
        System.out.println("*** Testing DescribeCoverage1.xml");
        String stringXml = "";
        boolean gotWcsException = false;
        stringXml = FileUtils.readFileToString(new File(pathPrefix + "DescribeCoverage1.xml"));

        WCS instance = new WCS(metadataDbSettings);
        String result = instance.DescribeCoverage(stringXml);

        ExceptionReport report = new ExceptionReport();
        // Read the ExceptionReport from the "result"
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(ExceptionReport.class.getPackage().getName());
            javax.xml.bind.Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
            report = (ExceptionReport) unmarshaller.unmarshal(new StringReader(result));
            gotWcsException = true;
        } catch (Exception e) {

        }

        Assert.assertEquals(true, gotWcsException);
    }

    /**
     * Test of DescribeCoverage method, of class WCS, with file DescribeCoverage2.xml
     * Output should be a CoverageDescriptions XML, not an ExceptionReport XML
     */
    @Test
// Uncomment the following line to skip this test
//    @Ignore
    public void testDescribeCoverage2() throws IOException {
        System.out.println("*** Testing DescribeCoverage2.xml");
        String stringXml = "";
        boolean gotWcsException = false;
        stringXml = FileUtils.readFileToString(new File(pathPrefix + "DescribeCoverage2.xml"));

        WCS instance = new WCS(metadataDbSettings);
        String result = instance.DescribeCoverage(stringXml);

        ExceptionReport report = new ExceptionReport();
        // Read the ExceptionReport from the "result"
        try {
            javax.xml.bind.JAXBContext jaxbCtx = javax.xml.bind.JAXBContext.newInstance(ExceptionReport.class.getPackage().getName());
            javax.xml.bind.Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
            report = (ExceptionReport) unmarshaller.unmarshal(new StringReader(result));
            gotWcsException = true;
        } catch (Exception e) {

        }

        Assert.assertEquals(false, gotWcsException);
    }

}
