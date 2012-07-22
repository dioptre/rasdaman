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
 *  rView thumbnail viewer class. Unlike the regular MDD viewers (e.g.
 *  rviewImage) this is not derived from rviewDisplay because it can
 *  display any number of MDD objects in contrast to regular viewers
 *  which manage exactly one object.
 *
 *  COMMENTS:
 *      None
 */

/**
*   @file rviewThumb.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_THUMB_H_
#define _RVIEW_THUMB_H_




#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/gmarray.hh"

#include "labelManager.hh"
#include "rviewUtils.hh"
#include "rviewDisplay.hh"
#include "rviewDModes.hh"




class wxPixmap;
class rviewThumb;


// List holding all the pixmap items for one mddObject.
// This chain is 1 item long for 2D data, but can be any length
// for higher dimensions
typedef struct rviewThumbPixList
{
    wxPixmap *pixmap;
    int dimproj, projval;
    rviewThumbPixList *next;
} rviewThumbPixList;

// List holding mdd/pixmap items
typedef struct rviewThumbList
{
    r_Ref<r_GMarray> mdd;
    rviewBaseType baseType;
    int numPix;
    rviewThumbPixList *pixmaps;
    rviewThumbList *next;
} rviewThumbList;




/*
 *  The canvas displaying the thumbnails
 */
class thumbCanvas: public wxCanvas
{
public:

    thumbCanvas(rviewThumb *par, int x, int y, int width, int height);
    ~thumbCanvas(void);

    void OnPaint(void);
    void updateDisplay(void);


protected:

    wxBrush brush;
    rviewThumb *parent;
};



/*
 *  A window containing small versions of images
 */
class rviewThumb: public rviewFrame
{
public:

    rviewThumb(void);
    ~rviewThumb(void);

    int addMDD(r_Ref<r_GMarray> &newMdd);
    int deleteMDD(r_Ref<r_GMarray> &obsMdd);
    void setLayout(int width, int npl);
    void newThumbWidth(int newWidth);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);
    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    void OnSize(int w, int h);
    void OnMenuCommand(int id);

    int userEvent(const user_event &ue);

    // Used by canvas to get data
    wxPixmap *getPixmapNumber(int no, char *caption);
    void getThumbInfo(int &num, int &npl);
    void getGridInfo(int &gx, int &gy);

    // constants
    // Initial dimensions of window
    static const int thumb_width;
    static const int thumb_height;
    // Default width of thumbnail images
    static const int thumb_imgwidth;
    // Default number of thumbnails per line
    static const int thumb_perline;
    // Space between thumbnails
    static const int thumb_space;
    // Borders used in thumbnail window
    static const int thumb_border;
    // Scrolling values
    static const int thumb_scrstep;
    static const int thumb_pgstep;
    // Dimensions of checkboxs
    static const int thumb_chkwidth;
    static const int thumb_chkheight;
    // Minimum / maximum width of thumbnails
    static const int thumb_minwidth;
    static const int thumb_maxwidth;
    // Minimum / maximum number of thumbnails per line
    static const int thumb_mincols;
    static const int thumb_maxcols;
    // Height of control panel at the top
    static const int thumb_cheight;
    // Height of text items
    static const int thumb_twidth;
    static const int thumb_theight;
    // Width of projDim / step widgets
    static const int thumb_prjwidth;


protected:

    void deletePixmapChain(rviewThumbList *tlst);
    int pixmapsFromMDD(rviewThumbList *tlst);
    wxPixmap *buildThumbnail(r_Ref<r_GMarray> &mddObj, rviewBaseType baseType, int dimproject, int projval);
    void updateCanvasSize(void);
    void rebuildThumbnails(bool fromScratch);
    void initForObject(r_Ref<r_GMarray> &mddObj);
    int parseProjection(r_Ref<r_GMarray> &mddObj);
    void configureCspace(bool mode);

    char projString[STRINGSIZE];
    r_Point pt1, pt2, mapIndex;
    int thumbs, thumbsperline, numPixmaps;
    int maxHeight;
    int gridX, gridY;
    int imgWidth;
    int dimMDD;       // should be constant for all objects!
    int dim1, dim2;   // dimensions to iterate over
    int dimproj;      // projection dim for 3+D objects
    int projstep;     // stepping value in dimproj
    unsigned int freeDims;
    rviewThumbList *listHead;
    thumbCanvas *canvas;
    wxMenuBar *mbar;
    wxPanel *panel;
    rviewText *thumbWidth;
    rviewText *thumbCols;
    rviewText *project;
    rviewText *thumbProj, *thumbStep;
    wxFont *font;
    colourspaceMapper *csmap;
    bool doValToCspace;
    bool doFullRangeCspace;
    bool canDoCspace;
};

#endif
