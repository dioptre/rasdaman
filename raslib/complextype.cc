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

#include <iomanip>
#include <string>
#include <cstring>

#include "raslib/complextype.hh"
#include "raslib/endian.hh"
#include "raslib/rmdebug.hh"
#include "raslib/error.hh"

r_Complex_Type::r_Complex_Type()
    :   r_Primitive_Type(),
        imOff(0)
{
}

r_Complex_Type::r_Complex_Type(const char* newTypeName, const r_Type::r_Type_Id newTypeId)
    :   r_Primitive_Type(newTypeName, newTypeId)
{
    imOff = 0;
    switch (typeId)
    {
    case COMPLEXTYPE1:
        imOff = sizeof(r_Float);
        break;
    case COMPLEXTYPE2:
        imOff = sizeof(r_Double);
        break;
    default:
        RMDBGONCE(3, RMDebug::module_raslib, "r_Complex_Type", "r_Complex_Type(...) bad typeId " << typeId);
        break;
    }
}

r_Complex_Type::~r_Complex_Type()
{
}

r_Complex_Type::r_Complex_Type(const r_Complex_Type& oldObj)
    :   r_Primitive_Type(oldObj),
        imOff(oldObj.imOff)
{
}

const r_Complex_Type&
r_Complex_Type::operator=(const r_Complex_Type& oldObj)
{
    if (this == &oldObj)
        return *this;

    r_Primitive_Type::operator =(oldObj);
    imOff = oldObj.imOff;
    return *this;
}

r_Type*
r_Complex_Type::clone() const
{
    return new r_Complex_Type(*this);
}

r_Double
r_Complex_Type::get_re(const char* cell) const throw(r_Error)
{
    double res = 0;

    if( (typeId != r_Type::COMPLEXTYPE1) &&
            (typeId != r_Type::COMPLEXTYPE2) )
    {
        RMInit::logOut << "r_Complex_Type::get_re(cell) type not a complex1 or complex2" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw( err );
    }


    switch (typeId)
    {
    case COMPLEXTYPE1:
        res = *(r_Float*)cell;
        break;
    case COMPLEXTYPE2:
        res = *(r_Double *)cell;
        break;
    default:
        RMDBGONCE(3, RMDebug::module_raslib, "r_Complex_Type", "get_re(...) bad typeId " << typeId);
        break;
    }
    return res;
}

r_Double
r_Complex_Type::get_im(const char* cell) const throw(r_Error)
{
    double res = 0;

    if( (typeId != r_Type::COMPLEXTYPE1) &&
            (typeId != r_Type::COMPLEXTYPE2) )
    {
        RMInit::logOut << "r_Complex_Type::get_im(cell) type not a complex1 or complex2" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw( err );
    }

    switch (typeId)
    {
    case COMPLEXTYPE1:
        res = *(r_Float*)(cell + imOff);
        break;
    case COMPLEXTYPE2:
        res = *(r_Double*)(cell + imOff);
        break;
    default:
        RMDBGONCE(3, RMDebug::module_raslib, "r_Complex_Type", "get_im(...) bad typeId " << typeId);
        break;
    }
    return res;
}




void
r_Complex_Type::set_re(char* cell, r_Double re) throw(r_Error)
{
    r_Float ref=0.;
    if( (typeId != r_Type::COMPLEXTYPE1) &&
            (typeId != r_Type::COMPLEXTYPE2) )
    {
        RMInit::logOut << "r_Complex_Type::set_re(cell, re) type not a complex1 or complex2" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw( err );
    }

    switch (typeId)
    {
    case COMPLEXTYPE1:
        ref=re;
        memmove(cell, &ref, imOff);
        break;
    case COMPLEXTYPE2:
        memmove(cell, &re, imOff);
        break;
    default:
        RMDBGONCE(3, RMDebug::module_raslib, "r_Complex_Type", "set_re(...) bad typeId " << typeId);
        break;
    }
}

void
r_Complex_Type::set_im(char* cell, r_Double im) throw(r_Error)
{
    r_Float imf= 0.;

    if( (typeId != r_Type::COMPLEXTYPE1) &&
            (typeId != r_Type::COMPLEXTYPE2) )
    {
        RMInit::logOut << "r_Complex_Type::set_im(cell, im) type not a complex1 or complex2" << endl;
        r_Error err( r_Error::r_Error_TypeInvalid );
        throw( err );
    }

    switch (typeId)
    {
    case COMPLEXTYPE1:
        imf=im;
        memmove((cell + imOff), &imf, imOff);
        break;
    case COMPLEXTYPE2:
        memmove((cell + imOff), &im, imOff);
        break;
    default:
        RMDBGONCE(3, RMDebug::module_raslib, "r_Complex_Type", "set_im(...) bad typeId " << typeId);
        break;
    }
}


void
r_Complex_Type::print_status(std::ostream& s) const
{
    switch (typeId)
    {
    case COMPLEXTYPE1:
        s << "complex(float, float)";
        break;
    case COMPLEXTYPE2:
        s << "complex(double, double)";
        break;
    default:
        RMDBGONCE(3, RMDebug::module_raslib, "r_Complex_Type", "print_status(...) bad typeId " << typeId);
        break;
    }
}

void
r_Complex_Type::print_value(const char* storage, std::ostream& s) const
{
    s << "(" << get_re(storage) << ", " << get_im(storage) << ")";
}

void
r_Complex_Type::convertToLittleEndian(char* cells, r_Area noCells) const
{
    switch (typeId)
    {
    case COMPLEXTYPE1:
        for (r_Area i = 0; i < noCells; ++i)
        {
            *(r_Float*)(cells + i * typeSize) = r_Endian::swap((r_Float)get_re(cells + i * typeSize));
            *(r_Float*)(cells + i * typeSize + imOff) = r_Endian::swap((r_Float)get_im(cells + i * typeSize));
        }
        break;

    case COMPLEXTYPE2:
        for (r_Area i = 0; i < noCells; ++i)
        {
            *(r_Double*)(cells + i * typeSize) = r_Endian::swap(get_re(cells + i * typeSize));
            *(r_Double*)(cells + i * typeSize + imOff) = r_Endian::swap(get_im(cells + i * typeSize));
        }
        break;
    default:
        RMDBGONCE(3, RMDebug::module_raslib, "r_Complex_Type", "convertToLittleEndian(...) bad typeId " << typeId);
        break;
    }
}

void
r_Complex_Type::convertToBigEndian(char* cells, r_Area noCells) const
{
    switch (typeId)
    {
    case COMPLEXTYPE1:
        for (r_Area i = 0; i < noCells; ++i)
        {
            *(r_Float*)(cells + i * typeSize) = r_Endian::swap((r_Float)get_re(cells + i * typeSize));
            *(r_Float*)(cells + i * typeSize + imOff) = r_Endian::swap((r_Float)get_im(cells + i * typeSize));
        }
        break;
    case COMPLEXTYPE2:
        for (r_Area i = 0; i < noCells; ++i)
        {
            *(r_Double*)(cells + i * typeSize) = r_Endian::swap(get_re(cells + i * typeSize));
            *(r_Double*)(cells + i * typeSize + imOff) = r_Endian::swap(get_im(cells + i * typeSize));
        }
        break;
    default:
        RMDBGONCE(3, RMDebug::module_raslib, "r_Complex_Type", "convertToBigEndian(...) bad typeId " << typeId);
        break;
    }
}

bool
r_Complex_Type::isComplexType() const
{
    return true;
}

std::ostream &operator<<( std::ostream &str, const r_Complex_Type &type )
{
    type.print_status(str);
    return str;
}
