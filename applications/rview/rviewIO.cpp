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
 *  PURPOSE:
 *
 *  rView data exchange interface for storing MDD to the filing system
 *  or loading from it. Currently supported operations are:
 *      - Load TIFF file.
 *      - Save wxPixmap as TIFF to file.
 *      - Load VFF file (if RVIEW_USE_VFF supported; use conversion module)
 *
 *  COMMENTS:
 *          None
 */



#ifdef __GNUG__
#pragma implementation
#endif


// changed in wxWindows 2.4.2:
//#include "wx_prec.h"
#include <wx/wxprec.h>


#ifdef __BORLANDC__
#pragma hdrstop
#endif


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif



#include <stdio.h>
#include <stdlib.h>

#include <iostream.h>


#include "wx_pixmap.h"


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/rmdebug.hh"
#include "raslib/basetype.hh"
#include "raslib/parseparams.hh"

#include "rviewIO.hh"
#include "rviewTypes.hh"


#include "tiffio.h"


#ifdef RVIEW_USE_VFF
/*
 *  The use of the conversion module might cause problems, therefore VFF is
 *  optional; if you have problems with it, build rView without VFF support.
 */
#include "conversion/vff.hh"
#endif




r_Parse_Params *rviewIO::dfltParams = NULL;
char *rviewIO::tiffCompStr = NULL;
int rviewIO::tiffCompression = COMPRESSION_DEFLATE;

// Base type structure formats
const char rviewIO::structure_format_mono[] = "marray <bool, [0:%d,0:%d]>";
const char rviewIO::structure_format_grey[] = "marray <char, [0:%d,0:%d]>";
const char rviewIO::structure_format_rgb[] = "marray <struct { char red, char green, char blue }, [0:%d,0:%d]>";

const char rviewIO::structure_format_cube8[] = "marray <char, [%d:%d,%d:%d,%d:%d]>";
const char rviewIO::structure_format_cube16[] = "marray <ushort, [%d:%d,%d:%d,%d:%d]>";
const char rviewIO::structure_format_cube32[] = "marray <uint, [%d:%d,%d:%d,%d:%d]>";

// parameters
const char rviewIO::param_KeyTiffComp[] = "comptype";
const char rviewIO::param_TiffCompNone[] = "none";
const char rviewIO::param_TiffCompPack[] = "packbits";
const char rviewIO::param_TiffCompLZW[] = "lzw";
const char rviewIO::param_TiffCompZLib[] = "deflate";
const char rviewIO::param_TiffCompJPEG[] = "jpeg";



rviewIO::rviewIO(void)
{
}


rviewIO::~rviewIO(void)
{
}


void rviewIO::ensureParams(void)
{
    if (dfltParams == NULL)
    {
        tiffCompStr = NULL;
        tiffCompression = COMPRESSION_DEFLATE;

        dfltParams = new r_Parse_Params;
        dfltParams->add(param_KeyTiffComp, &tiffCompStr, r_Parse_Params::param_type_string);
    }
}


void rviewIO::processParams(const char *params)
{
    ensureParams();
    dfltParams->process(params);
    tiffCompression = COMPRESSION_DEFLATE;
    if (tiffCompStr != NULL)
    {
        if (strcasecmp(tiffCompStr, param_TiffCompNone) == 0)
            tiffCompression = COMPRESSION_NONE;
        else if (strcasecmp(tiffCompStr, param_TiffCompPack) == 0)
            tiffCompression = COMPRESSION_PACKBITS;
        else if (strcasecmp(tiffCompStr, param_TiffCompLZW) == 0)
            tiffCompression = COMPRESSION_LZW;
        else if (strcasecmp(tiffCompStr, param_TiffCompZLib) == 0)
            tiffCompression = COMPRESSION_DEFLATE;
        else if (strcasecmp(tiffCompStr, param_TiffCompJPEG) == 0)
            tiffCompression = COMPRESSION_JPEG;
    }
}


void rviewIO::terminate(void)
{
    if (dfltParams != NULL)
    {
        delete dfltParams;
        dfltParams = NULL;
    }

    if (tiffCompStr != NULL)
    {
        delete [] tiffCompStr;
        tiffCompStr = NULL;
    }
}


const char *rviewIO::getExtension(const char *filename)
{
    const char *b, *ext;

    b = filename;
    ext = NULL;
    while (*b != '\0')
    {
        if (*b == '.')
            ext = b+1;
        b++;
    }
    return ext;
}

