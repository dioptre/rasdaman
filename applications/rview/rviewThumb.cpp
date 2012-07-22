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


#include "wx_pixmap.h"


#include "labelManager.hh"


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif



#include "rviewPrefs.hh"
#include "rviewTypes.hh"
#include "rviewUtils.hh"
#include "rviewThumb.hh"

#include "raslib/rmdebug.hh"

#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"




const int rviewThumb::thumb_width = 450;
const int rviewThumb::thumb_height = 300;
const int rviewThumb::thumb_imgwidth = 100;
const int rviewThumb::thumb_perline = 4;
const int rviewThumb::thumb_space = 32;
const int rviewThumb::thumb_border = 8;
const int rviewThumb::thumb_scrstep = 8;
const int rviewThumb::thumb_pgstep = 8;
const int rviewThumb::thumb_chkwidth = 100;
const int rviewThumb::thumb_chkheight = 30;
const int rviewThumb::thumb_minwidth = 10;
const int rviewThumb::thumb_maxwidth = 2000;
const int rviewThumb::thumb_mincols = 1;
const int rviewThumb::thumb_maxcols = 16;
const int rviewThumb::thumb_cheight = 64;
const int rviewThumb::thumb_twidth = 80;
const int rviewThumb::thumb_theight = 50;
const int rviewThumb::thumb_prjwidth = 60;






/*
 *  The thumbnail canvas class
 */
thumbCanvas::thumbCanvas(rviewThumb *par, int x, int y, int width, int height) : wxCanvas((wxWindow*)par, x, y, width, height, 0)
{
    parent = par;
    brush.SetStyle(wxSOLID);
    brush.SetColour((char)0xc0, (char)0xc0, (char)0xc0);
    SetBackground(&brush);
    font = new wxFont(12, wxROMAN, wxNORMAL, wxNORMAL);
}


thumbCanvas::~thumbCanvas(void)
{
    SetBackground(NULL);
    delete font;
}


void thumbCanvas::OnPaint(void)
{
    int thumbs, thumbsperline;
    int gridX, gridY;
    wxPixmap *thumbnail;
    wxUpdateIterator upd(this);
    //wxRect rect;
    int startx, starty, endx, endy, w, h, posx, posy;
    int scrollx, scrolly;
    int maxy, runx, runy;

    //cout << "thumbCanvas::OnPaint" << endl;

    parent->getThumbInfo(thumbs, thumbsperline);
    parent->getGridInfo(gridX, gridY);

    if ((thumbs <= 0) || (thumbsperline <= 0) || (gridX <= 0) || (gridY <= 0)) return;

    GetClientSize(&w, &h);

    scrollx = rviewThumb::thumb_scrstep * GetScrollPos(wxHORIZONTAL);
    scrolly = rviewThumb::thumb_scrstep * GetScrollPos(wxVERTICAL);

    startx = (scrollx / gridX);
    endx = ((scrollx + w + gridX - 1) / gridX);
    starty = (scrolly / gridY);
    endy = ((scrolly + h + gridY - 1) / gridY);
    posx = rviewThumb::thumb_space / 2 + gridX * startx;
    posy = rviewThumb::thumb_space / 2 + gridY * starty;

    maxy = (thumbs + thumbsperline - 1) / thumbsperline;

    if ((startx >= thumbsperline) || (endx < 0)) return;
    if ((starty >= maxy) || (endy < 0)) return;

    if (startx < 0) startx = 0;
    if (endx >= thumbsperline) endx = thumbsperline-1;

    if (starty < 0) starty = 0;
    if (endy >= maxy) endy = maxy - 1;

    BeginDrawing();

    SetFont(font);

    while (upd)
    {
        char caption[STRINGSIZE];

        runx = posx;
        for (w = startx; w <= endx; w++, runx += gridX)
        {
            runy = posy;
            for (h = starty; h <= endy; h++, runy += gridY)
            {
                if ((thumbnail = parent->getPixmapNumber(h * thumbsperline + w, caption)) != NULL)
                {
                    float tw, th;

                    /*cout << "Pixmap " << w << ' ' << h << ": " << (void*)thumbnail << ", at " << runx << ' ' << runy
                         << ", width " << thumbnail->getWidth() << ", height " << thumbnail->getHeight()
                             << ", depth " << thumbnail->getDepth() << ", ddepth " << thumbnail->getModeDepth() << endl;*/

                    // Pixmaps have the canvas coordinate system, text has the _scrolled_ canvas cosys!
                    thumbnail->plotPixmap(runx - scrollx, runy - scrolly);
                    GetTextExtent(caption, &tw, &th);
                    tw = ((float)(thumbnail->getWidth()) - tw) / 2;
                    th = thumbnail->getHeight();
                    DrawText(caption, runx + tw, runy + th);
                }
            }
        }
        upd++;
    }

    SetFont(NULL);

    EndDrawing();
}


