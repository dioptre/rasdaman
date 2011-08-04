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
package petascope.wcst.server;

import petascope.ConfigManager;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import petascope.exceptions.PetascopeException;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCPSException;
import petascope.exceptions.WCSTException;
import petascope.wcst.transaction.ServiceFirewall;
import petascope.wcst.transaction.executeAsyncTransaction;
import petascope.wcst.transaction.executeTransaction;
import wcst.transaction.schema.AcknowledgementType;
import wcst.transaction.schema.TransactionResponseType;
import wcst.transaction.schema.TransactionType;
import java.io.StringReader;
import java.io.StringWriter;
import java.util.Date;
import java.util.GregorianCalendar;
import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.datatype.DatatypeConfigurationException;
import javax.xml.datatype.DatatypeFactory;
import javax.xml.datatype.XMLGregorianCalendar;
import javax.xml.namespace.QName;
import javax.xml.stream.XMLOutputFactory;
import javax.xml.stream.XMLStreamWriter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.PetascopeXmlNamespaceMapper;
import petascope.core.DbMetadataSource;
import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;

/**
 * The Web Coverage Service, with the Transactional extension (WcstServer)
 *
 * @author Andrei Aiordachioaie
 */
public class WcstServer {

    private static Logger log = LoggerFactory.getLogger(WcstServer.class);
    private static Boolean finished;
    private boolean synchronous = true;
    private String responseHandler;
    private DbMetadataSource meta;

    /**
     * Public minimal constructor
     */
    public WcstServer(DbMetadataSource source) throws WCSException {
        finished = false;
        meta = source;

        /* Check user settings */
        String cfg = ConfigManager.WCST_DEFAULT_DATATYPE;
        if (source.getDataTypes().contains(cfg) == false) {
            throw new WCSException(ExceptionCode.InvalidPropertyValue, "The following setting is not a valid datatype: " + cfg);
        }
        cfg = ConfigManager.WCST_DEFAULT_INTERPOLATION;
        if (source.getInterpolationTypes().contains(cfg) == false) {
            throw new WCSException(ExceptionCode.InvalidPropertyValue, "The following setting is not a valid interpolation method: " + cfg);
        }
        cfg = ConfigManager.WCST_DEFAULT_NULL_RESISTANCE;
        if (source.getNullResistances().contains(cfg) == false) {
            throw new WCSException(ExceptionCode.InvalidPropertyValue, "The following setting is not a valid null resistance: " + cfg);
        }
    }