int rviewIO::isTIFF(const char *filename)
{
    const char *ext = getExtension(filename);

    // no extension ==> unrecognized
    if (ext != NULL)
    {
        if ((strcasecmp(ext, "tif") == 0) || (strcasecmp(ext, "tiff") == 0))
            return 1;
    }
    return 0;
}


int rviewIO::loadTIFF(const char *filename, r_Ref<r_GMarray> &mddObj, const char *params)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewIO", "loadTIFF()");

    TIFF* tif;
    uint32 width, height, x, y;
    uint16 bps, spp, planarc, photom;
    int depth, stepx, stepy;
    r_Minterval interv(2);
    r_Point prun(2);
    tdata_t buffer;
    unsigned char *b;
    char structure[STRINGSIZE]="";

    if ((tif = TIFFOpen(filename, "r")) == NULL)
        return RVIEW_IO_NOTFOUND;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarc);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photom);

    if ((width < 1) || (height < 1)) return 0;

    RMDBGMIDDLE(3, RMDebug::module_applications, "rviewIO", "loadTIFF() Width " << width << ", height " << height << ", bps " << bps << ", spp " << spp << ", planarconfig " << planarc);

    depth = bps * spp;

    interv << r_Sinterval(r_Range(0), r_Range(width-1)) << r_Sinterval(r_Range(0), r_Range(height-1));

    if ((buffer = _TIFFmalloc(TIFFScanlineSize(tif))) == NULL)
    {
        TIFFClose(tif);
        return RVIEW_IO_MEMORY;
    }

    switch (depth)
    {
    case 1:
    {
        r_Ref<r_Marray<r_Boolean> > mddPtr = new r_Marray<r_Boolean>(interv);
        r_Boolean *imgLine, *imgPtr;
        r_Boolean min, max;
        char val=0;
        int mask;

        // Adjust the photometric interpretation. Later on 0 will be interpreted as black.
        if (photom == PHOTOMETRIC_MINISWHITE)
        {
            min = 1;
            max = 0;
        }
        else
        {
            min = 0;
            max = 1;
        }
        stepx = &((*mddPtr)[r_Point(1,0)]) - &((*mddPtr)[r_Point(0,0)]);
        stepy = &((*mddPtr)[r_Point(0,1)]) - &((*mddPtr)[r_Point(0,0)]);
        imgLine = (r_Boolean*)(mddPtr->get_array());
        for (y=0; y<height; y++, imgLine += stepy)
        {
            if (planarc == PLANARCONFIG_CONTIG)
            {
                TIFFReadScanline(tif, buffer, y);
            }
#ifdef FILLORDER_MSB2LSB
            mask = 0;
#else
            mask = 0x100;
#endif
            imgPtr = imgLine;
            b = (unsigned char*)buffer;
            for (x=0; x<width; x++, imgPtr += stepx)
            {
#ifdef FILLORDER_MSB2LSB
                if (mask == 0)
                {
                    val = *b++;
                    mask = 0x80;
                }
                *imgPtr = ((val & mask) == 0) ? min : max;
                mask >>= 1;
#else
                if (mask == 0x100)
                {
                    val = *b++;
                    mask = 1;
                }
                *imgPtr = ((val & mask) == 0) ? min : max;
                mask <<= 1;
#endif
            }
        }
        mddPtr->set_type_by_name(rviewTypeNames[rbt_bool][2-1]);
        // Init base structure (hack, but better than leaving it undefined)
        sprintf(structure, structure_format_mono, width-1, height-1);
        mddObj = (r_Ref<r_GMarray>)mddPtr;
    }
    break;
    case 8:
    {
        int paletteIsGrey = 0;

        uint16 *reds, *greens, *blues;

        TIFFGetField(tif, TIFFTAG_COLORMAP, &reds, &greens, &blues);
        if (photom == PHOTOMETRIC_PALETTE)
        {
            for (x=0; x<256; x++)
            {
                if ((reds[x] != greens[x]) || (greens[x] != blues[x])) break;
            }
            if (x >= 256) paletteIsGrey = 1;
        }
        if ((photom == PHOTOMETRIC_PALETTE) && (paletteIsGrey == 0))
        {
            r_Ref<r_Marray<RGBPixel> > mddPtr = new r_Marray<RGBPixel>(interv);
            RGBPixel *imgLine, *imgPtr;

            stepx = &((*mddPtr)[r_Point(1,0)]) - &((*mddPtr)[r_Point(0,0)]);
            stepy = &((*mddPtr)[r_Point(0,1)]) - &((*mddPtr)[r_Point(0,0)]);
            imgLine = (RGBPixel*)(mddPtr->get_array());
            for (y=0; y<height; y++, imgLine += stepy)
            {
                if (planarc == PLANARCONFIG_CONTIG)
                {
                    TIFFReadScanline(tif, buffer, y);
                }
                imgPtr = imgLine;
                b = (unsigned char*)buffer;
                for (x=0; x<width; x++, imgPtr += stepx, b++)
                {
                    imgPtr->red = (reds[*b] >> 8);
                    imgPtr->green = (greens[*b] >> 8);
                    imgPtr->blue = (blues[*b] >> 8);
                }
            }
            mddPtr->set_type_by_name(rviewTypeNames[rbt_rgb][2-1]);
            sprintf(structure, structure_format_rgb, width-1, height-1);
            mddObj = (r_Ref<r_GMarray>)mddPtr;
        }
        else
        {
            r_Ref<r_Marray<r_Char> > mddPtr = new r_Marray<r_Char>(interv);
            r_Char *imgLine, *imgPtr;
            r_Char transTab[256];

            if (paletteIsGrey != 0)
            {
                for (x=0; x<256; x++) transTab[x] = (reds[x] >> 8);
            }
            else
            {
                // Build a translation table depending on the photometric interpretation
                if (photom == PHOTOMETRIC_MINISWHITE)
                {
                    for (x=0; x<256; x++) transTab[x] = 255-x;
                }
                else    // default to minisblack
                {
                    for (x=0; x<256; x++) transTab[x] = x;
                }
            }

            stepx = &((*mddPtr)[r_Point(1,0)]) - &((*mddPtr)[r_Point(0,0)]);
            stepy = &((*mddPtr)[r_Point(0,1)]) - &((*mddPtr)[r_Point(0,0)]);
            imgLine = (r_Char*)(mddPtr->get_array());
            for (y=0; y<height; y++, imgLine += stepy)
            {
                if (planarc == PLANARCONFIG_CONTIG)
                {
                    TIFFReadScanline(tif, buffer, y);
                }
                imgPtr = imgLine;
                b = (unsigned char*)buffer;
                for (x=0; x<width; x++, imgPtr += stepx, b++)
                {
                    *imgPtr = transTab[*b];
                }
            }
            mddPtr->set_type_by_name(rviewTypeNames[rbt_char][2-1]);
            sprintf(structure, structure_format_grey, width-1, height-1);
            mddObj = (r_Ref<r_GMarray>)mddPtr;
        }
    }
    break;
    case 24:
    {
        r_Ref<r_Marray<RGBPixel> > mddPtr = new r_Marray<RGBPixel>(interv);
        RGBPixel *imgLine, *imgPtr;

        stepx = &((*mddPtr)[r_Point(1,0)]) - &((*mddPtr)[r_Point(0,0)]);
        stepy = &((*mddPtr)[r_Point(0,1)]) - &((*mddPtr)[r_Point(0,0)]);
        imgLine = (RGBPixel*)(mddPtr->get_array());
        //cout << "stepx=" << stepx << ", stepy=" << stepy << endl;
        for (y=0; y<height; y++, imgLine += stepy)
        {
            if (planarc == PLANARCONFIG_CONTIG)
            {
                TIFFReadScanline(tif, buffer, y);
            }
            imgPtr = imgLine;
            b = (unsigned char*)buffer;
            for (x=0; x<width; x++, imgPtr += stepx, b+=3)
            {
                imgPtr->red = b[0];
                imgPtr->green = b[1];
                imgPtr->blue = b[2];
            }
        }
        mddPtr->set_type_by_name(rviewTypeNames[rbt_rgb][2-1]);
        sprintf(structure, structure_format_rgb, width-1, height-1);
        mddObj = (r_Ref<r_GMarray>)mddPtr;
    }
    break;
    default:
        return RVIEW_IO_FORMAT;
    }

    if (structure[0] != 0)
        mddObj->set_type_structure(structure);

    _TIFFfree(buffer);

    TIFFClose(tif);

    RMDBGEXIT(3, RMDebug::module_applications, "rviewImage", "~rviewImage() Created MDD object of domain " << mddObj->spatial_domain());

    return RVIEW_IO_OK;
}


