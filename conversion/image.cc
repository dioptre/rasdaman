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

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
/*
#include <strstream>

#include "raslib/rminit.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/set.hh"
#include "rasodmg/database.hh"
#include "rasodmg/partinsert.hh"
#include "raslib/type.hh"
#include "raslib/odmgtypes.hh"
*/
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "image.h"
#include "utilities.h"

using namespace std;
using namespace RasNITF;

 
typedef struct {
  unsigned char red, green, blue;
} RGBPixel;

image_band::image_band(){
    lutdnnm = NULL;
}

image_band::~image_band(){

    if (lutdnnm != NULL){
	delete lutdnnm;
	lutdnnm = NULL;
    }
}

image::image(){

    m_icom = NULL;
    m_bands = NULL;
    m_udid = NULL;
    m_ixshd = NULL;
    m_data = NULL;

}

image::~image(){
    
    if (m_icom != NULL) {
	delete m_icom;
	m_icom = NULL;
    }

    if (m_bands != NULL){
	delete[] m_bands;
	m_bands = NULL;
    }
    
    if (m_udid != NULL) {
	delete m_udid;
	m_udid = NULL;
    }

    if (m_ixshd != NULL) {
	delete m_ixshd;
	m_ixshd = NULL;
    }

    if (m_data != NULL) {
	delete m_data;
	m_data = NULL;
    }
}


/**********************************************************************
 * NAME: image::read_file
 * PURPOSE: read an image from an ntf file into an image object
 *********************************************************************/

