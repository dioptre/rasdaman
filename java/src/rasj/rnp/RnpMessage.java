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
   

public class RnpMessage
  {
    public RnpMessageHeader header;
    public Vector fragments;
    public RnpFragment currentFragment = null;
    
    void write(DataOutputStream dataStream) throws IOException
       {
       header.write(dataStream);	
       for(int i=0; i < header.countFragments(); i++)
	  {
	  RnpFragment frag = (RnpFragment)fragments.get(i);
	  frag.write(dataStream);
	  }
       }
       
    void read(DataInputStream dataStream) throws IOException, RnpException
       {
       header = new RnpMessageHeader(0);
       header.read(dataStream);	
       fragments = new Vector();	
       for(int i=0; i < header.countFragments(); i++)
	 {
	 currentFragment = new RnpFragment(0,0);	    
	 currentFragment.read(dataStream);            
	 fragments.add(currentFragment);
	 }
       currentFragment = null;   
       }
    
    void print()
       {
       header.print();	
       for(int i=0; i < header.countFragments(); i++)
	   {
	   System.out.print("Fragment " + i + "  ");	  
	   RnpFragment frag = (RnpFragment)fragments.get(i);
	   frag.print();
	   }
       }
       
    }

