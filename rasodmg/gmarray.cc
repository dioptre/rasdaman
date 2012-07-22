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
 * SOURCE: gmarray.cc
 *
 * MODULE: rasodmg
 * CLASS:  r_GMarray
 *
 * COMMENTS:
 *      None
*/

static const char rcsidgarray[] = "@(#)rasodmg, r_GMarray: $Id: gmarray.cc,v 1.45 2003/12/27 23:02:56 rasdev Exp $";

#include <vector>

#ifdef __VISUALC__
#ifndef __EXECUTABLE__
#define __EXECUTABLE__
#define GMARRAY_NOT_SET
#endif
#endif

#include "rasodmg/marray.hh"
#include "rasodmg/database.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/alignedtiling.hh"
#include "clientcomm/clientcomm.hh"

#ifdef GMARRAY_NOT_SET
#undef __EXECUTABLE__
#endif

#include "raslib/rmdebug.hh"
#include "raslib/type.hh"
#include "raslib/marraytype.hh"
#include "raslib/structuretype.hh"

#include <string.h>  // for memcpy()
#include <iostream>
#include <iomanip>



r_GMarray::r_GMarray() throw(r_Error)
    : r_Object(1),
      data(0),
      data_size(0),
      type_length(0),
      current_format(r_Array),
      storage_layout(0)
{
    storage_layout = new r_Storage_Layout();
}



r_GMarray::r_GMarray(const r_Minterval& initDomain, r_Bytes initLength, r_Storage_Layout* stl) throw (r_Error)
    : r_Object(1),
      data(0),
      data_size(0),
      domain(initDomain),
      type_length(initLength),
      current_format(r_Array),
      storage_layout(stl)
{
    int error = 0;
    if (domain.dimension() == 0)
    {
        error = 1;
    }
    else
    {
        if (storage_layout == NULL)
        {
            storage_layout = new r_Storage_Layout(new r_Aligned_Tiling(initDomain.dimension()));
        }
        if (!storage_layout->is_compatible(initDomain, initLength))
        {
            error = 2;
        }
    }
    if (error != 0)
    {
        RMInit::logOut << "r_GMarray::r_GMarray(" << initDomain << ", " << initLength << ", ";
        if (storage_layout == NULL)
            RMInit::logOut << "no storage layout";
        else
            RMInit::logOut << *storage_layout;
        RMInit::logOut << ") ";
        if (error == 1)
        {
            RMInit::logOut << "domain is not initialised";
            throw r_Error(DOMAINUNINITIALISED);
        }
        else
        {
            RMInit::logOut << "storage layout is not compatible";
            throw r_Error(STORAGERLAYOUTINCOMPATIBLEWITHGMARRAY);
        }
    }
    data_size = domain.cell_count() * initLength;
    data = new char[ data_size ];
    memset(data, 0, data_size);
}



r_GMarray::r_GMarray(const r_GMarray &obj) throw(r_Error)
    : r_Object(obj, 1),
      data(0),
      data_size(0),
      domain(obj.spatial_domain()),
      type_length(obj.type_length),
      current_format(obj.current_format),
      storage_layout(0)
{
    RMDBCLASS("r_GMarray", "r_GMarray(const r_GMarray)", "rasodmg", __FILE__, __LINE__)

    if (obj.data)
    {
        data_size = obj.data_size;
        data = new char[ data_size ];

        memcpy(data, obj.data, (unsigned int)data_size);
    }

    // clone the storage layout object
    if (obj.storage_layout)
        storage_layout = obj.storage_layout->clone();
    else
        RMInit::logOut << "copy constructor no storage layout" << endl;
}



r_GMarray::r_GMarray(r_GMarray &obj) throw(r_Error)
    : r_Object(obj, 1),
      data(obj.data),
      data_size(obj.data_size),
      domain(obj.spatial_domain()),
      type_length(obj.type_length),
      current_format(obj.current_format),
      storage_layout(0)
{
    RMDBCLASS("r_GMarray", "r_GMarray(r_GMarray)", "rasodmg", __FILE__, __LINE__)

    obj.data_size      = 0;
    obj.data           = 0;
    obj.domain         = r_Minterval();
    obj.type_length    = 0;

    // clone the storage layout object
    if (obj.storage_layout)
        storage_layout = obj.storage_layout->clone();
    else
        RMInit::logOut << "copy constructor (no data) no storage layout" << endl;
}



