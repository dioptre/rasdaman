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
 *      Colourspace mapper class and support classes.
 *  COMMENTS:
 *      No comments
 */



// Standard wxWindows preamble.
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



#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <limits.h>
#include <float.h>


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif


#include "raslib/rmdebug.hh"

#include "wx_pixmap_translate.h"

#include "rviewUtils.hh"
#include "labelManager.hh"
#include "rviewColMap.hh"
#include "rviewPrefs.hh"
#include "rviewMDD.hh"





/*
 *  Conversion functions shared by colourspaceCanvas and colourspaceMapper:
 *  convert a value normalized to [0,1] to a value normalized to [0,1]
 */

static inline double valueToGauss(double value, double peak, double invSig)
{
    double h;

    h = (value - peak) * invSig;
    return exp(-h*h);
}

static inline double valueToLinear(double value, double peak, double invSig)
{
    double h;

    if (value < peak)
        h = 1.0 - invSig*(peak - value);
    else
        h = 1.0 - invSig*(value - peak);

    if (h < 0) h = 0;

    return h;
}

static inline double valueToRectangle(double value, double peak, double invSig)
{
    double h;

    if (value < peak)
        h = 1.0 - invSig*(peak - value);
    else
        h = 1.0 - invSig*(value - peak);

    if (h > 0) h = 1.0;
    else h = 0.0;

    return h;
}

static inline double valueToAsymptotic(double value, double peak, double invSig)
{
    double h;

    if (value < peak) h = 0;
    else
        h = 1.0 - exp((peak - value) * invSig);

    return h;
}



/*
 *  Colourspace mapper editor canvas class.
 */

const int colourspaceCanvas::colcanv_cborder = 8;
const int colourspaceCanvas::colcanv_mheight = 8;

colourspaceCanvas::colourspaceCanvas(colourspaceFrame *parent, colourspace_params *p, int x, int y, int w, int h, long style) : wxCanvas((wxWindow*)parent, x, y, w, h, style)
{
    params = p;
    canvX = w;
    canvY = h;
    parentObj = parent;
    canvX=100;
    canvY=100;
    values = NULL;

    setDrawingFunction();

    brush.SetStyle(wxSOLID);
    brush.SetColour((char)0xe0, (char)0xe0, (char)0xe0);
    redPen.SetStyle(wxSOLID);
    redPen.SetColour((char)0xff, 0, 0);
    greenPen.SetStyle(wxSOLID);
    greenPen.SetColour(0, (char)0xff, 0);
    bluePen.SetStyle(wxSOLID);
    bluePen.SetColour(0, 0, (char)0xff);
    blackPen.SetStyle(wxSOLID);
    blackPen.SetColour(0, 0, 0);

    font = new wxFont(12, wxROMAN, wxNORMAL, wxNORMAL);

    SetBackground(&brush);
    SetFont(font);
}


colourspaceCanvas::~colourspaceCanvas(void)
{
    SetFont(NULL);
    SetBackground(NULL);
    delete font;
    if (values != NULL) delete [] values;
}


void colourspaceCanvas::setDrawingFunction(void)
{
    switch (params->type)
    {
    case cst_gauss:
        conversionFunction = valueToGauss;
        break;
    case cst_linear:
        conversionFunction = valueToLinear;
        break;
    case cst_rectangle:
        conversionFunction = valueToRectangle;
        break;
    case cst_asympt:
        conversionFunction = valueToAsymptotic;
        break;
    default:
        cerr << "Unknown drawing function (" << (int)(params->type) << ")" << endl;
        conversionFunction = NULL;
        break;
    }
    //cout << "cspace type " << (int)(params->type) << endl;
}


void colourspaceCanvas::enableOutlineSum(bool enable)
{
    if (enable)
    {
        if (values == NULL)
        {
            values = new float[canvX];
            Redraw();
        }
    }
    else
    {
        if (values != NULL)
        {
            delete [] values;
            values = NULL;
            Redraw();
        }
    }
}


void colourspaceCanvas::OnSize(int w, int h)
{
    canvX = w;
    canvY = h;
    height = (float)(canvY - 2*colcanv_cborder);
    base = (float)(canvY - colcanv_cborder);
    step = 1.0 / (canvX - 2*colcanv_cborder);
    cmin = colcanv_cborder;
    cmax = canvX - colcanv_cborder;
    if (values != NULL)
    {
        delete [] values;
        values = new float[canvX];
    }
}


void colourspaceCanvas::Redraw(void)
{
    Refresh(TRUE);
}


