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
 *      Efficient bitmaps of colour depths 1, 2, 4, 8, 16, 24 and 32bpp
 *
 *  COMMENTS:
 *          None
 */


#ifdef __GNUG__
#pragma implementation
#pragma implementation "wx_pixmap.h"
#pragma implementation "wx_pixmap_dither.h"
#endif


#include <stdlib.h>
#include <iostream>
#include <iomanip>

// changed in wxWindows 2.4.2:
//#include "wx_prec.h"
#include <wx/wxprec.h>
#include <wx/defs.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOM
#include <wx/wx.h>
#include <wx/utils.h>
#include "wx_pixmap.h"
#include "wx_pixmap_translate.h"
#endif

#ifdef wx_msw
#include <malloc.h>
#endif

#ifdef wx_x
#include <X11/Xlib.h>
#endif






/*
 *  Globals
 */

unsigned char *TrueTransTab = NULL;
int TrueTransCount = 0;
unsigned char *FastDitherTab = NULL;
int FastDitherCount = 0;

#ifdef wx_x
typedef struct wxVisualType
{
    int depth, vclass;
} wxVisualType;

static wxVisualType wxVisualTypes[] =
{
    {32, TrueColor},
    {24, TrueColor},
    {16, TrueColor},
    {15, TrueColor},
    {8, PseudoColor},
    {8, GrayScale},
    {-1, 0}
};
#endif


/* Order of RGB components in true-colour modes */
#define RGB_ORDER_NONE  0
#define RGB_ORDER_RGB   1
#define RGB_ORDER_BGR   2





/*
 *  Determine the colour component with the biggest weight for sorting and
 *  colour-lookup.
 */

#if (COLOUR_WEIGHT_RED > COLOUR_WEIGHT_GREEN)
#error here
#define AUX1_COMPONENT  green
#define COLOUR_WEIGHT_AUX1  COLOUR_WEIGHT_GREEN
#if (COLOUR_WEIGHT_RED > COLOUR_WEIGHT_BLUE)
#define SORT_COMPONENT  red
#define AUX2_COMPONENT  blue
#define COLOUR_WEIGHT_MAX   COLOUR_WEIGHT_RED
#define COLOUR_WEIGHT_AUX1  COLOUR_WEIGHT_BLUE
#else
#define SORT_COMPONENT  blue
#define AUX2_COMPONENT  red
#define COLOUR_WEIGHT_MAX   COLOUR_WEIGHT_BLUE
#define COLOUR_WEIGHT_AUX2  COLOUR_WEIGHT_RED
#endif
#else
#define AUX1_COMPONENT  red
#define COLOUR_WEIGHT_AUX1  COLOUR_WEIGHT_RED
#if (COLOUR_WEIGHT_GREEN > COLOUR_WEIGHT_BLUE)
#define SORT_COMPONENT  green
#define AUX2_COMPONENT  blue
#define COLOUR_WEIGHT_MAX   COLOUR_WEIGHT_GREEN
#define COLOUR_WEIGHT_AUX2  COLOUR_WEIGHT_BLUE
#else
#define SORT_COMPONENT  blue
#define AUX2_COMPONENT  green
#define COLOUR_WEIGHT_MAX   COLOUR_WEIGHT_BLUE
#define COLOUR_WEIGHT_AUX2  COLOUR_WEIGHT_GREEN
#endif
#endif

#define SELECT_COMPONENT(x,c)   x.c



// General, platform independent members
int wxPixmap::getWidth(void)
{
    return(width);
}

int wxPixmap::getHeight(void)
{
    return(height);
}

int wxPixmap::getDepth(void)
{
    return(depth);
}

int wxPixmap::getPitch(void)
{
    return(pitch);
}

char *wxPixmap::getData(void)
{
    return(data);
}

wxColour *wxPixmap::getPalette(void)
{
    return(palette);
}

// Returns the depth of the current mode
int wxPixmap::getModeDepth(void)
{
    return(displayDepth);
}

// Returns the pitch of the current mode
int wxPixmap::getModePitch(void)
{
    return(displayPitch);
}

// Returns a pointer to the mode-translated data or NULL if none.
char *wxPixmap::getModeData(void)
{
    return(modeData);
}

// Returns a pointer to the translation table or NULL if none exists.
// The format depends on the display depth: <= 8 ==> 8bit, else 32bit
unsigned char *wxPixmap::getTranslationTable(void)
{
    if ((depth > 12) && (displayDepth <= 8)) return TrueTransTab;
    return TransTab.c;
}


// Error reporting / handling
void wxPixmap::errorMemory(void)
{
#if (DEBUG > 0)
    *errorstr << "Can't claim memory!" << endl;
#endif
}

void wxPixmap::errorGeneric(char *message)
{
#if (DEBUG > 0)
    *errorstr << message << endl;
#endif
}


