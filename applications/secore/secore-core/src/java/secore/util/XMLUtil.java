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
 * Copyright 2003 - 2012 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package secore.util;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.logging.Level;
import java.util.logging.Logger;
import net.n3.nanoxml.IXMLElement;
import net.n3.nanoxml.IXMLParser;
import net.n3.nanoxml.IXMLReader;
import net.n3.nanoxml.StdXMLReader;
import net.n3.nanoxml.XMLException;
import net.n3.nanoxml.XMLParserFactory;
import net.n3.nanoxml.XMLWriter;
import org.slf4j.LoggerFactory;

/**
 * XML utilities.
 *
 * @author Dimitar Misev
 */
public class XMLUtil {
  
  private static org.slf4j.Logger log = LoggerFactory.getLogger(XMLUtil.class);

  public static IXMLElement parse(String xml) throws SecoreException {
    IXMLParser parser = null;
    try {
      parser = XMLParserFactory.createDefaultXMLParser();
    } catch (Exception ex) {
      log.error("Failed creating XML parser.");
      throw new SecoreException(ExceptionCode.InternalComponentError);
    }
    IXMLReader reader = StdXMLReader.stringReader(xml);
    parser.setReader(reader);
    try {
      return (IXMLElement) parser.parse();
    } catch (XMLException ex) {
      log.error("XML parsing failed.", ex);
      throw new SecoreException(ExceptionCode.XmlNotValid);
    }
  }
}
