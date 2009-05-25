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


package wcps.server.servlet;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

import org.xml.sax.InputSource;

//This is the servlet interface of WCPS. It mostly consists of sanity checks and initialization,
//the meat is onyl a few lines. The WCPS class does the actual work.

import wcps.server.core.CachedMetadataSource;
import wcps.server.core.DbMetadataSource;
import wcps.server.core.ProcessCoveragesRequest;
import wcps.server.core.WCPS;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;

//import syntaxParser.SyntaxErrorException;
import java.io.StringBufferInputStream;

import java.util.Iterator;
import java.util.List;
import java.util.Properties;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

//Note an important limitation: this will only return the first result if several are available. 
//The reason is that WCPS currently has no standardized way to return multiple byte streams to
//the user.
public class WCPSServlet extends HttpServlet
{
	private static final long serialVersionUID = 84786549L;
	private Properties dbParams;
	private DbMetadataSource metadataSource;
	private String rasdamanDatabase;
	private String rasdamanUrl;
	private WCPS wcps;

	public void init() throws ServletException
	{
		try
		{
			dbParams = new Properties();
			System.out.println("WCPS: loading database properties");
			dbParams.load(
			    new FileInputStream(
				getServletContext().getRealPath("/dbparams.properties")));
			rasdamanUrl      = dbParams.getProperty("rasdaman_url");
			rasdamanDatabase = dbParams.getProperty("rasdaman_database");

			System.out.println("WCPS: initializing metadata database");
			metadataSource =
				new DbMetadataSource(dbParams.getProperty("metadata_driver"),
						     dbParams.getProperty("metadata_url"),
						     dbParams.getProperty("metadata_user"),
						     dbParams.getProperty("metadata_pass"), false);

			System.out.println("WCPS: initializing WCPS core");
			wcps = new WCPS(
                        new File(getServletContext().getRealPath("/xml/ogc/wcps/1.0.0/wcpsProcessCoverages.xsd")),
                        new CachedMetadataSource(metadataSource));

			System.out.println("WCPS: initialization complete");
		}
		catch (Exception e)
		{
    			System.out.println("WCPS: initialization error");
			System.out.println("WCPS: closing metadata database");

			if (metadataSource != null)
			{
				metadataSource.close();
			}

			System.out.println("WCPS: done with init error");
			throw new ServletException("WCPS initialization error", e);
		}

	}

	public void doGet(HttpServletRequest request, HttpServletResponse response)
	    throws ServletException, IOException
	{
		System.out.println("WCPS: invoked with GET");
		printUsage(response);

	}

	public void doPost(HttpServletRequest request, HttpServletResponse response)
	    throws ServletException, IOException
	{
		System.out.println("WCPS: invoked with POST");
		OutputStream webOut = null;

		try
		{
			String xmlRequest = null;

			if (ServletFileUpload.isMultipartContent(request))
			{
				@SuppressWarnings(
					"unchecked") Iterator<FileItem> fileItems =
						(Iterator<FileItem>) (new ServletFileUpload(
						    new DiskFileItemFactory())).parseRequest(
							request).iterator();

				if (!fileItems.hasNext())
				{
					throw new IOException(
					    "Multipart POST request contains no parts");
				}

				FileItem fileItem = fileItems.next();

				if (fileItems.hasNext())
				{
					throw new IOException(
					    "Multipart POST request contains too many parts");
				}

				if (!fileItem.isFormField()
				    && fileItem.getContentType().equals("text/xml"))
				{
					xmlRequest = fileItem.getString();
				}

				if (xmlRequest == null)
				{
					System.out.println(
					    "WCPS: no XML file was uploaded within multipart POST request");
					printUsage(response);
					return;
				}

				System.out.println(
				    "WCPS: received XML via a multipart POST request");
			}
			else
			{
				String xml   = request.getParameter("xml");
				String query = request.getParameter("query");

				if (xml != null)
				{
					System.out.println("WCPS: received XML via a 'xml' parameter in a POST request");
					xmlRequest = xml;
				}
				else if (query != null)
				{
					System.out.println("WCPS: received the following  query via a 'query' " +
                            "parameter in a POST request:");
					System.out.println(query);

                    xmlRequest = ProcessCoveragesRequest.abstractQueryToXmlQuery(query);
                    System.out.println("WCPS: transformed the abstract syntax query into an XML query!");
				}
				else
				{
					System.out.println("WCPS: no request was received");
					printUsage(response);
					return;
				}
			}

			System.out.println("WCPS: received the following request:");
			System.out.println(xmlRequest);

			System.out.println("WCPS: preparing request");
			ProcessCoveragesRequest processCoverageRequest =
				wcps.pcPrepare(
				    rasdamanUrl, rasdamanDatabase,
				    new InputSource(new StringBufferInputStream(xmlRequest)));

			System.out.println("[" + processCoverageRequest.getMime() + "] "
					   + processCoverageRequest.getRasqlQuery());

			String query = processCoverageRequest.getRasqlQuery();

			System.out.println("Resulting RasQL query: " + query);
			String mimetype = processCoverageRequest.getMime();

			System.out.println("WCPS: executing request");

			List<byte[]> results = processCoverageRequest.execute();

			System.out.println("WCPS: setting response mimetype to " + mimetype);
			response.setContentType(mimetype);
			System.out.println("WCPS: returning response");
			webOut = response.getOutputStream();
			webOut.write(results.get(0));
			System.out.println("WCPS: done");
		}
		catch (Exception e)
		{
			printError(response, "Error: " + e.getMessage(), e);
		}
		finally
		{
			if (webOut != null)
			{
				try
				{
					webOut.close();
				}
				catch (IOException e) {}
			}
		}
	}

