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
 *  Purpose:
 *
 *  rView graphical display modes (flat and rendered images). Renderers
 *  provided by the cube_render sources, pixmap interface provided by
 *  the new wxWindows class wxPixmap. Also includes the ColourspaceMapper
 *  object.
 *
 *  COMMENTS:
 *          None
 */



// Standard wxWindows preamble.
#ifdef __GNUG__
#pragma implementation
#endif


#include <wx/wx_prec.h>


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


// I hate Windows...
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif


#include "raslib/rmdebug.hh"
#include "rasodmg/fastscale.hh"


// For information about bit order and so on
#include "wx_pixmap.h"
#include "wx_pixmap_translate.h"

#include "cube_render.h"

#include "rviewTypes.hh"

#include "labelManager.hh"

#include "rviewUtils.hh"
#include "rviewMDD.hh"
#include "rviewDModes.hh"
#include "rviewColMap.hh"
#include "rviewPrefs.hh"
#include "rviewIO.hh"
#include "rviewDb.hh"








// Table mapping base types to visualisztion types
int rviewImageTypes[] =
{
    RVIEW_IMGTYPE_NONE,   // rbt_none
    RVIEW_IMGTYPE_MONO,   // rbt_bool
    RVIEW_IMGTYPE_GREY,   // rbt_char
    RVIEW_IMGTYPE_GREY,   // rbt_uchar
    RVIEW_IMGTYPE_GREY12, // rbt_short
    RVIEW_IMGTYPE_HIGH,   // rbt_ushort
    RVIEW_IMGTYPE_TRUE32, // rbt_long
    RVIEW_IMGTYPE_TRUE32, // rbt_ulong
    RVIEW_IMGTYPE_TRUE24, // rbt_rgb
    RVIEW_IMGTYPE_NONE,   // rbt_float
    RVIEW_IMGTYPE_NONE    // rbt_double
};






/*
 *  rviewImageSetup members
 */

const int rviewImageSetup::imgset_border = rviewDisplay::display_border;
const int rviewImageSetup::imgset_theight = 30;
const int rviewImageSetup::imgset_chkheight = 24;
const int rviewImageSetup::imgset_renheight = 206;
const int rviewImageSetup::imgset_voxheight = 256;
const int rviewImageSetup::imgset_hgtheight = 60;
const int rviewImageSetup::imgset_bwidth = 80;
const int rviewImageSetup::imgset_bheight = 40;
const int rviewImageSetup::imgset_chowidth = 130;
const int rviewImageSetup::imgset_choheight = 30;
const int rviewImageSetup::imgset_width = 400;
const int rviewImageSetup::imgset_height = rviewImageSetup::imgset_renheight + rviewImageSetup::imgset_voxheight + rviewImageSetup::imgset_hgtheight + rviewImageSetup::imgset_bheight + 5*rviewImageSetup::imgset_border + rview_window_extra_height;


const char *rviewImageSetup::normalKernelSizes[] =
{
    "0",
    "1",
    "2",
    "3",
    NULL
};

const keyword_to_ident_c rviewImageSetup::normalKernelTypes[] =
{
    {RENDER_NORM_KERNEL_HOMO, "kernelTypeAvg"},
    {RENDER_NORM_KERNEL_LINEAR, "kernelTypeLin"},
    {RENDER_NORM_KERNEL_GAUSS, "kernelTypeGauss"},
    {0, NULL}
};

rviewImageSetup::rviewImageSetup(rview_image_setup *ris, rviewRenderImage *parentWin) : rviewFrame(NULL, lman->lookup("titleImgSetup"), 0, 0, imgset_width, imgset_height)
{
    char buffer[STRINGSIZE];
    int w, h, x, y, off, posx, posy;
    char **ktypes;
    char *b;
    int i;

    parent = parentWin;
    imgSetup = ris;
    memcpy(&oldSetup, ris, sizeof(rview_image_setup));

    GetClientSize(&w, &h);
    panel = new wxPanel((wxWindow*)this, 0, 0, w, h);
    w -= 2*imgset_border;
    posx = imgset_border;
    posy = imgset_border;
    renderGroup = new wxGroupBox(panel, "", posx, posy, w, imgset_renheight);
    posy += imgset_renheight + imgset_border;
    voxelGroup = new wxGroupBox(panel, "", posx, posy, w, imgset_voxheight);
    posy += imgset_voxheight + imgset_border;
    heightGroup = new wxGroupBox(panel, "", posx, posy, w, imgset_hgtheight);

    w -= 2*imgset_border;
    y = imgset_border;
    posx += imgset_border;
    renderGroup->GetClientSize(&x, &h);
    off = y + imgset_renheight - h;
    h = (h - 5*imgset_theight) / 5;

    zproWidget = new rviewText(panel);
    clipzWidget = new rviewText(panel);
    useLights = new rviewCheckBox(panel);
    lightsAngle = new rviewText(panel);
    lightsScintAngle = new rviewText(panel);
    lightsAmbient = new rviewText(panel);
    lightsGain = new rviewText(panel);
    lightsDir = new rviewText(panel);
    lightsDist = new rviewText(panel);
    pixThreshLowWidget = new rviewText(panel);
    pixThreshHighWidget = new rviewText(panel);
    wgtThreshWidget = new rviewText(panel);
    wgtQuantWidget = new rviewText(panel);
    kernelSize = new rviewChoice(panel, 4, (char**)normalKernelSizes, lman->lookup("imgSetKernSize"));
    ktypes = new char *[3];
    b = buffer;
    for (i=0; i<3; i++)
    {
        ktypes[i] = b;
        b += 1 + sprintf(b, "%s", lman->lookup(normalKernelTypes[i].keyword));
    }
    kernelType = new rviewChoice(panel, 3, ktypes, lman->lookup("imgSetKernType"));
    delete [] ktypes;
    useVoxCol = new rviewCheckBox(panel);
    useRgbBrightness = new rviewCheckBox(panel);
    voxColour = new rviewText(panel);
    gridSize = new rviewText(panel);
    scaleHeight = new rviewText(panel);

    okBut = new rviewButton(panel);
    cancelBut = new rviewButton(panel);

    label();

    updateSettings(*ris);

    frameWidth=-1;
    frameHeight=-1;

    OnSize(imgset_width, imgset_height);
    OnSize(imgset_width, imgset_height);

    Show(TRUE);
}



rviewImageSetup::~rviewImageSetup(void)
{
    if (parent != NULL)
    {
        user_event ue;

        ue.type = usr_child_closed;
        ue.data = (void*)this;
        parent->userEvent(ue);
    }
}


const char *rviewImageSetup::getFrameName(void) const
{
    return "rviewImageSetup";
}

rviewFrameType rviewImageSetup::getFrameType(void) const
{
    return rviewFrameTypeImgSet;
}


void rviewImageSetup::unlinkParent(void)
{
    parent = NULL;
}


void rviewImageSetup::updateSettings(const rview_image_setup &ris)
{
    char buffer[STRINGSIZE];
    char *b;
    int i;

    zproWidget->SetValue((long)(ris.zpro));
    clipzWidget->SetValue((long)(ris.clipz));
    useLights->SetValue(ris.useLights);
    lightsAngle->SetValue(ris.lightsAngle);
    lightsScintAngle->SetValue(ris.lightsScintAngle);
    lightsAmbient->SetValue(ris.lightsAmbient);
    lightsGain->SetValue(ris.lightsGain);
    lightsDist->SetValue(ris.lightsDist);
    pixThreshLowWidget->SetValue(ris.pixelThresholdLow, TRUE);
    pixThreshHighWidget->SetValue(ris.pixelThresholdHigh, TRUE);
    wgtThreshWidget->SetValue(ris.weightThreshold, TRUE);
    wgtQuantWidget->SetValue(ris.weightQuantisation);
    useRgbBrightness->SetValue(ris.useRgbBrightness);
    useVoxCol->SetValue(ris.useVoxCol);
    voxColour->SetValue(ris.voxColour);
    gridSize->SetValue(ris.gridSize);
    scaleHeight->SetValue(ris.scaleHeight);

    if ((ris.kernelSize >= 0) && (ris.kernelSize < 4))
    {
        kernelSize->SetSelection(ris.kernelSize);
    }
    for (i=0; normalKernelTypes[i].keyword != NULL; i++)
    {
        if (normalKernelTypes[i].ident == ris.kernelType)
        {
            kernelType->SetSelection(i);
            break;
        }
    }
    b = buffer;
    if ((ris.lightsDir & RVIEW_LIGHTDIR_LEFT) != 0) *b++ = 'l';
    else if ((ris.lightsDir & RVIEW_LIGHTDIR_RIGHT) != 0) *b++ = 'r';
    if ((ris.lightsDir & RVIEW_LIGHTDIR_DOWN) != 0) *b++ = 'd';
    else if ((ris.lightsDir & RVIEW_LIGHTDIR_UP) != 0) *b++ = 'u';
    if ((ris.lightsDir & RVIEW_LIGHTDIR_FRONT) != 0) *b++ = 'f';
    else if ((ris.lightsDir & RVIEW_LIGHTDIR_BACK) != 0) *b++ = 'b';
    *b++ = '\0';
    lightsDir->SetValue(buffer);
}



int rviewImageSetup::parseLightDirection(const char *dir)
{
    char *b;
    int val;

    b = (char*)dir;
    val = 0;
    while (*b != '\0')
    {
        switch (*b)
        {
        case 'l':
            val |= RVIEW_LIGHTDIR_LEFT;
            break;
        case 'r':
            val |= RVIEW_LIGHTDIR_RIGHT;
            break;
        case 'd':
            val |= RVIEW_LIGHTDIR_DOWN;
            break;
        case 'u':
            val |= RVIEW_LIGHTDIR_UP;
            break;
        case 'f':
            val |= RVIEW_LIGHTDIR_FRONT;
            break;
        case 'b':
            val |= RVIEW_LIGHTDIR_BACK;
            break;
        default:
            break;
        }
        b++;
    }
    return val;
}


void rviewImageSetup::readNewSetup(void)
{
    long val;

    val = asctoi(zproWidget->GetValue());
    if (val > 0) imgSetup->zpro = (unsigned long)val;
    val = asctoi(clipzWidget->GetValue());
    if (val > 0) imgSetup->clipz = (unsigned long)val;
    imgSetup->useLights = useLights->GetValue();
    imgSetup->lightsAngle = atof(lightsAngle->GetValue());
    imgSetup->lightsScintAngle = atof(lightsScintAngle->GetValue());
    imgSetup->lightsAmbient = atof(lightsAmbient->GetValue());
    imgSetup->lightsGain = atof(lightsGain->GetValue());
    imgSetup->lightsDist = asctoi(lightsDist->GetValue());
    imgSetup->pixelThresholdLow = asctof(pixThreshLowWidget->GetValue());
    imgSetup->pixelThresholdHigh = asctof(pixThreshHighWidget->GetValue());
    imgSetup->weightThreshold = asctof(wgtThreshWidget->GetValue());
    imgSetup->useRgbBrightness = useRgbBrightness->GetValue();
    val = asctoi(wgtQuantWidget->GetValue());
    if (val >= 0) imgSetup->weightQuantisation = (int)val;
    imgSetup->kernelSize = kernelSize->GetSelection();
    imgSetup->kernelType = normalKernelTypes[kernelType->GetSelection()].ident;
    imgSetup->useVoxCol = useVoxCol->GetValue();
    imgSetup->voxColour = asctof(voxColour->GetValue());
    imgSetup->lightsDir = parseLightDirection(lightsDir->GetValue());
    imgSetup->gridSize = asctoi(gridSize->GetValue());
    imgSetup->scaleHeight = atof(scaleHeight->GetValue());
}


void rviewImageSetup::label(void)
{
    SetTitle(lman->lookup("titleImgSetup"));

    renderGroup->SetLabel(lman->lookup("imgSetRender"));
    voxelGroup->SetLabel(lman->lookup("imgSetVoxel"));
    heightGroup->SetLabel(lman->lookup("imgSetHeight"));
    zproWidget->SetLabel(lman->lookup("imgSetRenZpro"));
    clipzWidget->SetLabel(lman->lookup("imgSetRenClipz"));
    useLights->SetLabel(lman->lookup("imgSetRenUseLight"));
    lightsAngle->SetLabel(lman->lookup("imgSetRenLightAn"));
    lightsScintAngle->SetLabel(lman->lookup("imgSetRenLightSc"));
    lightsAmbient->SetLabel(lman->lookup("imgSetRenLightAm"));
    lightsGain->SetLabel(lman->lookup("imgSetRenLightGn"));
    lightsDir->SetLabel(lman->lookup("imgSetRenLightDr"));
    lightsDist->SetLabel(lman->lookup("imgSetRenLightDs"));
    pixThreshLowWidget->SetLabel(lman->lookup("imgSetVoxPixThreshLow"));
    pixThreshHighWidget->SetLabel(lman->lookup("imgSetVoxPixThreshHigh"));
    wgtThreshWidget->SetLabel(lman->lookup("imgSetVoxWgtThresh"));
    wgtQuantWidget->SetLabel(lman->lookup("imgSetVoxWgtQuant"));
    useRgbBrightness->SetLabel(lman->lookup("imgSetVoxRgbBright"));
    kernelSize->SetLabel(lman->lookup("imgSetKernSize"));
    kernelType->SetLabel(lman->lookup("imgSetKernType"));
    useVoxCol->SetLabel(lman->lookup("imgSetUseVCol"));
    voxColour->SetLabel(lman->lookup("imgSetVoxCol"));
    gridSize->SetLabel(lman->lookup("imgSetGridSize"));
    scaleHeight->SetLabel(lman->lookup("imgSetHgtScale"));
    okBut->SetLabel(lman->lookup("textOK"));
    cancelBut->SetLabel(lman->lookup("textCancel"));
}



void rviewImageSetup::OnSize(int w, int h)
{
    int x, y, width, height, off, posx, posy;

    GetClientSize(&width, &height);

    //need to resize?
    if (( imgset_width != width) || ( imgset_height != height))
    {
        frameWidth =  imgset_width;
        frameHeight =  imgset_height;
        width=  imgset_width;
        height =  imgset_height;
        SetClientSize(width, height);
        return;
    }

    panel->SetSize(0, 0, width, height);
    width -= 2*imgset_border;
    height -= 2*imgset_border;
    posx = imgset_border;
    posy = imgset_border;
    renderGroup->SetSize(posx, posy, width, imgset_renheight);
    posy += imgset_renheight + imgset_border;
    voxelGroup->SetSize(posx, posy, width, imgset_voxheight);
    posy += imgset_voxheight + imgset_border;
    heightGroup->SetSize(posx, posy, width, imgset_hgtheight);

    width -= 2*imgset_border;
    y = imgset_border;
    posx += imgset_border;
    posy += imgset_border;
    renderGroup->GetClientSize(&x, &height);
    off = y + imgset_renheight - height;
    height = (height - 5*imgset_theight) / 5;
    posy = off + height/2;
    zproWidget->SetSize(posx, posy, width/2 - imgset_border, imgset_theight, wxTE_PROCESS_ENTER);
    clipzWidget->SetSize(posx + width/2, posy, width/2, imgset_theight, wxTE_PROCESS_ENTER);
    posy += height + imgset_theight;
    useLights->SetSize(posx, posy, width/2, imgset_theight);
    posy += height + imgset_theight;
    lightsAngle->SetSize(posx, posy, width/2 - imgset_border, imgset_theight, wxTE_PROCESS_ENTER);
    lightsScintAngle->SetSize(posx + width/2, posy, width/2, imgset_theight, wxTE_PROCESS_ENTER);
    posy += height + imgset_theight;
    lightsAmbient->SetSize(posx, posy, width/2 - imgset_border, imgset_theight, wxTE_PROCESS_ENTER);
    lightsGain->SetSize(posx + width/2, posy, width/2, imgset_theight, wxTE_PROCESS_ENTER);
    posy += height + imgset_theight;
    lightsDir->SetSize(posx, posy, width/2 - imgset_border, imgset_theight, wxTE_PROCESS_ENTER);
    lightsDist->SetSize(posx + width/2, posy, width/2, imgset_theight, wxTE_PROCESS_ENTER);

    y += imgset_renheight + imgset_border;
    voxelGroup->GetClientSize(&x, &height);
    off = y + imgset_voxheight - height;
    height = (height - 5*imgset_theight - imgset_chkheight - imgset_choheight) / 6;
    posy = off + height/2;
    pixThreshLowWidget->SetSize(posx, posy, width, imgset_theight, wxTE_PROCESS_ENTER);
    posy += height + imgset_theight;
    pixThreshHighWidget->SetSize(posx, posy, width, imgset_theight, wxTE_PROCESS_ENTER);
    posy += height + imgset_theight;
    wgtThreshWidget->SetSize(posx, posy, width, imgset_theight, wxTE_PROCESS_ENTER);
    posy += height + imgset_theight;
    wgtQuantWidget->SetSize(posx, posy, width, imgset_theight, wxTE_PROCESS_ENTER);
    posy += height + imgset_theight;
    kernelSize->SetSize(posx, posy, imgset_chowidth, imgset_choheight);
    kernelType->SetSize(width - imgset_chowidth - 3*imgset_border, posy, imgset_chowidth, imgset_choheight);
    posy += height + imgset_theight;
    useVoxCol->SetSize(posx, posy, width/2, imgset_theight);
    useRgbBrightness->SetSize(posx + width/2, posy, width/2, imgset_theight);
    posy += height + imgset_chkheight;
    voxColour->SetSize(posx, posy, width, imgset_theight);

    y += imgset_voxheight + imgset_border;
    heightGroup->GetClientSize(&x, &height);
    off = y + imgset_hgtheight - height;
    height = (height - imgset_theight);
    posy = off + height/2;
    gridSize->SetSize(posx, posy, width/2 - imgset_border, imgset_theight, wxTE_PROCESS_ENTER);
    scaleHeight->SetSize(posx + width/2, posy, width/2, imgset_theight, wxTE_PROCESS_ENTER);

    y += imgset_hgtheight + imgset_border;
    off = (width - 2*imgset_bwidth) / 2;
    okBut->SetSize(posx + off/2, y, imgset_bwidth, imgset_bheight);
    cancelBut->SetSize(posx + (3*off)/2 + imgset_bwidth, y, imgset_bwidth, imgset_bheight);
}



int rviewImageSetup::process(wxObject &obj, wxEvent &evt)
{
    int type;

    type = evt.GetEventType();
    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)okBut)
        {
            readNewSetup();
            if (parent != NULL) parent->closeEditor(TRUE);
            return 1;
        }
        else if (&obj == (wxObject*)cancelBut)
        {
            memcpy(imgSetup, &oldSetup, sizeof(rview_image_setup));
            if (parent != NULL) parent->closeEditor(TRUE);
            return 1;
        }
    }
    else if ((type == wxEVENT_TYPE_TEXT_ENTER_COMMAND) || (type == wxEVENT_TYPE_CHECKBOX_COMMAND))
    {
        int setFlags = -1;

        if ((&obj == (wxObject*)zproWidget) || (&obj == (wxObject*)clipzWidget) ||
                (&obj == (wxObject*)useLights)) setFlags = 0;
        // These only require an update of the view-window when voxel mode is on.
        else if ((&obj == (wxObject*)pixThreshLowWidget) || (&obj == (wxObject*)pixThreshHighWidget) ||
                 (&obj == (wxObject*)wgtThreshWidget) || (&obj == (wxObject*)wgtQuantWidget) ||
                 (&obj == (wxObject*)useRgbBrightness)) setFlags = RVIEW_IFLAG_VOXEL;
        // These only require an update of the view-window when lights are on.
        else if ((&obj == (wxObject*)useVoxCol) || (&obj == (wxObject*)lightsAngle) ||
                 (&obj == (wxObject*)lightsScintAngle) || (&obj == (wxObject*)lightsAmbient) ||
                 (&obj == (wxObject*)lightsGain) || (&obj == (wxObject*)lightsDir) ||
                 (&obj == (wxObject*)lightsDist)) setFlags = RVIEW_IFLAG_LIGHT;
        // These only require an update of the view-window when height mode is on
        else if ((&obj == (wxObject*)gridSize) || (&obj == (wxObject*)scaleHeight)) setFlags = RVIEW_IFLAG_HEIGHT;
        // Mixed modes
        else if (&obj == (wxObject*)voxColour) setFlags = RVIEW_IFLAG_LIGHT | RVIEW_IFLAG_HEIGHT;
        if (setFlags >= 0)
        {
            readNewSetup();
            if (parent != NULL) parent->updateSettings(setFlags);
            return 1;
        }
    }
    else if (type == wxEVENT_TYPE_CHOICE_COMMAND)
    {
        if ((&obj == (wxObject*)kernelSize) || (&obj == (wxObject*)kernelType))
        {
            readNewSetup();
            if (parent != NULL) parent->updateSettings(TRUE);
            return 1;
        }
    }

    return 0;
}





/*
 *  rendererControl class. For advanced control of the renderers + animations
 */

const int rendererControl::rctrl_border = rviewDisplay::display_border;
const int rendererControl::rctrl_bwidth = 80;
const int rendererControl::rctrl_bheight = 30;
const int rendererControl::rctrl_rwidth = 30;
const int rendererControl::rctrl_rheight = 30;
const int rendererControl::rctrl_sheight = 50;
const int rendererControl::rctrl_width = 300;
const int rendererControl::rctrl_height = 5*rendererControl::rctrl_border + 3*rendererControl::rctrl_sheight + rendererControl::rctrl_bheight + rview_window_extra_height;

