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
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package rasj.clientcommhttp;

import java.io.*;
import java.net.*;
import java.lang.*;

/**
 *
 * This class provides some useful methods for reading binary data from a stream
 * or converting objects to a byte array for transmission.
 *
 */
public final class RasUtils implements RasCommDefs
{
    static final String rcsid = "@(#)Package rasj.clientcommhttp, class RasUtils: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/clientcommhttp/RasUtils.java,v 1.5 2003/12/10 21:04:26 rasdev Exp $";

    public RasUtils()
    {
    }

    /**
     * This method is used for turning up to 4 unsigned bytes into signed integers.
     * 
     * @param uBytes one to four Bytes which are interpreted as an unsigned Integer
     * @param endianess determines the order of the bytes: 0 = bigendian, 1 = little endian
     */
    public static int ubytesToInt( byte[] uBytes, byte endianess ) 
    {
        int   tmpi;
        byte  tmpb;
        int   retval = 0;

        for( int i = 0; i < uBytes.length; i++ )
	    {
		if( endianess == BIG_ENDIAN )
		    tmpb = uBytes[uBytes.length-i-1];
		else
		    tmpb = uBytes[i];

		tmpi = 0;
		/* Byte < 0 */
		if( (int)tmpb < 0 )
		    tmpi = 256 + tmpb;
		else
		    tmpi = tmpb;

		tmpi <<= (i*8);
		retval += tmpi;
	    }
        return retval;
    }

    /**
     * Reads characters from a stream until a '\0' character is reached.
     *
     * @param in BufferedInputStream to be read from ( must have been initialized before! )
     */
    public static String readString( InputStream in )
        throws IOException
    {
        byte b = (byte) '\0';
        byte[] b1 = new byte[1];
        String retval = "";

        while(in.read(b1) == 0);
        while(b1[0] != b)
	    {
		retval =retval + (char)b1[0];
		while(in.read(b1) == 0);
	    }
        return retval;
    }



}
