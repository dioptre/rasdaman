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
/*  defs.h - Global defines and typedefs.                                 */
/*------------------------------------------------------------------------*/


#ifndef _DEFS_H
#define _DEFS_H


/* Name of this daemon
 */

#define    PROGRAM_NAME     "httpserver"
#define    PROGRAM_VERSION  "1.0"
#define    DAEMONNAME       PROGRAM_NAME "/" PROGRAM_VERSION
#define    SERVERFIELD      "Server: " DAEMONNAME "\r\n"


/* These constants *may* be changed
 */

#define     BUFFSIZE          8192
#define     MAXLINELEN        4096
#define     PIPE_BUFFSIZE     4096
#define     DATE_BUFFSIZE     40
#define     IO_BUFFSIZE       65536
#define     DEFAULT_MAXURLLENGTH 1000000
// #define     IO_BUFFSIZE       1024

/* TimeOut values are given in seconds
 */

   /*  Timeout when talking with subservers and client  */
#define     DIALOG_TIMEOUT    30
   /*  Timeout when trying to get an unspecified mesage body, for example  */
   /*  dynamically generated output (typically CGI output).                */
   /*  You may have to *really* wait for it, so don't make it too small!   */
#define     MSGBODY_TIMEOUT   30


/* Misc. defines
 */

#ifndef TRUE
#define     TRUE              1
#endif
#ifndef FALSE
#define     FALSE             0
#endif

#ifdef SOCKLENNOTDEFINED
typedef int socklen_t;
#endif

#define     max(a,b)          (a > b ? a : b)
#define     min(a,b)          (a < b ? a : b)

#define     FOREGROUND        0
#define     BACKGROUND        1

/* Std.-Zugriffsberechtigung für neue Dateien */
#define     FILE_MODE         ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

/* Filetypes:  */
#define     FT_NOFILE         0
#define     FT_CONFFILE       1
#define     FT_ACCESSLOG      2
#define     FT_SERVERLOG      3
#define     FT_COMMLOG        4
#define     FT_PIDFILE        5
#define     FT_CACHEFILE      6

/* Stringtypes:  */
#define     ST_NOSTRING       0
#define     ST_HOSTNAME       1
#define     ST_MAILADDRESS    2


/* Pipe Status:
 */

#define     PIPE_OPEN         1
#define     PIPE_CLOSED       0

/* LogFile Status:
 */

#define     FILE_OPEN         1
#define     FILE_CLOSED       0

/* Logging Modes:
 *    FILE    (0) ServerLog und AccessLog einrichten und oeffnen
 *    STDIO   (1) ServerLog -> stderr; AccessLog -> stdout
 *    DEBUG   (2) Detaillierte Meldungen
 */

#define    LF_VERB            0x0001
#define    LF_STDERR          0x0002

#define    LM_NORMAL          0   /* Not Verbose, write into log files  */
#define    LM_VERBOSE         1   /* Verbose,     write into log files  */
#define    LM_STDERR          2   /* Not Verbose, use STDOUT/STDERR     */
#define    LM_VERBOSE_STDERR  3   /* Verbose,     use STDOUT/STDERR     */


/* LogLevels / ReturnCodes:
 *    DEBUG   (1)   Detaillierte Aktionsmeldungen
 *    INFO    (2)   Statusmeldungen 
 *    NOTE    (3)   Hinweise auf besondere Situationen
 *    WARN    (5)   Behebbare Fehlersituationen
 *    ERROR   (10)  Fehler die zu Aktionsabbruch fuehren
 *    FAIL    (20)  Fehler die zum Programmabbruch fuehren
 */

#define    OK                 0
#define    DEBUG              1
#define    INFO               2
#define    NOTE               3
#define    WARN               5
#define    ERROR              10
#define    FAIL               20
#define    DUMP               50

#define    SYS_ERROR          -1

/* Logging sub-systems:
 *    LG_SERVER     generic server logfile
 *    LG_ACCESS     HTTP request logfile
 */

#define    LG_SERVER          1
#define    LG_ACCESS          2
#define    LG_COMM            3

/* Error-Type flags:
 *    SYS     TRUE  System Error: strerror(errno)
 *    PRIV    FALSE Application Error
 */

#define    E_SYS              TRUE
#define    E_PRIV             FALSE

/* Configuration Keyword Keys:
 */

#define    KEY_ACCESSLOG      1
#define    KEY_COMMLOG        2
#define    KEY_INDEXFILE      3
#define    KEY_MAXURLLENGTH   4
#define    KEY_PIDFILE        5
#define    KEY_PORT           6
#define    KEY_SERVERADMIN    7
#define    KEY_SERVERLOG      8
#define    KEY_SERVERNAME     9 
#define    KEY_SERVERROOT     10
#define    NUM_KEYS           11

/* SubServer Communication Status Codes:
 */

#define    COMM_UNSUPPORTED   -3  /* don't know how to handle comm state           */
#define    COMM_UNEXPECTED    -2  /* unexpected data received                      */
#define    COMM_FAILED        -1  /* trouble while communicating, protocol failure */
#define    COMM_IDLE          0   /* not connected and ready                       */
#define    COMM_CONNECTING    1   /* trying to connect or "Keep-Alive" connection  */
#define    COMM_GET_RESPHEAD  2   /* reading and processing response header        */
#define    COMM_GET_RESPBODY  3   /* reading response body                         */
#define    COMM_MAY_GET_BODY  4   /* have to check for response body               */
#define    COMM_HERE_IS_MORE  5   /* select() indicated that there IS more data    */
#define    COMM_DONE          9   /* communication done, protocol OK               */
/* Currently not in use:  */
#define    COMM_RECONNECTING  10  /* trying to reconnect for authorization         */
#define    COMM_VERIFYING     11  /* analysing response: add. processing required  */
#define    COMM_SENDINGAUTH   12  /* (re-)sending request with authorization       */

/* Connection Mode Codes:
 */

#define    CONN_FAILURE       -3  /* Failure when setting up connection     */
#define    CONN_BROKEN        -2  /* Connection broken while communicating  */
#define    CONN_UNDEFINED     -1  /* Connection not initiated               */
#define    CONN_CLOSE         0   /* Connection will be closed ASAP         */
#define    CONN_OPEN          1   /* Connection is ready for communication  */
#define    CONN_ERROR         2   /* Error condition in communication       */

/* ToDo Action Codes:
 */

#define    DO_NOTHING              0
#define    DO_SINGLE_SERVER        1
#define    DO_REWRITE              2
#define    DO_SEND_RESPONSE        3
#define    DO_SEND_ERROR           4
#define    DO_SHUTDOWN             5

/* Client Types */
#define    BROWSER      1
#define    RASCLIENT    2

/* ToDo Argument Codes:
 */

/* -  DO_REWRITE:  */
#define    MODE_HTTP_1_0        1
#define    MODE_HTTP_1_1        2

/* - INTERPRETE_POST_REQUEST: - */
#define    REQU_OK                 1
#define    REQU_UNKNOWN_PARAMETER  2
#define    REQU_UNKNOWN_CLIENT     3

/* - Result Types: Error, MDD, Skalar - */
#define    RESULT_ERROR        1
#define    RESULT_MDD          2
#define    RESULT_SKALAR       3

/* -  DO_SEND_ERROR:  */
/*        -> use HTTP Status Codes (see http-defs.h)  */

/* -  DO_SHUTDOWN:  */
#define    CLOSE_ALL            1
#define    CLOSE_CLIENT_ONLY    2       /* Currently not used */

/* -  DO_NOTHING:  */
#define    REALLY_NOTHING       0

#endif  /*  _DEFS_H not defined  */
