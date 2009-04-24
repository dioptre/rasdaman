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

#include "text.h"
#include "nitf.h"
#include "utilities.h"

using namespace std;
using namespace RasNITF;
	
RasNITF::text::text()
{
    m_text_data = NULL;
    m_txshd = NULL;
}

RasNITF::text::~text()
{
    if(m_text_data != NULL)
	delete m_text_data;
    m_text_data = NULL;
    
    if(m_txshd != NULL)
	delete m_txshd;
    m_txshd = NULL;

}
		
int RasNITF::text::read_file(istream &hNITF, long th_length, long text_length)
{
    int charsread = 0;
    long starting_position = 0;
	
    header_length=th_length;	
    starting_position = hNITF.tellg();
    charsread += read_verify2(hNITF, m_te, 2 + 7 + 3 + 14 + 80 + 1 + 2 + 11 + 2 + 20 + 2 + 8+ 4+ 1+ 8+ 43 + 1 + 40 + 1 + 8+ 15+ 1+ 3);
	
    // read txshdl

    charsread += read_verify2(hNITF, m_txshdl, 5);
    n_txshdl = charptrtolong(m_txshdl, 5);
    
    if (n_txshdl > 0) {
	n_txshdl -= 3;
	charsread += read_verify2(hNITF, m_txsofl, 3);
	m_txshd = new char[n_txshdl];
	if(m_txshd == NULL) cerr<<"ERROR: could not allocate memory";
	charsread += read_verify2(hNITF, m_txshd, n_txshdl);
	n_txshdl += 3;
	
    } 

    // check if we are where we should be
    if((long)(hNITF.tellg()) != starting_position + th_length) exit(2);
    
    // store the text data
    
    m_text_data = new char[text_length];
    if(m_text_data == NULL) cerr<<"ERROR: could not allocate memory";
    charsread += read_verify2(hNITF, m_text_data, text_length);
    data_length = text_length;
    
    return charsread;
} 

int RasNITF::text::write_file(ofstream &fNITF)
{

    fNITF.write(m_te, 2); 
    fNITF.write(m_textid, 7);
    fNITF.write(m_txtalvl, 3);	
    fNITF.write(m_txtdt, 14);
    fNITF.write(m_txtitl, 80);
    fNITF.write(m_tsclas, 1);
    fNITF.write(m_tsclsy, 2);
    fNITF.write(m_tscode, 11);
    fNITF.write(m_tsctlh, 2);
    fNITF.write(m_tsrel, 20);
    fNITF.write(m_tsdctp, 2);
    fNITF.write(m_tsdcdt, 8);	
    fNITF.write(m_tsdcxm, 4);
    fNITF.write(m_tsdg, 1);
    fNITF.write(m_tsdgdt, 8);	
    fNITF.write(m_tscltx, 43);
    fNITF.write(m_tscatp, 1);
    fNITF.write(m_tscaut, 40);
    fNITF.write(m_tscrsn, 1);
    fNITF.write(m_tssrdt, 8);	
    fNITF.write(m_tsctln, 15);
    fNITF.write(m_encryp, 1);
    fNITF.write(m_txtfmt, 3);
    
    fNITF.write(m_txshdl, 5);
    
    if (n_txshdl > 0) {
	fNITF.write(m_txsofl, 3);
	fNITF.write(m_txshd, n_txshdl - 3);
    }
    
    if( m_text_data != NULL) { 
	fNITF.write( m_text_data, data_length);
    }
    
    //TODO:
    return 0;
    
}

string RasNITF::text::get_lt() const {
   return text_dl;
}

string RasNITF::text::get_ltsh() const {
   return text_hl;
}
