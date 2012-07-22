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

static const char rcsid[] = "@(#)qlparser, QtComplexData: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtcomplexdata.cc,v 1.12 2003/12/27 20:51:28 rasdev Exp $";

#include "qlparser/qtcomplexdata.hh"
#include "relcatalogif/structtype.hh"
#include <stdio.h>
#include <cstring>

QtComplexData::QtComplexData()
    : QtScalarData()
{
}



QtComplexData::QtComplexData( QtComplexData::QtScalarDataList* &scalarDataList )
    : QtScalarData()
{
    char                          elementName[256];
    unsigned int                  i=0;
    std::list<QtScalarData*>::iterator iter;

    // Take care of dynamic memory management:
    //
    // Types, which are not in the typeFactory, have to be deleted. This means that, in general,
    // all complex types have to be deleted because they are constructed temporarily.

    // create a new struct type
    StructType* structType = new StructType( "", scalarDataList->size() );

    // add type elements, the first element inserted has no 0, the second no 1, and so on
    for( iter=scalarDataList->begin(), i=0; iter!=scalarDataList->end(); iter++, i++ )
    {
        sprintf( elementName, "%d", i );
        structType->addElement( elementName, (*iter)->getValueType() );
    }

    // add type to typeFactory
    TypeFactory::addTempType( structType );

    valueBuffer = new char[ structType->getSize() ];
    valueType   = structType;

    // copy data
    for( iter=scalarDataList->begin(), i=0; iter!=scalarDataList->end(); iter++, i++ )
    {
        char* destination = ((char*)valueBuffer) + structType->getOffset( i );

        memcpy( (void*)destination, (void*)((*iter)->getValueBuffer()), (*iter)->getValueType()->getSize() );
    }

    // delete the list of type elements
    // release( scalarDataList->begin(), scalarDataList->end() );
    for( iter=scalarDataList->begin(); iter!=scalarDataList->end(); iter++ )
    {
        delete *iter;
        *iter=NULL;
    }
    delete scalarDataList;
    scalarDataList = NULL;
}



QtComplexData::QtComplexData( const QtComplexData& obj )
    : QtScalarData( obj )
{
}



void
QtComplexData::printStatus( ostream& stream ) const
{
    stream << "complex, " << std::flush;
    QtScalarData::printStatus( stream );
    stream << std::endl;
}
