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
/*  logging.c - logging and error message handling.                       */
/*------------------------------------------------------------------------*/

#include   "defs.h"
#include   "protos.h"
#include   "server.h"


extern struct Logging *LogBase;


rc_t OpenLog( struct Logging *Log, char *AccessLog, char *ServerLog, char *CommLog )
{
    /* removed this, we don't need this logs
    if( (Log->Mode & LF_STDERR) == LF_STDERR )  // Log to STDOUT/STDERR ?
      {
        Log->Access.Filename =  NULL;
        Log->Access.FD =        STDOUT_FILENO;
        Log->Access.State =     FILE_OPEN;
        Log->Server.Filename =  NULL;
        Log->Server.FD =        STDERR_FILENO;
        Log->Server.State =     FILE_OPEN;
        Log->Comm.Filename =    NULL;
        Log->Comm.FD =          STDOUT_FILENO;
        Log->Comm.State =       FILE_OPEN;
      }
    else
      {
        Log->Access.Filename = AccessLog;
        if( AccessLog != NULL )
    {
      Log->Access.FD = open( Log->Access.Filename,
               O_WRONLY | O_CREAT | O_APPEND, FILE_MODE );
      if( Log->Server.FD < 0 )
        ErrorMsg( E_SYS, FAIL, "FAIL:  Open AccessLog failed." );
      Log->Access.State = FILE_OPEN;
    }
        else
    {
      Log->Access.State = FILE_CLOSED;
    }

        Log->Server.Filename = ServerLog;
        if( ServerLog != NULL )
    {
      Log->Server.FD = open( Log->Server.Filename,
               O_WRONLY | O_CREAT | O_APPEND, FILE_MODE );
      if( Log->Server.FD < 0 )
        ErrorMsg( E_SYS, FAIL, "FAIL:  Open ServerLog failed." );
      Log->Server.State = FILE_OPEN;
    }
        else
    {
      Log->Server.State = FILE_CLOSED;
    }
      }
      */

    Log->Server.State = FILE_CLOSED;
    Log->Access.State = FILE_CLOSED;
    Log->Comm.State   = FILE_CLOSED;
    return( OK );
}


void CloseLog( struct Logging *Log )
{
    //close( Log->Server.FD );
    Log->Server.State = FILE_CLOSED;
    //close( Log->Access.FD );
    Log->Access.State = FILE_CLOSED;
}



void ErrorMsg( int errnoFlag, int ErrLevel, const char *ErrMsg, ... )
{
    int      errno_save;
    char     ErrMsgBuffer[ MAXLINELEN ];
    size_t   StringSize, BuffSize;
    va_list  ArgPtr;

    errno_save = errno;

    va_start( ArgPtr, ErrMsg );
    if( VSNPrintf( ErrMsgBuffer, MAXLINELEN, ErrMsg, ArgPtr ) < 0)
        ErrorMsg( E_PRIV, WARN, "WARN:  Log message exceeded buffer size!" );
    va_end( ArgPtr );

    StringSize = strlen( ErrMsgBuffer );
    BuffSize   = MAXLINELEN - StringSize;

    if( errnoFlag )
    {
        if( SNPrintf( ErrMsgBuffer + StringSize, &BuffSize,
                      " Reason: %s", StrError( errno_save ) ) < 0)
        {
            ErrorMsg( E_PRIV, WARN, "WARN:  Log message exceeded buffer size!" );
        }
    }
    LogMsg( LG_SERVER, ErrLevel, ErrMsgBuffer );

    switch( ErrLevel )
    {
    case DUMP:
    {
        abort();              /* core dump erzeugen und beenden  */
        exit( DUMP );         /* sollte nicht soweit kommen...   */
    }
    break;
    case FAIL:
    {
        exit( FAIL );         /* Ausstieg mit Status "FAIL".  */
    }
    break;
    default:
        return;
    }
}


