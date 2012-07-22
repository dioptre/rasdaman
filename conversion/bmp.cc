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
 * SOURCE: bmp.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_BMP
 *
 * COMMENTS:
 *       Provides functions to convert data to BMP and back.
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
#include "conversion/bmp.hh"
#include "conversion/memfs.hh"


// Some Windows-structs, redefined for platform-independent use.
typedef struct
{
    unsigned short type;
    r_ULong size;
    unsigned short res0;
    unsigned short res1;
    r_ULong offset;
} bitmap_file_header_t;

typedef struct
{
    r_ULong size;
    r_Long width;
    r_Long height;
    unsigned short planes;
    unsigned short bitCount;
    r_ULong compression;
    r_ULong sizeImage;
    r_Long xpels;
    r_Long ypels;
    r_ULong clrUsed;
    r_ULong clrImportant;
} bitmap_info_header_t;

typedef struct
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char null;
} rgb_quad_t;

// Identifier of BMP data (first two bytes)
const unsigned short BMP_IDENTIFIER=0x4d42;
// Compression types (correspond to BI_RGB, BI_RLE8, BI_RLE4)
const int COMPRESS_NONE=0;
const int COMPRESS_RLE8=1;
const int COMPRESS_RLE4=2;
// Size of BITMAPFILEHEADER (the Windows struct)
const int BMPFILEHEADERSIZE=sizeof(bitmap_file_header_t) - sizeof(BMP_IDENTIFIER);
// Size of BITMAPINFOHEADER (the Windows struct)
const int BMPINFOHEADERSIZE=sizeof(bitmap_info_header_t);
// Total header size
const int BMPHEADERSIZE=(BMPFILEHEADERSIZE + BMPINFOHEADERSIZE);

// Shortcuts for reading and writing short and long types from/to little endian bytestreams
#define READ_LE_SHORT(p,s) \
  s = p[0] | (p[1] << 8); p += 2;
#define READ_LE_LONG(p,l) \
  l = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24); p += 4;
#define WRITE_LE_SHORT(p,s) \
  p[0] = s & 0xff; p[1] = (s >> 8) & 0xff; p += 2;
#define WRITE_LE_LONG(p,l) \
  p[0] = l & 0xff; p[1] = (l >> 8) & 0xff; p[2] = (l >> 16) & 0xff; p[3] = (l >> 24) & 0xff; p += 4;



void r_Conv_BMP::initBMP( void )
{
    memFS = NULL;

    compress = 1;

    if (params == NULL)
        params = new r_Parse_Params;

    params->add("compress", &compress, r_Parse_Params::param_type_int);
}


r_Conv_BMP::r_Conv_BMP(const char *src, const r_Minterval &interv, int tp) throw(r_Error)
    : r_Convertor(src, interv, tp)
{
    initBMP();
}


r_Conv_BMP::r_Conv_BMP(const char *src, const r_Minterval &interv, const r_Type *tp) throw(r_Error)
    : r_Convertor(src, interv, tp)
{
    initBMP();
}


r_Conv_BMP::~r_Conv_BMP(void)
{
    if (memFS != NULL)
    {
        memfs_killfs((void*)memFS);
        delete memFS;
        memFS=NULL;
    }
}


unsigned char *r_Conv_BMP::flushLiterals(int numLit, int pixelAdd, unsigned char *dest, const unsigned char *lastLit, const unsigned char *mapColours)
{
    unsigned char *destPtr = dest;

    while (numLit != 0)
    {
        // Must code literal runs of less than 3 bytes as runs
        if (numLit < 3)
        {
            while (numLit > 0)
            {
                *destPtr++ = 1;
                *destPtr++ = mapColours[*lastLit];
                lastLit += pixelAdd;
                numLit--;
            }
        }
        else
        {
            int litLength=0;
            r_Ptr align=0;

            litLength = numLit;
            if (litLength > 255) litLength = 255;
            *destPtr++ = 0x00;
            *destPtr++ = (unsigned char)litLength;
            numLit -= litLength;
            while (litLength > 0)
            {
                *destPtr++ = mapColours[*lastLit];
                lastLit += pixelAdd;
                litLength--;
            }
            align = (r_Ptr)destPtr;
            if ((align & 1) != 0) *destPtr++ = 0;
        }
    }
    return destPtr;
}


