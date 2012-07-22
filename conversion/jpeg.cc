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
 * SOURCE: jpeg.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_JPEG
 *
 * COMMENTS:
 *
 * Provides functions to convert data to JPEG and back
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <setjmp.h>

extern "C" {
#include "jpeglib.h"
}

#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "conversion/jpeg.hh"
#include "conversion/memfs.hh"



#define JPEG_IO_BUFFER_SIZE 4096


// JPEG-interface in C-namespace (like JPEG lib)
extern "C" {


    typedef struct my_compress_struct
    {
        jpeg_compress_struct pub;
        thandle_t handle;
        JOCTET *buffer;
        int bufferSize;
    } my_compress_struct;

    typedef struct my_decompress_struct
    {
        jpeg_decompress_struct pub;
        thandle_t handle;
        JOCTET *buffer;
        int bufferSize;
    } my_decompress_struct;

    typedef struct my_error_mgr
    {
        jpeg_error_mgr pub;
        jmp_buf setjmp_buffer;
    } my_error_mgr;


// See jpeg library example
    static void my_error_exit(jpeg_common_struct *cptr)
    {
        my_error_mgr *myerr = (my_error_mgr*)(cptr->err);
        (*cptr->err->output_message)(cptr);
        longjmp(myerr->setjmp_buffer, 1);
    }



    /*
     *  Memory IO wrapper functions, rely on memFS.
     */

// Destination manager methods
// cptr is actually a pointer to my_compress_struct
    static void dm_init_destination(jpeg_compress_struct *cptr)
    {
        my_compress_struct *mptr = (my_compress_struct*)cptr;

        if ((mptr->buffer = new JOCTET[JPEG_IO_BUFFER_SIZE]) == NULL)
        {
            RMInit::logOut << "r_Conv_JPEG@dm_init_destination(): out of memory" << endl;
            throw r_Error(MEMMORYALLOCATIONERROR);
        }
        mptr->bufferSize = JPEG_IO_BUFFER_SIZE;
        cptr->dest->next_output_byte = mptr->buffer;
        cptr->dest->free_in_buffer = mptr->bufferSize;
    }


    static boolean dm_empty_output_buffer(jpeg_compress_struct *cptr)
    {
        my_compress_struct *mptr = (my_compress_struct*)cptr;
        boolean retval=FALSE;

        if (memfs_write(mptr->handle, mptr->buffer, mptr->bufferSize) == mptr->bufferSize)
        {
            cptr->dest->next_output_byte = mptr->buffer;
            cptr->dest->free_in_buffer = mptr->bufferSize;
            retval=TRUE;
        }
        return retval;
    }


    static void dm_term_destination(jpeg_compress_struct *cptr)
    {
        my_compress_struct *mptr = (my_compress_struct*)cptr;

        if (cptr->dest->next_output_byte != mptr->buffer)
        {
            memfs_write(mptr->handle, mptr->buffer, (cptr->dest->next_output_byte - mptr->buffer));
        }
        delete [] mptr->buffer;
        mptr->buffer = NULL;
    }


// Source manager methods
// dptr is actually a pointer to my_decompress_struct
    static void sm_init_source(jpeg_decompress_struct *dptr)
    {
        my_decompress_struct *mptr = (my_decompress_struct*)dptr;

        if ((mptr->buffer = new JOCTET[JPEG_IO_BUFFER_SIZE]) == NULL)
        {
            RMInit::logOut << "r_Conv_JPEG@sm_init_source(): out of memory" << endl;
            throw r_Error(MEMMORYALLOCATIONERROR);
        }
        mptr->bufferSize = JPEG_IO_BUFFER_SIZE;
        dptr->src->next_input_byte = mptr->buffer;
        dptr->src->bytes_in_buffer = 0;
    }


// See jdatasrc.c
    static boolean sm_fill_input_buffer(jpeg_decompress_struct *dptr)
    {
        my_decompress_struct *mptr = (my_decompress_struct*)dptr;
        int read_bytes=0;

        if ((read_bytes = memfs_chunk_read(mptr->handle, mptr->buffer, mptr->bufferSize)) != 0)
        {
            dptr->src->bytes_in_buffer = read_bytes;
        }
        else
        {
            mptr->buffer[0] = (JOCTET)0xff;
            mptr->buffer[1] = (JOCTET)JPEG_EOI;
            dptr->src->bytes_in_buffer = 2;
        }
        dptr->src->next_input_byte = mptr->buffer;

        return TRUE;
    }


// See jdatasrc.c
    static void sm_skip_input_data(jpeg_decompress_struct *dptr, long num_bytes)
    {
        my_decompress_struct *mptr = (my_decompress_struct*)dptr;

        if (num_bytes < dptr->src->bytes_in_buffer)
        {
            dptr->src->next_input_byte += num_bytes;
            dptr->src->bytes_in_buffer -= num_bytes;
        }
        else
        {
            int read_bytes=0;

            num_bytes -= dptr->src->bytes_in_buffer;
            dptr->src->next_input_byte = mptr->buffer;
            while (num_bytes >= mptr->bufferSize)
            {
                memfs_chunk_seek(mptr->handle, mptr->bufferSize, SEEK_CUR);
                num_bytes -= mptr->bufferSize;
            }
            read_bytes = memfs_chunk_read(mptr->handle, mptr->buffer, mptr->bufferSize);
            if (read_bytes <= num_bytes)
            {
                mptr->buffer[0] = (JOCTET)0xff;
                mptr->buffer[1] = (JOCTET)JPEG_EOI;
                dptr->src->bytes_in_buffer = 2;
            }
            else
            {
                dptr->src->next_input_byte = mptr->buffer + num_bytes;
                dptr->src->bytes_in_buffer = read_bytes - num_bytes;
            }
        }
    }


    /*static boolean sm_resync_to_restart(jpeg_decompress_struct *dptr, int desired)
    {
      return FALSE;
    }*/


    static void sm_term_source(jpeg_decompress_struct *dptr)
    {
        my_decompress_struct *mptr = (my_decompress_struct*)dptr;

        delete [] mptr->buffer;
        mptr->buffer = NULL;
    }


} // end of C namespace




