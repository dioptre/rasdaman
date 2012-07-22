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

#include "raslib/error.hh"
#include "raslib/rminit.hh"

#include   "httpserver.h"

#include   "defs.h"
#include   "protos.h"
#include   "types.h"
#include   "server.h"

#ifdef PURIFY
#include <purify.h>
#endif

/*--- Global Data Section: ------------------------------------------------*/
/*      All we need to remember or may want to know is in this structure   */

struct ServerBase     Server;

struct Logging       *LogBase = &Server.Log;

/****** main/main ************************************************************
*
*   NAME
*       main -- the main function of this program.
*
*   SYNOPSIS
*       void main( int argc, char *argv[] );
*
*   FUNCTION
*       "main" calls "Initialize()" for setting up the program, activates
*       the main socket with the "listen()" system call, and then waits
*       forever for a client to connect. If a client connection request is
*       received, a child process will be spawned and the child will handle
*       the request while the parent process will wait again for new
*       connection requests.
*
*   INPUTS
*       argc - The number of arguments given on the command line.
*       argv[] - An array of strings, containing the arguments given on the
*           command line.
*
*   NOTES
*       The program can only be stopped by sending it the corresponding
*       signals ("SIGQUIT", "SIGTERM").
*
*   BUGS
*       None known. Not here.
*
*   SEE ALSO
*       Initialize(), Accept(), AddChild(), InitChild(), HandleRequest().
*
******************************************************************************
*
*/

extern int init_httpserver( int argc, char *argv[] )
{
    pid_t ChildPId;          /* -> Server.ChildInfo   */

    RMInit::logOut << "Initialising parameters for HTTP server... " << endl;
    RMInit::logOut.flush();
    Initialize( argc, argv, &Server );

    RMInit::logOut << "Initialising server socket for HTTP server... " << endl;
    RMInit::logOut.flush();
    listen( Server.SockFD, 5 );
    RMInit::logOut << "Waiting for client calls... " << endl;
    RMInit::logOut.flush();

#ifdef PURIFY
    purify_printf( "Server Startup Finnished." );
    purify_new_leaks();
#endif

    /*   // this is a quick hack for testing */
    /*   return 0; */
    for(;;)
    {
        Accept( Server.SockFD, &Server.Client );

        strcpy( Server.Client.Host.IPAddrString, inet_ntoa( Server.Client.Socket.sin_addr ) );

        if( Server.Client.Host.IPAddrString == NULL )
            strcpy( Server.Client.Host.IPAddrString, "0.0.0.0" );

        Server.Client.Host.IPAddress = inet_addr( Server.Client.Host.IPAddrString );
        Server.Client.Comm.ConnStatus      = CONN_UNDEFINED;
        InitHTTPMsg( &Server.Client.Response );
        InitReqInfo( &Server.Client.Request );

        LogMsg( LG_SERVER, INFO, "INFO:  ====== Connection from %s accepted...",
                Server.Client.Host.IPAddrString );
        HandleRequest( &Server.Client );
        LogMsg( LG_SERVER, INFO, "INFO:  ====== EOT. Disconnecting." );

        close( Server.Client.SockFD );

    }
    // otherwise Exit(OK) should have been called
    return -1;
}



/****** main/Exit ************************************************************
*
*   NAME
*       Exit -- Cleanup when exiting.
*
*   SYNOPSIS
*       void Exit( int RC );
*
*   FUNCTION
*       This function closes all open socket descriptors and the logging
*       subsystem. If the calling process is the parent process, it also
*       deletes the file where the process Id is stored.
*       After that, it calls the "exit()" function with the return code
*       "RC".
*
*   INPUTS
*       RC - return or exit code, indicating the error condition of the
*           program.
*
*   NOTES
*       This function is called by the signal handlers for "SIGQUIT",
*       "SIGTERM", "SIGINT" and "SIGHUP" and the main() function.
*       In future versions, "SIGHUP" should be changed to reread the
*       configuration file instead of calling Exit().
*
*   BUGS
*       The global variable "SubServer[]" should not be used.
*
*   SEE ALSO
*       Exit().
*
******************************************************************************
*
*/

int Exit( int RC )
{
    int i;

    //  for( i = 0; i < NUM_SUBSERVER; i++ )
    //    close( SubServer[i].SockFD );
    close( Server.SockFD );

    if( getpid() == Server.PId )  /*  Is this the parent process?  */
    {
        unlink( Server.PidFile );
        LogMsg( LG_SERVER, DEBUG, "DEBUG: Main process exiting (RC=%d)...", RC );
        LogMsg( LG_SERVER, INFO,
                "INFO:  ========= %s terminated. ============", DAEMONNAME );
    }
    else
    {
        LogMsg( LG_SERVER, DEBUG, "DEBUG: Exiting (RC=%d)...", RC );
    }

    CloseLog( &Server.Log );

    free(Server.Directory);

    if(RC != OK) throw r_Error( r_Error::r_Error_General );
    return RC;
}