r_GMarray::~r_GMarray()
{
    r_deactivate();
}



void
r_GMarray::r_deactivate()
{
    RMDBGENTER(2, RMDebug::module_rasodmg, "r_GMarray", "r_deactivate()");

    if (data)
    {
        delete[] data;
        data = 0;
    }

    // invoke deactivate of members with dynamic memory
    domain.r_deactivate();

    // delete dynamic members
    if (storage_layout)
    {
        delete storage_layout;
        storage_layout = 0;
    }

    RMDBGEXIT(2, RMDebug::module_rasodmg, "r_GMarray", "r_deactivate()");
}



const char*
r_GMarray::operator[] (const r_Point& point) const
throw(r_Edim_mismatch, r_Eindex_violation)
{
    return &(data[ domain.cell_offset(point)*type_length ]);
}



const r_Storage_Layout*
r_GMarray::get_storage_layout() const
{
    return storage_layout;
}


void
r_GMarray::set_storage_layout(r_Storage_Layout *stl) throw(r_Error)
{
    if (!stl->is_compatible(domain, type_length))
    {
        RMInit::logOut << "r_GMarray::set_storage_layout(" << *stl << ") gmarray is not compatible with tiling" << endl;
        RMInit::logOut << "\tgmarray domain   : " << spatial_domain() << endl;
        RMInit::logOut << "\tgmarray type size: " << get_type_length() << endl;
        throw r_Error(STORAGERLAYOUTINCOMPATIBLEWITHGMARRAY);
    }

    if (storage_layout != NULL)
        delete storage_layout;

    storage_layout = stl;
}



const r_GMarray&
r_GMarray::operator=(const r_GMarray& marray)
{
    if (this != &marray)
    {
        if (data)
        {
            delete[] data;
            data = 0;
        }

        if (marray.data)
        {
            data_size = marray.data_size;
            data = new char[ data_size ];

            memcpy(data, marray.data, (unsigned int)data_size);
        }

        if (storage_layout)
        {
            delete storage_layout;
            storage_layout = 0;
        }

        // this has to be changed to a clone() function in future
        if (marray.storage_layout)
            // storage_layout = new r_Storage_Layout(*marray.storage_layout);
            storage_layout = marray.storage_layout->clone();

        domain         = marray.domain;
        type_length    = marray.type_length;
        current_format = marray.current_format;
    }

    return *this;
}



void
r_GMarray::insert_obj_into_db()
{
    // Nothing is done in that case. r_Marray objects can just be inserted as elements
    // of a collection which invokes r_GMarray::insert_obj_into_db(const char* collName)
    // of the r_Marray objects.

    // r_Database::actual_database->communication->insertSingleMDDObj(this);
    RMInit::logOut << " do nothing " << std::flush;
}



void
r_GMarray::insert_obj_into_db(const char* collName)
{
    // Insert myself in database only if I have a type name, otherwise
    // an exception is thrown.
    if (!type_name)
    {
        r_Error err = r_Error(r_Error::r_Error_DatabaseClassUndefined);
        throw err;
    }

    r_Database::actual_database->getComm()->insertMDD(collName, this);
}




void
r_GMarray::print_status(std::ostream& s) const
{
    const r_Type*       typeSchema     = (r_Base_Type*)((r_GMarray*)this)->get_type_schema();
    const r_Base_Type*  baseTypeSchema = (r_Base_Type*)((r_GMarray*)this)->get_base_type_schema();

    s << "GMarray" << endl;
    s << "  Oid...................: " << get_oid() << endl;
    s << "  Type Structure........: " << (get_type_structure() ? get_type_structure() : "<nn>") << endl;
    s << "  Type Schema...........: " << std::flush;
    if (typeSchema)
        typeSchema->print_status(s);
    else
        s << "<nn>" << std::flush;
    s << endl;
    s << "  Domain................: " << domain << endl;
    s << "  Base Type Schema......: " << std::flush;
    if (baseTypeSchema)
        baseTypeSchema->print_status(s);
    else
        s << "<nn>" << std::flush;
    s << endl;
    s << "  Base Type Length......: " << type_length << endl;
    s << "  Data format.......... : " << current_format << endl;
    s << "  Data size (bytes).... : " << data_size << endl;
}