rendererControl::rendererControl(float drx, float dry, float drz, int mode, rviewRenderImage *parentWin) : rviewFrame(NULL, lman->lookup("titleRendererCtrl"), 0, 0, rctrl_width, rctrl_height)
{
    int w;

    parent = parentWin;
    active = mode;
    panel = new wxPanel((wxWindow*)this, 0, 0, rctrl_width, rctrl_height);
    w = rctrl_width - 2*rctrl_border;
    rotx = new rviewSlider(panel, 1000*drx, -1000, 1000, w, lman->lookup("textRotX"));
    roty = new rviewSlider(panel, 1000*dry, -1000, 1000, w, lman->lookup("textRotY"));
    rotz = new rviewSlider(panel, 1000*drz, -1000, 1000, w, lman->lookup("textRotZ"));

    resetX = new rviewButton(panel, "0");
    resetY = new rviewButton(panel, "0");
    resetZ = new rviewButton(panel, "0");

    actionBut = new rviewButton(panel);
    closeBut = new rviewButton(panel);

    label();

    OnSize(rctrl_width, rctrl_height);

    Show(TRUE);
}


rendererControl::~rendererControl(void)
{
    if (parent != NULL)
    {
        user_event ue;

        ue.type = usr_child_closed;
        ue.data = (void*)this;
        parent->userEvent(ue);
    }
}


const char *rendererControl::getFrameName(void) const
{
    return "rendererControl";
}

rviewFrameType rendererControl::getFrameType(void) const
{
    return rviewFrameTypeRenCtrl;
}


void rendererControl::unlinkParent(void)
{
    parent = NULL;
}


void rendererControl::OnSize(int w, int h)
{
    int x, y, dx, dy;

    GetClientSize(&x, &y);
    panel->SetSize(0, 0, x, y, wxSIZE_ALLOW_MINUS_ONE);
    dx = (x - 2*rctrl_bwidth) / 2;
    dy = (rctrl_sheight - rctrl_rheight) / 2;
    x -= 3*rctrl_border + rctrl_rwidth;
    y = rctrl_border;
    rotx->SetSize(rctrl_border, y, x, rctrl_sheight);
    resetX->SetSize(2*rctrl_border + x, y+dy, rctrl_rwidth, rctrl_rheight);
    y += rctrl_border + rctrl_sheight;
    roty->SetSize(rctrl_border, y, x, rctrl_sheight);
    resetY->SetSize(2*rctrl_border + x, y+dy, rctrl_rwidth, rctrl_rheight);
    y += rctrl_border + rctrl_sheight;
    rotz->SetSize(rctrl_border, y, x, rctrl_sheight);
    resetZ->SetSize(2*rctrl_border + x, y+dy, rctrl_rwidth, rctrl_rheight);
    y += rctrl_border + rctrl_sheight;

    actionBut->SetSize(dx/2, y, rctrl_bwidth, rctrl_bheight);
    closeBut->SetSize((3*dx)/2 + rctrl_bwidth, y, rctrl_bwidth, rctrl_bheight);
}


void rendererControl::setActiveMode(int mode)
{
    active = mode;
    actionBut->SetLabel(lman->lookup((active == 0) ? "textStart" : "textStop"));
    actionBut->SetSize(-1, -1, rctrl_bwidth, rctrl_bheight);
}


void rendererControl::label(void)
{
    rotx->SetLabel(lman->lookup("textRotX"));
    roty->SetLabel(lman->lookup("textRotY"));
    rotz->SetLabel(lman->lookup("textRotZ"));

    closeBut->SetLabel(lman->lookup("textClose"));

    setActiveMode(active);
}


void rendererControl::updateParameters(void)
{
    if (parent != NULL)
    {
        if (active == 0)
        {
            parent->setAutoRotation(1000.0, 1000.0, 1000.0);
        }
        else
        {
            parent->setAutoRotation((rotx->GetValue()) / 1000.0, (roty->GetValue()) / 1000.0, (rotz->GetValue()) / 1000.0);
        }
    }
}


int rendererControl::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_SLIDER_COMMAND)
    {
        if ((&obj == (wxObject*)rotx) || (&obj == (wxObject*)roty) || (&obj == (wxObject*)rotz))
        {
            updateParameters();
            return 1;
        }
    }
    else if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)actionBut)
        {
            setActiveMode(active ^ 1);
            updateParameters();
            return 1;
        }
        else if (&obj == (wxObject*)closeBut)
        {
            if (parent != NULL) parent->closeRendererControls();
            return 1;
        }
        else if ((&obj == (wxObject*)resetX) || (&obj == (wxObject*)resetY) || (&obj == (wxObject*)resetZ))
        {
            if (&obj == (wxObject*)resetX)
                rotx->SetValue(0);
            else if (&obj == (wxObject*)resetY)
                roty->SetValue(0);
            else if (&obj == (wxObject*)resetZ)
                rotz->SetValue(0);

            updateParameters();
            return 1;
        }
    }
    return 0;
}




/*
 *  Renderer view window
 */

const int rendererCurrentView::rcview_border = rviewDisplay::display_border;
const int rendererCurrentView::rcview_bwidth = 80;
const int rendererCurrentView::rcview_bheight = 30;
const int rendererCurrentView::rcview_theight = 30;
const int rendererCurrentView::rcview_width = 180;
const int rendererCurrentView::rcview_height = 220;

rendererCurrentView::rendererCurrentView(const vertex_fp &angles, long off, double scale, rviewRenderImage *parentWin) :
    rviewFrame(NULL, lman->lookup("titleRendererView"), 0, 0, rcview_width, rcview_height)
{
    parent = parentWin;

    panel = new wxPanel((wxWindow*)this, 0, 0, rcview_width, rcview_height);

    rotx = new rviewText(panel);
    roty = new rviewText(panel);
    rotz = new rviewText(panel);

    zoff = new rviewText(panel);
    cubeScale = new rviewText(panel);

    applyButton = new rviewButton(panel);
    closeButton = new rviewButton(panel);

    label();

    updateView(angles, off, scale);

    OnSize(rcview_width, rcview_height);

    Show(TRUE);
}


rendererCurrentView::~rendererCurrentView(void)
{
    if (parent != NULL)
    {
        user_event ue;

        ue.type = usr_child_closed;
        ue.data = (void*)this;
        parent->userEvent(ue);
    }
}


const char *rendererCurrentView::getFrameName(void) const
{
    return "rendererCurrentView";
}


rviewFrameType rendererCurrentView::getFrameType(void) const
{
    return rviewFrameTypeRenView;
}


void rendererCurrentView::unlinkParent(void)
{
    parent = NULL;
}


void rendererCurrentView::OnSize(int w, int h)
{
    int x, y;

    GetClientSize(&x, &y);

    panel->SetSize(0, 0, w, h, wxSIZE_ALLOW_MINUS_ONE);

    x -= 2*rcview_border;
    rotx->SetSize(rcview_border, rcview_border, x, rcview_theight);
    roty->SetSize(rcview_border, rcview_border + rcview_theight, x, rcview_theight);
    rotz->SetSize(rcview_border, rcview_border + 2*rcview_theight, x, rcview_theight);
    zoff->SetSize(rcview_border, rcview_border + 3*rcview_theight, x, rcview_theight);
    cubeScale->SetSize(rcview_border, rcview_border + 4*rcview_theight, x, rcview_theight);

    x -= 2*rcview_bwidth;
    if (x < 0) x = 0;
    applyButton->SetSize(rcview_border + x/4, 3*rcview_border + 5*rcview_theight, rcview_bwidth, rcview_bheight);
    closeButton->SetSize(rcview_border + (3*x)/4 + rcview_bwidth, 3*rcview_border + 5*rcview_theight, rcview_bwidth, rcview_bheight);
}


void rendererCurrentView::label(void)
{
    rotx->SetLabel(lman->lookup("textRotX"));
    roty->SetLabel(lman->lookup("textRotY"));
    rotz->SetLabel(lman->lookup("textRotZ"));
    zoff->SetLabel(lman->lookup("textZOffset"));
    cubeScale->SetLabel(lman->lookup("textScale"));
    applyButton->SetLabel(lman->lookup("textApply"));
    closeButton->SetLabel(lman->lookup("textClose"));
}


int rendererCurrentView::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)applyButton)
        {
            updateParameters();
            return 1;
        }
        else if (&obj == (wxObject*)closeButton)
        {
            Close(TRUE);
            return 1;
        }
    }
    else if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
    {
        updateParameters();
        return 1;
    }
    return 0;
}


void rendererCurrentView::updateView(const vertex_fp &angles, long off, double scale)
{
    rotx->SetValue((double)angles.x);
    roty->SetValue((double)angles.y);
    rotz->SetValue((double)angles.z);
    zoff->SetValue(off);
    cubeScale->SetValue(scale);
}


void rendererCurrentView::updateParameters(void)
{
    if (parent != NULL)
    {
        vertex_fp angles;
        angles.x = atof(rotx->GetValue());
        angles.y = atof(roty->GetValue());
        angles.z = atof(rotz->GetValue());
        parent->setCurrentView(angles, atoi(zoff->GetValue()), atof(cubeScale->GetValue()));
    }
}







/*
 *  pixmapCanvas class. For displaying wxPixmaps
 */
pixmapCanvas::pixmapCanvas(rviewImage *parent, int x, int y, int w, int h, long style) : wxCanvas((wxWindow*)parent, x, y, w, h, style)
{
    pixmap = NULL;
    font = NULL;
    parentWin = parent;
    myDC = (wxDC*)GetDC();
    brush.SetStyle(wxSOLID);
    brush.SetColour(0,0,0);
    border.SetStyle(wxSOLID);
    border.SetColour(0,0,0);
    bpen.SetStyle(wxSOLID);
    bpen.SetColour(0,0,0);
    // Pen used for frames
    fpen.SetStyle(wxSOLID);
    fpen.SetColour((char)0xff, (char)0xff, (char)0xff);
    fpen.SetCap(wxCAP_BUTT);
    fpen.SetJoin(wxJOIN_BEVEL);
    SetBackground(&brush);
    rect_x0 = -1;
    rect_y0 = -1;
    rect_x1 = -1;
    rect_y1 = -1;
    aspectRatio = 0;
}



// Don't delete the pixmap! This is not our job.
pixmapCanvas::~pixmapCanvas(void)
{
    if (font != NULL)
    {
        SetFont(NULL);
        delete font;
    }
    SetBackground(NULL);
}


void pixmapCanvas::setPixmap(wxPixmap *pmap)
{
    pixmap = pmap;
    pixWidth = pmap->getWidth();
    pixHeight = pmap->getHeight();
    if (rect_x0 >= 0)
    {
        if (((rect_x0 >= pixWidth) && (rect_x1 >= pixWidth)) ||
                ((rect_y0 >= pixHeight) && (rect_y1 >= pixHeight)))
        {
            rect_x0 = -1;
            rect_y0 = -1;
            rect_x1 = -1;
            rect_y1 = -1;
        }
        else
        {
            if (rect_x0 >= pixWidth) rect_x0 = pixWidth-1;
            if (rect_x1 >= pixWidth) rect_x1 = pixWidth-1;
            if (rect_y0 >= pixHeight) rect_y0 = pixHeight-1;
            if (rect_y1 >= pixHeight) rect_y1 = pixHeight-1;
        }
    }
}


void pixmapCanvas::SetAspectRatio(double ratio)
{
    aspectRatio = ratio;
}


void pixmapCanvas::adjustBoxToRatio(void)
{
    if ((rect_x0 >= 0) && (aspectRatio > 0))
    {
        int width, height;

        width = rect_x1 - rect_x0;
        if (width < 0) width = -width;
        height = (int)(width * aspectRatio);
        if (rect_y1 < rect_y0) height = -height;
        rect_y1 = rect_y0 + height;
        if ((rect_y1 < 0) || (rect_y1 >= pixHeight))
        {
            if (rect_y1 < 0) rect_y1 = 0;
            else rect_y1 = pixHeight-1;
            height = rect_y1 - rect_y0;
            if (height < 0) height = -height;
            width = (int)(height / aspectRatio);
            if (rect_x1 < rect_x0) width = -width;
            rect_x1 = rect_x0 + width;
        }
    }
}


void pixmapCanvas::ToggleDragBox(bool clearMode)
{
    int minx, miny, maxx, maxy;

    // Drag box defined?
    if (rect_x0 < 0) return;

    if (rect_x0 < rect_x1)
    {
        minx = rect_x0;
        maxx = rect_x1;
    }
    else
    {
        minx = rect_x1;
        maxx = rect_x0;
    }
    if (rect_y0 < rect_y1)
    {
        miny = rect_y0;
        maxy = rect_y1;
    }
    else
    {
        miny = rect_y1;
        maxy = rect_y0;
    }

    // Only redraw the drag box if it's visible...
    if ((rect_x0 != rect_x1) && (rect_y0 != rect_y1))
    {
        int ominx, ominy, omaxx, omaxy;

        SetLogicalFunction(wxINVERT);
        SetPen(&fpen);
        //cout << "Draw " << minx << ',' << miny << ',' << maxx << ',' << maxy << endl;
        ominx = minx + offX;
        ominy = miny + offY;
        omaxx = maxx + offX;
        omaxy = maxy + offY;
        SetClippingRegion((float)ominx, (float)ominy, omaxx-ominx+1.0, omaxy-ominy+1.0);
        IntDrawLine(ominx, ominy, omaxx - 1, ominy);
        IntDrawLine(omaxx, ominy, omaxx, omaxy - 1);
        IntDrawLine(omaxx, omaxy, ominx + 1, omaxy);
        IntDrawLine(ominx, omaxy, ominx, ominy + 1);
        DestroyClippingRegion();
    }

    if ((rect_x0 == rect_x1) && (rect_y0 == rect_y1) && !clearMode) return;

    // Is the old rectangle to be removed or the new one to be plotted?
    // This makes a difference with the text!
    if (clearMode)
    {
        Refresh(FALSE, &textBBox);
    }
    else
    {
        float posx, posy, twidth, theight;
        char buffer[STRINGSIZE];

        // Create font on demand only
        if (font == NULL)
        {
            // Do not use pure white as background, this isn't handled correctly under X
            wxColour tfore(0, 0, 0), tback((char)0xf0, (char)0xf0, (char)0xf0);

            font = new wxFont(12, wxROMAN, wxNORMAL, wxNORMAL);
            SetFont(font);
            SetTextForeground(&tfore);
            SetTextBackground(&tback);
            textBack.SetColour(tback);
            textBack.SetStyle(wxSOLID);
        }
        SetBrush(&textBack);
        SetPen(&bpen);
        SetLogicalFunction(wxCOPY);
        sprintf(buffer, "%d:%d, %d:%d", minx, maxx, miny, maxy);
        GetTextExtent(buffer, &twidth, &theight);
        posx = minx + offX + rviewImage::image_dragtoff;
        posy = miny + offY + rviewImage::image_dragtoff;
#ifdef wx_msw
        textBBox.x = (int)posx - rviewDisplay::display_scrstep * GetScrollPos(wxHORIZONTAL);
        textBBox.y = (int)posy - rviewDisplay::display_scrstep * GetScrollPos(wxVERTICAL);
#else
        textBBox.x = (int)posx;
        textBBox.y = (int)posy;
#endif
        twidth += rviewImage::image_dragtspace;
        theight += rviewImage::image_dragtspace;
        textBBox.width = (int)twidth;
        textBBox.height = (int)theight;
        DrawRectangle(posx, posy, twidth, theight);
        DrawText(buffer, posx + rviewImage::image_dragtspace/2, posy + rviewImage::image_dragtspace/2);
        SetBrush(NULL);
    }
    SetPen(NULL);
}


void pixmapCanvas::SetDragBox(int x0, int y0, int x1, int y1)
{
    int scrollX, scrollY;

    ToggleDragBox(TRUE);

    scrollX = 0;//rviewDisplay::display_scrstep * GetScrollPos(wxHORIZONTAL);
    scrollY = 0;//rviewDisplay::display_scrstep * GetScrollPos(wxVERTICAL);

    rect_x0 = x0 - offX + scrollX;
    rect_y0 = y0 - offY + scrollY;
    rect_x1 = x1 - offX + scrollX;
    rect_y1 = y1 - offY + scrollY;
    if (((rect_x0 < 0) && (rect_x1 < 0)) || ((rect_x0 >= pixWidth) && (rect_x1 >= pixWidth)) ||
            ((rect_y0 < 0) && (rect_y1 < 0)) || ((rect_y0 >= pixHeight) && (rect_y1 >= pixHeight)))
    {
        rect_x0 = -1;
        rect_y0 = -1;
        rect_x1 = -1;
        rect_y1 = -1;
        return;
    }
    adjustBoxToRatio();
    if (rect_x0 < 0) rect_x0 = 0;
    if (rect_x0 >= pixWidth) rect_x0 = pixWidth-1;
    if (rect_y0 < 0) rect_y0 = 0;
    if (rect_y0 >= pixHeight) rect_y0 = pixHeight-1;
    if (rect_x1 < 0) rect_x1 = 0;
    if (rect_x1 >= pixWidth) rect_x1 = pixWidth-1;
    if (rect_y1 < 0) rect_y1 = 0;
    if (rect_y1 >= pixHeight) rect_y1 = pixHeight-1;

    ToggleDragBox(FALSE);
}


bool pixmapCanvas::HasDragBox(void) const
{
    return ((rect_x0 >= 0) && (rect_x0 != rect_x1));
}


bool pixmapCanvas::GetDragBox(int &x0, int &y0, int &x1, int &y1) const
{
    if ((rect_x0 < 0) || (rect_x0 == rect_x1)) return FALSE;

    // output sorted: x0 < x1, y0 < y1
    if (rect_x0 < rect_x1)
    {
        x0 = rect_x0;
        x1 = rect_x1;
    }
    else
    {
        x0 = rect_x1;
        x1 = rect_x0;
    }
    if (rect_y0 < rect_y1)
    {
        y0 = rect_y0;
        y1 = rect_y1;
    }
    else
    {
        y0 = rect_y1;
        y1 = rect_y0;
    }

    return TRUE;
}


void pixmapCanvas::UpdateDragBox(int x1, int y1)
{
    if (rect_x0 < 0) return;

    ToggleDragBox(TRUE);

    rect_x1 = x1 - offX;
    if (rect_x1 < 0) rect_x1 = 0;
    if (rect_x1 >= pixWidth) rect_x1 = pixWidth-1;
    rect_y1 = y1 - offY;
    if (rect_y1 < 0) rect_y1 = 0;
    if (rect_y1 >= pixHeight) rect_y1 = pixHeight-1;

    adjustBoxToRatio();

    ToggleDragBox(FALSE);

#if 0
    int width, height;
    int scrollX, scrollY;
    int mapX, mapY;

    scrollX = GetScrollPos(wxHORIZONTAL);
    scrollY = GetScrollPos(wxVERTICAL);
    mapX = x1 - scrollX * rviewDisplay::display_scrstep;
    mapY = y1 - scrollY * rviewDisplay::display_scrstep;
    // Autoscroll?
    GetClientSize(&width, &height);
    if (width >= 2*rviewImage::image_draghotzone)
    {
        if (mapX < rviewImage::image_draghotzone)
        {
            scrollX -= (rviewImage::image_draghotzone - mapX + rviewDisplay::display_scrstep-1) / rviewDisplay::display_scrstep;
            if (scrollX >= 0)
            {
                SetScrollPos(wxHORIZONTAL, scrollX);
                x1 = rviewImage::image_draghotzone + (x1-mapX);
                WarpPointer(x1, y1);
            }
        }
        else if (mapX > (width - rviewImage::image_draghotzone))
        {
            scrollX += (mapX - width + rviewImage::image_draghotzone + rviewDisplay::display_scrstep-1) / rviewDisplay::display_scrstep;
            if (scrollX <= GetScrollRange(wxHORIZONTAL))
            {
                SetScrollPos(wxHORIZONTAL, scrollX);
                x1 = (width - rviewImage::image_draghotzone) + (x1-mapX);
                WarpPointer(x1, y1);
            }
        }
    }
    if (height >= 2*rviewImage::image_draghotzone)
    {
        if (mapY < rviewImage::image_draghotzone)
        {
            scrollY -= (rviewImage::image_draghotzone - mapY + rviewDisplay::display_scrstep-1) / rviewDisplay::display_scrstep;
            if (scrollY >= 0)
            {
                SetScrollPos(wxVERTICAL, scrollY);
                WarpPointer(x1, rviewImage::image_draghotzone + scrollY * rviewDisplay::display_scrstep);
            }
        }
        else if (mapY > (height - rviewImage::image_draghotzone))
        {
            scrollY += (mapY - height + rviewImage::image_draghotzone + rviewDisplay::display_scrstep-1) / rviewDisplay::display_scrstep;
            if (scrollY <= GetScrollRange(wxVERTICAL))
            {
                SetScrollPos(wxVERTICAL, scrollY);
                WarpPointer(x1, height-rviewImage::image_draghotzone + scrollY * rviewDisplay::display_scrstep);
            }
        }
    }
#endif
}