int rviewIO::saveTIFF(const char *filename, r_Ref<r_GMarray> &mddPtr, const char *params)
{
    cout << "rviewIO::saveTIFF() not implemented" << endl;
    return RVIEW_IO_UNSUPP;
}



/*
 *  Save a wxPixmap as a TIFF image
 */

int rviewIO::PixmapToTIFF(wxPixmap *pixmap, const char *filename, const char *params)
{
    TIFF *tif;
    int depth, pitch;
    uint32 width, height;
    uint8 *srcLine;
    uint32 i;

    depth = pixmap->getDepth();
    pitch = pixmap->getPitch();
    width = (uint32)(pixmap->getWidth());
    height = (uint32)(pixmap->getHeight());

    if ((tif = TIFFOpen(filename, "w")) == NULL)
    {
        char buffer[STRINGSIZE];

        sprintf(buffer, "%s %s.\n", lman->lookup("errorFileOpen"), filename);
        rviewErrorbox eb(buffer);
        eb.activate();
        return -1;
    }

    processParams(params);

    TIFFSetField(tif, TIFFTAG_ARTIST, "rView");
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, tiffCompression);
    TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, 90.0);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, 90.0);

    srcLine = (uint8*)(pixmap->getData());

    switch (depth)
    {
    case 1:
    {
        wxColour *pal;

        pal = pixmap->getPalette();
        if (pal == NULL)
        {
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        }
        else
        {
            if ((int)(pal[0].Red()+pal[0].Green()+pal[0].Blue()) < (int)(pal[1].Red()+pal[1].Green()+pal[1].Blue()))
            {
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            }
            else
            {
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
            }
        }
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        for (i=0; i<height; i++, srcLine += pitch)
        {
            if (TIFFWriteScanline(tif, (tdata_t)srcLine, i, 0) < 0) break;
        }
    }
    break;
    case 8:
    {
        wxColour *pal;

        pal = pixmap->getPalette();
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        if (pal == NULL)
        {
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        }
        else
        {
            uint16 reds[256], greens[256], blues[256];

            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE);
            for (i=0; i<256; i++)
            {
                reds[i] = ((pal[i].Red() << 16) - 1) / 255;
                greens[i] = ((pal[i].Green() << 16) - 1) / 255;
                blues[i] = ((pal[i].Blue() << 16) -1) / 255;
            }
            TIFFSetField(tif, TIFFTAG_COLORMAP, reds, greens, blues);
        }
        for (i=0; i<height; i++, srcLine += pitch)
        {
            if (TIFFWriteScanline(tif, (tdata_t)srcLine, i, 0) < 0) break;
        }
    }
    break;
    case 15:
    {
        uint32 j;
        uint8 *destLine, *destPtr;
        uint16 *srcPtr;

        destLine = new uint8[3*width];
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        for (i=0; i<height; i++, srcLine += pitch)
        {
            destPtr = destLine;
            srcPtr = (uint16*)srcLine;
            for (j=0; j<width; j++, srcPtr++, destPtr+=3)
            {
                destPtr[0] = (*srcPtr & 0x1f) << 3;
                destPtr[1] = (*srcPtr & 0x3e0) >> 2;
                destPtr[2] = (*srcPtr & 0x7c00) >> 7;
            }
            if (TIFFWriteScanline(tif, (tdata_t)destLine, i, 0) < 0) break;
        }
        delete [] destLine;
    }
    break;
    case 24:
    {
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        for (i=0; i<height; i++, srcLine += pitch)
        {
            if (TIFFWriteScanline(tif, (tdata_t)srcLine, i, 0) < 0) break;
        }
    }
    break;
    case 32:
    {
        uint32 j;
        uint8 *destLine, *destPtr;
        uint32 *srcPtr;

        destLine = new uint8[3*width];
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        for (i=0; i<height; i++, srcLine += pitch)
        {
            destPtr = destLine;
            srcPtr = (uint32*)srcLine;
            for (j=0; j<width; j++, srcPtr++, destPtr+=3)
            {
                destPtr[0] = (*srcPtr & 0xff);
                destPtr[1] = (*srcPtr >> 8) & 0xff;
                destPtr[2] = (*srcPtr >> 16) & 0xff;
            }
            if (TIFFWriteScanline(tif, (tdata_t)destLine, i, 0) < 0) break;
        }
        delete [] destLine;
    }
    break;
    default:
        cerr << "rviewIO::PixmapToTIFF(): Unsupported pixmap depth (" << depth << ')' << endl;
        TIFFClose(tif);
        break;
    }

    TIFFClose(tif);

    if (i < height)
    {
        char buffer[STRINGSIZE];

        sprintf(buffer, "%s %s.\n", lman->lookup("errorFileWrite"), filename);
        rviewErrorbox eb(buffer);
        eb.activate();
        return -1;
    }

    return 0;
}


