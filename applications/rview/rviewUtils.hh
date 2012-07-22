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
*  Central definitions of shared objects and constants, global tool functions.
*
*  - All menu codes used in the program (MENU_...)
*  - Global tool functions for handling collections, base types, projection
*    strings, X events, ... . See ``Global functions''.
*  - rviewFrame class which must be the base class of all frames used in rView.
*  - rviewFrameMgr class for handling all of rView's frames (dispatching events
*    etc)
*  - rviewMultiline class for displaying several lines of non-editable text.
*  - rviewDialog class which is the base class for all rView's dialog windows.
*  - rviewErrorbox class, derived from rviewDialog.
*  - rviewProgress class, derived from rviewDialog.
*  - rviewResults class for displaying database results and dispatching object
*    viewers. Relies on code provided by rviewMDD for scaling/resampling/endian
*    conversions.
*  - rviewAbout class for displaying information about rView itself.
*  - rviewStringSet class for displaying a set of strings in a scrollable
*    list box.
*
*  COMMENTS:
*        none
*/

/**
*   @file rviewUtils.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_UTILS_H_
#define _RVIEW_UTILS_H_


#include <sstream>

#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/database.hh"

#include "labelManager.hh"

// #include "wx_scrol.h"
#include "wx/xrc/xh_scrol.h"
#include "wx/msgout.h"

// #define wxUSE_GLCANVAS 1 // need this? -- PB 2006-jan-01
#include <wx/glcanvas.h>


#ifdef __VISUALC__
#define DIR_SEPARATOR   "\\"
#define DIR_SEPARATORC  '\\'
#else
#define DIR_SEPARATOR   "/"
#define DIR_SEPARATORC  '/'
#endif

extern const int rview_window_extra_height;
extern const int rview_choice_sub_width;



#define STRINGSIZE  256



// base type to name translations

// Identifiers for base types
enum rviewBaseType
{
    rbt_none,
    rbt_bool,
    rbt_char,
    rbt_uchar,
    rbt_short,
    rbt_ushort,
    rbt_long,
    rbt_ulong,
    rbt_rgb,
    rbt_float,
    rbt_double
};

// Maximum dimension of data rview can deal with
#define MAXIMUM_DIMENSIONS  4

// String names for objects / sets over the various basetypes.
extern char *rviewBaseTypes[];
extern char *rviewTypeNames[][MAXIMUM_DIMENSIONS];
extern char *rviewSetNames[][MAXIMUM_DIMENSIONS];

// Base type to image type mapping
extern int rviewImageTypes[];
// Possible types:
enum rview_image_types
{
    RVIEW_IMGTYPE_NONE,
    RVIEW_IMGTYPE_MONO,
    RVIEW_IMGTYPE_GREY,
    RVIEW_IMGTYPE_HIGH,
    RVIEW_IMGTYPE_GREY12,
    RVIEW_IMGTYPE_TRUE24,
    RVIEW_IMGTYPE_TRUE32
};



// Mouse buttons
#define MOUSE_LEFT      1
#define MOUSE_MIDDLE        2
#define MOUSE_RIGHT     4
// Ctrl pressed while mouse event?
#define MOUSE_CONTROL       8
// Maximum distance of mouse pointer to an object for manipulating / dragging
#define MOUSE_HOTZONE       8



// Menu items

enum rview_menu_ident
{
    MENU_DUMMY_IDENT, // 0 is illegal for a menu id

    // First the ones for the main window
    MENU_MAIN_FILE_QUERY,
    MENU_MAIN_FILE_PREFS,
    MENU_MAIN_FILE_EXIT,

    MENU_MAIN_VIEW_OPEN,
    MENU_MAIN_VIEW_CLOSE,

    MENU_MAIN_COLL_LOOK,
    MENU_MAIN_COLL_LKSCL,
    MENU_MAIN_COLL_LKORTH,
    MENU_MAIN_COLL_CREATE,
    MENU_MAIN_COLL_DELETE,

    MENU_MAIN_HELP_ABOUT,

    // Now the menu items for the results window
    MENU_RSLT_ITEM_OPENALL,
    MENU_RSLT_ITEM_THUMBALL,
    MENU_RSLT_ITEM_CLOSE,

    MENU_RSLT_SLCT_SLCTALL,
    MENU_RSLT_SLCT_CLEAR,
    MENU_RSLT_SLCT_OPEN,
    MENU_RSLT_SLCT_THUMB,
    MENU_RSLT_SLCT_DELETE,
    MENU_RSLT_SLCT_ENDIAN,
    MENU_RSLT_SLCT_TYPEMAN,
    MENU_RSLT_SLCT_INFO,

    // Now the menu items for the display windows
    MENU_DISP_DATA_INSERT,
    MENU_DISP_DATA_INSERTPRO,
    MENU_DISP_DATA_SAVE,
    MENU_DISP_DATA_CLOSE,
    MENU_DISP_DATA_SAVETIFF,
    MENU_DISP_VIEW_SAVE,
    MENU_DISP_VIEW_LOAD,
    MENU_DISP_VIEW_SHOW,
    // Additional menus in charts
    MENU_CHART_MODE_BAR,
    MENU_CHART_MODE_LINE,
    MENU_CHART_MODE_SPLINE,
    // Additional menus in images
    MENU_IMAGE_MODE_FLAT,
    MENU_IMAGE_MODE_SURF,
    MENU_IMAGE_MODE_VOXEL,
    MENU_IMAGE_MODE_HEIGHT,
    MENU_IMAGE_SETUP_RENDER,
    MENU_IMAGE_SETUP_RCONTROL,
    MENU_IMAGE_SETUP_CSPACE,
    MENU_IMAGE_SETUP_MOVIE,
    MENU_IMAGE_MOVIE_ONCE,
    MENU_IMAGE_MOVIE_START,
    MENU_IMAGE_MOVIE_SWITCH,
    MENU_IMAGE_CSPACE_ON,
    MENU_IMAGE_CSPACE_FULL,
    MENU_IMAGE_CSPACE_PROJ,
    MENU_IMAGE_CSPACE_EDIT,
    // Additional menus in tables
    MENU_TABLE_MODE_DECIMAL,
    MENU_TABLE_MODE_OCTAL,
    MENU_TABLE_MODE_HEX,

    // Thumbnail window
    MENU_THUMB_DATA_CLOSE,
    MENU_THUMB_SETUP_CSPACE,
    MENU_THUMB_CSPACE_ON,
    MENU_THUMB_CSPACE_FULL,
    MENU_THUMB_CSPACE_EDIT,

    // Now the menu items for the query window
    MENU_QUERY_FILE_OPEN,
    MENU_QUERY_FILE_SAVE,
    MENU_QUERY_FILE_EXIT,
    MENU_QUERY_EDIT_CUT,
    MENU_QUERY_EDIT_COPY,
    MENU_QUERY_EDIT_PASTE,
    MENU_QUERY_HELP_HELP,

    // Total number of menus = first free menu ID for dynamic menus:
    MENU_TOTAL_NUMBER
};

// Hotlist menu has no fixed length. Set upper limit with MENU_QUERY_HOTRANGE, however
#define MENU_QUERY_HOTLIST  MENU_TOTAL_NUMBER
// upper limit for queries in hotlist
#define MENU_QUERY_HOTRANGE 64



// Number of display modes
#define RVIEW_RESDISP_NUMBER    8
// Flags for each MDD object saying whether it's opened in a display mode
#define RVIEW_RESDISP_CHART 1
#define RVIEW_RESDISP_TABLE 2
#define RVIEW_RESDISP_SOUND 4
#define RVIEW_RESDISP_IMGFLAT   8
#define RVIEW_RESDISP_IMGVOLM   16
#define RVIEW_RESDISP_IMGHGHT   32
#define RVIEW_RESDISP_IMGOSECT  64
#define RVIEW_RESDISP_STRVIEW   128
// this one is special because it can't be used from the results window
#define RVIEW_RESDISP_IMGSCLD   256





// For frame_list type. Full declaration below.
class rviewFrame;
class rviewFrameMgr;




/*
 *  Types and structures.
 */

