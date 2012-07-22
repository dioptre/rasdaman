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
/
/**
 * SOURCE: ntf.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_NTF
 *
 * COMMENTS:
 *
 * Provides functions to convert data to NTF and back.
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <new>

#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "raslib/parseparams.hh"
#include "conversion/ntf.hh"
#include "conversion/memfs.hh"

// nitf reader library

#include "nitf.h"
#include "image.h"

r_Conv_NTF::r_Conv_NTF( const char *src, const r_Minterval &interv, const r_Type *tp )
throw(r_Error):r_Convert_Memory(src, interv, tp)
{
}

r_Conv_NTF::r_Conv_NTF( const char *src, const r_Minterval &interv, int tp )
throw(r_Error):r_Convert_Memory(src, interv, tp)
{
}

r_Conv_NTF::~r_Conv_NTF( void )
{
}

/***********************************************************
 * NAME: r_Conv_NTF::convertFrom
 * ---------------------------------------------------------
 * PURPOSE: convert from NITF image file data to the
 * corresponding rasdaman type pixel sequential data
 ***********************************************************/

r_convDesc& r_Conv_NTF::convertFrom( const char *options ) throw(r_Error)
{

    char *nitf_file_data = NULL;
    char *nitf_img = NULL;
    long fileSize = 0;
    RasNITF::nitf nitf_file;

    nitf_file_data = (char*) desc.src;

    //READ NITF HEADERS

    nitf_file.read_headers_from(nitf_file_data, int(desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1));

    // MOVE POINTER TO THE IMAGE DATA

    nitf_img = (char*)(desc.src + nitf_file.get_image_offset());
    fileSize = nitf_file.get_image_size();

    // CONVERT ALL DATA TO PIXEL SEQUENTIAL

    nitf_file.image_to_pixel_sequential(0);  // the address of the image is also known internaly so no need to pass the pointer

    // ALLOCATE STORAGE FOR DESC.DEST WITH? THE SIZE OF THE IMAGE

    if ((desc.dest = (char*)mystore.storage_alloc(fileSize)) == NULL)
    {
        RMInit::logOut << "r_Conv_NTF::convertTo(): out of memory!" << endl;
        throw r_Error(MEMMORYALLOCATIONERROR);
    }

    // COPY THE PIXEL SEQUENTIAL DATA TO THE DESC.DEST

    memcpy(desc.dest, nitf_img, fileSize);

    // SET THE INTERVALS

    desc.destInterv = r_Minterval(2);
    desc.destInterv << r_Sinterval(( r_Range)0, (r_Range) ( nitf_file.get_image_width() - 1))
                    << r_Sinterval(( r_Range)0, (r_Range) ( nitf_file.get_image_height() -1));

    // SETR BASE TYPE

    int image_index = 0;

    if(nitf_file.isRGB(image_index))
    {
        desc.baseType = ctype_rgb;
    }
    else if(nitf_file.isGRAY(image_index))
    {
        desc.baseType = ctype_char;
    }
    else if(nitf_file.isBOOL(image_index))
    {
        desc.baseType = ctype_bool;
    }
    else
    {
        RMInit::logOut << "r_Conv_NTF::convertFrom:" << "unsupported NITF file pixel type" << endl;
        throw r_Error(r_Error::r_Error_General);
    }



    /*    switch (nitf_file.get_image_pixel_type()){
        case NITF_BI:
            desc.baseType = ctype_bool;
            break;
        case NITF_GRAY:
            desc.baseType = ctype_char;
            break;
        case NITF_RGB:
            desc.baseType = ctype_rgb;
            break;
        default:
            RMInit::logOut << "r_Conv_NTF::convertFrom:" << "unsupported NITF file pixel type" << endl;
        }
    */
    desc.destType = get_external_type(desc.baseType);

    return desc;

}

/***********************************************************
 * NAME: r_Conv_NTF::convertTo
 * ---------------------------------------------------------
 * PURPOSE: convert from rasdaman data to the
 * corresponding nitf storage type
 ***********************************************************/

r_convDesc& r_Conv_NTF::convertTo( const char *options ) throw(r_Error)
{

    int width=0, height=0;
    int fileSize = 0;

    // DETERMINE WIDTH AND HEIGHT

    width  = (int)(desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1);
    height = (int)(desc.srcInterv[1].high() - desc.srcInterv[1].low() + 1);

    // CALCULATE THE SIZE OF THE IMAGE FILE

    switch (desc.baseType)
    {
    case ctype_bool:
        fileSize = width * height * 1;
        break;
    case ctype_char:
        fileSize = width * height * 1;
        break;
    case ctype_rgb:
        fileSize = width * height * 3;
    default:
        RMInit::logOut << "r_Conv_BMP::convertTo(): Unknown base type!" << endl;
        throw r_Error(r_Error::r_Error_General);
    }

    // ALLOCATE STORAGE FOR THE IMAGE DATA

    if ((desc.dest = (char*)mystore.storage_alloc(fileSize)) == NULL)
    {
        RMInit::logOut << "r_Conv_NTF::convertTo(): out of memory!" << endl;
        throw r_Error(MEMMORYALLOCATIONERROR);
    }

    memcpy(desc.dest, desc.src, fileSize);

    // SET THE SIZE OF THE DATA

    desc.destInterv = r_Minterval(1);
    desc.destInterv << r_Sinterval((r_Range)0, (r_Range)fileSize-1);

    // SET THE TYPE OF TEH DATA

    desc.destType = r_Type::get_any_type("char");

    return desc;
}


r_Convertor* r_Conv_NTF::clone( void ) const
{
    return new r_Conv_NTF(desc.src, desc.srcInterv, desc.baseType);
}

const char * r_Conv_NTF::get_name( void ) const
{
    return format_name_ntf;
}

r_Data_Format r_Conv_NTF::get_data_format( void ) const
{
    return r_NTF;
}
