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
 *  Class definitions for the regular object viewers and their support
 *  classes:
 *      - rviewImage hierarchy, including pixmapCanvas, rendererControl
 *        and rviewImageSetup. Source in rviewImage.cpp.
 *      - rviewChart (chartCanvas). Source in rviewChart.cpp.
 *      - rviewTable (tableCanvas). Source in rviewTable.cpp.
 *  COMMENTS:
 *          None
 */

/**
*   @file rviewDmodes.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_DMODES_H_
#define _RVIEW_DMODES_H_

#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/gmarray.hh"

#include "labelManager.hh"
#include "rviewUtils.hh"
#include "rviewDisplay.hh"
#include "rviewColMap.hh"

#ifdef __VISUALC__
struct vertex;
struct vertex_fp;
struct graph_env;
struct tex_desc;
struct voxel_desc;
struct mesh_desc;
struct mdd_desc;
#else
typedef struct vertex;
typedef struct vertex_fp;
typedef struct graph_env;
typedef struct tex_desc;
typedef struct voxel_desc;
typedef struct mesh_desc;
typedef struct mdd_desc;
#endif

class rviewImage;
class rviewRenderImage;


// Possible display formats
enum rviewImageMode
{
    rim_none,
    rim_surf,
    rim_voxel
};


// Flags for light direction
#define RVIEW_LIGHTDIR_LEFT 1
#define RVIEW_LIGHTDIR_RIGHT    2
#define RVIEW_LIGHTDIR_DOWN 4
#define RVIEW_LIGHTDIR_UP   8
#define RVIEW_LIGHTDIR_FRONT    16
#define RVIEW_LIGHTDIR_BACK 32

typedef struct rview_image_setup
{
    unsigned long zpro, clipz;
    double pixelThresholdLow, pixelThresholdHigh, weightThreshold;
    int weightQuantisation;
    bool useRgbBrightness;
    bool useLights;
    double lightsAmbient;
    double lightsGain;
    double lightsAngle;
    double lightsScintAngle;
    int lightsDir;
    int lightsDist;
    int kernelSize, kernelType;
    bool useVoxCol;
    double voxColour;
    int gridSize;
    double scaleHeight;
} rview_image_setup;


/*
 *  Image setup window for customizing the renderer.
 */
class rviewImageSetup: public rviewFrame
{
public:

    rviewImageSetup(rview_image_setup *ris, rviewRenderImage *parentWin);
    ~rviewImageSetup(void);

    void unlinkParent(void);

    void OnSize(int w, int h);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);

    void updateSettings(const rview_image_setup &ris);
    void readNewSetup(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    static int parseLightDirection(const char *dir);

    // constants
    // Borders used in setup window
    static const int imgset_border;
    // Height of text widgets in setup window
    static const int imgset_theight;
    // Height of checkboxes in setup window
    static const int imgset_chkheight;
    // Height of group boxes in setup window
    static const int imgset_renheight;
    static const int imgset_voxheight;
    static const int imgset_hgtheight;
    // Dimensions of buttons in setup window
    static const int imgset_bwidth;
    static const int imgset_bheight;
    // Dimensions of checkboxes in setup window
    static const int imgset_chowidth;
    static const int imgset_choheight;
    // Setup window dimensions
    static const int imgset_width;
    static const int imgset_height;


protected:

    rviewRenderImage *parent;
    wxPanel *panel;
    wxGroupBox *renderGroup, *voxelGroup, *heightGroup;
    rviewText *zproWidget, *clipzWidget;
    rviewText *pixThreshLowWidget, *pixThreshHighWidget, *wgtThreshWidget, *wgtQuantWidget;
    rviewCheckBox *useRgbBrightness;
    rviewCheckBox *useLights;
    rviewChoice *kernelSize, *kernelType;
    rviewText *lightsAmbient, *lightsGain, *lightsAngle, *lightsScintAngle;
    rviewText *lightsDir, *lightsDist;
    rviewCheckBox *useVoxCol;
    rviewText *voxColour;
    rviewText *gridSize, *scaleHeight;
    rviewButton *okBut, *cancelBut;
    rview_image_setup oldSetup, *imgSetup;

    static const char *normalKernelSizes[];
    static const keyword_to_ident_c normalKernelTypes[];
};



