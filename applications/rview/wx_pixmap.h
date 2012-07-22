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


#ifndef wx_pixmap_h
#define wx_pixmap_h


// should be in <wx/defs.h>, but only for OS/2 (?): -- PB 2006-jan-01
typedef unsigned long COLORREF;

// #include "wx_stat.h"
#include <wx/generic/statusbr.h>

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/object.h"

// changed in new wxWindows
//#include "wx_prec.h"
#include "wx/wxprec.h"




/* Weights for colour matching */
#define COLOUR_WEIGHT_RED   4
#define COLOUR_WEIGHT_GREEN 9
#define COLOUR_WEIGHT_BLUE  1


/* No changes below this line should be necessary. */


/* Flag bits for initialising functions*/
#define WX_PIXFLAG_TRANSLATE        1   /* class-resident mode translation enabled? */
#define WX_PIXFLAG_DITHER       2   /* Dither image. Overrides WX_PIXFLAG_TRANSLATE. */
#define WX_PIXFLAG_SAMEPALETTE      4   /* The palette is the same as the old one */
#define WX_PIXFLAG_FASTDITHER       8   /* Fast rather than best ditherer */
#define WX_PIXMODE_PALETTE      1   /* force making new colour tables on a mode change */



/* Structure for a permuted colour map (for more efficient colour matching) */
typedef struct wx_permute_cmap
{
    unsigned char red, green, blue, number;
} wx_permute_cmap;




/*
 *  Translation tables for true-colour-pixmaps to <= 8bpp modes and fast ditherer.
 *  Shared! Define WX_PIXMAP_GLOBAL_VOLATILE to free global translation tables
 *  as soon as no more references to it exist. Default is to keep them.
 */
extern unsigned char *TrueTransTab;
extern unsigned char *FastDitherTab;
extern int TrueTransCount;
extern int FastDitherCount;



class wxPixmap;

/* Type of pixmap translation function */
typedef void (*wx_pixmap_translate)(const unsigned char *src, unsigned char *dest, int width, int height, int srcPitch, int destPitch, const unsigned char *tt);
/* Type of pixmap dithering function */
typedef void (wxPixmap::*wx_pixmap_dither)(unsigned char *dest, int destPad);
/* Type of colour-matching functions (used in ditherers) */
typedef int (wxPixmap::*pixmap_colour_match)(unsigned char r, unsigned char g, unsigned char b);


/*
 *  Class for efficiently displaying raster images of any depth, initialising
 *  them from binary data.
 *  The macro wx_pixmap_alloc_cols distinguishes between two colour models.
 *  If it's defined the colours are allocated (X), otherwise the current
 *  colourmap is read and processed internally (much faster and usually
 *  much more accurate).
 */

class WXDLLEXPORT wxPixmap : public wxObject
{
public:
    /* Public member functions */
    wxPixmap(void);
    wxPixmap(wxWindow *Win, int Width, int Height, int Depth, int Pad, char *Data, unsigned int Flags=WX_PIXFLAG_TRANSLATE, wxColour *Palette=NULL);
    ~wxPixmap(void);
    int newPixmap(wxWindow *Win, int Width, int Height, int Depth, int Pad, char *Data, unsigned int Flags=WX_PIXFLAG_TRANSLATE, wxColour *Palette=NULL);
    int plotPixmap(int PosX, int PosY);
    void invalidatePixmap(void);
    int getWidth(void);
    int getHeight(void);
    int getDepth(void);
    int getPitch(void);
    char *getData(void);
    wxColour *getPalette(void);
    int getModeDepth(void);
    int getModePitch(void);
    char *getModeData(void);
    unsigned char *getTranslationTable(void);
    void modeChange(unsigned int Flags);
    void buildTranslationTable(void);
    void refreshGlobalTables(void);
    bool setBusyCursor(bool newMode);
#ifndef wx_pixmap_alloc_cols
    void processParentPalette(bool forceUpdate=FALSE);
    void processPixmapPalette(void);
    wx_permute_cmap *getParentPalette(void);
    int findBestColour(unsigned char red, unsigned char green, unsigned char blue);
    int findFastColour(unsigned char red, unsigned char green, unsigned char blue);
    /* Include auto-generated ditherer headers; these need the colour maps set up
       correctly, so it only works with wx_pixmap_alloc_cols not defined. */
#include "wx_pixmap_dither.h"
#endif

protected:
    /* protected member functions */
    void errorMemory(void);
    void errorGeneric(char *message);
    void initVariables(void);
    void freeResources(int Depth, unsigned int Flags);
    void translateToMode(void);
    void ditherToMode(void);
    void getDisplayAttributes(void);
    void initColoursForMode(bool forceUpdate=FALSE);
    void setupTranslators(void);
#ifndef wx_pixmap_alloc_cols
    void sortParentPalette(int from, int to); /* Quicksorter */
    void buildInverseTable(void);
#endif

    /* protected variables */
    int width, height, depth, pad, pitch;     /* parameters for image */
    int displayDepth, displayPitch, displayPad;   /* parameters for current display */
    char *data;       /* Raw source data */
    char *modeData;   /* Translated data suitable for plotting */
    union
    {
        unsigned char *c;
        COLORREF *l;
    } TransTab;  /* Colour translation table. */
    /* COLORREF is defined in wxWindows */
    int TransTabSize;
    wxColour *palette;    /* Source data palette */
    wxWindow *parentWin;  /* The parent window date should be plotted to */
    unsigned int pixFlags;
#ifdef wx_pixmap_alloc_cols
    char ColourAlloc[32]; /* bitfield (256 bits). Bit i set ==> X-colour #i allocated OK. */
    /* only used for displayDepth <= 8. */
#else
    int parentPaletteDepth;
    wx_permute_cmap *parentPalette, *pixmapPalette;
    unsigned char parentInverse[256];
#endif
    bool validDisplay;
    bool busyCursorMode;
    // wxDebugStreamBuf streamBuf; -- PB 2006-jan-01
    wxOutputStream streamBuf;
    std::ostream *errorstr;
    int destBitorder, destByteorder, rgbOrder;
    wx_pixmap_translate pixtrans1, pixtrans2, pixtrans4, pixtrans8, pixtrans12;
    wx_pixmap_translate pixtrans15, pixtrans24, pixtrans32;
    wx_pixmap_dither pixdither1, pixdither2, pixdither4, pixdither8, pixdither12;
    wx_pixmap_dither pixdither15, pixdither24, pixdither32;
    pixmap_colour_match colour_matcher;

#ifdef wx_x
    /* System Specifics */
    Display *display;
    XVisualInfo visualInfo;
    XImage *xim;
    XGCValues xgcvals;
    XWindowAttributes xat;
    GC myGC;
    int screen;
    Window rootwin, windowHandle;
#endif

#ifdef wx_msw
    void win_translate_24_to_24(unsigned char *dest);

    BITMAP bmDesc;
    HBITMAP bitmap;
    HWND windowHandle;
    HWND desktop;
    HDC rootDC, winDC, srcDC;
    HGDIOBJ oldDCObject;
    HPALETTE currentPal;
#endif
};


#endif