int image::read_file(istream &hNITF, long lish, long li,  bool read_image_data){
    
    int charsread=0;

    // read IM through ICORDS

    charsread += read_verify2(hNITF, m_im, (2+10+14+17+80+1+2+11+2+20+2+8+4+1+8+43+1+40+1+8+15+1+42+8+8+3+8+8+2+1+1));

    // read IGEOLO field if used

    if (m_icords[0] != ' ') { 	
	charsread += read_verify2(hNITF, m_igeolo, 60);
    } else {
	memset(m_igeolo, ' ', 60);
	charsread += 60;
    }

    // read NICOM and ICOM

    charsread += read_verify2(hNITF, m_nicom, 1);
    n_nicom = charptrtoint(m_nicom, 1);

    if (n_nicom > 0) {
	m_icom = new char[n_nicom * 80]; 
	charsread += read_verify2(hNITF, m_icom, n_nicom * 80);
     } else {
	 m_icom = NULL;
     }

    // read IC
     
    charsread += read_verify2(hNITF, m_ic, 2);

    // read COMRAT if necessary
    
    if ((strncmp(m_ic, "NC", 2) != 0) && (strncmp(m_ic, "NM", 2) != 0)) {
	charsread += read_verify2(hNITF, m_comrat, 4);
    }
             
    charsread += read_verify2(hNITF, m_nbands, 1);
    n_nbands = charptrtoint(m_nbands, 1);

    if (n_nbands == 0) {
	charsread += read_verify2(hNITF, m_xbands, 5);
	n_xbands = charptrtoint(m_xbands, 5);
	numbands = n_xbands;
    } else {
	n_xbands = 0; 
	numbands = n_nbands;
    }

    if (numbands > 0){
   
	m_bands = new image_band[numbands];

	for (int i = 0; i < numbands; i++) {

	    // read IREPBANDnn..NLUTSnn
	    charsread += read_verify2(hNITF, m_bands[i].irepbandnn, (2+6+1+3+1));
	    
	    // get # of LUTS for this band
	    m_bands[i].numluts = charptrtoint( m_bands[i].nlutsnn, 1);

	    // read LUTs entries per LUT
	    if (m_bands[i].numluts > 0) {

		// read # of LUT entries
		charsread += read_verify2(hNITF, m_bands[i].nelutnn, 5);
		m_bands[i].numlutentries = charptrtoint(m_bands[i].nelutnn, 5);

		// allocate space for the LUT entries
		m_bands[i].lutdnnm = new char[m_bands[i].numluts * m_bands[i].numlutentries];

		// read LUT entries
		charsread += read_verify2(hNITF, m_bands[i].lutdnnm, m_bands[i].numluts * m_bands[i].numlutentries);
        
	    } else {
	    
		m_bands[i].lutdnnm = NULL;
	    }
    
	} /* end for (band_num = 0... */
    
    } else {

	m_bands = NULL;

    }

    // read m_isync..udidl

    charsread += read_verify2(hNITF, m_isync, (1+1+4+4+4+4+2+3+3+10+4+5));
    n_udidl = charptrtolong(m_udidl, 5);

    if (n_udidl > 0) {                  
	charsread += read_verify2(hNITF, m_udofl, 3);
	m_udid = new char[n_udidl];
	charsread += read_verify2(hNITF, m_udid, n_udidl - 3);
    } 

    // read ixshdl

    charsread +=  read_verify2(hNITF, m_ixshdl, 5);
    n_ixshdl = charptrtolong(m_ixshdl, 5);

    if (n_ixshdl > 0) {
	charsread += read_verify2(hNITF, m_ixsofl, 3);
	m_ixshd = new char[n_ixshdl];
	charsread += read_verify2(hNITF, m_ixshd, n_ixshdl - 3);
    } 
  
    /*********************************************************************** 
     *   MASKED IMAGES ARE IGNORED FOR NOW !!!
     ***********************************************************************

    strncpy(temp_buffer, m_ic, 2); 
    temp_buffer[2] = '\0';
    
    if (strcmp(temp_buffer, "NM") == 0 || strcmp(temp_buffer, "M1") == 0
	                               || strcmp(temp_buffer, "M3") == 0
                                       || strcmp(temp_buffer, "M4") == 0
                                       || strcmp(temp_buffer, "M5") == 0) {

	// ERROR not implemented yet masked images
    }
    
    ************************************************************************/

    // copy image data

    header_length = lish;
    data_length = li;

    if(read_image_data == true) {
	m_data = new char[li];
	charsread += read_verify2(hNITF, m_data, li);
    } else {
	charsread += li; 
    }
    n_nbpr = 0;                    // number of blocks per row
    n_nbpc = 0;                    // number of blocks per column
    n_nppbh = 0;                   // number of of pixels per block horizontally
    n_nppbv = 0;                   // number of of pixels per block vertically
    
    n_nppbh =  charptrtoint( m_nppbh, 4); 
    n_nppbv =  charptrtoint( m_nppbv, 4);
    n_nbpr = charptrtoint( m_nbpr, 4);
    n_nbpc = charptrtoint( m_nbpc, 4);
	
	cout<<"IC: ";
	cout.write(m_ic, 2);

	cout<<"\nNBANDS: ";
	cout.write(m_nbands, 1);
 
	cout<<"\nnumber of blocks per row: ";
	cout.write(m_nbpr, 4);
	
	cout<<"\nNumber of blocks per column: ";
	cout.write(m_nbpc, 4);

	cout<<"\nImode: ";
	cout.write(m_imode, 1);

    return charsread;
   
}

/**********************************************************************
 * NAME: image::write_file
 * PURPOSE: write an image object into an ntf file on hard drive
 *********************************************************************/