// Display can't change on X, but it can on other systems.
// Returns depth of new mode. Internal variables holding mode information
// will be overwritten after calling this function, so make sure you copy
// them first if you need them.
void wxPixmap::getDisplayAttributes(void)
{
#ifdef wx_x
    int i;

#if 0
    for (i=0; wxVisualTypes[i].depth >= 0; i++)
    {
        if (XMatchVisualInfo(display, screen, wxVisualTypes[i].depth, wxVisualTypes[i].vclass, &visualInfo)) break;
    }
#else
    displayDepth = wxDisplayDepth();
    for (i=0; wxVisualTypes[i].depth >= 0; i++)
    {
        if (wxVisualTypes[i].depth == displayDepth)
        {
            if (XMatchVisualInfo(display, screen, wxVisualTypes[i].depth, wxVisualTypes[i].vclass, &visualInfo)) break;
        }
    }
#endif
    if (wxVisualTypes[i].depth < 0)
    {
        errorGeneric("Can't get appropriate X visual");
        displayDepth = -1;
    }

    // On X we have to find the bpp manually
    int j;
    XPixmapFormatValues *pixvals = XListPixmapFormats(display, &i);
    for (j=0; j<i; j++)
    {
        if (pixvals[j].depth == visualInfo.depth)
        {
            // Any match: set displayDepth
            displayDepth = pixvals[j].bits_per_pixel;
            // Exact match: stop.
            if (pixvals[j].bits_per_pixel == pixvals[j].depth) break;
        }
    }
    // Don't forget to free this structure!
    XFree(pixvals);

    // cout << "Use depth " << displayDepth << endl;

    rgbOrder = RGB_ORDER_RGB;
    if (displayDepth > 8)
    {
        if ((visualInfo.red_mask < visualInfo.green_mask) && (visualInfo.green_mask < visualInfo.blue_mask))
            rgbOrder = RGB_ORDER_RGB;
        else if ((visualInfo.red_mask > visualInfo.green_mask) && (visualInfo.green_mask > visualInfo.blue_mask))
            rgbOrder = RGB_ORDER_BGR;
#if (DEBUG > 0)
        else
            *errorstr << "Unrecognized pixel format, assuming r,g,b" << endl;
#endif
    }

    if (BitmapBitOrder(display) == LSBFirst) destBitorder = 0;
    else destBitorder = 1;
    if (ImageByteOrder(display) == LSBFirst) destByteorder = 0;
    else destByteorder = 1;
    //cout << "destBit " << destBitorder << ", destByte " << destByteorder << endl;
#endif

#ifdef wx_msw
    int bpp, planes, pfi;
    PIXELFORMATDESCRIPTOR pfd;

    desktop = GetDesktopWindow();
    rootDC = GetDC(desktop);
    bpp = GetDeviceCaps(rootDC, BITSPIXEL);
    planes = GetDeviceCaps(rootDC, PLANES);
    displayDepth = bpp * planes;
    pfi = 1;//GetPixelFormat(rootDC);
    if (pfi != 0)
    {
        if (DescribePixelFormat(rootDC, pfi, sizeof(PIXELFORMATDESCRIPTOR), &pfd) != 0)
        {
            displayDepth = (int)pfd.cRedBits + (int)pfd.cGreenBits + (int)pfd.cBlueBits;
            rgbOrder = RGB_ORDER_BGR;
            if (displayDepth > 8)
            {
                if ((pfd.cRedShift < pfd.cGreenShift) && (pfd.cGreenShift < pfd.cBlueShift))
                    rgbOrder = RGB_ORDER_RGB;
                else if ((pfd.cRedShift > pfd.cGreenShift) && (pfd.cGreenShift > pfd.cBlueShift))
                    rgbOrder = RGB_ORDER_BGR;
#if (DEBUG > 0)
                else
                    *errorstr << "Unrecognized pixel format, assuming b,g,r" << endl;
#endif
            }
#if (DEBUG > 0)
            else
                *errorstr << "Error describing pixel format (" << GetLastError() << ")" << endl;
#endif
        }
    }
#if (DEBUG > 0)
    else
        *errorstr << "Error reading pixel format (" << GetLastError() << ")" << endl;
#endif

    // On NT use the machine order
    destBitorder = WX_PIXMAP_SRC_BITORDER;
    destByteorder = WX_PIXMAP_SRC_BYTEORDER;
#endif

    // Filter out unsupported display depths
    switch (displayDepth)
    {
    case 1:
    case 2:
    case 4:
    case 8:
    case 15:
    case 16:
    case 24:
    case 32:
        break;
    default:
#if (DEBUG > 0)
        *errorstr << "Unsupported display depth " << displayDepth << endl;
#endif
        displayDepth = -1;
        return;
    }

#if (DEBUG > 0)
    *errorstr << "Display depth = " << displayDepth
              << ", rgb format " << ((rgbOrder == RGB_ORDER_RGB) ? "r,g,b" : "b,g,r") << endl;
#endif
}


void wxPixmap::initColoursForMode(bool forceUpdate)
{
#ifdef wx_pixmap_alloc_cols
    // Clear colour allocation bitfield
    memset(ColourAlloc, 0, 32);
#else
    processParentPalette(forceUpdate);
#endif
    setupTranslators();
}


// Stuff shared by all constructors.
void wxPixmap::initVariables(void)
{
    data = NULL;
    modeData = NULL;
    palette = NULL;
    TransTab.c = NULL;
    validDisplay = FALSE;
    busyCursorMode = TRUE;
    parentWin = NULL;
    errorstr = NULL;
#ifndef wx_pixmap_alloc_cols
    parentPalette = NULL;
    pixmapPalette = NULL;
    parentPaletteDepth = 0;
#endif
    width=0;
    height=0;
    depth=0;
    pad=0;

#if (DEBUG > 0)
#ifdef wx_x
    // For some reason the other variant crashes on Sun
    errorstr = (ostream*)&cerr;
#else
#ifdef wx_msw
    errorstr = new ofstream("debug.log",ios::out | ios::trunc);
#else
    errorstr = new ostream(&streamBuf);
#endif
#endif
#endif

#ifdef wx_x
    xim = NULL;
    display = wxGetDisplay();
    myGC = (GC)0;

    screen = XDefaultScreen(display);
    rootwin = XRootWindow(display, screen);

    //Screen *myScreen = DefaultScreenOfDisplay(display);
    //cout << "root depth " << myScreen->root_depth << endl;

#endif

#ifdef wx_msw
    bitmap = (HBITMAP)0;
    windowHandle = (HWND)0;
    rootDC = (HDC)0;
    srcDC = (HDC)0;
    winDC = (HDC)0;
    oldDCObject = (HGDIOBJ)0;
#endif

    getDisplayAttributes();
}


// New Depth and Flags are passed to this function to allow optimising colour handling.
void wxPixmap::freeResources(int Depth, unsigned int Flags)
{
#ifdef wx_msw
    if (bitmap != (HBITMAP)0)
    {
        DeleteObject(bitmap);
        bitmap = (HBITMAP)0;
    }
    if (modeData != data)
    {
        free(modeData);
        modeData = NULL;
    }
    // For symmetry to X
    if (data != NULL)
    {
        free(data);
        data = NULL;
    }
    if (srcDC != (HDC)0)
    {
        SelectObject(srcDC, oldDCObject);
        DeleteObject(srcDC);
        srcDC = (HDC)0;
    }
#endif

#ifdef wx_x
    if ((modeData != NULL) && (modeData != data))
    {
        // In this case the XImage structure was initialised with modeData
        // rather than data. Therefore on deleting xim modeData will be
        // deleted automatically. Use malloc/free combination rather than
        // new/delete.
        free(data);
        data = NULL;
    }

    // Free image if any. This also destroys the data it was initialised with
    // (see above).
    if (xim != NULL)
    {
        XDestroyImage(xim);
        xim = NULL;
    }

    // The graphical context can change every time newPixmap is called, therefore
    // it must be deleted here and not in the destructor.
    if (myGC != (GC)0)
    {
        XFreeGC(display, myGC);
        myGC = (GC)0;
    }

    // If the new palette is the same as the old one we leave the colours as they are.
    // Note that calling with no colour translation but with samepalette doesn't
    // free anthing - that's because the Translation table has to be present even
    // if no _automatic_ translation takes place.
#ifdef wx_pixmap_alloc_cols
    if ((Flags & WX_PIXFLAG_SAMEPALETTE) == 0)
    {
        // Free colours if any were allocated (i.e. displayDepth <= 8)
        if ((displayDepth <= 8) && (TransTab.c != NULL))
        {
            unsigned long pixels[256];
            int i, j;

            for (i=0, j=0; i<(1<<displayDepth); i++)
            {
                if ((ColourAlloc[i>>3] & (1 << (i & 7))) != 0)
                {
                    pixels[j++] = i;
                }
            }
            XFreeColors(display, xat.colormap, pixels, j, 0);
            // *errorstr << j << " colours freed." << endl;
        }
    }
#endif
#endif // #ifdef wx_x

    // This is for all machines again
    if ((Flags & WX_PIXFLAG_SAMEPALETTE) == 0)
    {
        // In case both depth > 8 (true-colour) and old depth > 8 we don't need to make a new
        // colourtable, hence we don't have to delete the old one.
        if ((depth <= 12) && (Depth <= 12) && (TransTab.c != NULL))
        {
            delete [] TransTab.c;
            TransTab.c = NULL;
        }
    }

    // Update TrueColour Translation table
    if ((depth > 12) && (displayDepth <= 8))
    {
        if (depth != Depth)
        {
            TrueTransCount--;
#ifdef WX_PIXMAP_GLOBAL_VOLATILE
            if (TrueTransCount == 0)
            {
                delete [] TrueTransTab;
                TrueTransTab = NULL;
            }
#endif
        }
    }
    // Fast dithering table?
    if ((pixFlags & WX_PIXFLAG_FASTDITHER) != 0)
    {
        FastDitherCount--;
#ifdef WX_PIXMAP_GLOBAL_VOLATILE
        if (FastDitherCount == 0)
        {
            delete [] FastDitherTab;
            FastDitherTab = NULL;
        }
#endif
    }
    if (palette != NULL)
    {
        delete [] palette;
        palette = NULL;
    }
}


