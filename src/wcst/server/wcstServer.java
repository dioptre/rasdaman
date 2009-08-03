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

import wcst.server.core.TransactionResponseType;
import wcst.server.core.TransactionType;

import wcst.transaction.WCSException;
import wcst.transaction.executeTransaction;

//~--- JDK imports ------------------------------------------------------------

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringReader;
import java.io.StringWriter;

import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;

import java.util.Date;
import java.util.GregorianCalendar;
import javax.jws.WebMethod;
import javax.jws.WebParam;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.datatype.DatatypeFactory;
import javax.xml.datatype.XMLGregorianCalendar;
import javax.xml.namespace.QName;
import wcst.server.core.AcknowledgementType;
import wcst.transaction.ServiceFirewall;
import wcst.transaction.executeAsyncTransaction;

/**
 * The Web Coverage Service, with the Transactional extension (wcstServer)
 *
 * @author Andrei Aiordachioaie
 */
public class wcstServer
{
	private static Boolean ok;
	private static Boolean printLog;
	private static Boolean printOutput;
	private String PetascopeURL = "http://kahlua.eecs.jacobs-university.de:8080/petascope-new/";
	private String metaDbPath = "/home/andrei/NetBeansProjects/WCS-Transaction/dbparams.properties";
    private boolean synchroneous = true;
    private String responseHandler;

	/**
	 * Public minimal constructor
	 */
	public wcstServer()
	{
		printLog = true;
		printOutput = true;
		ok = true;
	}

	/**
	 * Record a log message
	 * @param msg Text of the message
	 */
	public static void log(String msg)
	{
		if ( printLog )
			System.err.println(msg);
	}

	/**
	 * Send an XML request to the WCPS server. Hopefully it will succeed.
	 * Returns a message on error or null otherwise.
	 */
	private String sendXmlPostRequestToWcps(String param, String query)
            throws MalformedURLException, IOException
	{
//      System.out.println("--------------------");
//      System.out.println(query);
//      System.out.println("\t--------------------");

		// connect to the servlet
		URL servlet = new URL(PetascopeURL);
		HttpURLConnection conn = (HttpURLConnection) servlet.openConnection();

		// inform the connection that we will send output and accept input
		conn.setDoInput(true);
		conn.setDoOutput(true);

		// Don't use a cached version of URL connection.
		conn.setUseCaches(false);
		conn.setDefaultUseCaches(false);

		// Send POST request
		conn.setRequestMethod("POST");

		// Specify the content type that we will send binary data
		conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");

		String data = param + "=" + URLEncoder.encode(query);
		DataOutputStream out = new DataOutputStream(conn.getOutputStream());

		out.writeBytes(data);
		out.flush();
		out.close();

		BufferedReader cgiOutput = new BufferedReader(new InputStreamReader(conn.getInputStream()));
		String line1 = cgiOutput.readLine();
		String line2 = cgiOutput.readLine();
		String line3 = cgiOutput.readLine();

		System.out.println("\t" + line1);
		System.out.println("\t" + line2);
		System.out.println("\t" + line3);

		if ( (line1 != null) && (line2 != null) && (line3 != null) && line2.equals("<h1>An error has occured</h1>") )
		{
			while (cgiOutput.ready())
				System.out.println("\t" + cgiOutput.readLine());
//          System.out.println("Error executing query: ");
			String error = line3.substring(10, line3.length() - 4);

//          System.out.println("\t" + error);
			return error;
		}
		else
			return null;

	}