// User events
enum rviewUserEvent
{
    usr_none,
    usr_child_closed,     // data = rviewFrame *
    usr_mdd_dying,        // data = r_Ref<r_GMarray>*
    usr_db_opened,
    usr_db_closed,
    usr_update_closed,        // data = r_Ref<r_GMarray>*
    usr_viewer_closed,        // data = mdd_frame *
    usr_cspace_changed,       // data = colourspaceMapper*
    usr_typeman_convert,      // data = rviewTypeMan *
    usr_close_viewers
};


// The frame list item used but the frameManager.
typedef struct frame_list
{
    rviewFrame *frame;
    frame_list *next;
} frame_list;

// An MDD object and some flags
typedef struct mdd_frame
{
    r_Ref < r_GMarray > mdd;
    unsigned int flags;
} mdd_frame;

// Collection descriptor (for lookup / query exection)
// mddObjs != NULL ==> collection of marrays
// strObjs != NULL ==> collection of strings
typedef struct collection_desc
{
    char *collName;
    char *collType;
    char *collInfo;
    int number;
    mdd_frame *mddObjs;
    char **strObjs;
} collection_desc;

// User event used in rviewFrames / frameManager
typedef struct user_event
{
    rviewUserEvent type;
    void *data;
} user_event;

// Keyword to identifier lookup structure
typedef struct keyword_to_ident
{
    int ident;
    char *keyword;
} keyword_to_ident;