#ifndef wx_pixmap_alloc_cols

void wxPixmap::processParentPalette(bool forceUpdate)
{
    if (displayDepth > 8)
    {
        if (parentPalette != NULL)
        {
            delete [] parentPalette;
            parentPalette=NULL;
            parentPaletteDepth = 0;
        }
    }
    // For non-truecolour modes read the screen palette and install fast lookup functions
    // Essentially forceUpdate should only be set to true if only the palette, not the
    // more depth has changed.
    else if ((parentPaletteDepth != displayDepth) || forceUpdate)
    {
        unsigned int i, ncols;
        ncols = (1<<displayDepth);

        // just in case...
        if (parentPalette != NULL)
        {
            delete [] parentPalette;
            parentPalette = NULL;
            parentPaletteDepth = 0;
        }

#ifdef wx_x
        XColor displayColours[256];

        // *errorstr << "Colours = " << ncols << endl;
        for (i=0; i<ncols; i++)
        {
            displayColours[i].pixel = (unsigned long)i;
        }
        XQueryColors(display, xat.colormap, displayColours, ncols);
        if ((parentPalette = new wx_permute_cmap[ncols]) == NULL)
        {
            errorMemory();
            return;
        }
        for (i=0; i<ncols; i++)
        {
            register wx_permute_cmap *pc = &(parentPalette[i]);
            register XColor *xc = &(displayColours[i]);

            pc->red = ((xc->red)>>8);
            pc->green = ((xc->green)>>8);
            pc->blue = ((xc->blue)>>8);
            pc->number = i;
        }
#endif

#ifdef wx_msw
        PALETTEENTRY palEntries[256];

        GetSystemPaletteEntries(rootDC, 0, ncols, palEntries);

        if ((parentPalette = new wx_permute_cmap[ncols]) == NULL)
        {
            errorMemory();
            return;
        }
        for (i=0; i<ncols; i++)
        {
            register wx_permute_cmap *pc = &(parentPalette[i]);
            register PALETTEENTRY *pe = &(palEntries[i]);

            pc->red = pe->peRed;
            pc->green = pe->peGreen;
            pc->blue = pe->peBlue;
            pc->number = i;
        }
#endif

        sortParentPalette(0, ncols-1);

        buildInverseTable();

        parentPaletteDepth = displayDepth;
    }
}


// Returns a pointer to the parent palette. For use with findBestColour.
wx_permute_cmap *wxPixmap::getParentPalette(void)
{
    return parentPalette;
}



#define PPAL_SWAP(a,b,h) h=parentPalette[a]; parentPalette[a]=parentPalette[b]; parentPalette[b]=h;

// Quicksorter of parent palette, using the component with the biggest loading value
void wxPixmap::sortParentPalette(int from, int to)
{
    while (from < to)
    {
        int mid, i, j;
        wx_permute_cmap x, y;

        mid = (from + to)/2;
        j = from;
        PPAL_SWAP(mid, from, x);    // Side-effect: element at first index (before swap) is in x.
        for (i=from+1; i<=to; i++)
        {
            if (SELECT_COMPONENT(parentPalette[i],SORT_COMPONENT) < SELECT_COMPONENT(x,SORT_COMPONENT))
            {
                j++;
                PPAL_SWAP(i,j,y);
            }
        }
        PPAL_SWAP(from,j,y);

        // Decide which branch to recurse into to minimise Stack consumption
        if ((j - from) < (to - j))
        {
            sortParentPalette(from, j-1);
            from = j+1;
        }
        else
        {
            sortParentPalette(j+1, to);
            to = j-1;
        }
    }
}


// Builds an inverse lookup-table for the parent palette (after it was sorted).
// inverseTable[x] specifies the index in parentPalette from where to search for
// the colour with SORT_COMPONENT = x
void wxPixmap::buildInverseTable(void)
{
    unsigned int i, j, idx, last, value;

    i=0;
    last=0;
    while (i < (unsigned int)(1<<displayDepth))
    {
        idx = i;
        value = SELECT_COMPONENT(parentPalette[idx],SORT_COMPONENT);
        while (++i < (unsigned int)(1<<displayDepth))
        {
            if (SELECT_COMPONENT(parentPalette[i],SORT_COMPONENT) != value) break;
        }
        // *errorstr << '(' << idx << ',' << i << "):" << last << ',' << value << " ";
        for (j=last; j<=value; j++)
        {
            parentInverse[j] = (i+idx-1)>>1;
        }
        last = value+1;
    }
    // fill remaining entries
    while (last < 256)
    {
        parentInverse[last] = (1<<displayDepth)-1;
        last++;
    }

    /*for (i=0; i<256; i++)
    {
      *errorstr << '[' << i << ':' << (int)parentInverse[i] << "] ";
    }
    *errorstr << endl;*/
}


#define square_val(x)   ((x)*(x))

// Returns the index into parentPalette which addresses the best fit for the RGB colour
// passed to the function. Don't use for (display) modes with more than 8bpp.
// This requires parentPalette and parentInverse to be set up correctly. This function
// may only be used after parentPalette has been sorted and the inverse table has been
// built.
int wxPixmap::findBestColour(unsigned char red, unsigned char green, unsigned char blue)
{
    COLORREF bestMatch = 0x7fffffff, match;
    int up, down, bestIdx=-1;

    down = parentInverse[SORT_COMPONENT];
    up = down+1;
    while ((down >= 0) || (up < (1<<displayDepth)))
    {
        // Twice basically the same for up and down, but speed is _very_ critical here
        if (down >= 0)
        {
            match = COLOUR_WEIGHT_MAX * square_val((int)SORT_COMPONENT - (int)SELECT_COMPONENT(parentPalette[down], SORT_COMPONENT));
            if (match > bestMatch) down=-1;
            else
            {
                match += COLOUR_WEIGHT_AUX1 * square_val((int)AUX1_COMPONENT - (int)SELECT_COMPONENT(parentPalette[down], AUX1_COMPONENT)) + COLOUR_WEIGHT_AUX2 * square_val((int)AUX2_COMPONENT - (int)SELECT_COMPONENT(parentPalette[down], AUX2_COMPONENT));
                if (match < bestMatch)
                {
                    bestMatch = match;
                    bestIdx = down;
                }
                down--;
            }
        }
        if (up < (1<<displayDepth))
        {
            match = COLOUR_WEIGHT_MAX * square_val((int)SORT_COMPONENT - (int)SELECT_COMPONENT(parentPalette[up], SORT_COMPONENT));
            if (match > bestMatch) up=(1<<displayDepth);
            else
            {
                match += COLOUR_WEIGHT_AUX1 * square_val((int)AUX1_COMPONENT - (int)SELECT_COMPONENT(parentPalette[up], AUX1_COMPONENT)) + COLOUR_WEIGHT_AUX2 * square_val((int)AUX2_COMPONENT - (int)SELECT_COMPONENT(parentPalette[up], AUX2_COMPONENT));
                if (match < bestMatch)
                {
                    bestMatch = match;
                    bestIdx = up;
                }
                up++;
            }
        }
    }
    // *errorstr << "Best match for " << (int)red << ',' << (int)green << ',' << (int)blue << ": " << bestIdx << " (" << (int)parentPalette[bestIdx].red << ',' << (int)parentPalette[bestIdx].green << ',' << (int)parentPalette[bestIdx].blue << ')' << endl;

    return bestIdx;
}


