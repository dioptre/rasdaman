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
/*  http-readmsg.c - generic HTTP-Message functions:                      */
/*         Reading and parsing of HTTP-Messages.                          */
/*------------------------------------------------------------------------*/
/*  Comments:                                                             */
/*      - Status:                                                         */
/*          - Missing symbolic constant BUFFBLOCK_SIZE.                   */
/*          - To check: All Buffers free()ed? Also in error cases?        */
/*          - ParseReqLine()/ParseRespLine() use static buffers, but      */
/*            they are protected against overwriting.                     */
/*------------------------------------------------------------------------*/


#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"


/****** http-readmsg/ReadHeader **********************************************
*                                                                             
*   NAME                                                                      
*       ReadHeader -- Read a HTTP message header from a socket.               
*                                                                             
*   SYNOPSIS                                                                  
*       char *ReadHeader( int SockFD, size_t *size );                         
*                                                                             
*   FUNCTION                                                                  
*       This function reads from the socket descriptor "SockFD" until it      
*       detects an empty line (two <CR><LF> sequences in a row). Every read   
*       character is stored in a buffer, which is dynamically allocated.      
*       If the empty line is detected, it returns a pointer to the buffer     
*       and the variable "*size" contains the buffersize.                     
*                                                                             
*   INPUTS                                                                    
*       SockFD - The socket descriptor to read from.                          
*       size - A pointer to a variable which may hold the size of the buffer  
*           which contains the message header.                                
*                                                                             
*   RESULT                                                                    
*       A pointer a buffer with the HTTP message header.                      
*                                                                             
*   NOTES                                                                     
*       The function checks for errors returned by the "read()" system call.  
*       Other problems ("out-of-band" data, socket errors) are not checked.   
*                                                                             
******************************************************************************
*
*/

int ReadHeader( int SockFD, char **Buffer, size_t *BuffSize )
{
  size_t  BuffBlock = 1024; 
  size_t  minbuff   = 2;
  size_t  nread     = 0;
  size_t  sumread   = 0;
  char   *Ptr;
  int     check_eol = 0;
  int     error     = FALSE;
  int     elapsed   = 0;
  
  for(;;)
    {
      if( *BuffSize - sumread < minbuff )
	{
	  *BuffSize += BuffBlock;
	  *Buffer = (char*)realloc( *Buffer, *BuffSize );
	  if( *Buffer == NULL )
	    {
	      ErrorMsg( E_SYS, ERROR, "ERROR: ReadHeader(): malloc() failed." );
	      error = TRUE;
	      break;
	    }
          bzero( *Buffer, *BuffSize );
	}

      Ptr = *Buffer + sumread;
      nread = read( SockFD, Ptr, 1 );
      if( nread == 1 )
	{
	  elapsed = 0;          /*  Clear timeout counter  */
	  sumread++;
	  if( *Ptr == '\n' )    /* Check for two consecutive "\r\n"  */
	    {
	      if( check_eol == 1 )
		break;          /* if found => End-Of-Header  */
	      else
		check_eol = 1;
	    }
	  else if( *Ptr != '\r' )
	    check_eol = 0;
	}
      else if( nread == 0 )
	{
	  if( elapsed >= 30 )
	    {
	      ErrorMsg( E_PRIV, ERROR, "ERROR: ReadHeader() timed out." );
	      error = TRUE;
	      break;
	    }
	  sleep(1);
	  elapsed++;
	}
      else
	{
	  if( errno != EAGAIN )
	    {
	      ErrorMsg( E_SYS, ERROR, "ERROR: ReadHeader(): read() failed." );
	      error = TRUE;
	      break;
	    }
	  else
	    {
	      sleep( elapsed );
	      if( elapsed >= 30 )
		{
		  ErrorMsg( E_PRIV, ERROR, "ERROR: ReadHeader() timed out." );
		  error = TRUE;
		  break;
		}
	      sleep(1);
	      elapsed++;
	    }
	}
    }
  if( error == TRUE )
    {
      if( *Buffer != NULL )
	free( *Buffer );
      *Buffer = NULL;
      *BuffSize = 0;
      return( RI_READ_ERROR );
    }
  else
    return( RI_READ_OK );
}