	/**
	 * Web service operation
	 */
	@WebMethod(operationName = "Transaction")
	public String Transaction(@WebParam(name = "xml") String stringXml)
	{
		String output = "Default output. ";
		String errmsg = "No error. ";

		ok = false;

		try
		{
            // Check if Firewall allows the query
            if (ServiceFirewall.reject(stringXml))
                throw new WCSException("MalitiousQuery", "WCS-T Service Firewall refused to run possibly malitious query.");
			// read the input XML
			log(">>> Reading the input XML file ... ");
			JAXBContext context = JAXBContext.newInstance(TransactionType.class.getPackage().getName());
			Unmarshaller unmarshaller = context.createUnmarshaller();
			Object xml = unmarshaller.unmarshal(new StringReader(stringXml));

			if ( xml instanceof JAXBElement )
				xml = ((JAXBElement) xml).getValue();

			log(">>> It is a " + xml.getClass().getSimpleName() + " request.");

			log(">>> Executing the proper class for this request: executeTransaction ...");
			TransactionType input = (TransactionType) xml;

            /** We can work in both asynchroneous and synchroneous mode.
             *
             * If we are in async mode, we generate an acknowledgement that
             * we have received the Transaction request. Next, when we finish processing,
             * we send the output (as a String) to the specified responseHandler.
             * 
             * In synchroneous mode, we just do the processing and output
             * the result.
             */
            if (input.getResponseHandler() == null)
            {
                synchroneous = true;
                responseHandler = null;
            }
            else
            {
                synchroneous = false;
                responseHandler = input.getResponseHandler();
            }

            executeTransaction exec = new executeTransaction(input, metaDbPath);

            /** Synchroneous operation */
            if (synchroneous == true)
            {
                TransactionResponseType response = exec.get();
                JAXBElement jaxbOutput = new JAXBElement(new QName("", "TransactionResponse"), TransactionResponseType.class, response);

                // Write the output xml to a string
                JAXBContext jaxbCtx = JAXBContext.newInstance(response.getClass().getPackage().getName());
                Marshaller marshaller = jaxbCtx.createMarshaller();

                marshaller.setProperty(Marshaller.JAXB_ENCODING, "UTF-8");
                marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
                StringWriter strWriter = new StringWriter();

                marshaller.marshal(jaxbOutput, strWriter);
                output = strWriter.toString();
                log(">>> Done! User has the TransactionResponseType result !");
            }
            else    /** Asynchroneous operation */
            {
                /* (1) Create acknowledgement that we received the request */
                AcknowledgementType ack = new AcknowledgementType();
                GregorianCalendar c = new GregorianCalendar();
                c.setTime(new Date());
                XMLGregorianCalendar date2 = DatatypeFactory.newInstance().newXMLGregorianCalendar(c);
                ack.setTimeStamp(date2);
                ack.setOperationRequest(input);
                ack.setRequestId(exec.generateRequestId());
                JAXBElement jaxbOutput = new JAXBElement(new QName("", "Acknowledgement"), TransactionResponseType.class, ack);

                /* (2) Write the acknowledgement to a string */
                JAXBContext jaxbCtx = JAXBContext.newInstance(ack.getClass().getPackage().getName());
                Marshaller marshaller = jaxbCtx.createMarshaller();
                marshaller.setProperty(Marshaller.JAXB_ENCODING, "UTF-8"); //NOI18N
                marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
                StringWriter strWriter = new StringWriter();

                marshaller.marshal(jaxbOutput, strWriter);
                output = strWriter.toString();
                log(">>> Created the acknowledgement of the request !");
                log(">>> Now starting to asynchroneously execute the transaction...");

                /* (3) Start asynchroneous processing */
                executeAsyncTransaction execAsync = new executeAsyncTransaction(exec, responseHandler);
                execAsync.start();
            }
			ok = true;

		}
		catch (WCSException e)
		{
			ExceptionReport report = e.getReport();

			try
			{
				JAXBContext jaxbCtx = JAXBContext.newInstance(report.getClass().getPackage().getName());
				Marshaller marshaller = jaxbCtx.createMarshaller();

				marshaller.setProperty(Marshaller.JAXB_ENCODING, "UTF-8");
				marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
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
				log("ERROR 8");
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
			errmsg = e.toString();
			log("ERROR 9");
		}

		if ( ok == false )
			output = errmsg;
		if ( printOutput )
			System.out.println("*****************\nOUTPUT IS \n" + output);

		return output;
	}
}