int wxPixmap::findFastColour(unsigned char red, unsigned char green, unsigned char blue)
{
    return (int)(FastDitherTab[(red>>3) | ((green & 0xf8)<<2) | ((blue&0xf8)<<7)]);
}


// Converts the pixmap's palette into a wx_permute_cmap which is needed by the ditherer.
// Only call if depth <= 8! Other modes are assumed true-colour.
void wxPixmap::processPixmapPalette(void)
{
    int i;
    int tabsize;

    if (depth <= 8) tabsize = (1<<depth);
    else tabsize = (1<<12);

    if (pixmapPalette != NULL)
    {
        delete [] pixmapPalette;
        pixmapPalette=NULL;
    }

    if ((pixmapPalette = new wx_permute_cmap[tabsize]) == NULL)
    {
        errorMemory();
        return;
    }
    // Greyscale?
    if (palette == NULL)
    {
        int step, grey;

        step = 0xff00/(tabsize-1);
        grey = 0;
        for (i=0; i<tabsize; i++)
        {
            pixmapPalette[i].red = pixmapPalette[i].green = pixmapPalette[i].blue = (grey >> 8);
            grey += step;
        }
    }
    else
    {
        unsigned char red, green, blue;

        for (i=0; i<tabsize; i++)
        {
            // palette[i].Get(&red, &green, &blue); -- PB 2006-jan-01
            red   = palette[i].Red();
            green = palette[i].Green();
            blue  = palette[i].Blue();
            pixmapPalette[i].red = red;
            pixmapPalette[i].green = green;
            pixmapPalette[i].blue = blue;
        }
    }
}


#endif  // wx_pixmap_alloc_cols



