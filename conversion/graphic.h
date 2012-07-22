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


#ifndef __GRAPHIC_H_INCLUDED
#define __GRAPHIC_H_INCLUDED

#include<vector>
#include<iostream>
#include<fstream>

namespace RasNITF
{

class graphic
{
private:
    char m_sy[2];
    char m_sid[10];
    char m_sname[20];
    char m_ssclas[1];
    char m_ssclsy[2];
    char m_sscode[11];
    char m_ssctlh[2];
    char m_ssrel[20];
    char m_ssdctp[2];
    char m_ssdcdt[8];
    char m_ssdcxm[4];
    char m_ssdg[1];
    char m_ssdgdt[8];
    char m_sscltx[43];
    char m_sscatp[1];
    char m_sscaut[40];
    char m_sscrsn[1];
    char m_sssrdt[8];
    char m_ssctln[15];
    char m_encryp[1];
    char m_stype[1];
    char m_sres1[13];
    char m_sdlvl[3];
    char m_salvl[3];
    char m_sloc[10];
    char m_sbnd1[10];
    char m_scolor[1];
    char m_sbnd2[10];
    char m_sres2[2];
    char m_sxshdl[5];
    char m_sxsofl[3];
    char* m_sxshd;
    char *m_graphic_data;

    int header_length;
    int data_length;
    int n_sxshdl;
    int n_sxsofl;

    std::string graphic_hl ;
    std::string graphic_dl ;

public:
    graphic();
    ~graphic();
    int read_file(std::istream&, long, long);
    int write_file(std::ofstream &);
    std::string get_ls();
    std::string get_lssh();
};

}

#endif
