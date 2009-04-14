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

package wcs.server.core;

import java.sql.ResultSet;
import java.sql.SQLException;
import javax.xml.bind.JAXBElement;
import javax.xml.namespace.QName;
import net.opengis.ows.v_1_0_0.BoundingBoxType;
import net.opengis.wcs.ows.v_1_1_0.DomainMetadataType;
import net.opengis.wcs.ows.v_1_1_0.InterpolationMethodType;
import net.opengis.wcs.ows.v_1_1_0.InterpolationMethods;
import net.opengis.wcs.ows.v_1_1_0.UnNamedDomainType;
import net.opengis.wcs.v_1_1_0.CoverageDescriptionType;
import net.opengis.wcs.v_1_1_0.CoverageDescriptions;
import net.opengis.wcs.v_1_1_0.CoverageDomainType;
import net.opengis.wcs.v_1_1_0.DescribeCoverage;
import net.opengis.wcs.v_1_1_0.FieldType;
import net.opengis.wcs.v_1_1_0.RangeType;
import net.opengis.wcs.v_1_1_0.SpatialDomainType;

/**
 * This class takes a WCS DescribeCoverage XML request and executes request,
 * building the corresponding XML respose.
 * NOTE: Since the WCPS and WCS services share the format of DescribeCoverage
 * request and response, this class can also answer WCPS DescribeCoverage
 * requests. In fact, this is what it does!
 */

public class executeDescribeCoverage {

    private DescribeCoverage input;
    private CoverageDescriptions output;
    private boolean finished;
    private static boolean printLog = true;
    private MetadataDb meta;
    private String pathDbSettings;		// Path to the "dbparams.properties" file

    /**
     * Log a message to System.out
     * @param str Text of the message
     */
    static void log(String str)
    {
        if (printLog)
            System.out.println("<WCS DescribeCoverage> " + str);
    }

    /**
     * Default constructor
     * @param cap DescribeCoverage object, a WCS (or WCPS) request
	 * @param metadataDbPath Path to the "dbparams.properties" file
     */
    public executeDescribeCoverage(DescribeCoverage cap, String metadataDbPath) throws WCSException
    {
        input = cap;
        output = new CoverageDescriptions();
        finished = false;
		pathDbSettings = metadataDbPath;
        meta = new MetadataDb(pathDbSettings);
    }

    /**
     * Main method of this class. Retrieves the response to the DescribeCoverage
     * request given to the constructor. If needed, it also calls <b>process()</b>
     * @return a CoverageDescriptions object.
     * @throws wcs_web_service.WCSException
     */
    public CoverageDescriptions get() throws WCSException
    {
        try
        {
            if (finished == false)
                process();
        }
        catch (WCSException e)
        {
            System.err.println("ERROR ! Could not convert the WCS xml into WCPS xml !!!");
            e.printStackTrace();
            throw e;
        }
        if (finished == false)
            throw new WCSException("NoApplicableCode", "Could not execute the GetCapabilities request! " +
                    "Please see the other errors...");
        return output;
    }
    
    /**
     * Computes the response to the GetCapabilities request given to the constructor.
     */
    public void process() throws WCSException
    {
        String id;
        for (int i = 0; i < input.getIdentifier().size(); i++)
        {
            id = input.getIdentifier().get(i);
            output.getCoverageDescription().add(getCoverageDescription(id));
        }
        finished = true;
    }    