void colourspaceCanvas::OnPaint(void)
{
    wxRect rect;
    int y = canvY - colcanv_cborder;
    float x, xhigh, markstep;

    markstep = (canvX - 2*colcanv_cborder) / 10.0;
    xhigh = (float)(canvX - colcanv_cborder) + 0.9;

    BeginDrawing();

    wxUpdateIterator upd(this);
    while (upd)
    {
        upd.GetRect(&rect);

        if (values != NULL)
            memset(values, 0, canvX*sizeof(float));

        if (conversionFunction != NULL)
        {
            drawOutline(params->peak_red, params->sigm_red, &redPen, &rect);
            drawOutline(params->peak_green, params->sigm_green, &greenPen, &rect);
            drawOutline(params->peak_blue, params->sigm_blue, &bluePen, &rect);
        }
        if (values != NULL)
            drawOutlineSum(&blackPen, &rect);

        SetPen(&blackPen);
        IntDrawLine(colcanv_cborder, y, canvX - colcanv_cborder, y);

        for (x=(float)colcanv_cborder; x<=xhigh; x += markstep)
        {
            IntDrawLine((int)x, y + colcanv_mheight/2, (int)x, y - colcanv_mheight/2);
        }
        upd++;
    }
    SetPen(NULL);

    EndDrawing();
}


void colourspaceCanvas::OnEvent(wxMouseEvent &mevt)
{
    parentObj->processMouseEvent(mevt);
}


int colourspaceCanvas::setupRectangle(int &from, int &to, float &x, wxRect *rect)
{
    if (rect == NULL)
    {
        from = cmin;
        from = cmax;
        x = 0.0;
    }
    else
    {
        from = rect->x;
        to = from + rect->width + 1;
        if (((from < cmin) && (to < cmin)) || ((from > cmax) && (to > cmax)))
            return -1;
        if (from < cmin) from = cmin;
        if (to > cmax) to = cmax;
        x = (from - cmin) * step;
    }
    return 0;
}


void colourspaceCanvas::drawOutline(double peak, double sigma, wxPen *pen, wxRect *rect)
{
    float mid = (canvX - 2*colcanv_cborder) * peak + colcanv_cborder;
    float x, y, lastY;
    float invSigma;
    int i, j;

    SetPen(pen);

    DrawLine(mid, base, mid, 0.0);

    invSigma = 1.0 / sigma;

    if (setupRectangle(i, j, x, rect) != 0) return;

    lastY = height * conversionFunction(x, peak, invSigma);
    if (values != NULL)
        values[i] += lastY;

    for (i++, x += step; i < j; i++, x+=step)
    {
        y = height * conversionFunction(x, peak, invSigma);
        DrawLine((float)i-1, base - lastY, (float)i, base - y);
        lastY = y;
        if (values != NULL)
            values[i] += y;
    }
}


void colourspaceCanvas::drawOutlineSum(wxPen *pen, wxRect *rect)
{
    int i, j;
    float x, y, lastY;

    pen->SetStyle(wxSHORT_DASH);
    SetPen(pen);
    DrawLine((float)colcanv_cborder, base - height/3, (float)(canvX - colcanv_cborder), base - height/3);
    pen->SetStyle(wxSOLID);
    SetPen(pen);

    if (setupRectangle(i, j, x, rect) != 0) return;

    lastY = values[i] / 3;
    for (i++, x += step; i < j; i++, x+= step)
    {
        y = values[i] / 3;
        DrawLine((float)i-1, base - lastY, (float)i, base - y);
        lastY = y;
    }
}






/*
 *  Colourspace mapper editor frame class.
 */

const int colourspaceFrame::colspc_border = 8;
const int colourspaceFrame::colspc_width = 450;
const int colourspaceFrame::colspc_height = 400;
const int colourspaceFrame::colspc_bwidth = 80;
const int colourspaceFrame::colspc_bheight = 30;
const int colourspaceFrame::colspc_twidth = 100;
const int colourspaceFrame::colspc_theight = 30;
const int colourspaceFrame::colspc_chkheight = 20;
const int colourspaceFrame::colspc_chwidth = colourspaceFrame::colspc_twidth - rview_choice_sub_width;
const int colourspaceFrame::colspc_chheight = 20;
const int colourspaceFrame::colspc_cheight = colourspaceFrame::colspc_bheight + 4*colourspaceFrame::colspc_theight + 5*colourspaceFrame::colspc_border;


