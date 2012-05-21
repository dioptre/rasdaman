/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 * 
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Apr 4, 2008, 5:18:39 PM
 * 
 * Filename           $Id: XMLUtil.java 1976 2010-07-31 12:07:20Z dmisev $
 * Revision           $Revision: 1976 $
 * Last modified on   $Date: 2010-07-31 14:07:20 +0200 (Sat, 31 Jul 2010) $
 *               by   $Author: dmisev $
 * 
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU  Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */
package petascope.util;

import com.eaio.uuid.UUID;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.validation.SchemaFactory;
import nu.xom.Attribute;
import nu.xom.Builder;
import nu.xom.Comment;
import nu.xom.Document;
import nu.xom.Element;
import nu.xom.Node;
import nu.xom.Nodes;
import nu.xom.ParentNode;
import nu.xom.ParsingException;
import nu.xom.Text;
import nu.xom.XPathContext;
import nu.xom.converters.DOMConverter;
import org.xml.sax.EntityResolver;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.XMLReader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.xml.sax.ErrorHandler;
import petascope.util.traverse.DFSTraversor;
import petascope.util.traverse.Filter;
import petascope.util.traverse.TraversableXOM;

/**
 * Common utility methods for working with XML.
 * 
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class XMLUtil {

    private static Logger log = LoggerFactory.getLogger(XMLUtil.class);

    static class MyBuilder extends ThreadLocal<Builder> {

        boolean validating;

        public MyBuilder(boolean validating) {
            this.validating = validating;
        }

        @Override
        protected Builder initialValue() {
            factory.setValidating(validating);
            return newBuilder(!validating);
        }
    }
    /**
     * Separator, for debugging
     */
    public static String FSEP = "\n-------------------------------------\n";
    public static String HSEP = "\n-------------------\n";
    /**
     * Setup for building documents
     */
    private static SchemaFactory schemaFactory;
    private static SAXParserFactory factory;
    private static MyBuilder builder;
    private static File wcsSchema;
    public static final String XML_STD_ENCODING = "UTF-8";
    public static final String WCS_SCHEMA = "xml/ogc/wcs/2.0.0/wcsAll.xsd";

    static {
        init();
    }
    
    public static void init() {
        if (factory != null) {
            return;
        }
        System.setProperty("javax.xml.parsers.SAXParserFactory",
                "com.sun.org.apache.xerces.internal.jaxp.SAXParserFactoryImpl");
        factory = SAXParserFactory.newInstance();
        factory.setNamespaceAware(true);
        factory.setValidating(true);
        builder = new MyBuilder(false);
        builder.get();
    }

    private static Builder newBuilder(boolean ignoreDTD) {
//        if (schemaFactory == null) {
//            schemaFactory = SchemaFactory.newInstance("http://www.w3.org/2001/XMLSchema");
//            wcsSchema = IOUtil.findFile(WCS_SCHEMA, XMLUtil.class.getResource(""), -1);
//            if (wcsSchema == null) {
//                log.warn("WCS 2.0 XML schema not found, schema validity disabled.");
//            } else {
//                log.info("Found WCS 2.0 XML schema: " + wcsSchema.toString());
//                try {
//                    factory.setSchema(schemaFactory.newSchema(wcsSchema));
//                } catch (SAXException ex) {
//                    log.warn("Error instantiation new schema from file " + wcsSchema.toString(), ex);
//                }
//            }
//        }
        
        XMLReader xmlReader = null;
        try {
            xmlReader = factory.newSAXParser().getXMLReader();
            if (ignoreDTD) {
                xmlReader.setEntityResolver(new EntityResolver() {

                    public InputSource resolveEntity(String publicId, String systemId) throws SAXException, IOException {
                        return new InputSource(new StringReader(""));
                    }
                });
                xmlReader.setErrorHandler(new ErrorHandler() {

                    @Override
                    public void warning(SAXParseException saxpe) throws SAXException {
                        log.warn("XML parser warning: ", saxpe.getMessage());
                    }

                    @Override
                    public void error(SAXParseException saxpe) throws SAXException {
                        throw saxpe;
                    }

                    @Override
                    public void fatalError(SAXParseException saxpe) throws SAXException {
                        throw saxpe;
                    }
                });
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return new Builder(xmlReader);
    }

    /**
     * Build XOM Document from XML file.
     *
     * @param file input XML file
     * @return XOM Document
     * @throws IOException
     */
    public static Document buildDocument(File file) throws IOException, ParsingException {
        return buildDocument(file.toURI().toString(), new FileInputStream(file));
    }

    /**
     * Build XOM Document from an XML string.
     *
     * @param baseURI
     * @param document input XML string
     * @return XOM Document
     */
    public static Document buildDocument(String baseURI, String document) throws IOException, ParsingException {
        InputStream in = new ByteArrayInputStream(document.getBytes(XML_STD_ENCODING));
        return buildDocument(baseURI, in);
    }

    /**
     * Creates a {@link Document} given a input stream.
     * <p>
     * <i>Note</i>: If the input stream to parse contains a <code>DOCTYPE</code> definition, but the parser can't find the referenced <code>DTD</code>, then a
     * <code>FileNotFound</code> exception is raised discarding the parsing process.
     *
     * @param baseURI
     * @param in an input stream
     * @return the document
     */
    public static Document buildDocument(String baseURI, InputStream in) throws IOException, ParsingException {
        Document doc = null;

        try {
            doc = builder.get().build(in, baseURI);
        } catch (ParsingException ex) { 
            log.error(StringUtil.join("Error while building XML document: " + baseURI, ex.getMessage(),
                    "line: " + ex.getLineNumber() + ", column: " + ex.getColumnNumber()));
            throw ex;
        } catch (IOException ex) {
            log.error(StringUtil.join("Error while building XML document: " + baseURI,
                    "Error reading from the input stream.", ex.getMessage()));
            throw ex;
        } catch (Exception ex) {
            ex.printStackTrace();
            log.error(StringUtil.join("Error while building XML document: " + baseURI, ex.getMessage()));
            throw new RuntimeException("Error while building XML document: " + baseURI, ex);
        } finally {
            in.close();
        }

        if (baseURI != null && doc != null) {
            doc.setBaseURI(baseURI);
        }

        return doc;
    }

    /**
     * Build a collection of documents
     *
     * @param files the XML files to build
     * @return the Documents
     * @throws java.io.IOException
     */
    public static List<Document> buildDocuments(Collection<File> files) throws IOException, ParsingException {
        List<Document> ret = new ArrayList<Document>();
        for (File file : files) {
            ret.add(buildDocument(file));
        }
        return ret;
    }

    /**
     * Serialize a XOM Document.
     *
     * @param xomDocument the XOM Document to be serialized
     * @throws IOException
     */
    public static String serialize(Document xomDocument) throws IOException {
        return serialize(xomDocument, false);
    }

    /**
     * Serialize a XOM Document.
     *
     * @param xomDocument the XOM Document to be serialized
     * @throws IOException
     */
    public static String serialize(Document xomDocument, boolean noPrettyPrint) throws IOException {
        OutputStream os = new ByteArrayOutputStream();
        serialize(xomDocument, os, noPrettyPrint);
        return os.toString();
    }

    /**
     * Serialize a XOM Document.
     *
     * @param xomDocument the XOM Document to be serialized
     * @param os stream where to write the result
     * @throws IOException
     */
    public static void serialize(Document xomDocument, OutputStream os) throws IOException {
        serialize(xomDocument, os, false);
    }

    /**
     * Serialize a XOM Document without pretty printing the result.
     *
     * @param xomDocument the XOM Document to be serialized
     * @param os stream where to write the result
     * @throws IOException
     */
    public static void serialize(Document xomDocument, OutputStream os, boolean noPrettyPrint) throws IOException {
        nu.xom.Serializer serializer = new nu.xom.Serializer(os);
        if (!noPrettyPrint) {
            serializer.setIndent(2);
        }
        serializer.write(xomDocument);
    }

    /**
     * Serialize a XOM DOcument
     *
     * @param xomDocument the XOM Document to be serialized
     * @param file the file to which to serialize
     * @throws IOException
     */
    public static void serialize(Document xomDocument, File file) throws IOException {
        serialize(xomDocument, file, false);
    }

    /**
     * Serialize a XOM Document without pretty printing the result.
     *
     * @param xomDocument the XOM Document to be serialized
     * @throws IOException
     */
    public static void serialize(Document xomDocument, File file, boolean noPrettyPrint) throws IOException {
        serialize(xomDocument, new FileOutputStream(file), noPrettyPrint);
    }

    /**
     * @return the document name
     */
    public static String docName(Document doc) {
        if (doc == null || doc.getBaseURI() == null) {
            return null;
        }
        String ret = doc.getBaseURI();
        if (ret == null) {
            return null;
        }
        int ind = ret.lastIndexOf(File.separator);
        if (ind != -1) {
            ret = ret.substring(ind + 1);
        }
        return ret;
    }

    public static String getBaseURI(Element e) {
        if (e == null) {
            return null;
        }
        String ret = getBaseURI(e.getDocument());
        if (ret == null) {
            ParentNode p = e;
            while (true) {
                if (p == null) {
                    return null;
                }
                p = p.getParent();
            }
        }
        return ret;
    }

    public static String getBaseURI(Document doc) {
        if (doc != null) {
            return doc.getBaseURI();
        }
        return null;
    }

    /**
     * @return the depth of <code>e</code> in the XML tree
     */
    public static int depth(Element e) {
        int ret = 0;
        ParentNode p = e.getParent();
        while (p != null) {
            p = p.getParent();
            ++ret;
        }
        return ret;
    }

    /**
     * Shortcut method for creating a XOM Attribute in the XML namespace.
     *
     * @param name name of the attribute
     * @param value value of the attribute
     * @return the created attribute
     */
    public static Attribute createXMLAttribute(String name, String value) {
        return new Attribute(XMLSymbols.PREFIX_XML + ":" + name, XMLSymbols.NAMESPACE_XML, value);
    }

    /**
     * Deep copy one element to another.
     *
     * @param from the element to be copied
     * @param to the element to be replaced
     */
    public static void copyElement(Element from, Element to) {
        if (from == null || to == null) {
            return;
        }

        Element e = (Element) from.copy();
        to.removeChildren();
        while (to.getAttribute(0) != null) {
            to.removeAttribute(to.getAttribute(0));
        }
        while (e.getAttribute(0) != null) {
            Attribute a = e.getAttribute(0);
            a.detach();
            to.addAttribute(a);
        }
        if (e.getChildCount() > 0) {
            while (e.getChild(0) != null) {
                Node n = e.getChild(0);
                n.detach();
                to.appendChild(n);
            }
        }
        to.setLocalName(e.getLocalName());
        to.setBaseURI(e.getBaseURI());
        to.setNamespacePrefix(e.getNamespacePrefix());
        to.setNamespaceURI(e.getNamespaceURI());
    }

    /**
     * Return new element with the same name and attributes as the given element.
     * Note that we don't care about the prefix/namespaces here.
     *
     * @param ignoreAttributes set of attributes to ignore when copying
     */
    public static Element copyTag(Element e, Set<String> ignoreAttributes) {
        Element ret = null;
        if (e.getNamespacePrefix() != null) {
            ret = new Element(e.getQualifiedName(), e.getNamespaceURI());
        } else {
            ret = new Element(e.getLocalName());
        }
        for (int i = 0; i < e.getAttributeCount(); i++) {
            Attribute a = (Attribute) e.getAttribute(i).copy();
            if (ignoreAttributes != null && ignoreAttributes.contains(a.getLocalName())) {
                continue;
            }
            ret.addAttribute(a);
        }
        return ret;
    }

    /**
     * cp(<label att_1="val_1"...att_n="val_n">ch</label>) --> <label att_1="val_1"...att_n="val_n">ch'</label>,
     * where ch'!=Element
     */
    public static Node cp(Node n) {
        if (n instanceof Element) {
            Element e = (Element) n;
            Element ne = copyTag(e, null);
            if (firstChild(e) == null) {
                String v = e.getValue();
                if (!"".equals(v)) {
                    ne.appendChild(v);
                }
            }
            return ne;
        }

        if (!(n instanceof Text)) {
            n.detach();
            return n.copy();
        } else {
            return (Text) n.copy();
        }
    }

    public static boolean isFirstTag(String xml, String tag) {
        return tag.equals(getRootElementName(xml));
    }

    public static String getRootElementName(String xml) {
        int start = 0;
        while (start < xml.length()) {
            start = xml.indexOf("<", start);
            if (start == -1) {
                return null;
            }
            if (xml.charAt(start + 1) != '?') {
                int end = start + 1;
                while (end < xml.length() && xml.charAt(end) != ' ' && xml.charAt(end) != '>') {
                    if (xml.charAt(end) == ':') {
                        start = end;
                    }
                    ++end;
                }
                if (end == -1) {
                    return null;
                }
                ++start;
                return xml.substring(start, end);
            } else {
                ++start;
            }
        }
        return null;
    }

    public static String removeXmlDecl(String xml) {
        if (xml.startsWith("<?xml")) {
            xml = xml.substring(xml.indexOf("<", 1));
        }
        return xml;
    }

    /**
     * Return the text that some node contains.
     */
    public static String getText(Element node) {
        if (node == null) {
            return null;
        }
        StringBuilder ret = new StringBuilder();
        for (int i = 0; i < node.getChildCount(); i++) {
            ret.append(node.getChild(i).toXML());
        }
        return ret.toString().trim();
    }

    public static String getXMLID(Element e) {
        if (e == null) {
            return null;
        }
        String ret = e.getAttributeValue("id", XMLSymbols.NAMESPACE_XML);
        if (ret == null) {
            ret = e.getAttributeValue("id");
        }
        return ret;
    }

    public static String getXMLIDOrName(Element e) {
        if (e == null) {
            return null;
        }
        String ret = getXMLID(e);
        if (ret == null) {
            ret = e.getAttributeValue("name");
        }
        return ret;
    }

    /**
     * Collect all children of <code>e</code> with the given names.
     *
     * @return a list of the collected elements
     */
    public static List<Element> collectAll(Element e, String... names) {
        return collectAllExcept(e, null, true, names);
    }

    public static List<Element> collectAllExcept(Element e, String except, boolean recurse, String... names) {
        List<Element> ret = new ArrayList<Element>();
        Set<String> namesSet = new HashSet<String>(Arrays.asList(names));
        collectAllExcept(ret, e, except, recurse, namesSet);
        return ret;
    }

    private static void collectAllExcept(List<Element> ret, Element e, String except, boolean recurse, Set<String> names) {
        String elname = e.getLocalName();
        if (names.contains(elname)) {
            ret.add(e);
            if (!recurse) {
                return;
            }
        }
        int n = e.getChildCount();
        for (int i = 0; i < n; i++) {
            Node node = e.getChild(i);
            if (node instanceof Element && !((Element) node).getLocalName().equals(except)) {
                collectAllExcept(ret, (Element) node, except, recurse, names);
            }
        }
    }

    /**
     * Collect all children of <code>e</code> with the given name.
     *
     * @return a list of the collected elements
     */
    public static List<Element> collectAll(Element e, String prefix, String name, XPathContext ctx) {
        List<Element> ret = new ArrayList<Element>();
        Nodes notations = null;
        if (ctx != null) {
            if (prefix != null) {
                notations = e.query("//" + prefix + ":" + name, ctx);
            } else {
                notations = e.query("//" + name, ctx);
            }
        }
        if (notations != null) {
            for (int i = 0; i < notations.size(); i++) {
                ret.add((Element) notations.get(i));
            }
        }
        return ret;
    }

    /**
     * @return the first child of <code>e</code> with the given <code>id</code>
     * when searched with DFS.
     */
    public static Element childWithId(Element e, String id) {
        String value = getXMLIDOrName(e);
        if (value != null && value.equals(id)) {
            return e;
        } else {
            int n = e.getChildCount();
            Element ret = null;
            for (int i = 0; i < n; i++) {
                Node c = e.getChild(i);
                if (c instanceof Element) {
                    ret = childWithId((Element) c, id);
                    if (ret != null) {
                        return ret;
                    }
                }
            }
            return ret;
        }
    }

    /**
     * Collect all children of <code>e</code> for which xml:id=id.
     *
     * @return a list of the collected elements
     */
    public static List<Element> collectAllWithID(Element e, final String id) {
        DFSTraversor traversor = new DFSTraversor(new Filter() {

            public boolean evaluate(Object node, int depth) {
                if (node instanceof Element) {
                    Element x = (Element) node;
                    String value = getXMLID(x);
                    if (value == null) {
                        value = x.getAttributeValue("name");
                    }
                    if (value != null && value.equals(id)) {
                        return true;
                    }
                }
                return false;
            }
        });

        List<Object> res = traversor.traverse(new TraversableXOM(e));
        return ListUtil.<Element, Object>cast(res);
    }

    /**
     * Collect all children of <code>e</code> for which xml:id=id.
     *
     * @return a list of the collected elements
     */
    public static Element collectFirstWithID(Element e, final String id) {
        DFSTraversor traversor = new DFSTraversor(new Filter() {

            public boolean evaluate(Object node, int depth) {
                if (node instanceof Element) {
                    Element x = (Element) node;
                    String value = getXMLID(x);
                    if (value == null) {
                        value = x.getAttributeValue("name");
                    }
                    if (value != null && value.equals(id)) {
                        return true;
                    }
                }
                return false;
            }
        }, 1);

        List<Object> res = traversor.traverse(new TraversableXOM(e));
        if (res.isEmpty()) {
            return null;
        } else {
            return (Element) res.get(0);
        }
    }

    /**
     * Replaces an <code>o</code> with a new child node. If <code>o</code> does not have a parent node, then a <code>NoSuchChildException</code> is thrown.
     *
     * @param <T> the type parameter to specify the node type
     * @param o the old node
     * @param n the new node
     * @return the new node
     */
    public static <T extends Node> T substitute(T o, T n) {
        ParentNode p = o.getParent();
        if (p == null) {
            return n;
        }
        if (n != null) {
            n.detach();
            p.replaceChild(o, n);
        } else {
            p.removeChild(o);
        }
        return n;
    }

    /**
     * Remove all qualifiers in an XML code.
     *
     * @param xml the XML document
     * @return the same document with unqualified element names
     */
    public static String removeQualifiers(String xml) {
        // remove from elements
        xml = xml.replaceAll("<[^/:> ]+:", "<").replaceAll("</[^:> ]+:", "</");
        // remove namespace declarations
        xml = xml.replaceAll("xmlns:\\w+ *= *\"[^\"]*\"", "");
        // remove from attributes
        xml = xml.replaceAll("\" *\\w+ *: *(\\w+ *= *\")", "\" $1");
        xml = xml.replaceAll("(<\\w+ +)\\w+ *: *(\\w+ *= *\")", "$1$2");
        return xml;
    }

    /**
     * Remove all attributes from the given <code>xml</code> string
     */
    public static String removeAttributes(String xml, String[] attributes) {
        for (String att : attributes) {
            xml = xml.replaceAll(" *(\\w+:)?" + att + " *= *\"[^\"]*\" *", " ");
        }
        return xml;
    }

    /**
     * Wrap the given list of nodes in a root element
     *
     * @param n nodes to wrap
     * @param copy whether to append copies of the nodes. If false then the given nodes are detached!
     * @param prefix root element prefix
     * @param name root element name
     * @param namespace root element namespace
     * @return the root element
     */
    public static <T extends Node> Element wrap(boolean copy, String prefix, String name, String namespace, T... n) {
        Element ret = new Element(prefix + ":" + name, namespace);
        if (copy) {
            Node x = null;
            for (Node node : n) {
                x = node.copy();
                x.detach();
                ret.appendChild(node);
            }
        } else {
            for (Node node : n) {
                node.detach();
                ret.appendChild(node);
            }
        }
        return ret;
    }

    /**
     * Retrieve a type 4 (pseudo randomly generated) UUID.
     *
     * @return a randomly generated UUID.
     */
    public static String newUUID() {
        return "a" + (new UUID());
    }

    /**
     * Get the first child <code>Element</code> of <code>e</code>
     */
    public static Element firstChild(Element e) {
        return firstChild(e, null);
    }

    public static Element firstChild(Element e, String name) {
        Node n = null;
        for (int i = 0; i < e.getChildCount(); i++) {
            n = e.getChild(i);
            if (n instanceof Element) {
                if (name == null || ((Element) n).getLocalName().equals(name)) {
                    return (Element) n;
                }
            }
        }
        return null;
    }

    public static Element firstChildRecursive(Element e, String name) {
        Node n = null;
        for (int i = 0; i < e.getChildCount(); i++) {
            n = e.getChild(i);
            if (n instanceof Element) {
                if (name == null || ((Element) n).getLocalName().equals(name)) {
                    return (Element) n;
                }
                Element ret = firstChildRecursive((Element) n, name);
                if (ret != null) {
                    return ret;
                }
            }
        }
        return null;
    }

    /**
     * ch (e) --> [c_1,...,c_n], where parent(c_i)=e for all c_i
     */
    public static List<Node> ch(Node n) {
        List<Node> ret = new ArrayList<Node>();
        for (int i = 0; i < n.getChildCount(); i++) {
            Node c = n.getChild(i);

            if (c instanceof Text) {
                if (!"".equals(c.toXML().trim())) {
                    ret.add(c);
                }
            } else if (!(c instanceof Comment)) {
                ret.add(c);
            }
        }
        return ret;
    }

    /**
     * ch (e, n) --> [c_1,...,c_n], where parent(c_i)=e and name(c_i)=n for all c_i
     */
    public static List<Node> ch(Node n, String name) {
        if (n instanceof Element) {
            return ListUtil.<Node, Element>cast(ch((Element) n, name));
        } else {
            return Collections.<Node>emptyList();
        }
    }

    /**
     * ch (e) --> [c_1,...,c_n], where parent(c_i)=e for all c_i
     */
    public static List<Element> ch(Element e) {
        return ch(e, (String) null);
    }

    /**
     * ch (e, n) --> [c_1,...,c_n], where parent(c_i)=e and name(c_i)=n for all c_i
     */
    public static List<Element> ch(Element e, String name) {
        List<Element> l = new LinkedList<Element>();
        Element el = null;
        for (int i = 0; i < e.getChildCount(); i++) {
            if (e.getChild(i) instanceof Element) {
                el = (Element) e.getChild(i);
                if (name == null || el.getLocalName().equals(name)) {
                    l.add(el);
                }
            }
        }
        return l;
    }

    /**
     * ch (e, n) --> [c_1,...,c_n], where parent(c_i)=e and name(c_i)=n for all c_i
     */
    public static List<Element> children(Element e, String... names) {
        List<Element> l = new LinkedList<Element>();
        Element el = null;
        for (int i = 0; i < e.getChildCount(); i++) {
            if (e.getChild(i) instanceof Element) {
                el = (Element) e.getChild(i);
                if (names != null) {
                    String tmp = el.getLocalName();
                    for (String n : names) {
                        if (n.equals(tmp)) {
                            l.add(el);
                            break;
                        }
                    }
                } else {
                    l.add(el);
                }
            }
        }
        return l;
    }

    /**
     * ch (e) --> [c_1,...,c_n], where parent(c_i)=e for all c_i
     */
    public static List<Node> elch(Node n) {
        List<Node> ret = new ArrayList<Node>();
        for (int i = 0; i < n.getChildCount(); i++) {
            Node c = n.getChild(i);
            if (c instanceof Element) {
                ret.add(c);
            }
        }
        return ret;
    }

    /**
     * Extract elements.
     *
     * chex (e, n) --> [c_1,...,c_n], where parent(c_i)=e and name(c_i)!=n for all c_i
     */
    public static List<Node> chex(Node n, String name) {
        if (n instanceof Element) {
            Node child = null;
            Element e = null;
            List<Node> l = new LinkedList<Node>();
            for (int i = 0; i < n.getChildCount(); i++) {
                child = n.getChild(i);
                if (child instanceof Element) {
                    e = (Element) child;
                    if (!e.getLocalName().equals(name)) {
                        l.add(child);
                    }
                }
            }
            return l;
        } else {
            return Collections.<Node>emptyList();
        }
    }

    /**
     * Get the first parent element <code>name</code> of <code>el</code>
     *
     * @param el this element
     * @param name the name of the parent element
     * @return the parent element
     */
    public static Element getParent(Node el, String... names) {
        ParentNode p = el.getParent();
        while (p != null) {
            if (p instanceof Element) {
                Element e = (Element) p;
                String name = e.getLocalName();
                for (String s : names) {
                    if (name.equals(s)) {
                        return e;
                    }
                }
            }
            p = p.getParent();
        }
        return null;
    }

    /**
     * Get the first parent element <code>name</code> of <code>el</code>
     *
     * @param el this element
     * @param name the name of the parent element
     * @return the parent element
     */
    public static Element getParent(Element el, String name) {
        ParentNode p = el;
        while (p != null) {
            if (p instanceof Element) {
                Element e = (Element) p;
                if (e.getLocalName().equals(name)) {
                    return e;
                }
            }
            p = p.getParent();
        }
        return null;
    }

    public static org.w3c.dom.Document convert(Document n) {
        DocumentBuilder db = null;
        try {
            db = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            return DOMConverter.convert(n, db.getDOMImplementation());
        } catch (ParserConfigurationException exc) {
            System.err.println(exc.getMessage());
            return null;
        }
    }

    /**
     * Converts a DOM NodeList to a list of XOM nodes
     *
     * @param l the DOM NodeList
     * @return a list of XOM nodes
     */
    /* (cl) */
    public static List<Node> convert(org.w3c.dom.NodeList l) {
        List<Node> result = new ArrayList<Node>();
        for (int index = 0; index < l.getLength(); index++) {
            org.w3c.dom.Node n = l.item(index);
            if (n.getNodeType() != org.w3c.dom.Node.ELEMENT_NODE) {
                continue;
            }
            result.add(DOMConverter.convert((org.w3c.dom.Element) l.item(index)));
        }
        return result;
    }

    /**
     * Converts a list of XOM nodes to an equivalent DOM NodeList
     *
     * @param l the list of XOM nodes
     * @return a DOM NodeList equivalent to l
     */
    /* (cl) */
    public static org.w3c.dom.NodeList convert(List<Node> l) {
        Element dummyRoot = new Element("DummyRoot");
        for (Node n : l) {
            dummyRoot.appendChild(n);
        }
        Document dummyDoc = new Document(dummyRoot);

        DocumentBuilder db = null;
        try {
            db = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            final org.w3c.dom.Document doc = DOMConverter.convert(dummyDoc, db.getDOMImplementation());

            /*
             * TODO test this again once we use a newer version of Saxon For some strange reason, this does not work if convert is called by a Saxon XSLT extension function. If we return
             * this as instance of something that's more powerful than NodeList, Saxon 8.9 recognizes that and starts output at the DummyRoot element.
             */
            // return doc.getDocumentElement().chn();
            return new org.w3c.dom.NodeList() {

                public int getLength() {
                    return doc.getDocumentElement().getChildNodes().getLength();
                }

                public org.w3c.dom.Node item(int index) {
                    return doc.getDocumentElement().getChildNodes().item(index);
                }
            };
        } catch (ParserConfigurationException exc) {
            log.error("Error converting", exc);
            return null;
        }
    }

    /**
     * Collect all ids in <code>e</code>
     *
     * @param e the given element
     * @return a set of all ids
     */
    public static Set<String> collectIds(Element e) {
        Set<String> ret = new HashSet<String>();
        Nodes res = e.query("//@xml:id", XMLSymbols.CTX_XML);
        for (int i = 0; i < res.size(); i++) {
            Node n = res.get(i);
            if (n instanceof Attribute) {
                ret.add(((Attribute) n).getValue());
            }
        }
        return ret;
    }
}
