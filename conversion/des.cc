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

#include "des.h"
#include "nitf.h"
#include "utilities.h"

using namespace std;
using namespace RasNITF;

des::des()
{

    m_desshf = NULL;
    m_desdata = NULL;
    data_length = 0;
    header_length = 0;
}

des::~des()
{

    if(m_desshf != NULL)
    {
        delete m_desshf;
        m_desshf = NULL;
    }

    if(m_desdata != NULL)
    {
        delete m_desdata;
        m_desdata = NULL;
    }

}

int des::read_file(istream &hNITF, long desh_length, long des_length)
{

    int charsread = 0;

    header_length=desh_length;

    charsread += read_verify2(hNITF, m_de, 2 + 25 + 2 + 167);

    //if TRE_OF read teh next two
    if(strncmp(m_destag,"TRE_OVERFLOW",12)==0)
    {
        charsread += read_verify2(hNITF, m_desoflw, 6+3);
    }

    charsread += read_verify2(hNITF, m_desshl, 4);
    n_desshl = charptrtolong(m_desshl,4);

    if (n_desshl > 0)
    {
        m_desshf = new char[n_desshl];
        if(m_desshf == NULL) cerr<<"ERROR: could not allocate memory";
        charsread += read_verify2(hNITF, m_desshf, n_desshl);
    }

    m_desdata = new char[des_length];
    charsread += read_verify2(hNITF, m_desdata, des_length);
    data_length = des_length;

    return charsread;
}

int des::write_file(ofstream &fNITF)
{

    fNITF.write(m_de, 2);
    fNITF.write(m_destag, 25);
    fNITF.write(m_desver, 2);
    fNITF.write(m_dessg, 167);

    if(strncmp(m_destag,"TRE_OVERFLOW",12)==0)
    {
        fNITF.write(m_desoflw, 6);
        fNITF.write(m_desitem, 3);
    }

    fNITF.write( m_desshl, 4);

    if (n_desshl > 0)
    {
        fNITF.write(m_desshf, n_desshl);
    }

    if( m_desdata != NULL)
    {
        fNITF.write( m_desdata, data_length);
    }

    //TODO
    return 0;

}

string des::get_ld() const
{
    return des_dl;
}

string des::get_ldsh() const
{
    return des_hl;
}
