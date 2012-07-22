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
 *  Management of rView's preferences. This includes an object encapsulating
 *  the preferences and providing IO of these preferences (rviewPrefs) and a
 *  frame class that allows displaying / editing the current preferences
 *  (rviewPrefsWindow).
 *
 * COMMENTS:
 *      none
 */

/**
*   @file rviewPrefs.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_PREFS_H_
#define _RVIEW_PREFS_H_

// #include "wx_scrol.h"
#include "wx/xrc/xh_scrol.h"

#include "raslib/mddtypes.hh"

#include "rviewUtils.hh"
#include "rviewDModes.hh"



// Needed by rviewPrefsWindow. Full definition below.
class rviewPrefs;




/*
 *  The window for editing the preferences
 */
class rviewPrefsWindow: public rviewFrame
{
public:

    rviewPrefsWindow(void);
    rviewPrefsWindow(rviewPrefs *Prefs);
    ~rviewPrefsWindow(void);

    void setPrefs(rviewPrefs *Prefs);

    void unlinkParent(void);

    void OnSize(int w, int h);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);
    int userEvent(const user_event &ue);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;


protected:

    void setupVariables(void);
    void updatePrefs(void);
    void updateAndDie(void);
    int  findInChoices(int value, const char **choices, int number);

    static rviewChoice *buildFormatMenu(wxPanel *parent, int fmtNum, const char *label);
    static void textWindowToString(DynamicString &str, wxTextWindow *twin);

    rviewPrefs *editPrefs;
    rviewPrefs *myParent;

    wxPanel *panel, *butPanel;
    rviewButton *butOK, *butCancel, *butApply;
    // wxGroupBox *miscGroup, *imgGroup, *renderGroup, *thumbGroup, *heightGroup;
    // wxGroupBox *chartGroup, *tableGroup, *soundGroup, *commGroup, *orthoGroup;
    wxWindowBase *miscGroup, *imgGroup, *renderGroup, *thumbGroup, *heightGroup;
    wxWindowBase *chartGroup, *tableGroup, *soundGroup, *commGroup, *orthoGroup;
    rviewText *filePath, *queryPath, *queryFont;
    rviewText *vffParams;
    rviewText *maxDWidth, *maxDHeight;
    rviewCheckBox *imgDither, *ditherBest;
    rviewChoice *imgMode, *chartMode, *tableMode, *rgbSpace, *movieMode;
    rviewCheckBox *imgBBox;
    rviewText *imgZpro, *imgClipz, *imgPixThreshLow, *imgPixThreshHigh, *imgWgtThresh, *imgWgtQuant;
    rviewText *imgScale;
    rviewCheckBox *imgRgbBrightness, *imgVoxForType;
    rviewCheckBox *imgLight;
    rviewText *imgLightAmbient, *imgLightGain, *imgLightAngle, *imgLightScintAngle;
    rviewChoice *imgKernSize, *imgKernType;
    rviewCheckBox *imgUseVCol;
    rviewText *imgVoxColour;
    rviewText *imgLightDir, *imgLightDist;
    rviewCheckBox *imgOrthoBBox, *imgOrthoDragRel;
    rviewText *imgOrthoThick;
    rviewCheckBox *chartCosys;
    rviewText *chartStep, *chartMarkx, *chartMarky;
    rviewCheckBox *tableCosys;
    rviewText *tableStepx, *tableStepy;
    rviewText *thumbProjdim, *thumbProjstep, *thumbWidth, *thumbCols;
    rviewChoice *soundFreq, *soundLatency;
    rviewCheckBox *soundLoop;
    rviewText *imgHeightGrid, *imgHeightScale;
    rviewChoice *transferFmt, *storageFmt;
    wxTextWindow *transferParm, *storageParm;
    wxMessage *transferMsg, *storageMsg;
    wxScrollBar *scroll;
    rviewButton *cstrap;
    colourspaceMapper *csmap;

    // constants
    static const char *soundLatencyChoices[];
    static const char *soundFrequencyChoices[];

    // Width and height of preferences window
    static const int prefs_width;
    static const int prefs_height;
    // Borders used in prefs window
    static const int prefs_border;
    // Space at the bottom for button bar
    static const int prefs_bottom;
    // Button dimensions
    static const int prefs_bwidth;
    static const int prefs_bheight;
    // Height of a text widget
    static const int prefs_theight;
    // Height of a checkbox
    static const int prefs_chkheight;
    // Height of a text widget's writable field
    static const int prefs_eheight;
    // ScrollBar width
    static const int prefs_scrwidth;
    // TextWindow height
    static const int prefs_twheight;
    // Message height
    static const int prefs_mheight;
    // Group boxes
    static const int prefs_grpmsc_height;
    static const int prefs_grpimg_height;
    static const int prefs_grpren_height;
    static const int prefs_grphgt_height;
    static const int prefs_grport_height;
    static const int prefs_grpthb_height;
    static const int prefs_grpcht_height;
    static const int prefs_grptab_height;
    static const int prefs_grpsnd_height;
    static const int prefs_grpcom_height;
    // Height of panel
    static const int prefs_pheight;
};


