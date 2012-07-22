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

#include "conversion/tor.hh"
#include "raslib/endian.hh"
#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "raslib/primitivetype.hh"

void r_Conv_TOR::initTOR()
{
    if(params ==NULL)
    {
        params = new r_Parse_Params(3);
    }
}

r_Conv_TOR::r_Conv_TOR(const char* source, const r_Minterval& lengthordomain, const r_Type* tp) throw(r_Error)
    :   r_Convertor(source, lengthordomain, tp, true)
{
    initTOR();
}

r_Conv_TOR::r_Conv_TOR(const char* source, const r_Minterval& lengthordomain, int tp) throw(r_Error)
    :   r_Convertor(source, lengthordomain, tp)
{
    initTOR();
}

r_convDesc&
r_Conv_TOR::convertFrom(const char* options) throw (r_Error)
{
    int swap = 0;
    int rescale = 0;
    char* domain = NULL;
    params->add("swapendianness", &swap, r_Parse_Params::param_type_int);
    params->add("rescale", &rescale, r_Parse_Params::param_type_int);
    params->add("domain", &domain, r_Parse_Params::param_type_string);
    params->process(options);
    r_Minterval t;
    RMInit::logOut  << "r_Conv_TOR::convert swap " << swap << " rescale " << rescale << " domain " << (domain?domain:"NULL") << std::endl;
    if (domain == NULL)
    {
        RMInit::logOut << "r_Conv_TOR::convertFrom no domain specified in options string" << std::endl;
        throw r_Error();
    }
    try
    {
        t = r_Minterval(domain);
    }
    catch (r_Eno_interval& e)
    {
        RMInit::logOut << "r_Conv_TOR::convertFrom no correct domain specified in options string" << std::endl;
        throw r_Error();
    }
    if (t.dimension() != 2)
    {
        RMInit::logOut << "r_Conv_TOR::convertFrom domain in options string must have 2 dimensions" << std::endl;
        throw r_Error();
    }
    r_Range x = t[0].high() - t[0].low() + 1;
    r_Range y = t[1].high() - t[1].low() + 1;
    r_UShort* array = (r_UShort*)mystore.storage_alloc(x * y * sizeof(r_UShort));
    double max = 0;
    double min = 0;
    r_Char tc;
    double maxu = 0;
    double minu = 0;
    r_UShort tu;
    switch (desc.baseType)
    {
    case ctype_bool:
    case ctype_char:
    case ctype_uint8:
        if (desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1 != t.cell_count() * sizeof(r_Char))
        {
            RMInit::logOut << "r_Conv_TOR::convertFrom the supplied data is " << desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1 << " bytes, but " << t.cell_count() << " bytes were expected" << std::endl;
            throw r_Error();
        }
        get_limits(&tc, min, max);
        get_limits(&tu, minu, maxu);
        for(int i=0; i < x; i++)
        {
            for(int j=0; j < y; j++)
            {
                if (rescale)
                    array[i * x + j] = ((r_Octet*)desc.src)[j * x + i] / max * maxu;
                else
                    array[i * x + j] = ((r_Octet*)desc.src)[j * x + i];
            }
        }
        break;
    case ctype_uint16:
        if (desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1 != t.cell_count() * sizeof(r_UShort))
        {
            RMInit::logOut << "r_Conv_TOR::convertFrom the supplied data is " << desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1 << " bytes, but " << t.cell_count() << " bytes were expected" << std::endl;
            throw r_Error();
        }
        for(int i=0; i < x; i++)
        {
            for(int j=0; j < y; j++)
            {
                array[i * x + j] = ((r_UShort*)desc.src)[j * x + i];
            }
        }
        break;
    default:
        RMInit::logOut << "r_Conv_TOR unknown base type!" << std::endl;
        throw r_Error();
    }
    if (swap)
    {
        r_UShort* temp = (r_UShort*)mystore.storage_alloc(x * y * sizeof(r_UShort));
        r_Endian::swap_array(x * y * sizeof(r_UShort), array, temp);
        mystore.storage_free(array);
        array = temp;
    }

    desc.dest = (char*)array;
    desc.destInterv = t;
    desc.destType = new r_Primitive_Type("UShort", r_Type::USHORT);
    return desc;
}

r_convDesc&
r_Conv_TOR::convertTo(const char* options) throw (r_Error)
{
    int swap = 0;
    int rescale = 0;
    params->add("swapendianness", &swap, r_Parse_Params::param_type_int);
    params->add("rescale", &rescale, r_Parse_Params::param_type_int);
    params->process(options);
    if (desc.srcInterv.dimension() != 2)
    {
        RMInit::logOut << "r_Conv_TOR::convertFrom domain in options string must have 2 dimensions" << std::endl;
        throw r_Error();
    }
    r_Range x = desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1;
    r_Range y = desc.srcInterv[1].high() - desc.srcInterv[1].low() + 1;
    r_UShort* array = (r_UShort*)mystore.storage_alloc(x * y * sizeof(r_UShort));
    double max = 0;
    double min = 0;
    r_Char tc;
    double maxu = 0;
    double minu = 0;
    r_UShort tu;
    switch (desc.baseType)
    {
    case ctype_bool:
    case ctype_char:
    case ctype_uint8:
        get_limits(&tc, min, max);
        get_limits(&tu, minu, maxu);
        for(int i=0; i < x; i++)
        {
            for(int j=0; j < y; j++)
            {
                if (rescale)
                    array[i * x + j] = ((r_Octet*)desc.src)[j * x + i] / max * maxu;
                else
                    array[i * x + j] = ((r_Octet*)desc.src)[j * x + i];
            }
        }
        break;
    case ctype_uint16:
        for(int i=0; i < x; i++)
        {
            for(int j=0; j < y; j++)
            {
                array[i * x + j] = ((r_UShort*)desc.src)[j * x + i];
            }
        }
        break;
    default:
        RMInit::logOut << "r_Conv_TOR unknown base type!" << std::endl;
        throw r_Error();
    }
    if (swap)
    {
        r_UShort* temp =(r_UShort*)mystore.storage_alloc(x * y * sizeof(r_UShort));
        r_Endian::swap_array(x * y * sizeof(r_UShort), array, temp);
        mystore.storage_free(array);
        array = temp;
    }

    desc.dest = (char*)array;
    r_Minterval td(1);
    td << r_Sinterval((r_Range)0, (r_Range)(desc.srcInterv.cell_count() * sizeof(r_UShort) - 1));
    desc.destInterv = td;
    desc.destType = new r_Primitive_Type("Char", r_Type::CHAR);
    return desc;
}

const char*
r_Conv_TOR::get_name() const
{
    return get_name_from_data_format(r_TOR);
}

r_Data_Format
r_Conv_TOR::get_data_format() const
{
    return r_TOR;
}

r_Convertor*
r_Conv_TOR::clone() const
{
    return new r_Conv_TOR(desc.src, desc.srcInterv, desc.srcType);
}

