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
 * SOURCE: rasmgr_error.cc
 *
 * MODULE: rasmgr
 * CLASS:  RCError and derivates
 *
 * COMMENTS:
 *   none
 *
*/

#include "rasmgr_error.hh"

#include "debug-srv.hh"


RCError::RCError()
 {
  }   
  

RCErrorUnexpToken::RCErrorUnexpToken(const char *token)
:pcc(token)
  {
   }
     
const char* RCErrorUnexpToken::getString(char *destBuffer)
  {
    sprintf(destBuffer,"Unexpected token %s in command.",pcc);
    return destBuffer;
   }


RCErrorNoPermission::RCErrorNoPermission()
  {
   }

const char* RCErrorNoPermission::getString(char *destBuffer)
  {
   sprintf(destBuffer,"You don't have permission for this operation.");
   return destBuffer;
   }   


RCErrorInvalidName::RCErrorInvalidName(const char *name)
:pcc(name)
  {
   }

const char* RCErrorInvalidName::getString(char *destBuffer)
  {
    sprintf(destBuffer,"Invalid %s name.",pcc);
    return destBuffer;
   }   


RCErrorMissingParam::RCErrorMissingParam(const char *what)
:pcc(what)
  {
   }

const char* RCErrorMissingParam::getString(char *destBuffer)
  {
    sprintf(destBuffer,"Missing %s.",pcc);
    return destBuffer;
   }


RCErrorIncorNumberValue::RCErrorIncorNumberValue(const char *what)
:pcc(what)
  {
   }
   
const char* RCErrorIncorNumberValue::getString(char *destBuffer)
  {
    sprintf(destBuffer,"Incorect number value for parameter %s.",pcc);
    return destBuffer;
   }

