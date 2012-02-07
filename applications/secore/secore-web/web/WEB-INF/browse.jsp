<%--
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
--%>
<%-- 
    Document   : browse
    Created on : Dec 21, 2011, 10:06:30 AM
    Author     : Mihaela Rusu
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@page import="java.util.Set"%>
<%@page import="java.util.TreeSet"%>
<%@page import="secore.db.DbManager" %>
<%@page import="secore.util.Constants" %>
<!DOCTYPE html>
<html>  
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <title>Browse</title>
  </head>
  <body>

<%
  out.println("<span style=\"font-size:x-large;\"><a href='" + Constants.INDEX_FILE + "'>Index</a></span><br/>");
// Future work: assure a smooth transition between URNs and URLs for the new identifiers
  String url = (String) request.getAttribute("url");
  
  String up = url.substring(0, url.lastIndexOf(Constants.REST_SEPARATOR, url.length() - 2));
  if (!up.isEmpty()) {
    out.println("<br/><span style=\"font-size:x-large;\"><a href='" + up + "/" + Constants.ADMIN_FILE + "'>Up one level</a></span> "); 
  }

  out.println("<br/><span style=\"font-size:x-large;\">Current Prefix: " + url + "</span>");
  out.println("<br/><span style=\"font-size:x-large;\">The list of the nodes known at this level</span><br/>");
    
  String urlColon = url.replaceAll(Constants.REST_SEPARATOR, Constants.URN_SEPARATOR);
  urlColon = urlColon.replaceAll(Constants.URN_SEPARATOR + Constants.ZERO + Constants.URN_SEPARATOR, Constants.URN_SEPARATOR + Constants.URN_SEPARATOR);
  
  // Handle changed GML deffinitions
  String mod = request.getParameter("changedef");
  if (null != mod) {
    if (!mod.equals(Constants.EMPTY)) {
      String urlnoColon =  urlColon.substring(0, urlColon.length() -1);
      String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
          + "let $x := doc('gml')//gml:identifier[text() = '" + Constants.URN_SHORT_PREFIX + urlnoColon + "']/.." + Constants.NEW_LINE
          + "return replace node $x with " + mod;
      String result = DbManager.getInstance().getDb().query(query);
      out.println("<br/><span style=\"font-size:x-large;\">The database has been updated</span><br/>");
    } else {
      out.println("<br/><span style=\"font-size:x-large;\"><span style=\"color:red\">Empty definition submitted! The database remains unchanged<span></span><br/>");
    }  
  }
  
  // Handle newly added GML deffinitions
  String newd = request.getParameter("adddef");
  if (null != newd) {
    if (!newd.equals(Constants.EMPTY)) {
      String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
          + "let $x := doc('gml')" + Constants.NEW_LINE
          + "return insert node <dictionaryEntry xmlns=\"http://www.opengis.net/gml\">"+newd+"</dictionaryEntry> into $x";
      String result = DbManager.getInstance().getDb().query(query);
      out.println("<br/><span style=\"font-size:x-large;\">The database has been updated</span><br/>");
    } else {
      out.println("<br/><span style=\"font-size:x-large;\"><span style=\"color:red\">Empty definition submitted! The database remains unchanged<span></span><br/>");
    }  
  }  
  
  // Decide whether to display the form for a new definition or just the list
  String toadd = request.getParameter("add");
  if (null != toadd && toadd.equals("true")) {
    %>
    <span style="font-size:x-large;">Add a new GML definition in the space below:</span><br/>
    <form action="<%=url + Constants.ADMIN_FILE%>" method="post" name="gmlform">
    <textarea cols="150" rows="20" name="adddef" wrap="virtual"></textarea><br/>
    <input type="submit" name="Add" value="Add" />
    </form>
    <%
  } else {
    // Handles removal of definitions
    String todel = request.getParameter("delete");
    if (null != todel) {
      if (todel.equals(Constants.ZERO)) {
        todel = Constants.EMPTY;
      }  
      String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
          + "for $x in doc('gml')//gml:identifier[text() = '" + Constants.URN_SHORT_PREFIX + urlColon + todel + "']/.. union doc('gml')//gml:identifier[contains(.,'"+ urlColon + todel +":')]/.." + Constants.NEW_LINE
          + "return delete node $x";
      String result = DbManager.getInstance().getDb().query(query);
    }
    // Query for the list
    String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
      + "for $x in doc('gml')//gml:identifier[contains(.,'"+ urlColon +"')]/text()" + Constants.NEW_LINE
      + "return <el>{$x}</el>";
    String result = DbManager.getInstance().getDb().query(query);
    
    // Query for individual GML definitions
    if (result.isEmpty()) {
      urlColon =  urlColon.substring(0, urlColon.length() -1);
      query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
          + "let $d := doc('gml')" + Constants.NEW_LINE
          + "return $d//gml:identifier[text() = '" + Constants.URN_SHORT_PREFIX + urlColon + "']/..";
      result = DbManager.getInstance().getDb().query(query);
      %>
      <span style="font-size:x-large;">The definition below will be replaced by your submission:</span><br/>
      <form action="<%=url + Constants.ADMIN_FILE%>" method="post" name="gmlform">
      <textarea cols="150" rows="20" name="changedef" wrap="virtual"><%out.print(result);%></textarea><br/>
      <input type="submit" name="Save" value="Save" />
      </form>
      <%
    } else {
      // Create a TreeSet of elements at this level of the prefix
      result = result.replace("</el>", "");
      String [] ids = result.split("<el>");
      Set<String> list = new TreeSet<String>();
      boolean bottom = false;
      for (String id: ids) {
        if (!id.isEmpty()) {
          String tmp = id.substring(id.indexOf(Constants.URN_SHORT_PREFIX + Constants.URN_SEPARATOR) + (Constants.URN_SHORT_PREFIX + Constants.URN_SEPARATOR).length());
          tmp = tmp.substring(tmp.indexOf(urlColon) + urlColon.length());
          if (tmp.startsWith(Constants.URN_SEPARATOR)) {
            tmp = Constants.ZERO + tmp;
          }
          if (tmp.indexOf(Constants.URN_SEPARATOR) >= 0) {
            tmp = tmp.substring(0, tmp.indexOf(Constants.URN_SEPARATOR));
          } else {
            bottom = true;
          }
          list.add(tmp);
        }
      }
      // Show the link for creating new elements at the bottom of the hierarchy
      if (bottom) {
          out.println("<span style=\"font-size:x-large;\"><a href='" + url + Constants.ADMIN_FILE + Constants.FRAGMENT_SEPARATOR + "add=true'>Add a new entry at this level:</a></span><br/>");
      }
  %>
      <table>
      <%
      // Display the list in a table
      for (String l: list) {
        out.print("<tr><td><a href='" + l + "/" + Constants.ADMIN_FILE + "'>" + l + "</a></td><td><a href='"+Constants.ADMIN_FILE + Constants.FRAGMENT_SEPARATOR + "delete=" + l +"' onclick='javascript:return confirm(\"Do you really want to delete the node " + l + " and all entries under the path " + url + l + "?\")'>Remove</a></td></tr>");
      }  
    %>
    </table>
    <%
    }
  }  
    %>
</body>
</html>

