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

#include <iostream>
#include <fstream>

#include "res.h"
#include "nitf.h"
#include "utilities.h"

using namespace std;
using namespace RasNITF;

res::res()
{

    m_resshf = NULL;
    m_resdata = NULL;
}

res::~res()
{

    if(m_resshf != NULL)
    {
        delete m_resshf;
        m_resshf = NULL;
    }

    if(m_resdata != NULL)
    {
        delete m_resdata;
        m_resdata = NULL;
    }
}

int res::read_file(istream &hNITF, long lrsh, long lr)
{

    int charsread = 0;

    header_length = lrsh;
    data_length = lr;

    charsread += read_verify2(hNITF, m_re, 2 + 25 + 2 + 167 + 4);
    n_resshl = charptrtolong(m_resshl,4);

    if (n_resshl > 0)
    {
        m_resshf = new char[n_resshl];
        charsread += read_verify2(hNITF, m_resshf, n_resshl);
    }

    m_resdata = new char[data_length];
    charsread += read_verify2(hNITF, m_resdata, data_length);

    return charsread;
}

int res::write_file(ofstream &fNITF)

{

    int charswritten = 0;

    fNITF.write(m_re, 2);
    fNITF.write(m_restag, 25);
    fNITF.write(m_resver, 2);
    fNITF.write(m_ressg, 167);
    fNITF.write(m_resshl, 4);

    if (fNITF.good()) charswritten += 2 + 25 + 2 + 167 + 4;

    if (n_resshl > 0)
    {
        fNITF.write(m_resshf, n_resshl);
    }

    if (fNITF.good()) charswritten += n_resshl;

    if( m_resdata != NULL)
    {
        fNITF.write( m_resdata, data_length);
    }

    if (fNITF.good()) charswritten += data_length;

    return charswritten;

}

string res::get_lr() const
{
    return res_dl;
}

string res::get_lrsh() const
{
    return res_hl;
}