/****** http-readmsg/ReadBody ************************************************
*                                                                             
*   NAME                                                                      
*       ReadBody -- Read a HTTP message body from a socket.                   
*                                                                             
*   SYNOPSIS                                                                  
*       char *ReadBody( int SockFD, size_t BuffSize );                        
*                                                                             
*   FUNCTION                                                                  
*       This functions reads the HTTP message body with the size "BuffSize"   
*       from the socket "SockFD". A buffer for the message body is allocated  
*       within this function and a pointer to this buffer is returned.        
*                                                                             
*   INPUTS                                                                    
*       SockFD - The socket descriptor to read from.                          
*       BuffSize - The size of the message body.                              
*                                                                             
*   RESULT                                                                    
*       A pointer to the buffer which contains the message body.              
*                                                                             
*   NOTES                                                                     
*       The function checks for errors returned by the "read()" system call.  
*       Other problems ("out-of-band" data, socket errors) are not checked.   
*                                                                             
******************************************************************************
*
*/

char *ReadBody( int SockFD, size_t BuffSize )
{
  int     nread     = 0;
  char   *Buffer    = NULL;

  if( ( Buffer = (char*)mymalloc( BuffSize + 1 ) ) == NULL )
    {
      ErrorMsg( E_SYS, ERROR, "ERROR: malloc error for HTTP-Body buffer." );
      return( Buffer );
    }
  bzero( Buffer, BuffSize + 1);
  nread = ReadN( SockFD, Buffer, BuffSize );
  if( nread < 0 )
    {
      ErrorMsg( E_SYS, ERROR, "ERROR: ReadBody(): read() failed." );
      free( Buffer );
      Buffer = NULL;
      return( Buffer );
    }
  else if( nread < BuffSize )
    {
      ErrorMsg( E_PRIV, WARN, "WARN:  MessageBody not of expected size." );
    }
  return( Buffer );
}


/****** http-readmsg/ParseReqHeader ******************************************
*                                                                             
*   NAME                                                                      
*       ParseReqHeader - Parse the HTTP request header.                       
*                                                                             
*   SYNOPSIS                                                                  
*       rc_t ParseReqHeader( struct ReqInfo *Request );                       
*                                                                             
*   FUNCTION                                                                  
*       This is the main function for parsing the HTTP request headers.       
*       It first calls "ParseReqLine()" to actually parse the "Request        
*       Line" (the first line of an HTTP request) and then calls              
*       subsequently "ParseMsgLine()" for every non-empty line following      
*       the request-line.                                                     
*                                                                             
*       The results of this parsing process are stored in a data structure    
*       of the type "ReqInfo", which is later used for searching in and       
*       reconstructing of the header informations.                            
*                                                                             
*   INPUTS                                                                    
*       Request - A pointer to a data structure of the type ReqInfo.          
*                                                                             
*   RESULT                                                                    
*       Returns "OK" if everything went well.                                 
*                                                                             
*   BUGS                                                                      
*       It doesn't check if "Request" is a NULL pointer.                      
*                                                                             
*   SEE ALSO                                                                  
*       ParseRespHeader(), ParseReqLine(), ParseMsgLine().                    
*                                                                             
******************************************************************************
*
*/

rc_t ParseReqHeader( struct ReqInfo *Request )
{
  char   *Buffer; 
  char   *Param;
  int     Key;
  struct  MsgHeader *Header;

  Header = NULL;
  Buffer = Request->HeadBuff;
  if( Request->State == RI_READ_OK )
    {
      Buffer = ParseReqLine( Buffer, Request );
      //LogMsg( LG_SERVER, DEBUG, "DEBUG: ***  Parsed Req %s", Buffer ); 
      if( Request->State == RI_PARSE_ERROR )
	return( WARN );
    }
  else
    {
      Request->State = RI_PARSE_ERROR;
      return( WARN );
    }
  
  while( Buffer != NULL && *Buffer != '\0' )
    {
      Key = -1;
      Param = NULL;
      Buffer = ParseMsgLine( Buffer, &Key, &Param );
      //LogMsg( LG_SERVER, DEBUG, "DEBUG: ***  Parsed Msg %s", Buffer ); 
      if( Buffer != NULL )
	{
	  Header = AppendMsgHeader( Request->Last, Key, Param );
	  Request->Last = Header;
	}
      if( Request->First == NULL )
	Request->First = Header;
    }
  if( Request->First == NULL )
    {
      Request->State = RI_PARSE_WARN;
      return( WARN );
    }
  else
    {
      Request->State = RI_PARSE_OK;
      return( OK );
    }
}