colourspaceFrame::colourspaceFrame(colourspaceMapper *parent, const colourspace_params *p) : rviewFrame(NULL, lman->lookup("titleColourspace"), 0, 0, colspc_width, colspc_height)
{
    // defaults
    doImmediateUpdate = TRUE;
    doDrawSum = TRUE;

    parentObj = parent;
    memcpy(&newParams, p, sizeof(colourspace_params));
    memcpy(&origParams, p, sizeof(colourspace_params));
    canvX = 0;
    canvY = 0;
    mousebut = 0;
    didUpdate = 0;

    canvas = new colourspaceCanvas(this, &newParams, 0, 0, 100, 100, wxBORDER);
    panel = new wxPanel((wxWindow*)this, 0, 100, 100, 100);

    okBut = new rviewButton(panel);
    cancelBut = new rviewButton(panel);
    defaultBut = new rviewButton(panel);

    immediateUpdate = new rviewCheckBox(panel);
    immediateUpdate->SetValue(doImmediateUpdate);
    drawSum = new rviewCheckBox(panel);
    drawSum->SetValue(doDrawSum);

    // the text widgets displaying the current setup
    posR = new rviewText(panel);
    posG = new rviewText(panel);
    posB = new rviewText(panel);
    sigR = new rviewText(panel);
    sigG = new rviewText(panel);
    sigB = new rviewText(panel);
    minVal = new rviewText(panel);
    maxVal = new rviewText(panel);

    const char *cstypes[4];
    cstypes[0] = lman->lookup("cspaceTypeGauss");
    cstypes[1] = lman->lookup("cspaceTypeLin");
    cstypes[2] = lman->lookup("cspaceTypeRect");
    cstypes[3] = lman->lookup("cspaceTypeAsympt");
    csType = new rviewChoice(panel, 4, cstypes);
    csType->SetLabel("");

    updateDisplay();

    Show(TRUE);

    label();

    frameWidth = -1;
    frameHeight = -1;
    OnSize(colspc_width, colspc_height);

    canvas->enableOutlineSum(doDrawSum);
}


colourspaceFrame::~colourspaceFrame(void)
{
    if (parentObj != NULL) parentObj->closeEditor(FALSE);
}


const char *colourspaceFrame::getFrameName(void) const
{
    return "colourspaceFrame";
}

rviewFrameType colourspaceFrame::getFrameType(void) const
{
    return rviewFrameTypeCspace;
}


void colourspaceFrame::unlinkParent(void)
{
    parentObj = NULL;
}


void colourspaceFrame::setRange(double newMinVal, double newMaxVal)
{
    char buffer[STRINGSIZE];

    newParams.minVal = newMinVal;
    newParams.maxVal = newMaxVal;
    // Just update the text widgets. Rebuilding the pixmap is done automatically.
    sprintf(buffer, "%f", newMinVal);
    minVal->SetValue(buffer);
    sprintf(buffer, "%f", newMaxVal);
    maxVal->SetValue(buffer);
}


void colourspaceFrame::OnSize(int w, int h)
{
    int x, y, px, py;

    GetClientSize(&x, &y);
    if ((x == frameWidth) && (y == frameHeight))
        return;

    frameWidth = x;
    frameHeight = y;
    y -= colspc_cheight;
    canvX = x - 2*colspc_border;
    canvY = y - 2*colspc_border;
    canvas->SetSize(colspc_border, colspc_border, canvX, canvY);
    panel->SetSize(0, y, x, colspc_cheight);
    px = (x - 3*colspc_twidth) / 5;
    if(px<0) px=0;
    py = colspc_border;
    posR->SetSize(px, py, colspc_twidth, colspc_theight);
    posG->SetSize(2*px + colspc_twidth, py, colspc_twidth, colspc_theight);
    posB->SetSize(3*px + 2*colspc_twidth, py, colspc_twidth, colspc_theight);
    py += colspc_border + colspc_theight;
    sigR->SetSize(px, py, colspc_twidth, colspc_theight);
    sigG->SetSize(2*px + colspc_twidth, py, colspc_twidth, colspc_theight);
    sigB->SetSize(3*px+ 2*colspc_twidth, py, colspc_twidth, colspc_theight);
    py += colspc_border + colspc_theight;
    px = (x - 3*colspc_twidth)/4;
    immediateUpdate->SetSize(px, py, colspc_twidth, colspc_chkheight);
    drawSum->SetSize(px, py + colspc_chkheight, colspc_twidth, colspc_chkheight);
    minVal->SetSize(2*px + colspc_twidth, py, colspc_twidth, colspc_theight);
    maxVal->SetSize(2*px + colspc_twidth, py + colspc_theight, colspc_twidth, colspc_theight);
    csType->SetSize(3*px + 2*colspc_twidth, py, colspc_chwidth, colspc_chheight);
    py += colspc_border + 2*colspc_theight;
    px = (x - 3*colspc_bwidth)/3;
    okBut->SetSize(px/2, py, colspc_bwidth, colspc_bheight);
    cancelBut->SetSize((3*px)/2 + colspc_bwidth, py, colspc_bwidth, colspc_bheight);
    defaultBut->SetSize((5*px)/2 + 2*colspc_bwidth, py, colspc_bwidth, colspc_bheight);
}