    /**
     * Web service operation
     */
    public String Transaction(String stringXml) throws WCSTException, RasdamanException, WCPSException, PetascopeException {
        // Actual contents of these two strings do not matter
        String output = "Default output. ";
        String errmsg = "No error. ";

        finished = false;
        try {
            try {
                // Check if Firewall allows the query
                if (ServiceFirewall.reject(stringXml)) {
                    throw new WCSException(ExceptionCode.MaliciousQuery, "WCS-T Service Firewall "
                            + "refused to run possibly malitious query.");
                }

                // read the input XML
                log.debug("Reading the input XML file ... ");
                JAXBContext context = JAXBContext.newInstance(
                        TransactionType.class.getPackage().getName());
                Unmarshaller unmarshaller = context.createUnmarshaller();
                Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

                if (xml instanceof JAXBElement) {
                    xml = ((JAXBElement) xml).getValue();
                }

                TransactionType input = (TransactionType) xml;

                /**
                 * We can work in both asynchronous and synchronous mode.
                 *
                 * If we are in async mode, we generate an acknowledgement that
                 * we have received the Transaction request. Next, when we finish processing,
                 * we send the output (as a String) to the specified responseHandler.
                 *
                 * In synchronous mode, we just do the processing and output
                 * the result.
                 */
                if (input.getResponseHandler() == null) {
                    synchronous = true;
                    responseHandler = null;
                } else {
                    synchronous = false;
                    responseHandler = input.getResponseHandler();
                }

                executeTransaction exec = new executeTransaction(input, meta);

                /** Synchronous operation */
                if (synchronous == true) {
                    TransactionResponseType response = null;
                    exec.get();
                    JAXBElement jaxbOutput =
                            new JAXBElement(
                            new QName("http://www.opengis.net/wcs/1.1/wcst",
                            "TransactionResponse", XMLConstants.DEFAULT_NS_PREFIX),
                            TransactionResponseType.class, response);

                    // Write the output xml to a string
                    log.debug("Marshalling with context: " + response.getClass().getPackage().getName());
                    final StringWriter writer = new StringWriter();
                    try {
                        context = JAXBContext.newInstance(response.getClass());
                        final XMLStreamWriter xmlStreamWriter = XMLOutputFactory.newInstance().createXMLStreamWriter(writer);

                        final Marshaller marshaller = context.createMarshaller();
                        marshaller.setProperty("com.sun.xml.bind.namespacePrefixMapper", new PetascopeXmlNamespaceMapper());
                        marshaller.setProperty("jaxb.formatted.output", true);
                        marshaller.setProperty("jaxb.schemaLocation", "http://www.opengis.net/wcs/1.1/wcst http://schemas.opengis.net/wcst/ "
                                + "http://www.opengis.net/ows/1.1 http://schemas.opengis.net/ows/1.0.0/owsExceptionReport.xsd");
                        marshaller.marshal(jaxbOutput, xmlStreamWriter);
                    } catch (final Exception e) {
                        throw new RuntimeException(e.getMessage(), e);
                    }
                    output = writer.toString();
                    log.debug("Done! User has the TransactionResponse result !");
                } else /** Asynchronous operation */
                {
                    /* (1) Create acknowledgement that we received the request */
                    AcknowledgementType ack = new AcknowledgementType();
                    GregorianCalendar c = new GregorianCalendar();

                    c.setTime(new Date());
                    XMLGregorianCalendar date2 = DatatypeFactory.newInstance().newXMLGregorianCalendar(c);

                    ack.setTimeStamp(date2);
                    ack.setOperationRequest(input);
                    ack.setRequestId(exec.generateRequestId());
                    JAXBElement jaxbOutput = new JAXBElement(new QName("", "Acknowledgement"),
                            TransactionResponseType.class, ack);

                    /* (2) Write the acknowledgement to a string */
                    JAXBContext jaxbCtx = JAXBContext.newInstance(
                            ack.getClass().getPackage().getName());
                    Marshaller marshaller = jaxbCtx.createMarshaller();

                    marshaller.setProperty(Marshaller.JAXB_ENCODING, "UTF-8");
                    marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
                    StringWriter strWriter = new StringWriter();

                    marshaller.marshal(jaxbOutput, strWriter);
                    output = strWriter.toString();
                    log.debug("Created the acknowledgement of the request !");
                    log.debug("Now starting to asynchronously execute the transaction...");

                    /* (3) Start asynchronous processing */
                    executeAsyncTransaction execAsync = new executeAsyncTransaction(exec,
                            responseHandler);

                    execAsync.start();
                }
                finished = true;

            } catch (JAXBException e) {
                throw new WCSException(ExceptionCode.XmlStructuresError, "Could not marshall/unmarshall XML structures.", e);
            } catch (DatatypeConfigurationException e) {
                throw new WCSException(ExceptionCode.XmlStructuresError, "Could not build request acknowledgement. ", e);
            }
        } catch (WCSException e) {
            log.info("Caught WCST Exception");
            ExceptionReport report = e.getReport();

            try {
                JAXBContext jaxbCtx = JAXBContext.newInstance(
                        report.getClass().getPackage().getName());
                Marshaller marshaller = jaxbCtx.createMarshaller();

                marshaller.setProperty(Marshaller.JAXB_ENCODING, "UTF-8");
                marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
                StringWriter strWriter = new StringWriter();

                marshaller.marshal(report, strWriter);
                output = strWriter.toString();
                finished = true;
                log.error("WCS-T Exception: " + e.getExceptionCode() + ", with message '"
                        + e.getExceptionText() + "'");
                log.debug("Done with the Error Report !");
            } catch (JAXBException e2) {
                errmsg = e2.getMessage();
                log.error("Could not build XML error report. Stack trace: " + e2);
            }
        }

        if (finished == false) {
            output = errmsg;
        }

        return output;
    }
}