/*
 *  Renderer playback control window
 */
class rendererControl: public rviewFrame
{
public:

    rendererControl(float drx, float dry, float drz, int mode, rviewRenderImage *parentWin);
    ~rendererControl(void);

    void unlinkParent(void);
    void setActiveMode(int mode);

    void OnSize(int w, int h);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // constants
    // Borders used in renderer control window
    static const int rctrl_border;
    // Button dimensions
    static const int rctrl_bwidth;
    static const int rctrl_bheight;
    // Reset button dimensions
    static const int rctrl_rwidth;
    static const int rctrl_rheight;
    // Slider height
    static const int rctrl_sheight;
    // Window dimensions
    static const int rctrl_width;
    static const int rctrl_height;


protected:

    void updateParameters(void);

    wxPanel *panel;
    rviewSlider *rotx, *roty, *rotz;
    rviewButton *resetX, *resetY, *resetZ;
    rviewButton *actionBut, *closeBut;
    rviewRenderImage *parent;
    int active;
};


/*
 *  Renderer current view window (rotation, offset, scale)
 */
class rendererCurrentView: public rviewFrame
{
public:

    rendererCurrentView(const vertex_fp &angles, long off, double scale, rviewRenderImage *parentWin);
    ~rendererCurrentView(void);

    void unlinkParent(void);

    void OnSize(int w, int h);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    void updateView(const vertex_fp &angles, long off, double scale);

    // constants
    // Borders used in renderer view window
    static const int rcview_border;
    // Button dimensions
    static const int rcview_bwidth;
    static const int rcview_bheight;
    // Text widget height
    static const int rcview_theight;
    // Window dimensions
    static const int rcview_width;
    static const int rcview_height;


protected:

    void updateParameters(void);

    wxPanel *panel;
    rviewButton *applyButton, *closeButton;
    rviewText *rotx, *roty, *rotz;
    rviewText *zoff;
    rviewText *cubeScale;
    rviewRenderImage *parent;
};



class wxPixmap;

/*
 *  Container class for wxPixmap.
 */
class pixmapCanvas: public wxCanvas
{
public:

    pixmapCanvas(rviewImage *parent, int x, int y, int w, int h, long style=0);
    ~pixmapCanvas(void);

    void setPixmap(wxPixmap *pmap);
    void updateDisplay(bool borders=FALSE);

    void OnPaint(void);
    void OnEvent(wxMouseEvent &mevt);

    void SetAspectRatio(double ratio);    // 0 for none
    void ToggleDragBox(bool clearMode);
    void SetDragBox(int x0, int y0, int x1, int y1);
    bool HasDragBox(void) const;
    bool GetDragBox(int &x0, int &y0, int &x1, int &y1) const;
    void UpdateDragBox(int x1, int y1);
    void AdjustDragBox(int x1, int y1);


protected:

    void paintCore(int x, int y);
    void adjustBoxToRatio(void);

    wxPixmap *pixmap;
    wxDC *myDC;
    wxBrush brush;
    wxBrush border;
    wxBrush textBack;
    wxPen bpen;
    wxPen fpen;
    wxFont *font;
    rviewImage *parentWin;
    int pixWidth, pixHeight;
    int offX, offY;
    int rect_x0, rect_y0, rect_x1, rect_y1;
    double aspectRatio;
    wxRect textBBox;
};


// Flags used when settings are updated
#define RVIEW_IFLAG_VOXEL   1
#define RVIEW_IFLAG_LIGHT   2
#define RVIEW_IFLAG_HEIGHT  4

/*
 *  Abstract base class for all windows containing an image + controls
 */
class rviewImage: public rviewDisplay
{
public:

    rviewImage(mdd_frame *mf, int es, unsigned int flags=0);
    virtual ~rviewImage(void);