// same with a const char *
typedef struct keyword_to_ident_c
{
    int ident;
    const char *keyword;
} keyword_to_ident_c;




/*
 *  Global functions
 */

// Free all memory allocated by a filled collection descriptor
void rviewDeleteCollection(collection_desc *coll);

// Generic event handler. Calls the frame manager:
void rviewEventHandler(wxObject &obj, wxEvent &evt);

// Parse a projection string and return its dimensionality
extern int rviewParseProjection(const r_Minterval &interv, r_Point &pt1, r_Point &pt2, const char *projString, unsigned int *freeDims=NULL, r_Point *mapIndex=NULL);

// Return an object's base type as integer identifier
extern rviewBaseType rviewGetBasetype(r_Object *obj);

// Prints the contents of a cell with a given RasDaMan base type into the buffer, returns number
// of characters written.
extern int rviewPrintTypedCell(const r_Type *baseType, char *buffer, char *data, int numberBase);

// Quicksorts a char *[]
extern void rviewQuicksortStrings(char *array[], int from, int to);

// Init character lookup tables
extern void rviewInitCharacterTables(void);

// Look up a keyword in a sorted (!) keyword_to_ident array
extern int rviewLookupKeyword(const char *key, const keyword_to_ident_c *kti, int tabsize, bool caseSensitive);

// Checks if colourspace mapping is possible for a given base type
// and sets up some variables if so.
class colourspaceMapper;
struct colourspace_params_s;
extern int rviewCheckInitCspace(rviewBaseType baseType, colourspaceMapper **csmap, r_Ref<r_GMarray> &mddObj, bool fullRange=FALSE, r_Minterval *domain=NULL, int w=0, int *newPitch=NULL, int *newDepth=NULL, int *newPad=NULL, int *virtualPitch=NULL, const struct colourspace_params_s *cp=NULL);

// Smart number conversions: also understand 0x prefix
extern long asctol(const char *str);
extern int  asctoi(const char *str);
extern double   asctof(const char *str);
extern long stringtol(const char *str, char **endptr=NULL);
extern double   stringtof(const char *str, char **endptr=NULL);




// Dynamic string class
class DynamicString
{
public:

    DynamicString(void);
    DynamicString(const DynamicString &ds);
    DynamicString(const char *str);
    ~DynamicString(void);

    DynamicString &first(const char *str, unsigned int num);
    DynamicString &operator=(const DynamicString &ds);
    DynamicString &operator=(const char *str);
    bool operator==(const DynamicString &ds) const;
    bool operator==(const char *str) const;

    const char *ptr(void) const;
    operator const char*(void) const;


private:

    char *myString;

    static const char emptyString[];
};



// A stack template class
template<class T>
class DynamicStack
{
public:

    DynamicStack(unsigned int gran=8);
    DynamicStack(const DynamicStack<T> &src);
    ~DynamicStack(void);

    int push(const T &item);
    int pop(T &item);
    int peek(T &item) const;
    unsigned int getNumber(void) const;


protected:

    int ensureFree(void);

    unsigned int number, max, granularity;
    T *stack;
};





// Frame types
enum rviewFrameType
{
    rviewFrameTypeGeneric,
    rviewFrameTypeMain,
    rviewFrameTypeQuery,
    rviewFrameTypePrefs,
    rviewFrameTypeAbout,
    rviewFrameTypeDialog,
    rviewFrameTypeErrorbox,
    rviewFrameTypeProgress,
    rviewFrameTypeStrSet,
    rviewFrameTypeCspace,
    rviewFrameTypeImgSet,
    rviewFrameTypeRenCtrl,
    rviewFrameTypeResult,
    rviewFrameTypeThumb,
    rviewFrameTypeDisplay,
    rviewFrameTypeImage,
    rviewFrameTypeFltBsImage,
    rviewFrameTypeFlatImage,
    rviewFrameTypeRndImage,
    rviewFrameTypeVolImage,
    rviewFrameTypeHghtImage,
    rviewFrameTypeScaledImage,
    rviewFrameTypeOSectionImage,
    rviewFrameTypeChart,
    rviewFrameTypeTable,
    rviewFrameTypeSound,
    rviewFrameTypeStringViewer,
    rviewFrameTypeRenView,
    rviewFrameTypeNumberOfTypes
};


/*
 *  The base class for all frames. This makes managing open frames
 *  in a uniform way possible. Include after wx.h !
 */

class rviewFrame: public wxFrame
{
public:

    rviewFrame(wxFrame *parent, char *title, int x, int y, int w, int h);
    virtual ~rviewFrame(void);

    // When the checkobj(...) method is called the frame checks whether
    // it knows obj (e.g. it's a button contained in the frame) and claims
    // the broadcast by returning a value != 0. This is used in frame manager
    // broadcasts. You should not try to get involved with derived classes
    // at this point.
    int checkobj(wxObject &obj);

    // When the label() method is called the derived frame class
    // has to relabel all its widgets
    virtual void label(void) = 0;

    // The process() method is called for the object that claimed the
    // checkobj() function. It has to process the event and return a value
    // != 0 if it did anything, 0 if the event was ignored. The two
    // functions are separated because the Frame manager broadcast call
    // has to be re-entrant and we'll get huge problems if the frame
    // list changes during the loop.
    virtual int process(wxObject &obj, wxEvent &evt) = 0;

    // The userEvent method is called to notify frames of events like
    // the mdd object a frame displays being deleted.
    virtual int userEvent(const user_event &ue);

    // Called when the frame is to be killed. May be overloaded, but normally
    // doesn't have to be.
    virtual int requestQuit(int level);

    // Called for mouse events of some logical child. Normally ignored.
    virtual void childMouseEvent(wxWindow *child, wxMouseEvent &mevt);

    // The setParent(parent) function makes this frame a (logical) child
    // of parent. This will remove the frame from the parent's child frame
    // list when it's deleted.
    void setParent(rviewFrame *parent);
    void registerChild(rviewFrame *child);
    void deregisterChild(rviewFrame *child);

    // Returns the name of this class
    virtual const char *getFrameName(void) const;

    // Returns the identifier of this frame
    virtual rviewFrameType getFrameType(void) const;

    // By default each frame is closed by returning TRUE from this wxWindows
    // function. If you wish to override this behaviour for certain frames,
    // override the frame's virtual function OnClose().
    virtual bool OnClose(void);


protected:

    // For keeping track of child frames
    rviewFrameMgr *frames;
    rviewFrame *parentFrame;
    int frameWidth, frameHeight;



private:

    // Used internally by the checkobj() member function.
    int checkobj_rec(wxWindow *whence, wxObject &obj);
};




/*
 *  The frame manager. Provides central access to frames and
 *  offers functions like automatically re-labeling all existing
 *  frames. Each object of class rviewFrame registers itself here
 *  in the constructor and deregisters itself in the destructor.
 */

