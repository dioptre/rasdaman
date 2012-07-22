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
 *  Base class for all object viewers (rviewImage, rviewChart, rviewTable
 *  and rviewSound). Provides a frame with standard control widgets and
 *  menus, functions for parsing/advancing the projection string and
 *  initializing protected variables according to the MDD object that will
 *  be displayed.
 *
 *  COMMENTS:
 *      None
 */

/**
*   @file rviewDisplay.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_DISPLAY_H_
#define _RVIEW_DISPLAY_H_

#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/gmarray.hh"

#include "labelManager.hh"
#include "rviewUtils.hh"

/*
 *  A window containing an display mode + controls
 */
class rviewDisplay: public rviewFrame
{
public:

    rviewDisplay(mdd_frame *mf, int es, unsigned int flags=0);
    virtual ~rviewDisplay(void);

    // must use separate call because no virtual functions beyond level i can
    // be called in a constructor of level i. This became necessary when the
    // rviewImage class was split up. Returns 0 for OK, -1 for error.
    virtual int openViewer(void);
    // Same reason for virtual calls (getViewerType()) in the destructor; this
    // functions is called from a viewer's destructor to notify its parent of the
    // close event.
    void closeViewer(void);

    virtual void label();
    virtual int process(wxObject &obj, wxEvent &evt);

    virtual void OnSize(int w, int h);
    virtual void OnMenuCommand(int id);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const = 0;

    // Needs to process user events
    virtual int userEvent(const user_event &ue);
    // Notify derived classes that they should get ready to die. This is needed
    // for stuff like images with movie-playback running.
    virtual void prepareToDie(void);
    // Notify derived classes that the projection has changed
    virtual int newProjection(void);
    // get the minterval to use with the projection string
    virtual const r_Minterval &getVirtualDomain(void) const;

    // Called when an update object has been replaced with a new one
    void noLongerUpdate(void);
    // Returns the display ID (used in for update displays)
    int getIdentifier(void) const;
    int getDisplayCounter(void) const;
    // sets the ID of the query window in case it's an update object
    void setQueryWindow(int qwindowID);

    // viewer-related constants
    // Default width and height of display window
    static const int display_width;
    static const int display_height;
    // Space reserved around the canvas
    static const int display_cnvborder;
    // Borders used in display window
    static const int display_border;
    // Scrollbar steps
    static const int display_scrstep;
    // Page stepping
    static const int display_pgstep;
    // Height of control area
    static const int display_cheight;
    // Height of projection widget
    static const int display_pjheight;
    // Maximum width of projection widget
    static const int display_pjwidth;
    // Projection OK-button dimensions
    static const int display_pbwidth;
    static const int display_pbheight;
    // Minimum display window size (to avoid X errors)
    static const int display_minwidth;

    // Display flags
    static const int display_flag_standalone;
    static const int display_flag_update;

    // modes for advanceProjection()
    enum display_advmode_e
    {
        display_advmode_relative,
        display_advmode_absolute,
        display_advmode_reset
    };


protected:

    // to allow derived functions to add stuff to the file menu before the quit item
    virtual int fileMenuInitHook(wxMenu *menu);
    // to allow derived functions to add stuff to the view menu
    virtual int viewMenuInitHook(wxMenu *menu);
    // for appending menus to the menu bar
    virtual int menuBarInitHook(void);

    // Called by the derived classes to (un)grey certain widgets
    void setModeDimension(int dim);
    // Called by derived classes to set minimum size of viewer window
    void setMinimumViewerSize(int w, int h);
    // Advances projection (+/- buttons)
    const char *skipIndexMapping(const char *s);
    int advanceProjection(int direction, int advmode=display_advmode_relative);
    void newDBState(bool dbstate);
    void setDisplayTitle(const char *title=NULL);
    // view management
    int doSaveView(void);
    int doLoadView(void);
    int parseViewFile(FILE *fp);
    static void writeViewKey(FILE *fp, const char *key);
    static void writeViewParam(FILE *fp, const char *key, const char *value);
    static void writeViewParam(FILE *fp, const char *key, long value);
    static void writeViewParam(FILE *fp, const char *key, double value);
    static void writeViewParam(FILE *fp, const char *key, unsigned int num, const long *values);
    static void writeViewParam(FILE *fp, const char *key, unsigned int num, const double *values);
    static int readVector(const char *value, unsigned int num, long *values);
    static int readVector(const char *value, unsigned int num, double *values);

    // save the current view to a file descriptor
    virtual int saveView(FILE *fp);
    // read a parameter from a view file line; returns 0 if unknown key, 1 otherwise
    virtual int readView(const char *key, const char *value);
    // loading a new view is finished, allow updating of displays before the redraw
    virtual void loadViewFinished(void);

    unsigned int displayFlags;
    r_Ref<r_GMarray> mddObj;
    r_Minterval interv;
    r_Point pt1, pt2, mapIndex;
    int dimMDD, dimMode, baseSize;
    rviewBaseType baseType;
    wxMenuBar *mBar;
    wxPanel *ctrlPanel;
    rviewText *project, *advance;
    wxMessage *typeMsg;
    rviewButton *projBut, *projPlus, *projMinus;
    char projString[STRINGSIZE];
    char rootTitle[STRINGSIZE];
    int extraSpace;
    int totalCtrlHeight;
    int displayID;
    int qwindowID;
    int minViewX, minViewY, mbarHeight;
    bool displayOperation;
    bool objectInitializedOK;
    // number of menus that are always on the menu bar
    static const int fixedNumberOfMenus;


private:

    // This variable avoids closeViewer() being called more than once
    // in case there are viewer classes derived from a common, non-
    // abstract base class.
    bool closeViewerCalled;

    // This global variable is incremented every time a display window is
    // opened. It is used to init the displayID member to identify display
    // windows by a number.
    static int displayCounter;

    // file extension of view file
    static const char *viewFileExtension;

    // size of buffer for view loading
    static const unsigned int viewBuffSize;

    // view parameter keywords
    static const char *view_HeaderLine;
    static const char *view_ViewerType;
    static const char *view_ProjString;
    static const char *view_WindowSize;
};

#endif
