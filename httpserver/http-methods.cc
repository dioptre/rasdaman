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
/*------------------------------------------------------------------------*/
/*  http-methods.c - get and set HTTP Method names.                       */
/*------------------------------------------------------------------------*/


#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"

#include <ctype.h>

struct KeywordKey MethodKeyTable[] =
{
  { (char*)"GET",      MKEY_GET },
  { (char*)"HEAD",     MKEY_HEAD },
  { (char*)"PUT",      MKEY_PUT },
  { (char*)"POST",     MKEY_POST },
  { (char*)"DELETE",   MKEY_DELETE },
  { (char*)"OPTIONS",  MKEY_OPTIONS },
  { (char*)"TRACE",    MKEY_TRACE },
};


/****** http-methods/HTTP_GetMKey ********************************************
*                                                                             
*   NAME                                                                      
*       
*                                                                             
*   SYNOPSIS                                                                  
*       
*                                                                             
*   FUNCTION                                                                  
*       
*                                                                             
*   INPUTS                                                                    
*       
*                                                                             
*   RESULT                                                                    
*       
*                                                                             
*   NOTES                                                                     
*       
*                                                                             
*   BUGS                                                                      
*       
*                                                                             
*   SEE ALSO                                                                  
*       
*                                                                             
******************************************************************************
*
*/

int HTTP_GetMKey( char *Keyword )
{
  int cond;
  int low; 
  int high;
  int mid;
  int check[ NUM_MKEYS ];
  int i;

  low  = 0;
  high = NUM_MKEYS - 1;
  for( i = 0; i < NUM_MKEYS; i++ )
    check[ i ] = 0;
  if( !isalpha( *Keyword ) )
    return( MKEY_NONE );
  while( low <= high )
    {
      mid = ( low + high ) / 2;
      if( ( cond = strcmp( Keyword, MethodKeyTable[mid].Keyword ) ) < 0 )
	{
	  if( check[mid] == 0 )
	    {
	      check[mid] = 1;
	      high = mid - 1;
	    }
	  else
	    return( MKEY_UNKNOWN );
	}
      else if( cond > 0 )
	{
	  if( check[mid] == 0 )
	    {
	      check[mid] = 1;
	      low = mid + 1;
	    }
	  else
	    return( MKEY_UNKNOWN );
	}
      else 
	return( MethodKeyTable[mid].Key );
    }
  return( MKEY_UNKNOWN );
} 


/****** http-methods/HTTP_GetMethodName **************************************
*                                                                             
*   NAME                                                                      
*       
*                                                                             
*   SYNOPSIS                                                                  
*       
*                                                                             
*   FUNCTION                                                                  
*       
*                                                                             
*   INPUTS                                                                    
*       
*                                                                             
*   RESULT                                                                    
*       
*                                                                             
*   NOTES                                                                     
*       
*                                                                             
*   BUGS                                                                      
*       
*                                                                             
*   SEE ALSO                                                                  
*       
*                                                                             
******************************************************************************
*
*/

char *HTTP_GetMethodName( int Key )
{
  int i;
  
  for( i = 0; i < NUM_MKEYS; i++ )
    {
      if( MethodKeyTable[i].Key == Key )
	return( MethodKeyTable[i].Keyword );
    }
  return( NULL );
}
