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
   

public class RnpFragment
        {
        RnpFragmentHeader header;    
        Vector parameters;
	int parameterIndex = 0;
    
        RnpFragment(int fragmentType, int command)
            {
            header = new RnpFragmentHeader(fragmentType, command);	
	    parameters = new Vector();
            }
    
        RnpFragmentHeader getHeader()
            {   
            return header;
            }
       
        void addParameterInt32(int parameterType, int data)
            {
            addParameter(new ParameterInt32(parameterType,data));
            }
    
        void addParameterFloat32(int parameterType, float data)
            {
            addParameter(new ParameterFloat32(parameterType,data));
            }
    
        void addParameterDouble64(int parameterType, double data)
            {
            addParameter(new ParameterDouble64(parameterType,data));
            }
        void addParameterString(int parameterType, String data)
            {
            addParameter(new ParameterString(parameterType,data));
            }
    
        void addParameterOpaque(int parameterType, byte[] data)
            {
            addParameter(new ParameterOpaque(parameterType,data));
            }
    
        private void addParameter(RnpParameter param)
            {
            parameters.add(param);	
	    header.addParameter(param.getTotalLength());
            }   

        void write(DataOutputStream dataStream) throws IOException
            {
            header.write(dataStream);	
	    for(int i=0; i < header.countParameters(); i++)
	      {
	      RnpParameter param = (RnpParameter)parameters.get(i);	    
	      param.write(dataStream);
	      }
            }
    
        void read(DataInputStream dataStream) throws IOException, RnpException
            {
            header.read(dataStream);	
	    for(int i=0; i < header.countParameters(); i++)
	      {
	      RnpParameter param = RnpParameter.constructFromStream(dataStream);            
	      parameters.add(param);
	      }
            }
     
    void print()
            {
            header.print();
	    for(int i=0; i < header.countParameters(); i++)
	      {
  	      System.out.print(" " + i+ " ");
	      RnpParameter param = (RnpParameter)parameters.get(i);
	      param.print();
	      }
            }
     
 
      int getFragmentType()
            {
	    return header.fragmType;
	    }

      int getCommand()
            {
	    return header.command;
	    }

      int countParameters()
            {
	    return header.nrParams;
	    }

      int  getFragmentLength()
            {
	    return header.totalLength;
	    } 
     
     RnpParameter getFirstParameter()
            {
	     parameterIndex = 0;
	     return (RnpParameter)parameters.get(0);
	    }

     RnpParameter getNextParameter()
            {
	    parameterIndex++; 	 
	    return (RnpParameter)parameters.get(parameterIndex);
	    }
   
    }
    
