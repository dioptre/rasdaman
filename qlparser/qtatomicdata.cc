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

static const char rcsid[] = "@(#)qlparser, QtAtomicData: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtatomicdata.cc,v 1.18 2003/12/27 20:51:28 rasdev Exp $";

#include "qlparser/qtatomicdata.hh"
#include "relcatalogif/basetype.hh"
#include "relcatalogif/alltypes.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

#include <iostream>

#include "raslib/rmdebug.hh"


QtAtomicData::QtAtomicData()
  : QtScalarData()
{
}



QtAtomicData::QtAtomicData( r_Long value, unsigned short byteLength )
  : QtScalarData()
{
  switch( byteLength )
  {
    case 1: valueType = TypeFactory::mapType("Octet");  break;
    case 2: valueType = TypeFactory::mapType("Short");  break;
    case 4: valueType = TypeFactory::mapType("Long");   break;

    default:
      RMInit::logOut << "Error: QtAtomicData::QtAtomicData() - signed integer value with length "
                     << byteLength << " is not supported." << endl;
  }

  if( valueType )
  {
    r_Long temp = value;
    valueBuffer = new char[ valueType->getSize() ];
    valueType->makeFromCLong( valueBuffer, &temp ); 
  } 
}


QtAtomicData::QtAtomicData( r_ULong value, unsigned short byteLength )
  : QtScalarData()
{
  switch( byteLength )
  {
    case 1: valueType = TypeFactory::mapType("Char");   break;
    case 2: valueType = TypeFactory::mapType("UShort"); break;
    case 4: valueType = TypeFactory::mapType("ULong");  break;

    default:
      RMInit::logOut << "Error: QtAtomicData::QtAtomicData() - unsigned integer value with length "
                     << byteLength << " is not supported." << endl;
  }


  if( valueType )
  {
    r_ULong temp = value;
    valueBuffer = new char[ valueType->getSize() ];
    valueType->makeFromCULong( valueBuffer, &temp ); 
  } 
}



QtAtomicData::QtAtomicData( bool value )
  : QtScalarData()
{
  r_ULong valueULong = (r_ULong)value;

  valueType   = TypeFactory::mapType("Bool");
  valueBuffer = new char[ valueType->getSize() ];
  valueType->makeFromCULong( valueBuffer, &valueULong ); 
}



QtAtomicData::QtAtomicData( double value, unsigned short byteLength )
  : QtScalarData()
{
  switch( byteLength )
  {
    case 4: valueType = TypeFactory::mapType("Float");  break;
    case 8: valueType = TypeFactory::mapType("Double"); break;

    default:
      RMInit::logOut << "Error: QtAtomicData::QtAtomicData() - float value with length "
                     << byteLength << " is not supported." << endl;
  }


  if( valueType )
  {
    valueBuffer = new char[ valueType->getSize() ];
    valueType->makeFromCDouble( valueBuffer, &value ); 
  } 
}




QtAtomicData::QtAtomicData( const QtAtomicData& obj )
  : QtScalarData( obj )
{
}

QtAtomicData::~QtAtomicData()
{
}

r_ULong
QtAtomicData::getUnsignedValue() const
{
  r_ULong value=0;

  if( valueType )
    valueType->convertToCULong( valueBuffer, &value );

  return value;  
}



r_Long
QtAtomicData::getSignedValue() const
{
  r_Long value=0;

  if( valueType )
    valueType->convertToCLong( valueBuffer, &value );

  return value;  
}



double
QtAtomicData::getDoubleValue() const
{
  double value=0;

  if( valueType )
    valueType->convertToCDouble( valueBuffer, &value );

  return value;  
}



void
QtAtomicData::printStatus( ostream& stream ) const
{
  stream << "atomic, " << flush;

  QtScalarData::printStatus( stream );
}


// for complex types
QtAtomicData::QtAtomicData(double valRe, double valIm, unsigned short size)
: QtScalarData() {
	double dummyRe = valRe;
	double dummyIm = valIm;

	if(size == 2 * sizeof(float))
		valueType = TypeFactory::mapType("Complex1");
	else
		valueType = TypeFactory::mapType("Complex2");

    valueBuffer = new char[valueType->getSize()];
    valueType->makeFromCDouble(valueBuffer + ((GenericComplexType *)valueType)->getReOffset(), &dummyRe); 
    valueType->makeFromCDouble(valueBuffer + ((GenericComplexType *)valueType)->getImOffset(), &dummyIm); 
}

