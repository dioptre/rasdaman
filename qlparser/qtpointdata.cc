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

static const char rcsid[] = "@(#)qlparser, QtPointData: $Id: qtpointdata.cc,v 1.7 2005/09/03 20:17:55 rasdev Exp $";

using namespace std;

using namespace std;

#include "qlparser/qtpointdata.hh"
#include <cstring>
// deprecated, not any longer available -- PB 2005-jan-14
// #include <iostream>


QtPointData::QtPointData( const r_Point& point )
  : pointData(point), QtData()
{
}


QtPointData::~QtPointData()
{
}


QtDataType
QtPointData::getDataType() const
{
  return QT_POINT;
}


bool
QtPointData::equal( const QtData* obj ) const
{
  int returnValue = false;  // not equal by initialization

  if( obj->getDataType() == QT_POINT )
  {
    QtPointData* pt = (QtPointData*)obj;

    // check domains
    returnValue = (pointData == pt->getPointData());
  }

  return returnValue;
}


std::string
QtPointData::getSpelling() const
{
  std::string result;

  // buffer
  int        bufferLen = pointData.dimension() * 50; // on the save side for one integers per dimension plus colon and brackets
  char*      buffer    = new char[ bufferLen ];
  // replaced deprecated ostrstream -- PB 2005-jan-14
  // ostrstream bufferStream( buffer, bufferLen );
  ostringstream bufferStream( buffer );

  bufferStream << pointData << std::ends; 

  result.append( std::string( buffer ) );

  delete[] buffer;
  buffer=NULL;
  return result;
}


char* QtPointData::getTypeStructure() const
{
  return strdup("point");
}


void
QtPointData::printStatus( std::ostream& stream ) const
{
  stream << "point, value: " << std::flush;
  stream << pointData << std::flush; 
  QtData::printStatus( stream );
}