/****** http-readmsg/ParseRespHeader *****************************************
*                                                                             
*   NAME                                                                      
*       ParseRespHeader - Parse the HTTP response header.                     
*                                                                             
*   SYNOPSIS                                                                  
*       rc_t ParseRespHeader( struct RespInfo *Response );                    
*                                                                             
*   FUNCTION                                                                  
*       This is the main function for parsing the HTTP response headers.      
*       It first calls "ParseRespLine()" to actually parse the "Response      
*       Line" (the first line of an HTTP response) and then calls             
*       subsequently "ParseMsgLine()" for every non-empty line following      
*       the request-line.                                                     
*                                                                             
*       The results of this parsing process are stored in a data structure    
*       of the type "RespInfo", which is later used for searching in and      
*       reconstructing of the header informations.                            
*                                                                             
*   INPUTS                                                                    
*       Response - A pointer to a data structure of the type RespInfo.        
*                                                                             
*   RESULT                                                                    
*       Returns "OK" if everything went well.                                 
*                                                                             
*   BUGS                                                                      
*       Actually, it always returns an "OK" status code.                      
*       It doesn't check if "Response" is a NULL pointer.                     
*                                                                             
*   SEE ALSO                                                                  
*       ParseReqHeader(), ParseRespLine(), ParseMsgLine().                    
*                                                                             
******************************************************************************
*
*/

rc_t ParseRespHeader( struct RespInfo *Response )
{
  char   *Buffer; 
  char   *Keyword;
  char   *Param;
  size_t  i;
  int     Key;
  struct MsgHeader *Header;

  Header = NULL;
  Buffer = Response->HeadBuff;
  Buffer = ParseRespLine( Buffer, Response );
  
  while( Buffer != NULL )
    {
      Key = -1;
      Param = NULL;
      Buffer = ParseMsgLine( Buffer, &Key, &Param );
      if( Buffer != NULL )
	{
	  Header = AppendMsgHeader( Response->Last, Key, Param );
	  Response->Last = Header;
	}
      if( Response->First == NULL )
	Response->First = Header;
    }
  return( OK );
}


/****** http-readmsg/ParseReqLine ********************************************
*                                                                             
*   NAME                                                                      
*       ParseReqLine - Parse the HTTP request line.                           
*                                                                             
*   SYNOPSIS                                                                  
*       char *ParseReqLine( char *Buffer, struct ReqInfo *Request );          
*                                                                             
*   FUNCTION                                                                  
*       This function parses the beginning of the buffer "Buffer" for the     
*       contents of a HTTP request line. Since the parsing process (as        
*       implemented here) is destructive to the original string, the buffer   
*       is first scanned for a <CR><LF> sequence (end-of-line mark of a       
*       HTTP message) while copying any other character encountered to a      
*       secondary buffer. This "Backup" of the request line is later used     
*       for logging purposes, so it can be referred to in case of a problem.  
*                                                                             
*       Then the request line is parsed and broken up in its components, as   
*       they are: Request method, URL, protocol-id and version. These are     
*       stored in the appropriate places in a "ReqInfo" data structure.       
*                                                                             
*   INPUTS                                                                    
*       Buffer - A pointer to the raw data of a HTTP request.                 
*       Request - A pointer to a data structure of the type ReqInfo.          
*                                                                             
*   RESULT                                                                    
*       A pointer into the buffer, pointing to the rest of the data to be     
*       processed.                                                            
*                                                                             
*   BUGS                                                                      
*       "Buffer" and "Request" are not checked for NULL pointers.             
*       The "Backup" of the request line has a maximum length of              
*       "MAXLINELEN".                                                         
*                                                                             
*   SEE ALSO                                                                  
*       ParseRespLine().                                                      
*                                                                             
******************************************************************************
*
*/