// Builds the translation table for the current bitmap + palette / display setup.
void wxPixmap::buildTranslationTable(void)
{
#ifndef wx_pixmap_alloc_cols
    int bestIdx;
#else
#ifdef wx_x
    char cstring[16]; // buffer for holding colour strings
    XColor xcolour;
    unsigned long pixel;
#endif
#endif

    int i;
    unsigned char red, green, blue;
    bool BuildTrueTable = FALSE;

    // *errorstr << "Build translation table" << endl;

    // Essentially this function shouldn't be called with a valid translation table
    // but to avoid memory leaks let's free an old table anyway.
    if (TransTab.c != NULL)
    {
        delete [] TransTab.c;
        TransTab.c = NULL;
    }

    TransTabSize = -1;
    if (depth <= 8) TransTabSize = (1<<depth);
    else if (depth == 12) TransTabSize = (1<<8);
    else if (displayDepth <= 8) TransTabSize = (1<<15);

    if (TransTabSize > 0)
    {
        if (TransTabSize > 256) // For a true colour mode?
        {
            if (TrueTransTab == NULL)
            {
                if ((TrueTransTab = new unsigned char[(1<<15)]) == NULL)
                {
                    errorMemory();
                    return;
                }
                BuildTrueTable = TRUE;
            }
            TrueTransCount++;
        }
        if (TransTabSize <= 256);
        {
            // Base type of translation table depends on display depth
            if (displayDepth <= 8) TransTab.c = new unsigned char[TransTabSize];
            else TransTab.l = new COLORREF[TransTabSize];
            if (TransTab.c == NULL)
            {
                errorMemory();
                return;
            }
        }
        // *errorstr << "Allocated Translation table of size " << TransTabSize << endl;
    }

    if (((pixFlags & WX_PIXFLAG_FASTDITHER) != 0) && (displayDepth <= 8))
    {
        if (FastDitherTab == NULL)
        {
            if ((FastDitherTab = new unsigned char[(1<<15)]) == NULL)
            {
                errorMemory();
                return;
            }
            if (busyCursorMode) ::wxBeginBusyCursor();
            for (i=0; i<0x8000; i++)
            {
                PALETTE_SHORT_TO_RGB(i, red, green, blue);
                FastDitherTab[i] = findBestColour(red, green, blue);
            }
            if (busyCursorMode) ::wxEndBusyCursor();
        }
        FastDitherCount++;
    }

    // Palette indices --> pixel indices
    if ((depth <= 12) && (displayDepth <= 8))
    {
        int tabsize;

        if (depth <= 8) tabsize = (1<<depth);
        else tabsize = (1<<8);
        for (i=0; i<tabsize; i++)
        {
            if (palette != NULL)
            {
                red   = palette[i].Red();
                green = palette[i].Green();
                blue  = palette[i].Blue();
            }
            else
            {
                // Assume greyscale
                red = green = blue = i;
            }
#ifdef wx_pixmap_alloc_cols
#ifdef wx_x
            sprintf(cstring, "rgb:%x/%x/%x", red, green, blue);
            XParseColor(display, xat.colormap, cstring, &xcolour);
            if (XAllocColor(display, xat.colormap, &xcolour))
            {
                // ColourAlloc holds the display-, not the source colour numbers.
                // Allocate each XColour only once! Haven't found an X-call that just
                // returns the pixel value so I have to allocate and free it in case
                // it was already allocated.
                if ((ColourAlloc[(xcolour.pixel)>>3] & (1 << ((xcolour.pixel) & 7))) == 0)
                {
                    ColourAlloc[(xcolour.pixel)>>3] |= (1 << ((xcolour.pixel) & 7));
                }
                else
                {
                    pixel = xcolour.pixel;
                    XFreeColors(display, xat.colormap, &pixel, 1, 0);
                }
            }
            TransTab.c[i] = (unsigned char)xcolour.pixel;
            // *errorstr << i << " (" << cstring << "): " << xcolour.pixel << endl;
#endif
#ifdef wx_msw
            TransTab.c[i] = (unsigned char)(GetNearestPaletteIndex(rootDC, red | (green<<8) | (blue<<16)));
#endif
#else
            bestIdx = findBestColour(red, green, blue);
            TransTab.c[i] = parentPalette[bestIdx].number;
#endif
        }
    }
    if ((depth > 8) && (displayDepth <= 8))
    {
        if (BuildTrueTable)
        {
            if (busyCursorMode) ::wxBeginBusyCursor();
            // This is a bit heavy...
            for (i=0; i<0x8000; i++)
            {
                // The internal format is always the same
                PALETTE_SHORT_TO_RGB(i,red,green,blue);
#ifdef wx_pixmap_alloc_cols
#ifdef wx_x
                sprintf(cstring, "rgb:%x/%x/%x", red, green, blue);
                XParseColor(display, xat.colormap, cstring, &xcolour);
                if (XAllocColor(display, xat.colormap, &xcolour))
                {
                    if ((ColourAlloc[(xcolour.pixel)>>3] & (1 << ((xcolour.pixel) & 7))) == 0)
                    {
                        ColourAlloc[(xcolour.pixel)>>3] |= (1 << ((xcolour.pixel) & 7));
                    }
                    else
                    {
                        pixel = xcolour.pixel;
                        XFreeColors(display, xat.colormap, &pixel, 1, 0);
                    }
                }
                TrueTransTab[i] = (unsigned char)xcolour.pixel;
#endif
#ifdef wx_msw
                TrueTransTab[i] = (unsigned char)(GetNearestPaletteIndex(rootDC, red | (green<<8) | (blue<<16)));
#endif
#else
                bestIdx = (FastDitherTab == NULL) ? findBestColour(red, green, blue) : FastDitherTab[i];
                TrueTransTab[i] = parentPalette[bestIdx].number;
#endif
            }
            if (busyCursorMode) ::wxEndBusyCursor();
        }
    }

    // Special translation tables which are platform independent (true-colour displays):
    if ((depth <= 12) && (displayDepth > 8))
    {
        int tabsize;

        if (depth <= 8) tabsize = (1<<depth);
        else tabsize = (1<<8);

        // Data with <= 8bpp and no palette interpreted as greyscales.
        if (palette == NULL)
        {
            int step, grey, value = 0;

            step = 0xff00 / (tabsize -1); // stepping value in .8 fixpoint format
            if (displayDepth == 15)   // 15bpp
            {
                for (i=0; i<tabsize; i++, value += step)
                {
                    grey = ((value + 0x400) >> 8);    // convert fixpoint to integer, rounding.
                    if (grey > 255) grey = 255;

                    if (destByteorder == WX_PIXMAP_SRC_BYTEORDER)
                        TransTab.l[i] = _RGBL_TO_PALETTE_SHORT15(grey, grey, grey);
                    else
                        TransTab.l[i] = _RGBL_TO_PALETTE_SHORT15i(grey, grey, grey);
                }
            }
            else if (displayDepth == 16)  // 16bpp
            {
                for (i=0; i<tabsize; i++, value += step)
                {
                    grey = ((value + 0x400) >> 8);
                    if (grey > 255) grey = 255;

                    if (destByteorder == WX_PIXMAP_SRC_BYTEORDER)
                        TransTab.l[i] = _RGBL_TO_PALETTE_SHORT16(grey, grey, grey);
                    else
                        TransTab.l[i] = _RGBL_TO_PALETTE_SHORT16i(grey, grey, grey);
                }
            }
            else  // 24/32bpp
            {
                for (i=0; i<tabsize; i++, value += step)
                {
                    grey = (value + 0x80) >> 8;

                    if (destByteorder == WX_PIXMAP_SRC_BYTEORDER)
                        TransTab.l[i] = _RGB_TO_PALETTE_LONG(grey, grey, grey);
                    else
                        TransTab.l[i] = _RGB_TO_PALETTE_LONGi(grey, grey, grey);
                }
            }
        }
        else    // palette != NULL ==> determine the true-colour values
        {
            if (displayDepth == 15)
            {
                for (i=0; i<tabsize; i++)
                {
                    red   = palette[i].Red();
                    green = palette[i].Green();
                    blue  = palette[i].Blue();

                    if (destByteorder == WX_PIXMAP_SRC_BYTEORDER)
                        TransTab.l[i] = _RGBL_TO_PALETTE_SHORT15(red, green, blue);
                    else
                        TransTab.l[i] = _RGBL_TO_PALETTE_SHORT15i(red, green, blue);
                }
            }
            else if (displayDepth == 16)
            {
                for (i=0; i<tabsize; i++)
                {
                    red   = palette[i].Red();
                    green = palette[i].Green();
                    blue  = palette[i].Blue();

                    if (destByteorder == WX_PIXMAP_SRC_BYTEORDER)
                        TransTab.l[i] = _RGBL_TO_PALETTE_SHORT16(red, green, blue);
                    else
                        TransTab.l[i] = _RGBL_TO_PALETTE_SHORT16i(red, green, blue);
                }
            }
            else
            {
                for (i=0; i<tabsize; i++)
                {
                    red   = palette[i].Red();
                    green = palette[i].Green();
                    blue  = palette[i].Blue();

                    if (destByteorder == WX_PIXMAP_SRC_BYTEORDER)
                        TransTab.l[i] = _RGB_TO_PALETTE_LONG(red, green, blue);
                    else
                        TransTab.l[i] = _RGB_TO_PALETTE_LONGi(red, green, blue);
                }
            }
        }
    }

#if (DEBUG > 0)
    *errorstr << "Translation table (" << TransTabSize << "), table entry size ";
    if (displayDepth <= 8) *errorstr << (int)8;
    else *errorstr << (int)32;
    *errorstr << " bits." << endl;

    *errorstr << hex;
    if (displayDepth <= 8)
    {
        if (depth > 12)
        {
            for (i=0; i<TransTabSize; i++)
            {
                *errorstr << '[' << setw(2) << int(TrueTransTab[i]) << "] ";
                if ((i & 7) == 7)
                {
                    *errorstr << endl;
                }
            }
        }
        else
        {
            for (i=0; i<TransTabSize; i++)
            {
                *errorstr << '[' << setw(2) << int(TransTab.c[i]) << "] ";
                if ((i & 7) == 7)
                {
                    *errorstr << endl;
                }
            }
        }
    }
    else
    {
        for (i=0; i<TransTabSize; i++)
        {
            *errorstr << '[' << setw(8) << TransTab.l[i] << "] ";
            if ((i & 7) == 7)
            {
                *errorstr << endl;
            }
        }
    }
    *errorstr << dec << endl;
#endif
}


// To avoid repetitions...
#define _TRANSLATORS_FALSE(target) \
  pixtrans1  = &wx_pixmap_translate_1_to_##target;  pixdither1  = &wxPixmap::dither_1_to_##target; \
  pixtrans2  = &wx_pixmap_translate_2_to_##target;  pixdither2  = &wxPixmap::dither_2_to_##target; \
  pixtrans4  = &wx_pixmap_translate_4_to_##target;  pixdither4  = &wxPixmap::dither_4_to_##target; \
  pixtrans8  = &wx_pixmap_translate_8_to_##target;  pixdither8  = &wxPixmap::dither_8_to_##target; \
  pixtrans12 = &wx_pixmap_translate_12_to_##target; pixdither12 = &wxPixmap::dither_12_to_##target; \
  pixtrans15 = &wx_pixmap_translate_15_to_##target; pixdither15 = &wxPixmap::dither_15_to_##target; \
  pixtrans24 = &wx_pixmap_translate_24_to_##target; pixdither24 = &wxPixmap::dither_24_to_##target; \
  pixtrans32 = &wx_pixmap_translate_32_to_##target; pixdither32 = &wxPixmap::dither_32_to_##target;

