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
 *  Base class for all object viewers (rviewImage, rviewChart, rviewTable
 *  and rviewSound). Provides a frame with standard control widgets and
 *  menus, functions for parsing/advancing the projection string and
 *  initializing protected variables according to the MDD object that will
 *  be displayed.
 *
 *	COMMENTS:
 *			None
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

#include "rviewTypes.hh"

#include "labelManager.hh"

#include "rviewUtils.hh"
#include "rviewDisplay.hh"
#include "rviewPrefs.hh"
#include "rviewApp.hh"




/*
 *  Need a slightly more sophisticated panel than the default one
 */
class rviewDisplayPanel : public wxPanel
{
  public:
  rviewDisplayPanel(rviewDisplay *parent, int posx, int posy, int width, int height);
  virtual void OnEvent(wxMouseEvent &mevt);
  virtual void OnItemEvent(wxItem *item, wxMouseEvent &mevt);
};

rviewDisplayPanel::rviewDisplayPanel(rviewDisplay *parent, int posx, int posy, int width, int height) :
  wxPanel((wxWindow*)parent, posx, posy, width, height)
{
}

void rviewDisplayPanel::OnEvent(wxMouseEvent &mevt)
{
  rviewDisplay *disp = (rviewDisplay*)(GetParent());
  disp->childMouseEvent(this, mevt);
  wxPanel::OnEvent(mevt);
}

void rviewDisplayPanel::OnItemEvent(wxItem *item, wxMouseEvent &mevt)
{
  rviewDisplay *disp = (rviewDisplay*)(GetParent());
  disp->childMouseEvent(item, mevt);
  wxPanel::OnItemEvent(item, mevt);
}




/*
 *  Constants
 */

// window dimensions
const int rviewDisplay::display_width = 300;
const int rviewDisplay::display_height = 400;
const int rviewDisplay::display_cnvborder = 10;
const int rviewDisplay::display_border = 8;
const int rviewDisplay::display_scrstep = 8;
const int rviewDisplay::display_pgstep = 8;
const int rviewDisplay::display_cheight = 60;
const int rviewDisplay::display_pjheight = 50;
const int rviewDisplay::display_pjwidth = 100;
const int rviewDisplay::display_pbwidth = 40;
const int rviewDisplay::display_pbheight = 30;
const int rviewDisplay::display_minwidth = 4*(rviewDisplay::display_pbwidth + rviewDisplay::display_border);

// flags
const int rviewDisplay::display_flag_standalone = 1;
const int rviewDisplay::display_flag_update = 2;

// others
const int rviewDisplay::fixedNumberOfMenus = 2;
const unsigned int rviewDisplay::viewBuffSize = 1024;
const char *rviewDisplay::viewFileExtension = "*.rvw";

const char *rviewDisplay::view_HeaderLine = "# rView viewer snapshot";
const char *rviewDisplay::view_ViewerType = "viewerName";
const char *rviewDisplay::view_ProjString = "projString";
const char *rviewDisplay::view_WindowSize = "windowSize";



/*
 *  rviewDisplay members
 */

// The global display counter.
int rviewDisplay::displayCounter=0;

