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
 *  rView query shell. Handles editing, loading, saving and execution
 *  of RasQL queries. Actual database accesses are performed through
 *  class rView's member functions.
 *
 *	COMMENTS:
 *		None
 */



// Standard wxWindows preamble.
#ifdef __GNUG__
#pragma implementation
#endif


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

#include "rviewApp.hh"
#include "rviewUtils.hh"
//#include "rviewDb.hh"
#include "rviewQuery.hh"
#include "rviewPrefs.hh"




const int rviewQuery::query_width = 500;
const int rviewQuery::query_height = 300;
const int rviewQuery::query_border = 8;
const int rviewQuery::query_bottom = 50;
const int rviewQuery::query_bwidth = 80;
const int rviewQuery::query_bheight = 40;




const char rviewQuery::query_extension[] = ".ql";
const char rviewQuery::query_firstline[] = "-- rview-Query";



// The Query window counter
int rviewQuery::queryCounter = 0;


// Lookup tables for fonts. These have to be sorted case-sensitively!
const keyword_to_ident_c rviewQuery::fontNameTab[] = {
  {wxDECORATIVE, "decorative"},
  {wxDEFAULT,	"default"},
  {wxMODERN,	"modern"},
  {wxROMAN,	"roman"},
  {wxSCRIPT,	"script"},
  {wxSWISS,	"swiss"},
  {wxTELETYPE,	"teletype"}
};
const keyword_to_ident_c rviewQuery::fontStyleTab[] = {
  {wxITALIC,	"italic"},
  {wxSLANT,	"slant"}
};

const keyword_to_ident_c rviewQuery::fontWeightTab[] = {
  {wxBOLD,	"bold"},
  {wxLIGHT,	"light"}
};




rviewQuery::rviewQuery(rviewDatabase *db, char *query) : rviewFrame(NULL, NULL, 0, 0, query_width, query_height)
{
  int w, h;
  const char *b;
  char *d;
  char buffer[STRINGSIZE];
  int fontSize, fontName, fontStyle, fontWeight;
  const int fontNameSize = sizeof(fontNameTab) / sizeof(keyword_to_ident);
  const int fontStyleSize = sizeof(fontStyleTab) / sizeof(keyword_to_ident);
  const int fontWeightSize = sizeof(fontWeightTab) / sizeof(keyword_to_ident);

 RMDBGENTER(3, RMDebug::module_applications, "rviewQuery", "rviewQuery()");

  queryDb = db;
  updateDisplay = NULL;

  CreateStatusLine(1);

  mbar = NULL;
  for (w=0; w<3; w++) mbarMenus[w] = NULL;

  if (::wxDirExists((char*)(prefs->queryPath.ptr())))
    hotPath = prefs->queryPath;
  else
    hotPath = ".";

  // Read font from prefs
  b = prefs->queryFont;
  fontSize = 12; fontName = wxDEFAULT; fontStyle = wxNORMAL; fontWeight = wxNORMAL;
  while (*b != 0)
  {
    while ((*b == ' ') || (*b == ',')) b++;
    if (*b == 0) break;
    d = buffer;
    while ((*b != ' ') && (*b != ',') && (*b != 0)) {*d++ = *b++;}
    *d++ = 0;
    w = atoi(buffer);
    if (w != 0) fontSize = w;
    else
    {
      if ((w = rviewLookupKeyword(buffer, fontNameTab, fontNameSize, FALSE)) >= 0)
	fontName = w;
      else if ((w = rviewLookupKeyword(buffer, fontStyleTab, fontStyleSize, FALSE)) >= 0)
	fontStyle = w;
      else if ((w = rviewLookupKeyword(buffer, fontWeightTab, fontWeightSize, FALSE)) >= 0)
	fontWeight = w;
      else
	cerr << "Bad identifier in font string: " << buffer << endl;
    }
  }
  //cout << "size " << fontSize << ", name " << fontName << ", style " << fontStyle << ", weight " << fontWeight << endl;

  font = new wxFont(fontSize, fontName, fontStyle, fontWeight);
  //font = ::wxTheFontList->FindOrCreateFont(fontSize, fontName, fontStyle, fontWeight);
  //cout << "Font " << font->GetPointSize() << ',' << font->GetFamily() << ',' << font->GetStyle() << endl;

  GetClientSize(&w, &h);

  // Set identifier
  qwindowID = queryCounter++;
  updateID = -1;	// no update object

  w -= 2*query_border; h -= query_bottom;
  twin = new wxTextWindow((wxWindow*)this, query_border, query_border, w, h - 2*query_border, wxBORDER | wxNATIVE_IMPL);

  twin->SetFont(font);

  panel = new wxPanel((wxWindow*)this, query_border, h, w, query_bottom);
  butClear = new rviewButton(panel);
  butExec = new rviewButton(panel);
  butUpdt = new rviewButton(panel);

  if (query != NULL)
  {
    twin->WriteText(query);
  }

  buildMenubar();

  newDBState(rmanClientApp::theApp()->ReadDBState());

  label();

  OnSize(w, h);

  Show(TRUE);
}


