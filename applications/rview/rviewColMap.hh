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
 *      Class definitions for the colourspace mapping.
 *  COMMENTS:
 *      No comments
 */

/**
*   @file rviewColMap.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_COLMAP_H_
#define _RVIEW_COLMAP_H_

#include "rviewUtils.hh"

class colourspaceMapper;
class colourspaceFrame;


enum cspaceType
{
    cst_gauss,
    cst_linear,
    cst_rectangle,
    cst_asympt
};

typedef struct colourspace_params_s
{
    double peak_red, peak_green, peak_blue;
    double sigm_red, sigm_green, sigm_blue;
    double minVal, maxVal;
    unsigned long floatRange;
    cspaceType type;
} colourspace_params;


/*
 *  Colourspace configuration canvas
 */
class colourspaceCanvas: public wxCanvas
{
public:

    colourspaceCanvas(colourspaceFrame *parent, colourspace_params *p, int x, int y, int w, int h, long style=0);
    ~colourspaceCanvas(void);

    void enableOutlineSum(bool enable);
    void setDrawingFunction(void);

    void Redraw(void);

    void OnSize(int w, int h);
    void OnPaint(void);
    void OnEvent(wxMouseEvent &mevt);

    // constants
    // Colourspace canvas border
    static const int colcanv_cborder;
    // Height of markers on colourspace canvas
    static const int colcanv_mheight;


protected:

    int setupRectangle(int &from, int &to, float &x, wxRect *rect);
    void drawOutline(double peak, double sigma, wxPen *pen, wxRect *rect);
    void drawOutlineSum(wxPen *pen, wxRect *rect);
    // Function pointer
    double (*conversionFunction)(double, double, double);

    colourspaceFrame *parentObj;
    colourspace_params *params;
    int canvX, canvY;
    wxBrush brush;
    wxPen redPen, greenPen, bluePen, blackPen;
    wxFont *font;
    float *values;
    float height, base, step;
    int cmin, cmax;
};


/*
 *  Colourspace configuration window
 */
class colourspaceFrame: public rviewFrame
{
public:

    colourspaceFrame(colourspaceMapper *parent, const colourspace_params *p);
    ~colourspaceFrame(void);

    void setRange(double newMinVal, double newMaxVal);
    void unlinkParent(void);

    void OnSize(int w, int h);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);

    void updateSettings(void);
    void updateDisplay(const colourspace_params *cp=NULL);
    void processMouseEvent(wxMouseEvent &mevt);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // constants
    // Colourspace editor borders
    static const int colspc_border;
    // Colourspace editor window
    static const int colspc_width;
    static const int colspc_height;
    // Colourspace buttons
    static const int colspc_bwidth;
    static const int colspc_bheight;
    // Colourspace text widgets
    static const int colspc_twidth;
    static const int colspc_theight;
    // Colourspace checkbox widgets
    static const int colspc_chkheight;
    // Colourspace choice widgets
    static const int colspc_chwidth;
    static const int colspc_chheight;
    // Colourspace control panel height
    static const int colspc_cheight;


private:

    void makeUpdate(void);

    colourspaceMapper *parentObj;
    colourspace_params newParams;
    colourspace_params origParams;    // in case of immediate update

    colourspaceCanvas *canvas;
    wxPanel *panel;
    rviewText *posR, *posG, *posB;
    rviewText *sigR, *sigG, *sigB;
    rviewButton *okBut, *cancelBut, *defaultBut;
    rviewCheckBox *immediateUpdate;
    rviewCheckBox *drawSum;
    rviewText *minVal, *maxVal;
    rviewChoice *csType;
    bool doImmediateUpdate;
    bool doDrawSum;
    cspaceType cstype;
    float mousex, mousey;
    int mousebut;
    int dragColour;
    int canvX, canvY;
    int didUpdate;
};


// modes for calling processRange
#define CSPACE_RANGE_ACTUAL 0
#define CSPACE_RANGE_FULL   1
#define CSPACE_RANGE_OLD    2

/*
 *  Class for mapping large range values to RGB colourspace,
 *  implemented in rviewImage.cpp
 */
class colourspaceMapper
{
public:

    colourspaceMapper(r_Ref<r_GMarray> &mdd, rviewBaseType bt, const colourspace_params *cp, bool fullrange=FALSE, const r_Minterval *domain=NULL, unsigned long frange=0x10000);
    ~colourspaceMapper(void);

    void getObject(r_Ref<r_GMarray> &mdd, rviewBaseType &bt, bool *fullrange=NULL, r_Minterval **domain=NULL) const;
    int bindMapper(r_Ref<r_GMarray> &mdd, rviewBaseType bt, bool fullrange=FALSE, const r_Minterval *domain=NULL, const colourspace_params *cp=NULL);

    inline unsigned short ValToCS15(double value)
    {
        return (this->*convert15)(value);
    }
    inline unsigned long ValToCS24(double value)
    {
        return (this->*convert24)(value);
    }
    unsigned short *buildCSTab15(bool forceRebuild=FALSE);
    unsigned long *buildCSTab24(bool forceRebuild=FALSE);
    double getMinVal(void);
    double getMaxVal(void);
    double getScalingFactor(void);
    unsigned short *getCSTab15(void);
    unsigned long *getCSTab24(void);
    void processRange(int rangeMode);
    void updateProjection(const r_Minterval *domain);
    void colourspaceChanged(const colourspace_params *newParams, bool autoUpdate=TRUE);
    void openEditor(void);
    void closeEditor(bool activeClose=TRUE);
    void getParameters(colourspace_params *dest);
    void setMappingFunctions(void);


protected:

    static int getTableForType(rviewBaseType bt);

    unsigned short ValToGauss15(double value);
    unsigned long  ValToGauss24(double value);
    unsigned short ValToLinear15(double value);
    unsigned long  ValToLinear24(double value);
    unsigned short ValToRectangle15(double value);
    unsigned long  ValToRectangle24(double value);
    unsigned short ValToAsymptotic15(double value);
    unsigned long  ValToAsymptotic24(double value);
    // function pointers
    unsigned short (colourspaceMapper::*convert15)(double value);
    unsigned long  (colourspaceMapper::*convert24)(double value);

    colourspace_params par;       // all the important parameters
    double realMinVal, realMaxVal;    // actual range of object
    bool didRange;
    bool rangeModeFull;
    double peakR, peakG, peakB;
    double invSigR, invSigG, invSigB;
    double scalingFactor;
    int dimMDD;
    int tableKind;    // 15 or 24bpp tables?
    unsigned short *IntToRGBTab15;
    unsigned long *IntToRGBTab24;
    cspaceType tableType;
    colourspaceFrame *csFrame;
    r_Ref<r_GMarray> mddObj;
    rviewBaseType baseType;
    long projPixels;
    r_Minterval objInterv;
    r_Minterval lastInterv;
    const r_Minterval *useInterv;
};

#endif
