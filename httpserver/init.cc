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
/*  init.c - Functions to setup the server.                               */
/*------------------------------------------------------------------------*/

#include <iostream>

#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"

extern struct ServerBase    Server;


/****** init/Initialize ******************************************************
*                                                                             
*   NAME                                                                      
*       Initialize -- central function for initializing the servers data      
*           structures and environment.                                       
*                                                                             
*   SYNOPSIS                                                                  
*       int Initialize( int argc, char *argv[], struct ServerBase *Server );  
*                                                                             
*   FUNCTION                                                                  
*                                                                             
*   RESULT                                                                    
*                                                                             
*   NOTES                                                                     
*                                                                             
*   BUGS                                                                      
*                                                                             
*   SEE ALSO                                                                  
*                                                                             
******************************************************************************
*
*/

rc_t Initialize( int argc, char *argv[], struct ServerBase *Server )
{

  /*  Make sure that the Server structure is empty.  -------------------- */
  bzero( (char *)Server, sizeof( *Server ) );

  /*  Create Head of ChildList  */
  Server->ChildList = (struct ChildBase*)mymalloc( sizeof( struct ChildBase ) );
  if( Server->ChildList == NULL )
    ErrorMsg( E_SYS, FAIL, "FAIL:  Initialize(): malloc() failed!" );
  else
    {
      //LogMsg( LG_SERVER, DEBUG, "DEBUG:  Creating Head of ChildList" );
      Server->ChildList->next = Server->ChildList;
      Server->ChildList->prev = Server->ChildList;
      Server->ChildList->PId  = 0;
    }

  /* Read the environment variables */
  char* dummy = CONFDIR;
  if( dummy == NULL)
    ErrorMsg( E_SYS, FAIL, "FAIL:  configure error: CONFDIR not provided." );
  Server->Directory = (char*)mymalloc(strlen(dummy) + 2);
  strcpy(Server->Directory, dummy);
  if( Server->Directory[strlen(Server->Directory)] != '/' )
    strcat( Server->Directory, "/" );

  /*  Read Arguments and Server configuration file - and setup  --------- */
  /*      the corresponding data structures.  --------------------------- */

  ReadArgs( Server, argc, argv );
  //ReadConfig( Server );

  ConfigureServer( Server );
  // CheckConfig( Server );  // Check for reasonable configuration

  /*  Set timeout for communication with subservers and client  --------- */
  Server->Client.TimeOut.tv_sec  = DIALOG_TIMEOUT;
  Server->Client.TimeOut.tv_usec = 0;

  /*  Get the process into "daemon"-mode.  ------------------------------ */
  InitDaemon( Server->Log.Mode );
  Server->PId = getpid();

  //SavePId( Server->PidFile );
  /*  Initialize the Server Socket.  ------------------------------------ */
  InitSocket( &Server->SockFD, &Server->Socket, Server->Port );

  /*  Setup and Open the logfiles.  ------------------------------------- */
  OpenLog( &Server->Log, Server->Log.Access.Filename, 
	                 Server->Log.Server.Filename, 
	                 Server->Log.Comm.Filename );


  LogMsg( LG_SERVER, INFO, "INFO:  ========= %s started. ===============", DAEMONNAME );

  // We don't do that so that the one process running can be exited with simple ^C
  /*  Initialize the signal handlers.  ---------------------------------- */
  // InitSigHandler();

  /*  Everything is just great...  -------------------------------------- */
  return( OK );
}


/****** init/InitDaemon ******************************************************
*                                                                             
*   NAME                                                                      
*       InitDaemon -- going into "daemon"-mode: process forks to run in the   
*           background and closes all open file-descriptors.                  
*                                                                             
*   SYNOPSIS                                                                  
*       int InitDaemon( int Mode );                                           
*                                                                             
*   FUNCTION                                                                  
*                                                                             
*   RESULT                                                                    
*                                                                             
*   NOTES                                                                     
*                                                                             
*   BUGS                                                                      
*                                                                             
*   SEE ALSO                                                                  
*                                                                             
******************************************************************************
*
*/

rc_t InitDaemon( int Mode )
{
  int i;
  pid_t pid;
  int openmax;

  // RasDaMan does not go into background!
  chdir( Server.Directory );            /* Arbeitsverzeichnis wechseln */
  umask( 0 );                           /* Dateischutzbitmaske loeschen */
  return( OK );
}


/****** init/InitSocket ******************************************************
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
*       ###  Minimum Socket Setup. Stay tuned.                                
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

rc_t InitSocket( int *SockFD, struct sockaddr_in *Socket, int Port )
{
  int val = 0;
  size_t len;

  Socket->sin_family       = AF_INET;
  Socket->sin_addr.s_addr  = htonl( INADDR_ANY );
  Socket->sin_port         = htons( Port );
  if( ( *SockFD = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    ErrorMsg( E_SYS, FAIL, "FAIL:  Open socket for server." );

#ifdef SO_REUSEADDR
  val = 1;
  len = sizeof( val );
  if( setsockopt( *SockFD, SOL_SOCKET, SO_REUSEADDR, (char*)&val, len ) < 0 )
    ErrorMsg( E_SYS, WARN, "WARN:  InitSocket(): can't set sockopt REUSEADDR." );
#else
  LogMsg( LG_SERVER, INFO, "INFO:  InitSocket(): sockopt REUSEADDR not available." );
#endif

  if( bind( *SockFD, (struct sockaddr *)Socket, sizeof( *Socket ) ) < 0 )
    ErrorMsg( E_SYS, FAIL, "FAIL:  Bind socket for server." );
  return( OK );
}


/****** init/InitClientSocket ************************************************
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

rc_t InitClientSocket( int *SockFD, 
		      struct sockaddr_in *Socket, 
		      char *Hostname, 
		      int Port )
{
  struct hostent *Host;  

  bzero( (char *)Socket, sizeof( *Socket ) );

  Socket->sin_family       = AF_INET;
  Host = gethostbyname( Hostname );
  if( Host == NULL )
    {
      return( ERROR );
    }
  memcpy( (char *)&Socket->sin_addr, (char *)Host->h_addr, Host->h_length );

  Socket->sin_port         = htons( Port );
  if( ( *SockFD = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    return( ERROR );
  return( OK );
}


/****** init/SavePId *********************************************************
*                                                                             
*   NAME                                                                      
*       SavePId -- store process id in file.                                  
*                                                                             
*   SYNOPSIS                                                                  
*       void SavePId( char *PIdFilename );                                    
*                                                                             
*   FUNCTION                                                                  
*       THis function saves the parent process id in a file, so this file     
*       can be used by for example shell scripts to identify the correct      
*       process to signal it.                                                 
*                                                                             
*   INPUTS                                                                    
*       PIdFilename -- Full pathname of the file which stortes the pid.       
*                                                                             
*   RESULT                                                                    
*       Does not return anything.                                             
*                                                                             
*   NOTES                                                                     
*       This function is called by Initialize(), right after the logs are     
*       opened. So the content of the pidfile is also only valid afterwards.  
*       To ensure, that there is no stale pidfile, it is deleted on normal    
*       termination.                                                          
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


void SavePId( char *PIdFilename )
{
  FILE  *PIdFile;

  if( ( PIdFile = fopen( PIdFilename, "w" ) ) != NULL )
    {
      fprintf( PIdFile, "%d", getpid() );
      fclose( PIdFile );
    }
}
