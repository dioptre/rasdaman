/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/**
 * SOURCE:   structure.cc
 *
 * MODULE:   raslib
 * CLASS:    r_Structure
 *
 * COMMENTS:
 *
*/

#include "raslib/structure.hh"
#include "raslib/primitive.hh"
#include "raslib/structuretype.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"
#include "mymalloc/mymalloc.h"

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif
#include <string.h>
#include <fstream>
#include <stdlib.h>




r_Structure::r_Structure( const char* newBuffer, const r_Structure_Type* newType )
    : r_Scalar( newType ),
      numElements(0),
      elements(NULL),
      valueBuffer(NULL)
{
    if( newType )
    {

        numElements = newType->count_elements();
        elements = new r_Scalar*[numElements];

        valueBuffer = (char*)mymalloc(newType->size());

        if(newBuffer)
            memcpy(valueBuffer, newBuffer, newType->size());
        else
            memset(valueBuffer, 0, newType->size());

        r_Structure_Type::attribute_iterator iter( newType->defines_attribute_begin() );

        for( int i=0; iter != newType->defines_attribute_end(); iter++, i++ )
        {
            if( (*iter).type_of().type_id() == r_Type::STRUCTURETYPE )
                elements[i] = new r_Structure( valueBuffer + (*iter).offset(), (r_Structure_Type*)&((*iter).type_of()) );
            else
                elements[i] = new r_Primitive( valueBuffer + (*iter).offset(), (r_Primitive_Type*)&((*iter).type_of()) );
        }
    }
}



r_Structure::r_Structure( const r_Structure& obj )
    : r_Scalar( obj ),
      numElements(obj.numElements),
      elements(NULL),
      valueBuffer(NULL)
{
    if( numElements )
    {
        elements = new r_Scalar*[numElements];

        for( unsigned int i=0; i < numElements; i++ )
            elements[i] = obj.elements[i]->clone();
    }

    valueBuffer = (char*) mymalloc(valueType->size());

    if(obj.valueBuffer)
        memcpy(valueBuffer, obj.valueBuffer, valueType->size());
    else
        memset(valueBuffer, 0, valueType->size());
}



r_Structure::~r_Structure()
{
    if( numElements )
    {
        for( unsigned int i =0; i < numElements; i++ )
            delete elements[i];

        delete[] elements;
    }

    if(valueBuffer)
        free(valueBuffer);
}



r_Scalar*
r_Structure::clone() const
{
    return new r_Structure( *this );
}



const r_Structure&
r_Structure::operator=( const r_Structure& obj )
{
    if( this != &obj )
    {
        // assign scalar
        r_Scalar::operator=( obj );

        if( numElements )
        {
            for( unsigned int i =0; i < numElements; i++ )
                delete elements[i];

            delete[] elements;
        }

        if( obj.numElements )
        {
            numElements = obj.numElements;
            elements = new r_Scalar*[numElements];

            for( unsigned int i =0; i < numElements; i++ )
                elements[i] = obj.elements[i]->clone();
        }

        if(valueBuffer)
            free(valueBuffer);

        valueBuffer = (char*) mymalloc(valueType->size());

        if(obj.valueBuffer)
            memcpy(valueBuffer, obj.valueBuffer, valueType->size());
        else
            memset(valueBuffer, 0, valueType->size());

    }

    return *this;
}



unsigned int
r_Structure::count_elements() const
{
    return numElements;
}

const char*
r_Structure::get_buffer() const
{
    memset(valueBuffer, 0, valueType->size());

    r_Structure_Type::attribute_iterator iter( ((r_Structure_Type*)valueType)->defines_attribute_begin() );

    for( int i=0; iter != ((r_Structure_Type*)valueType)->defines_attribute_end(); iter++, i++ )
        if( (*iter).type_of().type_id() == r_Type::STRUCTURETYPE )
            memcpy( valueBuffer + (*iter).offset(), ((r_Structure*)elements[i])->get_buffer(), elements[i]->get_type()->size());
        else
            memcpy( valueBuffer + (*iter).offset(), ((r_Primitive*)elements[i])->get_buffer(), elements[i]->get_type()->size());

    return valueBuffer;
}


bool
r_Structure::isStructure() const
{
    return true;
}



const r_Scalar&
r_Structure::operator[]( unsigned int index ) const throw( r_Error )
{
    if( !valueType )
    {
        RMInit::logOut << "r_Structure::operator[](" << index << ") const value type is NULL" << endl;
        throw r_Error( r_Error::r_Error_TypeInvalid );
    }

    if( index > numElements )
    {
        RMInit::logOut << "r_Structure::operator[](" << index << ") const index is out of bounds (" << numElements - 1 << ")" << endl;
        throw r_Eindex_violation( 0, numElements, index );
    }

    return *(elements[index]);
}



const r_Scalar&
r_Structure::operator[]( const char* name ) const throw( r_Error )
{
    if( !valueType )
    {
        RMInit::logOut << "r_Structure::operator[](" << name << ") value type is NULL" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw( err );
    }

    r_Structure_Type* structType = (r_Structure_Type*)valueType;

    r_Structure_Type::attribute_iterator iter( structType->defines_attribute_begin() );

    int index = 0;
    for( ; iter != structType->defines_attribute_end() && strcmp((*iter).name(), name); iter++, index++ );

    if( iter == structType->defines_attribute_end() )
    {
        RMInit::logOut << "r_Structure::operator[](" << name << ") name is not valid" << endl;
        r_Error err( r_Error::r_Error_NameInvalid );
        throw( err );
    }

    return *(elements[index]);
}


void
r_Structure::print_status( std::ostream& s ) const
{
    if( valueType )
    {
        s << "{ " << std::flush;

        for( unsigned int i =0; i < numElements; i++ )
        {
            s << *(elements[i]) << std::flush;

            if( i < numElements-1 )
                s << ", " << std::flush;
        }

        s << " }" << std::flush;
    }
    else
        s << "<nn>" << std::flush;
}



std::ostream& operator<<( std::ostream& s, const r_Structure& obj )
{
    obj.print_status( s );
    return s;
}