void pixmapCanvas::AdjustDragBox(int x1, int y1)
{
    if (rect_x0 < 0) SetDragBox(x1, y1, x1, y1);
    else
    {
        ToggleDragBox(TRUE);

        x1 = x1 - offX;// + rviewDisplay::display_scrstep * GetScrollPos(wxHORIZONTAL);
        if (x1 < 0) x1 = 0;
        if (x1 >= pixWidth) x1 = pixWidth-1;
        y1 = y1 - offY;// + rviewDisplay::display_scrstep * GetScrollPos(wxVERTICAL);
        if (y1 < 0) y1 = 0;
        if (y1 >= pixHeight) y1 = pixHeight-1;

        if (abs(x1 - rect_x0) < abs(x1 - rect_x1))
            rect_x0 = rect_x1;
        if (abs(y1 - rect_y0) < abs(y1 - rect_y1))
            rect_y0 = rect_y1;
        rect_x1 = x1;
        rect_y1 = y1;

        adjustBoxToRatio();

        ToggleDragBox(FALSE);
    }
}


// Core functionality for plotting picture + border
void pixmapCanvas::paintCore(int x, int y)
{
    int sx, sy;

    GetSize(&sx, &sy);

    if ((offX > 0) || (offY > 0))
    {
        float bordx, bordy, height;

        bordx = (float)offX;
        bordy = (float)offY;
        height = (float)sy;

        SetLogicalFunction(wxCOPY);
        SetPen(&bpen);
        SetBrush(&border);

        if (offX > 0)
        {
            DrawRectangle(0.0, 0.0, bordx, height);
            DrawRectangle(bordx + pixWidth, 0.0, bordx+1, height);
        }
        if (offY > 0)
        {
            DrawRectangle(bordx, 0.0, (float)pixWidth, bordy);
            DrawRectangle(bordx, bordy + pixHeight, (float)pixWidth, bordy+1);
        }
        SetPen(NULL);
        SetBrush(NULL);
    }

    pixmap->plotPixmap(offX - x, offY - y);

    ToggleDragBox(FALSE);
}


void pixmapCanvas::OnPaint(void)
{
    wxUpdateIterator upd(this);
    wxRect rect;
    int w, h, x, y;

    GetClientSize(&w, &h);
    x = rviewDisplay::display_scrstep * GetScrollPos(wxHORIZONTAL);
    y = rviewDisplay::display_scrstep * GetScrollPos(wxVERTICAL);

    offX = (w - pixWidth) / 2;
    if (offX < 0) offX = 0;
    offY = (h - pixHeight) / 2;
    if (offY < 0) offY = 0;

    BeginDrawing();

    while (upd)
    {
        //upd.GetRect(&rect);
        paintCore(x, y);
        upd++;
    }

    EndDrawing();
}


void pixmapCanvas::updateDisplay(bool borders)
{
    if (parentWin->IsShown())
    {
        int cw, ch;
        int x, y;

        x = rviewDisplay::display_scrstep * GetScrollPos(wxHORIZONTAL);
        y = rviewDisplay::display_scrstep * GetScrollPos(wxVERTICAL);

        GetClientSize(&cw, &ch);
        offX = (cw - pixWidth)/2;
        if (offX < 0) offX = 0;
        offY = (ch - pixHeight)/2;
        if (offY < 0) offY = 0;
        pixmap->invalidatePixmap();
        if (borders)
        {
            int sx, sy;

            GetVirtualSize(&sx, &sy);
            sx *= rviewDisplay::display_scrstep;
            sy *= rviewDisplay::display_scrstep;
            SetClippingRegion(0, 0, sx, sy);
            paintCore(x, y);
        }
        else
        {
            SetClippingRegion(offX, offY, pixWidth, pixHeight);
            paintCore(x, y);
        }
        DestroyClippingRegion();
    }
}



void pixmapCanvas::OnEvent(wxMouseEvent &mevt)
{
    parentWin->processMouseEvent(mevt);
}






/*
 *  rviewImage members
 */

const int rviewImage::image_swidth = 100;
const int rviewImage::image_sheight = 40;
const int rviewImage::image_pbwidth = rviewDisplay::display_pbwidth;
const int rviewImage::image_pbheight = 30;
const int rviewImage::image_minwidth = 50;
const int rviewImage::image_minheight = 50;
const int rviewImage::image_chkwidth = 80;
const int rviewImage::image_chkheight = 30;
const int rviewImage::image_bbwidth = 60;
const int rviewImage::image_bbheight = 20;
const int rviewImage::image_twidth = 100;
const int rviewImage::image_theight = 50;
const int rviewImage::image_bwidth = 64;
const int rviewImage::image_bheight = 30;
const int rviewImage::image_dragtoff = 8;
const int rviewImage::image_dragtspace = 8;
const int rviewImage::image_draghotzone = 32;
const int rviewImage::image_ctrly = 60;
const int rviewImage::image_totaly = rviewDisplay::display_cheight + rviewImage::image_ctrly;

const char *rviewImage::view_ScrollPos = "scrollPos";
const char *rviewImage::view_UseCspace = "useCspace";
const char *rviewImage::view_CspaceFull = "cspaceFull";
const char *rviewImage::view_CspaceProj = "cspaceProj";
const char *rviewImage::view_CspaceMeans = "cspaceMeans";
const char *rviewImage::view_CspaceSigmas = "cspaceSigmas";
const char *rviewImage::view_CspaceType = "cspaceType";
const char *rviewImage::view_CspaceRange = "cspaceRange";
const char *rviewImage::view_ScaleValue = "scaleValue";

rviewImage::rviewImage(mdd_frame *mf, int es, unsigned int flags) : rviewDisplay(mf, es+image_ctrly, flags)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "rviewImage()");

    // make destructor safe first.
    csmap = NULL; // signifies no data for value->colourspace mapping initialised yet
    csInterv = NULL;
    pixmap = NULL;
    pcanv = NULL;
    scaleSlider = NULL;
    cspar = NULL;

    // This variable is set to true once openViewerEpilogue() is called. Its main use is
    // 1) on Windows: trap calls to OnSize() happening before the object is fully initialized
    // 2) for the renderers: avoid actually drawing anything until the window size is fixed.
    initPhaseFinished = FALSE;

    // Check the base type immediately:
    if (baseType == rbt_none)
    {
        objectInitializedOK = FALSE;
        return;
    }

#if 0
    // Test resampling routines...
    r_Ref<r_GMarray> mddPtr;
    r_Minterval newInterv(dimMDD);
    //for (i=0; i<dimMDD; i++) newInterv << r_Sinterval(4*mddObj->spatial_domain()[i].low(), 4*mddObj->spatial_domain()[i].high());
    //mdd_objectScaleInter(mddObj, mddPtr, newInterv, baseType);
    for (i=0; i<dimMDD; i++) newInterv << r_Sinterval((r_Range)(0.5*mddObj->spatial_domain()[i].low()), (r_Range)(0.5*mddObj->spatial_domain()[i].high()));
    mdd_objectScaleAverage(mddObj, mddPtr, newInterv, baseType);
    mddObj.destroy();
    mddObj = mddPtr;
    interv = mddObj->spatial_domain();
#endif
}



rviewImage::~rviewImage(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "~rviewImage()");

    if (cspar != NULL) delete cspar;
    if (csmap != NULL) delete csmap;
    if (csInterv != NULL) delete csInterv;
    delete pixmap;
}


// concentrate most intelligence in this function rather than the constructor
// because we can't use virtual functions (correctly) in the constructor.
int rviewImage::openViewer(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "openViewer()");

    if (!objectInitializedOK)
        return -1;

    // init lower level stuff like the menu bar
    rviewDisplay::openViewer();

    wxMenu *config;
    wxMenu *submenu;
    char buffer[STRINGSIZE];

    config = new wxMenu;
    configMenuInitHook(config);
    submenu = new wxMenu;
    submenu->Append(MENU_IMAGE_CSPACE_ON, "", NULL, TRUE);
    submenu->Append(MENU_IMAGE_CSPACE_FULL, "", NULL, TRUE);
    submenu->Append(MENU_IMAGE_CSPACE_PROJ, "", NULL, TRUE);
    submenu->Append(MENU_IMAGE_CSPACE_EDIT, "");
    config->Append(MENU_IMAGE_SETUP_CSPACE, lman->lookup("menCspaceTitle"), submenu, NULL);
    if (moviePossible())
    {
        submenu = new wxMenu;
        submenu->Append(MENU_IMAGE_MOVIE_ONCE, "", NULL, TRUE);
        submenu->Append(MENU_IMAGE_MOVIE_START, "", NULL, TRUE);
        submenu->Append(MENU_IMAGE_MOVIE_SWITCH, "", NULL, TRUE);
        config->AppendSeparator();
        config->Append(MENU_IMAGE_SETUP_MOVIE, lman->lookup("menImgSetupMovie"), submenu, NULL);
    }
    sprintf(buffer, "&%s", lman->lookup("menImgSetup"));
    mBar->Append(config, buffer);

    mBar->Enable(MENU_IMAGE_CSPACE_EDIT, FALSE);

    pcanv = NULL;
    imgData = NULL;
    scaleValue = 100.0;

    // Colourspace mapper
    doValToCspace = (prefs->rgbSpace != 0);
    doFullRangeCspace = (prefs->rgbSpace == 2);
    // We're not allowed to call setCspaceProjMode before a projection is set
    // in the init... functions
    doProjRangeCspace = (prefs->rgbSpace == 3);
    if (modeNeedsCspace(baseType)) doValToCspace = TRUE;

    mousex = -1.0;
    mousey = -1.0;
    mousebut = 0;

    if (showScaleSlider())
        scaleSlider = new rviewSlider(ctrlPanel, (int)scaleValue, 0, 500, image_swidth, lman->lookup("textScale"));

    if (rviewCheckInitCspace(baseType, NULL, mddObj) != 0)
    {
        mBar->Check(MENU_IMAGE_CSPACE_ON, doValToCspace);
        mBar->Check(MENU_IMAGE_CSPACE_FULL, doFullRangeCspace);
        mBar->Check(MENU_IMAGE_CSPACE_PROJ, doProjRangeCspace);
        mBar->Enable(MENU_IMAGE_CSPACE_FULL, doValToCspace);
        mBar->Enable(MENU_IMAGE_CSPACE_PROJ, doValToCspace);
        mBar->Enable(MENU_IMAGE_CSPACE_EDIT, doValToCspace);
        cspaceForType = TRUE;
    }
    else
    {
        cspaceForType = FALSE;
    }
    configureCspace(cspaceForType);

    if (moviePossible())
    {
        switch (prefs->movieMode)
        {
        case 1:
            lastMovieMode = MENU_IMAGE_MOVIE_START;
            break;
        case 2:
            lastMovieMode = MENU_IMAGE_MOVIE_SWITCH;
            break;
        default:
            lastMovieMode = MENU_IMAGE_MOVIE_ONCE;
            break;
        }
        playDirection = 0;

        playBack = new rviewButton(ctrlPanel, "<");
        playStop = new rviewButton(ctrlPanel, "[]");
        playFwd = new rviewButton(ctrlPanel, ">");
        mBar->Check(lastMovieMode, TRUE);
    }
    else
    {
        playBack = NULL;
        playStop = NULL;
        playFwd = NULL;
    }

    configureMode();

    wxColour palette[2];
    int w, h;

    GetClientSize(&w, &h);


    w -= 2*display_cnvborder;
    h -= 2*display_cnvborder + totalCtrlHeight;
    pcanv = new pixmapCanvas(this, display_cnvborder, display_cnvborder + totalCtrlHeight, w, h);

    scrollx = -1;
    scrolly = -1;
    pixWidth = -1;
    pixHeight = -1;
    pixPitch = -1;
    pixPad = 0;
    pixDepth = 0;
    virtualPitch = -1;

    if (initMode() == NULL)
    {
        objectInitializedOK = FALSE;
        return -1;
    }

    pcanv->Clear();

    if (rviewImageTypes[baseType] == RVIEW_IMGTYPE_MONO)
    {
        palette[0] = wxColour(0,0,0);
        palette[1] = wxColour(255,255,255);
    }
    pixmap = new wxPixmap((wxWindow*)pcanv, pixWidth, pixHeight, pixDepth, pixPad, imgData, getPixmapFlags(), ((rviewImageTypes[baseType] == RVIEW_IMGTYPE_MONO) ? palette : NULL));
    pcanv->setPixmap(pixmap);

    return 0;
}


void rviewImage::openViewerEpilogue(rviewFrameType ft)
{
    // Only do something if this was called from the top level image class, i.e. the
    // one that overloads getFrameType(). Otherwise NOP.
    if (ft == getFrameType())
    {
        int w, h;

        initPhaseFinished = TRUE;

        RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "openViewerEpilogue() from top level " << getFrameName() );

        label();

        setMinimumViewerSize(image_minwidth, image_minheight);

        GetSize(&w, &h);

        OnSize(w, h);
        OnSize(w, h);

        Show(TRUE);
    }
}


int rviewImage::freeDimsFromProjection(int &dim1, int &dim2, r_Point *map)
{
    // Apply the projection string
    if (rviewParseProjection(getVirtualDomain(), pt1, pt2, projString, &freeDims, map) != dimMDD)
    {
        rviewErrorbox::reportError(lman->lookup("errorProjection"), rviewImage::getFrameName(), "freeDimsFromProjection");
        return -1;
    }
    // Check whether there are more or less than 2 free dimensions.
    for (dim1=0; dim1<dimMDD; dim1++) if ((freeDims & (1<<dim1)) != 0) break;
    for (dim2=dim1+1; dim2<dimMDD; dim2++) if ((freeDims & (1<<dim2)) != 0) break;
    if ((dim1 >= dimMDD) || (dim2 >= dimMDD))
    {
        rviewErrorbox::reportError(lman->lookup("errorProjectFree"), rviewImage::getFrameName(), "freeDimsFromProjection");
        return -1;
    }
    if (map != NULL)
    {
        dim1 = (*map)[dim1];
        dim2 = (*map)[dim2];
    }
    return 0;
}


void rviewImage::projectObjectHook(void)
{
}


const char *rviewImage::getFrameName(void) const
{
    return "rviewImage";
}

rviewFrameType rviewImage::getFrameType(void) const
{
    return rviewFrameTypeImage;
}


int rviewImage::fileMenuInitHook(wxMenu *menu)
{
    menu->Append(MENU_DISP_DATA_SAVETIFF, "");
    return 1;
}


bool rviewImage::modeNeedsCspace(rviewBaseType bt) const
{
    return ((bt == rbt_float) || (bt == rbt_double));
}


void rviewImage::configureCspace(bool state)
{
    mBar->Enable(MENU_IMAGE_SETUP_CSPACE, state);
    // Bug (feature?) in WinNT version: menu having submenu can't be disabled.
    mBar->Enable(MENU_IMAGE_CSPACE_ON, state);
    mBar->Enable(MENU_IMAGE_CSPACE_FULL, state);
    mBar->Enable(MENU_IMAGE_CSPACE_PROJ, state);
    mBar->Enable(MENU_IMAGE_CSPACE_EDIT, state);
}


int rviewImage::configMenuInitHook(wxMenu *menu)
{
    return 0;
}

bool rviewImage::cspaceRangeHook(bool suggest)
{
    return suggest;
}

void rviewImage::configureMode(void)
{
    bool csranges;

    if (modeNeedsCspace(baseType)) doValToCspace = TRUE;

    csranges = cspaceRangeHook((cspaceForType && doValToCspace));

    if (doValToCspace)
    {
        mBar->Enable(MENU_IMAGE_CSPACE_ON, doValToCspace);
        mBar->Enable(MENU_IMAGE_CSPACE_EDIT, doValToCspace);
    }
    mBar->Enable(MENU_IMAGE_CSPACE_FULL, csranges);
    mBar->Enable(MENU_IMAGE_CSPACE_PROJ, csranges);
    mBar->Check(MENU_IMAGE_CSPACE_ON, doValToCspace);
}


void rviewImage::setCspaceProjMode(bool pm)
{
    if (pm)
    {
        int i=0;
        r_Minterval tempInterv(dimMDD);

        if(!csInterv)
            csInterv = new r_Minterval(dimMDD);

        for (i=0; i<dimMDD; i++)
        {
            tempInterv << r_Sinterval((r_Range)pt1[i], (r_Range)pt2[i]);
        }

        *csInterv=tempInterv;
    }

    doProjRangeCspace = pm;
}


void rviewImage::label(void)
{
    if (scaleSlider != NULL)
        scaleSlider->SetLabel(lman->lookup("textScale"));

    mBar->SetLabelTop(fixedNumberOfMenus, lman->lookup("menImgSetup"));
    mBar->SetLabel(MENU_IMAGE_SETUP_CSPACE, lman->lookup("menCspaceTitle"));
    mBar->SetLabel(MENU_IMAGE_CSPACE_ON, lman->lookup("menCspaceOn"));
    mBar->SetLabel(MENU_IMAGE_CSPACE_FULL, lman->lookup("menCspaceFull"));
    mBar->SetLabel(MENU_IMAGE_CSPACE_PROJ, lman->lookup("menCspaceProj"));
    mBar->SetLabel(MENU_IMAGE_CSPACE_EDIT, lman->lookup("menCspaceEdit"));
    if (moviePossible())
    {
        mBar->SetLabel(MENU_IMAGE_SETUP_MOVIE, lman->lookup("menImgSetupMovie"));
        mBar->SetLabel(MENU_IMAGE_MOVIE_ONCE, lman->lookup("menImgMovieOnce"));
        mBar->SetLabel(MENU_IMAGE_MOVIE_START, lman->lookup("menImgMovieStart"));
        mBar->SetLabel(MENU_IMAGE_MOVIE_SWITCH, lman->lookup("menImgMovieSwitch"));
    }
    mBar->SetLabel(MENU_DISP_DATA_SAVETIFF, lman->lookup("menDispDataSaveTIFF"));

    rviewDisplay::label();
}



char *rviewImage::movieNewFrame(void)
{
    return NULL;
}

int rviewImage::process(wxObject &obj, wxEvent &evt)
{
    RMDBGENTER(3, RMDebug::module_applications, "rviewImage", "process()");

    int type = evt.GetEventType();

    if (rviewDisplay::process(obj, evt) != 0)
    {
        return 1;
    }

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (moviePossible())
        {
            int oldDirection = playDirection;

            if (&obj == (wxObject*)playStop)
            {
                RMDBGMIDDLE(3, RMDebug::module_applications, "rviewImage", "process() Playback stop" );
                playDirection = 0;
                return 1;
            }
            if (&obj == (wxObject*)playBack)
                playDirection = -1;
            else if (&obj == (wxObject*)playFwd)
                playDirection = 1;

            // Only enter the loop if we didn't have playback before to avoid reentrancy.
            if ((oldDirection == 0) && (playDirection != 0))
            {
                char *data, *lastData;

                RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "process() Playback start " << playDirection );
                do
                {
                    while (advanceProjection(playDirection) != 0)
                    {
                        lastData = imgData;

                        data = movieNewFrame();

                        if (data == NULL)
                        {
                            playDirection = 0;
                            return 0;
                        }
                        updatePixmap(lastData, data);
                        ::wxYield();
                        // Order is _vitally _ important here. Check _after_ the call to ::wxYield()!
                        if (playDirection == 0) break;
                    }
                    // Allow for one NULL cycle after playback (ensures no infinite loops if playback
                    // dimension is only 1 element wide.
                    ::wxYield();
                    if (playDirection != 0)
                    {
                        if (mBar->Checked(MENU_IMAGE_MOVIE_ONCE))
                            playDirection = 0;
                        else if (mBar->Checked(MENU_IMAGE_MOVIE_START))
                        {
                            advanceProjection(-playDirection, display_advmode_reset);
                        }
                        else
                            playDirection = -playDirection;
                    }
                }
                while (playDirection != 0);
            }
            return 1;
        }
    }

    return 0;
}


bool rviewImage::moviePossible(void) const
{
    return FALSE;
}

bool rviewImage::canRotateObject(void) const
{
    return FALSE;
}

bool rviewImage::showScaleSlider(void) const
{
    return TRUE;
}

void rviewImage::rotateObject(wxMouseEvent &mevt)
{
}

void rviewImage::processMouseEvent(wxMouseEvent &mevt)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "processMouseEvent()" );

    int newbut=0;

    if (mevt.ControlDown()) newbut = MOUSE_CONTROL;
    else if (!canRotateObject())
    {
        mousebut = 0;
        return;
    }

    if (mevt.leftDown) newbut |= MOUSE_LEFT;
    if (mevt.middleDown) newbut |= MOUSE_MIDDLE;
    if (mevt.rightDown) newbut |= MOUSE_RIGHT;

    if (((newbut & MOUSE_CONTROL) == 0) && canRotateObject())
    {
        rotateObject(mevt);
    }
    else
    {
        // Drag a box
        if ((newbut & MOUSE_LEFT) != 0)
        {
            if ((mousebut & MOUSE_LEFT) == 0)
            {
                pcanv->SetDragBox(mevt.x, mevt.y, mevt.x, mevt.y);
            }
            else
            {
                pcanv->UpdateDragBox(mevt.x, mevt.y);
            }
        }
        else if ((newbut & MOUSE_RIGHT) != 0)
        {
            if ((mousebut & MOUSE_RIGHT) == 0)
            {
                pcanv->AdjustDragBox(mevt.x, mevt.y);
            }
            else
            {
                pcanv->UpdateDragBox(mevt.x, mevt.y);
            }
        }
    }

    mousebut = newbut;
    mousex = mevt.x;
    mousey = mevt.y;
}


