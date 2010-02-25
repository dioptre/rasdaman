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


package petascope.wcs.server.core;

import petascope.wcs.server.exceptions.WCSException;
import java.sql.SQLException;
import java.util.Iterator;
import javax.xml.bind.JAXBElement;
import javax.xml.namespace.QName;
import net.opengis.ows.v_1_0_0.CodeType;
import net.opengis.ows.v_1_0_0.ContactType;
import net.opengis.ows.v_1_0_0.OnlineResourceType;
import net.opengis.ows.v_1_0_0.ResponsiblePartySubsetType;
import net.opengis.ows.v_1_0_0.ServiceProvider;
import net.opengis.wcs.ows.v_1_1_0.AllowedValues;
import net.opengis.wcs.ows.v_1_1_0.DCP;
import net.opengis.wcs.ows.v_1_1_0.DomainType;
import net.opengis.wcs.ows.v_1_1_0.HTTP;
import net.opengis.wcs.ows.v_1_1_0.Operation;
import net.opengis.wcs.ows.v_1_1_0.OperationsMetadata;
import net.opengis.wcs.ows.v_1_1_0.ServiceIdentification;
import net.opengis.wcs.ows.v_1_1_0.ValueType;
import net.opengis.wcs.v_1_1_0.Capabilities;
import net.opengis.wcs.v_1_1_0.Contents;
import net.opengis.wcs.v_1_1_0.CoverageSummaryType;
import net.opengis.wcs.v_1_1_0.GetCapabilities;
import petascope.wcps.server.core.DbMetadataSource;
import petascope.wcps.server.core.Metadata;
import petascope.wcps.server.exceptions.ResourceException;

import javax.xml.XMLConstants;
import net.opengis.ows.v_1_0_0.AddressType;
import net.opengis.wcs.ows.v_1_1_0.RequestMethodType;
import petascope.ConfigManager;
import petascope.wcs.server.exceptions.InternalSqlException;
import petascope.wcs.server.exceptions.InvalidParameterValueException;
import petascope.wcs.server.exceptions.NoApplicableCodeException;

/**
 * This class takes a WCS GetCapabilities XML request and executes request,
 * building the corresponding XML respose.
 *
 * @author Andrei Aiordachioaie
 */
public class executeGetCapabilities
{

    private GetCapabilities input;
    private Capabilities output;
    private boolean finished;
    private DbMetadataSource meta;

    /**
     * Default constructor
     * @param cap GetCapabilities object, a WCS (or WCPS) request
	 * @param metadataDbPath Path to the "dbparams.properties" file
     */
    public executeGetCapabilities(GetCapabilities cap, DbMetadataSource source) throws WCSException
    {
        input = cap;
        output = new Capabilities();
        meta = source;
        finished = false;
    }

    /**
     * Main method of this class. Retrieves the response to the GetCapabilities
     * request given to the constructor. If needed, it also calls <b>process()</b>
     * @return a Capabilities object.
     * @throws wcs_web_service.WCSException
     */
    public Capabilities get() throws WCSException
    {
        if (finished == false)
            process();
        if (finished == false)
            throw new NoApplicableCodeException("Could not execute the GetCapabilities request! " +
                    "Please see the other errors...");
        return output;
    }

    /**
     * Computes the response to the GetCapabilities request given to the constructor.
     * @throws wcs_web_service.WCSException
     */
    public void process() throws WCSException
    {
        if (! input.SERVICE.equalsIgnoreCase("WCS"))
                throw new InvalidParameterValueException("Service");

        try
        {
            buildField1();      // Service Identification
            buildField2();      // Service Provider
            buildField3();      // Operations Metadata
            buildField4();      // Contents

            finishBuild();      // Add the remaining required attributes
            finished = true;
        }
        catch (SQLException se)
        {
            finished = false;
            throw new InternalSqlException(se.getMessage(), se);
        }
       
    }

    /**
     * Builds the output node "Service Identification"
     */
    private void buildField1() {
        ServiceIdentification ident = new ServiceIdentification();
        ident.setTitle("PetaScope");
        ident.setAbstract("PetaScope is a suite of OGC web-services comprising of " +
                "WCS, WCS-T and WCPS. It has been developed at Jacobs University, and " +
                "is mentained by the Jacobs University. Copyright Peter Baumann");
        CodeType code = new CodeType();
        code.setValue("WCS");
        ident.setServiceType(code);
        ident.getServiceTypeVersion().add("1.1.0");
        ident.setFees("NONE");

        output.setServiceIdentification(ident);
    }

    /**
     * Builds the output node "Service Provider"
     */
    private void buildField2() {
        ServiceProvider prov = new ServiceProvider();
        prov.setProviderName("Jacobs University Bremen");
        OnlineResourceType site = new OnlineResourceType();
        site.setHref("http://www.petascope.org/");
        prov.setProviderSite(site);
        ResponsiblePartySubsetType resp = new ResponsiblePartySubsetType();
        resp.setIndividualName("Prof. Dr. Peter Baumann");
        CodeType role = new CodeType();
        role.setValue("Project Leader");
        resp.setRole(role);
        ContactType contact = new ContactType();
        AddressType addr = new AddressType();
        addr.getElectronicMailAddress().add("p.baumann@jacobs-university.de");
        addr.setCountry("Germany");
        addr.setCity("Bremen");
        addr.setPostalCode("28717");
        contact.setAddress(addr);
        resp.setContactInfo(contact);
        prov.setServiceContact(resp);

        output.setServiceProvider(prov);
    }

