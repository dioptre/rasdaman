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


#ifndef __TEXT_H_INCLUDED
#define __TEXT_H_INCLUDED

#include<vector>
#include <iostream>
#include <fstream>

namespace RasNITF
{
/* forward declarations follow : */

class nitf;

class text
{
private:
    char m_te[2];
    char m_textid[7];
    char m_txtalvl[3];
    char m_txtdt[14];
    char m_txtitl[80];
    char m_tsclas[1];
    char m_tsclsy[2];
    char m_tscode[11];
    char m_tsctlh[2];
    char m_tsrel[20];
    char m_tsdctp[2];
    char m_tsdcdt[8];
    char m_tsdcxm[4];
    char m_tsdg[1];
    char m_tsdgdt[8];
    char m_tscltx[43];
    char m_tscatp[1];
    char m_tscaut[40];
    char m_tscrsn[1];
    char m_tssrdt[8];
    char m_tsctln[15];
    char m_encryp[1];
    char m_txtfmt[3];
    char m_txshdl[5];
    char m_txsofl[3];
    char* m_txshd;
    char *m_text_data;

    int header_length;
    int data_length;
    int n_txshdl;
    int n_txsofl;

    std::string text_hl ;
    std::string text_dl ;

public:
    text();
    ~text();        //destructor freeing memory
    int read_file(std::istream &,long,long);
    int write_file(std::ofstream &);    //writes the information to the data file
    std::string get_lt() const;
    std::string get_ltsh() const;
};
}
#endif