char *ParseReqLine( char *Buffer, struct ReqInfo *Request )
{
  char  Tmp[ MAXLINELEN ];
  char *NewBuffer;
  char *Ptr;
  char *Keyword;
  char *URL      = NULL;
  char *PVersion = NULL;
  int   Key      = 0;
  int   PVmaj    = -1;
  int   PVmin    = -1;
  int   i;
  int   eol_found = FALSE;

  Keyword = Buffer;

  for( Ptr = Buffer, i = 0; i < MAXLINELEN; Ptr++, i++ )
    {
      if( *Ptr == '\r')
	Tmp[i] = '\0';
      else if( *Ptr == '\n' )
	{
	  Tmp[i] = '\0';
	  eol_found = TRUE;
	  break;
	}
      else
	Tmp[i] = *Ptr;
    }
  Tmp[i+1] = '\0';
  if( eol_found != TRUE )
    {
      Request->State = RI_PARSE_ERROR;
      return( NULL );
    }
  
  for( Ptr = Buffer; *Ptr != '\n'; Ptr++ )
    {
      if( *Ptr == ' ' )
        {
          if( URL == NULL )
            {
              *Ptr = '\0';
              URL = ++Ptr;
            }
          else 
            {
              *Ptr = '\0';
              ++Ptr;
              if( strcmp( "HTTP/", Ptr ) )
                {
                  Ptr += 5;
                  PVersion = Ptr;
                }
            }
        }
      else if( *Ptr == '\r' )
        {
          *Ptr = '\0';
        }
    }
  *Ptr = '\0';
  NewBuffer = ++Ptr;

  if( PVersion != NULL )
    {
      for( Ptr = PVersion; *Ptr != '\0'; Ptr++ )
        {
          if( *Ptr == '.' )
            {
              *Ptr = '\0';
              ++Ptr;
              break;
            }
        }
      PVmaj = strtol( PVersion, NULL, 10 );
      PVmin = strtol( Ptr,      NULL, 10 );
    }

  if( URL != NULL )
    {
      Request->Line.Vanilla       = (char*)mymalloc( strlen( Tmp ) + 1 );
      Request->Line.Method        = HTTP_GetMKey( Keyword );
      //      SplitURL( URL, &Request->Line.URL );
      Request->Line.URL.Path      = URL;
      Request->Line.Version.Major = PVmaj;
      Request->Line.Version.Minor = PVmin;
      strcpy( Request->Line.Vanilla, Tmp );
      Request->State = RI_PARSE_OK;
      return( NewBuffer );
    }
  else
    {
      Request->State = RI_PARSE_ERROR;
      return( NULL );
    }
}


/****** http-readmsg/ParseRespLine *******************************************
*                                                                             
*   NAME                                                                      
*       ParseRespLine - Parse the HTTP response line.                         
*                                                                             
*   SYNOPSIS                                                                  
*       char *ParseRespLine( char *Buffer, struct RespInfo *Response );       
*                                                                             
*   FUNCTION                                                                  
*       This function parses the beginning of the buffer "Buffer" for the     
*       contents of a HTTP response line. Since the parsing process (as       
*       implemented here) is destructive to the original string, the buffer   
*       is first scanned for a <CR><LF> sequence (end-of-line mark of a       
*       HTTP message) while copying any other character encountered to a      
*       secondary buffer. This "Backup" of the response line is later used    
*       for logging purposes, so it can be referred to in case of a problem.  
*                                                                             
*       Then the response line is parsed and broken up in its components, as  
*       they are: Protocol-id and version, (numeric) status code and          
*       "reason phrase". These are stored in the appropriate places in a      
*       "RespInfo" data structure.                                            
*                                                                             
*   INPUTS                                                                    
*       Buffer - A pointer to the raw data of a HTTP request.                 
*       Response - A pointer to a data structure of the type RespInfo.        
*                                                                             
*   RESULT                                                                    
*       A pointer into the buffer, pointing to the rest of the data to be     
*       processed.                                                            
*                                                                             
*   BUGS                                                                      
*       "Buffer" and "Response" are not checked for NULL pointers.            
*       The "Backup" of the response line has a maximum length of             
*       "MAXLINELEN".                                                         
*                                                                             
*   SEE ALSO                                                                  
*       ParseReqLine().                                                       
*                                                                             
******************************************************************************
*
*/