void colourspaceFrame::label(void)
{
    okBut->SetLabel(lman->lookup("textOK"));
    cancelBut->SetLabel(lman->lookup("textCancel"));
    defaultBut->SetLabel(lman->lookup("textDefaults"));

    immediateUpdate->SetLabel(lman->lookup("cspaceImmUpdt"));
    drawSum->SetLabel(lman->lookup("cspaceDrawSum"));

    posR->SetLabel(lman->lookup("cspacePeakRed"));
    posG->SetLabel(lman->lookup("cspacePeakGreen"));
    posB->SetLabel(lman->lookup("cspacePeakBlue"));
    sigR->SetLabel(lman->lookup("cspaceSigmaRed"));
    sigG->SetLabel(lman->lookup("cspaceSigmaGreen"));
    sigB->SetLabel(lman->lookup("cspaceSigmaBlue"));
    minVal->SetLabel(lman->lookup("cspaceMinVal"));
    maxVal->SetLabel(lman->lookup("cspaceMaxVal"));
}


void colourspaceFrame::makeUpdate(void)
{
    canvas->Redraw();
    if (doImmediateUpdate)
    {
        if (parentObj != NULL) parentObj->colourspaceChanged(&newParams);
        didUpdate++;
    }
}


int colourspaceFrame::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)okBut)
        {
            updateSettings();
            if (parentObj != NULL) parentObj->colourspaceChanged(&newParams);
            Close(TRUE);
            return 1;
        }
        else if (&obj == (wxObject*)cancelBut)
        {
            // return to old values, but we did updates?
            if (didUpdate != 0)
            {
                if (parentObj != NULL) parentObj->colourspaceChanged(&origParams);
                Close(TRUE);
                return 1;
            }
            if (parentObj != NULL) parentObj->closeEditor();
            return 1;
        }
        else if (&obj == (wxObject*)defaultBut)
        {
            const colourspace_params *cp = &(prefs->csp);
            newParams.peak_red = cp->peak_red;
            newParams.peak_green = cp->peak_green;
            newParams.peak_blue = cp->peak_blue;
            newParams.sigm_red = cp->sigm_red;
            newParams.sigm_green = cp->sigm_green;
            newParams.sigm_blue = cp->sigm_blue;
            newParams.type = cp->type;
            updateDisplay();
            makeUpdate();
            return 1;
        }
    }
    if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
    {
        if ((&obj == (wxObject*)posR) || (&obj == (wxObject*)posG) || (&obj == (wxObject*)posB) ||
                (&obj == (wxObject*)sigR) || (&obj == (wxObject*)sigG) || (&obj == (wxObject*)sigB) ||
                (&obj == (wxObject*)minVal) || (&obj == (wxObject*)maxVal))
        {
            updateSettings();
            makeUpdate();
        }
    }
    if (type == wxEVENT_TYPE_CHECKBOX_COMMAND)
    {
        if (&obj == (wxObject*)immediateUpdate)
        {
            doImmediateUpdate = immediateUpdate->GetValue();
            return 1;
        }
        if (&obj == (wxObject*)drawSum)
        {
            doDrawSum = drawSum->GetValue();
            canvas->enableOutlineSum(doDrawSum);
            return 1;
        }
    }
    if (type == wxEVENT_TYPE_CHOICE_COMMAND)
    {
        if (&obj == (wxObject*)csType)
        {
            updateSettings();
            canvas->setDrawingFunction();
            makeUpdate();
            return 1;
        }
    }

    return 0;
}


void colourspaceFrame::updateSettings(void)
{
    newParams.peak_red = atof(posR->GetValue());
    newParams.peak_green = atof(posG->GetValue());
    newParams.peak_blue = atof(posB->GetValue());
    newParams.sigm_red = atof(sigR->GetValue());
    newParams.sigm_green = atof(sigG->GetValue());
    newParams.sigm_blue = atof(sigB->GetValue());
    newParams.minVal = atof(minVal->GetValue());
    newParams.maxVal = atof(maxVal->GetValue());
    switch (csType->GetSelection())
    {
    case 0:
        newParams.type = cst_gauss;
        break;
    case 1:
        newParams.type = cst_linear;
        break;
    case 2:
        newParams.type = cst_rectangle;
        break;
    case 3:
        newParams.type = cst_asympt;
        break;
    default:
        cerr << "Unknown colourspace type" << endl;
        break;
    }
}


