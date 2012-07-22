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

#include <string>
#include <stdio.h>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <exception>


#include "nitf.h"
#include "image.h"
#include "graphic.h"
#include "text.h"
#include "des.h"
#include "res.h"
#include "utilities.h"

using namespace std;
using namespace RasNITF;

nitf::nitf()
{

    isEmpty = true;

    m_li = NULL;
    m_ls = NULL;
    m_lt = NULL;
    m_ld = NULL;
    m_lr = NULL;
    m_udhd = NULL;
    m_xhd = NULL;

    m_images = NULL;
    m_graphics = NULL;
    m_texts = NULL;
    m_des = NULL;
    m_res = NULL;

    n_numi=0;
}
/*
nitf::nitf(char* data){

}
*/
nitf::~nitf()
{

    if (m_li != NULL)
    {
        delete m_li;
        m_li = NULL;
    }

    if (m_ls != NULL)
    {
        delete m_ls;
        m_ls = NULL;
    }

    if (m_lt != NULL)
    {
        delete m_lt;
        m_lt = NULL;
    }

    if (m_ld != NULL)
    {
        delete m_ld;
        m_ld = NULL;
    }

    if (m_lr != NULL)
    {
        delete m_lr;
        m_lr = NULL;
    }

    if (m_udhd != NULL)
    {
        delete m_udhd;
        m_udhd = NULL;
    }

    if (m_xhd != NULL)
    {
        delete m_xhd;
        m_xhd = NULL;
    }

    if (m_images != NULL)
    {
        delete[] m_images;
        m_images = NULL;
    }

    if (m_graphics != NULL)
    {
        delete[] m_graphics;
        m_graphics = NULL;
    }

    if (m_texts != NULL)
    {
        delete[] m_texts;
        m_texts = NULL;
    }

    if (m_des != NULL)
    {
        delete[] m_des;
        m_des = NULL;
    }

    if (m_res != NULL)
    {
        delete[] m_res;
        m_res = NULL;
    }

}

bool nitf::checkempty()
{
    return isEmpty;
}

void nitf::empty()
{

    if (!this->isEmpty)
    {

        if (m_li != NULL)
        {
            delete m_li;
            m_li = NULL;
        }

        if (m_ls != NULL)
        {
            delete m_ls;
            m_ls = NULL;
        }

        if (m_lt != NULL)
        {
            delete m_lt;
            m_lt = NULL;
        }

        if (m_ld != NULL)
        {
            delete m_ld;
            m_ld = NULL;
        }

        if (m_lr != NULL)
        {
            delete m_lr;
            m_lr = NULL;
        }

        if (m_udhd != NULL)
        {
            delete m_udhd;
            m_udhd = NULL;
        }

        if (m_xhd != NULL)
        {
            delete m_xhd;
            m_xhd = NULL;
        }

        if (m_images != NULL)
        {
            delete[] m_images;
            m_images = NULL;
        }

        if (m_graphics != NULL)
        {
            delete[] m_graphics;
            m_graphics = NULL;
        }

        if (m_texts != NULL)
        {
            delete[] m_texts;
            m_texts = NULL;
        }

        if (m_des != NULL)
        {
            delete[] m_des;
            m_des = NULL;
        }

        if (m_res != NULL)
        {
            delete[] m_res;
            m_res = NULL;
        }
    }

    this->isEmpty = true;
}