rviewDisplay::rviewDisplay(mdd_frame *mf, int es, unsigned int flags) : rviewFrame(NULL, "", 0, 0, display_width, display_height)
{
  int x, y, pw;
  r_Object *mo;
  char buffer[STRINGSIZE];

  if ((flags & display_flag_standalone & display_flag_update) != 0)
    RMDBGENTER(3, RMDebug::module_applications, "rviewDisplay", "rviewDisplay() [UA]")
  else if ((flags & display_flag_standalone) != 0) 
    RMDBGENTER(3, RMDebug::module_applications, "rviewDisplay", "rviewDisplay() [A]")
  else if ((flags & display_flag_update) != 0) 
    RMDBGENTER(3, RMDebug::module_applications, "rviewDisplay", "rviewDisplay() [U]")
       
  // Override by derived classes if an error occurs.
  // Can't use virtual functions in constructors!
  objectInitializedOK = TRUE;
  closeViewerCalled = FALSE;

  displayFlags = flags;
  mddObj = mf->mdd; extraSpace = es;
  totalCtrlHeight = display_cheight + extraSpace;
  rootTitle[0] = '\0';
  displayOperation = FALSE;	// used to avoid reentrancy on resize
  minViewX = 0; minViewY = 0; dimMode = -1;

  // Copy the current (global) display counter and increment it, thus creating a unique
  // ID for each display window.
  displayID = displayCounter++;
  qwindowID = -1;	// no query window

  projString[0] = '\0';

  baseSize = (int)(mddObj->get_type_length());
  // Determine base type
  mo = (r_Object*)(&(*mddObj));
  baseType = rviewGetBasetype(mo);

  interv = mddObj->spatial_domain();
  dimMDD = interv.dimension();

  mBar = NULL;
  ctrlPanel = NULL; project = NULL;

  GetClientSize(&x, &y);

  ctrlPanel = new rviewDisplayPanel(this, 0, 0, x, totalCtrlHeight);
  ctrlPanel->SetLabelPosition(wxVERTICAL);

  x -= 2*display_border; pw = x;
  if (pw > display_pjwidth) pw = display_pjwidth;
  project = new rviewText(ctrlPanel);
  projBut = new rviewButton(ctrlPanel);
  projMinus = new rviewButton(ctrlPanel, "-");
  advance = new rviewText(ctrlPanel);
  projPlus = new rviewButton(ctrlPanel, "+");
  typeMsg = new wxMessage(ctrlPanel, "");

  // Have to duplicate this code bit.
  sprintf(buffer, "%s: %s", lman->lookup("textBaseType"), rviewBaseTypes[baseType]);
  typeMsg->SetLabel(buffer);

  frameWidth=-1;
  frameWidth=-1;

  RMDBGEXIT(3, RMDebug::module_applications, "rviewDisplay", "rviewDisplay()");
}


rviewDisplay::~rviewDisplay(void)
{
  if((displayFlags & display_flag_standalone & display_flag_update) != 0)
    RMDBGEXIT(3, RMDebug::module_applications, "rviewDisplay", "~rviewDisplay() [UA]")
  else if ((displayFlags & display_flag_standalone) != 0) 
    RMDBGEXIT(3, RMDebug::module_applications, "rviewDisplay", "~rviewDisplay() [A]")
  else if ((displayFlags & display_flag_update) != 0) 
    RMDBGEXIT(3, RMDebug::module_applications, "rviewDisplay", "~rviewDisplay() [U]")

  // Update display dying, notify other frames (-> query windows)
  if ((displayFlags & display_flag_update) != 0)
  {
    user_event ue;
    ue.type = usr_update_closed;
    ue.data = (void*)(&mddObj);
    if (frameManager != NULL)
      frameManager->broadcastUserEvent(ue);
  }
  // If standalone free all memory.
  if ((displayFlags & display_flag_standalone) != 0)
  {
    mddObj.destroy();
  }
}


// notify the parent window that a viewer has been closed;
// called from viewer destructor.
void rviewDisplay::closeViewer(void)
{
  RMDBGONCE(3, RMDebug::module_applications, "rviewDisplay", "closeViewer()");

  if (!closeViewerCalled)
  {
    if (parentFrame != NULL)
    {
      mdd_frame mf;
      user_event ue;
      mf.flags = getViewerType(); mf.mdd = mddObj;
      ue.type = usr_viewer_closed; ue.data = (void*)(&mf);
      parentFrame->userEvent(ue);
    }
    closeViewerCalled = TRUE;
  }
}


const char *rviewDisplay::getFrameName(void) const
{
  return "rviewDisplay";
}

rviewFrameType rviewDisplay::getFrameType(void) const
{
  return rviewFrameTypeDisplay;
}


const r_Minterval &rviewDisplay::getVirtualDomain(void) const
{
  return interv;
}


