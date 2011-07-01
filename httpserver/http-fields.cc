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
  { (char*)"Accept"                  , HKEY_Accept },
  { (char*)"Accept-Charset"          , HKEY_Accept_Charset },
  { (char*)"Accept-Encoding"         , HKEY_Accept_Encoding },
  { (char*)"Accept-Language"         , HKEY_Accept_Language },
  { (char*)"Accept-Ranges"           , HKEY_Accept_Ranges },
  { (char*)"Accept-charset"          , HKEY_Accept_Charset },
  { (char*)"Accept-encoding"         , HKEY_Accept_Encoding },
  { (char*)"Accept-language"         , HKEY_Accept_Language },
  { (char*)"Accept-ranges"           , HKEY_Accept_Ranges },
  { (char*)"Age"                     , HKEY_Age },
  { (char*)"Allow"                   , HKEY_Allow },
  { (char*)"Authorization"           , HKEY_Authorization },
  { (char*)"Cache-Control"           , HKEY_Cache_Control },
  { (char*)"Cache-control"           , HKEY_Cache_Control },
  { (char*)"Compliance"              , HKEY_Compliance },
  { (char*)"Connection"              , HKEY_Connection },
  { (char*)"Content-Base"            , HKEY_Content_Base },
  { (char*)"Content-Encoding"        , HKEY_Content_Encoding },
  { (char*)"Content-Language"        , HKEY_Content_Language },
  { (char*)"Content-Length"          , HKEY_Content_Length },
  { (char*)"Content-Location"        , HKEY_Content_Location },
  { (char*)"Content-MD5"             , HKEY_Content_MD5 },
  { (char*)"Content-Range"           , HKEY_Content_Range },
  { (char*)"Content-Type"            , HKEY_Content_Type },
  { (char*)"Content-base"            , HKEY_Content_Base },
  { (char*)"Content-encoding"        , HKEY_Content_Encoding },
  { (char*)"Content-language"        , HKEY_Content_Language },
  { (char*)"Content-length"          , HKEY_Content_Length },
  { (char*)"Content-location"        , HKEY_Content_Location },
  { (char*)"Content-range"           , HKEY_Content_Range },
  { (char*)"Content-type"            , HKEY_Content_Type },
  { (char*)"Date"                    , HKEY_Date },
  { (char*)"ETag"                    , HKEY_ETag },
  { (char*)"Expect"                  , HKEY_Expect },
  { (char*)"Expires"                 , HKEY_Expires },
  { (char*)"From"                    , HKEY_From },
  { (char*)"Host"                    , HKEY_Host },
  { (char*)"If-Modified-Since"       , HKEY_If_Modified_Since },
  { (char*)"If-Match"                , HKEY_If_Match },
  { (char*)"If-None-Match"           , HKEY_If_None_Match },
  { (char*)"If-Range"                , HKEY_If_Range },
  { (char*)"If-Unmodified-Since"     , HKEY_If_Unmodified_Since },
  { (char*)"Keep-Alive"              , HKEY_Keep_Alive },
  { (char*)"Last-Modified"           , HKEY_Last_Modified },
  { (char*)"Location"                , HKEY_Location },
  { (char*)"Max-Forwards"            , HKEY_Max_Forwards },
  { (char*)"Non-Compliance"          , HKEY_Non_Compliance },
  { (char*)"Pragma"                  , HKEY_Pragma },
  { (char*)"Proxy-Authenticate"      , HKEY_Proxy_Authenticate },
  { (char*)"Proxy-Authorization"     , HKEY_Proxy_Authorization },
  { (char*)"Public"                  , HKEY_Public },
  { (char*)"Range"                   , HKEY_Range },
  { (char*)"Referer"                 , HKEY_Referer },
  { (char*)"Retry-After"             , HKEY_Retry_After },
  { (char*)"Server"                  , HKEY_Server },
  { (char*)"Set-Proxy"               , HKEY_Set_Proxy },
  { (char*)"Transfer-Encoding"       , HKEY_Transfer_Encoding },
  { (char*)"Upgrade"                 , HKEY_Upgrade },
  { (char*)"User-Agent"              , HKEY_User_Agent },
  { (char*)"Vary"                    , HKEY_Vary },
  { (char*)"WWW-Authenticate"        , HKEY_WWW_Authenticate },
  { (char*)"Warning"                 , HKEY_Warning },
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