void rviewImage::updatePixmap(char *oldData, char *newData)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "updatePixmap()" );

    if (pixmap == NULL) return;

    if (oldData != newData)
    {
        pixmap->newPixmap((wxWindow*)pcanv, pixWidth, pixHeight, pixDepth, pixPad, imgData, getPixmapFlags() | WX_PIXFLAG_SAMEPALETTE);
        pcanv->setPixmap(pixmap);
    }
    pcanv->updateDisplay((oldData != newData));
}


int rviewImage::getPixmapFlags(void)
{
    int pixflags = WX_PIXFLAG_TRANSLATE;

    if (prefs->imgDither)
    {
        if (prefs->ditherBest)
            pixflags |= WX_PIXFLAG_DITHER;
        else
            pixflags |= WX_PIXFLAG_FASTDITHER;
    }
    return pixflags;
}



void rviewImage::OnSize(int w, int h)
{
    int x, y, pos;
    bool resize=false;

    RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "OnSize " << w << ", " << h );

    // fully initialized yet?
    if (initPhaseFinished)
    {
        GetClientSize(&x, &y);

        if ((frameWidth == w) && (frameHeight == h))
            return;

        frameWidth = w;
        frameHeight = h;

        x -= 2*display_cnvborder;
        y -= 2*display_cnvborder + totalCtrlHeight;
        pcanv->SetSize(display_cnvborder, display_cnvborder + totalCtrlHeight, x, y);

        // Ctrl panel items
        if (scaleSlider != NULL)
            scaleSlider->SetSize(display_border, image_totaly - image_sheight, x, image_sheight);

        pos = x - 3*image_pbwidth - display_border + 2*display_cnvborder;
        // Buttons might not be present
        if (playBack != NULL)
            playBack->SetSize(pos, display_cheight, image_pbwidth, image_pbheight);
        if (playStop != NULL)
            playStop->SetSize(pos + image_pbwidth, display_cheight, image_pbwidth, image_pbheight);
        if (playFwd != NULL)
            playFwd->SetSize(pos + 2*image_pbwidth, display_cheight, image_pbwidth, image_pbheight);
    }

    rviewDisplay::OnSize(w, h);
}


void rviewImage::OnMenuCommand(int id)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "OnMenuCommand()" );

    switch (id)
    {
    case MENU_DISP_DATA_SAVETIFF:
    {
        char *file;
        char *prefDir = (char*)(prefs->filePath.ptr());
        file = ::wxFileSelector(lman->lookup("saveTIFF"), (::wxDirExists(prefDir)) ? prefDir : NULL, NULL, NULL, "*", 0, this);
        if (file != NULL)
        {
            ::wxBeginBusyCursor();
            rviewIO::PixmapToTIFF(pixmap, file, prefs->vffParams.ptr());
            prefs->filePath = ::wxPathOnly(file);
            ::wxEndBusyCursor();
        }
    }
    break;
    case MENU_IMAGE_CSPACE_EDIT:
        if (csmap != NULL)
        {
            csmap->openEditor();
        }
        break;
    case MENU_IMAGE_MOVIE_ONCE:
    case MENU_IMAGE_MOVIE_START:
    case MENU_IMAGE_MOVIE_SWITCH:
        if (moviePossible())
        {
            mBar->Check(lastMovieMode, FALSE);
            mBar->Check(id, TRUE);
            lastMovieMode = id;
        }
        break;
    case MENU_IMAGE_CSPACE_ON:
    case MENU_IMAGE_CSPACE_FULL:
    case MENU_IMAGE_CSPACE_PROJ:
    {
        if (id == MENU_IMAGE_CSPACE_ON)
        {
            // You're not allowed to leave cspace mode for these type/mode combinations
            if (modeNeedsCspace(baseType))
            {
                mBar->Check(MENU_IMAGE_CSPACE_ON, TRUE);
            }
            else
            {
                doValToCspace = mBar->Checked(MENU_IMAGE_CSPACE_ON);
                mBar->Enable(MENU_IMAGE_CSPACE_FULL, doValToCspace);
                mBar->Enable(MENU_IMAGE_CSPACE_PROJ, doValToCspace);
                mBar->Enable(MENU_IMAGE_CSPACE_EDIT, doValToCspace);
            }
        }
        else if (id == MENU_IMAGE_CSPACE_FULL)
        {
            doFullRangeCspace = mBar->Checked(MENU_IMAGE_CSPACE_FULL);
            if (csmap != NULL)
            {
                csmap->processRange((doFullRangeCspace) ? CSPACE_RANGE_FULL : CSPACE_RANGE_ACTUAL);
            }
        }
        else if (id == MENU_IMAGE_CSPACE_PROJ)
        {
            setCspaceProjMode(mBar->Checked(MENU_IMAGE_CSPACE_PROJ));
            if (csmap != NULL)
            {
                csmap->updateProjection(csInterv);
            }
        }
        newProjection();
    }
    break;
    default:
        rviewDisplay::OnMenuCommand(id);
        break;
    }
}


// Refuse to be killed if animation is in progress
bool rviewImage::OnClose(void)
{
    if (playDirection != 0) return FALSE;
    return TRUE;
}


int rviewImage::userEvent(const user_event &ue)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewImage", "userEvent()" );

    if ((ue.type == usr_cspace_changed) && (ue.data == (void*)csmap) && (doValToCspace))
    {
        // The csmapper has already updated the tables and internal information
        newProjection();
        return 1;
    }
    // Don't intercept unknown messages!
    return rviewDisplay::userEvent(ue);
}



void rviewImage::prepareToDie(void)
{
    // Stop movie-playback immediately.
    playDirection = 0;
    ::wxYield();
}


int rviewImage::requestQuit(int level)
{
    playDirection = 0;
    return rviewFrame::requestQuit(level);
}


void rviewImage::resizeImage(void)
{
    int x, y, sx, sy;

    if (scrollx >= 0)
        scrollx = pcanv->GetScrollPos(wxHORIZONTAL);
    else
        scrollx = 0;

    if (scrolly >= 0)
        scrolly = pcanv->GetScrollPos(wxVERTICAL);
    else
        scrolly = 0;

    pcanv->GetVirtualSize(&x, &y);
    sx = (pixWidth + display_scrstep - 1) / display_scrstep;
    sy = (pixHeight + display_scrstep - 1) / display_scrstep;

    // Only update the canvas size if this is absolutely necessary to avoid flicker.
    if ((x != display_scrstep*sx) || (y != display_scrstep*sy))
    {
        pcanv->SetScrollbars(display_scrstep, display_scrstep, sx, sy, display_pgstep, display_pgstep, scrollx, scrolly);
    }
}


void rviewImage::ensureViewCspace(void)
{
    if (cspar == NULL)
    {
        cspar = new colourspace_params;
        memset(cspar, 0, sizeof(colourspace_params));
    }
}


void rviewImage::deleteViewCspace(void)
{
    if (cspar != NULL)
    {
        delete cspar;
        cspar = NULL;
    }
}


int rviewImage::saveView(FILE *fp)
{
    int status = rviewDisplay::saveView(fp);

    long spos[2];
    spos[0] = (long)scrollx;
    spos[1] = (long)scrolly;

    writeViewParam(fp, view_ScrollPos, 2, spos);
    writeViewParam(fp, view_UseCspace, (long)doValToCspace);
    writeViewParam(fp, view_CspaceFull, (long)doFullRangeCspace);
    writeViewParam(fp, view_CspaceProj, (long)doProjRangeCspace);
    writeViewParam(fp, view_ScaleValue, scaleValue / 100.0);
    if (csmap != NULL)
    {
        colourspace_params par;
        double cvals[3];
        csmap->getParameters(&par);
        cvals[0] = par.peak_red;
        cvals[1] = par.peak_green;
        cvals[2] = par.peak_blue;
        writeViewParam(fp, view_CspaceMeans, 3, cvals);
        cvals[0] = par.sigm_red;
        cvals[1] = par.sigm_green;
        cvals[2] = par.sigm_blue;
        writeViewParam(fp, view_CspaceSigmas, 3, cvals);
        cvals[0] = par.minVal;
        cvals[1] = par.maxVal;
        writeViewParam(fp, view_CspaceRange, 2, cvals);
        writeViewParam(fp, view_CspaceType, (long)par.type);
    }
    return status;
}


int rviewImage::readView(const char *key, const char *value)
{
    int status = rviewDisplay::readView(key, value);

    if (status == 0)
    {
        if (strcmp(key, view_ScrollPos) == 0)
        {
            long spos[2];
            if (readVector(value, 2, spos) == 0)
            {
                scrollx = (int)spos[0];
                scrolly = (int)spos[1];
            }
            return 1;
        }
        else if (strcmp(key, view_UseCspace) == 0)
        {
            doValToCspace = (bool)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_CspaceFull) == 0)
        {
            doFullRangeCspace = (bool)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_CspaceProj) == 0)
        {
            doProjRangeCspace = (bool)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_CspaceMeans) == 0)
        {
            double mean[3];
            ensureViewCspace();
            if (readVector(value, 3, mean) == 0)
            {
                cspar->peak_red = mean[0];
                cspar->peak_green = mean[1];
                cspar->peak_blue = mean[2];
            }
            return 1;
        }
        else if (strcmp(key, view_CspaceSigmas) == 0)
        {
            double sigm[3];
            ensureViewCspace();
            if (readVector(value, 3, sigm) == 0)
            {
                cspar->sigm_red = sigm[0];
                cspar->sigm_green = sigm[1];
                cspar->sigm_blue = sigm[2];
            }
            return 1;
        }
        else if (strcmp(key, view_CspaceRange) == 0)
        {
            ensureViewCspace();
            double crange[2];
            if (readVector(value, 2, crange) == 0)
            {
                cspar->minVal = crange[0];
                cspar->maxVal = crange[1];
            }
            return 1;
        }
        else if (strcmp(key, view_CspaceType) == 0)
        {
            ensureViewCspace();
            cspar->type = (cspaceType)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_ScaleValue) == 0)
        {
            scaleValue = 100*atof(value);
            return 1;
        }
        return 0;
    }
    return status;
}


void rviewImage::loadViewFinished(void)
{
    rviewDisplay::loadViewFinished();

    pcanv->SetScrollPos(wxHORIZONTAL, scrollx);
    pcanv->SetScrollPos(wxVERTICAL, scrolly);

    if (scaleSlider != NULL)
        scaleSlider->SetValue((int)scaleValue);

    mBar->Check(MENU_IMAGE_CSPACE_ON, doValToCspace);
    mBar->Check(MENU_IMAGE_CSPACE_FULL, doFullRangeCspace);
    mBar->Check(MENU_IMAGE_CSPACE_PROJ, doProjRangeCspace);
    mBar->Enable(MENU_IMAGE_CSPACE_ON, doValToCspace);
    mBar->Enable(MENU_IMAGE_CSPACE_FULL, doValToCspace);
    mBar->Enable(MENU_IMAGE_CSPACE_PROJ, doValToCspace);
    mBar->Enable(MENU_IMAGE_CSPACE_EDIT, doValToCspace);
}





/*
 *  Flat image base class members
 */
rviewFlatBaseImage::rviewFlatBaseImage(mdd_frame *mf, int es, unsigned int flags) : rviewImage(mf, es, flags)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatBaseImage", "rviewFlatImage()" );
}


rviewFlatBaseImage::~rviewFlatBaseImage(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatBaseImage", "~rviewFlatBaseImage()" );
}


int rviewFlatBaseImage::openViewer(void)
{
    if (dimMDD < 2)
    {
        rviewErrorbox::reportError(lman->lookup("errorModeDim"), rviewFlatBaseImage::getFrameName(), "openViewer");
        objectInitializedOK = FALSE;
        return -1;
    }
    return rviewImage::openViewer();
}


const char *rviewFlatBaseImage::getFrameName(void) const
{
    return "rviewFlatBaseImage";
}

rviewFrameType rviewFlatBaseImage::getFrameType(void) const
{
    return rviewFrameTypeFltBsImage;
}


int rviewFlatBaseImage::newProjection(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatBaseImage", "newProjection()" );

    char *data, *lastData = imgData;

    if ((data = projectImage()) == NULL) return -1;

    updatePixmap(lastData, data);

    return 0;
}


char *rviewFlatBaseImage::initMode(void)
{
    int i=0, j=0, w=0, h=0;
    char *data=0;

    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatBaseImage", "initMode()" );

    // Initialise the projection string. Use a static default for now, later on use the
    // last value used.
    data = projString;
    data += sprintf(data, "*:*, *:*");
    for (i=2; i<dimMDD; i++)
    {
        data += sprintf(data, ", %ld", interv[i].low());
    }
    project->SetValue(projString);

    // This returns a pointer to the projected MDD data, ready for display by wxPixmap.
    // It will also set the variables pixWidth, pixHeight.
    data = projectImage();

    // Calculate the size of the whole window to display the entire image by
    // first examining the difference between the frame's size and the canvas'
    // client size (i.e. without the scrollbars)
    GetSize(&i, &j);
    pcanv->GetClientSize(&w, &h);
    w = pixWidth + (i-w);
    h = pixHeight + (j-h);

    // Limit the size of the window to the values specified in prefs.
    if (w > prefs->maxDWidth) w = prefs->maxDWidth;
    if (h > prefs->maxDHeight) h = prefs->maxDHeight;

    // ... then set the window size.
    SetSize(-1, -1, w, h);

    setModeDimension(2);

    return data;
}


char *rviewFlatBaseImage::projectImage(void)
{
    int dim1, dim2;

    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatBaseImage", "projectImage()" );

    mapIndex = r_Point(dimMDD);
    if (freeDimsFromProjection(dim1, dim2, &mapIndex) != 0) return NULL;

    projectObjectHook();

    rviewFlatProjEnv penv;

    penv.mddPtr = mddObj.ptr();
    penv.pt1 = pt1;
    penv.pt2 = pt2;
    penv.dim1 = dim1;
    penv.dim2 = dim2;
    penv.bt = baseType;
    penv.doCspace = doValToCspace;
    penv.scale = scaleValue / 100;
    if (penv.scale <= 0) penv.scale = 0.01;

    if (rviewPrepareFlatProjection(penv) != 0) return NULL;

    if (doValToCspace)
    {
        // init if necessary
        setCspaceProjMode(doProjRangeCspace);
        // No need for the virtual pitch here, as the image buffer won't be filled with data of the base type
        penv.cspaceState = rviewCheckInitCspace(baseType, &csmap, mddObj, doFullRangeCspace, csInterv, penv.width, &penv.pitch, &penv.depth, &penv.pad, NULL, cspar);
        if (csmap != NULL)
        {
            mBar->Enable(MENU_IMAGE_CSPACE_EDIT, TRUE);
            if (doProjRangeCspace)
            {
                setCspaceProjMode(doProjRangeCspace);
                csmap->updateProjection(csInterv);
            }
        }
        deleteViewCspace();
    }
    else
        penv.cspaceState = 0;

    penv.csmap = csmap;

    // Only allocate a new array if it's the first time or the size has changed.
    if ((penv.width != pixWidth) || (penv.height != pixHeight) || (pixPad != penv.pad) || (pixDepth != penv.depth) || (penv.pitch != virtualPitch))
    {
        pixWidth = penv.width;
        pixHeight = penv.height;
        pixPad = penv.pad;
        pixPitch = penv.pitch;
        pixDepth = penv.depth;
        virtualPitch = penv.pitch;
        if ((imgData = (char*)malloc(penv.pitch * pixHeight)) == NULL)
        {
            rviewErrorbox::reportError(lman->lookup("errorMemory"), rviewFlatBaseImage::getFrameName(), "projectImage");
            return NULL;
        }
    }

    if (rviewPerformFlatProjection(penv, imgData) != 0) return NULL;

    resizeImage();

    return imgData;
}




/*
 *  Standard flat images
 */

rviewFlatImage::rviewFlatImage(mdd_frame *mf, unsigned int flags) : rviewFlatBaseImage(mf, 0, flags)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatImage", "rviewFlatImage()" );
}

rviewFlatImage::~rviewFlatImage(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatImage", "~rviewFlatImage()" );
    closeViewer();
}

void rviewFlatImage::OnSize(int w, int h)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatImage", "OnSize()");

    if (initPhaseFinished)
    {
        int x, y, step, posx, posy;

        GetClientSize(&x, &y);

        if(x < 6*image_bwidth)
        {
            x= 6*image_bwidth;
            frameWidth=x;
            frameHeight=y;
            SetClientSize(x, y);
            return;
        }
    }
    rviewFlatBaseImage::OnSize(w, h);
}
int rviewFlatImage::openViewer(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatImage", "openViewer()" );

    if (rviewFlatBaseImage::openViewer() == 0)
    {
        openViewerEpilogue(rviewFlatImage::getFrameType());
        return 0;
    }
    return -1;
}


const char *rviewFlatImage::getFrameName(void) const
{
    return "rviewFlatImage";
}

rviewFrameType rviewFlatImage::getFrameType(void) const
{
    return rviewFrameTypeFlatImage;
}

int rviewFlatImage::getViewerType(void) const
{
    return RVIEW_RESDISP_IMGFLAT;
}


bool rviewFlatImage::moviePossible(void) const
{
    return (dimMDD >= 3);
}


char *rviewFlatImage::movieNewFrame(void)
{
    return projectImage();
}

void rviewFlatImage::label(void)
{
    setDisplayTitle(lman->lookup("titleImageFlat"));
    rviewFlatBaseImage::label();
}


int rviewFlatImage::process(wxObject &obj, wxEvent &evt)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewFlatImage", "process()" );

    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_SLIDER_COMMAND)
    {
        scaleValue = (double)(scaleSlider->GetValue());
        newProjection();
        return 1;
    }

    return rviewImage::process(obj, evt);
}


char *rviewFlatImage::initMode(void)
{
    if (playBack != NULL)
        playBack->Enable(TRUE);
    if (playFwd != NULL)
        playFwd->Enable(TRUE);

    return rviewFlatBaseImage::initMode();
}








/*
 *  Rendered image class members
 */

const char *rviewRenderImage::view_ZProject = "zProject";
const char *rviewRenderImage::view_ZClip = "zClip";
const char *rviewRenderImage::view_PixThreshLow = "pixThreshLow";
const char *rviewRenderImage::view_PixThreshHigh = "pixThreshHigh";
const char *rviewRenderImage::view_WeightThresh = "weightThresh";
const char *rviewRenderImage::view_WeightQuant = "weightQuant";
const char *rviewRenderImage::view_UseRGBBright = "useRgbBright";
const char *rviewRenderImage::view_UseLighting = "useLighting";
const char *rviewRenderImage::view_LightAmbient = "lightAmbient";
const char *rviewRenderImage::view_LightGain = "lightGain";
const char *rviewRenderImage::view_LightAngle = "lightAngle";
const char *rviewRenderImage::view_LightScint = "lightScintAngle";
const char *rviewRenderImage::view_LightDir = "lightDirection";
const char *rviewRenderImage::view_LightDist = "lightDistance";
const char *rviewRenderImage::view_KernelSize = "kernelSize";
const char *rviewRenderImage::view_KernelType = "kernelType";
const char *rviewRenderImage::view_UseVoxColour = "useVoxColour";
const char *rviewRenderImage::view_VoxColour = "voxColour";
const char *rviewRenderImage::view_GridSize = "gridSize";
const char *rviewRenderImage::view_ScaleHeight = "scaleHeight";
const char *rviewRenderImage::view_Rotation = "rotation";
const char *rviewRenderImage::view_ZOffset = "zOffset";

rviewRenderImage::rviewRenderImage(mdd_frame *mf, int es, unsigned int flags) : rviewImage(mf, es, flags)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "rviewRenderImage()" );

    // make destructor-safe first
    setupWindow = NULL;
    rcontrol = NULL;
    rcurview = NULL;
    geomData = NULL;
    geomUse = NULL;
    rot = NULL;
    graphEnv = NULL;

    // Default renderer parameters
    setup.zpro = prefs->imgZpro;
    setup.clipz = prefs->imgClipz;
    drx = 0.0;
    dry = 0.0;
    drz = 0.0;

    // Default voxel parameters. Try to use halfway sensible defaults. Values of -1
    // saved in the preferences object are substituted by default-values
    setup.useRgbBrightness = prefs->imgRgbBrightness;
    setup.weightQuantisation = 4;
    setup.pixelThresholdLow = prefs->imgPixThreshLow;
    setup.pixelThresholdHigh = prefs->imgPixThreshHigh;
    setup.weightThreshold = prefs->imgWgtThresh;
    // Light defaults
    setup.useLights = prefs->imgLight;
    setup.lightsAngle = prefs->imgLightAngle;
    setup.lightsScintAngle = prefs->imgLightScintAngle;
    setup.lightsAmbient = prefs->imgLightAmbient;
    setup.lightsGain = prefs->imgLightGain;
    setup.lightsDir = rviewImageSetup::parseLightDirection(prefs->imgLightDir);
    setup.lightsDist = prefs->imgLightDist;
    setup.kernelSize = prefs->imgKernSize;
    setup.kernelType = prefs->imgKernType + 1;
    setup.useVoxCol = prefs->imgUseVCol;
    setup.voxColour = prefs->imgVoxColour;
    // Height field defaults
    setup.gridSize = prefs->imgHeightGrid;
    setup.scaleHeight = prefs->imgHeightScale;

    rendererPlayback = 0;

    geomData = new vertex_fp[4];
    geomUse = new vertex_fp[4];
    rot = new vertex_fp[3];
    graphEnv = new graph_env;
}


