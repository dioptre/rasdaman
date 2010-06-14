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
import java.util.*;

public class RnpEncoder
    {
     public int serverType               = 0;
     RnpMessage message = null;
     
     public void startMessage(int serverType)
         {
	 message=new RnpMessage();
	 message.header = new RnpMessageHeader(serverType);	
         message.fragments = new Vector();
         }
     public void setEndianess()      
         {
	 }	
     public void startFragment(int fragmentType, int command)
         {       
	 message.currentFragment = new RnpFragment(fragmentType,command);	
	 }
     void addFragment(RnpFragmentHeader fHeader)
	{
        message.header.nrFragments++;
	message.header.dataLength         += fHeader.totalLength;
	message.header.totalMessageLength += fHeader.totalLength;
	}
		
     void addParameterInt32(int parameterType, int data)
        {
        message.currentFragment.addParameterInt32(parameterType,data);
        }
       
    void addParameterFloat32(int parameterType, float data)
        {
        message.currentFragment.addParameterFloat32(parameterType,data);
        }
       
    void addParameterDouble64(int parameterType, double data)
        {
        message.currentFragment.addParameterDouble64(parameterType,data);
        }
       
    void addParameterString(int parameterType, String data)
        {
        message.currentFragment.addParameterString(parameterType,data);
        }
    
    void addParameterOpaque(int parameterType, byte[] data)
        {
        message.currentFragment.addParameterOpaque(parameterType,data);
        }
    
    void endFragment()
        {
        if(message.currentFragment == null) return; // correct "assert"
        message.fragments.add(message.currentFragment);
	addFragment(message.currentFragment.getHeader());
        message.currentFragment = null;
        }
    
    void endMessage()
        {
        }	
	
  }