void colourspaceFrame::updateDisplay(const colourspace_params *cp)
{
    if (cp != NULL)
    {
        memcpy(&newParams, cp, sizeof(colourspace_params));
        memcpy(&origParams, cp, sizeof(colourspace_params));
    }
    posR->SetValue(newParams.peak_red);
    posG->SetValue(newParams.peak_green);
    posB->SetValue(newParams.peak_blue);
    sigR->SetValue(newParams.sigm_red);
    sigG->SetValue(newParams.sigm_green);
    sigB->SetValue(newParams.sigm_blue);
    minVal->SetValue(newParams.minVal);
    maxVal->SetValue(newParams.maxVal);

    switch (newParams.type)
    {
    case cst_linear:
        csType->SetSelection(1);
        break;
    case cst_rectangle:
        csType->SetSelection(2);
        break;
    case cst_asympt:
        csType->SetSelection(3);
        break;
    default:
        csType->SetSelection(0);
        break;
    }
    canvas->setDrawingFunction();
    canvas->Redraw();
}


void colourspaceFrame::processMouseEvent(wxMouseEvent &mevt)
{
    int type = mevt.GetEventType();

    if ((type == wxEVENT_TYPE_LEFT_DOWN) || (type == wxEVENT_TYPE_RIGHT_DOWN))
    {
        float delta;

        mevt.Position(&mousex, &mousey);
        dragColour = -1;
        delta = mousex - newParams.peak_red * (canvX -2*colourspaceCanvas::colcanv_cborder) - colourspaceCanvas::colcanv_cborder;
        if ((delta*delta) <= MOUSE_HOTZONE * MOUSE_HOTZONE) dragColour = 0;
        else
        {
            delta = mousex - newParams.peak_green * (canvX - 2*colourspaceCanvas::colcanv_cborder) - colourspaceCanvas::colcanv_cborder;
            if ((delta*delta) <= MOUSE_HOTZONE * MOUSE_HOTZONE) dragColour = 1;
            else
            {
                delta = mousex - newParams.peak_blue * (canvX - 2*colourspaceCanvas::colcanv_cborder) - colourspaceCanvas::colcanv_cborder;
                if ((delta*delta) <= MOUSE_HOTZONE * MOUSE_HOTZONE) dragColour = 2;
            }
        }
        if (dragColour >= 0)
        {
            if (type == wxEVENT_TYPE_LEFT_DOWN)
                mousebut = MOUSE_LEFT;
            else
                mousebut = MOUSE_RIGHT;
        }
    }
    else if ((type == wxEVENT_TYPE_MOTION) && (mousebut != 0))
    {
        float newx, newy;
        double *newVal=NULL;
        rviewText *newText;

        mevt.Position(&newx, &newy);

        // left button & left/right movement: move peak position
        if ((mousebut & MOUSE_LEFT) != 0)
        {
            switch (dragColour)
            {
            case 0:
                newVal = &newParams.peak_red;
                newText = posR;
                break;
            case 1:
                newVal = &newParams.peak_green;
                newText = posG;
                break;
            case 2:
                newVal = &newParams.peak_blue;
                newText = posB;
                break;
            default:
                return;
            }
            *newVal = (newx - colourspaceCanvas::colcanv_cborder) / (canvX - 2*colourspaceCanvas::colcanv_cborder);
            if (*newVal < 0.0) *newVal = 0.0;
            if (*newVal > 1.0) *newVal = 1.0;
        }
        // right button & up/down movement: change variance
        else if ((mousebut & MOUSE_RIGHT) != 0)
        {
            switch (dragColour)
            {
            case 0:
                newVal = &newParams.sigm_red;
                newText = sigR;
                break;
            case 1:
                newVal = &newParams.sigm_green;
                newText = sigG;
                break;
            case 2:
                newVal = &newParams.sigm_blue;
                newText = sigB;
                break;
            default:
                return;
            }
            *newVal -= 0.5 * (newy - mousey) / (canvX - 2*colourspaceCanvas::colcanv_cborder);
            // Must not let sigma become too small (floating exceptions)
            if (*newVal < 1e-6) *newVal = 1e-6;
        }

        mousex = newx;
        mousey = newy;

        if (newVal != NULL)
        {
            newText->SetValue(*newVal);
            canvas->Redraw();
            if (doImmediateUpdate)
            {
                if (parentObj != NULL) parentObj->colourspaceChanged(&newParams);
                didUpdate++;
            }
        }
    }
    else
    {
        mousebut = 0;
    }
}






// Threshold for lookup table size.
#define COLOURSPACE_TABLE_THRESHOLD 0x10000

/*
 *  Colourspace mapper class. For mapping large range integer values to RGB space
 *  baseType == rbt_none implies just the colourspace object without an associated MDD
 */