int image::write_file(ofstream &fs){  
    
    int charswritten=0;

    fs.write(m_im, 2);
    fs.write(m_iid1, 10);
    fs.write(m_idatim, 14);
    fs.write(m_tgtid, 17);
    fs.write(m_iid2, 80);
    fs.write(m_isclas, 1);
    fs.write(m_isclsy, 2);
    fs.write(m_iscode, 11);
    fs.write(m_isctlh, 2);
    fs.write(m_isrel, 20);
    fs.write(m_isdctp, 2);
    fs.write(m_isdcdt, 8);
    fs.write(m_isdcxm, 4);
    fs.write(m_isdg, 1);
    fs.write(m_isdgdt, 8);
    fs.write(m_iscltx, 43);
    fs.write(m_iscatp, 1);
    fs.write(m_iscaut, 40);
    fs.write(m_iscrsn, 1);
    fs.write(m_issrdt, 8);
    fs.write(m_isctln, 15);
    fs.write(m_encryp, 1);
    fs.write(m_isorce, 42);
    fs.write(m_nrows, 8);
    fs.write(m_ncols, 8);
    fs.write(m_pvtype, 3);
    fs.write(m_irep, 8);
    fs.write(m_icat, 8);
    fs.write(m_abpp, 2);
    fs.write(m_pjust, 1);
    fs.write(m_icords, 1);

    if (m_icords[0] != ' '){
	fs.write(m_igeolo, 60);  
	charswritten += 60;
    }
    fs.write(m_nicom, 1);

    charswritten += 2 + 10 + 14 + 17 + 80 + 1 + 2 + 11 + 2 + 20 + 2 + 8 + 4 +1 + 8 + 43 + 1 + 40 + 1 + 8 + 15 + 1 + 42 + 8 + 8 + 3 + 8 + 8 + 2 + 1 + 1 + 1;

    if (m_icom!=NULL) {
	fs.write(m_icom, n_nicom * 80); 
	charswritten += n_nicom * 80;
    }

    fs.write(m_ic, 2);
    charswritten += 2;

    if ((strncmp(m_ic, "NC", 2) != 0) && (strncmp(m_ic, "NM", 2) != 0))  {
	fs.write(m_comrat, 4);
	charswritten += 4;
    }
    fs.write(m_nbands, 1);
    charswritten += 1;

    if (n_nbands == 0) {
	fs.write(m_xbands, 5);
	 charswritten += 5;
    }

    if (m_bands != NULL){
	for (int i=0; i<numbands; i++){
	    fs.write(m_bands[i].irepbandnn, 2);
	    fs.write(m_bands[i].isubcatnn, 6);
	    fs.write(m_bands[i].ifcnn, 1);
	    fs.write(m_bands[i].imfltnn, 3);
	    fs.write(m_bands[i].nlutsnn, 1);

	    charswritten += 2 + 6 + 1 + 3 + 1;

	    if (m_bands[i].numluts > 0){
		fs.write(m_bands[i].nelutnn, 5);		
		fs.write(m_bands[i].lutdnnm, m_bands[i].numluts * m_bands[i].numlutentries);
		
		 charswritten += 5 + m_bands[i].numluts * m_bands[i].numlutentries;
	    }   
	}
    }

    fs.write(m_isync, 1);
    fs.write(m_imode, 1);
    fs.write(m_nbpr, 4);
    fs.write(m_nbpc, 4);
    fs.write(m_nppbh, 4);
    fs.write(m_nppbv, 4);
    fs.write(m_nbpp, 2);
    fs.write(m_idlvl, 3);
    fs.write(m_ialvl, 3);
    fs.write(m_iloc, 10);
    fs.write(m_imag, 4);
    fs.write(m_udidl, 5);

    charswritten += 1 + 1 + 4 + 4 + 4 +4 + 2 + 3 + 3 + 10 + 4 + 5;

    if (n_udidl > 0) {
	fs.write(m_udofl, 3);
	fs.write(m_udid, n_udidl - 3);
	charswritten += n_udidl;
    }

    fs.write(m_ixshdl, 5);
    charswritten += 5;

     if (n_ixshdl > 0) {
	fs.write(m_ixsofl, 3);
	fs.write(m_ixshd, n_ixshdl - 3);
	charswritten += n_ixshdl;
    }

    
cout<<" wrote image header length: " << charswritten <<endl;
if(m_data){ 
fs.write(m_data, data_length);
cout<< "image data length is : "<<data_length<<endl ;
charswritten += data_length;
}
cout<<"IMAGE WRITE_FILE wrote "<< charswritten<<endl; 

    return charswritten;
}


/**********************************************************************
 * NAME:image::get_li
 * -------------------------------------------------------------------
 * PURPOSE: image size
 *********************************************************************/

string image::get_li() const {
   string temp(textdl);
   return temp;
}

/**********************************************************************
 * NAME:image::get_lish
 * -------------------------------------------------------------------
 * PURPOSE: returns the image header size
 *********************************************************************/

string image::get_lish() const {
   string temp(texthl);
   return temp;
}