class rviewFrameMgr
{
public:

    rviewFrameMgr(void);
    rviewFrameMgr(bool delChild);
    ~rviewFrameMgr(void);

    void registerFrame(rviewFrame *client);
    void deregisterFrame(rviewFrame *client);
    int numberOfFrames(void) const;
    void setDeleteMode(bool delChild);

    // The following functions cause the frame manager to iterate over
    // all frames and call specific member functions. Some of these
    // member functions (e.g. process) can stop the iteration by
    // claiming the call.
    void labelAll(void);
    void broadcastEvent(wxObject &obj, wxEvent &evt);
    int broadcastQuit(int level);
    int broadcastUserEvent(const user_event &ue);


protected:

    frame_list *frameList;
    frame_list *tailList;
    int listLength;
    bool deleteChildren;
};





/*
 *  A continuous text displayed in multiple lines
 */

class rviewMultiline
{
public:

    rviewMultiline(wxPanel *Panel, int X, int Y, int H, int Lines);
    rviewMultiline(wxPanel *Panel, const char *Message, int X, int Y, int W, int H, int Lines);
    ~rviewMultiline(void);

    // Returns the height used by the messages
    int getMessageHeight(void) const;

    void rebuild(const char *Message, int W);

    // 10 * number of pixels used per character
    static const int multiline_ppc10;


protected:

    void setupVariables(wxPanel *Panel, int X, int Y, int H, int Lines);

    wxPanel *parent;
    wxMessageOutput **msg;
    int lines;
    int x, y, lHeight;
};



/*
 *  Convenient interface to standard widgets
 */
#include <wx/univ/textctrl.h>

class rviewText : public wxTextCtrl
{
public:

    rviewText(wxPanel *parent, const char *value=NULL, char *label="", int x=-1, int y=-1, int w=-1, int h=-1);
    rviewText(long style, wxPanel *parent, const char *value=NULL, char *label="", int x=-1, int y=-1, int w=-1, int h=-1);
    rviewText(wxPanel *parent, const DynamicString &value, char *label="", int x=-1, int y=-1, int w=-1, int h=-1);
    rviewText(wxPanel *parent, int value, char *label="", int x=-1, int y=-1, int w=-1, int h=-1);
    rviewText(wxPanel *parent, long value, char *label="", int x=-1, int y=-1, int w=-1, int h=-1);
    rviewText(wxPanel *parent, double value, bool sciForm=FALSE, char *label="", int x=-1, int y=-1, int w=-1, int h=-1);

    void SetValue(char *value);
    void SetValue(const char *value);
    void SetValue(const DynamicString &value);
    void SetValue(int value);
    void SetValue(unsigned int value);
    void SetValue(long value);
    void SetValue(double value, bool sciFrom=FALSE);

    char *GetValue(void);
    void GetValue(DynamicString &value);
    void GetValue(int &value);
    void GetValue(long &value);
    void GetValue(float &value);
    void GetValue(double &value);
};

class rviewButton : public wxButton
{
public:

    rviewButton(wxPanel *parent, char *label="", int x=-1, int y=-1, int w=-1, int h=-1, long style=0);
};

// BUG IN wxCoice: NEVER CREATE A (wx|rview)Choice ITEM WITH AN EMPTY LABEL!
class rviewChoice : public wxChoice
{
public:

    rviewChoice(wxPanel *parent, int n, char *choices[], char *label="X", int x=-1, int y=-1, int w=-1, int h=-1, long style=0);
    // this is kind of a hack to fix wxWindows' lack of const
    rviewChoice(wxPanel *parent, int n, const char *choices[], char *label="X", int x=-1, int y=-1, int w=-1, int h=-1, long style=0);
};

class rviewCheckBox : public wxCheckBox
{
public:

    rviewCheckBox(wxPanel *parent, char *label="", int x=-1, int y=-1, int w=-1, int h=-1);
};

class rviewRadioButton : public wxRadioButton
{
public:

    rviewRadioButton(wxPanel *parent, char *label="", bool value=FALSE, int x=-1, int y=-1, int w=-1, int h=-1);
};