colourspaceMapper::colourspaceMapper(r_Ref<r_GMarray> &mdd, rviewBaseType bt, const colourspace_params *cp, bool fullrange, const r_Minterval *domain, unsigned long frange)
{
    didRange = FALSE;
    mddObj = mdd;
    baseType = bt;
    IntToRGBTab15 = NULL;
    IntToRGBTab24 = NULL;
    csFrame = NULL;
    if (baseType != rbt_none)
    {
        objInterv = mddObj->spatial_domain();
        dimMDD = objInterv.dimension();
        lastInterv = objInterv;
    }
    // Defaults
    if (cp == NULL)
    {
        memcpy(&par, &(prefs->csp), sizeof(colourspace_params));
    }
    else
    {
        memcpy(&par, cp, sizeof(colourspace_params));
    }
    par.minVal = 0.0;
    par.maxVal = 0.0;
    par.floatRange = frange;
    rangeModeFull = fullrange;
    useInterv = domain;
    scalingFactor = 1.0;  // needed for FP types only
    tableKind = getTableForType(baseType);

    tableType = (cspaceType)-1;   // initialize to some illegal value
    setMappingFunctions();

    processRange((rangeModeFull) ? CSPACE_RANGE_FULL : CSPACE_RANGE_ACTUAL);

    //cout << "mustep " << mustep << ", sigma " << sigma << ", min " << par.minVal << ", max " << par.maxVal << endl;
}


int colourspaceMapper::getTableForType(rviewBaseType bt)
{
    int tkind;

    switch (bt)
    {
    case rbt_long:
    case rbt_ulong:
    case rbt_float:
    case rbt_double:
        tkind = 1;
        break;
    default:
        tkind = 0;
        break;
    }
    return tkind;
}


int colourspaceMapper::bindMapper(r_Ref<r_GMarray> &mdd, rviewBaseType bt, bool fullrange, const r_Minterval *domain, const colourspace_params *cp)
{
    bool cparChanged;

    if (cp == NULL)
        cparChanged = 0;
    else
        cparChanged =
            ((cp->peak_red   != par.peak_red)   || (cp->sigm_red   != par.sigm_red) ||
             (cp->peak_green != par.peak_green) || (cp->sigm_green != par.sigm_green) ||
             (cp->peak_blue  != par.peak_blue)  || (cp->sigm_blue  != par.sigm_blue) ||
             (cp->type != par.type));

    if ((mddObj.ptr() == mdd.ptr()) && (bt == baseType) && (fullrange == rangeModeFull) && (domain == useInterv) && (cparChanged == 0))
        return 0;   // nothing to do

    if (cparChanged)
    {
        colourspace_params cpar;
        memcpy(&cpar, cp, sizeof(colourspace_params));
        cpar.minVal = par.minVal;
        cpar.maxVal = par.maxVal;
        cpar.floatRange = par.floatRange;
        colourspaceChanged(&cpar);
        if (csFrame != NULL)
        {
            csFrame->updateDisplay(&cpar);
        }
    }

    if ((mddObj.ptr() != mdd.ptr()) || (bt != baseType))
    {
        mddObj = mdd;
        baseType = bt;
        useInterv = domain;
        scalingFactor = 1.0;
        tableKind = getTableForType(baseType);
        rangeModeFull = fullrange;
        didRange = FALSE;
        processRange((rangeModeFull) ? CSPACE_RANGE_FULL : CSPACE_RANGE_ACTUAL);
        return 1;
    }
    if (fullrange != rangeModeFull)
    {
        rangeModeFull = fullrange;
        processRange((rangeModeFull) ? CSPACE_RANGE_FULL : CSPACE_RANGE_ACTUAL);
        return 1;
    }
    if ((domain != useInterv) || ((domain != NULL) && (*domain != *useInterv)) || (cparChanged != 0))
    {
        updateProjection(domain);
    }
    return 1;
}


colourspaceMapper::~colourspaceMapper(void)
{
    if (IntToRGBTab15 != NULL) delete [] IntToRGBTab15;
    if (IntToRGBTab24 != NULL) delete [] IntToRGBTab24;

    if (csFrame != NULL)
    {
        csFrame->unlinkParent();
        csFrame->Close(TRUE);
    }
}


void colourspaceMapper::setMappingFunctions(void)
{
    switch (par.type)
    {
    default:
        cerr << "Unknown mapping function, default to gauss" << endl;
    case cst_gauss:
        convert15 = &colourspaceMapper::ValToGauss15;
        convert24 = &colourspaceMapper::ValToGauss24;
        break;
    case cst_linear:
        convert15 = &colourspaceMapper::ValToLinear15;
        convert24 = &colourspaceMapper::ValToLinear24;
        break;
    case cst_rectangle:
        convert15 = &colourspaceMapper::ValToRectangle15;
        convert24 = &colourspaceMapper::ValToRectangle24;
        break;
    case cst_asympt:
        convert15 = &colourspaceMapper::ValToAsymptotic15;
        convert24 = &colourspaceMapper::ValToAsymptotic24;
        break;
    }
}


// Visual C compiler bug: with optimizations sigma is undefined when !fullrange
#ifdef __VISUALC__
#pragma optimize ("", off)
#endif

