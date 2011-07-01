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

#include "mymalloc/mymalloc.h"
/*------------------------------------------------------------------------*/
/*  http-error.c - HTTP Error message handling.                           */
/*------------------------------------------------------------------------*/
/*  Comments:                                                             */
/*      - Status:                                                         */
/*          - The internal response strings should be completed and       */
/*            the overly long lines should be broken up.                  */
/*          - A somewhat more dynamic approach to the handling of the     */
/*            error string table should be used.                          */
/*------------------------------------------------------------------------*/

#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"

struct HTTPError HTTPErrorTable[] =
 {
   { 
     STATUS_Continue,
     (char*)"Continue",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Switching_Protocols,
     (char*)"Switching Protocols",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_OK,
     (char*)"OK",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Created,
     (char*)"Created",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Accepted,
     (char*)"Accepted",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Non_Authoritative_Information,
     (char*)"Non-Authoritative Information",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_No_Content,
     (char*)"No Content",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Reset_Content,
     (char*)"Reset Content",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Partial_Content,
     (char*)"Partial Content",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Multiple_Choices,
     (char*)"Multiple Choices",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Moved_Permanently,
     (char*)"Moved Permanently",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Moved_Temporarily,
     (char*)"Moved Temporarily",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_See_Other,
     (char*)"See Other",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Not_Modified,
     (char*)"Not Modified",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Use_Proxy,
     (char*)"Use Proxy",
     (char*)"",
     (char*)""
   },
   { 
     STATUS_Bad_Request,
     (char*)"Bad Request",
     (char*)"<HTML><HEAD>\n<TITLE>400 Bad Request</TITLE>\n</HEAD>"\
           "<BODY>\n<H1>Bad Request</H1>\n"\
           "Your browser sent a request that this server could not understand.\n"\
           "</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>400 Bad Request</TITLE>\n</HEAD>"\
           "<BODY>\n<H1>Problem mit Request</H1>\n"\
           "Ihr Browser hat einen Request geschickt, der von diesem Server "\
           "nicht verstanden wurde.\n"\
          "</BODY></HTML>\n"
  },
  { 
    STATUS_Unauthorized,
     (char*)"Unauthorized",
     (char*)"<HTML><HEAD>\n<TITLE>401 Unauthorized</TITLE>\n</HEAD>"\
           "<BODY>\n<H1>Unauthorized</H1>\n"\
           "You have not the necessary rights to get the requested document.\n"\
           "</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>401 Unauthorized</TITLE>\n</HEAD>"\
           "<BODY>\n<H1>Nicht Authorisiert</H1>\n"\
           "Sie haben nicht die notwendigen Rechte um auf das angeforderte "\
           "Dokument zuzugreifen.\n"\
          "</BODY></HTML>\n"
   },
   { 
     STATUS_Payment_Required,
     (char*)"Payment Required",
     (char*)"<HTML><HEAD>\n<TITLE>402 Payment Required</TITLE>\n</HEAD><BODY>\n<H1>Payment Required</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>402 Payment Required</TITLE>\n</HEAD><BODY>\n<H1>Payment Required</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Forbidden,
     (char*)"Forbidden",
     (char*)"<HTML><HEAD>\n<TITLE>403 Forbidden</TITLE>\n</HEAD><BODY>\n<H1>Forbidden</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>403 Forbidden</TITLE>\n</HEAD><BODY>\n<H1>Forbidden</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Not_Found,
     (char*)"Not Found",
     (char*)"<HTML><HEAD>\n<TITLE>404 Not Found</TITLE>\n</HEAD>"\
           "<BODY>\n<H1>Not Found</H1>\n"\
           "<EM>Tried hard, really...</EM>\n"\
           "</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>404 Not Found</TITLE>\n</HEAD>"\
           "<BODY>\n<H1>Nicht Gefunden</H1>\n"\
           "Das angeforderte Dokument wurde nicht gefunden.\n"\
           "</BODY></HTML>\n"
   },
   { 
     STATUS_Method_Not_Allowed,
     (char*)"Method Not Allowed",
     (char*)"<HTML><HEAD>\n<TITLE>405 Method Not Allowed</TITLE>\n</HEAD><BODY>\n<H1>Method Not Allowed</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>405 Method Not Allowed</TITLE>\n</HEAD><BODY>\n<H1>Method Not Allowed</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Not_Acceptable,
     (char*)"Not Acceptable",
     (char*)"<HTML><HEAD>\n<TITLE>406 Not Acceptable</TITLE>\n</HEAD><BODY>\n<H1>Not Acceptable</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>406 Not Acceptable</TITLE>\n</HEAD><BODY>\n<H1>Not Acceptable</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Proxy_Authentication_Required,
     (char*)"Proxy Authentication Required",
     (char*)"<HTML><HEAD>\n<TITLE>407 Proxy Authentication Required</TITLE>\n</HEAD><BODY>\n<H1>Proxy Authentication Required</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>407 Proxy Authentication Required</TITLE>\n</HEAD><BODY>\n<H1>Proxy Authentication Required</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Request_Timeout,
     (char*)"Request Timeout",
     (char*)"<HTML><HEAD>\n<TITLE>408 Request Timeout</TITLE>\n</HEAD><BODY>\n<H1>Request Timeout</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>408 Request Timeout</TITLE>\n</HEAD><BODY>\n<H1>Request Timeout</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Conflict,
     (char*)"Conflict",
     (char*)"<HTML><HEAD>\n<TITLE>409 Conflict</TITLE>\n</HEAD><BODY>\n<H1>Conflict</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>409 Conflict</TITLE>\n</HEAD><BODY>\n<H1>Conflict</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Gone,
     (char*)"Gone",
     (char*)"<HTML><HEAD>\n<TITLE>410 Gone</TITLE>\n</HEAD><BODY>\n<H1>Gone</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>410 Gone</TITLE>\n</HEAD><BODY>\n<H1>Gone</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Length_Required,
     (char*)"Length Required",
     (char*)"<HTML><HEAD>\n<TITLE>411 Length Required</TITLE>\n</HEAD><BODY>\n<H1>Length Required</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>411 Length Required</TITLE>\n</HEAD><BODY>\n<H1>Length Required</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Precondition_Failed,
     (char*)"Precondition Failed",
     (char*)"<HTML><HEAD>\n<TITLE>412 Precondition Failed</TITLE>\n</HEAD><BODY>\n<H1>Precondition Failed</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>412 Precondition Failed</TITLE>\n</HEAD><BODY>\n<H1>Precondition Failed</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Request_Entity_Too_Large,
     (char*)"Request Entity Too Large",
     (char*)"<HTML><HEAD>\n<TITLE>413 Request Entity Too Large</TITLE>\n</HEAD><BODY><H1>Request Entity Too Large\n</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>413 Request Entity Too Large</TITLE>\n</HEAD><BODY><H1>Request Entity Too Large\n</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Request_URI_Too_Long,
     (char*)"Request-URI Too Long",
     (char*)"<HTML><HEAD>\n<TITLE>414 Request-URI Too Long</TITLE>\n</HEAD><BODY><H1>Request-URI Too Long\n</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>414 Request-URI Too Long</TITLE>\n</HEAD><BODY><H1>Request-URI Too Long\n</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Unsupported_Media_Type,
     (char*)"Unsupported Media Type",
     (char*)"<HTML><HEAD>\n<TITLE>415 Unsupported Media Type</TITLE>\n</HEAD><BODY>\n<H1>Unsupported Media Type</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>415 Unsupported Media Type</TITLE>\n</HEAD><BODY>\n<H1>Unsupported Media Type</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Requested_Range_Not_Valid,
     (char*)"Requested Range Not Valid",
     (char*)"<HTML><HEAD>\n<TITLE>416 Requested Range Not Valid</TITLE>\n</HEAD><BODY>\n<H1>Requested Range Not Valid</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>416 Requested Range Not Valid</TITLE>\n</HEAD><BODY>\n<H1>Requested Range Not Valid</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Internal_Server_Error,
     (char*)"Internal Server Error",
     (char*)"<HTML><HEAD>\n<TITLE>500 Internal Server Error</TITLE>\n</HEAD><BODY>\n<H1>Internal Server Error</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>500 Internal Server Error</TITLE>\n</HEAD><BODY>\n<H1>Internal Server Error</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Not_Implemented,
     (char*)"Not Implemented",
     (char*)"<HTML><HEAD>\n<TITLE>501 Not Implemented</TITLE>\n</HEAD><BODY>\n<H1>Not Implemented</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>501 Not Implemented</TITLE>\n</HEAD><BODY>\n<H1>Not Implemented</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Bad_Gateway,
     (char*)"Bad Gateway",
     (char*)"<HTML><HEAD>\n<TITLE>502 Bad Gateway</TITLE>\n</HEAD><BODY>\n<H1>Bad Gateway</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>502 Bad Gateway</TITLE>\n</HEAD><BODY>\n<H1>Bad Gateway</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Service_Unavailable,
     (char*)"Service Unavailable",
     (char*)"<HTML><HEAD>\n<TITLE>503 Service Unavailable</TITLE>\n</HEAD><BODY>\n<H1>Service Unavailable</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>503 Service Unavailable</TITLE>\n</HEAD><BODY>\n<H1>Service Unavailable</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_Gateway_Timeout,
     (char*)"Gateway Timeout",
     (char*)"<HTML><HEAD>\n<TITLE>504 Gateway Timeout</TITLE>\n</HEAD><BODY>\n<H1>Gateway Timeout</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>504 Gateway Timeout</TITLE>\n</HEAD><BODY>\n<H1>Gateway Timeout</H1>\n</BODY></HTML>\n"
   },
   { 
     STATUS_HTTP_Version_Not_Supported,
     (char*)"HTTP Version Not Supported",
     (char*)"<HTML><HEAD>\n<TITLE>505 HTTP Version Not Supported</TITLE>\n</HEAD><BODY>\n<H1>HTTP Version Not Supported</H1>\n</BODY></HTML>\n",
     (char*)"<HTML><HEAD>\n<TITLE>505 HTTP Version Not Supported</TITLE>\n</HEAD><BODY>\n<H1>HTTP Version Not Supported</H1>\n</BODY></HTML>\n"
   },
 };

