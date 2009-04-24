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
/
/**
 * SOURCE: rminit.cc
 *
 * MODULE: raslib
 * CLASS:  RMInit
 *
 * COMMENTS:
 *
*/

static const char rcsid[] = "@(#)raslib, RMInit: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/rminit.cc,v 1.36 2005/09/09 16:14:24 rasdev Exp $";

#include "raslib/rminit.hh"

#include "raslib/error.hh"
#include "raslib/rmdebug.hh"

#include <string.h>
#include <stdlib.h>

using std::cout;
using std::ios;

char*         RMInit::userName = 0;
bool          RMInit::useTileContainer = false;
bool          RMInit::tiling;
unsigned long RMInit::timeOut;
bool          RMInit::noTimeOut = 0;
unsigned int  RMInit::clientcommMaxRetry = 6; // changed from 100, with new wait algorithm in rnprotocol/rnpclientcomm.cc this is ~6sec -- PB 2005-sep-09
unsigned int  RMInit::clientcommSleep = 0;
unsigned int  RMInit::rpcMaxRetry = 5;

r_Bytes RMInit::clientTileSize = 786432;

RMInit::RMInit( char initApplicationType )
  : applicationType( initApplicationType )
{
  char* optString;
   
  if( applicationType == 'C' )
  {
    char* value;
    
    // read environment options
    optString = getenv("RMANCLIENTOPT");
      
    //
    // set log stream
    //
    if( optString && checkOptionString( optString, "-l", value ) )
      if( value )
      {
        logFileOut.open( value );
        logOut.rdbuf(logFileOut.rdbuf());
        delete[] value;
      }
      else {
	logOut.rdbuf(cout.rdbuf());
      }
    else
    {
      // default
      logFileOut.open("/dev/null",ios::app);  //"client.log");
      logOut.rdbuf(logFileOut.rdbuf());
    }

  
  
    //
    // set dbg stream
    //
    if( optString && checkOptionString( optString, "-d", value ) )
      if( value )
      {
        dbgFileOut.open( value );
        dbgOut.rdbuf(dbgFileOut.rdbuf());
        delete[] value;
      }
      else
	dbgOut.rdbuf(cout.rdbuf());
    else
    {
      // default
      dbgFileOut.open("/dev/null",ios::app);  //"client.dbg");
      dbgOut.rdbuf(dbgFileOut.rdbuf());
    }
  
  
    //
    // set debug level
    //
    if( optString && checkOptionString( optString, "-dl", value ) )
      if( value )
      {
        RManDebug = (int)strtoul( value, (char **)NULL, 10 );
// It is not clarrified why the delete statement crashes with VISUALC.
#ifndef __VISUALC__
        delete[] value;
#endif
      }


    //
    // set bm stream
    //
    if( optString && checkOptionString( optString, "-b", value ) )
      if( value )
      {
        bmFileOut.open( value );
        bmOut.rdbuf(bmFileOut.rdbuf());
        delete[] value;
      }
      else
	bmOut.rdbuf(cout.rdbuf());
    else
    {
      // default
      bmFileOut.open("/dev/null",ios::app);  //"client.bm");
      bmOut.rdbuf(bmFileOut.rdbuf());
    }
  
  
    //
    // set benchmark level
    //
    if( optString && checkOptionString( optString, "-bl", value ) )
      if( value )
      {
        RManBenchmark = (int)strtoul( value, (char **)NULL, 10 );
// It is not clarrified why the delete statement crashes with VISUALC.
#ifndef __VISUALC__
        delete[] value;
#endif
      }


    //
    // set tiling parameters
    //
    if(optString)
      tiling    = !checkOptionString( optString, "-notiling", value );
    else
      tiling = 1;

    if( optString && checkOptionString( optString, "-tilesize", value ) )
      if( value )
      {
        RMInit::clientTileSize = strtoul( value, (char **)NULL, 10 );
	delete[] value;
      }
  
    if(optString && checkOptionString( optString, "-useTC", value ))
      useTileContainer = true;
    else
      useTileContainer = false;

    timeOut = 3600;  // default
    if( optString && checkOptionString( optString, "-timeout", value ) )
      if( value )
      {
        timeOut = strtoul( value, (char **)NULL, 10 );
	delete[] value;
      }

    if( optString && checkOptionString( optString, "-notimeout", value ) )
      noTimeOut = 1;
    else
      noTimeOut = 0;
  }
  else
  {
    // default
    logOut.rdbuf(cout.rdbuf());
    dbgOut.rdbuf(cout.rdbuf());
    bmOut.rdbuf(cout.rdbuf());
  }
  
  // initialize error text table
  initTextTable(); 
  
  // initialize user name
  if( userName )
  {
    delete [] userName;
    userName = 0;
  }
  
  optString = getenv("USER");
  if( optString )
  {
    userName = new char[strlen(optString)+1];
    strcpy( userName, optString );
  }
  else
  {
    userName = new char[8];
    strcpy( userName, "unknown" );
  }
      
#ifdef RMANDEBUG
  RMDebug::initRMDebug();
#endif
}



RMInit::~RMInit()
{
  // reset output streams
  logOut.rdbuf(NULL);
  dbgOut.rdbuf(NULL);
  bmOut.rdbuf(NULL);
    
  // free error text table
  freeTextTable(); 
  
  if( userName )
  {
    delete [] userName;
    userName = 0;
  }
}



int
RMInit::checkOptionString( const char* optString, const char* option, char* &value )
{
  char* optPos=0;
  char* optValueStart;
  char* optValueEnd;
  int   valueLength;

  using namespace std;
  
  value = 0;
  
  if( (optString != NULL) && (option != NULL) )
  {
    unsigned short found = 0;

    optPos = (char*)optString;

    do{
      optPos = strstr( optPos, option );   

      if( optPos )
      {
        // Check if character after option is either a space or end of string.
        // If not, continue with search.

        char* continuePos = optPos + strlen(option);

        found = (*continuePos == ' ') || (*continuePos == '\0');

        if( !found ) optPos = continuePos;
      }

    }while( !found && optPos != 0 );

  }

  if( optPos )
  {

    optValueStart = optPos + strlen(option);                // move over option
    while( *optValueStart == ' ' ) optValueStart++;         // move over spaces

    if( *optValueStart != '-' && *optValueStart != '\0' )
    {
      optValueEnd = optValueStart;
      while( *optValueEnd != ' ' && *optValueEnd != '\0' ) optValueEnd++; // move over option value
      
      valueLength = optValueEnd - optValueStart;
      if( valueLength )
      {
        value = new char[valueLength+1];
	strncpy( value, optValueStart, valueLength );
	value[valueLength] = '\0';
      }
    }
  }

  /*
  if( optPos )  
  {
    cout << "Option " << option << " specified;" << flush;

    if( value )
      cout << " value: " << value << " length: " << strlen(value) << endl;
    else
      cout << endl;
  }  
  else
    cout << "Option " << option << " not specified." << endl;
  */

  return optPos != 0;
}