rviewQuery::~rviewQuery(void)
{
  // If an update window is open notify it.
  if (updateDisplay != NULL)
  {
    updateDisplay->noLongerUpdate();
  }
}


const char *rviewQuery::getFrameName(void) const
{
  return "rviewQuery";
}

rviewFrameType rviewQuery::getFrameType(void) const
{
  return rviewFrameTypeQuery;
}


void rviewQuery::OnSize(int w, int h)
{
  int x, y;

  GetClientSize(&x, &y);
  if ((frameWidth == x) && (frameHeight == y)) return;
  frameWidth = x; frameHeight = y;

  x -= 2*query_border; y -= query_bottom;
  twin->SetSize(query_border, query_border, x, y - 2*query_border);

  panel->SetSize(query_border, y, x, query_bottom);

  y = (query_bottom - query_bheight) / 2;

  x = (x - 3*query_bwidth) / 3;
  butClear->SetSize(x/2, y, query_bwidth, query_bheight);
  butExec->SetSize((3*x)/2 + query_bwidth, y, query_bwidth, query_bheight);
  butUpdt->SetSize((5*x)/2 + 2*query_bwidth, y, query_bwidth, query_bheight);

  // doesn't work, unfortunately
  /*if (mbar != NULL)
  {
    int hw, hh;

    mbar->GetSize(&x, &y);
    mbarMenus[3]->GetSize(&hw, &hh);
    mbarMenus[3]->SetSize(x-hw, (y-hh)/2, hw, hh);
  }*/
}


void rviewQuery::OnMenuCommand(int id)
{
  // Load query from hotlist?
  if ((id >= MENU_QUERY_HOTLIST) && (id < MENU_QUERY_HOTLIST + hotNumber))
  {
    char buffer[STRINGSIZE];

    sprintf(buffer, "%s"DIR_SEPARATOR"%s%s", hotPath.ptr(), mbar->GetLabel(id), query_extension);
    loadQuery(buffer);
    return;
  }

  switch (id)
  {
    case MENU_QUERY_FILE_OPEN:
    case MENU_QUERY_FILE_SAVE:
      {
	char *name, *aux;
	char *prefDir = (char*)(hotPath.ptr());
	char filter[16];

	sprintf(filter, "*%s", query_extension);
	if (::wxDirExists(prefDir)) name = prefDir; else name = ".";
	name = ::wxFileSelector(lman->lookup((id == MENU_QUERY_FILE_OPEN) ? "titleQueryLoad" : "titleQuerySave"), name, NULL, NULL, filter, 0 , this);
	if (name != NULL)
	{
	  aux = ::wxFileNameFromPath(name);
	  if (strlen(aux) > 0)
	  {
	    if (id == MENU_QUERY_FILE_OPEN)
	    {
	      RMDBGONCE(3, RMDebug::module_applications, "rviewQuery", "loadQuery()");
	      loadQuery(name);
	    }
	    else
	    {
	      RMDBGONCE(3, RMDebug::module_applications, "rviewQuery", "saveQuery()");
	      saveQuery(name);
	    }
	    hotPath = ::wxPathOnly(name);
	    prefs->queryPath = hotPath;
	    prefs->markModified();
	    buildMenubar();
	  }
	}
      }
      break;
    case MENU_QUERY_FILE_EXIT: this->Close(TRUE); break;
    case MENU_QUERY_EDIT_CUT: twin->Cut(); break;
    case MENU_QUERY_EDIT_COPY: twin->Copy(); break;
    case MENU_QUERY_EDIT_PASTE: twin->Paste(); break;
    default: break;
  }
}



