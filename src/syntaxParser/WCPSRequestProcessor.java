/*
 * This file is part of Petascope.
 *
 * Petascope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Petascope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Petascope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.Petascope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package syntaxParser;

/**
 * WCPSRequestProcessor = Sample source code which makes use of the generated parser to parse a WCPS request stored in script file (request.wcps)
 * The result XML ProcessCoverage file is stored in the file wcpsRequest.xml
 */

import java.io.*;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.*;
import javax.xml.parsers.*;
import javax.xml.transform.dom.*;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.*;

class WCPSRequestProcessor {

	public static void main(String[] args){
		try {
			File inputFile = new File ("request.wcps");
			WCPSParser parser= new WCPSParser(new WCPSScanner(new FileInputStream(inputFile)));
			WCPSRequest pm= (WCPSRequest)parser.parse().value;
			String xmlString=pm.toXML();

			TransformerFactory factory = TransformerFactory.newInstance();
			factory.setAttribute("indent-number", new Integer(4));
			DocumentBuilder docBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
			Document doc = docBuilder.parse(new ByteArrayInputStream(xmlString.getBytes()));
			Transformer transformer = factory.newTransformer();
			transformer.setOutputProperty("omit-xml-declaration","no");
			transformer.setOutputProperty(OutputKeys.METHOD, "xml");
			transformer.setOutputProperty(OutputKeys.INDENT, "yes");
			StringWriter sw = new StringWriter();
			transformer.transform(new DOMSource(doc), new StreamResult(sw));
			String formattedXML = sw.toString();

			FileWriter fstream = new FileWriter("wcpsRequest.xml");
			BufferedWriter out = new BufferedWriter(fstream);
			out.write(formattedXML);
			out.close();	    		

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