int rviewIO::isVFF(const char *filename)
{
    const char *ext = getExtension(filename);

    if (ext != NULL)
    {
        if (strcasecmp(ext, "vff") == 0)
            return 1;
    }
    return 0;
}


int rviewIO::loadVFF(const char *filename, r_Ref<r_GMarray> &mddPtr, const char *params)
{
#ifdef RVIEW_USE_VFF
    FILE *fp;

    if ((fp = fopen(filename, "rb")) != NULL)
    {
        unsigned long vffSize;
        char *vffData;

        fseek(fp, 0, SEEK_END);
        vffSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        vffData = new char[vffSize];
        fread(vffData, 1, vffSize, fp);
        fclose(fp);

        r_Minterval dom(1);
        dom << r_Sinterval((r_Range)0, (r_Range)vffSize-1);

        r_Conv_VFF vff(vffData, dom, r_Convertor::ctype_char);
        r_Storage_Man_CPP sman;
        vff.set_storage_handler(sman);
        try
        {
            r_convDesc desc = vff.convertFrom(params);
            delete [] vffData;
            if (desc.destInterv.dimension() == 3)
            {
                const char *fmtString = NULL;
                const char *nameString = NULL;

                // Hmm... we seem to lack a set_type_schema() method...
                switch (desc.destType->type_id())
                {
                case r_Type::BOOL:
                case r_Type::CHAR:
                case r_Type::OCTET:
                    fmtString = structure_format_cube8;
                    nameString = rviewTypeNames[rbt_char][2];
                    break;
                case r_Type::SHORT:
                case r_Type::USHORT:
                    fmtString = structure_format_cube16;
                    nameString = rviewTypeNames[rbt_ushort][2];
                    break;
                case r_Type::LONG:
                case r_Type::ULONG:
                    fmtString = structure_format_cube32;
                    nameString = rviewTypeNames[rbt_ulong][2];
                    break;
                default:
                    break;
                }
                if (fmtString != NULL)
                {
                    char fmtBuffer[STRINGSIZE];

                    sprintf(fmtBuffer, fmtString, desc.destInterv[0].low(), desc.destInterv[0].high(), desc.destInterv[1].low(), desc.destInterv[1].high(), desc.destInterv[2].low(), desc.destInterv[2].high());
                    mddPtr = new r_GMarray;
                    mddPtr->set_spatial_domain(desc.destInterv);
                    mddPtr->set_array(desc.dest);
                    mddPtr->set_type_structure(fmtBuffer);
                    mddPtr->set_type_by_name(nameString);
                    mddPtr->set_type_length(((const r_Base_Type*)desc.destType)->size());
                    mddPtr->set_current_format(r_Array);
                    mddPtr->set_array_size(mddPtr->spatial_domain().cell_count() * mddPtr->get_type_length());
                    //cout << "LOADED VFF " << mddPtr->spatial_domain() << ", " << mddPtr->get_type_structure() << ", " << mddPtr->get_type_name() << endl;
                    delete desc.destType;
                    return RVIEW_IO_OK;
                }
            }
            delete desc.destType;
            delete [] desc.dest;
            return RVIEW_IO_FORMAT;
        }
        catch (r_Error &err)
        {
            cerr << "rviewIO::loadVFF(): " << err.what() << endl;
            delete [] vffData;
            return RVIEW_IO_FORMAT;
        }
    }
    return RVIEW_IO_NOTFOUND;
#else
    return RVIEW_IO_UNSUPP;
#endif
}


int rviewIO::saveVFF(const char *filename, r_Ref<r_GMarray> &mddPtr, const char *params)
{
    cout << "rviewIO::saveVFF() not implemented" << endl;
    return RVIEW_IO_UNSUPP;
}
