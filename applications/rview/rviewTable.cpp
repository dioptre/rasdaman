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
 *  rView table viewer. Can display MDD objects of any dimension and base type (!)
 *  as tables. Optional modes are decimal, octal and hex number bases.
 *
 *  COMMENTS:
 *      None
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
#include <iostream.h>


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif


#include "raslib/rmdebug.hh"
#include "raslib/primitivetype.hh"
#include "raslib/structuretype.hh"


#include "rviewTypes.hh"

#include "labelManager.hh"

#include "rviewUtils.hh"
#include "rviewDModes.hh"
#include "rviewPrefs.hh"


const int textCanvas::txcanv_cospace = 8;
const int textCanvas::txcanv_colspace = 16;
const int textCanvas::txcanv_border = rviewDisplay::display_border;

const int rviewTable::table_twidth = 64;
const int rviewTable::table_theight = 50;
const int rviewTable::table_cwidth = 100;
const int rviewTable::table_cheight = 50;
const int rviewTable::table_minwidth = 100;
const int rviewTable::table_minheight = 100;
const int rviewTable::table_ctrly = 64;
const int rviewTable::table_totaly = rviewDisplay::display_cheight + rviewTable::table_ctrly;




textCanvas::textCanvas(wxWindow *parent, int x, int y, int w, int h, long style) : wxCanvas(parent, x, y, w, h, style)
{
    wxColour fc(0x10, 0x10, 0x10);
    wxColour bc(0xf0, 0xf0, 0xf0);

    fore.SetStyle(wxSOLID);
    fore.SetColour(fc);
    back.SetStyle(wxSOLID);
    back.SetColour(bc);
    // Don't use wxDEFAULT! Doesn't work on Sun! */
    font = new wxFont(12, wxROMAN, wxNORMAL, wxNORMAL);
    SetBackground(&back);
    SetTextBackground(&bc);
    SetTextForeground(&fc);
    pen.SetStyle(wxSOLID);
    pen.SetColour(fc);

    scrollX = 0;
    scrollY = 0;
}


textCanvas::~textCanvas(void)
{
    SetBackground(NULL);
    SetTextBackground(NULL);
    SetTextForeground(NULL);
    pen.SetColour(0, 0, 0);
    delete font;
}


void textCanvas::setData(mdd_frame *mf, rviewBaseType bt, unsigned int bs)
{
    mddObj = mf->mdd;
    dimMDD = (int)(mddObj->spatial_domain().dimension());
    baseType = bt;
    baseSize = bs;
}


void textCanvas::setStep(int sx, int sy)
{
    stepx = sx;
    stepy = sy;
}


void textCanvas::setProjection(r_Point &p1, r_Point &p2, unsigned int fd, r_Point *mapIndex)
{
    pt1 = p1;
    pt2 = p2;
    freeDims = fd;
    for (dim1=0; dim1<dimMDD; dim1++) if ((freeDims & (1<<dim1)) != 0) break;
    for (dim2=dim1+1; dim2<dimMDD; dim2++) if ((freeDims & (1<<dim2)) != 0) break;

    if (dim1 >= dimMDD)
        dim1 = -1;
    else if (mapIndex != NULL)
        dim1 = (*mapIndex)[dim1];
    if (dim2 >= dimMDD)
        dim2 = -1;
    else if (mapIndex != NULL)
        dim2 = (*mapIndex)[dim2];
}


void textCanvas::setCoSys(bool cs, int &cl, int &ct)
{
    cosys = cs;

    if (cosys)
    {
        char buffer[STRINGSIZE];
        float twidth, theight;

        SetFont(font);
        if (dim2 < 0)
        {
            coleft = txcanv_cospace;
            sprintf(buffer, "%ld", pt1[dim1]);
            GetTextExtent(buffer, &twidth, &theight);
        }
        else
        {
            sprintf(buffer, "%ld", (abs(pt1[dim2]) > abs(pt2[dim2])) ? pt1[dim2] :pt2[dim2]);
            GetTextExtent(buffer, &twidth, &theight);
            coleft = (int)twidth + txcanv_cospace;
        }
        cotop = (int)theight + txcanv_cospace;
        SetFont(NULL);
    }
    else
    {
        coleft = 0;
        cotop = 0;
    }
    cl = coleft;
    ct = cotop;
}



