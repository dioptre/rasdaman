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
/*  http-support.c - get and set HTTP date strings.                       */
/*                                                                        */
/*------------------------------------------------------------------------*/
/*
 * RCS:
 *   $RCSfile: http-date.c,v $ $Revision: 1.1 $ $State: Exp $
 *   $Locker:  $ 
 */


#include   "defs.h"
#include   "types.h"
#include   "protos.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"


/****** http-date/HTTP_Date **************************************************
*                                                                             
*   NAME                                                                      
*       HTTP_Date -- create a HTTP date string.                               
*                                                                             
*   SYNOPSIS                                                                  
*       rc_t HTTP_Date( char *Buffer, size_t BuffSize );                      
*                                                                             
*   FUNCTION                                                                  
*       Creates a date (and time) string as defined in the HTTP               
*       specifications. It uses the `gmtime()' system function to get the     
*       current time in Greenwich Mean Time and creates a date string with    
*       the help of `strftime()'.                                             
*                                                                             
*   INPUTS                                                                    
*       Buffer - a pointer to a buffer for the new string.                    
*       BuffSize - size of the Buffer for the date string.                    
*                                                                             
*   RESULT                                                                    
*       Returns the "OK" status code if it succesfully created the date       
*       string, "ERROR" otherwise. The only reason for this function should   
*       fail, is when the Buffer is not large enough.                         
*       As a side effect, if the function succeeds, Buffer will contain the   
*       current time and date in a format conforming to the HTTP              
*       specifications.                                                       
*                                                                             
*   BUGS                                                                      
*       Does not check if Buffer is NULL-pointer.                             
*                                                                             
*   SEE ALSO                                                                  
*       RFC 2068: Hypertext Transfer Protocol -- HTTP/1.1                     
*                                                                             
******************************************************************************
*
*/

rc_t HTTP_Date( char *Buffer, size_t BuffSize )
{
  time_t         systime;
  time_t        *time_ptr;
  struct tm     *tm_ptr;
  size_t         strsize;

  time_ptr = &systime;
  time( time_ptr );

  /* HTTP Date:  "wdy, dd mmm yyyy hh:mm:ss GMT"  */

  tm_ptr = gmtime( time_ptr );
  strsize = strftime( Buffer, BuffSize, "%a, %d %b %Y %H:%M:%S GMT", tm_ptr );
  if( strsize == BuffSize )
    return( ERROR );
  else
    return( OK );
}

