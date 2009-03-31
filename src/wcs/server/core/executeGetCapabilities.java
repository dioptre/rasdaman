package wcs.server.core;

import java.sql.ResultSet;
import java.sql.SQLException;
import javax.xml.bind.JAXBElement;
import javax.xml.namespace.QName;
import net.opengis.wcs.v_1_1_0.Capabilities;
import net.opengis.ows.v_1_0_0.CodeType;
import net.opengis.ows.v_1_0_0.OnlineResourceType;
import net.opengis.wcs.ows.v_1_1_0.OperationsMetadata;
import net.opengis.wcs.ows.v_1_1_0.ServiceIdentification;
import net.opengis.ows.v_1_0_0.ServiceProvider;
import net.opengis.wcs.ows.v_1_1_0.AllowedValues;
import net.opengis.wcs.ows.v_1_1_0.DCP;
import net.opengis.wcs.ows.v_1_1_0.DomainType;
import net.opengis.wcs.ows.v_1_1_0.HTTP;
import net.opengis.wcs.ows.v_1_1_0.Operation;
import net.opengis.wcs.ows.v_1_1_0.RequestMethodType;
import net.opengis.wcs.ows.v_1_1_0.ValueType;
import net.opengis.wcs.v_1_1_0.Contents;
import net.opengis.wcs.v_1_1_0.CoverageSummaryType;
import net.opengis.wcs.v_1_1_0.GetCapabilities;

/**
 * This class takes a WCS GetCapabilities XML request and executes request,
 * building the corresponding XML respose.
 * NOTE: Since the WCPS and WCS services share the format of GetCapabilities
 * request and response, this class can also answer WCPS GetCapabilities
 * requests. In fact, this is what it does!
 *
 * @author Andrei Aiordachioaie
 */
public class executeGetCapabilities {

	private final static String SERVLET_URL = "http://kahlua.eecs.jacobs-university.de/wcs-andrei/";

    private GetCapabilities input;
    private Capabilities output;
    private boolean finished;
    private static boolean printLog = true;
    private MetadataDb meta;
    private String pathDbSettings;		// Path to the "dbparams.properties" file

    /**
     * Log a message to System.out
     * @param str Text of the message
     */
    static private void log(String str)
    {
        if (printLog)
            System.out.println("<WCS GetCapabilities> " + str);
    }

    /**
     * Default constructor
     * @param cap GetCapabilities object, a WCS (or WCPS) request
	 * @param metadataDbPath Path to the "dbparams.properties" file
     */
    public executeGetCapabilities(GetCapabilities cap, String metadataDbPath) throws WCSException
    {
        input = cap;
        output = new Capabilities();
		pathDbSettings = metadataDbPath;
        meta = new MetadataDb(pathDbSettings);
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
            throw new WCSException("NoApplicableCode", "Could not execute the GetCapabilities request! " +
                    "Please see the other errors...");
        return output;
    }