void rviewQuery::buildMenubar(void)
{
  char pattern[STRINGSIZE], leaf[STRINGSIZE];
  char *f;
  char *qhots[MENU_QUERY_HOTRANGE];
  int i;
  wxMenu *hotList;

  // Do we actually need to rebuild the menu?
  if ((mbar != NULL) && (strcmp(hotPath, lastHotPath) == 0)) return;

  // build hotlist menu
  hotList = new wxMenu;
  sprintf(pattern, "%s"DIR_SEPARATOR"*%s", hotPath.ptr(), query_extension);

  // Sort the hotlist alphabetically
  f = ::wxFindFirstFile(pattern); hotNumber = 0;
  while (f && (hotNumber < MENU_QUERY_HOTRANGE))
  {
    strcpy(leaf, ::wxFileNameFromPath(f));
    f = strstr(leaf, query_extension);
    if (f != NULL) *f = '\0';
    if ((qhots[hotNumber] = new char[strlen(leaf) + 1]) == NULL) break;
    strcpy(qhots[hotNumber], leaf);
    hotNumber++;
    f = wxFindNextFile();
  }

  rviewQuicksortStrings(qhots, 0, hotNumber-1);

  for (i=0; i<hotNumber; i++)
  {
    //cout << "Item " << i << ": " << qhots[i] << endl;
    hotList->Append(MENU_QUERY_HOTLIST + i, qhots[i]);
    delete [] qhots[i];
  }

  if (mbar == NULL)
  {
    mbarMenus[0] = new wxMenu;
    mbarMenus[0]->Append(MENU_QUERY_FILE_OPEN, "");
    mbarMenus[0]->Append(MENU_QUERY_FILE_SAVE, "");
    mbarMenus[0]->Append(MENU_QUERY_FILE_EXIT, "");

    mbarMenus[1] = new wxMenu;
    mbarMenus[1]->Append(MENU_QUERY_EDIT_CUT, "");
    mbarMenus[1]->Append(MENU_QUERY_EDIT_COPY, "");
    mbarMenus[1]->Append(MENU_QUERY_EDIT_PASTE, "");

    mbarMenus[2] = hotList;


    mbar = new wxMenuBar;
    sprintf(pattern, "&%s", lman->lookup("menQueryFile"));
    mbar->Append(mbarMenus[0], pattern);
    sprintf(pattern, "&%s", lman->lookup("menQueryEdit"));
    mbar->Append(mbarMenus[1], pattern);
    sprintf(pattern, "&%s", lman->lookup("menQueryHotlist"));
    mbar->Append(mbarMenus[2], pattern);

    SetMenuBar(mbar);
  }
  else
  {
    mbar->Delete(mbarMenus[2], 2);
    sprintf(pattern, "&%s", lman->lookup("menQueryHotlist"));
    mbar->Append(hotList, pattern);
    delete mbarMenus[2];
    mbarMenus[2] = hotList;
  }
}



