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
/*  config.c - configuration and setup stuff.                             */
/*------------------------------------------------------------------------*/
/*  Comments:                                                             */
/*      - intended purpose:                                               */
/*          Contains all code related to setup the configuration.         */
/*      - Status:                                                         */
/*          - There were more features planned than are now available.    */
/*          - Especially some arguments are disabled or non-functional,   */
/*            so: onle use "-c <config-filepath>", and don't care about   */
/*            the rest!                                                   */
/*          - The Flag Handling of Verbose/Debug-Mode is somewhat broken  */
/*          - ReadConfig() uses static buffers. But they are protected    */
/*            against overwriting.                                        */
/*          - ReadConfig() contains some printf()'s.                      */
/*          - Handling of Sub-Server configuration still disabled.        */
/*          - The rest should be okay.                                    */
/*------------------------------------------------------------------------*/

#include <iostream>
using namespace std;

#include   "defs.h"
#include   "protos.h"
#include   "server.h"
#include   <unistd.h>    // for gethostname()

#ifdef SOLARIS
// function prototype with C linkage
extern "C" int gethostname(char *name, int namelen);
#endif

extern int globalHTTPPort;

/****** config/ReadArgs ******************************************************
*
*   NAME
*       ReadArgs --
*
*   SYNOPSIS
*       int ReadArgs( int argc, char *argv[] );
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

rc_t ReadArgs( struct ServerBase *Server, int argc, char *argv[] )
{
    int   i;
    int   LogMode   = LM_VERBOSE;
    char *Filename  = NULL;

    for( i = 1; i < argc; ++i)
    {
        if( argv[i][0] != '-' )
        {
            break;
        }
        switch( argv[i][1] )
        {
        case 'D':             /* Debug-Flag setzen  */
            LogMode |= LF_STDERR;
            break;
        case 'V':             /* Verbose-Flag setzen  */
            LogMode |= LF_VERB;
            break;
        case 'c':             /* Config-File im naechsten Argument */
            if( ++i >= argc )   /* Keine weiteren Argumente? */
            {
                printf( "Oops: Missing config filename after '-c' switch.\n" );
                printf( "Please try again.\n" );
                Exit( FAIL );
            }
            else
            {
                Filename = argv[i];
            }
            break;
        case 'h':
            printf( "Usage: %s [-c <ConfigFile>] [-V] [-D]\n", argv[0] );
            Exit( WARN );
            break;
        case '?':
            printf( "Usage: %s [-c <ConfigFile>] [-V] [-D]\n", argv[0] );
            Exit( WARN );
            break;
        default:              /* Kein bekanntes Argument */
            printf( "Unknown option '%s'.\n", argv[i] );
            Exit( FAIL );
            break;
        }
    }

    return( CheckAndSet( Server, Filename, LogMode ) );
}


/****** config/CheckAndSet ***************************************************
*
*   NAME
*       CheckAndSet --
*
*   SYNOPSIS
*       int CheckAndSet( char *Directory, char *Filename, int LogMode );
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

rc_t CheckAndSet( struct ServerBase *Server, char *Filename, int LogMode )
{
    rc_t  RC = 0;

    /* we skipped the config file
    char *Def_ConfFile = "httpserver.conf";

    if( Filename == NULL )
      {
        LogMsg( LG_SERVER, WARN, "WARN:  No config file given - using default %s.",
               Def_ConfFile );
        Filename = Def_ConfFile;
      }

    SetFilename( Server, FT_CONFFILE, Filename );
    */
    Server->Log.Mode = LogMode;

    return( RC );
}


/****** config/SetServDir ****************************************************
*
*   NAME
*       SetServDir --
*
*   SYNOPSIS
*       int SetServDir( char *Dirname );
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

rc_t SetServDir( struct ServerBase *Server, char *Dirname )
{
    char   *Buffer;
    size_t  BuffSize;
    size_t  MaxPath;

    if ( ( Buffer = PathAlloc( &BuffSize ) ) == NULL )
        ErrorMsg( E_SYS, FAIL, "FAIL:  Buffer allocation for pathname of server directory." );
    Buffer[0] = '\0';

    /* Platz lassen, um auch noch Filenamen + '\0' im Buffer  */
    /* unterbringen zu koennen.  */

    MaxPath = BuffSize - strlen( "httpserver.conf" );
    if( strlen( Dirname ) >= MaxPath )
        ErrorMsg( E_SYS, FAIL, "FAIL:  Pathname of server directory too long." );
    Buffer = strncpy( Buffer, Dirname, MaxPath );

    if( Buffer[ strlen( Buffer ) - 1 ] != '/' )
        Buffer = strcat( Buffer, "/" );

    if(Server->Directory) free(Server->Directory);
    Server->Directory = Buffer;
    return( OK );
}