    virtual int openViewer(void);

    virtual void label(void);
    virtual int process(wxObject &obj, wxEvent &evt);
    virtual void processMouseEvent(wxMouseEvent &mevt);
    virtual int userEvent(const user_event &ue);
    virtual void prepareToDie(void);
    virtual int newProjection(void)=0;
    virtual int requestQuit(int level);

    virtual void OnSize(int w, int h);
    virtual void OnMenuCommand(int id);
    virtual bool OnClose(void);   // overload from rviewFrame

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // Default flags for wxPixmap class (dithering etc)
    static int getPixmapFlags(void);

    // constants
    // Slider dimensions
    static const int image_swidth;
    static const int image_sheight;
    // Playback buttons dimensions
    static const int image_pbwidth;
    static const int image_pbheight;
    // Minimum size of image
    static const int image_minwidth;
    static const int image_minheight;
    // Checkbox dimensions
    static const int image_chkwidth;
    static const int image_chkheight;
    // Bounding box checkbox dimensions
    static const int image_bbwidth;
    static const int image_bbheight;
    // Text field dimensions
    static const int image_twidth;
    static const int image_theight;
    // Button dimensions
    static const int image_bwidth;
    static const int image_bheight;
    // Offsets of drag box in canvas
    static const int image_dragtoff;
    static const int image_dragtspace;
    // Distance from the border at which to autoscroll
    static const int image_draghotzone;
    // Extra space reserved on control panel
    static const int image_ctrly;
    static const int image_totaly;


protected:

    void updatePixmap(char *oldData, char *newData);
    void configureCspace(bool state);
    void setCspaceProjMode(bool pmode);
    void resizeImage(void);
    void openViewerEpilogue(rviewFrameType ft);
    int freeDimsFromProjection(int &dim1, int &dim2, r_Point *map);
    void ensureViewCspace(void);
    void deleteViewCspace(void);
    virtual void projectObjectHook(void);
    virtual void configureMode(void);
    virtual char *initMode(void)=0;
    virtual bool cspaceRangeHook(bool suggest);
    virtual char *movieNewFrame(void);
    virtual void rotateObject(wxMouseEvent &mevt);
    virtual int fileMenuInitHook(wxMenu *menu);
    virtual int configMenuInitHook(wxMenu *menu);
    // Query viewer capabilities
    virtual bool modeNeedsCspace(rviewBaseType bt) const;
    virtual bool canRotateObject(void) const;
    virtual bool moviePossible(void) const;
    virtual bool showScaleSlider(void) const;
    // view management
    virtual int saveView(FILE *fp);
    virtual int readView(const char *key, const char *value);
    virtual void loadViewFinished(void);

    wxPixmap *pixmap;
    pixmapCanvas *pcanv;
    rviewSlider *scaleSlider;
    int pixWidth, pixHeight, pixPitch, pixPad, pixDepth;
    int virtualPitch;
    char *imgData;
    double scaleValue;
    int scrollx, scrolly;
    float mousex, mousey;
    int mousebut;
    unsigned int freeDims;
    // For intensity to RGB translations (ushort)
    colourspaceMapper *csmap;
    bool doValToCspace;
    bool doFullRangeCspace;
    bool doProjRangeCspace;
    // base type allows cspace mapping?
    bool cspaceForType;
    // image fully initialized?
    bool initPhaseFinished;
    r_Minterval *csInterv;
    // shared by flat and height field which don't have a shared derivation path
    rviewButton *playFwd, *playBack, *playStop;
    int playDirection;
    int lastMovieMode;
    // colourspace parameters for view loading
    colourspace_params *cspar;

    // view parameters
    static const char *view_ScrollPos;
    static const char *view_UseCspace;
    static const char *view_CspaceFull;
    static const char *view_CspaceProj;
    static const char *view_CspaceMeans;
    static const char *view_CspaceSigmas;
    static const char *view_CspaceType;
    static const char *view_CspaceRange;
    static const char *view_ScaleValue;
};