/*
 *  Object holding the preferences and the current setup.
 */
class rviewPrefs
{
public:

    rviewPrefs(void);
    rviewPrefs(const char *file);
    rviewPrefs(const rviewPrefs &srcPrefs);
    ~rviewPrefs(void);

    int load(const char *file);
    int save(const char *file);
    int edit(void);
    void editorClosed(void);
    void closeEditor(rviewPrefs *newPrefs);
    void updatePrefs(rviewPrefs *newPrefs);
    void markModified(void);

    static void copyPrefs(const rviewPrefs &src, rviewPrefs &dest);

    r_Data_Format getTransferFormat( void ) const;
    r_Data_Format getStorageFormat( void ) const;

    DynamicString serverName;
    int serverPort;
    DynamicString databaseName;
    DynamicString userName;
    DynamicString lastColl;
    DynamicString lastScColl;
    DynamicString lastOrthoColl;
    DynamicString filePath;
    DynamicString queryPath;
    DynamicString queryFont;
    DynamicString vffParams;
    int  lastDisplay;
    int maxDWidth, maxDHeight;
    bool imgDither, ditherBest;
    rviewImageMode imgMode;
    rviewChartMode chartMode;
    int movieMode, rgbSpace;
    int tableMode;
    bool imgBBox;
    unsigned long imgZpro, imgClipz, imgWgtQuant;
    double imgPixThreshLow, imgPixThreshHigh, imgWgtThresh;
    double imgScale;
    bool imgRgbBrightness, imgVoxForType;
    bool imgLight;
    double imgLightAmbient, imgLightGain, imgLightAngle, imgLightScintAngle;
    int imgKernSize, imgKernType;
    bool imgUseVCol;
    double imgVoxColour;
    DynamicString imgLightDir;
    int imgLightDist;
    int imgHeightGrid;
    double imgHeightScale;
    bool imgOrthoBBox, imgOrthoDragRel;
    int imgOrthoThick;
    bool chartCosys;
    int chartStep, chartMarkx;
    double chartMarky;
    bool tableCosys;
    int tableStepx, tableStepy;
    int thumbProjdim, thumbProjstep, thumbWidth, thumbCols;
    int soundFreq, soundLatency, soundLoop;
    int transferFmt, storageFmt;
    DynamicString transferParm, storageParm;
    colourspace_params csp;


protected:

    void setupVariables(void);
    // used for getting the value of an argument when reading the prefs file.
    char *getValue(char *b);
    // convert long strings (including newlines) from/to external representation
    static char *toExternal(const DynamicString &str);
    static void fromExternal(const char *ext, DynamicString &str);
    // read a line into the internal buffer; return a pointer to the start if successful
    char *readLine(FILE *fp);

    rviewPrefsWindow *pwin;
    bool prefsModified;

    char *inbuff;
    unsigned long buffSize;

    static const unsigned long buffExtendGranularity;
    static const keyword_to_ident_c prefsVarDesc[];
};



/*
 *  Global variables
 */

extern rviewPrefs *prefs;

#endif