void colourspaceMapper::processRange(int rangeMode)
{
    double h;

    if (baseType == rbt_none) return;

    if (rangeMode != CSPACE_RANGE_OLD)
    {
        int i;

        lastInterv = (useInterv == NULL) ? objInterv : (*useInterv);
        rangeModeFull = (rangeMode == CSPACE_RANGE_FULL) ? TRUE : FALSE;

        // The number of projected pixels is a measure whether a lookup table pays off.
        projPixels = lastInterv[0].high() - lastInterv[0].low() + 1;
        for (i=1; i<dimMDD; i++)
        {
            projPixels *= lastInterv[i].high() - lastInterv[i].low() + 1;
        }

        // Use the full range of the basetype or the actual range of the object?
        if (rangeModeFull)
        {
            switch (baseType)
            {
            case rbt_char:
                par.minVal = 0.0;
                par.maxVal = (double)UCHAR_MAX;
                break;
            case rbt_uchar:
                par.minVal = (double)SCHAR_MIN;
                par.maxVal = (double)SCHAR_MAX;
                break;
            case rbt_short:
                par.minVal = (double)SHRT_MIN;
                par.maxVal = (double)SHRT_MAX;
                break;
            case rbt_ushort:
                par.minVal = 0.0;
                par.maxVal = (double)USHRT_MAX;
                break;
            case rbt_long:
                par.minVal = (double)LONG_MIN;
                par.maxVal = (double)LONG_MAX;
                break;
            case rbt_ulong:
                par.minVal = 0.0;
                par.maxVal = (double)ULONG_MAX;
                break;
                // MIN/MAX or DBL and FLT are unsigned, i.e. MIN is the smallest number > 0!
            case rbt_float:
                par.minVal = (double)(-FLT_MAX);
                par.maxVal = (double)FLT_MAX;
                break;
            case rbt_double:
                par.minVal = (double)(-DBL_MAX);
                par.maxVal = (double)DBL_MAX;
                break;
            default:
                break;
            }
        }
        else
        {
            if (!didRange)
            {
                int state;

                ::wxBeginBusyCursor();
                state = mdd_objectRange(mddObj, lastInterv, realMinVal, realMaxVal);
                ::wxEndBusyCursor();
                if (state == 0)
                {
                    realMinVal = 0.0;
                    realMaxVal = 1.0;
                }
                //cout << lastInterv << ": " << realMinVal << ", " << realMaxVal << endl;
                didRange = TRUE;
            }
            par.minVal = realMinVal;
            par.maxVal = realMaxVal;
        }
        if (csFrame != NULL)
        {
            csFrame->setRange(par.minVal, par.maxVal);
        }
    }

    if ((baseType == rbt_float) || (baseType == rbt_double))
    {
        // Avoid overflows; min/max could be the lowest/highest values, so the difference is NaN!
        h = ((par.maxVal) / 256.0) - ((par.minVal) / 256.0);
        if (h == 0.0) h = 1.0;
        scalingFactor = ((par.floatRange - 1) / 256.0 ) / h;
    }

    h = (par.maxVal - par.minVal);
    peakR = h * par.peak_red;
    peakG = h * par.peak_green;
    peakB = h * par.peak_blue;
    invSigR = 1.0 / (h * par.sigm_red);
    invSigG = 1.0 / (h * par.sigm_green);
    invSigB = 1.0 / (h * par.sigm_blue);

    buildCSTab15(TRUE);
    buildCSTab24(TRUE);
}

#ifdef __VISUALC__
#pragma optimize ("", on)
#endif


void colourspaceMapper::updateProjection(const r_Minterval *domain)
{
    if (!rangeModeFull)
    {
        if ((domain == NULL) && (useInterv == NULL)) return;
        if (domain != NULL)
        {
            int i;

            for (i=0; i<dimMDD; i++)
            {
                if ((lastInterv[i].low() != (*domain)[i].low()) ||
                        (lastInterv[i].high() != (*domain)[i].high()))
                    break;
            }
            // All equal ==> nothing to do
            if (i >= dimMDD) return;
        }

        didRange = FALSE;
        useInterv = domain;
        processRange(CSPACE_RANGE_ACTUAL);
    }
}



/*
 *  Shortcuts for defining convertor functions
 */
#define CSPACE_CONVERT(returntype, funcname, convname, rgbname) \
  returntype colourspaceMapper::funcname(double value) \
  { \
    unsigned int red, green, blue; \
    blue  = (unsigned int)(255.99*convname(value, peakB, invSigB)); \
    green = (unsigned int)(255.99*convname(value, peakG, invSigG)); \
    red   = (unsigned int)(255.99*convname(value, peakR, invSigR)); \
    return rgbname(red, green, blue); \
  }

#define CSPACE_CONVERT15(funcname, convname) \
  CSPACE_CONVERT(unsigned short, funcname, convname, RGBL_TO_PALETTE_SHORT)
