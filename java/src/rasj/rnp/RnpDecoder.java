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

import java.io.*;


public class RnpDecoder
  {
    
     public RnpMessage    message;
     private RnpFragment  currentFragment;
     private RnpParameter currentParameter;
     private int          fragmentIndex;
      
     
     RnpDecoder(RnpMessage rnpmessage)       
        {
        message=rnpmessage;  
	currentFragment = null;
	currentParameter = null;
	fragmentIndex = 0;
        } 
     
     void verify() throws RnpException
	{
	if(message.header.protocolId != Rnp.rnpProtocolId) 
	       throw new RnpException("Not a RNP message");
	    
	if(message.header.messageEndianness != 0)
	       throw new RnpException("Incorrect endianness");
            
	if(message.header.majorVersion != 1 || message.header.minorVersion != 0 || message.header.dataStart != RnpMessageHeader.length)
	       throw new RnpException("Incorrect message version");
	}

     int getServerType()
        {
	return message.header.serverType;
	}

     byte getDesiredEndianness()
        {
	return message.header.desiredEndianness;
	}

     byte getMessageEndianness()
        {
	return message.header.messageEndianness;
	}

     int getMessageLength()
        {  
        return message.header.totalMessageLength;
      	}
     
     int countFragments()
        {
        return message.header.nrFragments;
	}
     byte getMajorVersion()
        {
	return  message.header.majorVersion;
	}
     byte getMinorVersion()
        {
	return  message.header.minorVersion;
	}
		  
     RnpFragment getFirstFragment()
        {
	fragmentIndex = 0;
	currentFragment = (RnpFragment)message.fragments.get(0);
	return currentFragment;
	}

    RnpFragment getNextFragment(RnpFragment fragment)
        {
	fragmentIndex++;
	currentFragment = (RnpFragment)message.fragments.get(fragmentIndex);
	return currentFragment;
	}
	
    int getFragmentType()	
        {
	//fgt_None, fgt_Command, fgt_OkAnswer, fgt_Error, fgt_DiscardedRequest
	return currentFragment.getFragmentType();
	}
    int countParameters()
        {
	return currentFragment.countParameters();
	} 
    RnpParameter getFirstParameter()
        {
	currentParameter = currentFragment.getFirstParameter();
	return currentParameter;
	}
    RnpParameter getNextParameter()
        {
	currentParameter = currentFragment.getNextParameter();
	return currentParameter;
	}
    int getParameterType() 
        { 
	return currentParameter.paramType; 
        }
   int getDataLength() 
        { 
	return currentParameter.getDataLength(); 
        }
		     
    int getDataType()      
        { 
	// dtt_None,dtt_Asciiz, dtt_Int32,dtt_Double64, dtt_Opaque
	return currentParameter.dataType;  
	}
	
    int getDataAsInteger()
        {
	return ((ParameterInt32)currentParameter).data;
        }
    float getDataAsFloat()
        {
	return((ParameterFloat32)currentParameter).data;
	} 
    double getDataAsDouble()
        {	
	return((ParameterDouble64)currentParameter).data;
	}
    String getDataAsString()
        {
	return new String(((ParameterString)currentParameter).data);
	}
    byte[] getDataOpaque()
        {
	return((ParameterOpaque)currentParameter).data;
	}				
    }

