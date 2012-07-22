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
#ifndef RNPRASSERVER_HH
#define RNPRASSERVER_HH

#include "rnprotocol/rnprotocol.hh"

using namespace rnp;


/**
  * \ingroup Rnprotocols
  */
class RnpRasserver : public Rnp
{
public:

    static const RnpQuark serverID; // 3072002
    enum ParameterType
    {
        pmt_none,
        pmt_clientid     = 1,
        pmt_rErrorString = 2,
        pmt_dbname       = 3,
        pmt_accesmode    = 4,
        pmt_querystring  = 5,
        pmt_httpqanswer  = 6,
        pmt_oidstring    = 7,
        pmt_capability   = 8,
        pmt_transstatus  = 9,
        pmt_objecttype   =10,
        pmt_returnstatus =11,
        pmt_skalarobject =12,
        pmt_tiledata     =13,
        pmt_domain       =14,
        pmt_typename     =15,
        pmt_typelength   =16,
        pmt_typetype     =17,
        pmt_typestructure = 18,
        pmt_collname    = 19,
        pmt_whichformat = 20,
        pmt_format      = 21,
        pmt_formatparams = 22,
        pmt_currentformat = 23,
        pmt_storageformat = 24,
        pmt_ispersistent  = 25,
        pmt_errorno       = 26,
        pmt_lineno        = 27,
        pmt_columnno      = 28,
        pmt_errortoken    = 29,
        pmt_indextype     = 30,
        //.......
        pmt_HowMany
    };

    enum Command
    {
        cmd_none,
        cmd_connect     =  1,
        cmd_disconnect  =  2,
        cmd_opendb      =  3,
        cmd_closedb     =  4,
        cmd_beginta     =  5,
        cmd_committa    =  6,
        cmd_abortta     =  7,
        cmd_istaopen    =  8,
        cmd_queryhttp   =  9,
        cmd_getnewoid   = 10,
        cmd_queryrpc    = 11,
        cmd_getnextelem = 12,
        cmd_endtransfer = 13,
        cmd_getnextmdd  = 14,
        cmd_getnexttile = 15,
        cmd_updaterpc   = 16,
        cmd_startinsTmdd = 17,
        cmd_inserttile  = 18,
        cmd_endinsmdd   = 19,
        cmd_initupdate  = 20,
        cmd_gettypestruct = 21,
        cmd_startinsPmdd = 22,
        cmd_insertmdd    = 23,
        cmd_insertcoll   = 24,
        cmd_remobjfromcoll = 25,
        cmd_delobjbyoid    = 26,
        cmd_delcollbyname  = 27,
        cmd_getcoll        = 28,
        cmd_getcolloids    = 29,
        cmd_getobjecttype  = 30,
        cmd_setformat      = 31,
        //-- until here the compatibility functions --

        cmd_createcollection = 32,
        cmd_createmdd        = 33,
        cmd_extendmdd        = 34,
        cmd_gettiledomains   = 35,
        cmd_HowMany
    };

    const char* getParameterTypeName(RnpQuark) const throw();
    const char* getCommandName(RnpQuark)       const throw();

protected:
    /// Arrays containing the names of the various elements
    static const char* parameterTypeNames[pmt_HowMany];
    static const char* commandNames[cmd_HowMany];

};
#endif
