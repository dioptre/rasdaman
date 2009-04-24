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
/*  http-fields.c - get and set HTTP Header fieldnames.                   */
/*------------------------------------------------------------------------*/


#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"


struct KeywordKey FieldnameKeyTable[] =
{
  { "Accept"                  , HKEY_Accept },
  { "Accept-Charset"          , HKEY_Accept_Charset },
  { "Accept-Encoding"         , HKEY_Accept_Encoding },
  { "Accept-Language"         , HKEY_Accept_Language },
  { "Accept-Ranges"           , HKEY_Accept_Ranges },
  { "Accept-charset"          , HKEY_Accept_Charset },
  { "Accept-encoding"         , HKEY_Accept_Encoding },
  { "Accept-language"         , HKEY_Accept_Language },
  { "Accept-ranges"           , HKEY_Accept_Ranges },
  { "Age"                     , HKEY_Age },
  { "Allow"                   , HKEY_Allow },
  { "Authorization"           , HKEY_Authorization },
  { "Cache-Control"           , HKEY_Cache_Control },
  { "Cache-control"           , HKEY_Cache_Control },
  { "Compliance"              , HKEY_Compliance },
  { "Connection"              , HKEY_Connection },
  { "Content-Base"            , HKEY_Content_Base },
  { "Content-Encoding"        , HKEY_Content_Encoding },
  { "Content-Language"        , HKEY_Content_Language },
  { "Content-Length"          , HKEY_Content_Length },
  { "Content-Location"        , HKEY_Content_Location },
  { "Content-MD5"             , HKEY_Content_MD5 },
  { "Content-Range"           , HKEY_Content_Range },
  { "Content-Type"            , HKEY_Content_Type },
  { "Content-base"            , HKEY_Content_Base },
  { "Content-encoding"        , HKEY_Content_Encoding },
  { "Content-language"        , HKEY_Content_Language },
  { "Content-length"          , HKEY_Content_Length },
  { "Content-location"        , HKEY_Content_Location },
  { "Content-range"           , HKEY_Content_Range },
  { "Content-type"            , HKEY_Content_Type },
  { "Date"                    , HKEY_Date },
  { "ETag"                    , HKEY_ETag },
  { "Expect"                  , HKEY_Expect },
  { "Expires"                 , HKEY_Expires },
  { "From"                    , HKEY_From },
  { "Host"                    , HKEY_Host },
  { "If-Modified-Since"       , HKEY_If_Modified_Since },
  { "If-Match"                , HKEY_If_Match },
  { "If-None-Match"           , HKEY_If_None_Match },
  { "If-Range"                , HKEY_If_Range },
  { "If-Unmodified-Since"     , HKEY_If_Unmodified_Since },
  { "Keep-Alive"              , HKEY_Keep_Alive },
  { "Last-Modified"           , HKEY_Last_Modified },
  { "Location"                , HKEY_Location },
  { "Max-Forwards"            , HKEY_Max_Forwards },
  { "Non-Compliance"          , HKEY_Non_Compliance },
  { "Pragma"                  , HKEY_Pragma },
  { "Proxy-Authenticate"      , HKEY_Proxy_Authenticate },
  { "Proxy-Authorization"     , HKEY_Proxy_Authorization },
  { "Public"                  , HKEY_Public },
  { "Range"                   , HKEY_Range },
  { "Referer"                 , HKEY_Referer },
  { "Retry-After"             , HKEY_Retry_After },
  { "Server"                  , HKEY_Server },
  { "Set-Proxy"               , HKEY_Set_Proxy },
  { "Transfer-Encoding"       , HKEY_Transfer_Encoding },
  { "Upgrade"                 , HKEY_Upgrade },
  { "User-Agent"              , HKEY_User_Agent },
  { "Vary"                    , HKEY_Vary },
  { "WWW-Authenticate"        , HKEY_WWW_Authenticate },
  { "Warning"                 , HKEY_Warning },
};

#define NUM_FIELDS  62


/****** http-fields/HTTP_GetHKey *********************************************
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

int HTTP_GetHKey( char *Keyword )
{
  int cond;
  int low; 
  int high;
  int mid;
  int check[ NUM_FIELDS ];
  int i;
  
  low  = 0;
  high = NUM_FIELDS - 1;
  for( i = 0; i < NUM_FIELDS; i++ )
    check[ i ] = 0;
  
  while( low <= high )
    {
      mid = ( low + high ) / 2;
      if( ( cond = strcmp( Keyword, FieldnameKeyTable[mid].Keyword ) ) < 0 )
	{
	  if( check[mid] == 0 )
	    {
	      check[mid] = 1;
	      high = mid - 1;
	    }
	  else
	    return( HKEY_UNKNOWN );
	}
      else if( cond > 0 )
	{
	  if( check[mid] == 0 )
	    {
	      check[mid] = 1;
	      low = mid + 1;
	    }
	  else
	    return( HKEY_UNKNOWN );
	}
      else 
	return( FieldnameKeyTable[mid].Key );
    }
  return( HKEY_UNKNOWN );
} 


/****** http-fields/HTTP_GetFieldName ****************************************
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

char *HTTP_GetFieldName( int Key )
{
  int i;
  
  for( i = 0; i < NUM_FIELDS; i++ )
    {
      if( FieldnameKeyTable[i].Key == Key )
	return( FieldnameKeyTable[i].Keyword );
    }
  return( NULL );
}
