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

#ifndef __RASNITF_RES_H
#define __RASNITF_RES_H

#include <vector>
#include <iostream>
#include <fstream>

namespace RasNITF
{

class res
{

    char m_re[2];
    char m_restag[25];
    char m_resver[2];
    char m_ressg[167];
    char m_resshl[4];
    char* m_resshf;
    char* m_resdata;

    long n_resshl;
    long data_length;
    long header_length;

    std::string res_hl;
    std::string res_dl;
public:

    res();
    ~res();

    int read_file(std::istream &,long,long);
    int write_file(std::ofstream &);
    std::string get_lr() const;
    std::string get_lrsh() const;

};

}

#endif
