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
/
/**
 * SOURCE: clientcomm.cc
 *
 * MODULE: clientcomm
 * CLASS:  ClientComm
 *
 * PURPOSE:
 *
 * COMMENTS:
 *		No Comments
*/

static const char rcsid[] = "@(#)clientcomm, ClientComm: $Id: clientcomm.cc,v 1.143 2005/09/09 16:16:29 rasdev Exp $";

using namespace std;

#include "clientcomm/clientcomm.hh"
#include "clientcomm/rpcclientcomm.hh"
#include "rnprotocol/rnpclientcomm.hh"


#include "raslib/endian.hh"
int
ClientComm::changeEndianness( r_GMarray* mdd, const r_Base_Type *bt )
{
  const r_Base_Type *baseType;
  const r_Minterval &interv = mdd->spatial_domain();

  baseType = (bt == NULL) ? mdd->get_base_type_schema() : bt;

  if (baseType == NULL )
  {
    cerr << "ClientComm::changeEndianness: No base type information!" << endl;
    return 0;
  }

  r_Endian::swap_array(baseType, interv, interv, mdd->get_array(), mdd->get_array());

  return 1;
}


int
ClientComm::changeEndianness( const r_GMarray* mdd, void *newMdd, const r_Base_Type* bt )
{
  const r_Base_Type *baseType;
  const r_Minterval &interv = mdd->spatial_domain();

  // Get the base type...
  baseType = (bt == NULL) ? ((r_GMarray*)mdd)->get_base_type_schema() : bt;

  if ( baseType == NULL )
  {
    cerr << "ClientComm::changeEndianness: No base type information!" << endl;
    memcpy( newMdd, mdd->get_array(), mdd->get_array_size());
    return 0;
  }

  r_Endian::swap_array(baseType, interv, interv, mdd->get_array(), newMdd);

  return 1;
}

ClientComm::ClientComm( ) throw( r_Error )
  {
    
   }

ClientComm* ClientComm::createObject(const char* rasmgrName, int rasmgrPort)
  {
    char *env = getenv("RMANPROTOCOL");
    
    bool createRNP = currentProtocolIsRNP;
    
    if(env != 0)
      {
        if(strcmp(env,"RNP") == 0 || strcmp(env,"HTTP") == 0)   createRNP = true;
        if(strcmp(env,"RPC") == 0 || strcmp(env,"COMPAT") == 0) createRNP = false;
	// rest is ignored
       }
	
    if(createRNP)
        return new RnpClientComm( rasmgrName, rasmgrPort);
    
    return new RpcClientComm(rasmgrName, rasmgrPort);
   }

ClientComm::~ClientComm() throw()
{
 }

// default comm protocol to be used:
//	true	use RNP
//	false	use RPC
bool ClientComm::currentProtocolIsRNP=true;

void ClientComm::useRNP() throw() { currentProtocolIsRNP = true;  }

void ClientComm::useRPC() throw() { currentProtocolIsRNP = false; }

bool ClientComm::internalSettingIsRNP()  throw() { return currentProtocolIsRNP;}