char *ParseRespLine( char *Buffer, struct RespInfo *Response )
{
  char  Tmp[ MAXLINELEN ];
  char *Ptr;
  char *Reason   = NULL;
  char *PVersion = NULL;
  char *Status   = NULL;
  char *PVmaj    = NULL;
  char *PVmin    = NULL;
  int   i;

  for( Ptr = Buffer, i = 0; i <= MAXLINELEN - 1; Ptr++, i++ )
    {
      if( *Ptr == '\r')
	Tmp[i] = '\0';
      else if( *Ptr == '\n' )
	{
	  Tmp[i] = '\0';
	  break;
	}
      else
	Tmp[i] = *Ptr;
    }
  Tmp[i+1] = '\0';

  Response->Line.Vanilla = (char*)mymalloc( strlen( Tmp ) + 1 );
  strcpy( Response->Line.Vanilla, Tmp );

  if( strcmp( "HTTP/", Buffer ) )
    {
      PVmaj = Buffer + 5;
      for( Ptr = PVmaj; *Ptr != ' '; Ptr++ )
	{
	  if( *Ptr == '.' )
	    {
	      *Ptr = '\0';
	      PVmin = ++Ptr;
	    }
	}
      *Ptr = '\0';
      Response->Line.Version.Major = strtol( PVmaj, NULL, 10 );
      Response->Line.Version.Minor = strtol( PVmin, NULL, 10 );
    }
  else
    return( NULL );

  Status = ++Ptr;
  while( *Ptr != ' ' )
    Ptr++;
  *Ptr = '\0';
  Response->Line.Status = strtol( Status, NULL, 10 );

  Ptr++;
  Response->Line.Reason = Ptr;

  while( *Ptr != '\n' )
    {
      if( *Ptr == '\r' )
	*Ptr = '\0';
      Ptr++;
    }
  *Ptr = '\0';

  return( ++Ptr );
}


/****** http-readmsg/ParseMsgLine ********************************************
*                                                                             
*   NAME                                                                      
*       ParseMsgLine - Parse a HTTP message header line.                      
*                                                                             
*   SYNOPSIS                                                                  
*       char *ParseMsgLine( char *Buffer, int *Key, char **Param );           
*                                                                             
*   FUNCTION                                                                  
*       This function parses a HTTP message header field into the             
*       components field name and field content ("value" of this field).      
*       The appropriate token for the field name is stored in the variable    
*       where "Key" points to and "Param" points to the field content         
*       string.                                                               
*                                                                             
*   INPUTS                                                                    
*       Buffer - A pointer to the raw data of a HTTP request.                 
*       Key - A pointer to a variable which will store the field name token.  
*       Param - A pointer to a character pointer, which will refer to         
*               the field content string.                                     
*                                                                             
*   RESULT                                                                    
*       A pointer into the buffer, pointing to the rest of the data to be     
*       processed.                                                            
*                                                                             
*   NOTES                                                                     
*       In case of unknown field names, the field content string will         
*       contain not only the value of the field, instead it will contain the  
*       complete header line.                                                 
*                                                                             
*   BUGS                                                                      
*       "Buffer", "Key" and "Param" are not checked against NULL pointers.    
*                                                                             
******************************************************************************
*
*/

char *ParseMsgLine( char *Buffer, int *Key, char **Param )
{
  char *Keyword;
  char *Ptr;
  int   HaveKey = FALSE;

  if( ( *Buffer == ' ' ) || ( *Buffer == '\t' ) )
    {
      *Key  = HKEY_CONTINUE;
      *Param = Buffer;

      for( Ptr = Buffer; *Ptr != '\n'; Ptr++ )
	{
	  if( *Ptr == '\r' )
	    *Ptr = '\0';
	  else if( *Ptr == '\0' )
	    break;
	}
      *Ptr = '\0';
      return( ++Ptr );
    }

  Keyword = Buffer;
  for( Ptr = Buffer; *Ptr != '\n'; Ptr++ )
    {
      if( *Ptr == ':' )
	{
	  if( HaveKey == FALSE )
	    {
	      HaveKey = TRUE;
	      *Ptr = '\0';
	      *Key = HTTP_GetHKey( Keyword );
	      if( *Key != HKEY_UNKNOWN )
		{
		  Ptr++;
		  while( ( *Ptr == ' ' ) || ( *Ptr == '\t' ) )
		    ++Ptr;
		  *Param = Ptr;
		}
	      else
		{
		  *Ptr = ':';
		  *Param = Buffer;
		}
	    }
	}
      else if( *Ptr == '\r' )
	*Ptr = '\0';
      else if( *Ptr == '\0' )
	break;
    }
  *Ptr = '\0';

  if( *Key == -1 || HaveKey == FALSE )
    return( NULL );
  else
    return( ++Ptr );
}