void textCanvas::OnPaint(void)
{
    wxUpdateIterator upd(this);
    wxRect rect;
    int w, h, x, y;
    r_Point prun = pt1;
    wxCanvasDC *cdc;
    r_Range startOffX, endOffX, startOffY, endOffY;
    float posx, posy;
    char textbuff[STRINGSIZE];
    bool redrawAll = FALSE;

    //cout << "textCanvas::OnPaint()" << endl;

    if (dim1 < 0) return;

    GetClientSize(&w, &h);

    cdc = GetDC();
    cdc->BeginDrawing();
    cdc->SetMapMode(MM_TEXT);
    cdc->SetBrush(&fore);
    cdc->SetFont(font);
    cdc->SetLogicalFunction(wxCOPY);

    w = GetScrollPos(wxHORIZONTAL);
    h = GetScrollPos(wxVERTICAL);
    x = rviewDisplay::display_scrstep * w;
    y = rviewDisplay::display_scrstep * h;

    // In case of scrolling we have to plot the whole thing. There's a bug in the
    // current version of wxWindows that means you don't get any rectangles returned
    // that were covered by other windows and are now visible.
    if ((w != scrollX) || (h != scrollY))
    {
        redrawAll = TRUE;
        GetClientSize(&rect.width, &rect.height);
        rect.x = 0;
        rect.y = 0;
        scrollX = w;
        scrollY = h;
    }

    // Use for, not while, because of continue in loop body
    for (; upd ; upd++)
    {
        if (!redrawAll) upd.GetRect(&rect);

        // cdc->SetClippingRegion(rect.x, rect.y, rect.width, rect.height);
        // Calculate the range to plot.
        startOffX = (x + rect.x - txcanv_border - coleft) / stepx;
        if (startOffX < 0) startOffX = 0;
        if (pt1[dim1] + startOffX > pt2[dim1]) continue;
        endOffX = (x + rect.x + rect.width - txcanv_border  - coleft + stepx - 1) / stepx;
        if (endOffX > pt2[dim1] - pt1[dim1]) endOffX = pt2[dim1] - pt1[dim1];
        if (dim2 < 0)
        {
            startOffY = 0;
            endOffY = 0;
        }
        else
        {
            startOffY = (y + rect.y - txcanv_border - cotop) / stepy;
            if (startOffY < 0) startOffY = 0;
            if (pt1[dim2] + startOffY > pt2[dim2]) continue;
            endOffY = (y + rect.y + rect.height - txcanv_border - cotop + stepy - 1) / stepy;
            if (endOffY > pt2[dim2] - pt1[dim2]) endOffY = pt2[dim2] - pt1[dim2];
        }

        //cout << "table: " << rect.x << ':' << rect.y << ':' << rect.width << ':' << rect.height << " -- " << startOffX << ':' << endOffX << ':' << startOffY << ':' << endOffY << endl;

        posy = (float)(startOffY * stepy + txcanv_border + cotop);
        r_Ref<r_Marray<r_Char> > mddPtr = (r_Ref<r_Marray<r_Char> >)mddObj;
        r_Char *srcBase;
        const r_Type *tp;

        if ((tp = mddPtr->get_base_type_schema()) == NULL)
        {
            cerr << "No schema information available" << endl; // FIXME
            return;
        }

        srcBase = (r_Char*)(mddPtr->get_array());
        if (dim2 < 0)
        {
            posx = (float)(startOffX * stepx + txcanv_border + coleft);
            for (prun[dim1]=pt1[dim1]+startOffX; prun[dim1]<=pt1[dim1]+endOffX; prun[dim1]++, posx+=stepx)
            {
                long offset;

                offset = ((&((*mddPtr)[prun])) - srcBase) * baseSize;
                rviewPrintTypedCell(tp, textbuff, (char*)(srcBase + offset), numberBase);
                cdc->DrawText(textbuff, posx, posy);
            }
        }
        else
        {
            for (prun[dim2]=pt1[dim2]+startOffY; prun[dim2]<=pt1[dim2]+endOffY; prun[dim2]++, posy+=stepy)
            {
                posx = (float)(startOffX * stepx + txcanv_border + coleft);
                for (prun[dim1]=pt1[dim1]+startOffX; prun[dim1]<=pt1[dim1]+endOffX; prun[dim1]++, posx+=stepx)
                {
                    long offset;

                    offset = ((&((*mddPtr)[prun])) - srcBase) * baseSize;
                    rviewPrintTypedCell(tp, textbuff, (char*)(srcBase + offset), numberBase);
                    cdc->DrawText(textbuff, posx, posy);
                }
            }
        }

        // Draw coordinate system if necessary
        if (cosys)
        {
            int i;

            // Mustn't plot entire lines in one go; the lengths seem to be broken into 16 bit quantities
            // internally so you could get overflows. Instead plot just what's visible.
            if (startOffX == 0)
            {
                cdc->DrawLine(txcanv_border, cotop, txcanv_border + coleft, cotop);
            }
            if (startOffY == 0)
            {
                cdc->DrawLine(txcanv_border + coleft + startOffX * stepx, cotop, txcanv_border + coleft + (endOffX+1)*stepx, cotop);
            }
            if (dim2 >= 0)
            {
                if (startOffX == 0)
                {
                    cdc->DrawLine(coleft, txcanv_border + cotop + startOffY * stepy, coleft, txcanv_border + cotop + (endOffY+1) * stepy);
                }
                if (startOffY == 0)
                {
                    cdc->DrawLine(coleft, txcanv_border, coleft, txcanv_border + cotop);
                }
                posx = txcanv_cospace/2;
                posy = startOffY * stepy + txcanv_border + cotop;
                for (i=startOffY; i<=endOffY; i++, posy+=stepy)
                {
                    sprintf(textbuff, "%ld", pt1[dim2] + i);
                    cdc->DrawText(textbuff, posx, posy);
                }
            }

            posx = startOffX * stepx + txcanv_border + coleft;
            posy = txcanv_cospace/2;
            for (i=startOffX; i<=endOffX; i++, posx+=stepx)
            {
                sprintf(textbuff, "%ld", pt1[dim1] + i);
                cdc->DrawText(textbuff, posx, posy);
            }
        }
    }
    cdc->SetBrush(NULL);
    cdc->SetFont(NULL);
    cdc->EndDrawing();
}