class rviewScrollBar : public wxScrollBar
{
public:

    rviewScrollBar(wxPanel *parent, int x=-1, int y=-1, int w=-1, int h=-1, long style=wxHORIZONTAL);
};

class rviewSlider : public wxSlider
{
public:

    rviewSlider(wxPanel *parent, int value, int min_val, int max_val, int width, char *label="", int x=-1, int y=-1, long style=wxHORIZONTAL);
};


/*
 *  The original slider class didn't allow me to pass on arbitrary mouse events
 *  to the (logical) parent which was needed in the orthosection viewer for
 *  automatically loading the data on a button up event. This slider class fixes
 *  this problem and has the same functionality as the original one. Use it
 *  whenever arbitrary mouse events are important.
 */

class rviewSpecialSlider : public wxGLCanvas
{
public:
    rviewSpecialSlider(rviewFrame *logParent, wxPanel *parent, int val, int min, int max, int width=-1, const char *label=NULL);
    ~rviewSpecialSlider(void);

    int GetMax(void) const;
    int GetMin(void) const;
    int GetValue(void) const;
    void SetRange(int min, int max);
    void SetValue(int val);
    bool PositionInWell(float posx, float posy);

    virtual void SetLabel(const char *label);
    virtual void OnPaint(void);
    virtual void OnEvent(wxMouseEvent &mevt);


protected:
    void getWellVert(int &y0, int &y1);
    void getBarParams(float &posx, float &posy, float &height);
    int calcNewValue(float posx, float posy, int &val, bool checky=FALSE);
    void redrawCore(float x, float y, float bheight);
    void getUpdateInterval(float oldx, float newx, float &clipx, float &clipw);
    void updateWell(float oldx, float newx, float posy, float bheight);
    wxColour background;
    wxColour foreground;
    wxColour wellground;
    wxColour outline;
    wxColour labelColour;
    wxBrush bback;
    wxBrush bfore;
    wxBrush bwell;
    wxPen outlinePen;
    wxFont labelFont;
    int border, barwidth, barheight;
    int value, vmin, vmax;
    int cwidth, cheight;
    float textx, texty;
    DynamicString myLabel;
    rviewFrame *logicalParent;
    static const int dflt_width;
    static const int dflt_height;
    static const int dflt_border;
    static const int dflt_barwidth;
    static const int dflt_barheight;
};




/*
 *  Dialog box, containing a message and some buttons.
 */
class rviewDialog: public rviewFrame
{
public:

    rviewDialog(const char *title, const char *message, int buttonNo, const char *buttons[]);
    virtual ~rviewDialog(void);
    void OnSize(int w, int h);

    int GetButton(void);

    // implementations of the rviewFrame virtual functions
    void label(void);
    int process(wxObject &obj, wxEvent &evt);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // constants
    // Width and height of the dialog box
    static const int dialog_width;
    static const int dialog_height;
    // Borders used in the dialog box
    static const int dialog_border;
    // Width and height of the buttons
    static const int dialog_buttonsx;
    static const int dialog_buttonsy;
    // Height reserved at the bottom for the buttons
    static const int dialog_bheight;
    // Number of text lines
    static const int dialog_lines;
    // Height of each of these lines
    static const int dialog_lheight;


protected:

    wxPanel *panel;
    char **buttonText;
    //wxTextWindow *msg;
    rviewMultiline *msg;
    rviewButton **but;
    int buttonNumber;
    int buttonPressed;
};



/*
 *  An error box.
 */

class rviewErrorbox: public rviewDialog
{
public:

    // Default error-box: a message and an OK button.
    rviewErrorbox(const char *message);
    rviewErrorbox(const char *title, const char *message, int buttonNo, const char *buttons[]);
    ~rviewErrorbox(void);
    int activate(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // creating errorboxes with formatted text
    static rviewErrorbox *newErrorbox(const char *message, const char *classname=NULL, const char *funcname=NULL);
    static rviewErrorbox *newErrorbox(const char *title, const char *message, int buttonNo, const char *buttons[], const char *classname=NULL, const char *funcname=NULL);
    static int reportError(const char *message, const char *classname=NULL, const char *funcname=NULL);
    static int reportError(const char *title, const char *message, int buttonNo, const char *buttons[], const char *classname=NULL, const char *funcname=NULL);

private:

    static char *buildErrorMessage(const char *message, const char *classname, const char *funcname);
};



/*
 *  Progress reports
 */

class rviewProgress: public rviewDialog
{
public:

    // Progress reporter (mostly used by database)
    rviewProgress(const char *message);
    ~rviewProgress(void);

    int process(wxObject &obj, wxEvent &evt);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
};



/*
 *  Window holding MDD results
 */

class rviewTypeMan;

class rviewResult: public rviewFrame
{
public:

    rviewResult(void);
    rviewResult(collection_desc *collection);
    ~rviewResult(void);
    void setCollection(collection_desc *collection);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);
    int userEvent(const user_event &ue);

    void OnSize(int w, int h);
    void OnMenuCommand(int id);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // constants
    // X and Y position of result box
    static const int result_x;
    static const int result_y;
    // Width and height of result box
    static const int result_width;
    static const int result_height;
    // Borders used in results box
    static const int result_border;
    // Height of a line (collection info)
    static const int result_lheight;
    // Space reserved for header (collection info)
    static const int result_header;
    // Width of choice item
    static const int result_cwidth;
    // Width and height of resample group
    static const int result_twidth;
    static const int result_theight;
    // Width and height of button
    static const int result_bwidth;
    static const int result_bheight;


protected:

    void setupVariables(void);
    void openViewer(int item);
    int updateSelection(void);
    void configureGrey(void);
    int parseResampleString(const char *resStr, double *values);
    int resampleSelection(void);
    char *mddDescriptorString(std::ostream &memstr, int number);
    void convertSelectedItems(void);
    // for printing information on time-consuming operations like scaling.
    void operationPrologue(void);
    void operationEpilogue(const char *opname);

    collection_desc *coll;

    char *selectedItems;  // bitfield
    int lastSelected;
    int cbfactor; // number of units a checkbox is bigger than the size set

    wxGroupBox *group;
    wxMessage *collName, *collType, *collInfo;
    rviewChoice *choice;
    wxPanel *panel;
    wxListBox *list;
    wxMenuBar *mBar;
    rviewText *resampText;
    rviewChoice *scaleMode;
    rviewButton *resampBut;

    rviewTypeMan *typeManager;
};




/*
 *  About rView window
 */
class rviewAbout: public rviewFrame
{
public:

    rviewAbout(void);
    ~rviewAbout(void);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);

    void OnSize(int w, int h);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // constants
    // Width and height of About window
    static const int about_width;
    static const int about_height;
    // Borders in About window
    static const int about_border;
    // Height of panel
    static const int about_pheight;
    // Width and height of buttons in About window
    static const int about_bwidth;
    static const int about_bheight;
    // Height of message item
    static const int about_mheight;


protected:

    void deleteLabels(void);

    wxPanel *panel;
    rviewButton *okBut;
    wxMessage **labels;
    int numlines;
};



/*
 *  rviewStringSet window
 */
class rviewStringSet: public rviewFrame
{
public:

    rviewStringSet(collection_desc *desc);
    ~rviewStringSet(void);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);
    void OnSize(int w, int h);

    int getNumItems(void);
    void addItem(const char *string);
    char *getItem(int number);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // constants
    // Width and height of the string set window
    static const int strset_width;
    static const int strset_height;
    // Borders in string set window
    static const int strset_border;
    // Reserve space at the bottom for the dismiss button
    static const int strset_reserve;
    // Width and height of the dismiss button
    static const int strset_bwidth;
    static const int strset_bheight;
    // Height of message fields
    static const int strset_mheight;


protected:

    wxPanel *panel;
    wxListBox *list;
    rviewButton *dismiss;
    wxMessage *collName, *collType, *collInfo;
};







/*
 *  Global variables
 */

extern labelManager *lman;
extern rviewFrameMgr *frameManager;
extern unsigned char lowerCaseTable[256];


#if (defined(EARLY_TEMPLATE) && defined(__EXECUTABLE__))
#include "rviewUtils.cpp"
#endif

#endif