rviewRenderImage::~rviewRenderImage(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "~rviewRenderImage()" );

    if (setupWindow != NULL)
    {
        setupWindow->unlinkParent();
        setupWindow->Close(TRUE);
    }
    if (rcontrol != NULL)
    {
        rcontrol->unlinkParent();
        rcontrol->Close(TRUE);
    }
    if (rcurview != NULL)
    {
        rcurview->unlinkParent();
        rcurview->Close(TRUE);
    }
    if(geomData)
    {
        delete [] geomData;
        geomData=0;
    }

    if(geomUse)
    {
        delete [] geomUse;
        geomUse=0;
    }
    if(rot)
    {
        delete [] rot;
        rot=0;
    }

    if(graphEnv)
    {
        delete graphEnv;
        graphEnv=0;
    }
}


const char *rviewRenderImage::getFrameName(void) const
{
    return "rviewRenderImage";
}

rviewFrameType rviewRenderImage::getFrameType(void) const
{
    return rviewFrameTypeRndImage;
}


int rviewRenderImage::configMenuInitHook(wxMenu *menu)
{
    menu->Append(MENU_IMAGE_SETUP_RENDER, "");
    menu->Append(MENU_IMAGE_SETUP_RCONTROL, "");
    menu->AppendSeparator();
    return 3;
}


int rviewRenderImage::viewMenuInitHook(wxMenu *menu)
{
    menu->Append(MENU_DISP_VIEW_SHOW, "");
    return 1;
}


void rviewRenderImage::label(void)
{
    mBar->SetLabel(MENU_IMAGE_SETUP_RENDER, lman->lookup("menImgSetupRender"));
    mBar->SetLabel(MENU_IMAGE_SETUP_RCONTROL, lman->lookup("menImgSetupRctrl"));
    mBar->SetLabel(MENU_DISP_VIEW_SHOW, lman->lookup("menDispViewShow"));

    rviewImage::label();
}


int rviewRenderImage::newProjection(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "newProjection()" );

    char *data, *lastData = imgData;

    if ((data = setupGraphEnv()) == NULL) return -1;
    fillBuffer();

    updatePixmap(lastData, data);

    return 0;
}


void rviewRenderImage::updateCurrentView(void)
{
    if (rcurview != NULL)
    {
        vertex_fp angles;
        matrixToAngles(angles);
        rcurview->updateView(angles, zoff, cubeScale);
    }
}


int rviewRenderImage::process(wxObject &obj, wxEvent &evt)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "process()" );

    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_SLIDER_COMMAND)
    {
        scaleValue = (double)(scaleSlider->GetValue());
        cubeScale = scaleValue / 100.0;
        if (cubeScale < 0.01) cubeScale = 0.01;
        fillBuffer();
        pcanv->updateDisplay(TRUE);
        updateCurrentView();
        return 1;
    }

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)playStop)
        {
            rendererPlayback = 0;
            if (rcontrol != NULL)
                rcontrol->setActiveMode(0);

            return 1;
        }
    }

    return rviewImage::process(obj, evt);
}


bool rviewRenderImage::canRotateObject(void) const
{
    return TRUE;
}


void rviewRenderImage::rotateObject(wxMouseEvent &mevt)
{
    float pos;

    // Rotate / translate 3D object
    if (mevt.leftDown)
    {
        pos = mevt.x - mousex;
        rotateCube(1, pos / 100);
        pos = mevt.y - mousey;
        rotateCube(0, pos / 100);
        fillBuffer();
        pcanv->updateDisplay();
        updateCurrentView();
    }

    if (mevt.rightDown)
    {
        pos = mevt.y - mousey;
        zoff += (long)pos;
        fillBuffer();
        pcanv->updateDisplay();
        updateCurrentView();
    }
}


void rviewRenderImage::OnSize(int w, int h)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "OnSize()" );

    int oldwidth = frameWidth;
    int oldheight = frameHeight;

    rviewImage::OnSize(w, h);

    // Fully initialized yet?
    if ((initPhaseFinished) && ((oldwidth != frameWidth) || (oldheight != frameHeight)))
    {
        char *data, *lastData = imgData;

        if ((data = setupGraphEnv()) != NULL)
        {
            fillBuffer();
            updatePixmap(lastData, data);
        }
    }
}


void rviewRenderImage::OnMenuCommand(int id)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "OnMenuCommand()" );

    switch (id)
    {
    case MENU_IMAGE_SETUP_RENDER:
        if (setupWindow == NULL)
        {
            setupWindow = new rviewImageSetup(&setup, this);
        }
        break;
    case MENU_IMAGE_SETUP_RCONTROL:
        if (rcontrol == NULL)
        {
            rcontrol = new rendererControl(drx, dry, drz, rendererPlayback, this);
        }
        break;
    case MENU_DISP_VIEW_SHOW:
        if (rcurview == NULL)
        {
            vertex_fp angles;
            matrixToAngles(angles);
            rcurview = new rendererCurrentView(angles, zoff, cubeScale, this);
        }
        break;
    default:
        rviewImage::OnMenuCommand(id);
        break;
    }
}


void rviewRenderImage::closeEditor(bool newSetup)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "closeEditor()" );

    if (newSetup)
    {
        updateSettings(FALSE);
    }
    setupWindow->Close(TRUE);
    setupWindow = NULL;
}


void rviewRenderImage::redrawSettingsChanged(void)
{
    fillBuffer();
    pcanv->updateDisplay();
}


void rviewRenderImage::updateSettings(int setFlags)
{
    bool doUpt = FALSE;

    if (setFlags == 0)
        doUpt = TRUE;
    else
        doUpt = doUpdate(setFlags);

    if (doUpt)
    {
        redrawSettingsChanged();
    }
}


bool rviewRenderImage::OnClose(void)
{
    if (rendererPlayback != 0) return FALSE;
    return rviewImage::OnClose();
}


int rviewRenderImage::userEvent(const user_event &ue)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "userEvent()" );

    if (ue.type == usr_child_closed)
    {
        if (ue.data == (void*)setupWindow)
        {
            setupWindow = NULL;
            return 1;
        }
        else if (ue.data == (void*)rcontrol)
        {
            rcontrol = NULL;
            return 1;
        }
        else if (ue.data == (void*)rcurview)
        {
            rcurview = NULL;
            return 1;
        }
    }
    return rviewImage::userEvent(ue);
}


void rviewRenderImage::closeRendererControls(void)
{
    if (rcontrol != NULL)
    {
        rcontrol->Close(TRUE);
        rcontrol = NULL;
    }
}


void rviewRenderImage::prepareToDie(void)
{
    rendererPlayback = 0;
    rviewImage::prepareToDie();
}


int rviewRenderImage::requestQuit(int level)
{
    rendererPlayback = 0;
    return rviewImage::requestQuit(level);
}


void rviewRenderImage::setAutoRotation(float rx, float ry, float rz)
{
    //cout << "rotate " << rx << ", " << ry << ", " << rz << endl;

    if (rx >= 100.0)
    {
        rendererPlayback = 0;
        return;
    }
    drx = rx;
    dry = ry;
    drz = rz;

    // No re-entrancy
    if (rendererPlayback != 0) return;

    rendererPlayback = 1;

    while (rendererPlayback != 0)
    {
        rotateCube(0, M_PI*drx/10);
        rotateCube(1, M_PI*dry/10);
        rotateCube(2, M_PI*drz/10);
        updateCurrentView();
        newProjection();
        ::wxYield();
    }
}


void rviewRenderImage::setCurrentView(const vertex_fp &angles, long off, double scale)
{
    anglesToMatrix(angles);
    zoff = off;
    cubeScale = scale;
    scaleSlider->SetValue((int)(100*cubeScale));

    newProjection();
}


char *rviewRenderImage::initMode(void)
{
    int i;
    char *data;

    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "initMode()" );

    // Scaling factor for the data cube.
    cubeScale = scaleValue / 100.0;
    if (cubeScale < 0.01) cubeScale = 0.01;

    // Init rotation matrix
    for (i=0; i<3; i++)
    {
        rot[i].x = 0.0;
        rot[i].y = 0.0;
        rot[i].z = 0.0;
    }
    rot[0].x = 1.0;
    rot[1].y = 1.0;
    rot[2].z = 1.0;

    zoff = 0;

    data = setupGraphEnv();

    if (initPhaseFinished) fillBuffer();

    // Do not update the pixmap here! That's done by the calling procedure, if necessary

    resizeImage();

    return data;
}


char *rviewRenderImage::setupGraphEnv(void)
{
    int w, h;

    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "setupGraphEnv()" );

    if (rviewParseProjection(getVirtualDomain(), pt1, pt2, projString, &freeDims) != dimMDD)
    {
        rviewErrorbox::reportError(lman->lookup("errorProjection"), rviewRenderImage::getFrameName(), "setupGraphEnv");
        return NULL;
    }

    pcanv->GetClientSize(&w, &h);

    setupEnvironment(w, h);

    graphEnv->zpro = setup.zpro;
    graphEnv->clipz = setup.clipz;
    graphEnv->clipl = -pixWidth/2;
    graphEnv->clipr = graphEnv->clipl + pixWidth - 1;
    graphEnv->clipd = -pixHeight/2;
    graphEnv->clipu = graphEnv->clipd + pixHeight - 1;
    graphEnv->midx = -graphEnv->clipl;
    graphEnv->midy = graphEnv->clipu;
    /* lineadd set in one of the modules above */
    graphEnv->dest = (void*)imgData;

    return imgData;
}


void rviewRenderImage::rotateCube(int axis, float angle, vertex_fp *matrix)
{
    real_t c, s;
    vertex_fp h;
    int i, j;

    c = cos(angle);
    s = sin(angle);

    switch (axis)
    {
    case 0:
        i = 1;
        j = 2;
        break;
    case 1:
        i = 2;
        j = 0;
        break;
    case 2:
        i = 0;
        j = 1;
        break;
    default:
        cerr << "Bad rotation axis " << axis;
        return;
    }
    h.x = c * matrix[i].x + s * matrix[j].x;
    h.y = c * matrix[i].y + s * matrix[j].y;
    h.z = c * matrix[i].z + s * matrix[j].z;
    matrix[j].x = c * matrix[j].x - s * matrix[i].x;
    matrix[j].y = c * matrix[j].y - s * matrix[i].y;
    matrix[j].z = c * matrix[j].z - s * matrix[i].z;
    matrix[i].x = h.x;
    matrix[i].y = h.y;
    matrix[i].z = h.z;

    /*for (i=0; i<3; i++)
    {
      cout << '[' << matrix[i].x << ',' << matrix[i].y << ',' << matrix[i].z << ']';
    }
    cout << endl;*/

}

void rviewRenderImage::rotateCube(int axis, float angle)
{
    rotateCube(axis, angle, rot);
}


void rviewRenderImage::getLightPos(vertex_fp *lpos)
{
    lpos->x = 0;
    lpos->y = 0;
    lpos->z = 0;
    if ((setup.lightsDir & RVIEW_LIGHTDIR_LEFT) != 0) lpos->x = -1.0;
    else if ((setup.lightsDir & RVIEW_LIGHTDIR_RIGHT) != 0) lpos->x = 1.0;
    if ((setup.lightsDir & RVIEW_LIGHTDIR_DOWN) != 0) lpos->y = -1.0;
    else if ((setup.lightsDir & RVIEW_LIGHTDIR_UP) != 0) lpos->y = 1.0;
    if ((setup.lightsDir & RVIEW_LIGHTDIR_FRONT) != 0) lpos->z = -1.0;
    else if ((setup.lightsDir & RVIEW_LIGHTDIR_BACK) != 0) lpos->z = 1.0;
    if (setup.lightsDir != 0)
    {
        double h;

        h = ((double) setup.lightsDist) /sqrt((lpos->x * lpos->x) + (lpos->y * lpos->y) + (lpos->z * lpos->z));
        lpos->x *= h;
        lpos->y *= h;
        lpos->z *= h;
    }
    lpos->z += graphEnv->zpro;
}


// not correct for x==0, but I don't need that case
#define SIGN(x) ((x > 0) ? 1 : -1)

#define PRINT_MATRIX(mat) { \
  for (unsigned int i=0; i<3; i++) \
    cout << i << ": " << (mat)[i].x << ", " << (mat)[i].y << ", " << (mat)[i].z << endl; \
  }

void rviewRenderImage::matrixToAngles(vertex_fp &angles) const
{
    vertex_fp matrix[3];

    // make copy of rotation matrix for working
    memcpy(&matrix, rot, 3*sizeof(vertex_fp));

    // Successively rotate it back to identity. Getting this right is a major headache;
    // one must bear in mind that rot[] are row-vectors and that atan() only returns
    // values in -pi/2 to pi/2, so the result may have to be shifted by pi.
    if (matrix[1].x == 0.0)
    {
        angles.z = 0.0;
    }
    else
    {
        angles.z = -((matrix[0].x == 0.0) ? SIGN(matrix[1].x) * M_PI/2 : atan(matrix[1].x / matrix[0].x));
    }
    if (matrix[0].x < 0) angles.z += M_PI;
    if (angles.z != 0.0)
    {
        rotateCube(2, -angles.z, matrix);
        //cout << "ROTz" << endl; PRINT_MATRIX(matrix);
    }

    if (matrix[2].x == 0.0)
    {
        angles.y = 0.0;
    }
    else
    {
        angles.y = -((matrix[0].x == 0.0) ? -SIGN(matrix[2].x) * M_PI/2 : -atan(matrix[2].x / matrix[0].x));
        rotateCube(1, -angles.y, matrix);
        //cout << "ROTy" << endl; PRINT_MATRIX(matrix);
    }

    if (matrix[2].y == 0.0)
    {
        angles.x = 0.0;
    }
    else
    {
        angles.x = -((matrix[1].y == 0.0) ? SIGN(matrix[2].y) * M_PI/2 : atan(matrix[2].y / matrix[1].y));
    }
    if (matrix[1].y < 0) angles.x += M_PI;
    if (angles.x != 0.0)
    {
        rotateCube(0, -angles.x, matrix);
        //cout << "ROTx" << endl; PRINT_MATRIX(matrix);
    }

#if 0
    rotateCube(0, angles.x, matrix);
    rotateCube(1, angles.y, matrix);
    rotateCube(2, angles.z, matrix);
    unsigned int i;
    double res;
    for (i=0, res=0.0; i<3; i++)
    {
        cout << "REC " << i << ": " << matrix[i].x << ", " << matrix[i].y << ", " << matrix[i].z
             << " vs. " << rot[i].x << ", " << rot[i].y << ", " << rot[i].z << endl;
        res += (matrix[i].x - rot[i].x) * (matrix[i].x - rot[i].x) + (matrix[i].y - rot[i].y) * (matrix[i].y - rot[i].y) + (matrix[i].z - rot[i].z) * (matrix[i].z - rot[i].z);
    }
    cout << "Residuum " << res << endl;
#endif
}


void rviewRenderImage::anglesToMatrix(const vertex_fp &angles)
{
    unsigned int i;

    for (i=0; i<3; i++)
    {
        rot[i].x = 0.0;
        rot[i].y = 0.0;
        rot[i].z = 0.0;
    }
    rot[0].x = 1.0;
    rot[1].y = 1.0;
    rot[2].z = 1.0;

    rotateCube(0, angles.x);
    rotateCube(1, angles.y);
    rotateCube(2, angles.z);
}


int rviewRenderImage::saveView(FILE *fp)
{
    int status = rviewImage::saveView(fp);

    writeViewParam(fp, view_ZProject, (long)setup.zpro);
    writeViewParam(fp, view_ZClip, (long)setup.clipz);
    writeViewParam(fp, view_PixThreshLow, setup.pixelThresholdLow);
    writeViewParam(fp, view_PixThreshHigh, setup.pixelThresholdHigh);
    writeViewParam(fp, view_WeightThresh, setup.weightThreshold);
    writeViewParam(fp, view_WeightQuant, (long)setup.weightQuantisation);
    writeViewParam(fp, view_UseRGBBright, (long)setup.useRgbBrightness);
    writeViewParam(fp, view_UseLighting, (long)setup.useLights);
    writeViewParam(fp, view_LightAmbient, setup.lightsAmbient);
    writeViewParam(fp, view_LightGain, setup.lightsGain);
    writeViewParam(fp, view_LightAngle, setup.lightsAngle);
    writeViewParam(fp, view_LightScint, setup.lightsScintAngle);
    writeViewParam(fp, view_LightDir, (long)setup.lightsDir);
    writeViewParam(fp, view_LightDist, (long)setup.lightsDist);
    writeViewParam(fp, view_KernelSize, (long)setup.kernelSize);
    writeViewParam(fp, view_KernelType, (long)setup.kernelType);
    writeViewParam(fp, view_UseVoxColour, (long)setup.useVoxCol);
    writeViewParam(fp, view_VoxColour, setup.voxColour);
    writeViewParam(fp, view_GridSize, (long)setup.gridSize);
    writeViewParam(fp, view_ScaleHeight, setup.scaleHeight);

    vertex_fp angles;
    double avals[3];
    matrixToAngles(angles);
    avals[0] = angles.x;
    avals[1] = angles.y;
    avals[2] = angles.z;
    writeViewParam(fp, view_Rotation, 3, avals);
    writeViewParam(fp, view_ZOffset, zoff);

    return status;
}


int rviewRenderImage::readView(const char *key, const char *value)
{
    int status = rviewImage::readView(key, value);

    if (status == 0)
    {
        if (strcmp(key, view_ZProject) == 0)
        {
            setup.zpro = (unsigned long)atol(value);
            return 1;
        }
        else if (strcmp(key, view_ZClip) == 0)
        {
            setup.clipz = (unsigned long)atol(value);
            return 1;
        }
        else if (strcmp(key, view_PixThreshLow) == 0)
        {
            setup.pixelThresholdLow = atof(value);
            return 1;
        }
        else if (strcmp(key, view_PixThreshHigh) == 0)
        {
            setup.pixelThresholdHigh = atof(value);
            return 1;
        }
        else if (strcmp(key, view_WeightThresh) == 0)
        {
            setup.weightThreshold = atof(value);
            return 1;
        }
        else if (strcmp(key, view_WeightQuant) == 0)
        {
            setup.weightQuantisation = atoi(value);
            return 1;
        }
        else if (strcmp(key, view_UseRGBBright) == 0)
        {
            setup.useRgbBrightness = (bool)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_UseLighting) == 0)
        {
            setup.useLights = (bool)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_LightAmbient) == 0)
        {
            setup.lightsAmbient = atof(value);
            return 1;
        }
        else if (strcmp(key, view_LightGain) == 0)
        {
            setup.lightsGain = atof(value);
            return 1;
        }
        else if (strcmp(key, view_LightAngle) == 0)
        {
            setup.lightsAngle = atof(value);
            return 1;
        }
        else if (strcmp(key, view_LightScint) == 0)
        {
            setup.lightsScintAngle = atof(value);
            return 1;
        }
        else if (strcmp(key, view_LightDir) == 0)
        {
            setup.lightsDir = atoi(value);
            return 1;
        }
        else if (strcmp(key, view_LightDist) == 0)
        {
            setup.lightsDist = atoi(value);
            return 1;
        }
        else if (strcmp(key, view_KernelSize) == 0)
        {
            setup.kernelSize = atoi(value);
            return 1;
        }
        else if (strcmp(key, view_KernelType) == 0)
        {
            setup.kernelType = atoi(value);
            return 1;
        }
        else if (strcmp(key, view_UseVoxColour) == 0)
        {
            setup.useVoxCol = (bool)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_VoxColour) == 0)
        {
            setup.voxColour = atof(value);
            return 1;
        }
        else if (strcmp(key, view_GridSize) == 0)
        {
            setup.gridSize = atoi(value);
            return 1;
        }
        else if (strcmp(key, view_ScaleHeight) == 0)
        {
            setup.scaleHeight = atof(value);
            return 1;
        }
        else if (strcmp(key, view_Rotation) == 0)
        {
            double avals[3];
            if (readVector(value, 3, avals) == 0)
            {
                vertex_fp angles;
                angles.x = avals[0];
                angles.y = avals[1];
                angles.z = avals[2];
                anglesToMatrix(angles);
            }
            return 1;
        }
        else if (strcmp(key, view_ZOffset) == 0)
        {
            zoff = atol(value);
            return 1;
        }
        return 0;
    }
    return status;
}


void rviewRenderImage::loadViewFinished(void)
{
    rviewImage::loadViewFinished();

    if (setupWindow != NULL)
        setupWindow->updateSettings(setup);

    if (rcurview != NULL)
    {
        vertex_fp angles;
        matrixToAngles(angles);
        rcurview->updateView(angles, zoff, cubeScale);
    }

    cubeScale = scaleValue / 100.0;
}