void rviewQuery::label(void)
{
  updateTitle();

  mbar->SetLabel(MENU_QUERY_FILE_OPEN, lman->lookup("menQueryFileOpen"));
  mbar->SetLabel(MENU_QUERY_FILE_SAVE, lman->lookup("menQueryFileSave"));
  mbar->SetLabel(MENU_QUERY_FILE_EXIT, lman->lookup("menQueryFileClose"));
  mbar->SetHelpString(MENU_QUERY_FILE_OPEN, lman->lookup("helpQueryFileOpen"));
  mbar->SetHelpString(MENU_QUERY_FILE_SAVE, lman->lookup("helpQueryFileSave"));
  mbar->SetHelpString(MENU_QUERY_FILE_EXIT, lman->lookup("helpQueryFileClose"));

  mbar->SetLabel(MENU_QUERY_EDIT_CUT, lman->lookup("menQueryEditCut"));
  mbar->SetLabel(MENU_QUERY_EDIT_COPY, lman->lookup("menQueryEditCopy"));
  mbar->SetLabel(MENU_QUERY_EDIT_PASTE, lman->lookup("menQueryEditPaste"));
  mbar->SetHelpString(MENU_QUERY_EDIT_CUT, lman->lookup("helpQueryEditCut"));
  mbar->SetHelpString(MENU_QUERY_EDIT_COPY, lman->lookup("helpQueryEditCopy"));
  mbar->SetHelpString(MENU_QUERY_EDIT_PASTE, lman->lookup("helpQueryEditPaste"));

  mbar->SetLabelTop(0, lman->lookup("menQueryFile"));
  mbar->SetLabelTop(1, lman->lookup("menQueryEdit"));
  mbar->SetLabelTop(2, lman->lookup("menQueryHotlist"));

  butClear->SetLabel(lman->lookup("textClear"));
  butExec->SetLabel(lman->lookup("textExec"));
  butUpdt->SetLabel(lman->lookup("textUpdt"));
}


void rviewQuery::updateTitle(void)
{
  if (updateID == -1)
  {
    SetTitle(lman->lookup("titleQuery"));
  }
  else
  {
    char buffer[STRINGSIZE];
    sprintf(buffer, "%s q%dd%d", lman->lookup("titleQuery"), qwindowID, updateID);
    SetTitle(buffer);
  }
}


int rviewQuery::process(wxObject &obj, wxEvent &evt)
{
  int type = evt.GetEventType();

  if (type == wxEVENT_TYPE_BUTTON_COMMAND)
  {
    if (&obj == (wxObject*)butClear)
    {
      twin->Clear();
      return 1;
    }
    if (&obj == (wxObject*)butExec)
    {
      int numl = twin->GetNumberOfLines();
      int i, totalLength = 1;
      char *b, *query;

      query = twin->GetContents();

      // Execute query.
      i = rmanClientApp::theApp()->executeQuery(query, (updateDisplay == NULL) ? NULL : &updateMddObj);
      // Did it fail?
      if (i == 0)
      {
	int line, column;

	// Was it a query error?
	if (queryDb->getErrorInfo(line, column) != 0)
	{
	  long offset = twin->XYToPosition(column-1, line-1);

	  // query was big enough to hold the entore query, so it's definitely
	  // big enough to hold one line
	  twin->GetLineText(line-1, query);
	  b = query + column-1;
	  while ((*b != ' ') && (!iscntrl(*b))) b++;
	  twin->SetSelection(offset, offset + (long)(b - (query + column-1)));
	}
      }
      delete [] query;
      // Notify that we did something, not whether it was a success
      return 1;
    }
    if (&obj == (wxObject*)butUpdt)
    {
      rviewDisplay *newDisplay;

      // Don't set updateDisplay directly. We might have a valid update image and
      // issued a cancel from the file selection box.
      if ((newDisplay = (rviewDisplay*)rmanClientApp::theApp()->OpenFile(rviewDisplay::display_flag_update, &updateMddObj, FALSE)) != NULL)
      {
	if (updateDisplay != NULL)
	{
	  updateDisplay->noLongerUpdate();
	}
	updateDisplay = newDisplay;
	updateDisplay->setQueryWindow(qwindowID);
	updateID = updateDisplay->getIdentifier();
	updateTitle();
        return 1;
      }
    }
  }
  return 0;
}


int rviewQuery::getIdentifier(void) const
{
  return qwindowID;
}