void LogMsg( int SubSys, int Level, const char *Msg, ... )
{
    return;
    /*
    char     MsgBuffer[ MAXLINELEN ];
    char    *BufferPtr;
    size_t   BuffSize = MAXLINELEN;
    size_t   TimeBuffSize = DATE_BUFFSIZE;
    va_list  ArgPtr;

    if( (Level == DEBUG) && ((LogBase->Mode & LF_VERB) == 0) ) // DEBUG Msgs wanted?
      return;

    bzero( MsgBuffer, MAXLINELEN );
    BufferPtr = &MsgBuffer[0];

    // ### HACK! access.log datestamp is already in msg string!
    if( SubSys != LG_ACCESS )
      {
        LogDate( BufferPtr, TimeBuffSize );
        strcat( BufferPtr, " " );
      }
    if( SubSys == LG_SERVER )
      {
        BufferPtr = &MsgBuffer[0] + strlen( MsgBuffer );
        sprintf( BufferPtr, "<%d> ", getpid() );
      }

    BufferPtr = &MsgBuffer[0] + strlen( MsgBuffer );
    BuffSize  = MAXLINELEN - strlen( MsgBuffer ) - 2;

    va_start( ArgPtr, Msg );
    if( VSNPrintf( BufferPtr, BuffSize , Msg, ArgPtr ) < 0)
      ErrorMsg( E_PRIV, WARN, "WARN:  Log message exceeded buffer size!" );
    va_end( ArgPtr );

    // Log-Meldungen sollten immer ein <EOL> am Ende haben.
    // Also kontrollieren und nötigenfalls ergänzen.
    // Da MsgBuffer mit maximal MAXLINELEN - 2 Zeichen gefüllt sein
    // sollte, kann gefahrlos <EOL> angehängt werden.

    if( MsgBuffer[ strlen( MsgBuffer ) - 1 ] != '\n' )
      strcat( MsgBuffer, "\n" );

    // Moeglichkeiten:
    //     stdio offen,       Server.Log nicht initialisiert
    //     stdio offen,       Server.Log initialisiert (Log auf stdio)
    //     stdio geschlossen, Server.Log initialisiert (Log in File)

    switch( SubSys )
      {
        case LG_SERVER:
    if( LogBase->Server.State == FILE_OPEN )
      write( LogBase->Server.FD, MsgBuffer, strlen( MsgBuffer ) );
    else
      write( STDERR_FILENO, MsgBuffer, strlen( MsgBuffer ) );
    break;
        case LG_ACCESS:
    if( LogBase->Access.State == FILE_OPEN )
      write( LogBase->Access.FD, MsgBuffer, strlen( MsgBuffer ) );
    else
      write( STDOUT_FILENO, Msg, strlen( MsgBuffer ) );
    break;
        case LG_COMM:
    if( LogBase->Comm.State == FILE_OPEN )
      write( LogBase->Comm.FD, MsgBuffer, strlen( MsgBuffer ) );
    else
      write( STDOUT_FILENO, Msg, strlen( MsgBuffer ) );
    break;
        default:
    ErrorMsg( E_PRIV, WARN, "WARN:  Unknown Logging sub-system!" );
      }
    */
    return;
}

rc_t LogDate( char *Buffer, int BuffSize )
{
    struct tm *ltime;
    int        TZoffset;
    char       sign;

    size_t     strsize;
    size_t     RestSize;

    ltime = Get_GMToffset( &TZoffset );

    /* LOG Date Format:  "[dd/mmm/yyyy:HH:MM:SS {-|+}XXXX]"  */
    /*      XXXX = Timezone offset format: HHMM              */
    sign = ( TZoffset < 0 ? '-' : '+' );
    if( TZoffset < 0 )
        TZoffset = -TZoffset;

    strsize = strftime( Buffer, BuffSize, "[%d/%b/%Y:%H:%M:%S ", ltime );
    if( strsize >= BuffSize-6 || strsize == 0 )
        return( ERROR );
    else
    {
        RestSize = BuffSize-strsize;
        SNPrintf( Buffer+strsize, &RestSize, "%c%.2d%.2d]",
                  sign, TZoffset/60, TZoffset%60 );
        return( OK );
    }
}


/****** logging/Get_GMToffset ************************************************
*
*   NAME
*       Get_GMToffset -- Get numerical offset of localtime to gmtime.
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
*       Source code taken from Apache 1.3.1 distribution.
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

struct tm *Get_GMToffset( int *tz )
{
    time_t tt = time(NULL);
    struct tm gmt;
    struct tm *t;
    int days, hours, minutes;

    /* Assume we are never more than 24 hours away. */
    gmt = *gmtime(&tt);         /* remember gmtime/localtime return ptr to static */
    t = localtime(&tt);         /* buffer... so be careful */
    days = t->tm_yday - gmt.tm_yday;
    hours = ((days < -1 ? 24 : 1 < days ? -24 : days * 24)
             + t->tm_hour - gmt.tm_hour);
    minutes = hours * 60 + t->tm_min - gmt.tm_min;
    *tz = minutes;
    return( t );
}
