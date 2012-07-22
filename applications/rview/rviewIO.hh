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
 *
 *  COMMENTS:
 *          None
 */

/**
*   @file rviewIO.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_IO_H_
#define _RVIEW_IO_H_


#ifdef __GNUG__
#pragma interface
#endif



#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/gmarray.hh"

#include "rviewUtils.hh"



// Return status
enum rviewIoStates
{
    RVIEW_IO_OK,
    RVIEW_IO_NOTFOUND,
    RVIEW_IO_MEMORY,
    RVIEW_IO_UNSUPP,
    RVIEW_IO_FORMAT
};


// pixmap conversion functions
class wxPixmap;

// parameter parser
class r_Parse_Params;


class rviewIO
{
public:

    rviewIO(void);
    ~rviewIO(void);

    static void terminate(void);

    static int isTIFF(const char *filename);
    static int loadTIFF(const char *filename, r_Ref<r_GMarray> &mddPtr, const char *params=NULL);
    static int saveTIFF(const char *filename, r_Ref<r_GMarray> &mddPtr, const char *params=NULL);
    static int PixmapToTIFF(wxPixmap *pixmap, const char *filename, const char *params=NULL);
    static int isVFF(const char *filename);
    static int loadVFF(const char *filename, r_Ref<r_GMarray> &mddPtr, const char *params=NULL);
    static int saveVFF(const char *filename, r_Ref<r_GMarray> &mddPtr, const char *params=NULL);


private:

    static void ensureParams(void);
    static void processParams(const char *params);

    static r_Parse_Params *dfltParams;
    static char *tiffCompStr;
    static int tiffCompression;

    static const char *getExtension(const char *filename);

    // tiff compression keywords
    static const char param_KeyTiffComp[];
    static const char param_TiffCompNone[];
    static const char param_TiffCompPack[];
    static const char param_TiffCompLZW[];
    static const char param_TiffCompZLib[];
    static const char param_TiffCompJPEG[];

    // TIFF formats
    static const char structure_format_mono[];
    static const char structure_format_grey[];
    static const char structure_format_rgb[];

    // VFF formats
    static const char structure_format_cube8[];
    static const char structure_format_cube16[];
    static const char structure_format_cube32[];
};

#endif
