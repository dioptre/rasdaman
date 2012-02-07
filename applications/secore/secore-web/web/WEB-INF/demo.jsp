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
    Document   : demo
    Created on : Feb 4, 2012, 12:37:30 PM
    Author     : mihaelams1
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@page import="secore.util.Constants" %>
<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <title>Demo</title>
  </head>
  <body>
    <%
    // Get the URL of the service with entrypoint /def
    String scheme = request.getScheme();             
    String serverName = request.getServerName();     
    int serverPort = request.getServerPort();      
    String contextPath = request.getContextPath();  
    // Reconstruct original requesting URL
    String url = scheme+"://"+serverName+":"+serverPort+contextPath;
    // Check if any of the demo pages has been requested
    String query = request.getParameter("crs");
    // Check if the crs demo was requested
    if (null != query && query.equals("true")) { 
      out.println("<span style=\"font-size:x-large;\"><a href='" + Constants.DEMO_FILE + "'>Back to demo page</a></span><br/>"); %>
      <h1>Resolving CRSs</h1>
      <p>
      This service delivers the CRS pertaining to the identifier passed.
      There is a RESTful notation (representing OGC's current CRS naming scheme) and, additionally, a KVP notation.
      </p>

      <table border="0" align="left" valign="center">
        <tr>
          <td valign="top" width="500">
          <ul>
              <li>Retrieve definition of CRS with code 4326 (RESTful, current OGC naming scheme)<br>
              <%out.print("<a target='wcs-target' href='" + url + "/crs/EPSG/0/4326'>" + url + "/crs/EPSG/0/4326</a>");%>                
              <li>Same request in GET-KVP<br>
              <%out.print("<a target='wcs-target' href='" + url + "/crs?authority=EPSG&version=0&code=4326'>" + url + "/crs?authority=EPSG&version=0&code=4326</a>");%>  
              <li>Parametric CRS<br>
              <%out.print("<a target='wcs-target' href='" + url + "/crs/ISO/0/10000'>" + url + "/crs/ISO/0/10000</a>");%>    
          </ul>
          </td>	
          <td>
            <iframe name="wcs-target" id="wcs-target" src="" width="500" height="400">
              Sorry, your browser doesn't support iframes. Unfortunately this service cannot be used in this case.
            </iframe>
          </td>
        </tr>

      </table>

    <%
    } else { 
      // Check if the axis demo was requested
      query = request.getParameter("axis");
      if (null != query && query.equals("true")) { 
        out.println("<span style=\"font-size:x-large;\"><a href='" + Constants.DEMO_FILE + "'>Back to demo page</a></span><br/>"); %>
        <h1>Resolving Axes Synonyms</h1>
        <p>
        This service deliveres the definition of an axis given its predefined synomym.
        </p>

        <table border="0" align="left" valign="center">
          <tr>
            <td valign="top" width="500">
            <ul>
                <li>Retrieve the definition of Geodetic longitude, also known as Y<br>
                <%out.print("<a target='wcs-target' href='" + url + "/axis/Y'>" + url + "/axis/Y</a>");%>
                <li>Retrieve the definition of Gravity-related height, also known as elevation<br>
                <%out.print("<a target='wcs-target' href='" + url + "/axis/elevation'>" + url + "/axis/elevation</a>");%>
            </ul>
            </td>	
            <td>
              <iframe name="wcs-target" id="wcs-target" src="" width="500" height="400">
                Sorry, your browser doesn't support iframes. Unfortunately this service cannot be used in this case.
              </iframe>
            </td>
          </tr>

        </table>
        
      <%  
      } else { 
        // Check if the Combining CRS demo was requested
        query = request.getParameter("comb");
        if (null != query && query.equals("true")) { 
          out.println("<span style=\"font-size:x-large;\"><a href='" + Constants.DEMO_FILE + "'>Back to demo page</a></span><br/>"); %>
          <h1>Combining CRSs</h1>
          <p>
          This service establishes definition of a CRS which is obtained by combining the input CRSs.
          Parameter numbering indicates sequence of the input CRSs.
          Axes sets of the CRSs must be disjoint (for example, two CRSs both having a latitude axis cannot be combined).
          </p>

          <table border="0" align="left" valign="center">
            <tr>
              <td valign="top" width="500">
              <ul>
                  <li>Retrieve a compound CRS composed of CRSs 4326 and 4440<br>
                <%out.print("<a target='wcs-target' href='" + url + "/crs-compound?1=" + url + "/crs/EPSG/0/4326&2=" + url +"/crs/EPSG/0/4440'>" + url + "/crs-compound?1=" + url + "/crs/EPSG/0/4326&2=" + url +"/crs/EPSG/0/4440</a>");%>
                <li>Parametric compound coordinate reference system<br>
                <%out.print("<a target='wcs-target' href='" + url + "/crs-compound?1=" + url + "/crs/EPSG/0/4326&2=" + url +"/crs/ISO/0/10000'>" + url + "/crs-compound?1=" + url + "/crs/EPSG/0/4326&2=" + url +"/crs/ISO/0/10000</a>");%> 
              </ul>
              </td>	
              <td>
                <iframe name="wcs-target" id="wcs-target" src="" width="500" height="400">
                  Sorry, your browser doesn't support iframes. Unfortunately this service cannot be used in this case.
                </iframe>
              </td>
            </tr>

          </table>
          
        <%
        } else { 
          // Deliver the mai page of the demo
          out.println("<span style=\"font-size:x-large;\"><a href='" + Constants.INDEX_FILE + "'>Index</a></span><br/>"); %>             
          <h1>SeCoRe Demo</h1>

          <p>
          This is a demonstration of a Coordinate Reference System (CRS) Resolver service.
          The service accepts a URL and delivers the corresponding CRS encoded in GML.
          URL syntax should be obvious from the examples.
          The underlying database is that of <a href="http://www.epsg.org">EPSG</a> and, hence, knows all its CRS definitions.
          </p>
          <p>
          The following pages exemplify use of the CRS Resolver; based on these examples, why not try out yourself with other URLs using your browser:
          </p>
          <ul>

            <li><%out.print("<a href='" + Constants.DEMO_FILE + "?crs=true'>resolve CRSs</a><br/>");%> -- provide a CRS identifier, retrieve its definition
            <li><%out.print("<a href='" + Constants.DEMO_FILE + "?axis=true'>resolve axes synonyms</a><br/>");%> -- provide an axis name, retrieve its definition
            <li><%out.print("<a href='" + Constants.DEMO_FILE + "?comb=true'>combine CRSs</a><br/>");%> -- provide two or more CRS identifiers, retrieve their combination 
          </ul>
        <%
        }
      }
    }%>
  </body>
</html>