/****** http-readmsg/splitURL ************************************************
*                                                                             
*   NAME                                                                      
*       splitURL - break up an URL string into its components.                
*                                                                             
*   SYNOPSIS                                                                  
*       void splitURL( char *Buffer, struct URLComps *URL );                  
*                                                                             
*   FUNCTION                                                                  
*       This function searches in a linked list of "MsgHeader" elements       
*       for one with a "Content_Length" token, and returns the integer        
*       value of it.                                                          
*                                                                             
*   INPUTS                                                                    
*       Buffer - the URL string.                                              
*       URL    - a pointer to structure, which holds the pointers to the      
*                the strings of the components.                               
*                                                                             
*   RESULT                                                                    
*       If the creation of a buffer for the components fails, all components  
*       of URL will have NULL pointers. If no protocol is found in Buffer,    
*       URL->Path will contain the complete contents of Buffer (it is         
*       assumed that Buffer conatins a relative URL). In this case            
*       URL->Extra whill always be NULL.                                      
*       URL->Servername may be NULL, if there was no Servername separator     
*       ("//") found - this may be the case for URLs pointing to "this"       
*       Server ("localhost").                                                 
*                                                                             
******************************************************************************
*
*/

void SplitURL( char *Buffer, struct URLComps *URL )
{
  char   *NewBuffer;
  size_t  BuffSize;
  char   *Ptr1;
  char   *Ptr2;
  char   *Tmp;
  int     HaveProto  = FALSE;
  int     CheckSep   = FALSE;
  int     HaveServer = FALSE;
  int     HavePath   = FALSE;
  int     HaveExtra  = FALSE;

  BuffSize = strlen( Buffer ) + 4;
  NewBuffer = (char*)mymalloc( BuffSize );
  if( NewBuffer != NULL )
    {
      bzero( NewBuffer, BuffSize );
      Tmp = NewBuffer;
      for( Ptr1 = Buffer, Ptr2 = NewBuffer; Ptr1 <= Buffer + BuffSize; Ptr1++, Ptr2++ )
	{
	  if( HaveProto == FALSE )
	    {
	      if( *Ptr1 == ':' )
		{
		  HaveProto = TRUE;
		  URL->Protocol = Tmp;
		  Tmp = Ptr2 + 1;
		}
	      else
		{
		  *Ptr2 = *Ptr1;
		}
	    }
	  else if( CheckSep == FALSE )
	    {
	      CheckSep = TRUE;
	      if( *Ptr1 == '/' && *(Ptr1+1) == '/' )
		{
		  Ptr1++;
		}
	      else
		{
		  HaveServer = TRUE;
		  URL->Servername = NULL;
		  Ptr1--;
		}
	    }
	  else if( HaveServer == FALSE )
	    {
	      if( *Ptr1 == '/' )
		{
		  HaveServer = TRUE;
		  URL->Servername = Tmp;
		  Tmp = Ptr2 + 1;
		  Ptr1--;
		}
	      else
		{
		  *Ptr2 = *Ptr1;
		}
	    }
	  else if( HavePath == FALSE )
	    {
	      if( *Ptr1 == '#' || *Ptr1 == '?' )
		{
		  HavePath = TRUE;
		  URL->Path = Tmp;
		  Tmp = Ptr2 + 1;
		  Ptr1--;
		}
	      else
		{
		  *Ptr2 = *Ptr1;
		}
	    }
	  else
	    {
	      if( HaveExtra == FALSE )
		{
		  HaveExtra = TRUE;
		  URL->Extra = Tmp;
		}
	      *Ptr2 = *Ptr1;
	    }
	}
      if( HaveProto == FALSE )
	{
	  URL->Path = Tmp;
	}
    }
  else
    {
      URL->Protocol   = NULL;
      URL->Servername = NULL;
      URL->Path       = NULL;
      URL->Extra      = NULL;
    }

  return;
}


/****** http-readmsg/GetContentLength ****************************************
*                                                                             
*   NAME                                                                      
*       GetContentLength - search for the "Content-Length" header in a HTTP   
*           message and return the value if found.                            
*                                                                             
*   SYNOPSIS                                                                  
*       size_t GetContentLength( struct MsgHeader *Ptr );                     
*                                                                             
*   FUNCTION                                                                  
*       This function searches in a linked list of "MsgHeader" elements       
*       for one with a "Content_Length" token, and returns the integer        
*       value of it.                                                          
*                                                                             
*   INPUTS                                                                    
*       Ptr - A pointer to a "MsgHeader" node.                                
*                                                                             
*   RESULT                                                                    
*       0 if no "Content-Length" header is found, otherwise the value of      
*       this message header converted into an integer (more precisely: the    
*       return value of "strtol()").                                          
*                                                                             
*   BUGS                                                                      
*       The return value of "strtol()" should maybe checked for proper        
*       results.                                                              
*                                                                             
*   SEE ALSO                                                                  
*       GetFieldContent().                                                    
*                                                                             
******************************************************************************
*
*/

