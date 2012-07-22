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

static const char rcsid[] = "@(#)qlparser, QtData: $Id: qtdata.cc,v 1.17 2002/06/05 18:18:17 coman Exp $";

#include "raslib/rmdebug.hh"
#include "qlparser/qtdata.hh"


QtData::QtData()
    :  referenceCounter(1),
       persistent( QT_TRANSIENT ),
       parseInfo(NULL)
{
    RMDBGONCE( 10, RMDebug::module_qlparser, "QtData", "QtData::QtData() Obj: " << this )
}



QtData::QtData( const std::string name )
    : iteratorName( name ),
      referenceCounter(1),
      persistent( QT_TRANSIENT ),
      parseInfo(NULL)
{
    RMDBGONCE( 10, RMDebug::module_qlparser, "QtData", "QtData::QtData( const std::string ) Obj: " << this )
}



QtData::QtData( const QtData &obj )
    : iteratorName( obj.iteratorName ),
      persistent( obj.persistent ),
      referenceCounter(1),
      parseInfo(NULL)
{
    RMDBGONCE( 10, RMDebug::module_qlparser, "QtData", "QtData::QtData( QtData& ) Obj: " << this )

    if( obj.parseInfo )
        parseInfo = new ParseInfo( *(obj.parseInfo) );
}



QtData::~QtData()
{
    RMDBGONCE( 10, RMDebug::module_qlparser, "QtData", "QtData::~QtData() Obj: " << this )

    if( parseInfo )
    {
        delete parseInfo;
        parseInfo=NULL;
    }
}



bool
QtData::isScalarData() const
{
    // default implementation returns false
    return false;
}



const QtData&
QtData::operator=( const QtData& obj )
{
    RMDBGONCE( 10, RMDebug::module_qlparser, "QtData", "QtData::operator=" )

    if( this != &obj )
    {
        iteratorName     = obj.iteratorName;
        persistent       = obj.persistent;
        referenceCounter = 1;

        if( parseInfo )
        {
            delete parseInfo;
            parseInfo = NULL;
        }

        if( obj.parseInfo )
            parseInfo = new ParseInfo( *(obj.parseInfo) );
    }

    return *this;
}



void
QtData::printStatus( std::ostream& stream ) const
{
    if( iteratorName.size() )
        stream << ", iter name: " << iteratorName.c_str() << std::flush;

    stream << ", ref#: " << referenceCounter
           << ( persistent == QT_TRANSIENT ? " trans " : " pers " )  << std::flush;
}