/**********************************************************************
 * NAME: image::compute_data_reordering_variables
 *---------------------------------------------------------------------
 * PURPOSE: computes some variable necessary for converting data from
 * the ntf internal data storage to pixel sequential storage and vice
 * versa
 *********************************************************************/

void image::compute_data_reordering_variables(){

    int px_comp_size_bits = charptrtoint(m_nbpp, 2);
    
    if ((px_comp_size_bits % 8) == 0) {
	 px_comp_size = px_comp_size_bits / 8;
    } else {
 	cout << "unsupported pixel component size "<<endl;
	exit(101);
    }
        
    px_size = px_comp_size * numbands;
    numblocks = n_nbpc * n_nbpr;    
    block_size_bytes = data_length / numblocks;
    block_size_pixels = n_nppbv * n_nppbh;
    block_row_size = block_size_bytes / n_nppbv; 
    block_band_size = block_size_bytes /numbands;
    block_band_size_px = block_size_pixels / numbands;
    image_band_size = data_length / numbands;

    n_nrows = charptrtolong(m_nrows, 8);
    n_ncols = charptrtolong(m_ncols, 8);

}

/**********************************************************************
 * NAME: image::blpxseq2pxseq
 * -------------------------------------------------------------------
 * PURPOSE: coverts memory data from block pixel sequential to pixel 
 * sequential storage
 *********************************************************************/

int image::blpxseq2pxseq(char* src, char* dest){  

    char *rowblockstart = NULL;
    char *writefrom = NULL;
    

    cout<< "n_nbpc: "<<n_nbpc <<endl;
    cout<< "n_nbpr: " <<n_nbpr<<endl;
    cout<< "block_size_bytes: " << block_size_bytes <<endl;
    cout<< "block_row_size: " << block_row_size  <<endl;   
 
    for (int vbindex=0; vbindex < n_nbpc; vbindex++){
	
	rowblockstart = (char*)(src + vbindex * n_nbpr * block_size_bytes); 
	
	for (int hbindex = 0; hbindex < n_nbpr; hbindex++){
	    
	    for(int i = 0; i < n_nppbv; i++){
		
		writefrom = (char*) (rowblockstart +  i * n_nbpr * block_row_size + hbindex * block_row_size );
		strncpy(dest, writefrom , block_row_size);
		dest += block_row_size;
		
		cout<< "writing row: "<< i <<" of BLOCK: " << hbindex << "IN BLOCK ROW "<< vbindex<<endl;

		cout << "writefrom "<< int(writefrom) <<"dest" << int(dest) <<" block row size " << block_row_size <<endl;	
	    }
	}
    }
    
    return 0;

}

/**********************************************************************
 * NAME: image::pxseq2blpxseq
 * -------------------------------------------------------------------
 * PURPOSE: coverts memory data from pixel sequential to block pixel 
 * sequential storage
 *********************************************************************/

int image::pxseq2blpxseq(char* src, char* dest){  
       
    char *rowblockstart = NULL;
    char *copyfrom = NULL;
   
    for (int vbindex=0; vbindex < n_nbpc; vbindex++){
	
	rowblockstart = (char*)(src + vbindex * n_nbpr * block_size_bytes); 
	
	for (int hbindex = 0; hbindex < n_nbpr; hbindex++){
	    for(int i = 0; i < n_nppbv; i++){
		copyfrom = (char*) (rowblockstart +  i * n_nbpr * block_row_size + hbindex * block_row_size );
		strncpy(dest, copyfrom, block_row_size);
		dest += block_row_size;
	    }
	}
    }
    
    return 0;
}

/**********************************************************************
 * NAME: image::to_pixel_sequential
 * --------------------------------------------------------------------
 * PURPOSE: determines internal ntf image storage and chooses which
 * convertion function to call in oreder to make the data pixel
 * sequential
 *********************************************************************/