/*
 *  r_Conv_JPEG class for converting MDD to JPEG and back
 */

void r_Conv_JPEG::initJPEG( void )
{
    quality = 80;

    if (params == NULL)
        params = new r_Parse_Params;

    params->add("quality", &quality, r_Parse_Params::param_type_int);
}


r_Conv_JPEG::r_Conv_JPEG(const char *src, const r_Minterval &interv, int tp) throw(r_Error)
    : r_Convert_Memory(src, interv, tp)
{
    initJPEG();
}


r_Conv_JPEG::r_Conv_JPEG(const char *src, const r_Minterval &interv, const r_Type *tp) throw(r_Error)
    : r_Convert_Memory(src, interv, tp)
{
    initJPEG();
}


r_Conv_JPEG::~r_Conv_JPEG(void)
{
}


r_convDesc &r_Conv_JPEG::convertTo( const char *options) throw(r_Error)
{
    struct jpeg_destination_mgr destMgr;
    struct jpeg_compress_struct *cptr=NULL;
    struct jpeg_error_mgr *jptr=NULL;
    my_error_mgr jerr;
    my_compress_struct cinfo;
    int width=0, height=0, lineAdd=0, pixelAdd=0;
    int i=0, j=0, spp=0;
    J_COLOR_SPACE jcs;
    JSAMPROW row_pointers[1];
    JSAMPLE *row=NULL, *rowPtr=NULL;
    r_Long jpegSize=0;

    row = NULL;
    memset(&cinfo, 0, sizeof(my_compress_struct));
    cinfo.handle = (thandle_t)handle;
    cptr = (struct jpeg_compress_struct*)&cinfo.pub;
    jptr = (struct jpeg_error_mgr*)&jerr.pub;

    cptr->err = jpeg_std_error(jptr);
    jptr->error_exit = my_error_exit;

    params->process(options);

    if (setjmp(jerr.setjmp_buffer))
    {
        if (row != NULL)
        {
            delete [] row;
            row = NULL;
        }
        jpeg_abort_compress(cptr);
        jpeg_destroy_compress(cptr);
        // destination manager destructor might not be called on an abort
        if (cinfo.buffer != NULL)
        {
            delete [] cinfo.buffer;
            cinfo.buffer=NULL;
        }
        RMInit::logOut << "r_Conv_JPEG::convertTo(" << options << "): unable to save the stack" << endl;
        throw r_Error(r_Error::r_Error_General);
    }

    jpeg_create_compress(cptr);
    jpeg_set_quality(cptr, quality, 0);

    memfs_newfile(handle);

    destMgr.init_destination = dm_init_destination;
    destMgr.empty_output_buffer = dm_empty_output_buffer;
    destMgr.term_destination = dm_term_destination;

    cptr->dest = &destMgr;

    width  = (int)(desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1);
    height = (int)(desc.srcInterv[1].high() - desc.srcInterv[1].low() + 1);

    cptr->image_width = (JDIMENSION)width;
    cptr->image_height = (JDIMENSION)height;

    switch (desc.baseType)
    {
    case ctype_bool:
    case ctype_char:
        jcs = JCS_GRAYSCALE;
        spp = 1;
        lineAdd = 1;
        pixelAdd = height;
        break;
    case ctype_rgb:
        jcs = JCS_RGB;
        spp = 3;
        lineAdd = 3;
        pixelAdd = 3*height;
        break;
    default:
        RMInit::logOut << "r_Conv_JPEG::convertTo(" << options  << "): unsupported base type" << endl;
        throw r_Error(r_Error::r_Error_General);
    }

    cptr->input_components = spp;
    cptr->in_color_space = jcs;

    jpeg_set_defaults(cptr);

    jpeg_set_quality(cptr, quality, TRUE);

    jpeg_start_compress(cptr, TRUE);

    if ((row = new JSAMPLE[width * spp]) == NULL)
    {
        RMInit::logOut << "r_Conv_JPEG::convertTo(" << options << "): out of memory" << endl;
        throw r_Error(MEMMORYALLOCATIONERROR);
    }

    row_pointers[0] = row;

    const unsigned char *src=NULL, *srcPtr=NULL;

    src = (const unsigned char*)desc.src;

    for (j=0; j<height; j++, src += lineAdd)
    {
        srcPtr = src;
        rowPtr = row;

        switch (desc.baseType)
        {
        case ctype_bool:
            for (i=0; i<width; i++, srcPtr += pixelAdd)
            {
                *rowPtr++ = (*srcPtr == 0) ? 0 : 0xff;
            }
            break;
        case ctype_char:
            for (i=0; i<width; i++, srcPtr += pixelAdd)
            {
                *rowPtr++ = *srcPtr;
            }
            break;
        case ctype_rgb:
            for (i=0; i<width; i++, srcPtr += pixelAdd)
            {
                *rowPtr++ = srcPtr[0];
                *rowPtr++ = srcPtr[1];
                *rowPtr++ = srcPtr[2];
            }
            break;
        }
        jpeg_write_scanlines(cptr, row_pointers, 1);
    }

    delete [] row;
    row = NULL;

    jpeg_finish_compress(cptr);

    jpeg_destroy_compress(cptr);

    jpegSize = memfs_size(handle);

    if ((desc.dest = (char*)mystore.storage_alloc(jpegSize)) == NULL)
    {
        RMInit::logOut << "r_Conv_JPEG::convertTo(" << options << "): out of memory" << endl;
        throw r_Error(MEMMORYALLOCATIONERROR);
    }
    memfs_seek(handle, 0, SEEK_SET);
    memfs_read(handle, desc.dest, jpegSize);

    desc.destInterv = r_Minterval(1);
    desc.destInterv << r_Sinterval((r_Range)0, (r_Range)jpegSize - 1);

    desc.destType = r_Type::get_any_type("char");

    return desc;
}