void textCanvas::CalcTextExtent(char *b, float &width, float &height)
{
    SetFont(font);
    GetTextExtent(b, &width, &height);
    SetFont(NULL);
}


void textCanvas::EstimateCellSize(int &width, int &height)
{
    char buffer[STRINGSIZE];
    float twidth, theight;

    sprintf(buffer, " ,");
    switch (numberBase)
    {
    case 8:
        sprintf(buffer+2, "%o", 255);
        break;
    case 16:
        sprintf(buffer+2, "%x", 255);
        break;
    default:
        sprintf(buffer+2, "%d", 255);
        break;
    }
    CalcTextExtent(buffer, twidth, theight);
    width = (int)(baseSize * twidth + txcanv_colspace);
    height = (int)(1.5 * theight);
}



void textCanvas::setNumberBase(int newBase)
{
    numberBase = newBase;
}







/*
 *  Class to display tables
 */

const char *rviewTable::view_StepSize = "stepSize";
const char *rviewTable::view_ScrollPos = "scrollPos";
const char *rviewTable::view_CoSys = "coordSys";
const char *rviewTable::view_NumBase = "numberBase";

rviewTable::rviewTable(mdd_frame *mf, unsigned int flags) : rviewDisplay(mf, table_ctrly, flags)
{
    int w, h, i;
    char *b;

    RMDBGONCE(3, RMDebug::module_applications, "rviewTable", "rviewTable()");

    // Mode defaults, move to prefs later on
    stepx = prefs->tableStepx;
    stepy = prefs->tableStepy;
    cosys = prefs->tableCosys;
    numberBase = prefs->tableMode;

    GetClientSize(&w, &h);
    w -= 2*display_cnvborder;
    h -= 2*display_cnvborder + table_totaly;
    canvas = new textCanvas((wxWindow*)this, display_cnvborder, display_cnvborder + table_totaly, w, h);
    canvas->setData(mf, baseType, baseSize);

    // Need to init these before calling EstimateCellSize()
    sxText = NULL;
    syText = NULL;
    // Try to make a good guess about the default table width
    canvas->setNumberBase(numberBase);
    EstimateCellSize(stepx, stepy);

    sxText = new rviewText(ctrlPanel, stepx);
    syText = new rviewText(ctrlPanel, stepy);
    csBox = new rviewCheckBox(ctrlPanel);
    csBox->SetValue(cosys);

    // Init projection string
    b = projString;
    b += sprintf(b, "*:*");
    if (dimMDD>1)
    {
        b += sprintf(b, ", *:*");
    }
    for (i=2; i<dimMDD; i++)
    {
        b += sprintf(b, ", %ld", interv[i].low());
    }
    project->SetValue(projString);

    scrollx = -1;
    scrolly = -1;
    newProjection();

    setModeDimension((dimMDD == 1) ? 1 : 2);

    setMinimumViewerSize(table_minwidth, table_minheight);
}