void nitf::stats() const
{
    int adress=n_hl;
    cout<<"Printing statistics for the NITF file in memmory\n";
    cout<<"================================================\n";
    cout<<"Filename:\t\t"<<endl;
    cout<<"File title:\n";
    cout.write(m_ftitle,80);
    cout<<"\nFile Length\t\t"<<n_fl;
    cout<<"\nFile Header Length\t"<<n_hl;
    cout<<"\nNumber of Images:\t"<<n_numi;
    cout<<"\n====================IMAGES======================\n";
    for(int i=0; i<n_numi; i++)
    {
        cout<<"Length of Image Header "<<i<<"\t"<<n_lish[i]<<" Starting at "<<adress<<endl;
        adress+=n_lish[i];
        cout<<"Length of Image "<<i<<"\t"<<n_li[i]<<" Starting at "<<adress<<endl;
        adress+=n_li[i];
    }
    cout<<"\nNumber of Graphics:\t"<<n_nums;
    cout<<"\n===================GRAPHICS======================\n";
    for(int i=0; i<n_nums; i++)
    {
        cout<<"Length of Graphic Header "<<i<<"\t"<<n_lssh[i]<<" Starting at "<<adress<<endl;
        adress+=n_lssh[i];
        cout<<"Length of Graphic "<<i<<"\t"<<n_ls[i]<<" Starting at "<<adress<<endl;
        adress+=n_ls[i];

    }
    cout<<"\nNumber of Texts:\t"<<n_numt;
    cout<<"\n====================TEXTS========================\n";
    for(int i=0; i<n_numt; i++)
    {

        cout<<"Length of Text Header "<<i<<"\t"<<n_ltsh[i]<<" Starting at "<<adress<<endl;
        adress+=n_ltsh[i];
        cout<<"Length of Text "<<i<<"\t"<<n_lt[i]<<" Starting at "<<adress<<endl;
        adress+=n_lt[i];
    }
    cout<<"\nNumber of Deses:\t"<<n_numdes;
    cout<<"\n====================DESES======================\n";
    for(int i=0; i<n_numdes; i++)
    {
        cout<<"Length of Des Header "<<i<<"\t"<<n_ldsh[i]<<" Starting at "<<adress<<endl;
        adress+=n_ldsh[i];
        cout<<"Length of Des "<<i<<"\t"<<n_ld[i]<<" Starting at "<<adress<<endl;
        adress+=n_ld[i];

    }
    cout<<"\nNumber of Reses:\t"<<n_numres;
    cout<<"\n====================RESES======================\n";
    for(int i=0; i<n_numres; i++)
    {

        cout<<"Length of Res Header "<<i<<"\t"<<n_lrsh[i]<<" Starting at "<<adress<<endl;
        adress+=n_lrsh[i];
        cout<<"Length of Res "<<i<<"\t"<<n_lr[i]<<" Starting at "<<adress<<endl;
        adress+=n_lr[i];
    }
    cout<<"\n\nThank you for using statistics, have a nice day\n";
    cout<<"---8<---8<---8<---8<---8<---8<---8<---8<---8<---8<---\n";
}


int nitf::read_file(string filename)
{

    ifstream hNITF;
    hNITF.open(filename.c_str(), ios::in);
    if (!hNITF.good()) exit(-1);
    bool read_image_data = true;

    return read(hNITF, read_image_data);

}

int nitf::read_headers_from(char* file_data, long file_length)
{

    istrstream hNITF(file_data, file_length);
    if (!hNITF.good()) exit(-1);
    bool read_image_data = false;

    return read(hNITF, read_image_data);;

}


