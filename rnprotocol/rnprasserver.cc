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
#include "debug.hh"

#include "rnprasserver.hh"

// forever and ever
const RnpQuark RnpRasserver::serverID = 3072002;


const char* RnpRasserver::parameterTypeNames[pmt_HowMany] =
  {
    "(pmt_none)", "pmt_clientid"
   };
   
const char* RnpRasserver::commandNames[cmd_HowMany] =
  {
    "(cmd_none)",        "cmd_connect",           "cmd_disconnect",
    "cmd_opendb",        "cmd_closedb",           "cmd_beginta", 
    "cmd_committa",      "cmd_abortta",           "cmd_istaopen", 
    "cmd_queryhttp",     "cmd_getnewoid",         "cmd_queryrpc", 
    "cmd_getnextelem",   "cmd_endtransfer",       "cmd_getnextmdd", 
    "cmd_getnexttile",   "cmd_updaterpc",         "cmd_startinsTmdd", 
    "cmd_inserttile",    "cmd_endinsmdd",         "cmd_initupdate", 
    "cmd_gettypestruct", "cmd_startinsPmdd",      "cmd_insertmdd", 
    "cmd_insertcoll",    "cmd_removeobjfromcoll", "cmd_delobjbyoid", 
    "cmd_delcollbyname", "cmd_getcoll",           "cmd_getcolloids", 
    "cmd_getobjecttype", "cmd_setformat", 
    
    "cmd_createcollection", "cmd_createmdd", "cmd_extendmdd",
    "cmd_gettiledomains"   
   };

const char* RnpRasserver::getParameterTypeName(RnpQuark pType) const throw()
  {
    if(0<= pType && pType < pmt_HowMany) return parameterTypeNames[pType];
    return undefValue;
   }
   
const char* RnpRasserver::getCommandName(RnpQuark cmd) const throw()
  {
    const char *result = NULL;

    if(0<= cmd && cmd < cmd_HowMany)
        result = commandNames[cmd];
    else
        result = undefValue;

    TALK( "RnpRasserver::getCommandName( " << cmd << " ) -> " << result );
    return result;
   }

