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
/*  support.c - support functions for the gateway httpd.                  */
/*------------------------------------------------------------------------*/
/*
 * RCS:
 *   $RCSfile: support.c,v $ $Revision: 1.6 $ $State: Exp $
 *   $Locker:  $ 
 */

#include <ctype.h>

#include   "defs.h"
#include   "types.h"
#include   "protos.h"
#include   "server.h"


/****** support/Get_OpenMax **************************************************
*                                                                             
*   NAME                                                                      
*       Get_OpenMax -- get the maximum of file descriptors per process.       
*                                                                             
*   SYNOPSIS                                                                  
*       int Get_OpenMax( void );                                              
*                                                                             
*   FUNCTION                                                                  
*       Get the systems maximum number of file descriptors per process. Uses  
*       OPEN_MAX -- if defined -- else asks the system per sysconf() call.    
*       If this still doesn't succeed, returns some kind of "reasonable       
*       default".                                                             
*                                                                             
*   INPUTS                                                                    
*       openmax - A global variable that holds the value of OPEN_MAX,         
*                 if defined, else '0'.                                       
*                                                                             
*   RESULT                                                                    
*       Returns the number of file descriptors, may be zero or a positive     
*       int value.                                                            
*       openmax will be set to an appropriate value if it was set to '0'      
*       when Get_OpenMax() was called.                                        
*                                                                             
*   NOTES                                                                     
*       Uses ErrMsg() to report problems with sysconf().  Hmmm...             
*                                                                             
*   BUGS                                                                      
*       Does also return the guessed value if sysconf() reported an error.    
*       This seems to be okay for my needs...                                 
*                                                                             
*   SEE ALSO                                                                  
*       Richard W. Stevens: "Advanced Programming in the UNIX Environment",   
*       1992 Addison-Wesley Publishing Company, Inc.                          
*                                                                             
******************************************************************************
*
*/

#ifdef     OPEN_MAX
static size_t openmax = OPEN_MAX;   /* Wenn vorhanden, dann ist das alles  */
#else                               /* was gebraucht wird.                 */
static size_t openmax = 0;
#endif

#ifdef     NOFILE
#define    OPEN_MAX_GUESS NOFILE
#else
#define    OPEN_MAX_GUESS 256   /* Falls OPEN_MAX undefiniert (dynamisch) */
#endif                          /* ist, ist dies u.U. nicht adaequat!     */

int Get_OpenMax( void )
{
  if( openmax == 0 )
    {
      errno = 0;
      if( ( openmax = sysconf( _SC_OPEN_MAX ) ) < 0 )
	{
	  if( errno == 0 )
	    {
	      openmax = OPEN_MAX_GUESS;
	    }
	  else
	    {
	      /* sysconf() returned an error... strange, but...        */
	      /* Does it justify to break the program?                 */
	      /* Naah, I don't think so...  So we just print a note    */
	      /* and give back our "reasonable default"...             */
	      ErrorMsg( E_SYS, WARN, "WARN:  sysconf error for _SC_OPEN_MAX." );
	      openmax = OPEN_MAX_GUESS;
	    }
	}
    }
    
    return( openmax );
}


/****** support/PathAlloc ****************************************************
*                                                                             
*   NAME                                                                      
*       PathAlloc -- allocate memory as buffer for pathnames.                 
*                                                                             
*   SYNOPSIS                                                                  
*       char *PathAlloc( size_t *size );                                      
*                                                                             
*   FUNCTION                                                                  
*       Get the systems maximum of characters in absolute pathnames. Uses     
*       PATH_MAX -- if defined -- else asks the system per pathconf() call.   
*       If this still doesn't succeed, uses some kind of "reasonable          
*       default".                                                             
*       Then it calls malloc() to allocate the buffer.                        
*                                                                             
*   INPUTS                                                                    
*       size    - A pointer to a variable, which will set to the size of      
*                 the buffer. If it is NULL, you won't get any size           
*                 information.                                                
*       pathmax - A global variable that holds the value of PATH_MAX,         
*                 if defined, else '0'                                        
*                                                                             
*   RESULT                                                                    
*       Returns a "char" pointer to the allocated buffer.                     
*       NULL, if the allocation failed. Then it will also print a log         
*       message.                                                              
*       pathmax will be set to an appropriate value if it was set to '0'      
*       when PathAlloc() was called.                                          
*                                                                             
*   NOTES                                                                     
*       Uses ErrMsg() to report problems with pathconf() and malloc().        
*                                                                             
*   BUGS                                                                      
*       Does also use the guessed value if pathconf() reported an error.      
*       This seems to be okay for my needs...                                 
*                                                                             
*   SEE ALSO                                                                  
*       Richard W. Stevens: "Advanced Programming in the UNIX Environment",   
*       1992 Addison-Wesley Publishing Company, Inc.                          
*                                                                             
******************************************************************************
*
*/

