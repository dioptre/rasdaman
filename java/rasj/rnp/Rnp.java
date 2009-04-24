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
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */
package rasj.rnp;

import java.io.*;

public class Rnp
  {
    public static final int rnpProtocolId = 0xc12d7f01; // this is swap(25112001);
    
    public static final int fgt_None             = 0;
    public static final int fgt_Command          = 1;    
    public static final int fgt_OkAnswer         = 2;
    public static final int fgt_Error		 = 3;
    public static final int fgt_DiscardedRequest = 4;
    
    public static final int dtt_None             = 0;
    public static final int dtt_Asciiz           = 1;
    public static final int dtt_Int32            = 2;
    public static final int dtt_Float32          = 3;
    public static final int dtt_Double64         = 4;    
    public static final int dtt_Opaque           = 5;
  }

