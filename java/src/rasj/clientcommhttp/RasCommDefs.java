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

public interface RasCommDefs
{
  static final String rcsid = "@(#)Package rasj.clientcommhttp, class RasCommDefs: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/clientcommhttp/RasCommDefs.java,v 1.5 2003/12/10 21:04:26 rasdev Exp $";

 final public byte RASCLIENT = 1;
 final public byte BROWSER   = 2;

 final public byte RESPONSE_ERROR       = 0;
 final public byte RESPONSE_MDDS        = 1;
 final public byte RESPONSE_SKALARS     = 2;
 final public byte RESPONSE_INT         = 3;
 final public byte RESPONSE_OID         = 4;
 final static byte RESPONSE_OK_NEGATIVE = 98;
 final public byte RESPONSE_OK          = 99;

 final public byte BIG_ENDIAN    = 0;
 final public byte LITTLE_ENDIAN = 1;
}
