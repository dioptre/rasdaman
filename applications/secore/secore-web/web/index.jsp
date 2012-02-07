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
    Document   : index
    Created on : Oct 1, 2011, 12:18:10 PM
    Author     : Dimitar Misev
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@page import="secore.db.DbManager" %>
<%@page import="secore.util.Constants" %>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Index</title>
    </head>
    <body>
     <% // Handle the direct queries
        String query = request.getParameter("runquery");
        if (null != query) {
          out.println("<br/><span style=\"font-size:x-large;\"><a href='"+ Constants.INDEX_FILE +"'>Index</a></span><br/>");
          if (!query.equals(Constants.EMPTY)) {
            String result = DbManager.getInstance().getDb().query(query);
            out.println("<br/><span style=\"font-size:x-large;\">Result:</span><br/>");
            out.println("<form name=mf><textarea name=mt cols=150 rows=20 readonly>" + result + "</textarea></form>");
          } else {
            out.println("<br/><span style=\"font-size:x-large;\"><span style=\"color:red\">Empty query submitted!<span></span><br/>");
          }  
        } else {

          String toquery = request.getParameter("query");
          if (null != toquery && toquery.equals("true")) {
            out.println("<br/><span style=\"font-size:x-large;\">Please write the XQuery to be executed on the database in the space below:</span>");
            out.println("<br/><span style=\"font-size:x-large;\"><a href='"+ Constants.INDEX_FILE +"'>Back</a></span><br/>");
            %>
            <form action="<%=Constants.INDEX_FILE%>" method="post" name="queryform">
            <textarea cols="150" rows="20" name="runquery" wrap="virtual"></textarea><br/>
            <input type="submit" name="Execute" value="Execute" />
            </form>
            <%
          } else {%>
            <h1>Coordinate Reference System Resolver</h1>
            <span style="font-size:x-large;"><%out.print("<a href='" + Constants.ADMIN_FILE + "'>Browse the definitions tree</a><br/>");%></span>
            <span style="font-size:x-large;"><%out.print("<a href='" + Constants.SYNONYMS_FILE + "'>View and edit axes synonyms</a><br/>");%></span>
            <span style="font-size:x-large;"><%out.print("<a href='" + Constants.INDEX_FILE + "?query=true'>Query the database directly</a><br/>");%></span>
            <span style="font-size:x-large;"><%out.print("<a href='" + Constants.DEMO_FILE + "'>View examples</a><br/>");%></span>
        <%}
        }%>
    </body>
</html>