void
r_GMarray::print_status(std::ostream& s, int hexoutput) const
{
    print_status(s);

    const r_Type*       typeSchema     = (r_Base_Type*)((r_GMarray*)this)->get_type_schema();
    const r_Base_Type*  baseTypeSchema = (r_Base_Type*)((r_GMarray*)this)->get_base_type_schema();

    if (domain.dimension())
    {
        r_Point p(domain.dimension());
        bool done = false;
        r_Dimension i = 0;

        // initialize point
        for (i = 0; i < domain.dimension(); i++)
            p << domain[i].low();

        // iterate over all cells
        while(!done)
        {
            //
            // print cell
            //

            // get cell address
            char* cell = data + domain.cell_offset(p) * type_length;

            if (hexoutput)
            {
                for (r_Bytes j = 0; j < type_length; j++)
                    s << std::hex << (unsigned int)(unsigned char)(cell[j]);
            }
            else
            {
                if(baseTypeSchema) baseTypeSchema->print_value(cell,  s );
                else s << "<nn>" << std::flush;
            }

            s << "   ";

            // increment coordinate
            i = 0;
            while(++p[i] > domain[i].high())
            {
                s << endl;
                p[i] = domain[i].low();
                i++;
                if (i >= domain.dimension())
                {
                    done = true;
                    break;
                }
            }
            if (i > 1) s << endl;
        }
    }
    else
    {
        s << "Cell value " << std::flush;

        // print cell
        if ((hexoutput) || (!baseTypeSchema))
        {
            for (unsigned int j=0; j<type_length; j++)
                s << std::hex << (unsigned int)(unsigned char)(data[j]);
        }
        else
        {
            if(baseTypeSchema) baseTypeSchema->print_value(data,  s );
            else s << "<nn>" << std::flush;
        }
        s << endl;
    }

    // turn off hex mode again
    s << std::dec << std::flush;
}




r_GMarray* r_GMarray::intersect(r_Minterval where) const
{
    r_GMarray* tile = new r_GMarray();

    r_Minterval obj_domain = spatial_domain();
    r_Dimension num_dims = obj_domain.dimension();
    r_Bytes tlength = get_type_length();

    char* obj_data = new char[where.cell_count() * tlength];
    tile->set_spatial_domain(where);
    tile->set_type_length(tlength);
    tile->set_array(obj_data);
    tile->set_array_size(where.cell_count() * tlength);

    r_Bytes block_length = where[num_dims-1].high() - where[num_dims-1].low() + 1;
    r_Bytes total = where.cell_count()/block_length;

    for (r_Area cell = 0; cell < total; cell++)
    {
        r_Point p = where.cell_point(cell*block_length);

        char* dest_off = obj_data;
        const char* source_off = get_array();

        memcpy((void*) (dest_off + where.cell_offset(p)*tlength),
               (void*) (source_off + obj_domain.cell_offset(p)*tlength),
               (size_t) (block_length * tlength));
    }

    return tile;
}



const r_Base_Type*
r_GMarray::get_base_type_schema()
{
    const r_Type*      typePtr     = r_Object::get_type_schema();
    const r_Base_Type* baseTypePtr = 0;

    if (typePtr)
    {
        if (typePtr->type_id() == r_Type::MARRAYTYPE)
        {
            const r_Marray_Type* marrayTypePtr = (const r_Marray_Type*)typePtr;
            baseTypePtr = &(marrayTypePtr->base_type());
        }
        else
        {
            //whenever this module is done correctly, it should be checked first, what kind of type we are dealing with.  therefore i do not declare a throw clause.
            RMInit::logOut << "r_GMarray::get_base_type_schema() the type retrieved (" << typePtr->name() << ") was not a marray type" << endl;
            throw r_Error(NOTANMARRAYTYPE);
        }
    }

    return baseTypePtr;
}