int rviewQuery::getQueryCounter(void) const
{
  return queryCounter;
}



int rviewQuery::userEvent(const user_event &ue)
{
  if ((ue.type == usr_db_opened) || (ue.type == usr_db_closed))
  {
    newDBState((ue.type == usr_db_opened));
    return 1;
  }
  if ((updateDisplay != NULL) && (ue.type == usr_update_closed))
  {
    r_Ref<r_GMarray> *whichMdd = (r_Ref<r_GMarray>*)(ue.data);
    if (*whichMdd == updateMddObj)
    {
      updateDisplay = NULL; updateID = -1;
      updateTitle();
      return 1;
    }
  }
  return 0;
}


void rviewQuery::newDBState(bool newState)
{
  butExec->Enable(newState);
}


bool rviewQuery::loadQuery(char *file)
{
  size_t length;
  int idlength;
  char *buffer, *b;
  FILE *fp;

  if ((fp = fopen(file, "rb")) == NULL)
  {
    char msg[STRINGSIZE];
    sprintf(msg, "%s %s", lman->lookup("errorFileOpen"), file);
    rviewErrorbox::reportError(msg, rviewQuery::getFrameName(), "loadQuery");
    return FALSE;
  }

  fseek(fp, 0, SEEK_END);
  length = ftell(fp);
  if ((buffer = new char[length+1]) == NULL)
  {
    rviewErrorbox::reportError(lman->lookup("errorMemory"), rviewQuery::getFrameName(), "loadQuery");
    fclose(fp); return FALSE;
  }
  fseek(fp, 0, SEEK_SET);

  b = buffer;
  for (idlength=0; idlength<(int)length; idlength++)
  {
    int c;

	if ((c = fgetc(fp)) == EOF) break;
#ifdef __VISUALC__
	if (c != '\r')
#endif
	*b++ = c;
  }
  *b++ = '\0';

  if (idlength < (int)length)
  {
    char msg[STRINGSIZE];
    sprintf(msg, "%s %s", lman->lookup("errorFileRead"), file);
    rviewErrorbox::reportError(msg, rviewQuery::getFrameName(), "loadQuery");
    fclose(fp);
    return FALSE;
  }

  fclose(fp);

  idlength = strlen("RasDaView-Query");
  if (strncmp(buffer, "RasDaView-Query", idlength) != 0)
  {
    idlength = strlen(query_firstline);
    if (strncmp(buffer,query_firstline, idlength) != 0) idlength = -1;
  }
  if (idlength < 0)
  {
    rviewErrorbox::reportError(lman->lookup("errorQueryFile"), rviewQuery::getFrameName(), "loadQuery");
    delete [] buffer;
    return FALSE;
  }
  twin->Clear();
  twin->WriteText(buffer + idlength + 1);
  delete [] buffer;
  return TRUE;
}


bool rviewQuery::saveQuery(char *file)
{
  FILE *fp;
  char *buffer, *b, *upper;

  if ((fp = fopen(file, "wb")) == NULL)
  {
    char msg[STRINGSIZE];
    sprintf(msg, "%s %s", lman->lookup("errorFileOpen"), file);
    rviewErrorbox::reportError(msg, rviewQuery::getFrameName(), "saveQuery");
    return FALSE;
  }

  fprintf(fp, query_firstline); fputc('\n', fp);
  buffer = twin->GetContents();
  b = buffer; upper = buffer + strlen(buffer);

  while (b < upper)
  {
#ifdef __VISUALC__
    if (*b != '\r')
#endif
    if (fputc(*b, fp) != *b) break;
    b++;
  }
  if (b != upper)
  {
    char msg[STRINGSIZE];
    sprintf(msg, "%s %s", lman->lookup("errorFileWrite"), file);
    rviewErrorbox::reportError(msg, rviewQuery::getFrameName(), "saveQuery");
    fclose(fp);
    return FALSE;
  }

  fclose(fp);

  delete [] buffer;

  return TRUE;
}