void thumbCanvas::updateDisplay(void)
{
    if (parent->IsShown())
    {
        int cw, ch;

        GetClientSize(&cw, &ch);
        SetClippingRegion(0, 0, cw, ch);
        OnPaint();
        DestroyClippingRegion();
    }
}



/*
 *  The rviewThumb class for displaying thumbnails of images
 */
rviewThumb::rviewThumb(void) : rviewFrame(NULL, "", 0, 0, thumb_width, thumb_height)
{
    int x, y, tw, py;
    wxMenu *menu;
    wxMenu *setup;
    wxMenu *submenu;
    char buffer[STRINGSIZE];

    thumbsperline = 4;
    if (prefs->thumbCols > 0) thumbsperline = prefs->thumbCols;
    projstep = 1;
    if (prefs->thumbProjstep > 0) projstep = prefs->thumbProjstep;
    doValToCspace = (prefs->rgbSpace != 0);
    doFullRangeCspace = (prefs->rgbSpace == 2);
    dimproj = prefs->thumbProjdim;
    imgWidth = 100;
    if (prefs->thumbWidth > 0) imgWidth = prefs->thumbWidth;
    if (imgWidth < thumb_minwidth) imgWidth = thumb_minwidth;
    if (imgWidth > thumb_maxwidth) imgWidth = thumb_maxwidth;

    thumbs = 0;
    numPixmaps = 0;
    maxHeight = -1;
    listHead = NULL;
    csmap = NULL;
    canDoCspace = FALSE;
    gridX = 0;
    gridY = 0;

    menu = new wxMenu;
    menu->Append(MENU_THUMB_DATA_CLOSE, "");
    submenu = new wxMenu;
    submenu->Append(MENU_THUMB_CSPACE_ON, "", NULL, TRUE);
    submenu->Append(MENU_THUMB_CSPACE_FULL, "", NULL, TRUE);
    submenu->Append(MENU_THUMB_CSPACE_EDIT, "");
    setup = new wxMenu;
    setup->Append(MENU_THUMB_SETUP_CSPACE, "", submenu, NULL);

    mbar = new wxMenuBar;
    sprintf(buffer, "&%s", lman->lookup("menThumbData"));
    mbar->Append(menu, buffer);
    sprintf(buffer, "&%s", lman->lookup("menThumbSetup"));
    mbar->Append(setup, buffer);
    mbar->Check(MENU_THUMB_CSPACE_ON, doValToCspace);
    mbar->Check(MENU_THUMB_CSPACE_FULL, doFullRangeCspace);
    configureCspace(FALSE);

    GetClientSize(&x, &y);

    panel = new wxPanel((wxWindow*)this, 0, thumb_border, x, thumb_cheight);
    panel->SetLabelPosition(wxVERTICAL);

    x -= 2*thumb_border;
    y -= 2*thumb_border + thumb_cheight;
    canvas = new thumbCanvas(this, thumb_border, thumb_border + thumb_cheight, x, y);

    tw = (x - 4*thumb_border - 2*thumb_prjwidth) / 3;
    py = thumb_border;
    projString[0] = '\0';
    project = new rviewText(panel);
    thumbProj = new rviewText(panel, dimproj);
    thumbStep = new rviewText(panel, projstep);
    thumbWidth = new rviewText(panel, imgWidth);
    thumbCols = new rviewText(panel, thumbsperline);

    SetMenuBar(mbar);

    label();

    frameWidth=-1;
    frameHeight=-1;

    OnSize(x, y);
    OnSize(x, y);


    Show(TRUE);
}


rviewThumb::~rviewThumb(void)
{
    if (thumbs != 0)
    {
        int i;
        rviewThumbList *tlst = listHead;

        for (i=0; i<thumbs; i++)
        {
            deletePixmapChain(tlst);

            //cout << "delete " << (void*)tlst << endl;
            listHead = tlst;
            tlst = tlst->next;
            delete listHead;
        }
    }
    if (csmap != NULL) delete csmap;
}