#define _TRANSLATORS_TRUE(target,target2) \
  pixtrans1  = wx_pixmap_translate_1_to_##target; \
  pixtrans2  = wx_pixmap_translate_2_to_##target; \
  pixtrans4  = wx_pixmap_translate_4_to_##target; \
  pixtrans8  = wx_pixmap_translate_8_to_##target; \
  pixtrans12 = wx_pixmap_translate_12_to_##target; \
  pixtrans15 = wx_pixmap_translate_15_to_##target2; \
  pixtrans24 = wx_pixmap_translate_24_to_##target2; \
  pixtrans32 = wx_pixmap_translate_32_to_##target2;

void wxPixmap::setupTranslators(void)
{
    // Init defaults...
    pixtrans1 = NULL;
    pixtrans2 = NULL;
    pixtrans4 = NULL;
    pixtrans8 = NULL;
    pixtrans12 = NULL;
    pixtrans15 = NULL;
    pixtrans24 = NULL;
    pixtrans32 = NULL;
    pixdither1 = NULL;
    pixdither2 = NULL;
    pixdither4 = NULL;
    pixdither8 = NULL;
    pixdither12 = NULL;
    pixdither15 = NULL;
    pixdither24 = NULL;
    pixdither32 = NULL;

#ifdef wx_x
    if ((destBitorder == WX_PIXMAP_SRC_BITORDER) && (destByteorder == WX_PIXMAP_SRC_BYTEORDER))
#endif
    {
        switch (displayDepth)
        {
        case 1:
            _TRANSLATORS_FALSE(1);
            break;
        case 2:
            _TRANSLATORS_FALSE(2);
            break;
        case 4:
            _TRANSLATORS_FALSE(4);
            break;
        case 8:
            _TRANSLATORS_FALSE(8);
            break;
        case 15:
            // For depths 1 to 12 the translators are identical.
            if (rgbOrder == RGB_ORDER_RGB)
            {
                _TRANSLATORS_TRUE(16rgb, 15rgb);
            }
            else
            {
                _TRANSLATORS_TRUE(16bgr, 15bgr);
            }
            break;
        case 16:
            if (rgbOrder == RGB_ORDER_RGB)
            {
                _TRANSLATORS_TRUE(16rgb, 16rgb);
            }
            else
            {
                _TRANSLATORS_TRUE(16bgr, 16bgr);
            }
            break;
        case 24:
            if (rgbOrder == RGB_ORDER_RGB)
            {
                _TRANSLATORS_TRUE(24rgb, 24rgb);
            }
            else
            {
                _TRANSLATORS_TRUE(24bgr, 24bgr);
            }
            break;
        case 32:
            if (rgbOrder == RGB_ORDER_RGB)
            {
                _TRANSLATORS_TRUE(32rgb, 32rgb);
            }
            else
            {
                _TRANSLATORS_TRUE(32bgr, 32bgr);
            }
            break;
        default:
            break;
        }
    }
#ifdef wx_x
    else
    {
        switch (displayDepth)
        {
        case 1:
            _TRANSLATORS_FALSE(1i);
            break;
        case 2:
            _TRANSLATORS_FALSE(2i);
            break;
        case 4:
            _TRANSLATORS_FALSE(4i);
            break;
        case 8:
            _TRANSLATORS_FALSE(8i);
            break;
        case 15:
            // For depths 1 to 12 the translators are identical.
            if (rgbOrder == RGB_ORDER_RGB)
            {
                _TRANSLATORS_TRUE(16irgb, 15irgb);
            }
            else
            {
                _TRANSLATORS_TRUE(16ibgr, 15ibgr);
            }
            break;
        case 16:
            if (rgbOrder == RGB_ORDER_RGB)
            {
                _TRANSLATORS_TRUE(16irgb, 16irgb);
            }
            else
            {
                _TRANSLATORS_TRUE(16ibgr, 16ibgr);
            }
            break;
        case 24:
            if (rgbOrder == RGB_ORDER_RGB)
            {
                _TRANSLATORS_TRUE(24irgb, 24irgb);
            }
            else
            {
                _TRANSLATORS_TRUE(24ibgr, 24ibgr);
            }
            break;
        case 32:
            if (rgbOrder == RGB_ORDER_RGB)
            {
                _TRANSLATORS_TRUE(32irgb, 32irgb);
            }
            else
            {
                _TRANSLATORS_TRUE(32ibgr, 32ibgr);
            }
            break;
        default:
            break;
        }
    }
#endif
}



// Must be called when the current mode changes. If the depth of new and old
// screen mode is identical Flags determines whether a new Translation table
// should be built (currently only this one bit used). This obviously never
// gets called on X.
void wxPixmap::modeChange(unsigned int modeFlags)
{
    int lastDisplayDepth = displayDepth;

    getDisplayAttributes();

    // depth hasn't changed or automatic translation not necessary ==> just build
    // the tables.
    if (((displayDepth == lastDisplayDepth) && ((modeFlags & WX_PIXMODE_PALETTE) != 0)) ||
            ((pixFlags & WX_PIXFLAG_TRANSLATE) == 0))
    {
        initColoursForMode(TRUE);
        buildTranslationTable();
    }
    else
    {
        // otherwise we need to set up completely new buffers and stuff.
        if (TrueTransTab != NULL)
        {
            delete [] TrueTransTab;
            TrueTransTab = NULL;
            TrueTransCount = 0;
        }
        if (FastDitherTab != NULL)
        {
            delete [] FastDitherTab;
            FastDitherTab = NULL;
            FastDitherCount = 0;
        }
        newPixmap(parentWin, width, height, depth, pad, data, pixFlags | WX_PIXFLAG_SAMEPALETTE, palette);
    }
}


wxPixmap::wxPixmap(void)
{
    initVariables();
}


wxPixmap::wxPixmap(wxWindow *Win, int Width, int Height, int Depth, int Pad, char *Data, unsigned int Flags, wxColour *Palette)
{
    initVariables();
    newPixmap(Win, Width, Height, Depth, Pad, Data, Flags, Palette);
}


wxPixmap::~wxPixmap(void)
{
    if (errorstr != NULL)
    {
#ifndef wx_x
#ifdef wx_msw
        errorstr->flush();
#endif
        delete errorstr;
#endif
    }

    // Use illegal value for depth to make sure everything is freed
    freeResources(0, 0);

#ifndef wx_pixmap_alloc_cols
    if (parentPalette != NULL)
    {
        delete [] parentPalette;
        parentPalette = NULL;
    }
    if (pixmapPalette != NULL)
    {
        delete [] pixmapPalette;
        pixmapPalette = NULL;
    }
#endif
}


