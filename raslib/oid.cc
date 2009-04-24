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
 * SOURCE:   oid.cc
 *
 * MODULE:   raslib
 * CLASS:    r_OId
 *
 * COMMENTS:
 *
*/

#include "raslib/oid.hh"

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>


r_OId::r_OId()
  : oidString(NULL),
    systemName(NULL),
    baseName(NULL),
    localOId(0)
{
}

r_OId::r_OId( const char* initOIdString )
  : oidString(NULL),
    systemName(NULL),
    baseName(NULL),
    localOId(0)
{
  // set oidString
  if( initOIdString )
  {
    oidString = new char[ strlen(initOIdString)+1 ];
    strcpy( oidString, initOIdString );

    //
    // extract oid parts
    //
    char* startPtr = NULL;
    char* endPtr = NULL;

    // system name 
    startPtr = endPtr = oidString;
    while( *endPtr != '|' && *endPtr != '\0' ) endPtr++;
    if( endPtr - startPtr >= 1 )
    {
      systemName = new char[ endPtr - startPtr + 1 ];
      strncpy( systemName, startPtr, endPtr - startPtr );
      systemName[endPtr - startPtr] = '\0';
    }

    if( *endPtr != '\0' )
    {
      // base name
      endPtr++;
      startPtr = endPtr;
      while( *endPtr != '|' && *endPtr != '\0' ) endPtr++;
      if( endPtr - startPtr >= 1 )
      {
        baseName = new char[ endPtr - startPtr + 1 ];
        strncpy( baseName, startPtr, endPtr - startPtr );
        baseName[endPtr - startPtr] = '\0';
      }

      if( *endPtr != '\0' )
      {
        // local oid
        endPtr++;
        startPtr = endPtr;

        localOId = atof( startPtr );
      }

    }

  }
}

r_OId::r_OId( const char* initSystemName, const char* initBaseName, double initLocalOId )
  : oidString(NULL),
    systemName(NULL),
    baseName(NULL),
    localOId( initLocalOId )
{
  // set members
  if( initSystemName )
  {
    systemName= new char[ strlen(initSystemName)+1 ];
    strcpy( systemName, initSystemName );
  }

  if( initBaseName )
  {
    baseName= new char[ strlen(initBaseName)+1 ];
    strcpy( baseName, initBaseName );
  }

  // allocate buffer which is big enough
  int bufferSize = ( systemName ? strlen( systemName ) : 0 ) +
                   ( baseName   ? strlen( baseName   ) : 0 ) +
                   1024;
  char* buffer = new char[bufferSize];
  std::ostrstream oidStream( buffer, bufferSize );

  // write into the string stream
  if( systemName )
    oidStream << systemName << "|";
  else
    oidStream << "|";

  if( baseName )
    oidStream << baseName << "|";
  else
    oidStream << "|";

  oidStream << std::setprecision(50) << localOId << std::ends;

  // allocate memory taking the final string
  oidString = new char[strlen(buffer)+1];
  
  // copy string
  strcpy( oidString, buffer );

  // delete buffer
  delete[] buffer;
}

r_OId::r_OId( const r_OId& obj )
  : oidString(NULL),
    systemName(NULL),
    baseName(NULL),
    localOId(0)
{
  if( obj.oidString )
  {
    oidString= new char[ strlen(obj.oidString)+1 ];
    strcpy( oidString, obj.oidString );
  }

  if( obj.systemName )
  {
    systemName= new char[ strlen(obj.systemName)+1 ];
    strcpy( systemName, obj.systemName );
  }

  if( obj.baseName )
  {
    baseName= new char[ strlen(obj.baseName)+1 ];
    strcpy( baseName, obj.baseName );
  }

  localOId = obj.localOId;
}

r_OId::~r_OId() 
{
  r_deactivate();
}

void
r_OId::r_deactivate()
{
  if( oidString )
  {
    delete[] oidString;
    oidString = NULL;
  }

  if( systemName )
  {
    delete[] systemName;
    systemName = NULL;
  }

  if( baseName )
  {
    delete[] baseName;
    baseName = NULL;
  }
}
 
void
r_OId::print_status( std::ostream& s ) const
{
  if( oidString )
    s << oidString;
}

const r_OId&
r_OId::operator=( const r_OId& obj )
{
  if( this != &obj )
  {
    if( oidString )
    {
      delete[] oidString;
      oidString = NULL;
    }

    if( obj.oidString )
    {
      oidString = new char[ strlen(obj.oidString)+1 ];
      strcpy( oidString, obj.oidString );
    }    

    if( systemName )
    {
      delete[] systemName;
      systemName = NULL;
    }

    if( obj.systemName )
    {
      systemName = new char[ strlen(obj.systemName)+1 ];
      strcpy( systemName, obj.systemName );
    }    

    if( baseName )
    {
      delete[] baseName;
      baseName = NULL;
    }

    if( obj.baseName )
    {
      baseName = new char[ strlen(obj.baseName)+1 ];
      strcpy( baseName, obj.baseName );
    }    

    localOId = obj.localOId;
  }

  return *this;
}

bool
r_OId::operator==( const r_OId& oid ) const
{
  bool equal = false;
  
  if( oidString && oid.oidString )
    equal = !strcmp( oidString, oid.oidString );
  
  return equal;
}

bool
r_OId::operator!=( const r_OId& oid ) const
{
  return !operator==( oid );
}

bool
r_OId::operator> ( const r_OId& oid ) const
{
  int comparison;

  if( systemName && oid.systemName )
  {
    comparison = strcmp( systemName, oid.systemName );

    if( !comparison && baseName && oid.baseName )
    {
      comparison = strcmp( baseName, oid.baseName );

      if( !comparison )
        if( localOId < oid.localOId )
          comparison = -1;
        else if( localOId == oid.localOId )
          comparison = 0;
        else
          comparison = 1;          
    }
  }

  return comparison > 0;
}

bool
r_OId::operator< ( const r_OId& oid ) const
{
  int comparison;

  if( systemName && oid.systemName )
  {
    comparison = strcmp( systemName, oid.systemName );

    if( !comparison && baseName && oid.baseName )
    {
      comparison = strcmp( baseName, oid.baseName );

      if( !comparison )
        if( localOId < oid.localOId )
          comparison = -1;
        else if( localOId == oid.localOId )
          comparison = 0;
        else
          comparison = 1;          
    }
  }

  return comparison < 0;
}

bool
r_OId::operator>=( const r_OId& oid ) const
{
  return !operator< ( oid );
}

bool
r_OId::operator<=( const r_OId& oid ) const
{
  return !operator> ( oid );
}

std::ostream& operator<<( std::ostream& s, const r_OId& oid )
{
  oid.print_status( s );
  return s;
}