int nitf::read(istream &hNITF, bool read_image_data)
{

    char temp_buffer[100];
    int charsread = 0;

    // CHECK IF EMPTY EXCEPTION
    if (!hNITF.good()) exit(-1);

    // read file name and version and check for consistency

    charsread += read_verify2(hNITF, m_fhdr, 9);

    if (strncmp(m_fhdr, "NITF02.10", 9) != 0)
        exit(2);

    // copy all security fields as character data
    charsread += read_verify2(hNITF, m_clevel, (2 + 4 + 10 + 14 + 80 + 1 + 2 + 11 + 2 +  20 + 2 + 8 + 4 + 1 + 8 + 43 +
                              1 + 40 + 1 + 8 + 15 + 5 + 5 + 1 + 3 + 24 + 18));

    // read file length
    charsread += read_verify2(hNITF, m_fl, 12);
    n_fl = charptrtolong(m_fl, 12);

    // read header length
    charsread += read_verify2(hNITF, m_hl, 6);
    n_hl = charptrtolong(m_hl, 6);

    // read number of images
    charsread += read_verify2(hNITF, m_numi, 3);
    n_numi = charptrtoint(m_numi, 3);
    m_li = new char[n_numi*(6+10)];

    for (int i = 0; i < n_numi; i++ )
    {

        charsread += read_verify2(hNITF, temp_buffer, 6);
        n_lish.push_back(charptrtolong(temp_buffer, 6));
        strncpy((char*)(m_li + i*16), temp_buffer, 6);

        charsread += read_verify2(hNITF, temp_buffer, 10);
        n_li.push_back(charptrtolong(temp_buffer, 10));
        strncpy((char*)(m_li + i*16 + 6), temp_buffer, 10);
    }

    // read number of graphics

    charsread += read_verify2(hNITF, m_nums, 3);
    n_nums = charptrtoint(m_nums, 3);
    m_ls = new char[n_nums*(4 + 6)];

    for (int i = 0; i < n_nums; i++ )
    {

        charsread += read_verify2(hNITF, temp_buffer, 4);
        n_lssh.push_back(charptrtolong(temp_buffer, 4));
        strncpy((char*)(m_ls + i*10), temp_buffer, 4);

        charsread += read_verify2(hNITF, temp_buffer, 6);
        n_ls.push_back(charptrtolong(temp_buffer, 6));
        strncpy((char*)(m_ls + i*10 + 4), temp_buffer, 6);
    }

    // read NUMX field

    charsread += read_verify2(hNITF, m_numx, 3);

    // read number of texts

    charsread += read_verify2(hNITF, m_numt, 3);
    n_numt = charptrtoint(m_numt, 3);
    m_lt = new char[n_numt*(4+5)];

    for (int i = 0; i < n_numt; i++ )
    {

        charsread += read_verify2(hNITF, temp_buffer, 4);
        n_ltsh.push_back(charptrtolong(temp_buffer, 4));
        strncpy((char*)(m_lt+i*9), temp_buffer, 4);

        charsread += read_verify2(hNITF, temp_buffer, 5);
        n_lt.push_back(charptrtolong(temp_buffer, 5));
        strncpy((char*)(m_lt + i*9 + 4), temp_buffer, 5);
    }

    // read DES sizes data

    charsread += read_verify2(hNITF, m_numdes, 3);
    n_numdes = charptrtoint(m_numdes, 3);
    m_ld = new char[n_numdes * (4 + 9)];

    for (int i = 0; i < n_numdes; i++ )
    {

        charsread += read_verify2(hNITF, temp_buffer, 4);
        n_ldsh.push_back(charptrtolong(temp_buffer, 4));
        strncpy((char*)(m_ld+i*13), temp_buffer, 4);

        charsread += read_verify2(hNITF, temp_buffer, 9);
        n_ld.push_back(charptrtolong(temp_buffer, 9));
        strncpy((char*)(m_ld + i*13 + 4), temp_buffer, 9);
    }

    // read RES sizes data

    charsread += read_verify2(hNITF, m_numres, 3);
    n_numres = charptrtoint(m_numres, 3);
    m_lr = new char[n_numres * (4 + 7)];

    for (int i = 0; i < n_numres; i++)
    {

        charsread += read_verify2(hNITF, temp_buffer, 4);
        n_lrsh.push_back(charptrtolong(temp_buffer, 4));
        strncpy((char*)(m_lr + i*11), temp_buffer, 4);

        charsread += read_verify2(hNITF, temp_buffer, 7);
        n_lr.push_back(charptrtolong(temp_buffer, 7));
        strncpy((char*)(m_lr + i*11 + 4), temp_buffer, 7);
    }

    // read UDHDL

    charsread += read_verify2(hNITF, m_udhdl, 5);
    n_udhdl = charptrtoint(m_udhdl, 5);

    // read UDHOFL and UDHD if necessary

    if (n_udhdl > 0)
    {

        charsread += read_verify2(hNITF, m_udhofl, 3);
        m_udhd = new char[n_udhdl-3];
        charsread += read_verify2(hNITF, m_udhd, n_udhdl-3);
    }

    // read XHDL

    charsread += read_verify2(hNITF, m_xhdl, 5);
    n_xhdl = charptrtoint(m_xhdl, 5);

    // read XHOFL and XHD if necessary

    if (n_xhdl > 0)
    {
        charsread += read_verify2(hNITF, m_xhdlofl, 3);
        m_xhd = new char[n_xhdl-3];
        charsread += read_verify2(hNITF, m_xhd, n_xhdl-3);
    }

    if (((long)(hNITF.tellg()))!= n_hl)
    {
        cout<< "header length read not as specified" << endl;
        cout<<endl<<"read: tellg:  "<< hNITF.tellg() << "  nhl " << n_hl<<endl;
        cout<< "chars read: " << charsread << endl;
        exit(3);
    }

    // read images

    if (n_numi > 0)
    {
        m_images = new image[n_numi];
        for(int i=0; i<n_numi; i++)
        {
            m_images[i].read_file(hNITF, n_lish[i], n_li[i], read_image_data);
        }
    }
    else
    {
        m_images = NULL;
    }

    // read graphics

    if (n_nums > 0)
    {
        m_graphics = new graphic[n_nums];
        for(int i=0; i<n_nums; i++)
        {
            m_graphics[i].read_file(hNITF, n_lssh[i], n_ls[i]);
        }
    }
    else
    {
        m_graphics = NULL;
    }

    // read texts

    if (n_numt > 0)
    {
        m_texts = new text[n_numt];
        for(int i=0; i<n_numt; i++)
        {
            m_texts[i].read_file(hNITF, n_ltsh[i], n_lt[i]);
        }
    }
    else
    {
        m_texts = NULL;
    }


    if (n_numdes > 0)
    {
        m_des = new des[n_numdes];
        for(int i=0; i<n_numdes; i++)
        {
            m_des[i].read_file(hNITF, n_ldsh[i], n_ld[i]);
        }
    }
    else
    {
        m_des = NULL;
    }

    if (n_numres > 0)
    {
        m_res = new res[n_numres];
        for(int i=0; i<n_numres; i++)
        {
            m_res[i].read_file(hNITF, n_lrsh[i], n_lr[i]);
        }
    }
    else
    {
        m_res = NULL;
    }


    this->isEmpty = false;

    return charsread;

}