/*
 *  Base class for flat images, i.e. 2D orthogonal projections
 */
class rviewFlatBaseImage: public rviewImage
{
public:

    rviewFlatBaseImage(mdd_frame *mf, int es, unsigned int flags=0);
    virtual ~rviewFlatBaseImage(void);

    virtual int newProjection(void);
    virtual int openViewer(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;


protected:

    virtual char *initMode(void);
    char *projectImage(void);
};


/*
 *  Standard flat images
 */
class rviewFlatImage: public rviewFlatBaseImage
{
public:

    rviewFlatImage(mdd_frame *mf, unsigned int flags=0);
    ~rviewFlatImage(void);

    void OnSize(int w, int h);

    virtual int openViewer(void);
    virtual void label(void);
    virtual int process(wxObject &obj, wxEvent &evt);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;


protected:

    virtual char *initMode(void);
    virtual bool moviePossible(void) const;
    virtual char *movieNewFrame(void);
};


/*
 *  Abstract base class for all rendered images (voxel, height field, whatever...)
 */
class rviewRenderImage: public rviewImage
{
public:

    rviewRenderImage(mdd_frame *mf, int es, unsigned int flags=0);
    virtual ~rviewRenderImage(void);

    virtual void label(void);
    virtual int process(wxObject &obj, wxEvent &evt);
    virtual int newProjection(void);
    virtual void prepareToDie(void);
    virtual int requestQuit(int level);
    virtual int userEvent(const user_event &ue);
    virtual void OnSize(int w, int h);
    virtual void OnMenuCommand(int id);
    virtual bool OnClose(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    void closeEditor(bool newSetup);
    void updateSettings(int setFlags);

    void closeRendererControls(void);
    void setAutoRotation(float rx, float ry, float rz);
    void setCurrentView(const vertex_fp &angles, long off, double scale);


protected:

    virtual char *initMode(void);
    virtual char *setupEnvironment(int w, int h)=0;
    virtual void rotateObject(wxMouseEvent &mevt);
    virtual bool doUpdate(int updateFlags)=0;
    virtual void redrawSettingsChanged(void);
    virtual void fillBuffer(void)=0;
    virtual void fillBackgroundCore(rviewBaseType bt, double minVal);
    virtual int configMenuInitHook(wxMenu *menu);
    virtual int viewMenuInitHook(wxMenu *menu);
    virtual bool canRotateObject(void) const;
    virtual void updateCurrentView(void);

    int setupEnvBase(int w, int h, r_Ref<r_GMarray> &mdd, colourspaceMapper **csm, r_Minterval *csdom);
    char *setupGraphEnv(void);
    void fillBufferBackground(bool doCspace, bool &cspaceOK, r_Ref<r_GMarray>& obj, colourspaceMapper **csm, r_Minterval *csdom, rviewBaseType bt, bool fullRange, double *useMinVal=NULL);
    void translateBufferToCspace(rviewBaseType bt, double *useMinVal=NULL, double *useMaxVal=NULL);
    static void rotateCube(int axis, float angle, vertex_fp *matrix);
    void rotateCube(int axis, float angle);
    void getLightPos(vertex_fp *lpos);

    virtual int saveView(FILE *fp);
    virtual int readView(const char *key, const char *value);
    virtual void loadViewFinished(void);

    // convert rotation matrix to angles and back
    void matrixToAngles(vertex_fp &angles) const;
    void anglesToMatrix(const vertex_fp &angles);

    int rendererPlayback;
    vertex_fp *geomData, *geomUse;
    vertex_fp *rot;
    graph_env *graphEnv;
    union
    {
        unsigned long l;
        float f;
        double d;
    } voxColour;
    long zoff;
    double cubeScale;
    // Rotation angle increments for renderer playback
    float drx, dry, drz;
    // Setup options
    rview_image_setup setup;
    rviewImageSetup *setupWindow;
    rendererControl *rcontrol;
    rendererCurrentView *rcurview;

    // view keywords
    static const char *view_ZProject;
    static const char *view_ZClip;
    static const char *view_PixThreshLow;
    static const char *view_PixThreshHigh;
    static const char *view_WeightThresh;
    static const char *view_WeightQuant;
    static const char *view_UseRGBBright;
    static const char *view_UseLighting;
    static const char *view_LightAmbient;
    static const char *view_LightGain;
    static const char *view_LightAngle;
    static const char *view_LightScint;
    static const char *view_LightDir;
    static const char *view_LightDist;
    static const char *view_KernelSize;
    static const char *view_KernelType;
    static const char *view_UseVoxColour;
    static const char *view_VoxColour;
    static const char *view_GridSize;
    static const char *view_ScaleHeight;
    static const char *view_Rotation;
    static const char *view_ZOffset;
};


/*
 *  Class for visualizations of volumetric images (3D texture mapper &
 *  voxel renderer).
 */
class rviewVolumeImage: public rviewRenderImage
{
public:

    rviewVolumeImage(mdd_frame *mf, unsigned int flags=0);
    virtual ~rviewVolumeImage(void);

    virtual void label(void);
    virtual int process(wxObject &obj, wxEvent &evt);
    virtual void OnSize(int w, int h);
    virtual void OnMenuCommand(int id);
    virtual int openViewer(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;


protected:

    virtual char *initMode(void);
    virtual char *setupEnvironment(int w, int h);
    virtual bool doUpdate(int updateFlags);
    virtual void fillBuffer(void);

    virtual int saveView(FILE *fp);
    virtual int readView(const char *key, const char *value);
    virtual void loadViewFinished(void);

    rviewCheckBox *boundingBox;
    rviewImageMode imode;
    int lastMode;
    tex_desc *texDesc;
    voxel_desc *voxDesc;
    bool initVoxParams;
    bool doBoundingBox;

    static const char *view_VolumeMode;
    static const char *view_UseBBox;
};


/*
 * Class for visualizing data as heightfields.
 */
class rviewHeightImage: public rviewRenderImage
{
public:

    rviewHeightImage(mdd_frame *mf, unsigned int flags=0);
    virtual ~rviewHeightImage(void);

    virtual void label(void);
    virtual int process(wxObject &obj, wxEvent &evt);
    virtual int newProjection(void);
    virtual void prepareToDie(void);
    virtual int requestQuit(int level);
    virtual int openViewer(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;


protected:

    int depthForHeightfield(void) const;
    virtual char *initMode(void);
    virtual char *setupEnvironment(int w, int h);
    virtual char *movieNewFrame(void);
    virtual bool doUpdate(int updateFlags);
    virtual void redrawSettingsChanged(void);
    virtual void fillBackgroundCore(rviewBaseType bt, double minVal);
    virtual void fillBuffer(void);
    virtual bool cspaceRangeHook(bool suggest);
    virtual bool moviePossible(void) const;
    virtual bool modeNeedsCspace(rviewBaseType bt) const;

    mdd_desc *mddDesc;
    mesh_desc *meshDesc;
    // for colourspace mapping of heightfields.
    r_Ref<r_GMarray> dummyMDD;
};



/*
 *  An image scaled by the DBMS
 */
class r_Fast_Base_Scale;

class rviewScaledImage: public rviewFlatBaseImage
{
public:

    rviewScaledImage(collection_desc *cd, r_Fast_Base_Scale *scaler, unsigned int flags=0);
    ~rviewScaledImage(void);

