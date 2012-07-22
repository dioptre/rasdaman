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
 * SOURCE:   primitive.cc
 *
 * MODULE:   raslib
 * CLASS:    r_Primitive
 *
 * COMMENTS:
 *
*/

#include "mymalloc/mymalloc.h"

#include "raslib/primitive.hh"
#include "raslib/primitivetype.hh"
#include "raslib/rminit.hh"
#include "raslib/error.hh"

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif
#include <string.h>
#include <fstream>
#include <stdlib.h>

r_Primitive::r_Primitive( const char* newBuffer, const r_Primitive_Type* newType )
    : r_Scalar( newType )
{
    if( valueType )
    {
        valueBuffer = (char*)mymalloc( valueType->size() );
        if( newBuffer)
            memcpy( (void*)valueBuffer, (void*)newBuffer, valueType->size() );
        else
            memset( (void*)valueBuffer, 0, valueType->size() );
    }
}

r_Primitive::r_Primitive( const r_Primitive& obj )
    : r_Scalar( obj ),
      valueBuffer(NULL)
{
    valueBuffer = (char*)mymalloc( valueType->size() );
    if( obj.valueBuffer )
        memcpy( (void*)valueBuffer, (void*)obj.valueBuffer, valueType->size() );
    else
        memset( (void*)valueBuffer, 0, valueType->size());
}

r_Primitive::~r_Primitive()
{
    if( valueBuffer ) free( valueBuffer );
}

bool
r_Primitive::isPrimitive() const
{
    return true;
}

r_Scalar*
r_Primitive::clone() const
{
    return new r_Primitive( *this );
}



const r_Primitive&
r_Primitive::operator=( const r_Primitive& obj )
{
    if( this != &obj )
    {
        // assign scalar
        r_Scalar::operator=( obj );

        if( valueBuffer )
        {
            free( valueBuffer );
            valueBuffer = NULL;
        }

        if( valueType )
        {
            valueBuffer = (char*)mymalloc( valueType->size() );
            if( obj.valueBuffer )
                memcpy( (void*)valueBuffer, (void*)obj.valueBuffer, valueType->size() );
            else
                memset((void*)valueBuffer, 0, valueType->size());
        }
    }

    return *this;
}



const char*
r_Primitive::get_buffer() const
{
    return valueBuffer;
}



void
r_Primitive::print_status( std::ostream& s ) const
{
    if( valueType && valueBuffer )
        valueType->print_value( valueBuffer, s );
    else
        s << "<nn>" << std::flush;
}



r_Boolean
r_Primitive::get_boolean() const throw( r_Error )
{
    if( !valueBuffer || !valueType )
    {
        RMInit::logOut << "r_Primitive::get_boolean() buffer null or type null " << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_boolean(valueBuffer);
}



r_Char
r_Primitive::get_char() const throw( r_Error )
{
    if( !valueBuffer || !valueType)
    {
        RMInit::logOut << "r_Primitive::get_char() buffer null or type null" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_char(valueBuffer);
}



r_Octet
r_Primitive::get_octet() const throw( r_Error )
{
    if( !valueBuffer || !valueType )
    {
        RMInit::logOut << "r_Primitive::get_octet() buffer null or type null" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_octet(valueBuffer);
}



r_Short
r_Primitive::get_short() const throw( r_Error )
{
    if( !valueBuffer || !valueType )
    {
        RMInit::logOut << "r_Primitive::get_short() buffer null or type null" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_short(valueBuffer);
}



r_UShort
r_Primitive::get_ushort() const throw( r_Error )
{
    if( !valueBuffer || !valueType )
    {
        RMInit::logOut << "r_Primitive::get_ushort() buffer null or type null" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_ushort(valueBuffer);
}



r_Long
r_Primitive::get_long() const throw( r_Error )
{
    if( !valueBuffer || !valueType )
    {
        RMInit::logOut << "r_Primitive::get_long() buffer null or type null" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_long(valueBuffer);
}



r_ULong
r_Primitive::get_ulong() const throw( r_Error )
{
    if( !valueBuffer || !valueType )
    {
        RMInit::logOut << "r_Primitive::get_ulong() buffer null or type null" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_ulong(valueBuffer);
}



r_Float
r_Primitive::get_float() const throw( r_Error )
{
    if( !valueBuffer || !valueType )
    {
        RMInit::logOut << "r_Primitive::get_float() buffer null or type null" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_float(valueBuffer);
}



r_Double
r_Primitive::get_double() const throw( r_Error )
{
    if( !valueBuffer || !valueType )
    {
        RMInit::logOut << "r_Primitive::get_double() buffer null or type null" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    return ((r_Primitive_Type*)valueType)->get_double(valueBuffer);
}


void
r_Primitive::set_boolean(r_Boolean val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::BOOL )
    {
        RMInit::logOut << "r_Primitive::set_boolean(" << val << ") not a boolean" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());

    memmove(valueBuffer, &val, valueType->size());
}



void
r_Primitive::set_char(r_Char val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::CHAR )
    {
        RMInit::logOut << "r_Primitive::set_char(" << val << ") not a char" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());

    memmove(valueBuffer, &val, valueType->size());
}



void
r_Primitive::set_octet(r_Octet val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::OCTET )
    {
        RMInit::logOut << "r_Primitive::set_octet(" << val << ") not a octet" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());

    memmove(valueBuffer, &val, valueType->size());
}



void
r_Primitive::set_short(r_Short val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::SHORT )
    {
        RMInit::logOut << "r_Primitive::set_short(" << val << ") not a short" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());

    memmove(valueBuffer, &val, valueType->size());
}



void
r_Primitive::set_ushort(r_UShort val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::USHORT )
    {
        RMInit::logOut << "r_Primitive::set_ushort(" << val << ") not a ushort" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());

    memmove(valueBuffer, &val, valueType->size());
}



void
r_Primitive::set_long(r_Long val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::LONG )
    {
        RMInit::logOut << "r_Primitive::set_long(" << val << ") not a long" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());

    memmove(valueBuffer, &val, valueType->size());
}



void
r_Primitive::set_ulong(r_ULong val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::ULONG )
    {
        RMInit::logOut << "r_Primitive::set_ulong(" << val << ") not a ulong" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());

    memmove(valueBuffer, &val, valueType->size());
}



void
r_Primitive::set_float(r_Float val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::FLOAT )
    {
        RMInit::logOut << "r_Primitive::set_float(" << val << ") not a float" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());

    memmove(valueBuffer, &val, valueType->size());
}



void
r_Primitive::set_double(r_Double val) throw( r_Error )
{
    if( !valueType || valueType->type_id() != r_Type::DOUBLE )
    {
        RMInit::logOut << "r_Primitive::set_double(" << val << ") not a double" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw err;
    }

    if( !valueBuffer )
        valueBuffer = (char*)mymalloc( valueType->size());
    memmove(valueBuffer, &val, valueType->size());
}


std::ostream& operator<<( std::ostream& s, const r_Primitive& obj )
{
    obj.print_status( s );
    return s;
}