	public void destroy()
	{
		super.destroy();

	}

	public String getServletInfo()
	{
		return "Web Coverage Processing Service";

	}

	private void printError(HttpServletResponse response, String message, Exception e)
	    throws IOException
	{
		System.out.println("WCPS: error");
		System.out.println("WCPS: setting response mimetype to text/html; charset=utf-8");
		response.setContentType("text/html; charset=utf-8");
		System.out.println("WCPS: returning the following error message");
		e.printStackTrace(System.out);
		System.out.println("WCPS: end of error message");
		PrintWriter out = new PrintWriter(response.getOutputStream());

		out.println(
		    "<html><head><title>Web Coverage Processing Service</title></head><body>");
		out.println("<h1>An error has occured</h1>");
		out.println("<p>" + message + "</p>");
		out.println("<p>Stack trace:<br/><small>");
		e.printStackTrace(out);
		out.println("</small></p></body></html>");
		out.close();
		System.out.println("WCPS: done with error");

	}

	private void printUsage(HttpServletResponse response) throws IOException
	{
		System.out.println("WCPS: setting response mimetype to text/html; charset=utf-8");
		System.out.println("WCPS: returning usage message");
		response.setContentType("text/html; charset=utf-8");
		PrintWriter out = new PrintWriter(response.getOutputStream());

		out.println(
		    "<html><head><title>Web Coverage Processing Service</title></head><body>");
		out.println("<h1>Hello</h1>");
		out.println("<p>There are 3 ways to invoke this service:</p>");
		out.println(
		    "<p>1. Upload a ProcessCoverage XML request as a multupart/form-data POST request containing a file. You can use the form below.</p>");
		out.println(
		    "<form action=\"\" method=\"post\" enctype=\"multipart/form-data\"><input type=\"file\" accept=\"text/xml\" size=\"64\" name=\"xmlfile\"/> <input type=\"submit\" value=\"Send\"/></form>");
		out.println(
		    "<p>2. Enter a ProcessCoverage XML request and submit it as a POST request with a parameter named <b>xml</b>. You can use the form below.</p>");
		out.println(
		    "<form action=\"\" method=\"post\"><textarea cols=\"64\" rows=\"16\" name=\"xml\"></textarea><input type=\"submit\" value=\"Send\"/></form>");
		out.println(
		    "<p>3. Enter a ProcessCoverage request in WCPS abstract syntax and submit it as a POST request with a parameter named <b>query</b>. You can use the form below.</p>");
		out.println(
		    "<form action=\"\" method=\"post\"><textarea cols=\"64\" rows=\"4\" name=\"query\"></textarea><input type=\"submit\" value=\"Send\"/></form>");
		out.println("</body></html>");
		out.close();
		System.out.println("WCPS: done nothing");

	}

	void FormatSyntaxException(Exception e, HttpServletResponse response, String query)
	    throws IOException
	{
		int column = 1;

		response.setContentType("text/html; charset=utf-8");
		PrintWriter o = new PrintWriter(response.getOutputStream());

		o.println(
		    "<html><head><title>Web Coverage Processing Service</title></head><body>");
		o.println("<p>There seems to be a syntax problem with your query:</p>");
		o.println("<p><font color='00FF00'>" + query.substring(0, column)
			  + "</font><font color='FF0000'>" + query.substring(column)
			  + "</font></p>");
		o.println("<p>Please correct it and try again.</p></body></html>");
		o.close();
		System.out.println("WCPS: Syntax Error reported");
	}
}