int image::to_pixel_sequential(){
  
    string ic(m_ic,2);

    if(ic=="NC"){

	compute_data_reordering_variables();

	if (numbands == 1) {
	    
	    imode_1band2ps();
   
	} else if (numbands == 3) {   

	    switch(*m_imode) {
	    case 'B':
		return imode_b2ps();
	    case 'P':
		return imode_p2ps();
	    case 'S':
		return imode_s2ps();
	    case 'R':
		return imode_r2ps();
	    default:
		exit(102);
	    }
	}
	
	return 0;
	
    } else {
	exit(103);
    }
    
}

/**********************************************************************
 * NAME: image::to_block_pixel_sequential
 * --------------------------------------------------------------------
 * PURPOSE: determines internal ntf image storage and chooses which
 * convertion function to call in oreder to make the data block pixel
 * sequential
 *********************************************************************/

int image::to_block_pixel_sequential(char* src){
  
    string ic(m_ic,2);

    if(ic=="NC"){

	compute_data_reordering_variables();

	if (numbands == 1) {
	    
	    imode_ps21band(src);
    
	} else if (numbands == 3) {   

	    switch(*m_imode) {
	    case 'B':
		return imode_ps2b(src);
	    case 'P':
		return imode_ps2p(src);
	    case 'S':
		return imode_ps2s(src);
	    case 'R':
		return imode_ps2r(src);
	    default:
		exit(104);
	    }
	}
	
	return 0;
	
    } else {
	exit(105);
    }
    
}

/**********************************************************************
 * NAME: image::imode_1band2ps
 * --------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image with 1 
 * band only (for details see NITF spec imode field)
 *********************************************************************/

int image::imode_1band2ps() {

    char* temp_data = new char [data_length];   
    strncpy(temp_data, m_data, data_length);   
    blpxseq2pxseq(temp_data, m_data);
    delete temp_data;

    return 0;
}

/**********************************************************************
 * NAME: image::imode_ps21band
 * --------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image with 1 
 * band only (for details see NITF spec imode field)
 *********************************************************************/

int image::imode_ps21band(char* src) {

    pxseq2blpxseq(src, m_data);

    return 0;
}


/**********************************************************************
 * NAME: image::imode_b2ps
 * -------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image stored
 * in band interleaved by block internal storage (for details see NITF 
 * spec imode field)
 *********************************************************************/

int image::imode_b2ps() {
/*
    char* kurets = new char[24];
    strncpy(kurets, "ADMPBENQCFORGJSVHKTXILUY", 24);
    char* temp_data = new char[24];
    char* beginning = temp_data;
    char* read_sofar = kurets;
    char* RED_offset =   NULL;
    char* GREEN_offset = NULL;
    char* BLUE_offset =  NULL;  
    
    for(int block_index = 0; block_index < 2; block_index++) {
	
	RED_offset = read_sofar;
	GREEN_offset = (char*)(read_sofar + 4);
	BLUE_offset =  (char*)(read_sofar + 8);     
	
	for (int i=0; i < 4; i++) {
	    for (int j = 0; j < 1; j++){
		cout<<RED_offset[0]<<endl;
		*temp_data++ = *RED_offset++;
		read_sofar++;
	    }
	    for (int j = 0; j < 1; j++){
		cout<<GREEN_offset[0]<<endl;
		*temp_data++ = *GREEN_offset++;
		read_sofar++;
	    }
	    for (int j = 0; j < 1; j++){
		cout<<BLUE_offset[0]<<endl;
		*temp_data++ = *BLUE_offset++;
		read_sofar++;
	    }
	}
    }  
    temp_data=beginning;
    cout.write(temp_data, 24);
    cout<<endl;
    blpxseq2pxseq(temp_data, kurets);
    cout.write(kurets, 24);
    exit(1234);
    delete temp_data;    
    return 0;
*/

    char* temp_data = new char[data_length];
    char* read_sofar = m_data;
    char* RED_offset =   NULL;
    char* GREEN_offset = NULL;
    char* BLUE_offset =  NULL;  
    
    for(int block_index = 0; block_index < numblocks; block_index++) {
	
	RED_offset = read_sofar;
	GREEN_offset = (char*)(read_sofar + block_band_size);
	BLUE_offset =  (char*)(read_sofar + 2 * block_band_size);     
	
	for (int i=0; i < block_band_size_px; i++) {
	    for (int j = 0; j < px_comp_size; j++){
		*temp_data++ = *RED_offset++;
		read_sofar++;
	    }
	    for (int j = 0; j < px_comp_size; j++){
		*temp_data++ = *GREEN_offset++;
		read_sofar++;
	    }
	    for (int j = 0; j < px_comp_size; j++){
		*temp_data++ = *BLUE_offset++;
		read_sofar++;
	    }
	}
    }  
    
    blpxseq2pxseq(temp_data, m_data);

    delete temp_data;    
    return 0;

}

