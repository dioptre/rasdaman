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
#include <cstring>

#include "graphic.h"
#include "nitf.h"
#include "utilities.h"

using namespace std;
using namespace RasNITF;

graphic::graphic()
{
    m_graphic_data = NULL;
    m_sxshd = NULL;
}

graphic::~graphic()
{
    if(m_graphic_data != NULL)
        delete m_graphic_data;
    m_graphic_data = NULL;

    if(m_sxshd != NULL)
        delete m_sxshd;
    m_sxshd = NULL;

}

int graphic::read_file(istream &hNITF, long gh_length, long graph_length)
{
    int charsread = 0;
    long start_position = 0;

    header_length = gh_length;

    // here find where are we relative to the begining
    start_position = hNITF.tellg();

    // here read the header data
    charsread += read_verify2(hNITF,m_sy, 2+10+20+1+2+11+2+20+2+8+4+1+8+43+1+40+1+8+15+1+1+13+3+3+10+10+1+10+2);

    charsread += read_verify2(hNITF, m_sxshdl, 5);

    n_sxshdl = charptrtolong(m_sxshdl, 5);

    if (n_sxshdl > 0)
    {
        n_sxshdl -= 3;
        read_verify2(hNITF, m_sxsofl, 3);
        m_sxshd = new char[n_sxshdl - 3];
        if(m_sxshd == NULL) cerr<<"ERROR: could not allocate memory";
        read_verify2(hNITF, m_sxshd, n_sxshdl);
        n_sxshdl += 3;

    }

    // check if we are where we should be

    if( ((long)hNITF.tellg()) != (start_position + gh_length))
    {
        cerr << "Error in graph.cpp";
        exit(2);
    }

    // store the text data

    m_graphic_data = new char[graph_length];
    if(m_graphic_data == NULL) cerr<<"ERROR: could not allocate memory";
    data_length = read_verify2(hNITF, m_graphic_data, graph_length);
    return charsread;
}

int graphic::write_file(ofstream &fNITF)
{

    fNITF.write( m_sy, 2);
    fNITF.write( m_sid, 10);
    fNITF.write( m_sname, 20);
    fNITF.write( m_ssclas, 1);
    fNITF.write( m_ssclsy, 2);
    fNITF.write( m_sscode, 11);
    fNITF.write( m_ssctlh, 2);
    fNITF.write( m_ssrel, 20);
    fNITF.write( m_ssdctp, 2);
    fNITF.write( m_ssdcdt, 8);
    fNITF.write( m_ssdcxm, 4);
    fNITF.write( m_ssdg, 1);
    fNITF.write( m_ssdgdt, 8);
    fNITF.write( m_sscltx, 43);
    fNITF.write( m_sscatp, 1);
    fNITF.write( m_sscaut, 40);
    fNITF.write( m_sscrsn, 1);
    fNITF.write( m_sssrdt, 8);
    fNITF.write( m_ssctln, 15);
    fNITF.write( m_encryp, 1);
    fNITF.write( m_stype, 1);
    fNITF.write( m_sres1, 13);
    fNITF.write( m_sdlvl, 3);
    fNITF.write( m_salvl, 3);
    fNITF.write( m_sloc, 10);
    fNITF.write( m_sbnd1, 10);
    fNITF.write( m_scolor, 1);
    fNITF.write( m_sbnd2, 10);
    fNITF.write( m_sres2, 2);
    fNITF.write(m_sxshdl, 5);

    if (n_sxshdl > 0)
    {
        fNITF.write(m_sxsofl, 3);
        fNITF.write(m_sxshd, n_sxshdl - 3);
    }

    if( m_graphic_data != NULL)
    {
        fNITF.write( m_graphic_data, data_length);
    }

    //TODO change to charswritten
    return 0;

}

string graphic::get_ls()
{
    return graphic_dl;
}

string graphic::get_lssh()
{
    return graphic_hl;
}
