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
   

public class RnpFragmentHeader
          {
          int fragmType;    //{FragmentType command, okAnswer, error}
          int command;
          int nrParams;
          int totalLength;
       
          public static final int length = 16;
	
          RnpFragmentHeader(int fragmentType, int nCommand)
 	     { 
       	     fragmType   = fragmentType;
	     command     = nCommand;
	     nrParams    = 0;
	     totalLength = length;
	     }
	
	  int countParameters()
	     { 
	     return nrParams;
	     }
	
	  void print()
	     {
	     System.out.print("  type=" + fragmType);
    	     System.out.print("  Command=" + command);
	     System.out.print("  nrParams=" + nrParams);
	     System.out.print("  totalLength=" + totalLength);
	     System.out.println();
	     }
	   
	  void write(DataOutputStream dataStream) throws IOException
	     {
	     dataStream.writeInt(fragmType);
	     dataStream.writeInt(command);
	     dataStream.writeInt(nrParams);
   	     dataStream.writeInt(totalLength);
             }
	  void read(DataInputStream dataStream) throws IOException
	     {
	     fragmType   = dataStream.readInt();
	     command     = dataStream.readInt();
	     nrParams    = dataStream.readInt();
   	     totalLength = dataStream.readInt();
             }
	   
	  void addParameter(int size)
	     {
	     nrParams++;
	     totalLength += size;
	     }   
         }
       

