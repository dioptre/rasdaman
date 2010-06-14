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
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
*/
/*************************************************************
 * <pre>
 *
 * PURPOSE: generate debug output into servlet log
 * Methods of this static class perform debugging output into
 * the servlet log (usually /opt/jakarta/logs/servlet.log).
 * For entering a method to be traced, logging indent level is
 * increased; the logging methods for leaving a method decrease
 * indent, and the trace methods inbetween leave indentation unchanged.
 * [Needless to say that both statically and dynamically the number
 * of enters and leaves should match.]
 * Four trace levels are provided, intended for reporting increasingly
 * verbose (i.e., for decreasingly dangerous events):
 *      critical (always logged),
 *      warning (default level),
 *      sparse,
 *      verbose.
 * For each combination of enter/inside/leave of a method and trace
 * levels corresponding report functions are provided.
 * Finally, the debug level can be changed any time with
 * setDebugLevel().
 * Initialisation requires passing the servlet object to gain an
 * output stream to write to.
 *
 *
 * COMMENTS:
 * - keep consistent with rasogc.Debug !!!
 * - documentation to be done
 * </pre>
 ************************************************************/

package rasj.global;

import java.lang.System;

/**
 * Methods of this static class perform debugging output into
 * the servlet log (usually /opt/jakarta/logs/servlet.log).
 * For entering a method to be traced, logging indent level is
 * increased; the logging methods for leaving a method decrease
 * indent, and the trace methods inbetween leave indentation unchanged.
 * [Needless to say that both statically and dynamically the number
 * of enters and leaves should match.]
 * <P>
 * Four trace levels are provided, intended for reporting increasingly
 * verbose (i.e., for decreasingly dangerous events):
 * <UL>
 *      <LI>critical (always logged),
 *      <LI>warning (default level),
 *      <LI>sparse,
 *      <LI>verbose.
 * </UL>
 * For each combination of enter/inside/leave of a method and trace
 * levels corresponding report functions are provided.
 * Finally, the debug level can be changed any time with
 * setDebugLevel().
 * <P>
 * Initialisation requires passing the servlet object to gain an
 * output stream to write to.
 * <P>
 *
 * @version $Revision: 1.2 $
 *
 */

public class Debug
  {
    static final String rcsid = "@(#)Package rasj.global, class Debug: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/global/Debug.java,v 1.2 2006/01/19 18:24:16 rasdev Exp $";

    //
    // debug threshold levels (constant defs):
    //

    /** log only critical events **/
    public static final int CRITICAL_LEVEL = 0;	// print always

    /** log also warnings which do not really impede processing **/
    public static final int WARNING_LEVEL  = 1;

    /** log also occasional output interesting for debugging **/
    public static final int SPARSE_LEVEL   = 2;

    /** log all information available to aid debugging **/
    public static final int VERBOSE_LEVEL  = 3;


    /**
    * debug threshold default
    **/ 
    public static final int DEFAULT_LEVEL  = WARNING_LEVEL;

    /**
    * debug threshold - only levels below it are displayed:
    **/ 
    private static int debugThreshold = DEFAULT_LEVEL;

    /**
    unit of indentation to display calling hierarchy
    **/
    private static final String INDENT = ". ";

    /**
    * indentation counter
    **/
    private static int indentLevel = 0;

    /**
    * set debug threshold to control further output
    **/
    public static void setDebugThreshold(int level)
      {
        if (level >= 0)
        {
            debugThreshold = level;
            System.err.println( "Debug::setDebugThreshold: setting debug level to " + level );
        }
        else
            System.err.println( "Debug::setDebugThreshold: ignoring illegal debug level value: " + level );
      }

    /**
    the following set of methods logs for entering/within/leaving a method.
    Entering increases printing indent level, talk leaves it unchanged, and leaving decreases indentation.
    these choices are crossed with the debug levels to achieve handy, short calls.
    **/

    /** method enter msg, output only with critical level **/
    public static void enterCritical( String what )	{ enter( CRITICAL_LEVEL, what ); }
    /** method enter msg, output only with warning level **/
    public static void enterWarning( String what )	{ enter( WARNING_LEVEL,  what ); }
    /** method enter msg, output only with sparse level **/
    public static void enterSparse( String what )	{ enter( SPARSE_LEVEL,   what ); }
    /** method enter msg, output only with verbose log level **/
    public static void enterVerbose( String what )	{ enter( VERBOSE_LEVEL,  what ); }

    /** method leave msg, output only with critical level **/
    public static void leaveCritical( String what )	{ leave( CRITICAL_LEVEL, what ); }
    /** method leave msg, output only with warning level **/
    public static void leaveWarning( String what )	{ leave( WARNING_LEVEL,  what ); }
    /** method leave msg, output only with sparse level **/
    public static void leaveSparse( String what )	{ leave( SPARSE_LEVEL,   what ); }
    /** method leave msg, output only with verbose log level **/
    public static void leaveVerbose( String what )	{ leave( VERBOSE_LEVEL,  what ); }

    /** method inside msg, output only with critical level **/
    public static void talkCritical( String what )	{ talk( CRITICAL_LEVEL, what ); }
    /** method inside msg, output only with warning level **/
    public static void talkWarning( String what )	{ talk( WARNING_LEVEL,  what ); }
    /** method inside msg, output only with sparse level **/
    public static void talkSparse( String what )	{ talk( SPARSE_LEVEL,   what ); }
    /** method inside msg, output only with verbose log level **/
    public static void talkVerbose( String what )	{ talk( VERBOSE_LEVEL,  what ); }

    /**
    * writes messages to the "standard" error log stream, increments indentation
    **/
    private static void enter(int level, String what)
      {
        StringBuffer s = new StringBuffer(100);		// to hold indent prefix

        if(level <= debugThreshold)
          {
            indentLevel++;				// indent one more to the right
            s.append( "rasj[" + Integer.toString(level) + "] " );	// document log level
            for (int i=0; i<indentLevel; i++)
                s.append( INDENT );
            s.append( what );
            System.err.println( s );
          }
      }


   /**
    * writes messages to the "standard" error log stream, decrements indentation
    **/
    static void leave(int level, String what)
      {
        StringBuffer s = new StringBuffer(100);		// to hold indent prefix

        if(level <= debugThreshold)
          {
            s.append( "rasj[" + Integer.toString(level) + "] " );	// document log level
            for (int i=0; i<indentLevel; i++)
                s.append( INDENT );
            s.append( what );
            System.err.println( s );
            indentLevel--;				// indent one less, go left
          }
      }

    /**
    * writes messages to the "standard" error log stream; indentation unchanged
    **/
    static void talk(int level, String what)
      {
        StringBuffer s = new StringBuffer(100);		// to hold indent prefix

        if(level <= debugThreshold)
          {
            s.append( "rasj[" + Integer.toString( level) + "] " );	// document log level
            for (int i=0; i<indentLevel; i++)
                s.append( INDENT );
            s.append( what );
            System.err.println( s );
          }
      }
  }