r_convDesc &r_Conv_BMP::convertTo( const char *options) throw(r_Error)
{
    void *handle=NULL;
    bitmap_info_header_t ihead;
    rgb_quad_t *palette = NULL;
    int i=0, j=0;
    int paletteSize=0, pixelSize=0;
    int destPitch=0, pixelAdd=0, lineAdd=0;
    int width=0, height=0;
    r_ULong offset=0;
    r_ULong fileSize=0;
    unsigned char *dest=NULL, *destPtr=NULL;
    const unsigned char *srcLine=NULL, *srcPtr=NULL;
    unsigned char bmpHeaders[BMPHEADERSIZE];
    unsigned char mapColours[256];

    memFS = new memFSContext;
    handle = (void*)memFS;
    if ((memFS == NULL) || (memfs_initfs(handle) < 0))
    {
        RMInit::logOut << "r_Conv_BMP::convertTo(): couldn't initialize memfs!" << endl;
        throw r_Error(MEMMORYALLOCATIONERROR);
    }
    memfs_newfile(handle);

    width  = (int)(desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1);
    height = (int)(desc.srcInterv[1].high() - desc.srcInterv[1].low() + 1);

    params->process(options);

    ihead.size = BMPINFOHEADERSIZE;
    ihead.width = (r_Long)width;
    ihead.height = (r_Long)height;
    ihead.planes = 1;
    ihead.compression = COMPRESS_NONE;
    ihead.xpels = 0;
    ihead.ypels = 0;

    switch (desc.baseType)
    {
    case ctype_bool:
        ihead.bitCount = 1;
        destPitch = ((width + 31) & ~31) >> 3;
        paletteSize = 2;
        pixelSize = 1;
        ihead.clrUsed = 2;
        ihead.clrImportant = 2;
        palette = new rgb_quad_t[2];
        palette[0].red = 0x00;
        palette[0].green = 0x00;
        palette[0].blue = 0x00;
        palette[0].null = 0x00;
        palette[1].red = 0xff;
        palette[1].green = 0xff;
        palette[1].blue = 0xff;
        palette[1].null = 0x00;
        break;
    case ctype_char:
    {
        ihead.bitCount = 8;
        destPitch = ((width + 3) & ~3);
        pixelSize = 1;
        if (compress != 0) ihead.compression = COMPRESS_RLE8;
        // Determine which colours actually appear in the image
        memset(mapColours, 0, 256);
        srcLine = (const unsigned char*)desc.src;
        for (i=0; i<width*height; i++) mapColours[*srcLine++] = 1;
        // Count distinct colours
        paletteSize = 0;
        for (i=0; i<256; i++) if (mapColours[i] != 0) paletteSize++;
        // Create palette
        RMDBGONCE(3, RMDebug::module_conversion, "r_Conv_BMP", "convertTo(): number of distinct colours: " << paletteSize )
        palette = new rgb_quad_t[paletteSize];
        paletteSize = 0;
        for (i=0; i<256; i++)
        {
            if (mapColours[i] != 0)
            {
                palette[paletteSize].red = (unsigned char)i;
                palette[paletteSize].green = (unsigned char)i;
                palette[paletteSize].blue = (unsigned char)i;
                palette[paletteSize].null = 0;
                paletteSize++;
            }
        }
        // ``compress'' colourmap
        paletteSize = 0;
        for (i=0; i<256; i++)
        {
            if (mapColours[i] != 0) mapColours[i] = paletteSize++;
        }
        ihead.clrUsed = paletteSize;
        ihead.clrImportant = 0;  // for simplicity's sake
        break;
    }
    case ctype_rgb:
        ihead.bitCount = 24;
        destPitch = ((width * 3 + 3) & ~3);
        paletteSize = 0;
        pixelSize = 3;
        ihead.clrUsed = 0;
        ihead.clrImportant = 0;
        break;
    default:
        RMInit::logOut << "r_Conv_BMP::convertTo(): Unknown base type!" << endl;
        throw r_Error(r_Error::r_Error_General);
    }
    lineAdd = pixelSize;
    pixelAdd = height * pixelSize;

    // Write dummy header, to be replaced later
    memset(bmpHeaders, 0, BMPHEADERSIZE);
    memfs_write(handle, bmpHeaders, BMPHEADERSIZE);
    if (paletteSize != 0)
    {
        memfs_write(handle, palette, paletteSize * sizeof(rgb_quad_t));
        delete [] palette;
        palette = NULL;
    }

    srcLine = (const unsigned char*)(desc.src + (height-1) * pixelSize);

    if (ihead.compression == COMPRESS_NONE)
    {
        if ((dest = new unsigned char[destPitch]) == NULL)
        {
            RMInit::logOut << "r_Conv_BMP::convertTo(): out of memory!" << endl;
            throw r_Error(MEMMORYALLOCATIONERROR);
        }
        for (j=0; j<height; j++, srcLine -= lineAdd)
        {
            srcPtr = srcLine;
            destPtr = dest;
            switch (desc.baseType)
            {
            case ctype_bool:
            {
                int mask=0;
                unsigned char val=0;

                mask = 0x80;
                val = 0;
                for (i=0; i<width; i++, srcPtr += pixelAdd)
                {
                    if (*srcPtr != 0) val |= mask;
                    mask >>= 1;
                    if (mask == 0)
                    {
                        *destPtr++ = val;
                        mask = 0x80;
                        val = 0;
                    }
                }
                if (mask != 0x80) *destPtr++ = val;
            }
            break;
            case ctype_char:
                for (i=0; i<width; i++, srcPtr += pixelAdd)
                {
                    *destPtr++ = mapColours[*srcPtr];
                }
                break;
            case ctype_rgb:
                for (i=0; i<width; i++, srcPtr += pixelAdd)
                {
                    *destPtr++ = srcPtr[2];
                    *destPtr++ = srcPtr[1];
                    *destPtr++ = srcPtr[0];
                }
                break;
            }
            // Align to 32bit-boundary
            for (i = (4 - (r_Ptr)destPtr) & 3; i>0; i--) *destPtr++ = 0;
            memfs_write(handle, dest, destPitch);
        }
        delete [] dest;
        dest = NULL;
    }
    else  // implies RLE 8
    {
        if ((dest = new unsigned char[2*destPitch]) == NULL)
        {
            RMInit::logOut << "r_Conv_BMP::convertTo(): out of memory!" << endl;
            throw r_Error(MEMMORYALLOCATIONERROR);
        }

        for (j=0; j<height; j++, srcLine -= lineAdd)
        {
            const unsigned char *lastLit=NULL, *tryRun=NULL;
            int k=0, numLit=0;

            srcPtr = srcLine;
            destPtr = dest;
            lastLit = srcPtr;
            i = 0;
            numLit = 0;
            while (i < width)
            {
                k = i;
                tryRun = srcPtr;
                while (k < width-1)
                {
                    if (*tryRun != *(tryRun + pixelAdd)) break;
                    tryRun += pixelAdd;
                    k++;
                }
                // If k < width-1 tryRun points to the first symbol not int the run,
                // otherwise it points to the last one in the run
                if (k == width-1) k = width - i;
                else k -= i;
                // Run found ==> encode literals + run
                // If a literal sequence has to be broken for the run we require a longer run.
                // For the sequence <lit><run><lit> a run shorter than 5 bytes is best merged into lit.
                // For the sequence <lit><run1><run2> the first run should be merged with lit if it's
                // less than 3 bytes long. Therefore on average 4 bytes minimum runs are best.
                if (((numLit == 0) && (k > 1)) || (k > 3))
                {
                    // First output the pending literals, if any
                    destPtr = flushLiterals(numLit, pixelAdd, destPtr, lastLit, mapColours);
                    numLit = 0;
                    i += k;
                    // Now output the run
                    while (k > 0)
                    {
                        int runLength=0;

                        runLength = k;
                        if (runLength > 255) runLength = 255;
                        *destPtr++ = runLength;
                        *destPtr++ = mapColours[*srcPtr];
                        k -= runLength;
                    }
                    srcPtr = tryRun;
                    lastLit = srcPtr;
                }
                else
                {
                    numLit++;
                    srcPtr += pixelAdd;
                    i++;
                }
            }
            // Flush remaining literals
            destPtr = flushLiterals(numLit, pixelAdd, destPtr, lastLit, mapColours);
            // EOL
            *destPtr++ = 0;
            *destPtr++ = 0;
            memfs_write(handle, dest, (destPtr - dest));
        }
        // EOB
        dest[0] = 0;
        dest[1] = 1;
        memfs_write(handle, dest, 2);

        delete [] dest;
        dest = NULL;
    }

    fileSize = memfs_size(handle);
    RMDBGONCE( 3, RMDebug::module_conversion, "r_Conv_BMP", "convertTo(): size: " << fileSize );
    offset = BMPHEADERSIZE + paletteSize * sizeof(rgb_quad_t);
    dest = bmpHeaders;
    ihead.sizeImage = fileSize - offset;

    WRITE_LE_SHORT(dest, BMP_IDENTIFIER);
    WRITE_LE_LONG(dest, fileSize);
    WRITE_LE_SHORT(dest, 0);
    WRITE_LE_SHORT(dest, 0);
    WRITE_LE_LONG(dest, offset);

    WRITE_LE_LONG(dest, ihead.size);
    WRITE_LE_LONG(dest, ihead.width);
    WRITE_LE_LONG(dest, ihead.height);
    WRITE_LE_SHORT(dest, ihead.planes);
    WRITE_LE_SHORT(dest, ihead.bitCount);
    WRITE_LE_LONG(dest, ihead.compression);
    WRITE_LE_LONG(dest, ihead.sizeImage);
    WRITE_LE_LONG(dest, ihead.xpels);
    WRITE_LE_LONG(dest, ihead.ypels);
    WRITE_LE_LONG(dest, ihead.clrUsed);
    WRITE_LE_LONG(dest, ihead.clrImportant);

    memfs_seek(handle, 0, SEEK_SET);
    memfs_write(handle, bmpHeaders, BMPHEADERSIZE);

    if ((desc.dest = (char*)mystore.storage_alloc(fileSize)) == NULL)
    {
        RMInit::logOut << "r_Conv_BMP::convertTo(): out of memory!" << endl;
        throw r_Error(MEMMORYALLOCATIONERROR);
    }
    memfs_seek(handle, 0, SEEK_SET);
    memfs_read(handle, desc.dest, fileSize);

    memfs_killfs(handle);
    delete memFS;
    memFS = NULL;

    desc.destInterv = r_Minterval(1);
    desc.destInterv << r_Sinterval((r_Range)0, (r_Range)fileSize-1);

    desc.destType = r_Type::get_any_type("char");

    return desc;
}