// Must concentrate all functionality relying on virtual functions here rather than
// the constructor
int rviewDisplay::openViewer(void)
{
  int w, h, x, y;
  char buffer[STRINGSIZE];

  RMDBGONCE(3, RMDebug::module_applications, "rviewDisplay", "openViewer()");

  mBar = new wxMenuBar;

  wxMenu *menu = new wxMenu;
  menu->Append(MENU_DISP_DATA_INSERT, "");
  menu->Append(MENU_DISP_DATA_INSERTPRO, "");
  menu->Append(MENU_DISP_DATA_SAVE, "");
  fileMenuInitHook(menu);
  menu->Append(MENU_DISP_DATA_CLOSE, "");
  sprintf(buffer, "&%s", lman->lookup("menDispData"));
  mBar->Append(menu, buffer);

  menu = new wxMenu;
  menu->Append(MENU_DISP_VIEW_SAVE, "");
  menu->Append(MENU_DISP_VIEW_LOAD, "");
  viewMenuInitHook(menu);
  sprintf(buffer, "&%s", lman->lookup("menDispView"));
  mBar->Append(menu, buffer);

  menuBarInitHook();

  GetSize(&w, &h);
  //lastWidth = w; lastHeight = h;
  GetClientSize(&x, &y);
  mbarHeight = h - y;
  //cout << "mbar height " << mbarHeight << endl;

  SetMenuBar(mBar);

  newDBState(rmanClientApp::theApp()->ReadDBState());

  return 0;
}


void rviewDisplay::setModeDimension(int dim)
{
  bool enable;
  char adbuff[4];

  dimMode = dim; sprintf(adbuff, "%d", dim);
  advance->SetValue(adbuff);
  enable = (dimMDD > dimMode);
  projPlus->Enable(enable); projMinus->Enable(enable); advance->Enable(enable);
}


void rviewDisplay::label(void)
{
  char btbuf[STRINGSIZE];

  mBar->SetLabel(MENU_DISP_DATA_INSERT, lman->lookup("menDispDataIsrt"));
  mBar->SetLabel(MENU_DISP_DATA_INSERTPRO, lman->lookup("menDispDataIsrtPro"));
  mBar->SetLabel(MENU_DISP_DATA_SAVE, lman->lookup("menDispDataSave"));
  mBar->SetLabel(MENU_DISP_DATA_CLOSE, lman->lookup("menDispDataClose"));
  mBar->SetLabelTop(0, lman->lookup("menDispData"));

  mBar->SetLabel(MENU_DISP_VIEW_SAVE, lman->lookup("menDispViewSave"));
  mBar->SetLabel(MENU_DISP_VIEW_LOAD, lman->lookup("menDispViewLoad"));
  mBar->SetLabelTop(1, lman->lookup("menDispView"));

  project->SetLabel(lman->lookup("textProjString"));
  projBut->SetLabel(lman->lookup("textOK"));

  sprintf(btbuf, "%s: %s", lman->lookup("textBaseType"), rviewBaseTypes[baseType]);
  typeMsg->SetLabel(btbuf);
}


void rviewDisplay::newDBState(bool dbstate)
{
  mBar->Enable(MENU_DISP_DATA_INSERT, dbstate);
  mBar->Enable(MENU_DISP_DATA_INSERTPRO, dbstate);
}


int rviewDisplay::process(wxObject &obj, wxEvent &evt)
{
  int type = evt.GetEventType();

  if (((&obj == (wxObject*)project) && (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)) || ((&obj == (wxObject*)projBut) && (type == wxEVENT_TYPE_BUTTON_COMMAND)))
  {
    strcpy(projString, project->GetValue());
    newProjection();
    return 1;
  }
  if (((&obj == (wxObject*)projPlus) || (&obj == (wxObject*)projMinus)) && (type == wxEVENT_TYPE_BUTTON_COMMAND))
  {
    if (advanceProjection((&obj == (wxObject*)projPlus) ? 1 : -1) != 0)
    {
      strcpy(projString, project->GetValue());
      newProjection();
      return 1;
    }
  }
  return 0;
}