    /**
     * Builds the output node "Operations Metadata"
     * @throws java.sql.SQLException
     */
    private void buildField3() throws SQLException
    {
        OperationsMetadata opmeta = new OperationsMetadata();

            Operation op1 = new Operation();
            op1.setName("GetCapabilities");
                DomainType postE = new DomainType();
                postE.setName("PostEncoding");
                    AllowedValues val1 = new AllowedValues();
                        ValueType valX = new ValueType();
                        valX.setValue("XML");
                    val1.getValueOrRange().add(valX);
                postE.setAllowedValues(val1);
            op1.getConstraint().add(postE);

                DomainType store = new DomainType();
                store.setName("store");
                    AllowedValues val2 = new AllowedValues();
                        ValueType v = new ValueType();
                        v.setValue("false");
                    val2.getValueOrRange().add(v);
                store.setAllowedValues(val2);
            op1.getParameter().add(store);
                DCP dcp = new DCP();
                    HTTP http = new HTTP();
                        RequestMethodType post = new RequestMethodType();
                        post.setHref(ConfigManager.PETASCOPE_SERVLET_URL);
                        post.setType("simple");
                    http.getGetOrPost().add(new JAXBElement<RequestMethodType>(
                            new QName("http://www.opengis.net/wcs/1.1/ows", "Post",
                            XMLConstants.DEFAULT_NS_PREFIX)
                            , RequestMethodType.class, post));
                dcp.setHTTP(http);
            op1.getDCP().add(dcp);
                DomainType paramOnlyXml = new DomainType();
                paramOnlyXml.setName("Format");
                    AllowedValues vals = new AllowedValues();
                        ValueType val = new ValueType();
                        val.setValue("text/xml");
                    vals.getValueOrRange().add(val);
                paramOnlyXml.setAllowedValues(vals);
            op1.getParameter().add(paramOnlyXml);
        opmeta.getOperation().add(op1);

            Operation op2 = new Operation();
            op2.setName("GetCoverage");
            op2.getConstraint().add(postE);     // POST Encoding accepts only XML
            op2.getParameter().add(store);      // Store parameter: not implemented
            op2.getDCP().add(dcp);              // HTTP request URL
            op2.getConstraint().add(paramOnlyXml);
        opmeta.getOperation().add(op2);

            Operation op3 = new Operation();
            op3.setName("DescribeCoverage");
            op3.getConstraint().add(postE);     // POST Encoding accepts only XML
            op3.getParameter().add(store);      // Store parameter: not implemented
            op3.getDCP().add(dcp);              // HTTP request URL
            op3.getConstraint().add(paramOnlyXml);
        opmeta.getOperation().add(op3);

        Operation op4 = new Operation();
            op4.setName("ProcessCoverages");
            op4.getConstraint().add(postE);     // POST Encoding accepts only XML
            op4.getParameter().add(store);      // Store parameter: not implemented
            op4.getDCP().add(dcp);              // HTTP request URL
            op4.getConstraint().add(paramOnlyXml);
        opmeta.getOperation().add(op4);

        Operation op5 = new Operation();
            op5.setName("Transaction");
            op5.getConstraint().add(postE);     // POST Encoding accepts only XML
            op5.getParameter().add(store);      // Store parameter: not implemented
            op5.getDCP().add(dcp);              // HTTP request URL
            op5.getConstraint().add(paramOnlyXml);
        opmeta.getOperation().add(op5);

        output.setOperationsMetadata(opmeta);
    }

    /**
     * Builds the output node "Contents"
     * @throws java.sql.SQLException
     */
    private void buildField4()
    {
        Contents cont = new Contents();
        Iterator<String> coverages = null;
                
        try
        {
            coverages = meta.coverages().iterator();
        }
        catch (ResourceException e)
        {}
        while (coverages.hasNext())
        {
            Metadata metadata = null;
            try { metadata = meta.read(coverages.next()); } catch (Exception e) {}
            String covName = metadata.getCoverageName();
            CoverageSummaryType sum = new CoverageSummaryType();
            sum.setAbstract(metadata.getAbstract());
            sum.setTitle(metadata.getTitle());

            // code holds the coverage value
            CodeType code = new CodeType();
            code.setValue(covName);
            // code is encapsulated in JAXBElement "jelem"
            JAXBElement<String> jelem =
                    new JAXBElement<String>(
                    new QName("http://www.opengis.net/wcs/1.1", "Identifier", XMLConstants.DEFAULT_NS_PREFIX),
                    String.class, covName);
            // Insert "jelem" into this coveragesummary
            sum.getRest().add(jelem);

            // Add this CoverageSummary to the list of coverage summaries, to make up the Contents
            cont.getCoverageSummary().add(sum);
        }
        
        output.setContents(cont);
    }

    private void finishBuild()
    {
        /* WCS Standard Version ! */
        output.setVersion("1.1.0");
    }
    
}
