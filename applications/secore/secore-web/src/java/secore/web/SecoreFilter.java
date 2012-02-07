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
package secore.web;

import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import secore.util.Config;
import secore.util.Constants;

/**
 *
 * @author mihaelams1
 */
public class SecoreFilter implements Filter {
  
  private static Logger log = LoggerFactory.getLogger(SecoreServlet.class);
  
  private static final boolean debug = true;
  // The filter configuration object we are associated with.  If
  // this value is null, this filter instance is not currently
  // configured. 
  private FilterConfig filterConfig = null;
  
  public SecoreFilter() {
    Config.getInstance();
    log.info("Initializing SECORE web front-end...");
  }  
  
  private void doBeforeProcessing(ServletRequest request, ServletResponse response)
      throws IOException, ServletException {
    if (debug) {
      log.debug("Filter:DoBeforeProcessing");
    }
  }  
  
  private void doAfterProcessing(ServletRequest request, ServletResponse response)
      throws IOException, ServletException {
    if (debug) {
      log.debug("Filter:DoAfterProcessing");
    }
  }

  /**
   *
   * @param request The servlet request we are processing
   * @param response The servlet response we are creating
   * @param chain The filter chain we are processing
   *
   * @exception IOException if an input/output error occurs
   * @exception ServletException if a servlet error occurs
   */
  public void doFilter(ServletRequest request, ServletResponse response,
      FilterChain chain)
      throws IOException, ServletException {
    
    if (debug) {
      log.debug("Filter:doFilter()");
    }
    
    doBeforeProcessing(request, response);
    
    Throwable problem = null;
    try {
      String uri = ((HttpServletRequest) request).getRequestURI();
      log.debug("Request URI: " + uri);
      if (uri.endsWith(Constants.ADMIN_FILE)) {
          log.debug("Call" + Constants.ADMIN_FILE);
          uri = uri.substring(0, uri.length() - Constants.ADMIN_FILE.length());
          request.setAttribute("url", uri);
          request.getRequestDispatcher("/WEB-INF/" + Constants.ADMIN_FILE).forward(request, response);
      } else if (uri.endsWith(Constants.SYNONYMS_FILE)) {
          log.debug("Call" + Constants.SYNONYMS_FILE);
          request.getRequestDispatcher("/WEB-INF/" + Constants.SYNONYMS_FILE).forward(request, response);
      } else if (uri.endsWith(Constants.DEMO_FILE)) {
          log.debug("Call" + Constants.DEMO_FILE);
          request.getRequestDispatcher("/WEB-INF/" + Constants.DEMO_FILE).forward(request, response);
      } else if (uri.endsWith(Constants.INDEX_FILE)) {
          log.debug("Call" + Constants.INDEX_FILE);
          request.getRequestDispatcher("/" + Constants.INDEX_FILE).forward(request, response);
      } else if (uri.startsWith("/def")) {
          request.getRequestDispatcher(uri).forward(request, response);
      } else {
          chain.doFilter(request, response); // Goes to default servlet.
      }
    } catch (Throwable t) {
      // If an exception is thrown somewhere down the filter chain,
      // we still want to execute our after processing, and then
      // rethrow the problem after that.
      problem = t;
      t.printStackTrace();
    }
    
    doAfterProcessing(request, response);

    // If there was a problem, we want to rethrow it if it is
    // a known type, otherwise log it.
    if (problem != null) {
      if (problem instanceof ServletException) {
        throw (ServletException) problem;
      }
      if (problem instanceof IOException) {
        throw (IOException) problem;
      }
      sendProcessingError(problem, response);
    }
  }

  /**
   * Return the filter configuration object for this filter.
   */
  public FilterConfig getFilterConfig() {
    return (this.filterConfig);
  }

  /**
   * Set the filter configuration object for this filter.
   *
   * @param filterConfig The filter configuration object
   */
  public void setFilterConfig(FilterConfig filterConfig) {
    this.filterConfig = filterConfig;
  }

  /**
   * Destroy method for this filter 
   */
  public void destroy() {    
  }

  /**
   * Init method for this filter 
   */
  public void init(FilterConfig filterConfig) {    
    this.filterConfig = filterConfig;
    if (filterConfig != null) {
      if (debug) {        
        log("Filter:Initializing filter");
      }
    }
  }

  /**
   * Return a String representation of this object.
   */
  @Override
  public String toString() {
    if (filterConfig == null) {
      return ("Filter()");
    }
    StringBuffer sb = new StringBuffer("Filter(");
    sb.append(filterConfig);
    sb.append(")");
    return (sb.toString());
  }
  
  private void sendProcessingError(Throwable t, ServletResponse response) {
    String stackTrace = getStackTrace(t);    
    
    if (stackTrace != null && !stackTrace.equals("")) {
      try {
        response.setContentType("text/html");
        PrintStream ps = new PrintStream(response.getOutputStream());
        PrintWriter pw = new PrintWriter(ps);        
        pw.print("<html>\n<head>\n<title>Error</title>\n</head>\n<body>\n"); //NOI18N

        // PENDING! Localize this for next official release
        pw.print("<h1>The resource did not process correctly</h1>\n<pre>\n");        
        pw.print(stackTrace);        
        pw.print("</pre></body>\n</html>"); //NOI18N
        pw.close();
        ps.close();
        response.getOutputStream().close();
      } catch (Exception ex) {
      }
    } else {
      try {
        PrintStream ps = new PrintStream(response.getOutputStream());
        t.printStackTrace(ps);
        ps.close();
        response.getOutputStream().close();
      } catch (Exception ex) {
      }
    }
  }
  
  public static String getStackTrace(Throwable t) {
    String stackTrace = null;
    try {
      StringWriter sw = new StringWriter();
      PrintWriter pw = new PrintWriter(sw);
      t.printStackTrace(pw);
      pw.close();
      sw.close();
      stackTrace = sw.getBuffer().toString();
    } catch (Exception ex) {
    }
    return stackTrace;
  }
  
  public void log(String msg) {
    filterConfig.getServletContext().log(msg);    
  }
}
