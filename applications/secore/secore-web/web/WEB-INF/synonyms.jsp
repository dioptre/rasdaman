<%--
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
--%>
<%-- 
    Document   : synonyms
    Created on : Dec 23, 2011, 5:41:46 AM
    Author     : mihaelams1
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@page import="secore.db.DbManager" %>
<%@page import="secore.util.Constants" %>
<!DOCTYPE html>
<html>  
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <title>Synonyms</title>
  </head>
  <body>
    
<%
    out.println("<span style=\"font-size:x-large;\"><a href='" + Constants.INDEX_FILE + "'>Index</a></span><br/>");
    // Handle changed synonyms
    String mod = request.getParameter("modsyn");
    if (null != mod) {
      if (!mod.equals(Constants.EMPTY)) {
        String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
            + "let $x := doc('gml')//gml:axis[@identifier = '" +  request.getParameter("axisid") + "']" + Constants.NEW_LINE
            + "return replace node $x with " + mod;
        String result = DbManager.getInstance().getDb().query(query);
        out.println("<br/><span style=\"font-size:x-large;\">The database has been updated</span><br/>");
      } else {
        out.println("<br/><span style=\"font-size:x-large;\"><span style=\"color:red\">Empty definition submitted! The database remains unchanged<span></span><br/>");
      }  
    }

    // Handle newly added synonyms
    String newd = request.getParameter("addnew");
    if (null != newd) {
      if (!newd.equals(Constants.EMPTY)) {
        String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
            + "let $x := doc('gml')//gml:synonyms" + Constants.NEW_LINE
            + "return insert node "+newd+" into $x";
        String result = DbManager.getInstance().getDb().query(query);
        out.println("<br/><span style=\"font-size:x-large;\">The database has been updated</span><br/>");
      } else {
        out.println("<br/><span style=\"font-size:x-large;\"><span style=\"color:red\">Empty definition submitted! The database remains unchanged<span></span><br/>");
      }  
    }  
    // Handles editing of synonyms
    String toedit = request.getParameter("edit");
    if (null != toedit) {
      if (toedit.equals(Constants.ZERO)) {
        toedit = Constants.EMPTY;
      }  
      String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
          + "let $d := doc('gml')" + Constants.NEW_LINE    
          + "return $d//gml:axis[@identifier = '" +  toedit + "']";
      String result = DbManager.getInstance().getDb().query(query);
      out.println("<br/><span style=\"font-size:x-large;\">This reference axis and its synonyms below will be replaced by your submission:</span>");
      out.println("<br/><span style=\"font-size:x-large;\"><a href='"+ Constants.SYNONYMS_FILE +"'>Back</a></span><br/>");
      %>
      <form action="<%=Constants.SYNONYMS_FILE%>" method="post" name="gmlform">
      <textarea cols="150" rows="20" name="modsyn" wrap="virtual"><%=result%></textarea><br/>
      <input type="submit" name="Save" value="Save" />
      <input type="hidden" name="axisid" value="<%=toedit%>"/>
      </form>
      <%
    } else {
      
      // Decide whether to display the form for a new definition or just the list
      String toadd = request.getParameter("add");
      if (null != toadd && toadd.equals("true")) {
        out.println("<br/><span style=\"font-size:x-large;\">Please write the XML node containing a new reference axis and its synonyms in the space below:</span>");
        out.println("<br/><span style=\"font-size:x-large;\"><a href='"+ Constants.SYNONYMS_FILE +"'>Back</a></span><br/>");
        %>
        <form action="<%=Constants.SYNONYMS_FILE%>" method="post" name="gmlform">
        <textarea cols="150" rows="20" name="addnew" wrap="virtual"></textarea><br/>
        <input type="submit" name="Add" value="Add" />
        </form>
        <%
      } else {

        // Handles removal of synonyms
        String todel = request.getParameter("delete");
        if (null != todel) {
          if (todel.equals(Constants.ZERO)) {
            todel = Constants.EMPTY;
          }  
          String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
              + "let $d := doc('gml')" + Constants.NEW_LINE    
              + "return delete node $d//gml:axis[@identifier = '" +  todel + "']";
          String result = DbManager.getInstance().getDb().query(query);
        }
        out.println("<br/><span style=\"font-size:x-large;\">The list of the reference axes for which synonyms are provided:</span>");
        out.println("<br/><span style=\"font-size:x-large;\"><a href='"+ Constants.SYNONYMS_FILE + Constants.FRAGMENT_SEPARATOR + "add=true'>Add new</a></span><br/>");
        String query = "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
            + "for $x in data(doc('gml')//gml:synonym/../@identifier)"
            + "return <el>{$x}</el>";
        String result = DbManager.getInstance().getDb().query(query);
        result = result.replace("</el>", Constants.EMPTY);
        String [] ids = result.split("<el>");

      %>
        <table>
        <%
        // Display the list in a table
        for (String id: ids) {
          if (!id.isEmpty()) {
            if (id.endsWith(Constants.NEW_LINE)) {
              id = id.substring(0, id.length() - 1);
            }
            query = "declare namespace epsg=\"urn:x-ogp:spec:schema-xsd:EPSG:0.1:dataset\";" + Constants.NEW_LINE
                    + "declare namespace gml = \"http://www.opengis.net/gml\";" + Constants.NEW_LINE
              + "let $d := doc('gml')" + Constants.NEW_LINE
              + "return $d//epsg:AxisName[gml:identifier[text() = '" + id + "']]/gml:name";
            result = DbManager.getInstance().getDb().query(query);
            out.print("<tr><td>" + id + " (" + result + ")</td><td><a href='" + Constants.SYNONYMS_FILE + Constants.FRAGMENT_SEPARATOR + "edit=" + id + "'>Edit synonyms</a></td><td><a href='"+Constants.SYNONYMS_FILE + Constants.FRAGMENT_SEPARATOR + "delete=" + id + "' onclick='javascript:return confirm(\"Do you really want to delete the reference " + id + " and all the existent synonyms?\");'>Remove</a></td></tr>");
          }
        }       
        %>
        </table>
        <%
      } 
    }
    %>
  </body>
</html>