void rviewThumb::configureCspace(bool mode)
{
    mbar->Enable(MENU_THUMB_SETUP_CSPACE, mode);
    mbar->Enable(MENU_THUMB_CSPACE_ON, mode);
    mbar->Enable(MENU_THUMB_CSPACE_FULL, mode);
    mbar->Enable(MENU_THUMB_CSPACE_EDIT, mode);
}



// Deletes all the pixmaps for one MDD object
void rviewThumb::deletePixmapChain(rviewThumbList *tlst)
{
    if (tlst->pixmaps != NULL)
    {
        rviewThumbPixList *tplst, *tplast;

        tplst = tlst->pixmaps;
        while (tplst != NULL)
        {
            //cout << "   delete " << (void*)tplst << endl;
            tplast = tplst;
            tplst = tplst->next;
            if (tplast->pixmap != NULL) delete tplast->pixmap;
            delete tplast;
            numPixmaps--;
        }
        tlst->numPix = 0;
        tlst->pixmaps = NULL;
    }
}


// Creates a pixmap chain for a given MDD object
int rviewThumb::pixmapsFromMDD(rviewThumbList *tlst)
{
    rviewThumbPixList *newPixmap, *lastPixmap;
    int i, projval;
    bool pixLoop;

    //cout << "pixmapsFromMDD" << endl;

    // delete old pixmaps
    deletePixmapChain(tlst);

    lastPixmap = NULL;

    if (dimproj >= 0)
    {
        projval = pt1[dimproj];
    }

    // Create all thumbnails for this object. This is 1 for 2D objects, any number for 3+D objects
    do
    {
        pixLoop = FALSE;
        newPixmap = new rviewThumbPixList;
        newPixmap->next = NULL;
        newPixmap->dimproj = dimproj;
        newPixmap->projval = projval;

        if (lastPixmap == NULL)
        {
            tlst->pixmaps = newPixmap;
        }
        else
        {
            lastPixmap->next = newPixmap;
        }
        lastPixmap = newPixmap;

        try
        {
            if ((newPixmap->pixmap = buildThumbnail(tlst->mdd, tlst->baseType, newPixmap->dimproj, newPixmap->projval)) == NULL)
            {
                cerr << lman->lookup("errorProjThumb") << endl;
                return 0;
            }
        }
        catch (r_Error &errObj)
        {
            cerr << errObj.what() << endl;
            return 0;
        }

        (tlst->numPix)++;
        numPixmaps++;

        i = newPixmap->pixmap->getHeight();
        if (i > maxHeight)
        {
            maxHeight = i;
        }

        if (dimproj >= 0)
        {
            projval += projstep;
            if (projval <= pt2[dimproj]) pixLoop = TRUE;
        }

    }
    while (pixLoop);

    return 1;
}



int rviewThumb::addMDD(r_Ref<r_GMarray> &newMdd)
{
    rviewThumbList *newItem;
    int i;
    r_Object *mo;
    bool oldCstate;

    RMDBGONCE(3, RMDebug::module_applications, "rviewThumb", "addMDD(...)");

    if ((newItem = new rviewThumbList) == NULL) return 0;

    oldCstate = canDoCspace;

    // Init object-dependent internal data?
    if (projString[0] == '\0')
    {
        // No ==> init to a decent default:
        initForObject(newMdd);
    }
    mo = (r_Object*)(&(*newMdd));
    newItem->mdd = newMdd;
    newItem->baseType = rviewGetBasetype(mo);
    newItem->numPix = 0;
    newItem->next = NULL;
    newItem->pixmaps = NULL;

    if (pixmapsFromMDD(newItem) == 0)
    {
        deletePixmapChain(newItem);
        delete newItem;
        return 0;
    }

    // If it was true before it's true now.
    if (oldCstate)
        canDoCspace = TRUE;
    else
    {
        // Otherwise try the transition FALSE -> TRUE
        if (canDoCspace)
        {
            // Enabling the parent automatically enables the children too, so reset them...
            mbar->Enable(MENU_THUMB_SETUP_CSPACE, TRUE);
            mbar->Enable(MENU_THUMB_CSPACE_FULL, doValToCspace);
            mbar->Enable(MENU_THUMB_CSPACE_EDIT, doValToCspace);
        }
    }

    if (thumbs == 0)
    {
        listHead = newItem;
    }
    else
    {
        rviewThumbList *tlst = listHead;

        for (i=1; i<thumbs; i++) tlst = tlst->next;

        tlst->next = newItem;
    }

    thumbs++;

    updateCanvasSize();

    return 1;
}