int rviewTable::openViewer(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewTable", "openViewer()");

    if (rviewDisplay::openViewer() == 0)
    {
        int w, h;
        wxMenu *men;
        char buffer[STRINGSIZE];

        GetClientSize(&w, &h);

        men = new wxMenu;
        men->Append(MENU_TABLE_MODE_DECIMAL, "", NULL, TRUE);
        men->Append(MENU_TABLE_MODE_OCTAL, "", NULL, TRUE);
        men->Append(MENU_TABLE_MODE_HEX, "", NULL, TRUE);
        sprintf(buffer, "&%s\n", lman->lookup("menTabMode"));
        mBar->Append(men, buffer);

        checkModeMenu();

        label();

        frameWidth=-1;
        frameHeight=-1;

        OnSize(w, h);
        OnSize(w, h);

        Show(TRUE);

        return 0;
    }
    return -1;
}


void rviewTable::checkModeMenu(void)
{
    switch (numberBase)
    {
    case 8:
        lastMode = MENU_TABLE_MODE_OCTAL;
        break;
    case 16:
        lastMode = MENU_TABLE_MODE_HEX;
        break;
    default:
        lastMode = MENU_TABLE_MODE_DECIMAL;
        break;
    }
    mBar->Check(lastMode, TRUE);
}


const char *rviewTable::getFrameName(void) const
{
    return "rviewTable";
}

rviewFrameType rviewTable::getFrameType(void) const
{
    return rviewFrameTypeTable;
}

int rviewTable::getViewerType(void) const
{
    return RVIEW_RESDISP_TABLE;
}


void rviewTable::EstimateCellSize(int &width, int &height)
{
    int w, h;
    char buffer[STRINGSIZE];

    canvas->EstimateCellSize(w, h);
    // If prefs specify defaults (<= 0), update width/height, otherwise leave old values.
    if (prefs->tableStepx <= 0)
    {
        width = w;
        if (sxText != NULL)
        {
            sprintf(buffer, "%d", w);
            sxText->SetValue(buffer);
        }
    }
    if (prefs->tableStepy <= 0)
    {
        height = h;
        if (syText != NULL)
        {
            sprintf(buffer, "%d", h);
            syText->SetValue(buffer);
        }
    }
}



rviewTable::~rviewTable(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewTable", "~rviewTable()");
    closeViewer();
}



void rviewTable::label(void)
{
    setDisplayTitle(lman->lookup("titleTable"));

    sxText->SetLabel(lman->lookup("textStepx"));
    syText->SetLabel(lman->lookup("textStepy"));
    csBox->SetLabel(lman->lookup("textCosys"));

    mBar->SetLabelTop(fixedNumberOfMenus, lman->lookup("menTabMode"));
    mBar->SetLabel(MENU_TABLE_MODE_DECIMAL, lman->lookup("menTabModeDec"));
    mBar->SetLabel(MENU_TABLE_MODE_OCTAL, lman->lookup("menTabModeOct"));
    mBar->SetLabel(MENU_TABLE_MODE_HEX, lman->lookup("menTabModeHex"));

    rviewDisplay::label();
}



void rviewTable::OnSize(int w, int h)
{
    int x, y, i, j;

    GetClientSize(&x, &y);
    x -= 2*display_border;
    i = x - 2*table_twidth - table_cwidth;
    j = 2*display_border + display_cheight;
    sxText->SetSize(display_border + i/6, j, table_twidth, table_theight);
    syText->SetSize(display_border + (3*i)/6 + table_twidth, j, table_twidth, table_theight);
    csBox->SetSize(display_border + (5*i)/6 + 2*table_twidth, j, table_cwidth, table_cheight);
    y -= 2*display_border + table_totaly;

    canvas->SetSize(display_border, display_border + table_totaly, x, y);

    rviewDisplay::OnSize(w, h);
}



void rviewTable::OnMenuCommand(int id)
{
    int newBase;

    switch (id)
    {
    case MENU_TABLE_MODE_DECIMAL:
        newBase = 10;
        break;
    case MENU_TABLE_MODE_OCTAL:
        newBase = 8;
        break;
    case MENU_TABLE_MODE_HEX:
        newBase = 16;
        break;
    default:
        newBase = -1;
        break;
    }
    if (newBase > 0)
    {
        mBar->Check(lastMode, FALSE);
        numberBase = newBase;
        checkModeMenu();
        canvas->setNumberBase(numberBase);
        EstimateCellSize(stepx, stepy);
        newTableSize();
    }
    else
    {
        rviewDisplay::OnMenuCommand(id);
    }
}