/*
 *  Handling the renderer buffer
 */

#define FILL_BACKGROUND_CORE(type) \
  type value, *imgSrcPtr; \
  int fbcx, fbcy; \
  imgSrcPtr = (type*)(graphEnv->dest); value = (type)minVal; \
  for (fbcy=0; fbcy < pixHeight; fbcy++) \
  { \
    for (fbcx=0; fbcx < pixWidth; fbcx++) \
    { \
      imgSrcPtr[fbcx] = value; \
    } \
    imgSrcPtr = (type*)(((char*)imgSrcPtr) + virtualPitch); \
  }

void rviewRenderImage::fillBackgroundCore(rviewBaseType bt, double minVal)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "fillBackgroundCore()" );

    switch (bt)
    {
    case rbt_char:
    {
        FILL_BACKGROUND_CORE(r_Char);
    }
    break;
    case rbt_uchar:
    {
        FILL_BACKGROUND_CORE(r_Octet);
    }
    break;
    case rbt_short:
    {
        FILL_BACKGROUND_CORE(r_Short);
    }
    break;
    case rbt_ushort:
    {
        FILL_BACKGROUND_CORE(r_UShort);
    }
    break;
    case rbt_long:
    {
        FILL_BACKGROUND_CORE(r_Long);
    }
    break;
    case rbt_ulong:
    {
        FILL_BACKGROUND_CORE(r_ULong);
    }
    break;
    case rbt_float:
    {
        FILL_BACKGROUND_CORE(r_Float);
    }
    break;
    case rbt_double:
    {
        FILL_BACKGROUND_CORE(r_Double);
    }
    break;
    default:
        break;
    }
}

void rviewRenderImage::fillBufferBackground(bool doCspace, bool &cspaceOK, r_Ref<r_GMarray> &obj, colourspaceMapper **csm, r_Minterval *csdom, rviewBaseType bt, bool fullRange, double *useMinVal)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "fillBufferBackground()" );

    // In case of colourspace mapping and rendering don't fill the background with 0 but
    // rather with the currently configured minimum value!
    if (doCspace)
    {
        cspaceOK = FALSE;
        if (rviewCheckInitCspace(bt, csm, obj, fullRange, csdom) != 0)
        {
            double minVal;

            cspaceOK = TRUE;
            minVal = (useMinVal == NULL) ? (*csm)->getMinVal() : *useMinVal;

            fillBackgroundCore(bt, minVal);
        }
    }
    else
    {
        memset(graphEnv->dest, 0, pixPitch * pixHeight);
    }
}


// Macro for colourspace translation from/to various basetypes
// Fill in inverse order because source type may be smaller than destination type
#define TRANSLATE_TO_COLOURSPACE15(type) \
   type *imgSrcPtr; \
   long value; \
   for (j=0; j<pixHeight; j++, imgLine+=pixPitch) \
   { \
     imgPtrS = ((unsigned short*)imgLine) + pixWidth-1; imgSrcPtr = ((type*)imgLine) + pixWidth-1; \
     for (i=0; i<pixWidth; i++, imgPtrS--, imgSrcPtr--) \
     { \
       value = (long)(*imgSrcPtr); if (value > maxValL) value = maxValL; \
       value -= minValL; if (value < 0) value = 0; \
       *imgPtrS = IntToRGBTab15[value]; \
     } \
   }

// Fill in ascending order here because the destination type may be smaller than the source type
#define TRANSLATE_TO_COLOURSPACE32(type, minval, maxval, scale) \
  type value, *imgSrcPtr; \
  char *imgSrcBase = (char*)imgLine; \
  if (IntToRGBTab24 == NULL) \
  { \
    for (j=0; j<pixHeight; j++, imgLine+=pixPitch, imgSrcBase+=virtualPitch) \
    { \
      imgPtrL = (unsigned long*)imgLine; imgSrcPtr = (type*)imgSrcBase; \
      for (i=0; i<pixWidth; i++, imgPtrL++, imgSrcPtr++) \
      { \
    *imgPtrL = csmap->ValToCS24((double)(*imgSrcPtr) - minVal); \
      } \
    } \
  } \
  else \
  { \
    for (j=0; j<pixHeight; j++, imgLine+=pixPitch, imgSrcBase+=virtualPitch) \
    { \
      imgPtrL = (unsigned long*)imgLine; imgSrcPtr = (type*)imgSrcBase; \
      for (i=0; i<pixWidth; i++, imgPtrL++, imgSrcPtr++) \
      { \
    value = *imgSrcPtr; if (value > maxval) value = maxval; \
    value -= minval; if (value < 0) value = 0; \
    *imgPtrL = IntToRGBTab24[(unsigned long)(value * scale)]; \
      } \
    } \
  }

void rviewRenderImage::translateBufferToCspace(rviewBaseType bt, double *useMinVal, double *useMaxVal)
{
    double minVal;
    unsigned char *imgLine;
    unsigned short *IntToRGBTab15, *imgPtrS;
    unsigned long *IntToRGBTab24, *imgPtrL;
    double maxVal, scalingFactor;
    long minValL, maxValL;
    int i, j;

    RMDBGONCE(3, RMDebug::module_applications, "rviewRenderImage", "translateBufferToCspace()" );

    // If bounding boxes are drawn there might be pixels out of range here, so trap those!
    minVal = (useMinVal == NULL) ? csmap->getMinVal() : *useMinVal;
    maxVal = (useMaxVal == NULL) ? csmap->getMaxVal() : *useMaxVal;
    minValL = (long)minVal;
    maxValL = (long)maxVal;
    scalingFactor = csmap->getScalingFactor();

    imgLine = (unsigned char*)(graphEnv->dest);
    IntToRGBTab15 = csmap->getCSTab15();
    IntToRGBTab24 = csmap->getCSTab24();

    switch (bt)
    {
    case rbt_char:
    {
        TRANSLATE_TO_COLOURSPACE15(r_Char);
    }
    break;
    case rbt_uchar:
    {
        TRANSLATE_TO_COLOURSPACE15(r_Octet);
    }
    break;
    case rbt_short:
    {
        TRANSLATE_TO_COLOURSPACE15(r_Short);
    }
    break;
    case rbt_ushort:
    {
        TRANSLATE_TO_COLOURSPACE15(r_UShort);
    }
    break;
    case rbt_long:
    {
        TRANSLATE_TO_COLOURSPACE32(r_Long, minValL, maxValL, 1);
    }
    break;
    case rbt_ulong:
    {
        TRANSLATE_TO_COLOURSPACE32(r_ULong, (r_ULong)minValL, (r_ULong)maxValL, 1);
    }
    break;
    case rbt_float:
    {
        TRANSLATE_TO_COLOURSPACE32(r_Float, minVal, maxVal, scalingFactor);
    }
    break;
    case rbt_double:
    {
        TRANSLATE_TO_COLOURSPACE32(r_Double, minVal, maxVal, scalingFactor);
    }
    break;
    default:
    {
        rviewErrorbox::reportError(lman->lookup("errorBaseType"), rviewRenderImage::getFrameName(), "translateBufferToCspace");
        return;
    }
    }
}


int rviewRenderImage::setupEnvBase(int w, int h, r_Ref<r_GMarray> &mdd, colourspaceMapper **csm, r_Minterval *csdom)
{
    int needDepth, newPitch, newPad, newVirtPitch;

    needDepth = 8*baseSize;
    if ((rviewImageTypes[baseType] == RVIEW_IMGTYPE_NONE) && (doValToCspace)) needDepth = 32;
    if (rviewImageTypes[baseType] == RVIEW_IMGTYPE_HIGH) needDepth = 15;
    if (rviewImageTypes[baseType] == RVIEW_IMGTYPE_GREY12) needDepth = 12;
    newPad = 64;
    newPitch = (w * baseSize + 7) & ~7;
    newVirtPitch = newPitch;

    if (doValToCspace)
    {
        // Init if necessary
        setCspaceProjMode(doProjRangeCspace);
        rviewCheckInitCspace(baseType, csm, mdd, doFullRangeCspace, csdom, w, &newPitch, &needDepth, &newPad, &newVirtPitch, cspar);
        if (*csm != NULL)
        {
            mBar->Enable(MENU_IMAGE_CSPACE_EDIT, TRUE);
            if (doProjRangeCspace)
            {
                setCspaceProjMode(doProjRangeCspace);
                (*csm)->updateProjection(csdom);
            }
        }
        deleteViewCspace();
    }

    if ((w != pixWidth) || (h != pixHeight) || (pixPad != newPad) || (pixDepth != needDepth) || (newPitch != pixPitch) || (newVirtPitch != virtualPitch))
    {
        pixWidth = w;
        pixHeight = h;
        pixDepth = needDepth;
        pixPitch = newPitch;
        pixPad = newPad;
        virtualPitch = newVirtPitch;
        if ((imgData = (char*)malloc(virtualPitch * pixHeight)) == NULL)
        {
            rviewErrorbox::reportError(lman->lookup("errorMemory"), getFrameName(), "setupEnvironment");
            return -1;
        }
    }

    graphEnv->lineadd = virtualPitch;

    return 0;
}




/*
 *  Volume image renderer members
 */

const char *rviewVolumeImage::view_VolumeMode = "volumeMode";
const char *rviewVolumeImage::view_UseBBox = "useBBox";

rviewVolumeImage::rviewVolumeImage(mdd_frame *mf, unsigned int flags) : rviewRenderImage(mf, 0, flags)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewVolumeImage", "rviewVolumeImage()" );

    // make destructor safe
    texDesc = NULL;
    voxDesc = NULL;

    initVoxParams = FALSE;
    // Mode defaults
    imode = prefs->imgMode;
    if ((imode != rim_surf) && (imode != rim_voxel)) imode = rim_surf;

    if (prefs->imgVoxForType != 0)
    {
        initVoxParams = TRUE;
        switch (baseSize)
        {
        case 1:
            setup.pixelThresholdLow = 4.0;
            setup.pixelThresholdHigh = 1e6;
            setup.weightThreshold = 64.0;
            setup.voxColour = 0xff;
            break;
        case 2:
            setup.pixelThresholdLow = 256.0;
            setup.pixelThresholdHigh = 65535.0;
            setup.weightThreshold = 64.0;
            setup.voxColour = 0xffff;
            break;
        case 3:
            setup.pixelThresholdLow = 4.0;
            setup.pixelThresholdHigh = 1e6;
            setup.weightThreshold = 64.0;
            setup.voxColour = 0xffffff;
            break;
        case 4:
            setup.pixelThresholdLow = 16384.0;
            setup.pixelThresholdHigh = 1e6;
            setup.weightThreshold = 64.0;
            setup.voxColour = 0xffffffff;
            break;
        case 8:
            setup.pixelThresholdLow = 0.0;
            setup.pixelThresholdHigh = 1e12;
            setup.weightThreshold = 64.0;
            setup.voxColour = 1e12;
        default:
            break;
        }
    }

    // Align bbox with projection's OK-button
    boundingBox = new rviewCheckBox(ctrlPanel);
    // Preferences
    doBoundingBox = prefs->imgBBox;
    boundingBox->SetValue(doBoundingBox);

    texDesc = new tex_desc;
    voxDesc = new voxel_desc;
    texDesc->floatType = ((baseType == rbt_float) || (baseType == rbt_double)) ? 1 : 0;

}


int rviewVolumeImage::openViewer(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewVolumeImage", "openViewer()" );

    if (dimMDD != 3)
    {
        rviewErrorbox::reportError(lman->lookup("errorModeDim"), rviewVolumeImage::getFrameName(), "openViewer");
        objectInitializedOK = FALSE;
        return -1;
    }

    if (rviewRenderImage::openViewer() == 0)
    {
        // first add the menus of the parent class
        int madd = rviewRenderImage::menuBarInitHook();

        // then add my own
        wxMenu *modes;
        char buffer[STRINGSIZE];
        modes = new wxMenu;
        modes->Append(MENU_IMAGE_MODE_SURF, "", NULL, TRUE);
        modes->Append(MENU_IMAGE_MODE_VOXEL, "", NULL, TRUE);
        sprintf(buffer, "&%s", lman->lookup("menImgMode"));
        mBar->Append(modes, buffer);

        switch (imode)
        {
        case rim_surf:
            lastMode = MENU_IMAGE_MODE_SURF;
            break;
        case rim_voxel:
            lastMode = MENU_IMAGE_MODE_VOXEL;
            break;
        default:
            break;
        }
        modes->Check(lastMode, TRUE);

        openViewerEpilogue(rviewVolumeImage::getFrameType());

        return 0;
    }
    return -1;
}


rviewVolumeImage::~rviewVolumeImage(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewVolumeImage" ,"~rviewVolumeImage()" );

    closeViewer();
    delete texDesc;
    delete voxDesc;
}


const char *rviewVolumeImage::getFrameName(void) const
{
    return "rviewVolumeImage";
}

rviewFrameType rviewVolumeImage::getFrameType(void) const
{
    return rviewFrameTypeVolImage;
}

int rviewVolumeImage::getViewerType(void) const
{
    return RVIEW_RESDISP_IMGVOLM;
}


void rviewVolumeImage::label(void)
{
    setDisplayTitle(lman->lookup("titleImageVolume"));

    boundingBox->SetLabel(lman->lookup("textBBox"));

    mBar->SetLabel(MENU_IMAGE_MODE_SURF, lman->lookup("menImgModeSurf"));
    mBar->SetLabel(MENU_IMAGE_MODE_VOXEL, lman->lookup("menImgModeVoxel"));
    mBar->SetLabelTop(fixedNumberOfMenus + 1, lman->lookup("menImgMode"));

    rviewRenderImage::label();
}


int rviewVolumeImage::process(wxObject &obj, wxEvent &evt)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewVolumeImage", "process()" );

    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_CHECKBOX_COMMAND)
    {
        if (&obj == (wxObject*)boundingBox)
        {
            doBoundingBox = boundingBox->GetValue();
            fillBuffer();
            pcanv->updateDisplay();
            return 1;
        }
    }

    return rviewRenderImage::process(obj, evt);
}


void rviewVolumeImage::OnMenuCommand(int id)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewVolumeImage", "OnMenuCommand()" );

    rviewImageMode newMode = rim_none;

    switch (id)
    {
    case MENU_IMAGE_MODE_SURF:
        newMode = rim_surf;
        break;
    case MENU_IMAGE_MODE_VOXEL:
        newMode = rim_voxel;
        break;
    default:
        rviewRenderImage::OnMenuCommand(id);
        break;
    }
    if (newMode != rim_none)
    {
        configureMode();

        // We have to do this in any case or the menus get screwed
        mBar->Check(lastMode, FALSE);
        mBar->Check(id, TRUE);
        lastMode = id;

        if (newMode != imode)
        {
            imode = newMode;
            fillBuffer();
            updatePixmap(imgData, imgData);
        }
    }
}


void rviewVolumeImage::OnSize(int w, int h)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewVolumeImage", "OnSize()" );

    rviewRenderImage::OnSize(w, h);

    if (boundingBox != NULL)
        boundingBox->SetSize(w + 2*display_cnvborder - 3*display_border - 4*display_pbwidth, display_border, image_bbwidth, image_bbheight);
}


bool rviewVolumeImage::doUpdate(int flags)
{
    if (((flags & RVIEW_IFLAG_VOXEL) != 0) && (imode == rim_voxel)) return TRUE;
    if (((flags & RVIEW_IFLAG_LIGHT) != 0) && setup.useLights) return TRUE;
    return FALSE;
}


char *rviewVolumeImage::initMode(void)
{
    RMDBGENTER(3, RMDebug::module_applications, "rviewVolumeImage", "initMode()" );

    // Initialise the projection string. Use a static default for now, later on use the
    // last value used.
    sprintf(projString, "*:*, *:*, *:*");

    project->SetValue(projString);
    setModeDimension(3);

    if (boundingBox != NULL)
        boundingBox->Enable(TRUE);

    texDesc->dimx = interv[0].high() - interv[0].low() + 1;
    texDesc->dimy = interv[1].high() - interv[1].low() + 1;
    texDesc->dimz = interv[2].high() - interv[2].low() + 1;
    texDesc->baseSize = baseSize;

    RMDBGEXIT(3, RMDebug::module_applications, "rviewVolumeImage", "initMode() tx=" << texDesc->dimx << ", ty=" << texDesc->dimy << ", tz=" << texDesc->dimz << ", tbase=" << texDesc->baseSize );

    return rviewRenderImage::initMode();
}


char *rviewVolumeImage::setupEnvironment(int w, int h)
{
    RMDBGENTER(3, RMDebug::module_applications, "rviewVolumeImage", "setupEnvironment()" );

    int i, offset;

    if (setupEnvBase(w, h, mddObj, &csmap, csInterv) != 0)
        return NULL;

    // These values change for each projection
    texDesc->widthx = pt2[0] - pt1[0] + 1;
    texDesc->widthy = pt2[1] - pt1[1] + 1;
    texDesc->widthz = pt2[2] - pt1[2] + 1;

    r_Ref<r_Marray<r_Char> > tempMdd = (r_Ref<r_Marray<r_Char> >) mddObj;
    // Do it like this to avoid having to check all base types
    r_Point paux = r_Point(dimMDD);
    for (i=0; i<dimMDD; i++)
    {
        paux[i] = interv[i].low();
    }
    offset = ((int)(&((*tempMdd)[pt1]) - &((*tempMdd)[paux]))) * baseSize;

    texDesc->data = (void*)((char*)(tempMdd->get_array()) + offset);

    for (i=0; i<4; i++)
    {
        geomData[i].x = 0;
        geomData[i].y = 0;
        geomData[i].z = 0;
    }
    geomData[1].x = (real_t)(pt2[0] - pt1[0] + 1);
    geomData[2].y = (real_t)(pt2[1] - pt1[1] + 1);
    geomData[3].z = (real_t)(pt2[2] - pt1[2] + 1);

    RMDBGMIDDLE(3, RMDebug::module_applications, "rviewVolumeImage", "w=" << pixWidth << ", h=" << pixHeight << ", p=" << pixPitch << ", cl=" << graphEnv->clipl << ", cr=" << graphEnv->clipr << ", cd=" << graphEnv->clipd << ", cu=" << graphEnv->clipu << ", mx=" << graphEnv->midx << ", my=" << graphEnv->midy << ", img=" << graphEnv->dest );

    RMDBGMIDDLE(3, RMDebug::module_applications, "rviewVolumeImage", "twx=" << texDesc->widthx << ", twy=" << texDesc->widthy << ", twz=" << texDesc->widthz << ", offset=" << (int)((char*)(texDesc->data) - mddObj->get_array()) << ", base size " << texDesc->baseSize << ", dest=" << (void*)imgData );

    RMDBGEXIT(3, RMDebug::module_applications, "rviewVolumeImage", "setupEnvironment()");
    return imgData;
}


void rviewVolumeImage::fillBuffer(void)
{
    int i;
    vertex_fp v;
    bool cspaceOK;

    RMDBGONCE(3, RMDebug::module_applications, "rviewVolumeImage", "fillBuffer()" );

    // Plot bounding box or not?
    graphEnv->bbox_colour = (doBoundingBox) ? 0xffffff : 0xffffffff;
    // Update z-rendering parameters
    graphEnv->zpro = setup.zpro;
    graphEnv->clipz = setup.clipz;

    for (i=1; i<4; i++)
    {
        // Make sure we don't get zero length vectors
        // (only a problem with strongly deformed ``cubes'', like planes)
        v.x = cubeScale * (geomData[i].x * rot[0].x + geomData[i].y * rot[0].y + geomData[i].z * rot[0].z);
        v.y = cubeScale * (geomData[i].x * rot[1].x + geomData[i].y * rot[1].y + geomData[i].z * rot[1].z);
        v.z = cubeScale * (geomData[i].x * rot[2].x + geomData[i].y * rot[2].y + geomData[i].z * rot[2].z);
        geomUse[i].x = v.x;
        geomUse[i].y = v.y;
        geomUse[i].z = v.z;
    }

    // Rotate around middle of cube
    geomUse[0].x = -(geomUse[1].x + geomUse[2].x + geomUse[3].x) / 2;
    geomUse[0].y = -(geomUse[1].y + geomUse[2].y + geomUse[3].y) / 2;
    geomUse[0].z = -(geomUse[1].z + geomUse[2].z + geomUse[3].z) / 2 + geomData[3].z / 2 + graphEnv->zpro + zoff;

    /*for (i=0; i<4; i++)
      {
        cout << i << ": " << geomUse[i].x << ", " << geomUse[i].y << ", " << geomUse[i].z << endl;
      }*/

    fillBufferBackground(doValToCspace, cspaceOK, mddObj, &csmap, csInterv, baseType, doFullRangeCspace);

    if ((texDesc->floatType != 0) && (csmap != NULL))
    {
        texDesc->minVal = csmap->getMinVal();
        texDesc->maxVal = csmap->getMaxVal();
    }

    if (imode == rim_surf)
    {
        RenderCubeSurf(geomUse, graphEnv, texDesc);
    }
    else
    {
        if (initVoxParams)
        {
            if (csmap != NULL)
            {
                setup.pixelThresholdLow = csmap->getMinVal();
                setup.pixelThresholdHigh = csmap->getMaxVal();
                setup.weightThreshold = (setup.pixelThresholdHigh - setup.pixelThresholdLow) / 4;
                setup.voxColour = csmap->getMaxVal();
                if (setupWindow != NULL) setupWindow->updateSettings(setup);
            }
            initVoxParams = FALSE;
        }

        voxDesc->pixelThresholdLow = setup.pixelThresholdLow;
        voxDesc->pixelThresholdHigh = setup.pixelThresholdHigh;
        voxDesc->weightThreshold = setup.weightThreshold;
        voxDesc->weightQuantisation = setup.weightQuantisation;
        voxDesc->useRgbBrightness = setup.useRgbBrightness;
        voxDesc->light.ambient = (setup.useLights) ? setup.lightsAmbient : -1.0;
        voxDesc->light.gain = setup.lightsGain;
        voxDesc->light.cosine = cos((M_PI * setup.lightsAngle) / 180);
        voxDesc->light.scintCos = cos((M_PI * setup.lightsScintAngle) / 180);
        getLightPos(&(voxDesc->light.lights));
        voxDesc->kernSize = setup.kernelSize;
        voxDesc->kernType = setup.kernelType;
        if (setup.useVoxCol)
        {
            switch (baseType)
            {
            case rbt_float:
                voxColour.f = (float)setup.voxColour;
                break;
            case rbt_double:
                voxColour.d = (double)setup.voxColour;
                break;
            default:
                voxColour.l = (unsigned long)setup.voxColour;
                break;
            }
            voxDesc->voxColour = (void*)&voxColour;
        }
        else
        {
            voxDesc->voxColour = NULL;
        }
        ::wxBeginBusyCursor(wxHOURGLASS_CURSOR);
        RenderCubeVoxel(geomUse, graphEnv, texDesc, voxDesc);
        ::wxEndBusyCursor();
    }

    if (doValToCspace && cspaceOK)
    {
        translateBufferToCspace(baseType);
    }
}