void rviewDisplay::OnSize(int w, int h)
{
  int x, y, pw, pos, minw, minh;
  float tw, th;

  RMDBGONCE(3, RMDebug::module_applications, "rviewDisplay", "OnSize(" << w << ", " << h << " )");

  if (!displayOperation)
  {
    project->GetTextExtent((const char*)(lman->lookup("textProjString")), &tw, &th, NULL, NULL, wxSWISS_FONT, FALSE);
    minw = display_minwidth + ((int)tw);
    if (minw < minViewX) minw = minViewX;
    minh = totalCtrlHeight + minViewY + mbarHeight;
    if ((w < minw) || (h < minh))
    {
      // Avoid infinite loops / reentrancy
      displayOperation = TRUE;
      if (w < minw) w = minw;
      if (h < minh) h = minh;
      //cout << "resize display frame to " << w << ", " << h << " / min " << minViewX << ", " << minViewY << endl;
      SetSize(-1, -1, w, h);
      displayOperation = FALSE;
    }
  }

  GetClientSize(&x, &y);

  ctrlPanel->SetSize(0, 0, x, totalCtrlHeight);
  pos = x - 3*display_pbwidth - display_border;
  pw = pos - 3*display_border - display_pbwidth;
  x -= 2*display_border;
  project->SetSize(display_border, display_border, pw, display_pjheight);
  y = display_border + (display_pjheight - display_pbheight);
  projBut->SetSize(2*display_border + pw, y, display_pbwidth, display_pbheight);
  projMinus->SetSize(pos, y, display_pbwidth, display_pbheight);
  advance->SetSize(pos + display_pbwidth, y - display_border, display_pbwidth, 4*display_pbheight/3);
  projPlus->SetSize(pos + 2*display_pbwidth, y, display_pbwidth, display_pbheight);

  // Default label font seems to be swiss font. Changing it doesn't appear to
  // have any effect.
  typeMsg->GetTextExtent((const char*)(typeMsg->GetLabel()), &tw, &th, NULL, NULL, wxSWISS_FONT, FALSE);
  typeMsg->SetSize(display_border + x - tw, display_border, tw, th);
}



void rviewDisplay::OnMenuCommand(int id)
{
  switch (id)
  {
    case MENU_DISP_DATA_INSERT:
      rmanClientApp::theApp()->insertMDD(mddObj);
      break;
    case MENU_DISP_DATA_INSERTPRO:
      {
	r_Minterval useInterv(dimMDD);
	int i;

	for (i=0; i<dimMDD; i++)
	  useInterv << r_Sinterval((r_Range)pt1[i], (r_Range)pt2[i]);

        rmanClientApp::theApp()->insertMDD(mddObj, NULL, &useInterv);
      }
      break;
    case MENU_DISP_DATA_SAVE:
      {
	char *prefDir = (char*)(prefs->filePath.ptr());
	char *s = wxFileSelector(lman->lookup("saveData"), (::wxDirExists(prefDir)) ? prefDir : NULL, NULL, NULL, "*", 0, this);

	if (s)
	{
	  FILE *fp;
	  size_t dataSize = (size_t)baseSize;
	  int i;

	  prefs->filePath = ::wxPathOnly(s);

          for (i=0; i<dimMDD; i++)
	  {
	    dataSize *= (interv[i].high() - interv[i].low() + 1);
	  }
	  if ((fp = fopen(s, "wb")) == NULL)
	  {
	    char buffer[STRINGSIZE];
	    sprintf(buffer, "%s %s", lman->lookup("errorFileOpen"), s);
	    rviewErrorbox::reportError(buffer, rviewDisplay::getFrameName(), "OnMenuCommand");
	  }
	  else
	  {
	    if (fwrite(mddObj->get_array(), 1, dataSize, fp) != dataSize)
	    {
	      char buffer[STRINGSIZE];
	      sprintf(buffer, "%s %s", lman->lookup("errorFileWrite"), s);
	      rviewErrorbox::reportError(buffer, rviewDisplay::getFrameName(), "OnMenuCommand");
	    }
	    fclose(fp);
	  }
	}
      }
      break;
    case MENU_DISP_DATA_CLOSE:
      prepareToDie(); this->Close(TRUE);
      break;
    case MENU_DISP_VIEW_SAVE:
      doSaveView();
      break;
    case MENU_DISP_VIEW_LOAD:
      doLoadView();
      break;
    default: break;
  }
}




int rviewDisplay::userEvent(const user_event &ue)
{
  if (ue.type == usr_mdd_dying)
  {
    if (*((r_Ref<r_GMarray>*)(ue.data)) == mddObj)
    {
      prepareToDie();
      this->Close(TRUE);
      return 1;
    }
  }
  if ((ue.type == usr_db_opened) || (ue.type == usr_db_closed))
  {
    newDBState((ue.type == usr_db_opened));
    return 1;
  }
  if (ue.type == usr_close_viewers)
  {
    Close(TRUE);
    return 1;
  }
  return 0;
}



