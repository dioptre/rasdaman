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
 * PURPOSE: Stores Global information for the RasODMGInterface
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */
package rasj.odmg;

/**
 * This class provides some global defines and variables needed for the
 * ODMG implementation of the RasDaMan httpserver. This information is
 * only used for internal purposes and not visible to the user of the
 * ODMG interface.
 *
 * @version 1.0  (07-Apr-2000)
 *
 * @author Peter Zoller
 */
public abstract class RasODMGGlobal
{
    // client requests allowed
    public static final int commOpenDB      = 1;
    public static final int commCloseDB     = 2;
    public static final int commBTreadOnly  = 3;
    public static final int commBTreadWrite = 4;
    public static final int commCT          = 5;
    public static final int commAT          = 6;
    public static final int commIsOpenTA    = 7;
    public static final int commQueryExec   = 8;
    public static final int commUpdateQueryExec = 9;
    public static final int commGetNewOID = 10;
}