#define NUM_STATUSCODES 37


/****** http-error/GetHTTPErrorTableEntry ************************************
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

int GetHTTPErrorTableEntry( int Code )
{
  int low; 
  int high;
  int mid;
  int check[ NUM_STATUSCODES ];
  int i;

  low  = 0;
  high = NUM_STATUSCODES - 1;
  for( i = 0; i < NUM_STATUSCODES; i++ )
    check[ i ] = 0;
    
  while( low <= high )
    {
      mid = ( low + high ) / 2;
      if( Code < HTTPErrorTable[mid].Code )
	{
	  if( check[mid] == 0 )
	    {
	      check[mid] = 1;
	      high = mid - 1;
	    }
	  else
	    return( 0 );
	}
      else if( Code > HTTPErrorTable[mid].Code )
	{
	  if( check[mid] == 0 )
	    {
	      check[mid] = 1;
	      low = mid + 1;
	    }
	  else
	    return( 0 );
	}
      else
	{
	  return( mid );
	}
    }
}


/****** http-error/CreateHTTPError *******************************************
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

rc_t CreateHTTPError( int Code, struct HTTPMode *Mode, struct HTTPMsg *Msg )
{
  char           *Head;
  char           *Buff;
  size_t          BuffSize = BUFFSIZE;
  char           *Body;
  size_t          BodySize;
  size_t          Used;

  int             Entry;
  char            Date[DATE_BUFFSIZE];

  bzero( Date, DATE_BUFFSIZE );
  if( ( Head = (char*)mymalloc( BuffSize ) ) == NULL )
    {
      ErrorMsg( E_SYS, ERROR, 
		"ERROR: CreateHTTPError() - malloc() error for head buffer." );
      return( ERROR );
    }
  Buff = Head;
  BuffSize -= 1;

  HTTP_Date( Date, DATE_BUFFSIZE );
  Entry     = GetHTTPErrorTableEntry( Code );
  Body      = HTTPErrorTable[Entry].Message_en;
  BodySize  = strlen( HTTPErrorTable[Entry].Message_en );

  CreateStatusLine( Buff, &BuffSize, Code, Mode->Protocol );
  Used = strlen( Head );
  Buff = Head + Used;
  BuffSize -= Used;
  SNPrintf( Buff, &BuffSize, 
	    "Date: %s\r\n" 
	    "Connection: close\r\n"
	    "%s"
	    "Content-Type: text/html\r\n"
	    "Content-Language: %s\r\n"
	    "Content-Length: %d\r\n\r\n",
	    Date, SERVERFIELD, "en", BodySize );

  Msg->Head = Head;
  Msg->Body = strdup(Body);
  Msg->BodySize = BodySize;

  return( OK );
}