int rviewThumb::deleteMDD(r_Ref<r_GMarray> &obsMdd)
{
    rviewThumbList *last, *tlst;
    int i;

    RMDBGONCE(3, RMDebug::module_applications, "rviewThumb", "deleteMDD(...)");

    last = NULL;
    tlst = listHead;

    for (i=0; i<thumbs; i++)
    {
        if (tlst->mdd == obsMdd) break;
        last = tlst;
        tlst = tlst->next;
    }
    if (i < thumbs)
    {
        deletePixmapChain(tlst);

        if (last == NULL)
        {
            listHead = tlst->next;
        }
        else
        {
            last->next = tlst->next;
        }
        delete tlst;
        thumbs--;

        // Removing a thumbnail means a new layout and new bounding boxes
        maxHeight = 0;
        tlst = listHead;
        for (i=0; i<thumbs; i++)
        {
            int ph;
            rviewThumbPixList *tplst = tlst->pixmaps;

            while (tplst != NULL)
            {
                ph = tplst->pixmap->getHeight();
                if (ph > maxHeight) maxHeight = ph;
                tplst = tplst->next;
            }
            tlst = tlst->next;
        }
        // Close thumbnail viewer when the last thumbnail has been deleted
        if (thumbs <= 0)
        {
            this->Close(TRUE);
        }
        else
        {
            updateCanvasSize();
        }
        return 1;
    }
    return 0;
}


void rviewThumb::setLayout(int width, int npl)
{
    imgWidth = width;
    thumbsperline = npl;
    updateCanvasSize();
}


void rviewThumb::label(void)
{
    SetTitle(lman->lookup("titleThumb"));
    mbar->SetLabelTop(0, lman->lookup("menThumbData"));
    mbar->SetLabel(MENU_THUMB_DATA_CLOSE, lman->lookup("menThumbDataClose"));
    mbar->SetLabelTop(1, lman->lookup("menThumbSetup"));
    mbar->SetLabel(MENU_THUMB_SETUP_CSPACE, lman->lookup("menCspaceTitle"));
    mbar->SetLabel(MENU_THUMB_CSPACE_ON, lman->lookup("menCspaceOn"));
    mbar->SetLabel(MENU_THUMB_CSPACE_FULL, lman->lookup("menCspaceFull"));
    mbar->SetLabel(MENU_THUMB_CSPACE_EDIT, lman->lookup("menCspaceEdit"));

    project->SetLabel(lman->lookup("textProjString"));
    thumbProj->SetLabel(lman->lookup("textThumbProjDim"));
    thumbStep->SetLabel(lman->lookup("textThumbProjStep"));
    thumbWidth->SetLabel(lman->lookup("textThumbWidth"));
    thumbCols->SetLabel(lman->lookup("textThumbColumns"));
}


int rviewThumb::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
    {
        char buffer[STRINGSIZE];
        int newVal;

        if (&obj == (wxObject*)thumbWidth)
        {
            newVal = atoi(thumbWidth->GetValue());
            if ((newVal < thumb_minwidth) || (newVal > thumb_maxwidth))
            {
                sprintf(buffer, "%d", imgWidth);
                thumbWidth->SetValue(buffer);
            }
            else
            {
                newThumbWidth(newVal);
            }
            return 1;
        }
        if (&obj == (wxObject*)thumbCols)
        {
            newVal = atoi(thumbCols->GetValue());
            if ((newVal < thumb_mincols) || (newVal > thumb_maxcols))
            {
                sprintf(buffer, "%d", thumbsperline);
                thumbCols->SetValue(buffer);
            }
            else
            {
                thumbsperline = newVal;
                updateCanvasSize();
            }
            return 1;
        }
        if (&obj == (wxObject*)project)
        {
            if (thumbs > 0)
            {
                unsigned int oldProj;
                int oldFrom, oldTo;

                oldProj = dimproj;
                if (dimproj >= 0)
                {
                    oldFrom = pt1[dimproj];
                    oldTo = pt2[dimproj];
                }
                strcpy(projString, project->GetValue());
                if (parseProjection(listHead->mdd) != 0)
                {
                    bool fromScratch = TRUE;

                    // Determine whether we have to rebuild everything from scratch or not
                    if ((int)oldProj == dimproj)
                    {
                        if (dimproj >= 0)
                        {
                            if ((pt1[dimproj] == oldFrom) && (pt2[dimproj] == oldTo)) fromScratch = TRUE;
                        }
                    }
                    rebuildThumbnails(fromScratch);
                }
            }
            return 1;
        }
        if (&obj == (wxObject*)thumbStep)
        {
            int oldStep = projstep;

            projstep = atoi(thumbStep->GetValue());
            if (projstep <= 0)
            {
                projstep = 1;
                thumbProj->SetValue("1");
            }
            if ((dimMDD > 2) && (oldStep != projstep)) rebuildThumbnails(TRUE);

            return 1;
        }
        if (&obj == (wxObject*)thumbProj)
        {
            if ((thumbs > 0) && (dimMDD > 2))
            {
                if (parseProjection(listHead->mdd) != 0)
                {
                    rebuildThumbnails(TRUE);
                }
            }
            return 1;
        }
    }
    return 0;
}