#ifdef     PATH_MAX
static size_t pathmax = PATH_MAX;   /* Wenn vorhanden, dann ist das alles  */
#else                               /* was gebraucht wird.                 */
static size_t pathmax = 0;
#endif

#ifdef     MAXPATHLEN
#define    PATH_MAX_GUESS MAXPATHLEN
#else
#define    PATH_MAX_GUESS 1024  /* Falls PATH_MAX undefiniert (dynamisch) */
#endif                          /* ist, ist dies u.U. nicht adaequat!      */

char *PathAlloc( size_t *size )
{
  char *Ptr;

  if( pathmax == 0 )
    {
      errno = 0;
      if( ( pathmax = pathconf( "/", _PC_PATH_MAX ) ) < 0 )
	{
	  if( errno == 0 )
	    {
	      pathmax = PATH_MAX_GUESS;
	    }
	  else
	    {
	      /* pathconf() returned an error... strange, but...     */
	      /* See also Get_OpenMax().                             */
	      /* We give back our "reasonable default"...            */
	      ErrorMsg( E_SYS, WARN, "WARN:  pathconf error for _PC_PATH_MAX." );
	      pathmax = PATH_MAX_GUESS;
	    }
	}
    }
  else
    {
      // it appears that every call makes the path 1 byte longer. Anyway, the config file
      // is not read too often and I am not willing to debug it now
      pathmax++;        /* Don't forget the "/" character...  */
    }

  if( ( Ptr = (char*)mymalloc( pathmax + 1 ) ) == NULL )
    {
      if( size != NULL )
	*size = 0;
      ErrorMsg( E_SYS, ERROR, "ERROR: malloc error for pathname buffer." );
    }
  else
    {
      if( size != NULL )
	*size = pathmax + 1;
    }
  return( Ptr );
}


/****** support/ReadN ********************************************************
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
/**************************************************************************
 *  Read "n" bytes from a descriptor.
 *  Use in place of read() when fd is a stream socket.
 *  Checks for error 'EAGAIN', in case of nonblocking IO.
 */

int ReadN( register int fd, register char *ptr, register int nbytes )
{
  int nleft;
  int nread;

  nleft = nbytes;
  while( nleft > 0 )
    {
      nread = read( fd, ptr, nleft );
      if( nread < 0 )
	if( errno != EAGAIN )
	  return( nread );              /* error, return < 0 */
	else
	  continue;
      else if( nread == 0 )
	break;                          /* EOF */
      nleft -= nread;
      ptr += nread;
    }
  return( nbytes - nleft );             /* return >= 0 */
}


/****** support/WriteN *******************************************************
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
/**************************************************************************
 *  Write "n" bytes to a descriptor.
 *  Use in place of write() when fd is a stream socket.
 *  Checks for error 'EAGAIN', in case of nonblocking IO.
 */

int WriteN( register int fd, register char *ptr, register int nbytes )
{
  long nleft;
  long nwritten = 0;

  nleft = nbytes;
  while( nleft > 0 )
    {
      nwritten = write( fd, ptr, nleft );
      if( nwritten < 0 )
	{
	  if( errno != EAGAIN )
	    return( nwritten );         /* error */
	  else
	    continue;
	}
      nleft -= nwritten;
      ptr += nwritten;
    }
  return( nbytes - nleft );
}


/****** support/ReadLine *****************************************************
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
/**************************************************************************
 *  Read a line from a descriptor.
 *  Checks for error 'EAGAIN', in case of nonblocking IO.
 */
 
int ReadLine( register int fd, register char *ptr, register int maxlen )
{
  int n;
  int rc;
  char c;

  for( n = 1; n < maxlen; n++ )
    {
      if( ( rc = read( fd, &c, 1 ) ) == 1 )
	{
	  *ptr++ = c;
	  if( c == '\n' )
	    break;
	}
      else if( rc == 0 )
	if( n == 1 )
	  return( 0 );                  /* EOF, no data read */
	else
	  break;                        /* EOF, some data was read */
      else
	if( errno != EAGAIN )
	  return( -1 );                 /* error */
    }
  *ptr = 0;
  return( n );
}


