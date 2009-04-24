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
/*************************************************************
 *
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

static const char rcsid[] = "@(#)qlparser, QtMintervalData: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtmintervaldata.cc,v 1.10 2005/09/03 20:17:55 rasdev Exp $";

using namespace std;

using namespace std;

#include "qlparser/qtmintervaldata.hh"

#include <iostream>
#include <cstring>

QtMintervalData::QtMintervalData( const r_Minterval& minterval, vector<bool>* initTrimFlags )
  : mintervalData(minterval), QtData(), trimFlags( initTrimFlags )
{
  if( !trimFlags )
  {
    trimFlags = new vector<bool>( minterval.dimension() );

    for( int i=0; i<trimFlags->size(); i++ )
      (*trimFlags)[i] = true;
  }
}



QtMintervalData::~QtMintervalData()
{
  if( trimFlags ) {delete trimFlags; trimFlags=NULL; }
}



QtDataType
QtMintervalData::getDataType() const
{
  return QT_MINTERVAL;
}



bool
QtMintervalData::equal( const QtData* obj ) const
{
  bool returnValue = false;  // not equal by initialization

  if( obj->getDataType() == QT_MINTERVAL )
  {
    QtMintervalData* mint = (QtMintervalData*)obj;

    // 1. check domains
    returnValue = (mintervalData == mint->getMintervalData());

    // 2. check projection flags !!!
    if( returnValue && trimFlags && mint->getTrimFlags() )
      for( int i=0; i<mintervalData.dimension(); i++ )
        if (!((*trimFlags)[i] == (*(mint->getTrimFlags()))[i]))
        {
          returnValue = false; 
          break;
        }
  }

  return returnValue;
}



std::string
QtMintervalData::getSpelling() const
{
  std::string result;

  // buffer
  int        bufferLen = mintervalData.dimension() * 50; // on the save side for two integers per dimension plus colon and brackets
  char*      buffer    = new char[ bufferLen ];
  // replaced deprecated ostrstream -- PB 2005-jan-14
  // ostrstream bufferStream( buffer, bufferLen );
  ostringstream bufferStream( buffer );

  if( trimFlags )
  {
    bufferStream << "[" << std::flush;
    for( int i=0; i<mintervalData.dimension(); i++ )
    {
      if( i > 0 ) bufferStream << "'" << std::flush;

      if( (*trimFlags)[i] )
        bufferStream << mintervalData[i] << std::flush;
      else
        bufferStream << mintervalData[i].low() << std::flush;
    }     
    bufferStream << "]" << std::ends;
  }
  else
    bufferStream << mintervalData << std::ends; 

  result.append( std::string( buffer ) );

  delete[] buffer;
  buffer = NULL;

  return result;
}



char* QtMintervalData::getTypeStructure() const
{
  return strdup("minterval");
}



void
QtMintervalData::printStatus( std::ostream& stream ) const
{
  stream << "minterval, value: " << std::flush;

  if( trimFlags )
  {
    stream << "[" << std::flush;
    for( int i=0; i<mintervalData.dimension(); i++ )
    {
      if( i > 0 ) stream << "," << std::flush;

      if( (*trimFlags)[i] )
        stream << mintervalData[i] << std::flush;
      else
        stream << mintervalData[i].low() << std::flush;
    }     
    stream << "]" << std::flush;
  }
  else
    stream << mintervalData << std::flush; 

  QtData::printStatus( stream );
}



