const char *rviewThumb::getFrameName(void) const
{
    return "rviewThumb";
}

rviewFrameType rviewThumb::getFrameType(void) const
{
    return rviewFrameTypeThumb;
}


void rviewThumb::OnSize(int w, int h)
{
    int x, y, tw, py, px;

    GetClientSize(&x, &y);
    if((x < thumb_width) || (y < thumb_height))
    {
        frameWidth = x;
        frameHeight = y;
        SetClientSize(thumb_width, thumb_height);
        return;
    }

    panel->SetSize(0, thumb_border, x, thumb_cheight);
    x -= 2*thumb_border;
    y -= 2*thumb_border + thumb_cheight;
    canvas->SetSize(thumb_border, thumb_border + thumb_cheight, x, y);

    tw = (x - 2*thumb_prjwidth - 2*thumb_twidth - 6*thumb_border);
    py = thumb_border;
    px = thumb_border;
    project->SetSize(px, py, tw , thumb_theight);
    px +=thumb_border + tw;
    thumbProj->SetSize(px, py, thumb_prjwidth, thumb_theight);
    px +=thumb_prjwidth + thumb_border;
    thumbStep->SetSize(px, py, thumb_prjwidth, thumb_theight);
    px +=thumb_prjwidth + thumb_border;
    thumbWidth->SetSize(px, py, thumb_twidth, thumb_theight);
    px +=thumb_twidth + thumb_border;
    thumbCols->SetSize(px, py, thumb_twidth, thumb_theight);
}


void rviewThumb::OnMenuCommand(int id)
{
    switch (id)
    {
    case MENU_THUMB_DATA_CLOSE:
        this->Close(TRUE);
        break;
    case MENU_THUMB_CSPACE_EDIT:
        csmap->openEditor();
        break;
    case MENU_THUMB_CSPACE_ON:
    {
        rviewThumbList *tlst = listHead;
        int i;

        // Don't allow switching off cspace mapping if at least one object is float/double.
        for (i=0; i<thumbs; i++)
        {
            if ((tlst->baseType == rbt_float) || (tlst->baseType == rbt_double)) break;
            tlst = tlst->next;
        }
        if (i < thumbs)
        {
            mbar->Check(MENU_THUMB_CSPACE_ON, TRUE);
        }
        else
        {
            doValToCspace = mbar->Checked(MENU_THUMB_CSPACE_ON);
            mbar->Enable(MENU_THUMB_CSPACE_FULL, doValToCspace);
            mbar->Enable(MENU_THUMB_CSPACE_EDIT, doValToCspace);
            rebuildThumbnails(FALSE);
        }
    }
    break;
    case MENU_THUMB_CSPACE_FULL:
        doFullRangeCspace = mbar->Checked(MENU_THUMB_CSPACE_FULL);
        if (csmap != NULL)
        {
            csmap->processRange((doFullRangeCspace) ? CSPACE_RANGE_FULL : CSPACE_RANGE_ACTUAL);
        }
        rebuildThumbnails(FALSE);
        break;
    default:
        break;
    }
}


int rviewThumb::userEvent(const user_event &ue)
{
    if (ue.type == usr_mdd_dying)
    {
        return deleteMDD(*((r_Ref<r_GMarray>*)(ue.data)));
    }
    if (ue.type == usr_cspace_changed)
    {
        if (ue.data == (void*)csmap)
        {
            rebuildThumbnails(FALSE);
            return 1;
        }
    }
    return 0;
}