#define CSPACE_CONVERT24(funcname, convname) \
  CSPACE_CONVERT(unsigned long, funcname, convname, RGB_TO_PALETTE_LONG)

CSPACE_CONVERT15(ValToGauss15, valueToGauss)
CSPACE_CONVERT24(ValToGauss24, valueToGauss)
CSPACE_CONVERT15(ValToLinear15, valueToLinear)
CSPACE_CONVERT24(ValToLinear24, valueToLinear)
CSPACE_CONVERT15(ValToRectangle15, valueToRectangle)
CSPACE_CONVERT24(ValToRectangle24, valueToRectangle)
CSPACE_CONVERT15(ValToAsymptotic15, valueToAsymptotic)
CSPACE_CONVERT24(ValToAsymptotic24, valueToAsymptotic)




unsigned short *colourspaceMapper::buildCSTab15(bool forceRebuild)
{
    int i, j;
    double val;

    // Don't build a 15bpp table if it's not needed
    if (tableKind != 0) return NULL;

    if (IntToRGBTab15 != NULL)
    {
        if ((!forceRebuild) && (tableType == par.type)) return IntToRGBTab15;
        delete [] IntToRGBTab15;
        IntToRGBTab15 = NULL;
    }

    // The code always assumes a table for this depth!
    //if ((par.maxVal - par.minVal + 1) > (double)COLOURSPACE_TABLE_THRESHOLD) return NULL;
    j = (int)((par.maxVal - par.minVal) * scalingFactor) + 1;

    IntToRGBTab15 = new unsigned short[j];
    for (i=0, val=0.0; i<j; i++, val += scalingFactor)
    {
        IntToRGBTab15[i] = ValToCS15(val);
    }
    tableType = par.type;
    return IntToRGBTab15;
}



unsigned long *colourspaceMapper::buildCSTab24(bool forceRebuild)
{
    int i, j;
    double val;
    double invScale;

    // Don't build a 32bpp table if it's not needed
    if (tableKind == 0) return NULL;

    if (IntToRGBTab24 != NULL)
    {
        if ((!forceRebuild) && (tableType == par.type)) return IntToRGBTab24;
        delete [] IntToRGBTab24;
        IntToRGBTab24 = NULL;
    }

    j = (int)((par.maxVal - par.minVal) * scalingFactor) + 1;
    if (j > COLOURSPACE_TABLE_THRESHOLD) return NULL;

    // If we use the range of the current projection and there are fewer visible pixels than
    // table entries then don't build a table (or we'd sacrifice a lot of speed)
    if ((!rangeModeFull) && (useInterv != NULL) && (j > projPixels)) return NULL;

    invScale = (scalingFactor == 0.0) ? 1.0 : 1/scalingFactor;
    IntToRGBTab24 = new unsigned long[j];
    for (i=0, val=0.0; i<j; i++, val += invScale)
    {
        IntToRGBTab24[i] = ValToCS24(val);
    }
    tableType = par.type;
    return IntToRGBTab24;
}



double colourspaceMapper::getMinVal(void)
{
    return par.minVal;
}


double colourspaceMapper::getMaxVal(void)
{
    return par.maxVal;
}


double colourspaceMapper::getScalingFactor(void)
{
    return scalingFactor;
}


unsigned short *colourspaceMapper::getCSTab15(void)
{
    return IntToRGBTab15;
}


unsigned long *colourspaceMapper::getCSTab24(void)
{
    return IntToRGBTab24;
}


void colourspaceMapper::colourspaceChanged(const colourspace_params *newParams, bool autoUpdate)
{
    user_event ue;

    memcpy(&par, newParams, sizeof(colourspace_params));
    setMappingFunctions();
    processRange(CSPACE_RANGE_OLD);

    if (csFrame != NULL)
        csFrame->updateDisplay(newParams);

    if (autoUpdate)
    {
        // Notify whoever owns this object that its settings have changed (can't have one
        // specific type of parent because it can be owned by images, thumbnails, prefs, ...)
        ue.type = usr_cspace_changed;
        ue.data = (void*)this;
        //parentFrame->userEvent(ue);
        if (frameManager != NULL)
            frameManager->broadcastUserEvent(ue);
    }
}


void colourspaceMapper::openEditor(void)
{
    if (csFrame != NULL) return;

    csFrame = new colourspaceFrame(this, &par);
}


void colourspaceMapper::closeEditor(bool activeClose)
{
    if (activeClose) csFrame->Close(TRUE);
    csFrame = NULL;
}


void colourspaceMapper::getParameters(colourspace_params *dest)
{
    if (csFrame != NULL)
    {
        csFrame->updateSettings();
    }
    memcpy(dest, &par, sizeof(colourspace_params));
}