    /**
     * Retrieve details for one coverage.
     * @param name Name of the coverage
     * @return CoverageDescriptionType object, that can just be plugged in the respose object
     */
    private CoverageDescriptionType getCoverageDescription(String name) throws WCSException
    {
        String query;
        CoverageDescriptionType desc = new CoverageDescriptionType();

        // Error checking: is the coverage available?
        if (!this.existsCoverage(name))
            throw new WCSException("InvalidParameterValue", "Identifier",
                    "Coverage " + name + " is not served by this server !");

        
        desc.setIdentifier(name);
        desc.setTitle("Coverage " + name);
        desc.setAbstract("Available coverage, fetched from the database");

        // Read the coverage Domain from the database
        CoverageDomainType domain = null;
        boolean ok1 = false;
        query = "SELECT  d.name, lo, hi FROM " +
                "wcps_celldomain AS cd JOIN wcps_coverage AS cov ON cd.coverage = cov.id " +
                "JOIN wcps_domain AS d ON d.coverage = cov.id AND d.i = cd.i " +
                "WHERE cov.name='" + name + "'" +
                " ORDER BY cd.i ASC";
        try
        {
            ResultSet r = meta.executeQuery(query);
            log(query);
            Double lo0=0.0, lo1=0.0, hi0=0.0, hi1=0.0;
            if (r.next() && ! r.getString("name").equals("t"))
            {
                log(r.getString("name"));
                lo0 = r.getDouble("lo");
                hi0 = r.getDouble("hi");
                if (r.next() && ! r.getString("name").equals("t"))
                {
                    log(r.getString("name"));
                    lo1 = r.getDouble("lo");
                    hi1 = r.getDouble("hi");
                    ok1 = true;
                }
            }

            domain = new CoverageDomainType();
                SpatialDomainType spatial = new SpatialDomainType();
                    BoundingBoxType bbox = new BoundingBoxType();
                    bbox.getLowerCorner().add(lo0);
                    bbox.getLowerCorner().add(lo1);
                    bbox.getUpperCorner().add(hi0);
                    bbox.getUpperCorner().add(hi1);
                spatial.getBoundingBox().add(new JAXBElement<BoundingBoxType>(new QName("BoundingBox"), BoundingBoxType.class, bbox));
            domain.setSpatialDomain(spatial);
        }
        catch (SQLException se)
        {
            System.err.println("SQL Error in query:\n " + query);
            se.printStackTrace();
            throw new WCSException("NoApplicableCode",
                    "SQL Error in query:\n " + query, "SQL Error in query:\n " + query);
        }

        log("ok1 = " + String.valueOf(ok1));
        if (ok1)
            desc.setDomain(domain);

        // Read the coverage Range from the database
        RangeType range = new RangeType();
        boolean ok2 = false;
        FieldType field;
        query = "SELECT c.name AS cov, r.name AS field , d.datatype AS definterp, n.nullresistance AS defnull, d2.datatype " +
                "FROM wcps_range AS r JOIN wcps_coverage AS c ON r.coverage = c.id " +
                "JOIN wcps_datatype AS d ON interpolationtypedefault = d.id " +
                "JOIN wcps_nullresistance AS n ON n.id = nullresistancedefault " +
                "JOIN wcps_datatype AS d2 ON r.type = d2.id " +
                "WHERE c.name = '" + name + "' " +
                "ORDER BY r.i ASC";
        try
        {
            ResultSet r = meta.executeQuery(query);
            log(query);
            while (r.next())
            {
                field = new FieldType();

                field.setIdentifier(r.getString("field"));
                    UnNamedDomainType domtype = new UnNamedDomainType();
                        DomainMetadataType dommeta = new DomainMetadataType();
                        dommeta.setValue(r.getString("datatype"));
                    domtype.setDataType(dommeta);
                field.setDefinition(domtype);

                    InterpolationMethods interp = new InterpolationMethods();

                        InterpolationMethodType meth = new InterpolationMethodType();
                        meth.setValue(r.getString("definterp"));
                        meth.setNullResistance(r.getString("defnull"));
                    interp.setDefaultMethod(meth);

                        query = "SELECT c.name, datatype AS interp, n.nullresistance " +
                                "FROM wcps_interpolationset AS i JOIN wcps_coverage AS c ON i.coverage = c.id " +
                                "JOIN wcps_datatype AS d ON interpolationtypedefault = d.id " +
                                "JOIN wcps_nullresistance AS n ON n.id = nullresistancedefault " +
                                "WHERE c.name = '" + name + "' " +
                                "ORDER BY coverage";
                        ResultSet r2 = meta.executeQuery(query);
                        while (r2.next())
                        {
                            meth = new InterpolationMethodType();
                            meth.setValue(r2.getString("interp"));
                            meth.setNullResistance(r2.getString("nullresistance"));
                            if ( ! (meth.getValue().equalsIgnoreCase(interp.getDefaultMethod().getValue())
                                    && meth.getNullResistance().equalsIgnoreCase(interp.getDefaultMethod().getNullResistance())) )
                                        interp.getOtherMethod().add(meth);
                            ok2 = true;
                        }
                field.setInterpolationMethods(interp);
                range.getField().add(field);
            }
   
        }
        catch (SQLException se)
        {
            System.err.println("SQL Error in query:\n " + query);
            se.printStackTrace();
            ok2 = false;
            throw new WCSException("NoApplicableCode",
                    "SQL Error in query:\n " + query, "SQL Error in query:\n " + query);
        }

        log("ok2 = " + String.valueOf(ok2));
        if (ok2)
            desc.setRange(range);

        // Supported format for GetCoverage
        desc.getSupportedFormat().add("text/xml");

        // Available CRSs for current coverage
        try
        {
            query = "SELECT crs FROM wcps_coverage WHERE name='" + name + "'";
            ResultSet r = meta.executeQuery(query);
            if (r.next())
            {
                desc.getSupportedCRS().add(r.getString("crs"));
            }
        }
        catch (SQLException se)
        {
            se.printStackTrace();
            throw new WCSException("NoApplicableCode",
                    "SQL Error in query:\n " + query, "SQL Error in query:\n " + query);
        }
        
        return desc;
    }

    /**
     * Checks the metadata DB to ,ake sure that the WCPS server has coverage "name"
     * @param name the name of the coverage to check
     * @return boolean whether the coverage is available
     */
    private boolean existsCoverage(String name)
    {

        String query = "SELECT * FROM wcps_coverage AS cov " +
                "WHERE cov.name='" + name + "'";
        try
        {
            ResultSet r = meta.executeQuery(query);
            log(query);
            if (r.next())
            {
                log("Coverage " + name + " is in the database !");
            }
            else
            {
                log("Coverage " + name + " is NOT in the database !");
                return false;
            }
        }
        catch (Exception e)
        {
            return false;
        }

        return true;
    }
}