int rviewVolumeImage::saveView(FILE *fp)
{
    int status = rviewRenderImage::saveView(fp);

    writeViewParam(fp, view_VolumeMode, (long)imode);
    writeViewParam(fp, view_UseBBox, (long)doBoundingBox);

    return status;
}


int rviewVolumeImage::readView(const char *key, const char *value)
{
    int status = rviewRenderImage::readView(key, value);

    if (status == 0)
    {
        if (strcmp(key, view_VolumeMode) == 0)
        {
            imode = (rviewImageMode)atoi(value);
            return 1;
        }
        else if (strcmp(key, view_UseBBox) == 0)
        {
            doBoundingBox = (bool)atoi(value);
            return 1;
        }
        return 0;
    }
    return status;
}


void rviewVolumeImage::loadViewFinished(void)
{
    rviewRenderImage::loadViewFinished();

    mBar->Check(MENU_IMAGE_MODE_SURF, (imode == rim_surf));
    mBar->Check(MENU_IMAGE_MODE_VOXEL, (imode == rim_voxel));
    lastMode = imode;

    boundingBox->SetValue(doBoundingBox);

    // if we're in voxel mode, the parameters loaded are OK, don't overwrite them in fillBuffer()!
    if (imode == rim_voxel)
        initVoxParams = FALSE;
}



/*
 *  Height field rendered images members
 */
rviewHeightImage::rviewHeightImage(mdd_frame *mf, unsigned int flags) : rviewRenderImage(mf, 0, flags)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewHeightImage", "rviewHeightImage()" );

    // make destructor safe
    mddDesc = NULL;
    meshDesc = NULL;
    mddDesc = NULL;

    // create dummy MDD object
    r_Minterval dummyInterv(2);
    dummyInterv << r_Sinterval((r_Range)0, (r_Range)1) << r_Sinterval((r_Range)0, (r_Range)1);
    dummyMDD = (r_Ref<r_GMarray>)(new r_Marray<r_Char>(dummyInterv));

    meshDesc = new mesh_desc;
    memset(meshDesc, 0, sizeof(mesh_desc));
    mddDesc = new mdd_desc;
    mddDesc->numDims = dimMDD;
    mddDesc->dims = new int[dimMDD];
    mddDesc->widths = new int[dimMDD];
    mddDesc->floatType = ((baseType == rbt_float) || (baseType == rbt_double)) ? 1 : 0;
}


rviewHeightImage::~rviewHeightImage(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewHeightImage", "~rviewHeightImage()" );

    closeViewer();
    dummyMDD.destroy();
    if (meshDesc != NULL)
    {
        RenderHeightFreeMesh(meshDesc);
        delete meshDesc;
    }
    if (mddDesc != NULL)
    {
        delete [] mddDesc->dims;
        delete [] mddDesc->widths;
        delete mddDesc;
    }
}


int rviewHeightImage::openViewer(void)
{
    if (dimMDD < 2)
    {
        rviewErrorbox::reportError(lman->lookup("errorModeDim"), rviewHeightImage::getFrameName(), "openViewer");
        objectInitializedOK = FALSE;
        return -1;
    }
    if (baseType == rbt_rgb)
    {
        rviewErrorbox::reportError(lman->lookup("errorModeBase"), rviewHeightImage::getFrameName(), "openViewer");
        objectInitializedOK = FALSE;
        return -1;
    }

    if (rviewRenderImage::openViewer() == 0)
    {
        openViewerEpilogue(rviewHeightImage::getFrameType());
        return 0;
    }
    return -1;
}


const char *rviewHeightImage::getFrameName(void) const
{
    return "rviewHeightImage";
}

rviewFrameType rviewHeightImage::getFrameType(void) const
{
    return rviewFrameTypeHghtImage;
}

int rviewHeightImage::getViewerType(void) const
{
    return RVIEW_RESDISP_IMGHGHT;
}


bool rviewHeightImage::cspaceRangeHook(bool suggest)
{
    return FALSE;
}


bool rviewHeightImage::modeNeedsCspace(rviewBaseType bt) const
{
    return FALSE;
}


int rviewHeightImage::newProjection(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewHeightImage", "newProjection()" );

    char *data, *lastData = imgData;

    meshDesc->srcData = NULL;
    if ((data = setupGraphEnv()) == NULL) return -1;
    fillBuffer();

    updatePixmap(lastData, data);

    return 0;
}


bool rviewHeightImage::moviePossible(void) const
{
    return (dimMDD >= 3);
}


char *rviewHeightImage::movieNewFrame(void)
{
    char *data;

    if ((data = setupGraphEnv()) != NULL)
        fillBuffer();
    return data;
}


bool rviewHeightImage::doUpdate(int flags)
{
    if ((flags & RVIEW_IFLAG_LIGHT) != 0) return TRUE;
    if ((flags & RVIEW_IFLAG_HEIGHT) != 0) return TRUE;
    return FALSE;
}


void rviewHeightImage::redrawSettingsChanged(void)
{
    if (depthForHeightfield() != pixDepth)
    {
        char *data = NULL, *lastData = imgData;
        data = setupGraphEnv();
        fillBuffer();
        updatePixmap(lastData, data);
    }
    else
        rviewRenderImage::redrawSettingsChanged();
}


void rviewHeightImage::label(void)
{
    setDisplayTitle(lman->lookup("titleImageHeight"));
    rviewRenderImage::label();
}


int rviewHeightImage::process(wxObject &obj, wxEvent &evt)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewHeightImage", "process()" );

    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)playStop)
            playDirection = 0;
        // do not intercept the call (return 1), however, because the renderer
        // might also rotate the object, which'll be handled on rviewRenderImage
        // level.
    }

    return rviewRenderImage::process(obj, evt);
}


void rviewHeightImage::prepareToDie(void)
{
    playDirection = 0;
    rviewRenderImage::prepareToDie();
}


int rviewHeightImage::requestQuit(int level)
{
    playDirection = 0;
    return rviewRenderImage::requestQuit(level);
}


char *rviewHeightImage::initMode(void)
{
    int i;
    char *b;

    RMDBGONCE(3, RMDebug::module_applications, "rviewHeightImage", "initMode()" );

    b = projString;
    b += sprintf(b, "*:*, *:*");
    for (i=2; i<dimMDD; i++)
    {
        b += sprintf(b, ", %ld", interv[i].low());
    }

    project->SetValue(projString);
    setModeDimension(2);

    for (i=0; i<dimMDD; i++)
    {
        mddDesc->dims[i] = interv[i].high() - interv[i].low() + 1;
        mddDesc->widths[i] = mddDesc->dims[i];
    }
    mddDesc->baseSize = baseSize;

    return rviewRenderImage::initMode();
}


int rviewHeightImage::depthForHeightfield(void) const
{
    return (setup.voxColour >= 256) ? 24 : 8;
}


char *rviewHeightImage::setupEnvironment(int w, int h)
{
    int i, needDepth, newPitch, newPad, offset, newVirtPitch;

    RMDBGONCE(3, RMDebug::module_applications, "rviewHeightImage", "setupEnvironment()" );

    needDepth = depthForHeightfield();
    newPad = 32;
    newPitch = (w * (needDepth >> 3) + 3) & ~3;
    newVirtPitch = newPitch;

    if ((doValToCspace) && (needDepth == 8))
    {
        setCspaceProjMode(FALSE);
        rviewCheckInitCspace(rbt_char, &csmap, dummyMDD, TRUE, csInterv, w, &newPitch, &needDepth, &newPad, &newVirtPitch, cspar);
        if (csmap != NULL)
        {
            mBar->Enable(MENU_IMAGE_CSPACE_EDIT, TRUE);
        }
        deleteViewCspace();
    }

    if ((w != pixWidth) || (h != pixHeight) || (pixPad != newPad) || (pixDepth != needDepth) || (newPitch != pixPitch) || (newVirtPitch != virtualPitch))
    {
        pixWidth = w;
        pixHeight = h;
        pixDepth = needDepth;
        pixPitch = newPitch;
        pixPad = newPad;
        virtualPitch = newVirtPitch;
        if ((imgData = (char*)malloc(virtualPitch * pixHeight)) == NULL)
        {
            rviewErrorbox::reportError(lman->lookup("errorMemory"), rviewHeightImage::getFrameName(), "setupEnvironment");
            return NULL;
        }
    }
    graphEnv->lineadd = virtualPitch;

    for (i=0; i<dimMDD; i++)
    {
        mddDesc->widths[i] = pt2[i] - pt1[i] + 1;
    }

    r_Ref<r_Marray<r_Char> > tempMdd = (r_Ref<r_Marray<r_Char> >) mddObj;
    r_Point paux(dimMDD);
    for (i=0; i<dimMDD; i++)
    {
        paux[i] = interv[i].low();
    }

    offset = ((int)(&((*tempMdd)[pt1]) - &((*tempMdd)[paux]))) * baseSize;

    mddDesc->data = (void*)((char*)(tempMdd->get_array()) + offset);

    return imgData;
}


void rviewHeightImage::fillBackgroundCore(rviewBaseType bt, double minVal)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewHeightImage", "fillBackgroundCore()" );

    memset(graphEnv->dest, 0, pixPitch * pixHeight);
}


void rviewHeightImage::fillBuffer(void)
{
    real_t gridScale, scaleHeight;
    light_desc light;
    bool useCspace, cspaceOK;

    RMDBGONCE(3, RMDebug::module_applications, "rviewHeightImage", "fillBuffer()" );

    gridScale = cubeScale * setup.gridSize;
    scaleHeight = cubeScale * setup.scaleHeight;
    graphEnv->zpro = setup.zpro;
    graphEnv->clipz = setup.clipz;
    memcpy(geomUse+1, rot, 3*sizeof(vertex_fp));

    // Calculate the coordinates of the center point of the base diagonal for
    // centering the rotation
    int dimx, dimz;

    if (RenderHeightGetDomain(mddDesc, &dimx, &dimz, NULL, NULL) != 0) return;

    geomUse[0].x = -0.5 * gridScale * (dimx * rot[0].x + dimz * rot[0].z);
    geomUse[0].y =  0.0;
    geomUse[0].z = -0.5 * gridScale * (dimx * rot[2].x + dimz * rot[2].z) + graphEnv->zpro + zoff;

    useCspace = doValToCspace;

    light.ambient = (setup.useLights) ? setup.lightsAmbient : -1.0;
    light.gain = setup.lightsGain;
    light.cosine = cos((M_PI * setup.lightsAngle) / 180);
    light.scintCos = cos((M_PI * setup.lightsScintAngle) / 180);
    getLightPos(&(light.lights));
    meshDesc->scaleGrid = gridScale;
    meshDesc->scaleHeight = scaleHeight;
    meshDesc->colour = (unsigned int)(setup.voxColour);
    if (meshDesc->colour >= 256)
    {
        meshDesc->colour |= 0xff000000; // mark as RGB
        useCspace = FALSE;
    }

    // Colourspace mapping in the height renderer always means a source type of
    // 8bpp. Besides there's no correlation between the rendered colours and the
    // MDD values, therefore the colourspace is always set to [0,255] here.
    double useMinVal = 0;
    fillBufferBackground(useCspace, cspaceOK, dummyMDD, &csmap, csInterv, rbt_char, true, &useMinVal);

    ::wxBeginBusyCursor(wxHOURGLASS_CURSOR);

    RenderHeightField(meshDesc, geomUse, graphEnv, mddDesc, &light);

    if (useCspace && cspaceOK)
    {
        double useMaxVal=255;
        translateBufferToCspace(rbt_char, &useMinVal, &useMaxVal);
    }

    ::wxEndBusyCursor();
}




/*
 *  Prescaled image class
 */

const double rviewScaledImage::scaleStep = 2.0;

const char *rviewScaledImage::view_CurrentBox = "currentBox";
const char *rviewScaledImage::view_BoxScale = "boxScale";

rviewScaledImage::rviewScaledImage(collection_desc *cd, r_Fast_Base_Scale *scaler, unsigned int flags) : rviewFlatBaseImage(cd->mddObjs, 0, flags)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "rviewScaledImage()" );

    int i;

    scaleObject = scaler;
    fullDomain = scaleObject->get_full_domain();

    initialScale = scaler->get_last_scale();
    thisView.scale = initialScale;
    thisView.dim1 = 0;
    thisView.dim2 = 1;
    thisView.low = r_Point(dimMDD);
    thisView.high = r_Point(dimMDD);
    for (i=0; i<dimMDD; i++)
    {
        thisView.low[i] = fullDomain[i].low();
        thisView.high[i] = fullDomain[i].high();
    }

    dontLoad = TRUE;
    loadedView = NULL;

    collDesc = cd;
}


rviewScaledImage::~rviewScaledImage(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "~rviewScaledImage()" );
    closeViewer();
    rviewDeleteCollection(collDesc);
    // I own the scaler object!
    delete scaleObject;
    if (loadedView != NULL)
        delete loadedView;
}


int rviewScaledImage::openViewer(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "openViewer()" );

    if (rviewFlatBaseImage::openViewer() == 0)
    {
        zoomInBut = new rviewButton(ctrlPanel);
        zoomOutBut = new rviewButton(ctrlPanel);
        lastZoomBut = new rviewButton(ctrlPanel);
        zoomBoxBut = new rviewButton(ctrlPanel);
        scaleString = new rviewText(ctrlPanel, thisView.scale);

        lastZoomBut->Enable((viewHistory.getNumber() != 0));
        boxState = pcanv->HasDragBox();
        zoomBoxBut->Enable(boxState);

        mBar->Enable(MENU_DISP_DATA_INSERT, FALSE);
        mBar->Enable(MENU_DISP_DATA_INSERTPRO, FALSE);

        openViewerEpilogue(rviewScaledImage::getFrameType());

        dontLoad = FALSE;

        return 0;
    }
    return -1;
}


void rviewScaledImage::label(void)
{
    if (initPhaseFinished)
    {
        setDisplayTitle(lman->lookup("titleImageScaled"));
        scaleString->SetLabel(lman->lookup("textScaleFactor"));
        zoomBoxBut->SetLabel(lman->lookup("textZoomBox"));
        lastZoomBut->SetLabel(lman->lookup("textLastZoom"));
        zoomInBut->SetLabel(lman->lookup("textZoomIn"));
        zoomOutBut->SetLabel(lman->lookup("textZoomOut"));
    }
    rviewFlatBaseImage::label();
}


void rviewScaledImage::OnSize(int w, int h)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "OnSize()");

    if (initPhaseFinished)
    {
        int x, y, step, posx, posy;

        GetClientSize(&x, &y);

        if(x < 6*image_bwidth)
        {
            x= 6*image_bwidth;
            frameWidth=x;
            frameHeight=y;
            SetClientSize(x, y);
            return;
        }

        x -= 2*display_border;
        scaleString->SetSize(display_border, image_totaly - image_theight, image_twidth, image_theight);
        step = (x - image_twidth - 4*image_bwidth) / 4;
        posx = image_twidth + display_border + step/2;
        posy = image_totaly - image_bheight;
        zoomBoxBut->SetSize(posx, posy, image_bwidth, image_bheight);
        posx += step + image_bwidth;
        lastZoomBut->SetSize(posx, posy, image_bwidth, image_bheight);
        posx += step + image_bwidth;
        zoomInBut->SetSize(posx, posy, image_bwidth, image_bheight);
        posx += step + image_bwidth;
        zoomOutBut->SetSize(posx, posy, image_bwidth, image_bheight);
    }

    rviewFlatBaseImage::OnSize(w, h);

    if (initPhaseFinished)
    {
        int w, h, vw, vh;

        pcanv->GetClientSize(&w, &h);
        pcanv->SetAspectRatio(((double)h) / w);
        w = (int)(w / thisView.scale);
        h = (int)(h / thisView.scale);
        vw = (int)(thisView.high[thisView.dim1] - thisView.low[thisView.dim1]);
        vh = (int)(thisView.high[thisView.dim2] - thisView.low[thisView.dim2]);
        if ((vw < w) || (vh < h))
        {
            if (vw < w)
                thisView.high[thisView.dim1] = (r_Range)(thisView.low[thisView.dim1] + w);
            if (vh < h)
                thisView.high[thisView.dim2] = (r_Range)(thisView.low[thisView.dim2] + h);

            newView();
        }
    }
}


double rviewScaledImage::getLastScale(void) const
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "getLastScale()");
    view_desc_t lastView;

    if (viewHistory.peek(lastView) == 0)
        return lastView.scale;
    else
        return initialScale;
}


void rviewScaledImage::scaleViewBy(double scale)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "scaleViewBy()");

    thisView.high[thisView.dim1] = thisView.low[thisView.dim1]
                                   + (r_Range)((thisView.high[thisView.dim1] - thisView.low[thisView.dim1]) / scale);
    thisView.high[thisView.dim2] = thisView.low[thisView.dim2]
                                   + (r_Range)((thisView.high[thisView.dim2] - thisView.low[thisView.dim2]) / scale);
    thisView.scale *= scale;

    // If we magnify, check whether we can increase the view box
    if (scale > 1.0)
    {
        int w, h;

        pcanv->GetClientSize(&w, &h);
        w = (int)(w / thisView.scale);
        h = (int)(h / thisView.scale);
        // extend the viewbox to the maximum size of the canvas
        if (thisView.high[thisView.dim1] - thisView.low[thisView.dim1] < w)
            thisView.high[thisView.dim1] = (r_Range)(thisView.low[thisView.dim1] + w);
        if (thisView.high[thisView.dim2] - thisView.low[thisView.dim2] < h)
            thisView.high[thisView.dim2] = (r_Range)(thisView.low[thisView.dim2] + h);
        // no clipping necessary here, will be done in newView()
    }
}


int rviewScaledImage::process(wxObject &obj, wxEvent &evt)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "process()");

    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)lastZoomBut)
        {
            if (viewHistory.pop(thisView) == 0)
            {
                newView();
            }
            if (viewHistory.getNumber() == 0)
                lastZoomBut->Enable(FALSE);

            return 1;
        }
        else if (&obj == (wxObject*)zoomInBut)
        {
            viewHistory.push(thisView);
            lastZoomBut->Enable(TRUE);
            scaleViewBy(scaleStep);
            newView();
            return 1;
        }
        else if (&obj == (wxObject*)zoomOutBut)
        {
            viewHistory.push(thisView);
            lastZoomBut->Enable(TRUE);
            scaleViewBy(1/scaleStep);
            newView();
            return 1;
        }
        else if (&obj == (wxObject*)zoomBoxBut)
        {
            if (pcanv->HasDragBox())
            {
                int x0, y0, x1, y1;
                double relScale;

                viewHistory.push(thisView);
                lastZoomBut->Enable(TRUE);
                pcanv->GetDragBox(x0, y0, x1, y1);
                relScale = (thisView.high[thisView.dim1] - thisView.low[thisView.dim1]) / ((double)(pixmap->getWidth()));
                thisView.high[thisView.dim1] = thisView.low[thisView.dim1] + (r_Range)(x1*relScale);
                thisView.low[thisView.dim1] += (r_Range)(x0 * relScale);
                thisView.high[thisView.dim2] = thisView.low[thisView.dim2] + (r_Range)(y1*relScale);
                thisView.low[thisView.dim2] += (r_Range)(y0 * relScale);
                thisView.scale *= ((double)(pixmap->getWidth())) / (x1 - x0);
                newView();
            }
            return 1;
        }
    }
    else if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
    {
        if (&obj == (wxObject*)scaleString)
        {
            double newScale = atof(scaleString->GetValue());
            if (newScale != thisView.scale)
            {
                viewHistory.push(thisView);
                lastZoomBut->Enable(TRUE);
                scaleViewBy(newScale / thisView.scale);
                newView();
            }
            return 1;
        }
    }

    return rviewFlatBaseImage::process(obj, evt);
}


