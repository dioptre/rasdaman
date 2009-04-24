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
 * COMMENTS:
 *
 ************************************************************/

static const char rcsid[] = "@(#)qlparser, QtStringData: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtstringdata.cc,v 1.8 2003/12/27 20:51:28 rasdev Exp $";

#include "qlparser/qtstringdata.hh"

#include <iostream>
#include <cstring>



QtStringData::QtStringData( const std::string& str )
  : stringData(str), QtData()
{
}



QtDataType
QtStringData::getDataType() const
{
  return QT_STRING;
}



bool
QtStringData::equal( const QtData* obj ) const
{
  bool returnValue = false;  // not equal by initialization

  if( obj->getDataType() == QT_STRING )
    returnValue = (stringData == ((QtStringData*)obj)->getStringData());

  return returnValue;
}



std::string
QtStringData::getSpelling() const
{
  return stringData;
}



char* QtStringData::getTypeStructure() const
{
  return strdup("string");
}



void
QtStringData::printStatus( std::ostream& stream ) const
{
  stream << "string, value: " << stringData.c_str() << std::flush; 

  QtData::printStatus( stream );
}
