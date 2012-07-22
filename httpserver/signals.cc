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
/*  signals.c - signal handling stuff.                                    */
/*------------------------------------------------------------------------*/
/*
 * RCS:
 *   $RCSfile: signals.c,v $ $Revision: 1.8 $ $State: Exp $
 *   $Locker:  $
*/


#include   "defs.h"
#include   "protos.h"
#include   "server.h"

extern struct ServerBase Server;


/****** signals/Signal *******************************************************
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
*       Own Signal() function, based on sigaction().
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

sighandler Signal( int signo, sighandler x)
{
    struct sigaction SigAction;
    struct sigaction OldAction;

    SigAction.sa_handler = (void(*)(int))SigHandler;
    sigemptyset( & SigAction.sa_mask );
    SigAction.sa_flags = 0;

    if( signo == SIGALRM )
    {
#ifdef SA_INTERRUPT
        SigAction.sa_flags |= SA_INTERRUPT;      /*  SunOS  */
#endif
    }
    else
    {
#ifdef SA_RESTART
        SigAction.sa_flags |= SA_RESTART;        /*  SVR4, 4.3+ BSD  */
#endif
    }

    if( sigaction( signo, &SigAction, &OldAction ) < 0 )
        return( SIG_ERR );
    return( OldAction.sa_handler );
}


/****** signals/SigHandler ***************************************************
*
*   NAME
*       SigHandler -- Little Signal Handler.
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

void SigHandler( int Signal )
{
    //not used(see line 169) char   Buffer[PIPE_BUFFSIZE];
    int    errno_bak;
    int    ChildStatus;
    pid_t  ChildPId;
    struct ChildBase *Child = NULL;

    errno_bak = errno;
    switch( Signal )
    {
    case SIGHUP:
        LogMsg( LG_SERVER, INFO, "INFO:  Received 'SIGHUP'." );
        errno = errno_bak;
        Exit( OK );
        break;

    case SIGINT:
        LogMsg( LG_SERVER, INFO, "INFO:  Received 'SIGINT'." );
        errno = errno_bak;
        Exit( OK );
        break;

    case SIGQUIT:
        LogMsg( LG_SERVER, INFO, "INFO:  Received 'SIGQUIT'." );
        errno = errno_bak;
        Exit( OK );
        break;

    case SIGUSR1:
        LogMsg( LG_SERVER, INFO, "INFO:  Received 'SIGUSR1'." );
        //      SaveCache( Server.Cache, Server.CacheFile );
        break;

    case SIGUSR2:
        LogMsg( LG_SERVER, INFO, "INFO:  Received 'SIGUSR2'." );
        break;

    case SIGPIPE:
        LogMsg( LG_SERVER, INFO, "INFO:  Received 'SIGPIPE'." );
        break;

    case SIGTERM:
        LogMsg( LG_SERVER, INFO, "INFO:  Received 'SIGTERM'." );
        errno = errno_bak;
        Exit( OK );
        break;

    case SIGCHLD:
        LogMsg( LG_SERVER, INFO, "INFO:  Received 'SIGCHLD'." );
        ChildPId = waitpid( -1, &ChildStatus, WNOHANG );
        while( ChildPId > 0 )
        {
            if( WIFEXITED( ChildStatus ) )
            {
                /*-- Has Child send a Message?  --*/
                if( WEXITSTATUS( ChildStatus ) == NOTE )
                {
                    Child = GetChild( Server.ChildList, ChildPId );
                    if( Child != NULL )
                    {
                        /*
                        bzero( Buffer, PIPE_BUFFSIZE );
                        read( Child->PD[0], Buffer, PIPE_BUFFSIZE );
                        HandlePipeMsg( Buffer, Server.SubServerList, Server.Cache );
                        */
                    }
                }
                LogMsg( LG_SERVER, DEBUG,
                        "DEBUG: Child <%d> exited normally with status %d.",
                        ChildPId, WEXITSTATUS( ChildStatus ) );
            }
            else if( WIFSIGNALED( ChildStatus ) )
            {
                LogMsg( LG_SERVER, DEBUG,
                        "DEBUG: Child <%d> exited abnormally, signal number = %d.",
                        ChildPId, WTERMSIG( ChildStatus ) );
            }
            else if( WIFSTOPPED( ChildStatus ) )
            {
                LogMsg( LG_SERVER, DEBUG,
                        "DEBUG: Child <%d> is stopped, signal number = %d.",
                        ChildPId, WSTOPSIG( ChildStatus ) );
            }
            else
            {
                LogMsg( LG_SERVER, WARN,
                        "WARN:  Something strange with Child <%d> (Status: %d).",
                        ChildPId, ChildStatus );
            }

            CleanupChild( Server.ChildList, &Server.PipeSets, ChildPId );
            ChildPId = waitpid( -1, &ChildStatus, WNOHANG );

        }
        if( ChildPId == -1 && errno != ECHILD )
            ErrorMsg( E_SYS, WARN, "WARN:  waitpid() returned an Error.", errno );
        LogMsg( LG_SERVER, DEBUG, "DEBUG: Exiting Signal Handler ..." );
        break;
    default:
        ErrorMsg( E_PRIV, ERROR, "ERROR: Signal %d couldn't be handled!", Signal );
        break;
    }
    errno = errno_bak;
    return;
}


/****** signals/InitSigHandler ***********************************************
*
*   NAME
*       InitSigHandler -- Minimum Signal Handler setup.
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

rc_t InitSigHandler( void )
{
    if( Signal( SIGHUP, SigHandler ) == SIG_ERR )
        ErrorMsg( E_SYS, ERROR, "ERROR: Installing Signal Handler (HUP)." );
    if( Signal( SIGINT,  SigHandler ) == SIG_ERR )
        ErrorMsg( E_SYS, ERROR, "ERROR: Installing Signal Handler (INT)." );
    if( Signal( SIGQUIT, SigHandler ) == SIG_ERR )
        ErrorMsg( E_SYS, ERROR, "ERROR: Installing Signal Handler (QUIT)." );
    if( Signal( SIGTERM, SigHandler ) == SIG_ERR )
        ErrorMsg( E_SYS, ERROR, "ERROR: Installing Signal Handler (TERM)." );
    if( Signal( SIGCHLD, SigHandler ) == SIG_ERR )
        ErrorMsg( E_SYS, ERROR, "ERROR: Installing Signal Handler (CHLD)." );
    if( Signal( SIGUSR1, SigHandler ) == SIG_ERR )
        ErrorMsg( E_SYS, ERROR, "ERROR: Installing Signal Handler (USR1)." );
    if( Signal( SIGUSR2, SigHandler ) == SIG_ERR )
        ErrorMsg( E_SYS, ERROR, "ERROR: Installing Signal Handler (USR2)." );

    if( Signal( SIGPIPE, SigHandler ) == SIG_ERR )
        ErrorMsg( E_SYS, ERROR, "ERROR: Installing Signal Handler (PIPE)." );
    return( OK );
}

