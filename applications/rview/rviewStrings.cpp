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
 *  rView string viewer. Can display 1D MDD objects as strings
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
#include <ctype.h>


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


const int rviewStringViewer::strview_msgheight = 30;
const int rviewStringViewer::strview_minwidth = 100;
const int rviewStringViewer::strview_minheight = 64;
const int rviewStringViewer::strview_ctrly = 32;
const int rviewStringViewer::strview_totaly = rviewStringViewer::strview_ctrly + rviewDisplay::display_cheight;



rviewStringViewer::rviewStringViewer(mdd_frame *mf, unsigned int flags) :
    rviewDisplay(mf, strview_ctrly, flags)
{
    RMDBGONCE( 3, RMDebug::module_applications, "rviewStringViewer", "rviewStringViewer()" );

    msgString = new wxMessage(ctrlPanel, "");

    // init projection string -- this viewer works for 1D only!
    strcpy(projString, "*:*");
    project->SetValue(projString);

    setModeDimension(1);

    setMinimumViewerSize(strview_minwidth, strview_minheight);
}


int rviewStringViewer::openViewer(void)
{
    RMDBGONCE( 3, RMDebug::module_applications, "rviewStringViewer", "openViewer()" );

    if (dimMDD == 1)
    {
        if (baseSize == 1)
        {
            if (rviewDisplay::openViewer() == 0)
            {
                int w, h;

                newProjection();

                label();
                frameWidth = -1;
                frameHeight = -1;

                GetClientSize(&w, &h);

                SetSize(w, strview_totaly);
                OnSize(w, h);

                Show(TRUE);

                return 0;
            }
        }
        else
        {
            rviewErrorbox::reportError(lman->lookup("errorBaseType"), getFrameName(), "openViewer()");
        }
    }
    else
    {
        rviewErrorbox::reportError(lman->lookup("errorModeDim"), getFrameName(), "openViewer()");
    }
    return -1;
}


const char *rviewStringViewer::getFrameName(void) const
{
    return "rviewStringViewer";
}

rviewFrameType rviewStringViewer::getFrameType(void) const
{
    return rviewFrameTypeStringViewer;
}

int rviewStringViewer::getViewerType(void) const
{
    return RVIEW_RESDISP_STRVIEW;
}


rviewStringViewer::~rviewStringViewer(void)
{
    RMDBGONCE( 3, RMDebug::module_applications, "rviewStringViewer", "~rviewStringViewer()" );
    closeViewer();
}


void rviewStringViewer::OnSize(int w, int h)
{
    int x, y;

    GetClientSize(&x, &y);

    msgString->SetSize(display_border, display_border + display_cheight, x - 2*display_border, strview_msgheight);

    rviewDisplay::OnSize(w, h);
}


int rviewStringViewer::newProjection(void)
{
    unsigned int len, i;
    const char *b;
    char *newMsg;

    mapIndex = r_Point(dimMDD);
    if (rviewParseProjection(getVirtualDomain(), pt1, pt2, projString, &freeDims, &mapIndex) != dimMDD)
    {
        rviewErrorbox::reportError(lman->lookup("errorProjection"), getFrameName(), "newProjection");
        return -1;
    }
    if ((freeDims & 1) == 0)
    {
        rviewErrorbox::reportError(lman->lookup("errorProjectFree"), getFrameName(), "newProjection");
        return -1;
    }
    len = pt2[0] - pt1[0] + 1;
    newMsg = new char[len + 1];
    b = mddObj->get_array() + pt1[0];
    // make sure the message is printable
    for (i=0; i<len; i++)
    {
        if (isprint(b[i]))
            newMsg[i] = b[i];
        else
            newMsg[i] = ' ';
    }
    newMsg[i] = '\0';
    //cout << "MSG: " << newMsg << endl;
    msgString->SetLabel(newMsg);
    delete [] newMsg;

    return 0;
}