// Virtual function, overload by derived classes if you need to.
void rviewDisplay::prepareToDie(void)
{
}

// Virtual function, overload by derived classes
int rviewDisplay::newProjection(void)
{
  return 0;
}

int rviewDisplay::fileMenuInitHook(wxMenu *menu)
{
  return 0;
}

int rviewDisplay::viewMenuInitHook(wxMenu *menu)
{
  return 0;
}

int rviewDisplay::menuBarInitHook(void)
{
  return 0;
}



// Support function for advanceProjection
const char *rviewDisplay::skipIndexMapping(const char *s)
{
  const char *b, *d;

  b = s+1;
  while ((*b == ' ') || (*b == '\t')) b++;
  d = b;
  while ((*b >= '0') && (*b <= '9')) b++;
  if (b == d) return NULL;
  while ((*b == ' ') || (*b == '\t')) b++;
  if (*b != ']') return NULL;
  return b+1;
}

/*
 *  Change the chosen fixed coordinate according to the advancement mode:
 *  relative:	coord += direction
 *  absolute:	coord = direction
 *  reset:	if (direction <= 0) then to start else to end
 */
int rviewDisplay::advanceProjection(int direction, int advmode)
{
  const char *b, *d;
  int cordnt;
  r_Range value;
  char tailbuff[STRINGSIZE];
  unsigned int freeDims=0;
  const char **dimDescs;
  const r_Minterval useIv = getVirtualDomain();

  if ((dimDescs = new const char*[dimMDD]) == NULL)
    return 0;

  // Find free dimensions in projection string
  strcpy(projString, project->GetValue());
  b = projString; value = 0;
  while (*b != '\0')
  {
    cordnt = 0;	// indicates it's a number
    while ((*b == ' ') || (*b == '\t')) b++;
    if (*b == '\0') break;
    if (value < dimMDD) dimDescs[value] = b;
    if (*b == '*')	// not a number
    {
      b++; cordnt=1;
    }
    else
    {
      if ((*b == '-') || (*b == '+')) b++;
      d = b;
      while ((*b >= '0') && (*b <= '9')) b++;
      if (b == d)
      {
	delete [] dimDescs; return 0;
      }
    }
    while (*b == ' ') b++;
    if (*b == '[')
    {
      if ((b = skipIndexMapping(b)) == NULL)
      {
	delete [] dimDescs; return 0;
      }
    }
    else if (*b == ':')
    {
      b++;
      while ((*b == ' ') || (*b == '\t')) b++;
      if (*b == '*') b++;
      else
      {
        if ((*b == '-') || (*b == '+')) b++;
	d = b;
        while ((*b >= '0') && (*b <= '9')) b++;
	if (b == d)
	{
	  delete [] dimDescs; return 0;
	}
      }
      while ((*b == ' ') || (*b == '\t')) b++;
      if (*b == '[')
      {
	if ((b = skipIndexMapping(b)) == NULL)
	{
	  delete [] dimDescs; return 0;
	}
      }
    }
    else if (cordnt == 0)
    {
      freeDims |= (1<<value);
    }
    while ((*b == ' ') || (*b == '\t')) b++;
    if (*b == ',') b++;
    value++;
  }
  if ((value != dimMDD) || (freeDims == 0))
  {
    delete [] dimDescs; return 0;
  }
  cordnt = atoi(advance->GetValue());
  // Is the specified coordinate a free one?
  if ((cordnt < 0) || (cordnt >= dimMDD) || ((freeDims & (1<<cordnt)) == 0))
  {
    for (cordnt=0; cordnt < (int)(8*sizeof(unsigned int)); cordnt++)
    {
      // freeDims is != 0  here.
      if ((freeDims & (1<<cordnt)) != 0) break;
    }
    sprintf(tailbuff, "%d", cordnt);
    advance->SetValue(tailbuff);
  }
  b = dimDescs[cordnt];
  value = atoi(b);

  // Save tail
  if (cordnt < dimMDD-1)
    sprintf(tailbuff, ", %s", dimDescs[cordnt+1]);
  else
    tailbuff[0] = '\0';

  // update the coordinate according to the advancement mode
  if (advmode == display_advmode_relative)
    value += direction;
  else if (advmode == display_advmode_absolute)
    value = direction;
  else if (advmode == display_advmode_reset)
  {
    if (direction <= 0)
      value = useIv[cordnt].low();
    else
      value = useIv[cordnt].high();
  }

  if ((value >= useIv[cordnt].low()) && (value <= useIv[cordnt].high()))
  {
    // this cast here is OK, doesn't compromise the function interface.
    sprintf((char*)b, "%ld%s", value, tailbuff);
    project->SetValue(projString);
    delete [] dimDescs;
    return 1;
  }
  delete [] dimDescs;
  return 0;
}