/**********************************************************************
 * NAME: image::imode_ps2b
 * -------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image stored
 * in band interleaved by block internal storage (for details see NITF 
 * spec imode field)
 *********************************************************************/

int image::imode_ps2b(char* src) {

    char* temp_data = new char[data_length];
    pxseq2blpxseq(src, temp_data);

    char* written_sofar = m_data;
    char* RED_offset =   NULL;
    char* GREEN_offset = NULL;
    char* BLUE_offset =  NULL;  
    
    for(int block_index = 0; block_index < numblocks; block_index++) {
	
	RED_offset = m_data;
	GREEN_offset = (char*)(written_sofar + block_band_size);
	BLUE_offset =  (char*)(written_sofar + 2 * block_band_size);     
	
	for (int i=0; i < block_band_size_px; i++) {
	    for (int j = 0; j < px_comp_size; j++){
		*RED_offset++ = *temp_data++;
		written_sofar++;
	    }
	    for (int j = 0; j < px_comp_size; j++){
		 *GREEN_offset++ = *temp_data++;
		 written_sofar++;  
	    }
	    for (int j = 0; j < px_comp_size; j++){
		*BLUE_offset++ = *temp_data++;
		written_sofar++;
	    }
	}
    }  
    
    delete temp_data;    

    return 0;
}
 
/**********************************************************************
 * NAME:image::imode_p2ps
 * --------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image stored
 * in band interleaved by pixel by block internal storage  (for details 
 * see NITF spec imode field)
 *********************************************************************/

int image::imode_p2ps() {

    char* temp_data = new char [data_length];   
    strncpy(temp_data, m_data, data_length);   
    blpxseq2pxseq(temp_data, m_data);
    delete temp_data;

    return 0;
}

/**********************************************************************
 * NAME:image::imode_ps2p
 * --------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image stored
 * in band interleaved by pixel by block internal storage  (for details 
 * see NITF spec imode field)
 *********************************************************************/

int image::imode_ps2p(char* src){

    pxseq2blpxseq(src, m_data);
    return 0;
}


/**********************************************************************
 * NAME:image::imode_r2ps
 * --------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image stored
 * in band interleaved by row by block internal storage ( for details 
 * see NITF spec imode field)
 *********************************************************************/

int image::imode_r2ps() {

    char* temp_data = new char [data_length];

    char* read_sofar = m_data;
    char* RED_offset =   NULL;
    char* GREEN_offset = NULL;
    char* BLUE_offset =  NULL;  
	     
    int block_row_band_size = block_row_size/numbands;

    for(int block_index = 0; block_index < numblocks ; block_index++) {
	for (int rowinblock = 0; rowinblock < n_nppbv; rowinblock++){

	    RED_offset = read_sofar;
	    GREEN_offset =(char*) (read_sofar + block_row_band_size);
	    BLUE_offset = (char*) (read_sofar+ 2 * block_row_band_size);

	    for (int i = 0; i < n_nppbh; i++){

   		for (int j = 0; j < px_comp_size; j++){
		    *temp_data++ = *RED_offset++;
		    read_sofar++;
		}

		for (int j = 0; j < px_comp_size; j++){
		    *temp_data++ = *GREEN_offset++;
		    read_sofar++;
		}

		for (int j = 0; j < px_comp_size; j++){
		    
		    *temp_data++ = *BLUE_offset++;
		    read_sofar++;
		}
	    }
	}           	        	    
    }  
    
    blpxseq2pxseq(temp_data, m_data);
    delete temp_data;       
    return 0;
}