    /**
     * Computes the response to the GetCapabilities request given to the constructor.
     * @throws wcs_web_service.WCSException
     */
    @SuppressWarnings("static-access")
    public void process() throws WCSException
    {
        if (! input.SERVICE.equalsIgnoreCase("WCS"))
                throw new WCSException("InvalidParameterValue", "Service",
                    "Identifier must be a child of GetCoverage !");

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
            se.printStackTrace();
            finished = false;
        }
       
    }

    /**
     * Builds the output node "Service Identification"
     */
    private void buildField1() {
        ServiceIdentification ident = new ServiceIdentification();
        ident.setTitle("WCS Coverage Server @ Jacobs University");
        ident.setAbstract("WCS Server developed at jacobs University");
        CodeType code = new CodeType();
        code.setValue("OGC WCS");
        ident.setServiceType(code);
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
        site.setHref("http://www.jacobs-university.de/");
        prov.setProviderSite(site);

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
                DCP dcp = new DCP();
                    HTTP http = new HTTP();
					/*
								// What is the URL for SOAP? 
                        RequestMethodType soap = new RequestMethodType();
                        soap.setHref("http://localhost/SOAP/URL/for/GetCapabalities");
                    http.getGetOrPost().add(new JAXBElement<RequestMethodType>(new QName("Soap"), RequestMethodType.class, soap));
					*/
                        RequestMethodType post = new RequestMethodType();
                        post.setHref(SERVLET_URL);
                        // TODO: Add "Get" request type once KVP encoding is supported
                    http.getGetOrPost().add(new JAXBElement<RequestMethodType>(new QName("Post"), RequestMethodType.class, post));
                dcp.setHTTP(http);
            op1.getDCP().add(dcp);
        opmeta.getOperation().add(op1);

            Operation op2 = new Operation();
            op2.setName("GetCoverage");
                dcp = new DCP();
                    http = new HTTP();
					/*
								// What is the URL for SOAP? 
                        soap = new RequestMethodType();
                        soap.setHref("http://localhost/SOAP/URL/for/GetCapabalities");
                    http.getGetOrPost().add(new JAXBElement<RequestMethodType>(new QName("Soap"), RequestMethodType.class, soap));
					*/
                        post = new RequestMethodType();
                        post.setHref(SERVLET_URL);
                        // TODO: Add "Get" request type once KVP encoding is supported
                    http.getGetOrPost().add(new JAXBElement<RequestMethodType>(new QName("Post"), RequestMethodType.class, post));
                dcp.setHTTP(http);
            op2.getDCP().add(dcp);
                DomainType param = new DomainType();
                param.setName("Format");
                    AllowedValues vals = new AllowedValues();
                        ValueType val;
                    ResultSet set = this.meta.executeQuery("SELECT * FROM wcps_format;");
                    while (set.next())
                    {
                        val = new ValueType();
                        val.setValue(set.getString("mimetype"));
                    vals.getValueOrRange().add((Object)val);
                    }
                param.setAllowedValues(vals);
            op2.getConstraint().add(param);
        opmeta.getOperation().add(op2);

            Operation op3 = new Operation();
            op3.setName("DescribeCoverage");
                dcp = new DCP();
                    http = new HTTP();
					/*	
								// What is the URL for SOAP ? 
                        soap = new RequestMethodType();
                        soap.setHref("http://localhost/SOAP/URL/for/GetCapabalities");
                    http.getGetOrPost().add(new JAXBElement<RequestMethodType>(new QName("Soap"), RequestMethodType.class, soap));
					*/
                        post = new RequestMethodType();
						post.setHref(SERVLET_URL);
                        // TODO: Add "Get" request type once KVP encoding is supported
                    http.getGetOrPost().add(new JAXBElement<RequestMethodType>(new QName("Post"), RequestMethodType.class, post));
                dcp.setHTTP(http);
            op3.getDCP().add(dcp);
                param = new DomainType();
                param.setName("Format");
                    vals = new AllowedValues();
                        val = new ValueType();
                        val.setValue("text/xml");
                    vals.getValueOrRange().add((Object)val);
                param.setAllowedValues(vals);
            op2.getConstraint().add(param);
        opmeta.getOperation().add(op3);

        output.setOperationsMetadata(opmeta);
    }

    /**
     * Builds the output node "Contents"
     * @throws java.sql.SQLException
     */
    private void buildField4() throws SQLException {
        Contents cont = new Contents();

        ResultSet set = meta.executeQuery("SELECT name FROM wcps_coverage;");
        while (set.next())
        {
            String covName = set.getString("name");
            CoverageSummaryType sum = new CoverageSummaryType();
            sum.setAbstract("Available coverage, fetched from the database");
            sum.setTitle("Coverage " + covName);

            // code holds the coverage value
            net.opengis.gml.v_3_1_1.CodeType code = new net.opengis.gml.v_3_1_1.CodeType();
            code.setValue(covName);
            // code is encapsulated in JAXBElement "jelem"
            JAXBElement<net.opengis.gml.v_3_1_1.CodeType> jelem =
                    new JAXBElement<net.opengis.gml.v_3_1_1.CodeType>(new QName("Identifier"),
                    net.opengis.gml.v_3_1_1.CodeType.class, code);
            jelem.setValue(code);
            // Insert "jelem" into this coveragesummary
            sum.getRest().add(jelem);

            // Add this CoverageSummary to the list of coverage summaries, to make up the Contents
            cont.getCoverageSummary().add(sum);
        }
        
        output.setContents(cont);
    }

    private void finishBuild() {
        output.setVersion("1.1.0");
    }
    
}