void rviewDisplay::setDisplayTitle(const char *title)
{
  char titleString[STRINGSIZE];
  char *b;

  if (title != NULL)
  {
    strcpy(rootTitle, title);
  }
  // Create the title postfix string
  strcpy(titleString, rootTitle);
  b = titleString + strlen(titleString);
  if ((displayFlags & (display_flag_standalone | display_flag_update)) != 0)
  {
    *b++ = ' '; *b++ = '[';
    if ((displayFlags & display_flag_standalone) != 0)
      b += sprintf(b, "StAln");
    if ((displayFlags & display_flag_update) != 0)
    {
      b += sprintf(b, "U");
      if (qwindowID != -1)
	b += sprintf(b, "d%dq%d", displayID, qwindowID);
      *b++ = ' ';
    }
    *b++ = ']';
  }
  *b++ = '\0';
  SetTitle(titleString);
}



void rviewDisplay::noLongerUpdate(void)
{
  displayFlags &= ~display_flag_update;
  setDisplayTitle();
}




int rviewDisplay::getIdentifier(void) const
{
  return displayID;
}


int rviewDisplay::getDisplayCounter(void) const
{
  return displayCounter;
}



void rviewDisplay::setQueryWindow(int qwid)
{
  qwindowID = qwid;
  setDisplayTitle();
}



void rviewDisplay::setMinimumViewerSize(int w, int h)
{
  minViewX = w; minViewY = h;
}



int rviewDisplay::doSaveView(void)
{
  char *name = ::wxFileSelector(lman->lookup("saveView"), (char*)(::wxDirExists(prefs->filePath.ptr()) ? prefs->filePath.ptr() : NULL), NULL, NULL, (char*)viewFileExtension);

  if (name != NULL)
  {
    FILE *fp = fopen(name, "w");
    if (fp != NULL)
    {
      fprintf(fp, "%s\n\n", view_HeaderLine);
      int status = saveView(fp);
      fclose(fp);
      return status;
    }
    return -1;
  }
  return 0;
}


int rviewDisplay::doLoadView(void)
{
  char *name = ::wxFileSelector(lman->lookup("loadView"), (char*)(::wxDirExists(prefs->filePath.ptr()) ? prefs->filePath.ptr() : NULL), NULL, NULL, (char*)viewFileExtension);

  if (name != NULL)
  {
    FILE *fp = fopen(name, "r");
    if (fp != NULL)
    {
      int status = parseViewFile(fp);
      fclose(fp);
      loadViewFinished();
      newProjection();
      return status;
    }
    return -1;
  }
  return 0;
}


int rviewDisplay::parseViewFile(FILE *fp)
{
  char *buffer = new char[viewBuffSize];
  unsigned int line = 0;
  int status = -1;

  if (fgets(buffer, viewBuffSize, fp) != NULL)
  {
    int len = strlen(buffer);
    line++;
    if (strncmp(view_HeaderLine, buffer, len-1) == 0)
    {
      while (!feof(fp))
      {
	if (fgets(buffer, viewBuffSize, fp) == NULL)
	  break;
	line++;
	char *b = buffer;
	while (isspace((unsigned int)(*b))) b++;
	if (*b != '\0')
	{
	  char *key = b;
	  while (isalnum((unsigned int)(*b))) b++;
	  char *kend = b;
	  while (isspace((unsigned int)(*b))) b++;
	  if (*b != '=')
	  {
	    cerr << "Missing '=' at line " << line << endl;
	  }
	  else
	  {
	    *kend = '\0'; b++;
	    while (isspace((unsigned int)(*b))) b++;
	    len = strlen(b);
	    while ((len > 0) && (isspace((unsigned int)(b[len-1])))) b[--len] = '\0';
	    //cout << "key " << key << ", value " << b << endl;
	    if (readView(key, b) < 0)
	    {
	      delete [] buffer;
	      return -1;
	    }
	  }
	}
      }
      status = 0;
    }
  }
  delete [] buffer;
  return status;
}


