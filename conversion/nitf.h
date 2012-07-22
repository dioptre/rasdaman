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

/**
  * This is the definition of the NITF class, an agregation of the Classes Header,
  * Image, Graphic and Text
  */

#ifndef __RASNITF_NITF_H
#define __RASNITF_NITF_H

#include<string>
#include<vector>
#include<iostream>
#include<fstream>


namespace RasNITF
{

class image;
class graphic;
class text;
class des;
class res;

class nitf
{

    char m_fhdr[9];
    char m_clevel[2];
    char m_stype[4];
    char m_ostaid[10];
    char m_fdt[14];
    char m_ftitle[80];
    char m_fsclas[1];
    char m_fsclsy[2];
    char m_fscode[11];
    char m_fsctlh[2];
    char m_fsrel[20];
    char m_fsdctp[2];
    char m_fsdcdt[8];
    char m_fsdcxm[4];
    char m_fsdg[1];
    char m_fsdgdt[8];
    char m_fscltx[43];
    char m_fscatp[1];
    char m_fscaut[40];
    char m_fscrsn[1];
    char m_fssrdt[8];
    char m_fsctln[15];
    char m_fscop[5];
    char m_fscpys[5];
    char m_encryp[1];
    char m_fbkgc[3];
    char m_oname[24];
    char m_ophone[18];

    char m_fl[12];
    char m_hl[6];
    char m_numi[3];
    char* m_li;           // image sizes data
    char m_nums[3];
    char* m_ls;           // graphic sizes data
    char m_numx[3];
    char m_numt[3];
    char* m_lt;           // text sizes data
    char m_numdes[3];
    char* m_ld;           // des sizes data
    char m_numres[3];
    char* m_lr;           // res sizes data
    char m_udhdl[5];
    char m_udhofl[3];
    char* m_udhd;
    char m_xhdl[5];
    char m_xhdlofl[3];
    char* m_xhd;

    // DATA

    //std::ifstream hNITF;

    long n_fl;
    long n_hl;

    int n_numi;
    std::vector<long> n_lish;
    std::vector<long> n_li;
    image *m_images;

    int n_nums;
    std::vector<long> n_lssh;
    std::vector<long> n_ls;
    graphic *m_graphics;

    int n_numt;
    std::vector<long> n_ltsh;
    std::vector<long> n_lt;
    text *m_texts;

    int n_numdes;
    std::vector<long> n_ldsh;
    std::vector<long> n_ld;
    des *m_des;

    int n_numres;
    std::vector<long> n_lrsh;
    std::vector<long> n_lr;
    res *m_res;

    int n_udhdl;
    int n_xhdl;

    bool isEmpty;

public:

    nitf();
    ~nitf();

    int read_file(std::string filename);
    int read_headers_from(char* file_data, long file_length);
    int read(std::istream &hNITF, bool read_image_data);

    int write_file(std::string filename);

    void image_to_pixel_sequential(int index);

    void stats() const;
    void empty();
    bool checkempty();

    long get_image_size(int index = 0) const;
    int get_image_width(int index = 0) const;
    int get_image_height(int index = 0) const;
    int get_image_pixel_type(int index = 0) const;
    long get_image_offset(int index = 0) const;

    bool isRGB(int image_index);
    bool isGRAY(int image_index);
    bool isBOOL(int image_index);

};

}

#endif