/****** config/SetFilename ***************************************************
*
*   NAME
*       SetConfFile --
*
*   SYNOPSIS
*       int SetConfFile( char *Filename );
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


rc_t SetFilename( struct ServerBase *Server, int Type, char *Filename )
{
    char   *Buffer = NULL;
    size_t  BuffSize = 0;
    size_t  PathLength, FileLength;

    if( Filename != NULL )
    {
        if ( ( Buffer = PathAlloc( &BuffSize ) ) == NULL )
            ErrorMsg( E_SYS, FAIL, "FAIL:  Buffer allocation for filename \"%s\".", Filename );
        Buffer[0] = '\0';

        if( Server->Directory != NULL )
            PathLength = strlen( Server->Directory );
        FileLength = strlen( Filename );

        if( FileLength >= BuffSize )
            ErrorMsg( E_SYS, FAIL, "FAIL:  Pathname of file \"%s\" too long.", Filename );

        if( Filename[0] != '/' )
        {
            if( ( PathLength + FileLength ) >= BuffSize )
                ErrorMsg( E_SYS, FAIL, "FAIL:  Path/filename of file \"%s\" too long.", Filename );

            Buffer = strncpy( Buffer, Server->Directory, BuffSize );
        }
        Buffer = strncat( Buffer, Filename, FileLength );
    }

    switch( Type )
    {
    case FT_CONFFILE:
        Server->ConfigFile = Buffer;
        break;
    case FT_ACCESSLOG:
        Server->Log.Access.Filename = Buffer;
        break;
    case FT_SERVERLOG:
        Server->Log.Server.Filename = Buffer;
        break;
    case FT_COMMLOG:
        Server->Log.Comm.Filename = Buffer;
        break;
    case FT_PIDFILE:
        Server->PidFile = Buffer;
        break;
    default:
        ErrorMsg( E_SYS, ERROR, "ERROR: Unknown filetype: %d.", Type );
        return( ERROR );
        break;
    }
    return( OK );
}


rc_t SetString( struct ServerBase *Server, int Type, char *String )
{
    char   *Buffer = NULL;
    size_t  BuffSize = 0;

    if( String != NULL )
    {
        BuffSize = strlen( String );
        if ( ( Buffer = (char*)mymalloc( BuffSize+1 ) ) == NULL )
            ErrorMsg( E_SYS, FAIL, "FAIL:  Buffer allocation for string \"%s\".", String );
        strcpy( Buffer, String );

        switch( Type )
        {
        case ST_HOSTNAME:
            Server->Host.Name = Buffer;
            break;
        case ST_MAILADDRESS:
            Server->AdminMailAddress = Buffer;
            break;
        default:
            ErrorMsg( E_SYS, ERROR, "ERROR: Unknown stringtype: %d.", Type );
            return( ERROR );
            break;
        }
        return( OK );
    }
    else
        return( ERROR );
}


/****** config/ConfigureServer ***********************************************
*
*   NAME
*       ConfigureServer --
*
*   SYNOPSIS
*       int ConfigureServer( char *Keyword, char *Value1, char *Value2,
*                            char *Value3 );
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

rc_t ConfigureServer( struct ServerBase *Server )
{
    char* myName;
    char* rmanhome = (char*)CONFDIR;
    int MaxURLLength = 120;
    int BuffSize;
    char* Buffer;

    BuffSize = strlen( rmanhome );

    if ( ( Buffer = (char*)mymalloc( BuffSize+200 ) ) == NULL )
        ErrorMsg( E_SYS, FAIL, "FAIL:  Buffer allocation for Server Configuration" );
    if( gethostname( Buffer, 200 ) )
        ErrorMsg( E_SYS, FAIL, "FAIL:  Could not determine my Hostname" );

    myName=strdup(Buffer);

    strcpy(Buffer,rmanhome);
    SetFilename( Server, FT_COMMLOG, strcat(Buffer,"/httpcomm.log") );

    Server->Port = globalHTTPPort;

    SetString( Server, ST_MAILADDRESS, (char*)"admin@localhost" );

    SetString( Server, ST_HOSTNAME, myName );

    SetServDir( Server, rmanhome );

    strcpy(Buffer,rmanhome);
    SetFilename( Server, FT_ACCESSLOG, strcat(Buffer,"/httpaccess.log") );

    Server->MaxURLLength = MaxURLLength;

    strcpy(Buffer,rmanhome);
    SetFilename( Server, FT_PIDFILE, strcat(Buffer,"/httpserver.pid") );

    strcpy(Buffer,rmanhome);
    SetFilename( Server, FT_SERVERLOG, strcat(Buffer,"/httpserver.log") );

    free(Buffer);
    free(myName);

    return( OK );
}


/****** config/GetConfigKey **************************************************
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
*        With Help from K&R, Chapter 6, binsearch().
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

int GetConfigKey( char *Keyword )
{
    struct KeywordKey KeyTable[] =
    {
        { (char*)"AccessLog",    KEY_ACCESSLOG },
        { (char*)"CommLog",      KEY_COMMLOG },
        { (char*)"IndexFile",    KEY_INDEXFILE },
        { (char*)"MaxURLLength", KEY_MAXURLLENGTH },
        { (char*)"PidFile",      KEY_PIDFILE },
        { (char*)"Port",         KEY_PORT },
        { (char*)"ServerAdmin",  KEY_SERVERADMIN },
        { (char*)"ServerLog",    KEY_SERVERLOG },
        { (char*)"ServerName",   KEY_SERVERNAME },
        { (char*)"ServerRoot",   KEY_SERVERROOT },
    };

    int cond;
    int low;
    int high;

    low  = 0;
    high = NUM_KEYS - 1;

    while( low < high )
    {
        if( ( cond = strcmp( Keyword, KeyTable[ low ].Keyword ) ) == 0 )
            return( KeyTable[ low ].Key );
        low++;
    }

    return -1;
}