// Return the pixmap with number no and write a textual description into
// caption which has to be at least STRINGSIZE bytes large.
wxPixmap *rviewThumb::getPixmapNumber(int no, char *caption)
{
    rviewThumbList *tlst = listHead;
    int major, minor;
    wxPixmap *retPix = NULL;

    if (no >= numPixmaps) return NULL;

    // major = number of MDD object, minor = number of pixmap for this object
    major = 0;
    minor = 0;

    while ((tlst != NULL) && (no > 0))
    {
        minor = 0;
        if (no < tlst->numPix)
        {
            rviewThumbPixList *tplst = tlst->pixmaps;
            while ((tplst != NULL) && (no > 0))
            {
                tplst = tplst->next;
                minor++;
                no--;
            }
            if (tplst == NULL)
            {
                retPix = NULL;
                break;
            }
            else
            {
                //cout << "found " << (void*)(tplst->pixmap) << endl;
                retPix = tplst->pixmap;
                break;
            }
        }
        else
        {
            no -= tlst->numPix;
            tlst = tlst->next;
            major++;
        }
    }
    if ((retPix == NULL) && (tlst != NULL))
    {
        if ((no == 0) && (tlst->numPix > 0))
        {
            retPix = tlst->pixmaps->pixmap;
        }
    }

    if (retPix != NULL)
    {
        sprintf(caption, "MDD %d [%d]", major, minor*projstep);
    }

    return retPix;
}


void rviewThumb::getThumbInfo(int &num, int &npl)
{
    num = numPixmaps;
    npl = thumbsperline;
}


void rviewThumb::getGridInfo(int &gx, int &gy)
{
    gx = gridX;
    gy = gridY;
}


void rviewThumb::updateCanvasSize(void)
{
    int stepx, stepy;
    int scrollx, scrolly;

    RMDBGONCE(3, RMDebug::module_applications, "rviewThumb", "updateCanvasSize()");

    gridX = imgWidth + thumb_space;
    gridY = maxHeight + thumb_space;

    scrollx = canvas->GetScrollPos(wxHORIZONTAL);
    scrolly = canvas->GetScrollPos(wxVERTICAL);

    stepx = (gridX * thumbsperline + thumb_scrstep - 1) / thumb_scrstep;
    if ((numPixmaps == 0) || (thumbsperline == 0))
    {
        stepy = 1;
        canvas->EnableScrolling(TRUE, FALSE);
        canvas->SetScrollRange(wxVERTICAL, 0);
    }
    else
    {
        stepy = (((numPixmaps + thumbsperline - 1) / thumbsperline) * gridY + thumb_scrstep - 1) / thumb_scrstep;
        canvas->EnableScrolling(TRUE, TRUE);
    }
    canvas->SetScrollbars(thumb_scrstep, thumb_scrstep, stepx, stepy, thumb_pgstep, thumb_pgstep, scrollx, scrolly);
}



// Init internal variables (e.g. projection-related) according to a sample object
void rviewThumb::initForObject(r_Ref<r_GMarray> &mddObj)
{
    char *data;
    int i;
    r_Minterval interv;

    interv = mddObj->spatial_domain();
    dimMDD = interv.dimension();
    // freeDims is a bitfield specifying which dimensions are free
    if (dimMDD <= 2)
    {
        freeDims = 3;
    }
    else
    {
        if ((dimproj == -1) || (dimproj >= dimMDD)) freeDims = 3;
        else
        {
            if (dimproj == 0) freeDims = 6;
            else if (dimproj == 1) freeDims = 5;
            else freeDims = 3;
        }
    }
    pt1 = r_Point(dimMDD);
    pt2 = r_Point(dimMDD);
    data = projString;
    for (i=0; i<dimMDD; i++)
    {
        pt1[i] = interv[i].low();
        if ((freeDims & (1<<i)) == 0)
        {
            data += sprintf(data, "%d, ", interv[i].low());
            pt2[i] = interv[i].low();
        }
        else
        {
            data += sprintf(data, "*:*, ");
            pt2[i] = interv[i].high();
        }
    }
    data[-2] = 0; // cut the final ", "
    project->SetValue(projString);
    parseProjection(mddObj);
}