int rviewTable::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();
    int i, j;

    if (((&obj == (wxObject*)sxText) || (&obj == (wxObject*)syText)) && (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND))
    {
        i = atoi(sxText->GetValue());
        j = atoi(syText->GetValue());
        if ((i > 0) && (j > 0))
        {
            stepx = i;
            stepy = j;
            newTableSize();
            return 1;
        }
    }

    if ((&obj == (wxObject*)csBox) && (type == wxEVENT_TYPE_CHECKBOX_COMMAND))
    {
        cosys = csBox->GetValue();
        newProjection();
        return 1;
    }

    if (rviewDisplay::process(obj, evt) != 0)
    {
        return 1;
    }

    return 0;
}



int rviewTable::newProjection(void)
{
    int dim1, dim2;

    mapIndex = r_Point(dimMDD);
    if (rviewParseProjection(getVirtualDomain(), pt1, pt2, projString, &freeDims, &mapIndex) != dimMDD)
    {
        rviewErrorbox::reportError(lman->lookup("errorProjection"), rviewTable::getFrameName(), "newProjection");
        return -1;
    }

    for (dim1=0; dim1<dimMDD; dim1++) if ((freeDims & (1<<dim1)) != 0) break;
    for (dim2=dim1+1; dim2<dimMDD; dim2++) if ((freeDims & (1<<dim2)) != 0) break;
    if (dim1 >= dimMDD)
    {
        rviewErrorbox::reportError(lman->lookup("errorProjectFree"), rviewTable::getFrameName(), "newProjection");
        return -1;
    }
    dim1 = mapIndex[dim1];
    canvas->setProjection(pt1, pt2, freeDims, &mapIndex);
    fieldsx = pt2[dim1] - pt1[dim1] + 1;
    if (dim2 >= dimMDD)
    {
        fieldsy = 1;
    }
    else
    {
        dim2 = mapIndex[dim2];
        fieldsy = pt2[dim2] - pt1[dim2] + 1;
    }

    //EstimateCellSize(&stepx, &stepy);

    newTableSize();

    return 0;
}



void rviewTable::newTableSize(void)
{
    int cl, ct;
    int newstepx, newstepy;

    canvas->setStep(stepx, stepy);
    canvas->setCoSys(cosys, cl, ct);

    if (scrollx >= 0)
        scrollx = canvas->GetScrollPos(wxHORIZONTAL);
    else
        scrollx = 0;

    if (scrolly >= 0)
        scrolly = canvas->GetScrollPos(wxVERTICAL);
    else
        scrolly = 0;

    newstepx = (int)((fieldsx*stepx + 2*display_border + cl + display_scrstep - 1) / display_scrstep);
    newstepy = (int)((fieldsy*stepy + 2*display_border + ct + display_scrstep - 1) / display_scrstep);

    canvas->SetScrollbars(display_scrstep, display_scrstep, newstepx, newstepy, display_pgstep, display_pgstep, scrollx, scrolly);
}


int rviewTable::saveView(FILE *fp)
{
    int status = rviewDisplay::saveView(fp);

    long lvals[2];
    lvals[0] = (long)stepx;
    lvals[1] = (long)stepy;
    writeViewParam(fp, view_StepSize, 2, lvals);
    lvals[0] = (long)(canvas->GetScrollPos(wxHORIZONTAL));
    lvals[1] = (long)(canvas->GetScrollPos(wxVERTICAL));
    writeViewParam(fp, view_ScrollPos, 2, lvals);
    writeViewParam(fp, view_CoSys, (long)cosys);
    writeViewParam(fp, view_NumBase, (long)numberBase);

    return status;
}


int rviewTable::readView(const char *key, const char *value)
{
    int status = rviewDisplay::readView(key, value);

    if (status == 0)
    {
        if (strcmp(key, view_StepSize) == 0)
        {
            long lvals[2];
            if (readVector(value, 2, lvals) == 0)
            {
                stepx = (int)lvals[0];
                stepy = (int)lvals[1];
            }
            return 1;
        }
        else if (strcmp(key, view_ScrollPos) == 0)
        {
            long lvals[2];
            if (readVector(value, 2, lvals) == 0)
            {
                scrollx = (int)lvals[0];
                scrolly = (int)lvals[1];
            }
            return 1;
        }
        else if (strcmp(key, view_CoSys) == 0)
        {
            cosys = (bool)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_NumBase) == 0)
        {
            numberBase = atoi(value);
            return 1;
        }
        return 0;
    }
    return status;
}


void rviewTable::loadViewFinished(void)
{
    sxText->SetValue(stepx);
    syText->SetValue(stepy);
    csBox->SetValue(cosys);

    mBar->Check(lastMode, FALSE);
    checkModeMenu();

    canvas->SetScrollPos(wxHORIZONTAL, scrollx);
    canvas->SetScrollPos(wxVERTICAL, scrolly);
}
