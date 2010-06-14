package rasj;

import java.util.*;
import rasj.global.*;
import org.odmg.*;

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
/** ***********************************************************
 * <pre>
 *
 * PURPOSE:
 * This exception extends the ODMGQueryInvalidException by offering direct access to the
 * RasDaMan error number and the line, column and token in the querystring that produced the error.
 * @version $Revision: 1.10 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */


public class RasQueryExecutionFailedException extends QueryInvalidException
{
    static final String rcsid = "@(#)Package rasj, class RasQueryExecutionFailedException: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasQueryExecutionFailedException.java,v 1.10 2003/12/19 16:22:27 rasdev Exp $";

    /**
     * the official RasDaMan error number
     */
    private int errNo;

    /**
     * line number of the error
     */    
    private int line;

    /**
     * column of the error
     */
    private int column;

    /**
     * token that caused the error
     */
    private String token;
    
    public RasQueryExecutionFailedException(String string)
    {
        if (string==null)
        {
	  line = 0;
	  column = 0;
	  token = "(null)"; 
        }
        else
        {
            StringTokenizer tokenizer= new StringTokenizer( string, RasGlobalDefs.KEYWORD_TAB );
	    String Ttoken=tokenizer.nextToken(); 
	    Ttoken=tokenizer.nextToken();
	    errNo=Integer.parseInt(Ttoken);
	    if(tokenizer.hasMoreTokens())
	    {
	       line=Integer.parseInt(tokenizer.nextToken());
	       column=Integer.parseInt(tokenizer.nextToken());
	       token=tokenizer.nextToken(); 
	    }
	    else
	    {
	      line=0;
	      column=0;
	      token=""; 
	    }    
        }
    }

    /**
     * Default Constructor setting the error specification
     *@param errorNo official RasDaMan error number
     *@param lineNo line in the query string that contained the error
     *@param colNo column in the query string that contains the error
     *@param tok token that caused the error
     */
    public RasQueryExecutionFailedException( int errorNo, int lineNo, int colNo, String tok )
    {
	super();
	errNo = errorNo;
        line = lineNo;
        column = colNo;
        token = ( (tok==null) ? "(null)" : tok );
    }

    public String getMessage()
    {
	int index;

        if(super.getMessage() == null)

	    {
	    String msg = RasErrorTexts.getErrorMessage(errNo);

	    StringBuffer buf = new StringBuffer(msg);
	    index = msg.indexOf( RasGlobalDefs.KEYWORD_TOKEN );
	    if(index != -1)
	        buf.replace(index, index+RasGlobalDefs.KEYWORD_TOKEN.length(), token);
	    
	    msg = buf.toString();
	    index = msg.indexOf( RasGlobalDefs.KEYWORD_ERRNO );
	    if(index != -1)
	        buf.replace(index, index+RasGlobalDefs.KEYWORD_ERRNO.length(), String.valueOf(errNo));
	    
	    msg = buf.toString();
	    index = msg.indexOf( RasGlobalDefs.KEYWORD_LINENO );
	    if(index != -1)
	        buf.replace(index, index+RasGlobalDefs.KEYWORD_LINENO.length(), String.valueOf(line));
	    
	    msg = buf.toString();
	    index = msg.indexOf( RasGlobalDefs.KEYWORD_COLNO );
	    if(index != -1)
	        buf.replace(index, index+RasGlobalDefs.KEYWORD_COLNO.length(), String.valueOf(column));
	    
	    return buf.toString();		
	    }
        else
	    return super.getMessage();
    }

    /**
     * returns the line number of the error
     */
    public int getLine()
        {
        return line;
        }

    /**
     * return the column number of the error
     */
    public int getColumn()
        {
        return column;
        }

    /**
     * returns the token that caused the error
     */
    public String getToken()
        {
        return token;
        }

  }
