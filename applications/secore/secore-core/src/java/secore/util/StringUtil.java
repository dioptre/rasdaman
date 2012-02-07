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

import secore.ResolveRequest;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import static secore.util.Constants.*;

/**
 * String utilities.
 *
 * @author Dimitar Misev
 */
public class StringUtil {

  private static Logger log = LoggerFactory.getLogger(StringUtil.class);
  
  private static final String HTTP_PREFIX = "http://";
  private static final String JAVA_SCRIPT_ENGINE = "JavaScript";
  
  private static final String UTF_ENCODING = "UTF-8";
  private static final String DEF = "/def/";
  
  private static final ScriptEngineManager engineManager;
  private static final ScriptEngine engine;
  
  static {
    engineManager = new ScriptEngineManager();
    engine = engineManager.getEngineByName(JAVA_SCRIPT_ENGINE);
  }
  
  /**
   * Evaluate a JavaScript expression
   * @param expr JavaScript expression
   * @return the value of the expression as string
   * @throws ScriptException 
   */
  public static String evaluate(String expr) throws ScriptException {
    log.trace("Evaluating expression: " + expr);
    Object res = engine.eval(expr);
    if (res != null) {
      return res.toString();
    }
    return null;
  }

  /** URL-decode a string, if needed */
  public static String urldecode(String encodedText) {
    if (encodedText == null) {
      return null;
    }
    String decoded = encodedText;
    if (encodedText.indexOf(WHITESPACE) == -1) {
      try {
        decoded = URLDecoder.decode(encodedText, UTF_ENCODING);
      } catch (UnsupportedEncodingException ex) {
        decoded = URLDecoder.decode(encodedText);
      }
    }
    return decoded;
  }

  public static String stripDef(String s) {
    if (s.contains(DEF)) {
      return s.substring(s.indexOf(DEF) + DEF.length());
    }
    return s.substring(1);
  }
  
  public static String removeDuplicateDef(String s) {
    int ind = s.indexOf("/def");
    return s.substring(0, ind) + s.substring(ind + "/def".length());
  }

  public static String getServiceUrl(String s) {
    if (s.contains(DEF)) {
      return s.substring(0, s.indexOf(DEF) + DEF.length());
    }
    return s;
  }

  public static ResolveRequest buildRequest(String uri) throws SecoreException {
    log.trace("Building request for URI: " + uri);
    uri = StringUtil.urldecode(uri);
    String fullUri = uri;
    log.trace("Decoded URI: " + uri);
    String serviceUri = StringUtil.getServiceUrl(uri);
    uri = StringUtil.stripDef(uri);
    log.trace("Parameters: " + uri);

    // set operation
    int ind1 = uri.indexOf(REST_SEPARATOR);
    int ind2 = uri.indexOf(FRAGMENT_SEPARATOR);
    int ind = -1;
    if (ind1 != -1 && (ind2 == -1 || ind1 < ind2)) {
      ind = ind1;
    } else if (ind2 != -1 && (ind1 == -1 || ind2 < ind1)) {
      ind = ind2;
    } else {
      log.error("Couldn't determine request operation in " + uri);
      throw new SecoreException(ExceptionCode.InvalidRequest, "Mallformed request: " + uri);
    }
    String operation = uri.substring(0, ind);
    uri = uri.substring(ind + 1);
    ResolveRequest ret = new ResolveRequest(operation, serviceUri, fullUri);

    // set params
    String[] pairs = null;
    if (uri.contains(HTTP_PREFIX)) {
      // special case for crs-compound
      while ((ind = uri.indexOf(HTTP_PREFIX, HTTP_PREFIX.length() + 5)) != -1) {
        String key = uri.substring(0, ind);
        if (key.matches(".+&\\d+=")) {
          key = key.substring(0, key.lastIndexOf(PAIR_SEPARATOR));
          uri = uri.substring(key.length() + 1);
        } else {
          uri = uri.substring(ind);
        }
        addParam(ret, key);
      }
      addParam(ret, uri);
    } else {
      // all other cases
      pairs = uri.split("[/\\?&]");
      for (String pair : pairs) {
        if (!pair.equals(EMPTY)) {
          String key = pair;
          String val = null;
          if (pair.contains(KEY_VALUE_SEPARATOR)) {
            String[] tmp = pair.split(KEY_VALUE_SEPARATOR);
            key = tmp[0];
            if (tmp.length > 1) {
              val = tmp[1];
            }
          }
          ret.addParam(key, val);
        }
      }
    }
    log.trace("Built request: " + ret);
    return ret;
  }

  private static void addParam(ResolveRequest req, String param) throws SecoreException {
    if (param.matches("\\d+=.+")) {
      String[] tmp = param.split(KEY_VALUE_SEPARATOR);
      req.addParam(tmp[0], tmp[1]);
    } else {
      req.addParam(param, null);
    }
  }

  /**
   * Get the text content of an element in given xml.
   * @param xml xml
   * @param elname element name
   * @return the text content of elname
   */
  public static String getElementValue(String xml, String elname) {
    String ret = null;
    Pattern pattern = Pattern.compile("<(.+:)?" + elname + "[^>]*>([^<]+)</(.+:)?" + elname + ">.*");
    Matcher matcher = pattern.matcher(xml);
    if (matcher.find()) {
      ret = matcher.group(2);
    }
    return ret;
  }

  /**
   * Get the root element name of the given xml document.
   * @param xml an XML document
   * @return the root name, or null in case of an error
   */
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

  /**
   * Converts a collection to a string, separating the elements by c
   *
   * @param l The StringList
   * @param c The delimiter
   * @return A String of the ListElements separated by c.
   */
  public static <T> String ltos(Collection<T> l, Character c) {
    String s = EMPTY;
    for (Iterator<T> iter = l.iterator(); iter.hasNext();) {
      if (!s.equalsIgnoreCase(EMPTY)) {
        s = s + c + iter.next().toString();
      } else {
        s = s + iter.next().toString();
      }
    }
    return s;
  }

  /**
   * Converts a string to a list
   *
   * @param s The StringList
   * @return A String of the ListElements separated by c.
   */
  public static List<String> stol(String s) {
    List<String> l = new LinkedList<String>();
    if (s == null) {
      return l;
    }
    String[] sl = s.split(",");
    for (int i = 0; i < sl.length; i++) {
      l.add(sl[i]);
    }
    return l;
  }

  /**
   * Replace all URNs in s with URLs in REST format.
   * 
   * @param s a GML definition
   * @return the definition with the fixed links
   */
  public static String fixLinks(String s) {
    // $1 = operation
    // $2 = authority
    // $3 = code
    // TODO fix handling of version, 0 is assumed now
    return s.replaceAll(URN_PREFIX + ":(.+):(.+)::(.+)",
        Config.getInstance().getServiceUrl() + "/$1/$2/0/$3");
  }
}