// Parse the projection string and set up all internal variables that depend on it.
int rviewThumb::parseProjection(r_Ref<r_GMarray> &mddObj)
{
    int x, h;

    mapIndex = r_Point(dimMDD);
    if (rviewParseProjection(mddObj->spatial_domain(), pt1, pt2, projString, &freeDims, &mapIndex) != dimMDD)
    {
        cerr << lman->lookup("errorProjection") << endl;
        rviewErrorbox eb(lman->lookup("errorProjection"));
        eb.activate();
        return 0;
    }

    dimproj = -1;
    dim1 = -1;
    dim2 = -1;
    for (x=0; x<dimMDD; x++)
    {
        if ((freeDims & (1<<x)) != 0)
        {
            if (dim1 < 0)
                dim1 = x;
            else if (dim2 < 0)
                dim2 = x;
            else if (dimproj < 0)
                dimproj = x;
            else
                break;
        }
    }
    if (x < dimMDD)
    {
        cerr << lman->lookup("errorProjThumb") << endl;
        return 0;
    }
    //cout << "parseProj: dim1 " << dim1 << ", dim2 " << dim2 << ", dimproj " << dimproj << endl;
    // are there 3 free dimensions to begin with?
    if (dimproj >= 0)
    {
        // user-specified projection value overrides default (last free one)
        h = atoi(thumbProj->GetValue());
        // Now swap around the dimensions as required
        if (h == dim1)
        {
            dim1 = dim2;
            dim2 = dimproj;
            dimproj = h;
        }
        else if (h == dim2)
        {
            dim2 = dimproj;
            dimproj = h;
        }
        // otherwise just keep the default dimproj value
    }
    dim1 = mapIndex[dim1];
    dim2 = mapIndex[dim2];

    // Also read the other variables that can change the appearance of the thumbnails
    h = atoi(thumbWidth->GetValue());
    if ((h >= thumb_minwidth) && (h <= thumb_maxwidth))
    {
        imgWidth = h;
    }
    h = atoi(thumbStep->GetValue());
    if (h > 0)
    {
        projstep = h;
    }

    return 1;
}




wxPixmap *rviewThumb::buildThumbnail(r_Ref<r_GMarray> &mddObj, rviewBaseType baseType, int dimproject, int projval)
{
    char *data;
    wxPixmap *pixmap;
    wxColour palette[2];
    int baseSize;
    int x;
    r_Minterval thisInterv;
    int objdim;
    int srcWidth;
    r_Point ptlow, pthigh;

    RMDBGENTER(3, RMDebug::module_applications, "rviewThumb", "buildThumbnail(...)");

    //cout << "thumbnail " << dimproject << ", " << projval << endl;

    thisInterv = mddObj->spatial_domain();
    objdim = thisInterv.dimension();

    // Only data with at least 2 dimensions is allowed
    if (objdim < 2)
        return NULL;

    // Check if all the thumbnails have the same number of dimensions
    if (dimMDD != objdim)
    {
        cerr << "Warning: inconsistent number of dimensions in collection!" << endl;
    }

    // Make sure the points we use are within the spatial domain of this object, in case
    // the objects in this collection don't have a uniform spatial domain.
    ptlow = r_Point(dimMDD);
    pthigh = r_Point(dimMDD);
    for (x=0; x<dimMDD; x++)
    {
        if (pt1[x] >= thisInterv[x].low()) ptlow[x] = pt1[x];
        else ptlow[x] = thisInterv[x].low();
        if (pt2[x] <= thisInterv[x].high()) pthigh[x] = pt2[x];
        else pthigh[x] = thisInterv[x].high();
    }
    //cout << "Corners " << pt1 << ", " << pt2 << endl;
    //cout << "low " << ptlow << ", high " << pthigh << endl;

    baseSize = (int)(mddObj->get_type_length());

    if ((baseType == rbt_float) || (baseType == rbt_double))
    {
        doValToCspace = TRUE;
        mbar->Check(MENU_THUMB_CSPACE_ON, TRUE);
        configureCspace(TRUE);
    }

    rviewFlatProjEnv penv;

    penv.mddPtr = mddObj.ptr();
    penv.pt1 = ptlow;
    penv.pt2 = pthigh;
    penv.dim1 = dim1;
    penv.dim2 = dim2;
    penv.bt = baseType;
    penv.doCspace =  doValToCspace;

    if (dimproject >= 0)
    {
        penv.pt1[dimproject] = projval;
        penv.pt2[dimproject] = projval;
    }
    srcWidth = (int)(pthigh[dim1] - ptlow[dim1] + 1);
    //cout << "srcWidth " << srcWidth << endl;
    if (srcWidth < 1) srcWidth = 1;
    penv.scale = ((double)imgWidth) / srcWidth;
    //cout << "dimproject " << dimproject << ", projval " << projval
    //     << ", scale " << penv.scale << endl;
    //cout << "penv " << penv.pt1 << ", " << penv.pt2 << endl;

    if (rviewPrepareFlatProjection(penv) != 0) return NULL;

    if (rviewImageTypes[baseType] == RVIEW_IMGTYPE_MONO)
    {
        palette[0] = wxColour(0,0,0);
        palette[1] = wxColour(255,255,255);
    }

    // Colourspace mapping possible for this object?
    if (rviewCheckInitCspace(baseType, NULL, mddObj) != 0) canDoCspace = TRUE;

    if (doValToCspace)
    {
        penv.cspaceState = rviewCheckInitCspace(baseType, &csmap, mddObj, doFullRangeCspace, NULL, penv.width, &penv.pitch, &penv.depth, &penv.pad);
        if (csmap != NULL) mbar->Enable(MENU_THUMB_CSPACE_EDIT, TRUE);
    }
    else
        penv.cspaceState = 0;

    penv.csmap = csmap;

    /*if (penv.pitch * penv.height >= 16*1024*1024)
      return NULL;*/

    if ((data = (char*)malloc(penv.pitch * penv.height)) == NULL)
        return NULL;

    if (rviewPerformFlatProjection(penv, data) != 0)
    {
        free(data);
        return NULL;
    }

    pixmap = new wxPixmap((wxWindow*)canvas, penv.width, penv.height, penv.depth, penv.pad, data, rviewImage::getPixmapFlags(), (rviewImageTypes[baseType] == RVIEW_IMGTYPE_MONO) ? palette : NULL);

    RMDBGEXIT(3, RMDebug::module_applications, "rviewThumb", "buildThumbnail(...) Pixmap " << (void*)pixmap );

    return pixmap;
}