int wxPixmap::newPixmap(wxWindow *Win, int Width, int Height, int Depth, int Pad, char *Data, unsigned int Flags, wxColour *Palette)
{
    int lastDepth = depth;
#ifdef wx_msw
    int virtWidth;
#endif

    // Can we display it at all?
    if (displayDepth <= 0) return -1;

    // Filter out unsupported depths
    switch (Depth)
    {
    case 1:
    case 2:
    case 4:
    case 8:
    case 12:
    case 15:
    case 24:
    case 32:
        break;
    default:
#if (DEBUG > 0)
        *errorstr << "Unsupported colour depth " << Depth << endl;
#endif
        return -1;
    }

    freeResources(Depth, Flags);

    width = Width;
    height = Height;
    depth = Depth;
    pad = Pad;
    if ((depth == 12) || (depth == 15) || (depth == 16))
        pitch = (width * 16 + Pad - 1) >> 3;
    else
        pitch = (width * depth + Pad - 1) >> 3;

    switch (Pad)
    {
    case 16:
        pitch &= ~1;
        break;
    case 32:
        pitch &= ~3;
        break;
    case 64:
        pitch &= ~7;
        break;
    case 128:
        pitch &= ~15;
        break;
    case 256:
        pitch &= ~31;
        break;
    default:
        break;
    }
    data = Data;
    validDisplay = FALSE;
    pixFlags = Flags;
    parentWin = Win;
    if ((pixFlags & WX_PIXFLAG_FASTDITHER) != 0)
    {
        pixFlags |= WX_PIXFLAG_DITHER;
        colour_matcher = &wxPixmap::findFastColour;
    }
    else
    {
        colour_matcher = &wxPixmap::findBestColour;
    }

    // If the depth changed the translation tables _must_ be updated
    if (lastDepth != Depth)
    {
        pixFlags &= ~WX_PIXFLAG_SAMEPALETTE;
    }

    if ((pixFlags & WX_PIXFLAG_SAMEPALETTE) == 0)
    {
        if ((Palette != NULL) && (Depth <= 8))
        {
            palette = new wxColour[1<<Depth];
            memcpy(palette, Palette, (1<<Depth)*sizeof(wxColour));
        }
    }

#ifndef wx_msw
    // The data has to be translated if its depth differs from the display depth
    // or the format is different from the internal one. A colour translation
    // table is needed if translated data is needed AND either mode is <= 8bpp.
    if (((depth == displayDepth) && (depth == 24) && (rgbOrder == RGB_ORDER_RGB)) ||
            ((pixFlags & WX_PIXFLAG_TRANSLATE) == 0))
    {
        modeData = data;
        displayPitch = pitch;
        displayPad = Pad;
    }
    else
#endif
    {
        displayPad = 32;
        int useDepth;

        if (displayDepth == 15)
            useDepth = 16;
        else
            useDepth = displayDepth;

#ifdef wx_x
        displayPitch = ((width * useDepth + 31) >> 3) & ~3;
        if ((modeData = (char*)malloc(displayPitch*height*sizeof(char))) == NULL)
        {
            errorMemory();
            return(-1);
        }
#endif
#ifdef wx_msw
        virtWidth = width;

        while (((displayPitch = virtWidth * useDepth) & 31) != 0) virtWidth++;
        displayPitch >>= 3;
        if ((modeData = (char*)malloc(displayPitch * height * sizeof(char))) == NULL)
        {
            errorMemory();
            return -1;
        }
#endif
        // *errorstr << "Allocated buffer of size " << width << '*' << height << ", " << displayPitch*height << endl;
    }

#ifdef wx_msw
    bmDesc.bmType = 0;
    bmDesc.bmWidth = virtWidth;
    bmDesc.bmHeight = height;
    bmDesc.bmWidthBytes = displayPitch;
    bmDesc.bmPlanes = 1;
    bmDesc.bmBitsPixel = displayDepth;
    bmDesc.bmBits = (LPVOID)modeData;
    bitmap = CreateBitmapIndirect(&bmDesc);
    if (bitmap == (HBITMAP)0)
    {
        errorMemory();
        return(-1);
    }
    // *errorstr << "bitmap handle = " << (long)bitmap << endl;
#endif

#ifdef wx_x
    // *errorstr << "Get Handle... " << endl;
    windowHandle = parentWin->GetXWindow();
    // *errorstr << "windowHandle = " << windowHandle << endl;

    // Use visualInfo.depth rather than displayDepth (24bpp vs 32bpp!)
    xim = XCreateImage(display, visualInfo.visual, visualInfo.depth, ZPixmap, 0, modeData, width, height, displayPad, displayPitch);

    // Get parent window attributes to get colourmap
    XGetWindowAttributes(display, windowHandle, &xat);
    //cout << "window depth " << xat.depth << endl;
    xgcvals.function = GXcopy;
    myGC = XCreateGC(display, windowHandle, GCFunction, &xgcvals);
#endif

#ifdef wx_msw
    windowHandle = parentWin->GetHWND();
    winDC = GetDC(windowHandle);
    srcDC = CreateCompatibleDC(winDC);
    //srcDC = CreateCompatibleDC(NULL);
    oldDCObject = SelectObject(srcDC, (HGDIOBJ)bitmap);
#endif

    initColoursForMode();

    // Now create the translation table if necessary (if depth, lastDepth > 8 we can use the
    // previous one.
    if (((depth != displayDepth) && (lastDepth <= 12)) || (depth <= 12))
    {
        if ((pixFlags & WX_PIXFLAG_SAMEPALETTE) == 0)
        {
            buildTranslationTable();
        }
    }
    // In case the fast dither table has to be built, call function too...
    else if ((FastDitherTab == NULL) && ((pixFlags & WX_PIXFLAG_FASTDITHER) != 0) && (displayDepth < 15))
    {
        buildTranslationTable();
    }

    // In case the pixmap should be dithered:
    if (((pixFlags & WX_PIXFLAG_DITHER) != 0) && (depth <= 12) && (displayDepth <= 8))
    {
        processPixmapPalette();
    }

    return(0);
}



#ifdef wx_msw
// Special Win hack
void wxPixmap::win_translate_24_to_24(unsigned char *dest)
{
    int i, j;
    unsigned char *b, *d, *bline, *dline;

    bline = (unsigned char*)data;
    dline = dest;
    for (i=0; i<height; i++, bline+=pitch, dline+=displayPitch)
    {
        b = bline;
        d = dline;
        if (rgbOrder == RGB_ORDER_RGB)
        {
            for (j=0; j<width; j++)
            {
                d[0] = b[0];
                d[1] = b[1];
                d[2] = b[2];
                d += 3;
                b += 3;
            }
        }
        else
        {
            for (j=0; j<width; j++)
            {
                d[0] = b[2];
                d[1] = b[1];
                d[2] = b[0];
                d += 3;
                b += 3;
            }
        }
    }
}
#endif