int nitf::write_file(std::string filename)
{

    int charswritten = 0;
    ofstream fNITF(filename.c_str());

    fNITF.write(m_fhdr, 9);
    fNITF.write(m_clevel, 2);
    fNITF.write(m_stype, 4);
    fNITF.write(m_ostaid, 10);
    fNITF.write(m_fdt, 14);
    fNITF.write(m_ftitle, 80);
    fNITF.write(m_fsclas, 1);
    fNITF.write(m_fsclsy, 2);
    fNITF.write(m_fscode, 11);
    fNITF.write(m_fsctlh, 2);
    fNITF.write(m_fsrel, 20);
    fNITF.write(m_fsdctp, 2);
    fNITF.write(m_fsdcdt, 8);
    fNITF.write(m_fsdcxm, 4);
    fNITF.write(m_fsdg, 1);
    fNITF.write(m_fsdgdt, 8);
    fNITF.write(m_fscltx, 43);
    fNITF.write(m_fscatp, 1);
    fNITF.write(m_fscaut, 40);
    fNITF.write(m_fscrsn, 1);
    fNITF.write(m_fssrdt, 8);
    fNITF.write(m_fsctln, 15);
    fNITF.write(m_fscop, 5);
    fNITF.write(m_fscpys, 5);
    fNITF.write(m_encryp, 1);
    fNITF.write(m_fbkgc, 3);
    fNITF.write(m_oname, 24);
    fNITF.write(m_ophone, 18);
    fNITF.write(m_fl, 12);
    fNITF.write(m_hl, 6);

    charswritten += 9 + 2 + 4 + 10 + 14 + 80 + 1+ 2 + 11 + 2 + 20 + 2 + 8 + 4 + 1 + 8 + 43 + 1 + 40 + 1 +8 + 15 + 5 + 5 + 1 + 3 + 24 + 18 + 12 + 6;

    fNITF.write(m_numi, 3);
    charswritten += 3;
    if(n_numi > 0)
    {
        fNITF.write(m_li, n_numi * (6 + 10));
        charswritten += n_numi * (6 + 10);
    }

    fNITF.write(m_nums, 3);
    charswritten += 3;
    if(n_nums > 0)
    {
        fNITF.write(m_ls, n_nums * (4 + 6));
        charswritten += n_nums * (4 + 6);
    }

    fNITF.write(m_numx, 3);

    fNITF.write(m_numt, 3);
    charswritten += 6;
    if(n_numt > 0)
    {
        fNITF.write(m_lt, n_numt * (4 + 5));
        charswritten += n_numt * (4 + 5);
    }

    fNITF.write(m_numdes, 3);
    charswritten += 3;
    if(n_numdes > 0)
    {
        fNITF.write(m_ld, n_numdes * (4 + 9));
        charswritten += n_numdes * (4 + 9);
    }

    fNITF.write(m_numres, 3);
    charswritten += 3;
    if(n_numres > 0)
    {
        fNITF.write(m_lr, n_numdes * (4 + 7));
        charswritten += n_numdes* (4 + 7);
    }

    fNITF.write(m_udhdl, 5);
    if (n_udhdl > 0)
    {
        fNITF.write(m_udhofl, 3);
        fNITF.write(m_udhd, n_udhdl - 3);
        charswritten += n_udhdl;
    }


    fNITF.write(m_xhdl, 5);
    if (n_xhdl > 0)
    {
        fNITF.write(m_xhdlofl, 3);
        fNITF.write(m_xhd, n_xhdl - 3);
        charswritten += n_xhdl;
    }

    for(int i=0; i< n_numi; i++)
    {
        m_images[i].write_file(fNITF);
    }

    for(int i=0; i< n_nums; i++)
    {
        m_graphics[i].write_file(fNITF);
    }

    for(int i=0; i< n_numt; i++)
    {
        m_texts[i].write_file(fNITF);
    }

    for(int i=0; i< n_numdes; i++)
    {
        m_des[i].write_file(fNITF);
    }

    for(int i=0; i< n_numres; i++)
    {
        m_res[i].write_file(fNITF);
    }

    return charswritten;
}

