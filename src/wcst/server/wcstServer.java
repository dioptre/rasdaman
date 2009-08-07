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


package wcst.server;

//~--- non-JDK imports --------------------------------------------------------

import net.opengis.ows.v_1_0_0.ExceptionReport;

import wcst.transaction.ServiceFirewall;
import wcst.transaction.WCSTException;
import wcst.transaction.executeAsyncTransaction;
import wcst.transaction.executeTransaction;
import wcst.transaction.schema.AcknowledgementType;
import wcst.transaction.schema.TransactionResponseType;
import wcst.transaction.schema.TransactionType;

//~--- JDK imports ------------------------------------------------------------

import java.io.StringReader;
import java.io.StringWriter;

import java.util.Date;
import java.util.GregorianCalendar;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.datatype.DatatypeConfigurationException;
import javax.xml.datatype.DatatypeFactory;
import javax.xml.datatype.XMLGregorianCalendar;
import javax.xml.namespace.QName;

/**
 * The Web Coverage Service, with the Transactional extension (wcstServer)
 *
 * @author Andrei Aiordachioaie
 */
public class wcstServer
{
	private static Boolean finished;
	private static Boolean printLog;
	private boolean synchronous = true;
	private String metaDbPath;
	private String responseHandler;

	/**
	 * Public minimal constructor
	 */
	public wcstServer(String metadataBasePath)
	{
		printLog = ConfigManager.PRINT_LOG;
		finished = false;
		this.metaDbPath = metadataBasePath;
	}

	/**
	 * Record a log message
	 * @param msg Text of the message
	 */
	public static void log(String msg)
	{
		if ( printLog )
			System.err.println("WCS-T: " + msg);
	}

	/**
	 * Web service operation
	 */
	public String Transaction(String stringXml)
	{
		// Actual contents of these two strings do not matter
		String output = "Default output. ";
		String errmsg = "No error. ";

		finished = false;
		try
		{
			try
			{
				// Check if Firewall allows the query
				if ( ServiceFirewall.reject(stringXml) )
					throw new WCSTException(
						"MaliciousQuery",
						"WCS-T Service Firewall refused to run possibly malitious query.");

				// read the input XML
				log("Reading the input XML file ... ");
				JAXBContext context = JAXBContext.newInstance(
					TransactionType.class.getPackage().getName());
				Unmarshaller unmarshaller = context.createUnmarshaller();
				Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

				if ( xml instanceof JAXBElement )
					xml = ((JAXBElement) xml).getValue();

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
				if ( input.getResponseHandler() == null )
				{
					synchronous = true;
					responseHandler = null;
				}
				else
				{
					synchronous = false;
					responseHandler = input.getResponseHandler();
				}

				executeTransaction exec = new executeTransaction(input, metaDbPath);

				/** Synchronous operation */
				if ( synchronous == true )
				{
					TransactionResponseType response = exec.get();
					JAXBElement jaxbOutput = new JAXBElement(new QName("", "TransactionResponse"),
						TransactionResponseType.class, response);

					// Write the output xml to a string
					JAXBContext jaxbCtx = JAXBContext.newInstance(
						response.getClass().getPackage().getName());
					Marshaller marshaller = jaxbCtx.createMarshaller();

					marshaller.setProperty(Marshaller.JAXB_ENCODING, "UTF-8");
					marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
					StringWriter strWriter = new StringWriter();

					marshaller.marshal(jaxbOutput, strWriter);
					output = strWriter.toString();
					log("Done! User has the TransactionResponseType result !");
				}
				else

				/** Asynchronous operation */
				{
					/* (1) Create acknowledgement that we received the request */
					AcknowledgementType ack = new AcknowledgementType();
					GregorianCalendar c = new GregorianCalendar();

					c.setTime(new Date());
					XMLGregorianCalendar date2 = DatatypeFactory.newInstance()
						.newXMLGregorianCalendar(c);

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
					log("Created the acknowledgement of the request !");
					log("Now starting to asynchronously execute the transaction...");

					/* (3) Start asynchronous processing */
					executeAsyncTransaction execAsync = new executeAsyncTransaction(exec,
						responseHandler);

					execAsync.start();
				}
				finished = true;

			}
			catch (JAXBException e)
			{
				e.printStackTrace();

				throw new WCSTException("XmlStructuresError",
										"Could not marshall/unmarshall XML structures.");
			}
			catch (DatatypeConfigurationException e)
			{
				e.printStackTrace();

				throw new WCSTException("XmlStructuresError",
										"Could not build request acknowledgement. ");
			}
			catch (WCSTException e)
			{
				throw e;
			}
		}
		catch (WCSTException e)
		{
			log("Caught WCST Exception");
			ExceptionReport report = e.getReport();

			try
			{
				JAXBContext jaxbCtx = JAXBContext.newInstance(
					report.getClass().getPackage().getName());
				Marshaller marshaller = jaxbCtx.createMarshaller();

				marshaller.setProperty(Marshaller.JAXB_ENCODING, "UTF-8");
				marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
				StringWriter strWriter = new StringWriter();

				marshaller.marshal(report, strWriter);
				output = strWriter.toString();
				finished = true;
				error("WCS-T Exception: " + e.getErrorCode() + ", with message '"
					  + e.getErrorDetail() + "'");
				log("Done with the Error Report !");
			}
			catch (JAXBException e2)
			{
				e2.printStackTrace();
				errmsg = e2.getMessage();
				error("Could not build XML error report.");
			}
		}

		if ( finished == false )
			output = errmsg;

		return output;
	}

	private void error(String msg)
	{
		System.err.println("WCS-T:" + msg);
	}
}
