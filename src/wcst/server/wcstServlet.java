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

import org.apache.commons.io.FileUtils;

//~--- JDK imports ------------------------------------------------------------

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;


import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/** Servlet interface for the WCS-Transactional server.
 *
 * @author Andrei Aiordachioaie
 */
public class wcstServlet extends HttpServlet
{
	private static final long serialVersionUID = 84786549L;
	private String defaultHtmlResponse;
	private String servletHtmlPath;
    private wcstServer server;

    @Override
	public void init() throws ServletException
	{
        // Initialize the configuration manager
        String settingsPath = getServletContext().getRealPath("/settings.properties");
        ConfigManager config = ConfigManager.getInstance(settingsPath);
        
        // Initialize the WCS-T server with proper metadata
        String metadataDbPath = getServletContext().getRealPath("/dbparams.properties");
        server = new wcstServer(metadataDbPath);

        // Load the servlet HTML response
		servletHtmlPath = getServletContext().getRealPath("/misc/wcst-servlet.html");
		try
		{
			defaultHtmlResponse = FileUtils.readFileToString(new File(servletHtmlPath));
		}
		catch (IOException e)
		{
			throw new ServletException(e.getMessage());
		}
	}

	/**
	 * Handles the HTTP <code>GET</code> method.
	 * @param request wcstServlet request
	 * @param response wcstServlet response
	 * @throws ServletException if a wcstServlet-specific error occurs
	 * @throws IOException if an I/O error occurs
	 */
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException
	{
		System.out.println("WCS-T: invoked with GET");
		printUsage(response);
	}

	/**
	 * Handles the HTTP <code>POST</code> method.
	 * @param request wcstServlet request
	 * @param response wcstServlet response
	 * @throws ServletException if a wcstServlet-specific error occurs
	 * @throws IOException if an I/O error occurs
	 */
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
	{
		// Select the operation
		int op = -1;

		if ( request.getParameter("TransactionXML") != null )
			op = 4;

		// Convert the WCS request into a WCPS request
		try
		{
			// initialize WebService operation arguments here
			String inputXml, outputXml;
			PrintWriter out;

			switch (op)
			{
			case 4:
				inputXml = request.getParameter("TransactionXML");
				outputXml = server.Transaction(inputXml);
				out = new PrintWriter(response.getOutputStream());
				out.write(outputXml);
				out.flush();

				break;
			default:
				throw new Exception("No valid operation specified !");
			}

		}
		catch (Exception e)
		{
			e.printStackTrace();

			throw new ServletException("WCS-T servlet error !", e);
		}
	}

	/**
	 * Returns a short description of the wcstServlet.
	 * @return a String containing wcstServlet description
	 */
	@Override
	public String getServletInfo()
	{
        return ConfigManager.SERVLET_INFO;
	}

	private void printUsage(HttpServletResponse response) throws IOException
	{
		System.out.println("WCS-T: returning usage message");
		response.setContentType("text/html; charset=utf-8");
		PrintWriter out = new PrintWriter(response.getOutputStream());

		out.println(defaultHtmlResponse);

		out.close();
		System.out.println("WCS-T: done nothing");

	}

}