void nitf::image_to_pixel_sequential(int index)
{

    if( !isEmpty )
    {
        if( (index < 0) || (index > n_numi))
        {
            throw(74);
            exit(3);
        }
        else
        {
            m_images[index].to_pixel_sequential();
        }
    }
}

long nitf::get_image_size(int image_index) const
{
    return m_images[image_index].get_size();
}

int nitf::get_image_width(int image_index) const
{
    return m_images[image_index].get_width();
}

int nitf::get_image_height(int image_index) const
{
    return m_images[image_index].get_height();
}

long nitf::get_image_offset(int image_index) const
{
    long offset = 0;

    offset += n_hl;

    for(int ctr = 0; ctr < image_index; ctr++ )
    {
        offset+= n_li[ctr] + n_lish[ctr];
    }

    offset += n_lish[image_index];

    return offset;
}

bool nitf::isRGB(int image_index)
{

    bool  RGB = false;

    if (m_images[image_index].get_irep() == "     RGB")
    {
        if (m_images[image_index].get_nbpp_bytes() == 1 )
        {
            RGB = true;
        }
        else
        {
            // throw(74);
            exit(1704);
        }
    }

    return RGB;

}

bool nitf::isGRAY(int image_index)
{

    bool GRAY = false;

    if ((m_images[image_index].get_irep() == "    MONO") && (m_images[image_index].get_pvtype() == "INT"))
    {
        if (m_images[image_index].get_nbpp_bytes() == 1 )
        {
            GRAY = true;
        }
        else
        {
            // throw(74);
            exit(1704);
        }
    }

    return GRAY;
}

bool nitf::isBOOL(int image_index)
{

    bool isBOOL = false;

    if ((m_images[image_index].get_irep() == "    MONO") && (m_images[image_index].get_pvtype() == "  B"))
    {
        if (m_images[image_index].get_nbpp_bytes() == 1 )
        {
            isBOOL = true;
        }
        else
        {
            // throw(74);
            exit(1704);
        }
    }

    return isBOOL;
}