/****** support/ParseString **************************************************
*                                                                             
*   NAME                                                                      
*       ParseString -- Break string into words.                               
*                                                                             
*   SYNOPSIS                                                                  
*       int ParseString( char *String, char *Token, ... );                    
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

rc_t ParseString( char *String, char *Token, ... )
{
  /* ###TODO: Das letzte Argument sollte den kompletten Reststring erhalten */
  /*          Buffer loeschen vor Austritt                                  */
  va_list  ArgPtr;
  char    *Keyword;
  char    *Value;
  char    *NextToken;
  char    *NNextToken;
  char    *Delim = (char*)" \t\n\r";                    /* Begrenzungszeichen          */
  
  if( ( String[0] == '#' ) ||                           /* Kommentar,                  */
      ( String[0] == '\n' ) ||                          /* oder Leerzeile?             */
      ( String[0] == '\r' ) )
    return( ERROR );                                    /*   => Zur�ck.                */

  Keyword = strtok( String, Delim );                    /* 1. Token suchen             */
  if( Keyword != NULL )                                 /*   Etwas gefunden?           */
    {
      if( Keyword[0] == '#' )                           /* Auch Kommentarzeile:        */
	return( ERROR );                                /*   => Zur�ck.                */
      strcpy( Token, Keyword );                         /* Keyword in Buffer kopieren  */

      /* Var.Arg. Liste abarbeiten bis Arg == NULL.                                    */
      /* Letztes Arg. muss NULL sein, da va_arg() nicht selbst in der Lage             */
      /* ist das letzte Argument zu erkennen.                                          */

      va_start( ArgPtr, Token );                        /* VarArg initialisieren       */
      NextToken = va_arg( ArgPtr, char * );             /* 1. zus. Argument holen.     */
      if( NextToken != NULL)
	{
	  NNextToken = va_arg( ArgPtr, char * );        /* 2. zus. Arg. holen          */
	  while( NNextToken != NULL )                   /*   Argument vorhanden?       */
	    {
	      Value = strtok( NULL, Delim );            /* Weiteres Token suchen       */
	      if( Value != NULL )                       /* Wort gefunden?              */
		strcpy( NextToken, Value );             /*   Token in Buffer kopieren  */
	      else                                      /* Ansonsten:                  */
		strcpy( NextToken, "\0" );              /*   Leerstring uebergeben     */
	      NextToken = NNextToken;                   
	      NNextToken = va_arg( ArgPtr, char * );    /* Zeiger auf n�chstes Arg.    */
	    }
	  Value = strtok( NULL, "\n\r" );               /* Zeiger auf Reststring holen */
	  if( Value != NULL )                           /* Wenn vorhanden,             */
	    strcpy( NextToken, Value );                 /*   in letztes Arg. �bergeben */
	  else                                          /* wenn nicht,                 */
	    strcpy( NextToken, "\0" );                  /*   Leerstring �bergeben      */
	}
      va_end( ArgPtr );                                 /* VarArg aufr�umen            */
      return( OK );                                     /* OK: Es wurde etwas gefunden */
    }
  return( ERROR );                                      /* ERROR: String war leer.     */
}


/****** support/SNPrintf *****************************************************
*                                                                             
*   NAME                                                                      
*       SNPrintf -- print into buffer with size-check.                        
*                                                                             
*   SYNOPSIS                                                                  
*       int SNPrintf( char *String, size_t *Size, const char *Format, ... );  
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

int SNPrintf( char *String, size_t *Size, const char *Format, ... )
{
  va_list  ArgPtr;
  int      n = 0;

  if( *Size > 0 )
    {
      va_start( ArgPtr, Format );
      n = VSNPrintf( String, *Size, Format, ArgPtr );
      va_end( ArgPtr );

      if( n >= 0 )
	*Size = *Size - n;
      else
	*Size = 0;
    }
  return( n );
}


/****** support/VSNPrintf ****************************************************
*                                                                             
*   NAME                                                                      
*       VSNPrintf -- print variable argument list into buffer with size-check.
*                                                                             
*   SYNOPSIS                                                                  
*       int VSNPrintf( char *String, size_t Size,                             
*                      const char *Format, va_list ArgPtr );                  
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

int VSNPrintf( char *String, size_t Size, const char *Format, va_list ArgPtr )
{
  int   n = 0;
  char *Buffer = NULL;

#ifdef NO_vsnprintf
  if( Size == 0 )
    n = 0;
  else if( Size > 0 )
    {
      if( ( Buffer = (char*)mymalloc( BUFFSIZE ) ) != NULL )
	{
	  n = vsprintf( Buffer, Format, ArgPtr );
	  if( n < Size )
	    strncpy( String, Buffer, Size );
	  else
	    {
	      strncpy( String, Buffer, Size-1 );
	      *( String + Size ) = '\0';
	      n = -1;
	    }
	  free( Buffer );
	}
      else 
	n = -1;
    }
  else
    n = -1;
#else
  n = vsnprintf( String, Size, Format, ArgPtr );
#endif
  return( n );
}


/****** support/StrError *****************************************************
*                                                                             
*   NAME                                                                      
*       StrError - return error string for given error code.                  
*                                                                             
*   SYNOPSIS                                                                  
*       char *StrError( int ErrNum );                                         
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

char *StrError( int ErrNum )
{
#ifdef NO_strerror
  char Buffer[20];

  bzero( Buffer, 20 );
  sprintf( Buffer, "Error %d", ErrNum );
  return( Buffer );
#else
  return( strerror( ErrNum ) );
#endif
}

/****** support/StrToLower ***************************************************
*                                                                             
*   NAME                                                                      
*       StrToLower - make all characters of a string lower case.              
*                                                                             
*   SYNOPSIS                                                                  
*       char *StrToLower( char *String );                                     
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

char *StrToLower( char *String )
{
  while( *String != '\0' )
    {
      *String = tolower( *String );
      String++;
    }
  return( String );
}