// Auxiliary makro for RLE coders
#define BMP_RLE_LINEFEED \
  destLine -= lineAdd; destPtr = destLine; j++; i = 0;

r_convDesc &r_Conv_BMP::convertFrom(const char *options) throw(r_Error)
{
    bitmap_file_header_t fhead;
    bitmap_info_header_t ihead;
    const rgb_quad_t *palette=NULL;
    const unsigned char *bmp=NULL;
    int i=0, j=0;
    int srcPitch=0;
    int pixelSize=0, destType=0;
    int paletteIsGrey=0, paletteSize=0;
    int width=0, height=0;
    unsigned char emit0=0, emit1=0;   // in case of bitmap -> bool: values to emit for 0 and 1
    int lineAdd=0, pixelAdd=0;

    bmp = (const unsigned char*)desc.src;

    // Read file header
    READ_LE_SHORT(bmp, fhead.type);
    READ_LE_LONG(bmp, fhead.size);
    READ_LE_SHORT(bmp, fhead.res0);
    READ_LE_SHORT(bmp, fhead.res1);
    READ_LE_LONG(bmp, fhead.offset);

    if (fhead.type != BMP_IDENTIFIER)
    {
        RMInit::logOut << "r_Conv_BMP::convertFrom(): not a BMP file" << endl;
        throw r_Error(r_Error::r_Error_General);
    }

    // Read info header
    READ_LE_LONG(bmp, ihead.size);
    READ_LE_LONG(bmp, ihead.width);
    READ_LE_LONG(bmp, ihead.height);
    READ_LE_SHORT(bmp, ihead.planes);
    READ_LE_SHORT(bmp, ihead.bitCount);
    READ_LE_LONG(bmp, ihead.compression);
    READ_LE_LONG(bmp, ihead.sizeImage);
    READ_LE_LONG(bmp, ihead.xpels);
    READ_LE_LONG(bmp, ihead.ypels);
    READ_LE_LONG(bmp, ihead.clrUsed);
    READ_LE_LONG(bmp, ihead.clrImportant);

    width = (int)ihead.width;
    height = (int)ihead.height;

    RMDBGIF(4, RMDebug::module_conversion, "r_Conv_BMP", \
            RMInit::dbgOut << "File: type " << std::hex << fhead.type << ", size " << std::dec << fhead.size; \
            RMInit::dbgOut << ", rsv0 " << fhead.res0 << ", rsv1 " << fhead.res1 << ", offs " << fhead.offset << endl; \
            RMInit::dbgOut << "Info: size " << ihead.size << ", width " << ihead.width << ", height " << ihead.height; \
            RMInit::dbgOut << ", planes " << ihead.planes << ", bits " << ihead.bitCount << ", comp " << ihead.compression; \
            RMInit::dbgOut << ", sizeImg " << ihead.sizeImage << ", xpels " << ihead.xpels << ", ypels " << ihead.ypels; \
            RMInit::dbgOut << ", clrUsed " << ihead.clrUsed << ", clrImp " << ihead.clrImportant << endl; )

    palette = (const rgb_quad_t*)(desc.src + BMPFILEHEADERSIZE + ihead.size);
    paletteIsGrey = 0;
    paletteSize = ihead.clrUsed;
    if ((paletteSize == 0) && (ihead.bitCount != 24)) paletteSize = (1 << ihead.bitCount);

    switch (ihead.bitCount)
    {
    case 1:
        srcPitch = ((width + 31) & ~31) >> 3;
        // Grey?
        if ((palette[0].red == palette[0].green) && (palette[0].green == palette[0].blue) &&
                (palette[1].red == palette[1].green) && (palette[1].green == palette[1].blue))
        {
            paletteIsGrey = 1;
            pixelSize = 1;
            // Yes; also black + white?
            if (((palette[0].red == 0x00) && (palette[1].red == 0xff)) ||
                    ((palette[0].red == 0xff) && (palette[1].red == 0x00)))
            {
                // Yes
                destType = ctype_bool;
                if (palette[0].red == 0)
                {
                    emit0 = 0;
                    emit1 = 1;
                }
                else
                {
                    emit0 = 1;
                    emit1 = 0;
                }
            }
            else
            {
                // No
                destType = ctype_char;
            }
        }
        else
        {
            // Convert to RGB
            destType = ctype_rgb;
            pixelSize = 3;
        }
        break;
    case 4:
    case 8:
    {
        if (ihead.bitCount == 4)
        {
            srcPitch = ((width + 7) & ~7) >> 1;
        }
        else
        {
            srcPitch = ((width + 3) & ~3);
        }
        // Check whether the palette is greyscale
        for (i=0; i<paletteSize; i++)
        {
            if ((palette[i].red != palette[i].green) || (palette[i].green != palette[i].blue)) break;
        }
        if (i < paletteSize)
        {
            destType = ctype_rgb;
            pixelSize = 3;
        }
        else
        {
            destType = ctype_char;
            paletteIsGrey = 1;
            pixelSize = 1;
        }
    }
    break;
    case 24:
        destType = ctype_rgb;
        srcPitch = (width * 3 + 3) & ~3;
        pixelSize = 3;
        break;
    default:
    {
        RMInit::logOut << "r_Conv_BMP::convertFrom(): Bad bitmap depth" << endl;
        throw r_Error(r_Error::r_Error_General);
    }
    }

    RMDBGONCE( 4, RMDebug::module_conversion, "r_Conv_BMP", "convertFrom(): type " << destType << ", srcPitch " << srcPitch << ", pixelSize " << pixelSize << ", palsize " << paletteSize );

    unsigned char *dest=NULL, *destPtr=NULL, *destLine=NULL;
    const unsigned char *imgPtr=NULL, *imgLine=NULL;

    pixelAdd = pixelSize * height;
    lineAdd = pixelSize;

    imgLine = (const unsigned char *)(palette + paletteSize);

    if ((dest = (unsigned char*)mystore.storage_alloc(width * height * pixelSize)) == NULL)
    {
        RMInit::logOut << "r_Conv_BMP::convertFrom(): out of memory" << endl;
        throw r_Error(MEMMORYALLOCATIONERROR);
    }

    if (ihead.compression != COMPRESS_NONE)
    {
        // This is only defined for 8bpp and 4bpp data. The result can only be 8bpp or 24bpp.
        // Init the entire picture to colour #0. Is this correct? Undocumented.
        destPtr = dest;
        i = width * height;
        if (paletteIsGrey == 0)
        {
            while (i > 0)
            {
                destPtr[0] = palette[0].red;
                destPtr[1] = palette[0].green;
                destPtr[2] = palette[0].blue;
                destPtr += pixelSize;
                i--;
            }
        }
        else
        {
            while (i > 0)
            {
                destPtr[0] = palette[0].red;
                destPtr += pixelSize;
                i--;
            }
        }
    }

    destLine = dest + ((height - 1) * pixelSize);

    switch (ihead.compression)
    {
    case COMPRESS_NONE:
        for (j=0; j<height; j++, destLine -= lineAdd, imgLine += srcPitch)
        {
            destPtr = destLine;
            imgPtr = imgLine;
            switch (ihead.bitCount)
            {
            case 1:
            {
                int mask=0;
                unsigned char val=0;

                mask = 0x00;
                switch (destType)
                {
                case ctype_bool:
                    for (i=0; i<width; i++, destPtr += pixelAdd)
                    {
                        if (mask == 0x00)
                        {
                            mask = 0x80;
                            val = *imgPtr++;
                        }
                        *destPtr = ((val & mask) == 0) ? emit0 : emit1;
                        mask >>= 1;
                    }
                    break;
                case ctype_char:
                    for (i=0; i<width; i++, destPtr += pixelAdd)
                    {
                        if (mask == 0x00)
                        {
                            mask = 0x80;
                            val = *imgPtr++;
                        }
                        *destPtr = palette[(((val & mask) == 0) ? 0 : 1)].red;
                        mask >>= 1;
                    }
                    break;
                case ctype_rgb:
                    for (i=0; i<width; i++, destPtr += pixelAdd)
                    {
                        int idx=0;

                        if (mask == 0x00)
                        {
                            mask = 0x80;
                            val = *imgPtr++;
                        }
                        idx = ((val & mask) == 0) ? 0 : 1;
                        destPtr[0] = palette[idx].red;
                        destPtr[1] = palette[idx].green;
                        destPtr[2] = palette[idx].blue;
                        mask >>= 1;
                    }
                    break;
                }
            }
            break;
            case 4:
            {
                switch (destType)
                {
                case ctype_char:
                    for (i=0; i<width; i+=2)
                    {
                        *destPtr = palette[(*imgPtr) >> 4].red;
                        destPtr += pixelAdd;
                        *destPtr = palette[(*imgPtr & 0x0f)].red;
                        destPtr += pixelAdd;
                        imgPtr++;
                    }
                    if (i < width)
                    {
                        *destPtr = palette[(*imgPtr) >> 4].red;
                    }
                    break;
                case ctype_rgb:
                    for (i=0; i<width; i+=2)
                    {
                        int idx;

                        idx = (*imgPtr) >> 4;
                        destPtr[0] = palette[idx].red;
                        destPtr[1] = palette[idx].green;
                        destPtr[2] = palette[idx].blue;
                        destPtr += pixelAdd;
                        idx = (*imgPtr) & 0x0f;
                        destPtr[0] = palette[idx].red;
                        destPtr[1] = palette[idx].green;
                        destPtr[2] = palette[idx].blue;
                        destPtr += pixelAdd;
                        imgPtr++;
                    }
                    if (i < width)
                    {
                        int idx;

                        idx = (*imgPtr) >> 4;
                        destPtr[0] = palette[idx].red;
                        destPtr[1] = palette[idx].green;
                        destPtr[2] = palette[idx].blue;
                    }
                    break;
                }
            }
            break;
            case 8:
                switch (destType)
                {
                case ctype_char:
                    for (i=0; i<width; i++, destPtr += pixelAdd)
                    {
                        *destPtr = palette[*imgPtr++].red;
                    }
                    break;
                case ctype_rgb:
                    for (i=0; i<width; i++, destPtr += pixelAdd)
                    {
                        destPtr[0] = palette[*imgPtr].red;
                        destPtr[1] = palette[*imgPtr].green;
                        destPtr[2] = palette[*imgPtr].blue;
                        imgPtr++;
                    }
                    break;
                }
                break;
            case 24:
                for (i=0; i<width; i++, destPtr += pixelAdd)
                {
                    destPtr[0] = imgPtr[2];
                    destPtr[1] = imgPtr[1];
                    destPtr[2] = imgPtr[0];
                    imgPtr += 3;
                }
                break;
            }
        }
        break;
    case COMPRESS_RLE8:
    {
        i = 0;
        j = 0;
        destPtr = destLine;
        imgPtr = imgLine;
        while (j >= 0)
        {
            unsigned char val=0, cmd=0;

            //cout << "(" << i << "," << j << ")" << endl;
            cmd = *imgPtr++;
            if (cmd == 0) // escape
            {
                cmd = *imgPtr++;
                switch (cmd)
                {
                case 0:   // end of line
                    //cout << "EOL" << endl;
                    BMP_RLE_LINEFEED;
                    break;
                case 1:   // end of bitmap
                    //cout << "EOB" << endl;
                    j = -1;
                    break;
                case 2:   // delta
                    //cout << "DELTA" << endl;
                    val = *imgPtr++;
                    i += val;
                    destPtr += val * pixelAdd;
                    val = *imgPtr++;
                    j += val;
                    destPtr -= val * lineAdd;
                    destLine -= val * lineAdd;
                    break;
                default:  // absolute mode
                    //cout << "ABS" << endl;
                    while (cmd > 0)
                    {
                        val = *imgPtr++;
                        if (j < height)
                        {
                            if (i >= width)
                            {
                                BMP_RLE_LINEFEED;
                            }
                            destPtr[0] = palette[val].red;
                            if (paletteIsGrey == 0)
                            {
                                destPtr[1] = palette[val].green;
                                destPtr[2] = palette[val].blue;
                            }
                            destPtr += pixelAdd;
                            i++;
                        }
                        cmd--;
                    }
                    // Align srcPtr to a Windows-Word position (==> aligned to short)
                    if ((((r_Ptr)imgPtr) & 1) != 0) imgPtr++;
                    break;
                }
            }
            else  // repeat sequence
            {
                //cout << "RUN" << endl;
                val = *imgPtr++;
                while (cmd > 0)
                {
                    if (j < height)
                    {
                        if (i >= width)
                        {
                            BMP_RLE_LINEFEED;
                        }
                        destPtr[0] = palette[val].red;
                        if (paletteIsGrey == 0)
                        {
                            destPtr[1] = palette[val].green;
                            destPtr[2] = palette[val].blue;
                        }
                        destPtr += pixelAdd;
                        i++;
                    }
                    cmd--;
                }
            }
        }
    }
    break;
    case COMPRESS_RLE4:
    {
        i = 0;
        j = 0;
        destPtr = destLine;
        imgPtr = imgLine;
        while (j >= 0)
        {
            unsigned char val=0, cmd=0;

            cmd = *imgPtr++;
            if (cmd == 0) // escape
            {
                cmd = *imgPtr++;
                switch (cmd)
                {
                case 0:   // end of line
                    BMP_RLE_LINEFEED;
                    break;
                case 1:   // end of bitmap
                    j = -1;
                    break;
                case 2:   // delta
                    val = *imgPtr++;
                    i += val;
                    destPtr += val * pixelAdd;
                    val = *imgPtr++;
                    j += val;
                    destPtr -= val * lineAdd;
                    destLine -= val * lineAdd;
                    break;
                default:  // absolute mode
                    while (cmd > 1)
                    {
                        val = *imgPtr++;
                        if (j < height)
                        {
                            if (i >= width)
                            {
                                BMP_RLE_LINEFEED;
                            }
                            destPtr[0] = palette[val >> 4].red;
                            if (paletteIsGrey == 0)
                            {
                                destPtr[1] = palette[val >> 4].green;
                                destPtr[2] = palette[val >> 4].blue;
                            }
                            destPtr += pixelAdd;
                            i++;
                            if (i >= width)
                            {
                                BMP_RLE_LINEFEED;
                            }
                            destPtr[0] = palette[val & 0x0f].red;
                            if (paletteIsGrey == 0)
                            {
                                destPtr[1] = palette[val & 0x0f].green;
                                destPtr[2] = palette[val & 0x0f].blue;
                            }
                            destPtr += pixelAdd;
                            i++;
                        }
                        cmd-=2;
                    }
                    if (cmd != 0)
                    {
                        val = *imgPtr++;
                        if (j < height)
                        {
                            if (i >= width)
                            {
                                BMP_RLE_LINEFEED;
                            }
                            destPtr[0] = palette[val >> 4].red;
                            if (paletteIsGrey == 0)
                            {
                                destPtr[1] = palette[val >> 4].green;
                                destPtr[2] = palette[val >> 4].blue;
                            }
                            destPtr += pixelAdd;
                            i++;
                        }
                    }
                    // Align srcPtr to a Windows-Word position (==> aligned to short)
                    if ((((r_Ptr)imgPtr) & 1) != 0) imgPtr++;
                    break;
                }
            }
            else  // repeat sequence
            {
                val = *imgPtr++;
                while (cmd > 1)
                {
                    if (j < height)
                    {
                        if (i >= width)
                        {
                            BMP_RLE_LINEFEED;
                        }
                        destPtr[0] = palette[val >> 4].red;
                        if (paletteIsGrey == 0)
                        {
                            destPtr[1] = palette[val >> 4].green;
                            destPtr[2] = palette[val >> 4].blue;
                        }
                        destPtr += pixelAdd;
                        i++;
                        if (i >= width)
                        {
                            BMP_RLE_LINEFEED;
                        }
                        destPtr[0] = palette[val & 0x0f].red;
                        if (paletteIsGrey == 0)
                        {
                            destPtr[1] = palette[val & 0x0f].green;
                            destPtr[2] = palette[val & 0x0f].blue;
                        }
                        destPtr += pixelAdd;
                        i++;
                    }
                    cmd -= 2;
                }
                if (cmd != 0)
                {
                    if (j < height)
                    {
                        if (i >= width)
                        {
                            BMP_RLE_LINEFEED;
                        }
                        destPtr[0] = palette[val >> 4].red;
                        if (paletteIsGrey == 0)
                        {
                            destPtr[1] = palette[val >> 4].green;
                            destPtr[2] = palette[val >> 4].blue;
                        }
                        destPtr += pixelAdd;
                        i++;
                    }
                }
            }
        }
    }
    break;
    default:
    {
        RMInit::logOut << "r_Conv_BMP::convertFrom(): bad compression type" << endl;
        mystore.storage_free(dest);
        throw r_Error(r_Error::r_Error_General);
    }
    }

    desc.dest = (char*)dest;
    desc.baseType = destType;

    // Build destination interval
    if (desc.srcInterv.dimension() == 2)
        // this means it was explicitly specified, so we shouldn't override it
        desc.destInterv = desc.srcInterv;
    else
    {
        desc.destInterv = r_Minterval(2);
        desc.destInterv << r_Sinterval((r_Range)0, (r_Range)width-1)
                        << r_Sinterval((r_Range)0, (r_Range)height-1);
    }

    desc.destType = get_external_type(desc.baseType);

    return desc;
}



const char *r_Conv_BMP::get_name( void ) const
{
    return format_name_bmp;
}


r_Data_Format r_Conv_BMP::get_data_format( void ) const
{
    return r_BMP;
}


r_Convertor *r_Conv_BMP::clone( void ) const
{
    return new r_Conv_BMP(desc.src, desc.srcInterv, desc.baseType);
}