r_convDesc &r_Conv_JPEG::convertFrom(const char *options) throw(r_Error)
{
    struct jpeg_source_mgr srcMgr;
    struct jpeg_decompress_struct *dptr=NULL;
    struct jpeg_error_mgr *jptr=NULL;
    my_error_mgr jerr;
    my_decompress_struct cinfo;
    int width=0, height=0, lineAdd=0, pixelAdd=0;
    int i=0, j=0, spp=0;
    J_COLOR_SPACE jcs;
    JSAMPROW row_pointers[1];
    JSAMPLE *row=NULL, *rowPtr=NULL;
    char *dest=NULL, *destPtr=NULL;

    row = NULL;
    desc.dest = NULL;
    memset(&cinfo, 0, sizeof(my_decompress_struct));
    cinfo.handle = (thandle_t)handle;
    dptr = (struct jpeg_decompress_struct*)&cinfo.pub;
    jptr = (struct jpeg_error_mgr*)&jerr.pub;

    dptr->err = jpeg_std_error(jptr);
    jptr->error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer))
    {
        if (row != NULL)
        {
            delete [] row;
            row = NULL;
        }
        if (desc.dest != NULL)
        {
            mystore.storage_free(desc.dest);
            desc.dest = NULL;
        }
        jpeg_abort_decompress(dptr);
        jpeg_destroy_decompress(dptr);
        // Source manager destructor might not be called on an abort
        if (cinfo.buffer != NULL)
        {
            delete [] cinfo.buffer;
            cinfo.buffer=NULL;
        }
        RMInit::logOut << "r_Conv_JPEG::convertFrom(" << options << "): unable to save the stack" << endl;
        throw r_Error(r_Error::r_Error_General);
    }

    jpeg_create_decompress(dptr);

    memfs_chunk_initfs(handle, (char*)desc.src, (r_Long)(desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1));

    srcMgr.init_source = sm_init_source;
    srcMgr.fill_input_buffer = sm_fill_input_buffer;
    srcMgr.skip_input_data = sm_skip_input_data;
    srcMgr.resync_to_restart = NULL; //sm_resync_to_restart;
    srcMgr.term_source = sm_term_source;

    dptr->src = &srcMgr;

    jpeg_read_header(dptr, TRUE);
    width = (int)(dptr->image_width);
    height = (int)(dptr->image_height);

    if (dptr->num_components == 1)
    {
        desc.baseType = ctype_char;
        lineAdd = 1;
        pixelAdd = height;
    }
    else
    {
        desc.baseType = ctype_rgb;
        lineAdd = 3;
        pixelAdd = 3*height;
        dptr->out_color_space = JCS_RGB;
    }

    jpeg_start_decompress(dptr);

    spp = (int)(dptr->output_components);

    row = new JSAMPLE[width * spp];
    desc.dest = (char*)mystore.storage_alloc(width * height * spp);

    if ((row == NULL) || (desc.dest == NULL))
    {
        if (row != NULL)
        {
            delete [] row;
            row = NULL;
        }
        RMInit::logOut << "r_Conv_JPEG::convertFrom(): out of memory" << endl;
        throw r_Error(MEMMORYALLOCATIONERROR);
    }

    row_pointers[0] = row;
    dest = desc.dest;
    for (j=0; j<height; j++, dest += lineAdd)
    {
        jpeg_read_scanlines(dptr, row_pointers, 1);
        destPtr = dest;
        rowPtr = row;
        switch (spp)
        {
        case 1:
            for (i=0; i<width; i++, destPtr += pixelAdd)
            {
                *destPtr = *rowPtr++;
            }
            break;
        case 3:
            for (i=0; i<width; i++, destPtr += pixelAdd)
            {
                destPtr[0] = *rowPtr++;
                destPtr[1] = *rowPtr++;
                destPtr[2] = *rowPtr++;
            }
            break;
        }
    }

    delete [] row;
    row = NULL;

    jpeg_finish_decompress(dptr);

    jpeg_destroy_decompress(dptr);

    if (desc.srcInterv.dimension() == 2)
        // this means it was explicitly specified, so we shouldn't override it
        desc.destInterv = desc.srcInterv;
    else
    {
        desc.destInterv = r_Minterval(2);
        desc.destInterv << r_Sinterval((r_Range) 0, (r_Range) width - 1)
                        << r_Sinterval((r_Range) 0, (r_Range) height - 1);
    }

    desc.destType = get_external_type(desc.baseType);

    return desc;
}



const char *r_Conv_JPEG::get_name( void ) const
{
    return format_name_jpeg;
}


r_Data_Format r_Conv_JPEG::get_data_format( void ) const
{
    return r_JPEG;
}


r_Convertor *r_Conv_JPEG::clone( void ) const
{
    return new r_Conv_JPEG(desc.src, desc.srcInterv, desc.baseType);
}
