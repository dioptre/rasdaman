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
 * SOURCE: miterd.cc
 *
 * MODULE: raslib
 * CLASS:  r_Miter
 *
*/

#include <iostream>

#include "raslib/miterd.hh"
#include "raslib/minterval.hh"



r_MiterDirect::r_MiterDirect(void *data, const r_Minterval &total, const r_Minterval &iter, unsigned int tlen, unsigned int step)
    :   done(false),
        dim(total.dimension()),
        length(step),
        id(NULL),
        baseAddress(data)
{
    int i = 0;
    r_Range s = tlen;
    r_Range offset = 0;

    id=new r_miter_direct_data[dim];

    for (i=(int)dim-1; i>=0; i--)
    {
        id[i].low = iter[i].low();
        id[i].high = iter[i].high();
        id[i].pos = id[i].low;
        id[i].origin = total[i].low();
        id[i].extent = (total[i].high() - total[i].low() + 1);
        id[i].baseStep = s;
        id[i].step = s * step;
        offset += s * (id[i].pos - id[i].origin);
        s *= id[i].extent;
    }
    for (i=0; i<(int)dim; i++)
    {
        id[i].data = (void*)(((r_Octet*)data) + offset);
    }
}

r_MiterDirect::~r_MiterDirect(void)
{
    delete [] id;
}


void r_MiterDirect::reset(void)
{
    r_Dimension i = 0;
    r_ULong offset = 0;

    for (i=0; i<dim; i++)
    {
        id[i].pos = id[i].low;
        offset += id[i].step * (id[i].low - id[i].origin);
    }
    for (i=0; i<dim; i++)
    {
        id[i].data = (void*)(((r_Octet*)baseAddress) + offset);
    }
    done = false;
}


void
r_MiterDirect::print_pos(std::ostream &str) const
{
    str << '[' << id[0].pos;
    for (r_Dimension i=1; i<dim; i++) str << ',' << id[i].pos;
    str << ']';
}



std::ostream &operator<<(std::ostream &str, const r_MiterDirect &iter)
{
    iter.print_pos(str);
    return str;
}

r_miter_direct_data::r_miter_direct_data()
    :data(NULL), pos(0), low(0), high(0),
     step(0), baseStep(0), extent(0), origin(0)
{
}

r_miter_direct_data::~r_miter_direct_data()
{
}
