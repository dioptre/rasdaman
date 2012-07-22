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

#ifndef __RASNITF_IMAGE_H
#define __RASNITF_IMAGE_H

#include <fstream>
#include <iostream>

namespace RasNITF
{

class image_band
{

public:

    char irepbandnn[2];
    char isubcatnn[6];
    char ifcnn[1];
    char imfltnn[3];
    char nlutsnn[1];
    char nelutnn[5];
    int numluts;
    int numlutentries;
    char* lutdnnm;

    image_band();
    ~image_band();

};

class image
{

    /*********************************
     * image header fields
     ********************************/

    char m_im[2];
    char m_iid1[10];
    char m_idatim[14];
    char m_tgtid[17];
    char m_iid2[80];
    char m_isclas[1];
    char m_isclsy[2];
    char m_iscode[11];
    char m_isctlh[2];
    char m_isrel[20];
    char m_isdctp[2];
    char m_isdcdt[8];
    char m_isdcxm[4];
    char m_isdg[1];
    char m_isdgdt[8];
    char m_iscltx[43];
    char m_iscatp[1];
    char m_iscaut[40];
    char m_iscrsn[1];
    char m_issrdt[8];
    char m_isctln[15];
    char m_encryp[1];
    char m_isorce[42];
    char m_nrows[8];
    char m_ncols[8];
    char m_pvtype[3];
    char m_irep[8];
    char m_icat[8];
    char m_abpp[2];
    char m_pjust[1];
    char m_icords[1];
    char m_igeolo[60];
    char m_nicom[1];
    char* m_icom;
    char m_ic[2];
    char m_comrat[4];
    char m_nbands[1];
    char m_xbands[5];
    image_band* m_bands;
    char m_isync[1];
    char m_imode[1];
    char m_nbpr[4];
    char m_nbpc[4];
    char m_nppbh[4];
    char m_nppbv[4];
    char m_nbpp[2];
    char m_idlvl[3];
    char m_ialvl[3];
    char m_iloc[10];
    char m_imag[4];
    char m_udidl[5];
    char m_udofl[3];
    char* m_udid;
    char m_ixshdl[5];
    char m_ixsofl[3];
    char* m_ixshd;

    /*****************************
     *  image data mask table
     ****************************/

    /*
    long imdataoff;
    unsigned short bmrlnth;
    unsigned short tmrlnth;
    unsigned short tpxcdlnth;
    long tpxcd;
    char* BMRnBNDm;
    char* TMRnBNDm;
    */

    /*****************************
     * raw image data
     *****************************/

    char* m_data;

    /*****************************
     *  additional variables
     *****************************/

//    bool compressed;
    int n_nicom;
    int n_nbands;
    int n_xbands;
    int numbands;
    int n_udidl;
    int n_ixshdl;
    long header_length;
    long data_length;
    char texthl[6];
    char textdl[10];
    int n_nbpr;
    int n_nbpc;
    int n_nppbh;
    int n_nppbv;

    /*****************************
     *  variables for data reordering
     *  populated by compute_data_reordering_variables()
     *****************************/

    int n_nrows;
    int n_ncols;
    int px_comp_size;
    int px_size;
    int numblocks;

    long block_size_bytes;
    long block_size_pixels;
    long block_row_size; //bytes
    long block_band_size; //bytes
    long block_band_size_px;
    long image_band_size;


    int copy_block_data(char* writeto, int hor_block_index, int ver_block_index );

    // functions for data reordering

    void compute_data_reordering_variables();

    int blpxseq2pxseq(char* src, char* dest);
    int pxseq2blpxseq(char*, char*);

    int imode_1band2ps();
    int imode_b2ps();
    int imode_r2ps();
    int imode_p2ps();
    int imode_s2ps();

    int imode_ps21band(char*);
    int imode_ps2b(char*);
    int imode_ps2r(char*);
    int imode_ps2p(char*);
    int imode_ps2s(char*);


public:

    image();
    ~image();

    int read_file(std::istream&, long, long, bool read_image_data);
    int write_file(std::ofstream& fs);

    std::string get_li() const;
    std::string get_lish() const;
    long get_size() const;
    int get_width() const;
    int get_height() const;

    int to_pixel_sequential();
    int to_block_pixel_sequential(char* src);

    std::string get_irep() const;
    std::string get_pvtype() const;

    int get_nbpp_bytes() const;

//    char* get_data_ptr();


};

}

#endif