// It's a biggie: translate the data into the correct format for the current display.
void wxPixmap::translateToMode(void)
{
    const unsigned char *src = (unsigned char *)data;
    unsigned char *dest = (unsigned char *)modeData;
    int status = 0;

    if ((data == NULL) || (modeData == NULL) || (displayDepth <= 0)) return;

    // *errorstr << "translate mode " << depth << " to " << displayDepth << endl;
    if ((data != modeData) && ((pixFlags & WX_PIXFLAG_TRANSLATE) != 0))
    {
        switch (depth)
        {
        case 1:
            if (pixtrans1 != NULL) pixtrans1(src, dest, width, height, pitch, displayPitch, TransTab.c);
            break;
        case 2:
            if (pixtrans2 != NULL) pixtrans2(src, dest, width, height, pitch, displayPitch, TransTab.c);
            break;
        case 4:
            if (pixtrans4 != NULL) pixtrans4(src, dest, width, height, pitch, displayPitch, TransTab.c);
            break;
        case 8:
            if (pixtrans8 != NULL) pixtrans8(src, dest, width, height, pitch, displayPitch, TransTab.c);
            break;
        case 12:
            if (pixtrans12 != NULL) pixtrans12(src, dest, width, height, pitch, displayPitch, TransTab.c);
            break;
        case 15:
            if (pixtrans15 != NULL) pixtrans15(src, dest, width, height, pitch, displayPitch, TrueTransTab);
            break;
        case 24:
#ifdef wx_msw
            if (displayDepth == 24)
            {
                win_translate_24_to_24(dest);
                break;
            }
#endif
            if (pixtrans24 != NULL) pixtrans24(src, dest, width, height, pitch, displayPitch, TrueTransTab);
            break;
        case 32:
            if (pixtrans32 != NULL) pixtrans32(src, dest, width, height, pitch, displayPitch, TrueTransTab);
            break;
        default:
            status = -1;
            break;
        }

        // *errorstr << "Translate bitmap status " << status << endl;

        if (status < 0)
        {
#if (DEBUG > 0)
            *errorstr << "Can't translate from " << depth << " to " << displayDepth << endl;
#endif
        }
    }
    validDisplay = TRUE;
}


// Ditherer to display modes with <= 8bpp. Otherwise use the simple translator.
void wxPixmap::ditherToMode(void)
{
    int status=0;
    unsigned char *dest = (unsigned char *)modeData;

    if ((data == NULL) || (modeData == NULL) || (displayDepth <= 0)) return;

    /*for (int i=0; i<(1<<depth); i++)
    {
      *errorstr << '[' << (int)(pixmapPalette[i].red) << ',' << (int)(pixmapPalette[i].green) << ',' << (int)(pixmapPalette[i].blue) << ']';
    }
    *errorstr << endl;*/

    // *errorstr << "Dither mode " << depth << " to " << displayDepth << endl;
    if (displayDepth <= 8)
    {
        if ((busyCursorMode) && ((pixFlags & WX_PIXFLAG_FASTDITHER) == 0))
            ::wxBeginBusyCursor();

        switch (depth)
        {
        case 1:
            if (pixdither1 != NULL) (this->*pixdither1)(dest, displayPad);
            break;
        case 2:
            if (pixdither2 != NULL) (this->*pixdither2)(dest, displayPad);
            break;
        case 4:
            if (pixdither4 != NULL) (this->*pixdither4)(dest, displayPad);
            break;
        case 8:
            if (pixdither8 != NULL) (this->*pixdither8)(dest, displayPad);
            break;
        case 12:
            if (pixdither12 != NULL) (this->*pixdither12)(dest, displayPad);
            break;
        case 15:
            if (pixdither15 != NULL) (this->*pixdither15)(dest, displayPad);
            break;
        case 24:
            if (pixdither24 != NULL) (this->*pixdither24)(dest, displayPad);
            break;
        case 32:
            if (pixdither32 != NULL) (this->*pixdither32)(dest, displayPad);
            break;
        default:
            status = -1;
            break;
        }

        if ((busyCursorMode) && ((pixFlags & WX_PIXFLAG_FASTDITHER) == 0))
            ::wxEndBusyCursor();

        if (status < 0)
        {
#if (DEBUG > 0)
            *errorstr << "Can't dither from " << depth << " to " << displayDepth << endl;
#endif
        }
    }
    validDisplay = TRUE;
}


int wxPixmap::plotPixmap(int PosX, int PosY)
{
    // *errorstr << "plotPixmap: parentWin=" << (long)parentWin << ", data=" << (long)data << ", modeData=" << (long)modeData << endl;
    if ((parentWin != NULL) && (data != NULL))
    {
        if (parentWin->IsShown())
        {
            // Do we have to do a colour translation?
            if (!validDisplay)
            {
                if (data != modeData)
                {
                    if (((pixFlags & WX_PIXFLAG_DITHER) != 0) && (displayDepth <= 8))
                    {
                        ditherToMode();
                    }
                    else if ((pixFlags & WX_PIXFLAG_TRANSLATE) != 0)
                    {
                        translateToMode();
                    }
                }
#ifdef wx_msw
                // I'd really like to avoid this but Windoze gets in the way
                SetBitmapBits(bitmap, displayPitch * height, modeData);
#endif
            }
#ifdef wx_x
            /*cout << "width = " << xim->width << ", pitch = " << xim->bytes_per_line << ", bpp = " << xim->bits_per_pixel << ", depth = " << xim->depth << ", pad = " << xim->bitmap_pad << endl;
            cout << "format = " << xim->format << ", byte_order = " << xim->byte_order << ", bitmap_unit = " << xim->bitmap_unit << ", bitmap_bit_order = " << xim->bitmap_bit_order << ", rmask = " << xim->red_mask << ", gmask = " << xim->green_mask << ", bmask = " << xim->blue_mask << endl;*/
            XPutImage(display, windowHandle, myGC, xim, 0, 0, PosX, PosY, width, height);
#endif

#ifdef wx_msw
            if (BitBlt(winDC, PosX, PosY, width, height, srcDC, 0, 0, SRCCOPY) == 0)
                return -1;
#endif
        }
        return 0;
    }
    else
    {
        errorGeneric("Window or image definition missing!");
    }
    return -1;
}


void wxPixmap::invalidatePixmap(void)
{
    validDisplay = FALSE;
}



/*
 *  re-build the true-colour --> colourmapped table and the fast ditherer table
 *  (in case another application modified a colour and the display goes wrong).
 */
void wxPixmap::refreshGlobalTables(void)
{
    int newTables=0;

    if ((depth > 12) && (displayDepth <= 8))
    {
        newTables |= 1;
        if (TrueTransTab != NULL)
        {
            delete [] TrueTransTab;
            TrueTransTab = NULL;
            TrueTransCount--;
        }
    }
    if ((pixFlags & WX_PIXFLAG_FASTDITHER) != 0)
    {
        newTables |= 2;
        if (FastDitherTab != NULL)
        {
            delete [] FastDitherTab;
            FastDitherTab = NULL;
            FastDitherCount--;
        }
    }
    if (newTables != 0)
        buildTranslationTable();
}




/*
 *  Display the busy mouse pointer when an operation takes longer (e.g. dithering)...
 */
bool wxPixmap::setBusyCursor(bool newMode)
{
    bool oldMode = busyCursorMode;
    busyCursorMode = newMode;
    return oldMode;
}


// Finally compile the auto-generated ditherers
#include "wx_pixmap_dither.cpp"
