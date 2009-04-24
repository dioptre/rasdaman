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
 * PURPOSE: 
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */
package rasj.rnp;


import java.util.*;
import java.io.*;
   

public class RnpMessageHeader
       {
        int  protocolId;
        byte messageEndianness;
        byte desiredEndianness;
        byte majorVersion;
        byte minorVersion;
        int  totalMessageLength;
        int  nrFragments;
        int  serverType;
        int  authInfoStart;
        int  authInfoLength;
        int  comprInfoStart;
        int  comprInfoLength;
        int  dataStart;
        int  dataLength;
        //int  _unused[5];
	
	public static final int length = 64;
	
        RnpMessageHeader(int nServerType)
	    {
	    protocolId = Rnp.rnpProtocolId;
            messageEndianness  = 0; // big endianness
            desiredEndianness  = 0; // big also
            majorVersion       = 1;
            minorVersion       = 0;
            totalMessageLength = length;
            nrFragments        = 0;
            serverType         = nServerType;
            authInfoStart      = 0;
            authInfoLength     = 0;
            comprInfoStart     = 0;
            comprInfoLength    = 0;
            dataStart          = length;
            dataLength         = 0;
	    }
	   
	int  countFragments()
	    { 
	    return nrFragments;
	    }
	
	void print()
	    {
	    System.out.println("RNP Header");
	    System.out.println(" totalLength=" + totalMessageLength);
	    System.out.println(" nrFragments=" + nrFragments);
	    System.out.println("  serverType=" + serverType);
	    System.out.println("   dataStart=" + dataStart);
    	    System.out.println("  dataLength=" + dataLength);
	    System.out.println("");
	    }
	   
	void write(DataOutputStream dataStream) throws IOException
	  {
	    dataStream.writeInt(protocolId);
	    dataStream.writeByte(messageEndianness);
	    dataStream.writeByte(desiredEndianness);
	    dataStream.writeByte(majorVersion);
	    dataStream.writeByte(minorVersion);
	    
	    dataStream.writeInt(totalMessageLength);
	    dataStream.writeInt(nrFragments);
	    dataStream.writeInt(serverType);
	    dataStream.writeInt(authInfoStart);
	    dataStream.writeInt(authInfoLength);
	    dataStream.writeInt(comprInfoStart);
	    dataStream.writeInt(comprInfoLength);
	    dataStream.writeInt(dataStart);
	    dataStream.writeInt(dataLength);
	    
	    // the unused 5 ints
	    dataStream.writeInt(0);
	    dataStream.writeInt(0);
	    dataStream.writeInt(0);
	    dataStream.writeInt(0);
	    dataStream.writeInt(0);
	    
	   }
	
	void read(DataInputStream dataStream) throws IOException, RnpException
	  {
	    protocolId         = dataStream.readInt();
	    messageEndianness  = dataStream.readByte();
	    desiredEndianness  = dataStream.readByte();
	    majorVersion       = dataStream.readByte();
	    minorVersion       = dataStream.readByte();
	    
	    totalMessageLength = dataStream.readInt();
	    nrFragments        = dataStream.readInt();
	    serverType         = dataStream.readInt();
	    authInfoStart      = dataStream.readInt();
	    authInfoLength     = dataStream.readInt();
	    comprInfoStart     = dataStream.readInt();
	    comprInfoLength    = dataStream.readInt();
	    dataStart          = dataStream.readInt();
	    dataLength         = dataStream.readInt();
	    
	    // the unused 5 ints
	    dataStream.readInt();
	    dataStream.readInt();
	    dataStream.readInt();
	    dataStream.readInt();
	    dataStream.readInt();
	    
	    //verify();
	   }
	}