size_t GetContentLength( struct MsgHeader *Ptr )
{
  while( Ptr != NULL )
    {
      if( Ptr->Field == HKEY_Content_Length )
	return( strtol( Ptr->Content, NULL, 10 ) );
      Ptr = Ptr->Next;
    }
  return( 0 );
}


/****** http-readmsg/GetFieldContent *****************************************
*                                                                             
*   NAME                                                                      
*       GetFieldContent - Return the content of a specific message header.    
*                                                                             
*   SYNOPSIS                                                                  
*       char *GetFieldContent( struct MsgHeader *Ptr, int Field,              
*                              struct MsgHeader **Next );                     
*                                                                             
*   FUNCTION                                                                  
*       This function searches in a linked list of "MsgHeader" elements       
*       for one with the token in the argument "Field", and returns the       
*       value of it.                                                          
*                                                                             
*   INPUTS                                                                    
*       Ptr - A pointer to a "MsgHeader" node.                                
*       Field - The token of the field to search for.                         
*       Next - A pointer to the next "MsgHeader" in the list.                 
*                                                                             
*   RESULT                                                                    
*       NULL if the header is not found, otherwise a pointer to the content   
*       string.                                                               
*                                                                             
*   SEE ALSO                                                                  
*       GetContentLength().                                                   
*                                                                             
******************************************************************************
*
*/

char *GetFieldContent( struct MsgHeader *Ptr, int Field, struct MsgHeader **Next )
{
  while( Ptr != NULL )
    {
      if( Ptr->Field == Field )
	{
	  if( Next != NULL )
	    *Next = Ptr->Next;
	  return( Ptr->Content );
	}
      Ptr = Ptr->Next;
    }
  return( NULL );
}


/****** http-readmsg/GetRealm ************************************************
*                                                                             
*   NAME                                                                      
*       GetRealm - Search for specific Authentication realms.                 
*                                                                             
*   SYNOPSIS                                                                  
*       int GetRealm( char *String );                                         
*                                                                             
*   FUNCTION                                                                  
*       This function searches in the "WWW-Authenticate" header for specific  
*       authentication realm strings. If one of them is found, the            
*       appropriate code for this realm is returned.                          
*                                                                             
*   INPUTS                                                                    
*       String - A pointer to the contents of the "WWW-Authenticate" header   
*               field.                                                        
*                                                                             
*   RESULT                                                                    
*       A realm type code.                                                    
*                                                                             
*   BUGS                                                                      
*       "String" is not checked against a NULL pointer.                       
*                                                                             
******************************************************************************
*
*/

int GetRealm( char *String )
{
  struct KeywordKey RealmKeyTable[] =
  {
    { "httpserver-IPClass-A", REALM_IPCLASS_A },
    { "httpserver-IPClass-B", REALM_IPCLASS_B },
    { "httpserver-IPClass-C", REALM_IPCLASS_C },
    { "httpserver-IPAddress", REALM_IPADDRESS },
    { "httpserver-Hostname",  REALM_HOSTNAME },
    { "httpserver-Domain",    REALM_DOMAIN },
  };
#define NUM_REALMS 6

  char  *Buff;
  char  *Ptr;
  char  *Tmp;
  int    i;
  
  Ptr = String;
  if( ( strncasecmp( "realm=", Ptr, 6 ) == 0 ) )
    {
      Ptr = Ptr + 6;
      if( ( Buff = (char*)mymalloc( strlen( Ptr ) + 1 ) ) == NULL )
	return( REALM_ERROR ); 
      strcpy( Buff, Ptr );

      /*  Kill Quotes...    */
      if( *Ptr == '"' )
	{
	  Ptr++;
	  Tmp = strrchr( Ptr, '"' );
	  if( Tmp != NULL )
	    *Tmp = '\0';
	}
      for( i = 0; i < NUM_REALMS; i++ )
	{
	  if( strcmp( RealmKeyTable[i].Keyword, Ptr ) == 0 )
	    {
	      free( Buff );
	      return( RealmKeyTable[i].Key );
	    }
	}
      return( REALM_UNKNOWN );
    }
  else
    return( REALM_ERROR );
}