/**********************************************************************
 * NAME:image::imode_ps2r
 * --------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image stored
 * in band interleaved by row by block internal storage ( for details 
 * see NITF spec imode field)
 *********************************************************************/

int image::imode_ps2r(char* src) {

    char* temp_data = new char [data_length];
    pxseq2blpxseq(src, temp_data);   

    char* written_sofar = m_data;
    char* RED_offset =   NULL;
    char* GREEN_offset = NULL;
    char* BLUE_offset =  NULL;  
	     
    int block_row_band_size = block_row_size/numbands;

    for(int block_index = 0; block_index < numblocks ; block_index++) {
	for (int rowinblock = 0; rowinblock < n_nppbv; rowinblock++){

	    RED_offset = written_sofar;
	    GREEN_offset =(char*) (written_sofar + block_row_band_size);
	    BLUE_offset = (char*) (written_sofar+ 2 * block_row_band_size);

	    for (int i = 0; i < n_nppbh; i++){

   		for (int j = 0; j < px_comp_size; j++){
		    *RED_offset++ = *temp_data++;
		    written_sofar++;
		}

		for (int j = 0; j < px_comp_size; j++){
		    *GREEN_offset++ = *temp_data++;
		    written_sofar++;
		}

		for (int j = 0; j < px_comp_size; j++){
		    *BLUE_offset++ = *temp_data++;
		    written_sofar++;
		}
	    }
	}           	        	    
    }  
    
    delete temp_data;       
    return 0;
}


/**********************************************************************
 * NAME:image::imode_s2ps
 * -------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image stored
 * in band sequential internal storage (for details see NITF spec  
 * imode field)
 *********************************************************************/

int image::imode_s2ps() {
 
    int numpix = n_nrows * n_ncols; 

    char* temp_data = new char [data_length];
        
    char* RED_offset = m_data;
    char* GREEN_offset = (char*)(m_data + image_band_size);
    char* BLUE_offset =  (char*)(m_data + 2 * image_band_size);  
    
    for (int i=0; i < numpix; i++) {
	
	for (int j = 0; j < px_comp_size; j++){
	    *temp_data++ = *RED_offset++;
	}
	
	for (int j = 0; j < px_comp_size; j++){
	    *temp_data++ = *GREEN_offset++;
	}
	
	for (int j = 0; j < px_comp_size; j++){
	    
	    *temp_data++ = *BLUE_offset++;
	}
    }
    
    blpxseq2pxseq(temp_data, m_data);
    delete temp_data;
        
    return 0;
}  

/**********************************************************************
 * NAME:image::imode_ps2s
 * -------------------------------------------------------------------
 * PURPOSE: data convertion fucntion for the data of an image stored
 * in band sequential internal storage (for details see NITF spec  
 * imode field)
 *********************************************************************/

int image::imode_ps2s(char* src) {
   
    char* temp_data = new char[data_length];
    pxseq2blpxseq(src, temp_data);   

    int numpix = n_nrows * n_ncols; 
        
    char* RED_offset = m_data;
    char* GREEN_offset = (char*)(m_data + image_band_size);
    char* BLUE_offset =  (char*)(m_data + 2 * image_band_size);  
    
    for (int i=0; i < numpix; i++) {
	
	for (int j = 0; j < px_comp_size; j++){
	    *RED_offset++ = *temp_data++;
	}
	
	for (int j = 0; j < px_comp_size; j++){
	    *GREEN_offset++ = *temp_data++;
	}
	
	for (int j = 0; j < px_comp_size; j++){
	    *BLUE_offset++ = *temp_data++;
	}
    }
    
    delete temp_data;
        
    return 0;
}

long image::get_size() const
{
   return data_length;    
}

int image::get_width() const
{
   return n_nrows; 
}

int image::get_height() const
{
    return n_ncols;
}

string image::get_irep() const {
    return string(m_irep, 8);
}

string image::get_pvtype() const {
    return string(m_pvtype, 3);
}

int image::get_nbpp_bytes() const {

    return charptrtoint(m_nbpp, 2);
}