// Intercept the mouse event.
void rviewScaledImage::processMouseEvent(wxMouseEvent &mevt)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "processMouseEvent()");

    int newbut = 0;
    bool newBoxState;

    if (mevt.leftDown) newbut |= MOUSE_LEFT;
    if (mevt.middleDown) newbut |= MOUSE_MIDDLE;
    if (mevt.rightDown) newbut |= MOUSE_RIGHT;

    // Drag a box
    if ((newbut & MOUSE_LEFT) != 0)
    {
        if ((mousebut & MOUSE_LEFT) == 0)
        {
            pcanv->SetDragBox(mevt.x, mevt.y, mevt.x, mevt.y);
        }
        else
        {
            pcanv->UpdateDragBox(mevt.x, mevt.y);
        }
    }
    else if ((newbut & MOUSE_RIGHT) != 0)
    {
        if ((mousebut & MOUSE_RIGHT) == 0)
        {
            pcanv->AdjustDragBox(mevt.x, mevt.y);
        }
        else
        {
            pcanv->UpdateDragBox(mevt.x, mevt.y);
        }
    }
    mousebut = newbut;
    mousex = mevt.x;
    mousey = mevt.y;

    newBoxState = pcanv->HasDragBox();
    if (newBoxState != boxState)
    {
        zoomBoxBut->Enable(newBoxState);
        boxState = newBoxState;
    }
}


const char *rviewScaledImage::getFrameName(void) const
{
    return "rviewPrescaledFrame";
}


rviewFrameType rviewScaledImage::getFrameType(void) const
{
    return rviewFrameTypeScaledImage;
}


int rviewScaledImage::getViewerType(void) const
{
    return RVIEW_RESDISP_IMGSCLD;
}


bool rviewScaledImage::showScaleSlider(void) const
{
    return FALSE;
}


void rviewScaledImage::projectionStringForView(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "projectionStringView()");

    int i;
    char *b = projString;

    for (i=0; i<thisView.dim1; i++)
    {
        b += sprintf(b, "%ld, ", thisView.low[i]);
    }
    b += sprintf(b, "%ld:%ld, ", thisView.low[i], thisView.high[i]);
    i++;
    for (; i<thisView.dim2; i++)
    {
        b += sprintf(b, "%ld, ", thisView.low[i]);
    }
    b += sprintf(b, "%ld:%ld, ", thisView.low[i], thisView.high[i]);
    i++;
    for (; i<dimMDD; i++)
    {
        b += sprintf(b, "%ld, ", thisView.low[i]);
    }
    if (b != projString)
        b[-2] = '\0';
}


char *rviewScaledImage::initMode(void)
{
    return rviewFlatBaseImage::initMode();
}


const r_Minterval &rviewScaledImage::getVirtualDomain(void) const
{

    return fullDomain;
}


void rviewScaledImage::projectObjectHook(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "projectObjectHook()");
    int i, j;

    for (i=0; i<(int)dimMDD; i++)
    {
        thisView.low[i] = pt1[i];
        thisView.high[i] = pt2[i];
    }
    for (i=0, j=0; i<(int)dimMDD; i++)
    {
        if ((freeDims & (1<<i)) != 0)
        {
            if (j == 0)
            {
                thisView.dim1 = i;
                j++;
            }
            else
                thisView.dim2 = i;
        }
    }

    r_Minterval projFull(dimMDD);
    r_Minterval projScaled;

    for (i=0; i<dimMDD; i++)
    {
        // shift point into full domain's origin...
        projFull << r_Sinterval(fullDomain[i].low(), fullDomain[i].low() + (pt2[i] - pt1[i]));
    }
    // now adapt the values to the actual MDD object
    if (scaleObject->get_scaled_domain(projFull, projScaled, thisView.scale) != 0)
    {
        //cout << projFull << ", " << projScaled << ", " << interv << endl;
        // for now just make sure we're not outside the range...
        for (i=0; i<dimMDD; i++)
        {
            r_Range offset = pt1[i] - fullDomain[i].low();
            pt1[i] = projScaled[i].low() + offset;
            if (pt1[i] < interv[i].low()) pt1[i] = interv[i].low();
            if (pt1[i] > interv[i].high()) pt1[i] = interv[i].high();
            pt2[i] = projScaled[i].high() + offset;
            if (pt2[i] < interv[i].low()) pt2[i] = interv[i].low();
            if (pt2[i] > interv[i].high()) pt2[i] = interv[i].high();
        }
        //cout << "Translated " << pt1 << ", " << pt2 << endl;
    }
    else
    {
        rviewErrorbox::reportError(lman->lookup("errorScaledObjSize"));
    }
}


char *rviewScaledImage::projectImage(void)
{
    return rviewFlatBaseImage::projectImage();
}


bool rviewScaledImage::compareViews(const view_desc_t &v1, const view_desc_t &v2)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "compareViews()");
    if ((v1.scale == v2.scale) && (v1.dim1 == v2.dim1) && (v1.dim2 == v2.dim2)
            && (v1.low == v2.low) && (v1.high == v2.high))
        return TRUE;

    return FALSE;
}


int rviewScaledImage::newProjection(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "newProjection()");
    int status;
    view_desc_t lastView = thisView;

    if ((status = rviewFlatBaseImage::newProjection()) == 0)
    {
        if (!compareViews(thisView, lastView))
        {
            viewHistory.push(lastView);
            lastZoomBut->Enable(TRUE);
        }
    }
    return status;
}


void rviewScaledImage::newView(bool loadImage)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewScaledImage", "newView()");
    int i;

    // Remove drag boxes
    pcanv->SetDragBox(-1, -1, -1, -1);
    boxState = FALSE;
    zoomBoxBut->Enable(boxState);

    // Clip rectangle to actual domain. Ratio already ensured by canvas
    for (i=0; i<(int)dimMDD; i++)
    {
        if (thisView.low[i] < fullDomain[i].low())
            thisView.low[i] = fullDomain[i].low();
        if (thisView.high[i] > fullDomain[i].high())
            thisView.high[i] = fullDomain[i].high();
    }

    if (!dontLoad)    // or loadImage?
    {
        r_Minterval orgIv(dimMDD);
        r_Minterval scaledDom;
        int i;

        for (i=0; i<(int)dimMDD; i++)
        {
            orgIv << r_Sinterval(thisView.low[i], thisView.high[i]);
        }

        collDesc->mddObjs[0].mdd.destroy();

        ::wxBeginBusyCursor(wxHOURGLASS_CURSOR);

        collDesc->mddObjs[0].mdd = rviewDatabase::getScaledObject(scaleObject, orgIv, thisView.scale);

        ::wxEndBusyCursor();

        if (collDesc->mddObjs[0].mdd.is_null())
        {
            Close(TRUE);
            return;
        }
        mddObj = collDesc->mddObjs[0].mdd;
        interv = mddObj->spatial_domain();
    }

    projectionStringForView();
    project->SetValue(projString);
    scaleString->SetValue(thisView.scale);
    scaleValue = 100;

    rviewFlatBaseImage::newProjection();
}


int rviewScaledImage::saveView(FILE *fp)
{
    int status = rviewFlatBaseImage::saveView(fp);

    long *boxdesc = new long[2*dimMDD + 2];

    boxdesc[0] = (long)(thisView.dim1);
    boxdesc[1] = (long)(thisView.dim2);
    for (unsigned int i=0; i<dimMDD; i++)
    {
        boxdesc[2+i] = (long)(thisView.low[i]);
        boxdesc[2+i+dimMDD] = (long)(thisView.high[i]);
    }
    writeViewParam(fp, view_CurrentBox, 2*dimMDD + 2, boxdesc);
    writeViewParam(fp, view_BoxScale, thisView.scale);

    delete [] boxdesc;

    return status;
}


int rviewScaledImage::readView(const char *key, const char *value)
{
    int status = rviewFlatBaseImage::readView(key, value);

    if (status == 0)
    {
        if (strcmp(key, view_CurrentBox) == 0)
        {
            long *boxdesc = new long[2*dimMDD + 2];

            if (readVector(value, 2*dimMDD + 2, boxdesc) == 0)
            {
                ensureLoadedView();

                loadedView->dim1 = (int)(boxdesc[0]);
                loadedView->dim2 = (int)(boxdesc[1]);
                // why do I always forget these !*&@^$()&^ dim constructors...???
                loadedView->low = r_Point(dimMDD);
                loadedView->high = r_Point(dimMDD);
                for (unsigned int i=0; i<dimMDD; i++)
                {
                    loadedView->low[i] = (r_Range)(boxdesc[2+i]);
                    loadedView->high[i] = (r_Range)(boxdesc[2+i+dimMDD]);
                }
            }
            delete [] boxdesc;
            return 1;
        }
        else if (strcmp(key, view_BoxScale) == 0)
        {
            ensureLoadedView();
            loadedView->scale = atof(value);
            return 1;
        }
        return 0;
    }
    return status;
}


void rviewScaledImage::loadViewFinished(void)
{
    rviewFlatBaseImage::loadViewFinished();

    viewHistory.push(thisView);
    lastZoomBut->Enable(TRUE);

    thisView.scale = loadedView->scale;
    thisView.low = loadedView->low;
    thisView.high = loadedView->high;
    thisView.dim1 = loadedView->dim1;
    thisView.dim2 = loadedView->dim2;

    delete loadedView;
    loadedView = NULL;

    newView(TRUE);
}


void rviewScaledImage::ensureLoadedView(void)
{
    if (loadedView == NULL)
    {
        loadedView = new view_desc_t;
        memset(loadedView, 0, sizeof(view_desc_t));
    }
}







/*
 *  Functions performing the actual translation of MDD into a scaled
 *  image. Code shared between rviewFlatImage and rviewThumb.
 */

// Number of fractional fixpoint bits
#define IMAGE_FIXPREC   16

// A macro for recurring code in projectImage
#define PROJECT_HEADER(type) \
  if (penv.pt1[penv.dim1] == penv.pt2[penv.dim1]) stepx=0; \
  else \
  { \
    prun[penv.dim1]=penv.pt1[penv.dim1]+1; \
    prun[penv.dim2]=penv.pt1[penv.dim2]; \
    stepx = &((*mddPtr)[prun]) - &((*mddPtr)[penv.pt1]); \
  } \
  if (penv.pt1[penv.dim2] == penv.pt2[penv.dim2]) stepy=0; \
  else \
  { \
    prun[penv.dim1]=penv.pt1[penv.dim1]; \
    prun[penv.dim2]=penv.pt1[penv.dim2]+1; \
    stepy = &((*mddPtr)[prun]) - &((*mddPtr)[penv.pt1]); \
  } \
  scalestepx = stepx * (int)(1 / penv.scale); \
  scalestepy = stepy * (int)(1 / penv.scale); \
  fracstepx = ((int)((1<<IMAGE_FIXPREC) / penv.scale)) & ((1<<IMAGE_FIXPREC)-1); \
  fracstepy = ((int)((1<<IMAGE_FIXPREC) / penv.scale)) & ((1<<IMAGE_FIXPREC)-1); \
  imgLine = (type*)(&((*mddPtr)[penv.pt1]));

#define PROJECT_IMAGE_INCREMENTX \
  imgPtr += scalestepx; fracx += fracstepx; \
  if (fracx >= (1<<IMAGE_FIXPREC)) \
  { \
    imgPtr += stepx; fracx &= ((1<<IMAGE_FIXPREC)-1); \
  }

#define PROJECT_IMAGE_INCREMENTY \
  imgLine += scalestepy; fracy += fracstepy; \
  if (fracy >= (1<<IMAGE_FIXPREC)) \
  { \
    imgLine += stepy; fracy &= ((1<<IMAGE_FIXPREC)-1); \
  }

// A macro for colourspace translators
// Do not terminate this macro with a semicolon when using it!
#define PROJECT_IMAGE_START(type) \
  r_Marray<type> *mddPtr = (r_Marray<type>*) (penv.mddPtr); \
  type *imgLine, *imgPtr; \
  PROJECT_HEADER(type); \
  for (h=0; h<penv.height; h++, destLine.c+=penv.pitch) \
  { \
    for (w=0, imgPtr=imgLine, destPtr.c=destLine.c, fracx=0; w<penv.width; w++)

#define PROJECT_IMAGE_BASE(type) \
  PROJECT_IMAGE_START(type) \
    { \
      PROJECT_IMAGE_TRANSFER_PIXEL(type); \
      PROJECT_IMAGE_INCREMENTX; \
    } \
    PROJECT_IMAGE_INCREMENTY; \
  }

#define PROJECT_COLOURSPACE32(type, minval, maxval, scale) \
  if ((IntToRGBTab24 = penv.csmap->getCSTab24()) == NULL) \
  { \
    PROJECT_IMAGE_START(type) \
    { \
        *destPtr.l++ = penv.csmap->ValToCS24((double)(*imgPtr) - minVal); \
        PROJECT_IMAGE_INCREMENTX; \
      } \
      PROJECT_IMAGE_INCREMENTY; \
    } \
  } \
  else \
  { \
    PROJECT_IMAGE_START(type) \
    { \
        type value; \
        value = *imgPtr; \
        if (value > maxval) value = maxval; value -= minval; if (value < 0) value = 0;\
        *destPtr.l++ = IntToRGBTab24[(unsigned long)(value * scale)]; \
        PROJECT_IMAGE_INCREMENTX; \
      } \
      PROJECT_IMAGE_INCREMENTY; \
    } \
  }


int rviewPrepareFlatProjection(rviewFlatProjEnv &penv)
{
    int baseSize = penv.mddPtr->get_type_length();

    penv.width = (int)((penv.pt2[penv.dim1] - penv.pt1[penv.dim1] + 1) * penv.scale);
    penv.height = (int)((penv.pt2[penv.dim2] - penv.pt1[penv.dim2] + 1) * penv.scale);

    switch (rviewImageTypes[penv.bt])
    {
    case RVIEW_IMGTYPE_NONE:
    {
        if (penv.doCspace)
        {
            penv.pitch = penv.width * 4;
            penv.pad = 32;
            penv.depth = 32;
        }
        else
        {
            rviewErrorbox::reportError(lman->lookup("errorUnknownBase"), "rviewPrepareFlatProjection");
            return -1;
        }
    }
    break;
    case RVIEW_IMGTYPE_MONO:
        penv.pitch = (penv.width + 7) >> 3;
        penv.pad = 8;
        penv.depth = 1;
        break;
    case RVIEW_IMGTYPE_HIGH:
        penv.pitch = (penv.width * 2 + 3) & ~3;
        penv.pad = 32;
        penv.depth = 15;
        break;
    case RVIEW_IMGTYPE_GREY12:
        // unsigned short = 12bpp grey, gets transformed to 8bpp grey
        penv.pitch = (penv.width + 3) & ~3;
        penv.pad = 32;
        penv.depth = 8;
        break;
    default:
        penv.pitch = (penv.width * baseSize + 3) & ~3;
        penv.pad = 32;
        penv.depth = 8*baseSize;
        break;
    }

#ifdef wx_msw
    if (penv.bt == rbt_rgb)
    {
        penv.pitch = (penv.width * baseSize);
        penv.pad = 8;
    }
#endif

    return 0;
}


int rviewPerformFlatProjection(rviewFlatProjEnv &penv, char *data)
{
    int stepx, stepy, scalestepx, scalestepy;
    int fracstepx, fracstepy, fracx, fracy;
    union
    {
        char *c;
        short *s;
        RGBPixel *r;
        long *l;
    } destPtr, destLine;
    r_Point prun;
    int w, h;

    destLine.c = data;
    prun = penv.pt1;
    fracy = 0;

    // Do a colourspace mapping?
    if (penv.cspaceState != 0)
    {
        double minVal, maxVal;
        long minValL, maxValL;
        double scalingFactor;
        unsigned short *IntToRGBTab15;
        unsigned long *IntToRGBTab24;

        minVal = penv.csmap->getMinVal();
        minValL = (long)minVal;
        maxVal = penv.csmap->getMaxVal();
        maxValL = (long)maxVal;
        scalingFactor = penv.csmap->getScalingFactor();

        IntToRGBTab15 = penv.csmap->getCSTab15();

        switch (penv.bt)
        {
#define PROJECT_IMAGE_TRANSFER_PIXEL(type)  long value = (long)(*imgPtr); if (value > maxValL) value = maxValL; value -= minValL; if (value < 0) value = 0; *destPtr.s++ = IntToRGBTab15[value]
        case rbt_char:
        {
            PROJECT_IMAGE_BASE(r_Char);
        }
        break;
        case rbt_uchar:
        {
            PROJECT_IMAGE_BASE(r_Octet);
        }
        break;
        case rbt_short:
        {
            PROJECT_IMAGE_BASE(r_Short);
        }
        break;
        case rbt_ushort:
        {
            PROJECT_IMAGE_BASE(r_UShort);
        }
        break;
        case rbt_long:
        {
            PROJECT_COLOURSPACE32(r_Long, minValL, maxValL, 1);
        }
        break;
        case rbt_ulong:
        {
            PROJECT_COLOURSPACE32(r_ULong, (r_ULong)minValL, (r_ULong)maxValL, 1);
        }
        break;
        case rbt_float:
        {
            PROJECT_COLOURSPACE32(r_Float, minVal, maxVal, scalingFactor);
        }
        break;
        case rbt_double:
        {
            PROJECT_COLOURSPACE32(r_Double, minVal, maxVal, scalingFactor);
        }
        break;
        default:
        {
            rviewErrorbox::reportError(lman->lookup("errorBaseType"), "rviewPerformFlatProjectionCmap");
            return -1;
        }
        break;
        }
    }
    // No, use actual source data with no transformations.
    else
    {
        switch (rviewImageTypes[penv.bt])
        {
        case RVIEW_IMGTYPE_MONO:
        {
            r_Marray<r_Boolean> *mddPtr = (r_Marray<r_Boolean> *)(penv.mddPtr);
            r_Boolean *imgLine, *imgPtr;
            char val;
            int mask;

            PROJECT_HEADER(r_Boolean);
            for (h=0; h<penv.height; h++, destLine.c+=penv.pitch)
            {
#if (WX_PIXMAP_SRC_BITORDER == 0)
                mask = 1;
#else
                mask = 0x80;
#endif
                val = 0;
                for (w=0, imgPtr=imgLine, destPtr.c=destLine.c, fracx=0; w<penv.width; w++)
                {
                    if (*imgPtr) val |= mask;
#if (WX_PIXMAP_SRC_BITORDER == 0)
                    mask <<= 1;
                    if (mask == 0x100)
                    {
                        *destPtr.c++ = val;
                        val = 0;
                        mask = 1;
                    }
#else
                    mask >>= 1;
                    if (mask == 0)
                    {
                        *destPtr.c++ = val;
                        val = 0;
                        mask = 0x80;
                    }
#endif
                    PROJECT_IMAGE_INCREMENTX;
                }
#if (WX_PIXMAP_SRC_BITORDER == 0)
                if (mask != 1)
#else
                if (mask != 0x80)
#endif
                    *destPtr.c++ = val;
                PROJECT_IMAGE_INCREMENTY;
            }
        }
        break;

#undef PROJECT_IMAGE_TRANSFER_PIXEL
#define PROJECT_IMAGE_TRANSFER_PIXEL(type)  *destPtr.c++ = *imgPtr
        case RVIEW_IMGTYPE_GREY:
        {
            PROJECT_IMAGE_BASE(r_Char)
        }
        break;

#undef PROJECT_IMAGE_TRANSFER_PIXEL
#define PROJECT_IMAGE_TRANSFER_PIXEL(type)  *destPtr.s++ = *imgPtr
        case RVIEW_IMGTYPE_HIGH:
        {
            PROJECT_IMAGE_BASE(r_Short)
        }
        break;

#undef PROJECT_IMAGE_TRANSFER_PIXEL
#define PROJECT_IMAGE_TRANSFER_PIXEL(type)  *destPtr.c++ = (char)((*imgPtr) >> 4)
        case RVIEW_IMGTYPE_GREY12:
        {
            PROJECT_IMAGE_BASE(r_UShort)
        }
        break;

#undef PROJECT_IMAGE_TRANSFER_PIXEL
#define PROJECT_IMAGE_TRANSFER_PIXEL(type)  *destPtr.l++ = *imgPtr
        case RVIEW_IMGTYPE_TRUE32:
        {
            PROJECT_IMAGE_BASE(r_Long)
        }
        break;

#undef PROJECT_IMAGE_TRANSFER_PIXEL
#define PROJECT_IMAGE_TRANSFER_PIXEL(type)  *destPtr.r++ = *imgPtr
        case RVIEW_IMGTYPE_TRUE24:
        {
            PROJECT_IMAGE_BASE(RGBPixel)
        }
        break;

        default:
        {
            rviewErrorbox::reportError(lman->lookup("errorBaseType"), "rviewPerformFlatProjection");
            return -1;
        }
        break;
        }
    }

    return 0;
}
