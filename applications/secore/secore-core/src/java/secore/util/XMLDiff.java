/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2012 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package secore.util;

import java.util.HashSet;
import java.util.Set;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.ErrorListener;
import javax.xml.transform.TransformerException;
import org.custommonkey.xmlunit.Diff;
import org.custommonkey.xmlunit.Difference;
import org.custommonkey.xmlunit.DifferenceConstants;
import org.custommonkey.xmlunit.DifferenceListener;
import org.custommonkey.xmlunit.XMLUnit;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Node;

/**
 * Specialized XML diff functionality, as comparing XML with string comparison
 * isn't good enough.
 *
 * @author Dimitar Misev
 */
public class XMLDiff {
  
  private static Logger log = LoggerFactory.getLogger(XMLDiff.class);
  
  private static final IgnoreNamedElementsDifferenceListener diffListener;

  private static final ErrorListener silentErrorListener = new ErrorListener() {
    public void warning(TransformerException exception) throws TransformerException {
    }

    public void error(TransformerException exception) throws TransformerException {
    }

    public void fatalError(TransformerException exception) throws TransformerException {
    }
  };

  /**
   * Setup the diff engine
   */
  static {
    diffListener = new IgnoreNamedElementsDifferenceListener();
    diffListener.addIgnoreAttribute("href", "id");
    diffListener.addIgnoreElements("identifier", "metaDataProperty", "scope");
    
    
    XMLUnit.setIgnoreDiffBetweenTextAndCDATA(true);
    XMLUnit.setIgnoreAttributeOrder(true);
    XMLUnit.setIgnoreComments(true);
    XMLUnit.setIgnoreWhitespace(true);
    XMLUnit.setNormalizeWhitespace(true);
//    XMLUnit.setNormalize(true);

    XMLUnit.getSAXParserFactory().setNamespaceAware(false);
    XMLUnit.getSAXParserFactory().setValidating(false);

    DocumentBuilderFactory factory = XMLUnit.getControlDocumentBuilderFactory();
    factory.setCoalescing(true);
    factory.setExpandEntityReferences(false);
    factory.setIgnoringComments(true);
    factory.setIgnoringElementContentWhitespace(true);
    factory.setNamespaceAware(false);
    factory.setValidating(false);
    XMLUnit.setControlDocumentBuilderFactory(factory);
    XMLUnit.setTestDocumentBuilderFactory(factory);
    XMLUnit.getTransformerFactory().setErrorListener(silentErrorListener);
  }

  /**
   * Perform a diff on two XML strings.
   *
   * @param xml1
   * @param xml2
   * @return true if same, false otherwise
   */
  public static Result equals(String xml1, String xml2) throws SecoreException {
    Diff diff = null;
    try {
      diff = new Diff(xml1, xml2);
      diff.overrideDifferenceListener(diffListener);
    } catch (Exception ex) {
      throw new SecoreException(ExceptionCode.XmlNotValid, "Failed comparing the XML definitions.", ex);
    }
    return new Result(diff);
  }
  
  /**
   * Wrapper for the comparison result, adds method to create the XML result
   */
  public static class Result {
    private final Diff diff;
    
    public Result(Diff diff) {
      this.diff = diff;
    }
    
    public String toXML() {
      boolean equal = diff.identical();
      
      String ret = "  <equal>" + equal + "</equal>";
      if (!equal) {
        StringBuffer buf = new StringBuffer();
        diff.appendMessage(buf);
        ret = ret + "\n  <reason>\n  <![CDATA[" + buf.toString() + "  ]]>\n  </reason>";
      }
      ret = "<comparisonResult xmlns='" + Constants.CRSNTS_NAMESPACE + "'>\n" + ret + "\n</comparisonResult>";
      return ret;
    }

    public Diff getDiff() {
      return diff;
    }
  }
}

/**
 * Custom difference listener, to allow ignoring irrelevant elements/attributes.
 * 
 * @author Dimitar Misev
 */
class IgnoreNamedElementsDifferenceListener implements DifferenceListener {
  
  private static Logger log = LoggerFactory.getLogger(IgnoreNamedElementsDifferenceListener.class);

  private Set<String> ignoreElements = new HashSet<String>();
  private Set<String> ignoreAttributes = new HashSet<String>();

  public int differenceFound(Difference difference) {
    if (difference.getId() == DifferenceConstants.TEXT_VALUE_ID) {
      String elName = difference.getControlNodeDetail().getNode().getParentNode().getNodeName();
      if (ignoreElements.contains(elName)) {
        log.trace("Ignoring element " + elName);
        return DifferenceListener.RETURN_IGNORE_DIFFERENCE_NODES_IDENTICAL;
      }
    } else if (difference.getId() == DifferenceConstants.ATTR_VALUE_ID) {
      String xpath = difference.getControlNodeDetail().getXpathLocation();
      String attName = xpath.substring(xpath.lastIndexOf("@") + 1);
      if (ignoreAttributes.contains(attName)) {
        log.trace("Ignoring attribute " + attName);
        return DifferenceListener.RETURN_IGNORE_DIFFERENCE_NODES_IDENTICAL;
      }
    }

    log.debug("Difference found: " + difference.getDescription());
    log.debug("           XPath: " + difference.getControlNodeDetail().getXpathLocation());
    
    return DifferenceListener.RETURN_ACCEPT_DIFFERENCE;
  }

  public void skippedComparison(Node node, Node node1) {
  }
  
  public void addIgnoreAttribute(String... attributeNames) {
    for (String att : attributeNames) {
      ignoreAttributes.add(att);
    }
  }
  
  public void addIgnoreElements(String... elementNames) {
    for (String el : elementNames) {
      ignoreElements.add(el);
    }
  }
}