void rviewThumb::newThumbWidth(int newWidth)
{
    if (newWidth == imgWidth) return;
    imgWidth = newWidth;

    rebuildThumbnails(FALSE);
}



void rviewThumb::rebuildThumbnails(bool fromScratch)
{
    int i, j;
    rviewThumbList *tlst;
    rviewThumbPixList *tplst;

    //cout << "rebuildThumbnails " << fromScratch << endl;

    tlst = listHead;

    if (fromScratch)
    {
        // Delete all thumbnail pixmaps, including the chain structure
        for (i=0; i<thumbs; i++, tlst = tlst->next)
        {
            deletePixmapChain(tlst);
        }
        maxHeight = 0;
        for (i=0, tlst=listHead; i<thumbs; i++, tlst=tlst->next)
        {
            if (pixmapsFromMDD(tlst) == 0)
            {
                cerr << lman->lookup("errorProjThumb") << endl;
            }
        }
    }
    else
    {
        // 1) Delete all the thumbnail pixmaps, but leave the chain intact!
        for (i=0; i<thumbs; i++, tlst = tlst->next)
        {
            tplst = tlst->pixmaps;
            while (tplst != NULL)
            {
                if (tplst->pixmap != NULL) delete tplst->pixmap;
                tplst->pixmap = NULL;
                tplst = tplst->next;
            }
        }
        // 2) Create new ones. Doing this in two passes is better for the memory allocation
        for (i=0, tlst=listHead; i<thumbs; i++, tlst = tlst->next)
        {
            tplst = tlst->pixmaps;
            while (tplst != NULL)
            {
                if ((tplst->pixmap = buildThumbnail(tlst->mdd, tlst->baseType, tplst->dimproj, tplst->projval)) == NULL)
                {
                    cerr << lman->lookup("errorProjThumb") << endl;
                }
                tplst = tplst->next;
            }
        }
    }

    // Now adapt the maximum height
    maxHeight = 0;
    tlst = listHead;
    for (i=0; i<thumbs; i++, tlst=tlst->next)
    {
        tplst = tlst->pixmaps;
        while (tplst != NULL)
        {
            if (tplst->pixmap != NULL)
            {
                j = tplst->pixmap->getHeight();
                if (j > maxHeight) maxHeight = j;
            }
            tplst = tplst->next;
        }
    }

    updateCanvasSize();
}