    virtual void processMouseEvent(wxMouseEvent &mevt);
    virtual void label(void);
    virtual int process(wxObject &obj, wxEvent &evt);
    virtual int openViewer(void);
    virtual void OnSize(int w, int h);
    virtual int newProjection(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;
    virtual const r_Minterval &getVirtualDomain(void) const;


protected:

    virtual char *initMode(void);
    virtual void projectObjectHook(void);

    char *projectImage(void);
    bool showScaleSlider(void) const;
    void scaleViewBy(double scale);
    void newView(bool loadImage=TRUE);
    void projectionStringForView(void);
    double getLastScale(void) const;

    // view management
    virtual int saveView(FILE *fp);
    virtual int readView(const char *key, const char *value);
    virtual void loadViewFinished(void);
    void ensureLoadedView(void);

    rviewButton *zoomInBut;
    rviewButton *zoomOutBut;
    rviewButton *lastZoomBut;
    rviewButton *zoomBoxBut;
    rviewText *scaleString;

    bool boxState;

    typedef struct
    {
        double scale;
        r_Point low, high;
        int dim1, dim2;
    } view_desc_t;

    bool compareViews(const view_desc_t &v1, const view_desc_t &v2);

    // The current view
    view_desc_t thisView;
    r_Minterval fullDomain;
    bool dontLoad;
    double initialScale;
    view_desc_t *loadedView;

    DynamicStack<view_desc_t> viewHistory;

    r_Fast_Base_Scale *scaleObject;

    collection_desc *collDesc;

    static const double scaleStep;

    // view parameters
    static const char *view_CurrentBox;
    static const char *view_BoxScale;
};


// Structure for static conversion of MDD -> pixmap
typedef struct rviewFlatProjEnv
{
    // initialized by caller
    r_GMarray *mddPtr;
    r_Point pt1, pt2;
    int dim1, dim2;
    rviewBaseType bt;
    double scale;
    bool doCspace;
    colourspaceMapper *csmap;
    int cspaceState;
    // initialized by prepareFlatProjection()
    int width, height;
    int pitch, pad, depth;
} rviewFlatProjEnv;

// Setup variables for projecting images
int rviewPrepareFlatProjection(rviewFlatProjEnv &penv);
// Project an MDD object into an image bitmap
int rviewPerformFlatProjection(rviewFlatProjEnv &env, char *data);






enum rviewChartMode
{
    rcm_none,
    rcm_bar,
    rcm_line,
    rcm_spline
};

/*
 *  Canvas displaying a chart
 */

class chartCanvas: public wxCanvas
{
public:

    chartCanvas(wxWindow *parent, int x, int y, int w, int h, long style=0);
    ~chartCanvas(void);

    void setData(mdd_frame *mf, rviewBaseType bt);
    void setVars(int s, double cs, int ds, bool cy, rviewChartMode cm);
    int  setProjection(r_Point &p1, r_Point &p2);
    void OnPaint(void);

    // constants
    // Space reserved on top / bottom of chart
    static const int chcanv_cospace;
    // Length of a corrdinate axis marker line
    static const int chcanv_colength;
    // Start exponential axis numbering when |exponent| >= x
    static const int chcanv_exponents;


protected:

    // Core-functionality of chart modes
    void redrawBar(wxDC *cdc, int height, int dim, int startOff, int endOff, float scale, float posx, float stepx, float orgy);
    void redrawLine(wxDC *cdc, int dim, int startOff, int endOff, float scale, float posx, float stepx, float orgy);
    void redrawSpline(wxDC *cdc, int dim, int startOff, int endOff, float scale, float posx, float stepx, float orgy);

    r_Ref<r_GMarray> mddObj;
    int step, dimMDD;
    r_Point pt1, pt2;
    double min, max, costep;
    rviewBaseType baseType;
    rviewChartMode cmode;
    wxBrush brush, back;
    wxBrush brush_r, brush_g, brush_b;
    wxPen pen, pen_r, pen_g, pen_b;
    wxFont *font;
    bool cosys;
    char format[10];
    int coleft, datastep;
    int scroll;
};


/*
 *  A class for drawing charts
 */

class rviewChart: public rviewDisplay
{
public:

    rviewChart(mdd_frame *mf, unsigned int flags=0);
    ~rviewChart(void);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);
    int newProjection(void);
    virtual int openViewer(void);

    void OnSize(int w, int h);
    void OnMenuCommand(int id);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;

    // constants
    // Text widget dimensions (step)
    static const int chart_twidth;
    static const int chart_theight;
    // checkbox dims
    static const int chart_cwidth;
    static const int chart_cheight;
    // Minimum size of viewer
    static const int chart_minwidth;
    static const int chart_minheight;
    static const int chart_ctrly;
    static const int chart_totaly;


protected:

    virtual int saveView(FILE *fp);
    virtual int readView(const char *key, const char *value);
    virtual void loadViewFinished(void);

    void checkModeMenu(void);

    chartCanvas *canvas;
    int step, datastep;
    int lastMode;
    double costep;
    int scroll;
    bool cosys;
    rviewChartMode cmode;
    rviewText *stText, *coText, *dataText;
    rviewCheckBox *csBox;

    static const char *view_StepSize;
    static const char *view_Markers;
    static const char *view_ScrollPos;
    static const char *view_CoSys;
    static const char *view_ChartMode;
};



/*
 *  Canvas for plotting text onto
 */

class textCanvas: public wxCanvas
{
public:

    textCanvas(wxWindow *parent, int x, int y, int w, int h, long style=0);
    ~textCanvas(void);

    void setData(mdd_frame *mf, rviewBaseType bt, unsigned int bs);
    void setStep(int sx, int sy);
    void setCoSys(bool cs, int &cl, int &ct);
    void setProjection(r_Point &pt1, r_Point &pt2, unsigned int fd, r_Point *mapIndex=NULL);
    void setNumberBase(int newBase);
    void OnPaint(void);
    void CalcTextExtent(char *b, float &width, float &height);
    void EstimateCellSize(int &width, int &height);

    // constants
    // Space around coordinate system
    static const int txcanv_cospace;
    // Default space between columns
    static const int txcanv_colspace;
    // Border used in text canvas
    static const int txcanv_border;


protected:

    r_Ref<r_GMarray> mddObj;
    int stepx, stepy, dimMDD;
    int scrollX, scrollY;
    int coleft, cotop;
    int dim1, dim2;
    unsigned int freeDims;
    r_Point pt1, pt2;
    int numberBase;
    bool cosys;
    rviewBaseType baseType;
    unsigned int baseSize;
    wxBrush fore, back;
    wxPen pen;
    wxFont *font;
};



/*
 *  Class for displaying tables
 */

class rviewTable: public rviewDisplay
{
public:

    rviewTable(mdd_frame *mf, unsigned int flags=0);
    ~rviewTable(void);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);
    int newProjection(void);
    void newTableSize(void);
    virtual int openViewer(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;

    void OnSize(int w, int h);
    void OnMenuCommand(int id);

    // constants
    // Dimensions of additional wxText items (configs)
    static const int table_twidth;
    static const int table_theight;
    // Dimensions of check box
    static const int table_cwidth;
    static const int table_cheight;
    // Minimum dimensions of table
    static const int table_minwidth;
    static const int table_minheight;
    static const int table_ctrly;
    static const int table_totaly;


protected:

    void EstimateCellSize(int &width, int &height);

    virtual int saveView(FILE *fp);
    virtual int readView(const char *key, const char *value);
    virtual void loadViewFinished(void);

    void checkModeMenu(void);

    textCanvas *canvas;
    int stepx, stepy;
    int scrollx, scrolly;
    int fieldsx, fieldsy;
    int lastMode;
    unsigned int freeDims;
    bool cosys;
    rviewText *sxText, *syText;
    rviewCheckBox *csBox;
    int numberBase;

    static const char *view_StepSize;
    static const char *view_ScrollPos;
    static const char *view_CoSys;
    static const char *view_NumBase;
};



/*
 *  Class for displaying strings
 */

class rviewStringViewer: public rviewDisplay
{
public:

    rviewStringViewer(mdd_frame *mf, unsigned int flags=0);
    ~rviewStringViewer(void);

    int newProjection(void);
    virtual int openViewer(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;

    void OnSize(int w, int h);

    // constants
    static const int strview_msgheight;
    static const int strview_minwidth;
    static const int strview_minheight;
    static const int strview_ctrly;
    static const int strview_totaly;


protected:

    unsigned int freeDims;
    wxMessage *msgString;
};

#endif