void rviewDisplay::loadViewFinished(void)
{
  project->SetValue(projString);
}


void rviewDisplay::writeViewKey(FILE *fp, const char *key)
{
  fprintf(fp, "%s\t=\t", key);
}


void rviewDisplay::writeViewParam(FILE *fp, const char *key, const char *value)
{
  fprintf(fp, "%s\t=\t%s\n", key, value);
}


void rviewDisplay::writeViewParam(FILE *fp, const char *key, long value)
{
  fprintf(fp, "%s\t=\t%ld\n", key, value);
}


void rviewDisplay::writeViewParam(FILE *fp, const char *key, double value)
{
  fprintf(fp, "%s\t=\t%.15f\n", key, value);
}


void rviewDisplay::writeViewParam(FILE *fp, const char *key, unsigned int num, const long *values)
{
  fprintf(fp, "%s\t=\t%ld", key, values[0]);
  for (unsigned int i=1; i<num; i++)
    fprintf(fp, ", %ld", values[i]);
  fprintf(fp, "\n");
}


void rviewDisplay::writeViewParam(FILE *fp, const char *key, unsigned int num, const double *values)
{
  fprintf(fp, "%s\t=\t%.15f", key, values[0]);
  for (unsigned int i=1; i<num; i++)
    fprintf(fp, ", %.15f", values[i]);
  fprintf(fp, "\n");
}


int rviewDisplay::readVector(const char *value, unsigned int num, long *values)
{
  unsigned int i = 0;
  const char *b, *rest;

  b = value;
  while (i < num)
  {
    while (isspace((unsigned int)(*b))) b++;
    values[i] = strtol(b, (char**)&rest, 10);
    if (rest == b)
      return -1;
    b = rest; i++;
    while (isspace((unsigned int)(*b))) b++;
    if (*b == ',') b++;
  }
  return 0;
}


int rviewDisplay::readVector(const char *value, unsigned int num, double *values)
{
  unsigned int i = 0;
  const char *b, *rest;

  b = value;
  while (i < num)
  {
    while (isspace((unsigned int)(*b))) b++;
    values[i] = strtod(b, (char**)&rest);
    if (rest == b)
      return -1;
    b = rest; i++;
    while (isspace((unsigned int)(*b))) b++;
    if (*b == ',') b++;
  }
  return 0;
}


int rviewDisplay::saveView(FILE *fp)
{
  writeViewParam(fp, view_ViewerType, getFrameName());
  writeViewParam(fp, view_ProjString, projString);
  GetSize(&frameWidth, &frameHeight);
  long wsize[2];
  wsize[0] = (long)frameWidth; wsize[1] = (long)frameHeight;
  writeViewParam(fp, view_WindowSize, 2, wsize);
  return 0;
}


int rviewDisplay::readView(const char *key, const char *value)
{
  if (strcmp(key, view_ViewerType) == 0)
  {
    if (strcmp(value, getFrameName()) != 0)
    {
      char buffer[STRINGSIZE];
      sprintf(buffer, "%s: %s", lman->lookup("errorViewType"), value);
      rviewErrorbox::reportError(buffer, getFrameName(), "readView");
      return -1;
    }
    return 1;
  }
  else if (strcmp(key, view_ProjString) == 0)
  {
    strcpy(projString, value);
    return 1;
  }
  else if (strcmp(key, view_WindowSize) == 0)
  {
    long wsize[2];
    if (readVector(value, 2, wsize) == 0)
    {
      SetSize(wsize[0], wsize[1]);
      return 1;
    }
  }
  return 0;
}
