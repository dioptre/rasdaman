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
 *  rView main application (class rView) and main frame (class rviewMainFrame).
 *  The main application includes the database object which has to be accessed
 *  through rView-member functions.
 *
 *  COMMENTS:
 *       none
 */

// #ifdef __GNUG__
// #pragma implementation
// #endif

// # define __GNUC__
// # define __GNUC_MINOR__

// changed in wxWindows 2.6.2:
// #include "wx_prec.h"
#include <wx/wxprec.h>


#ifdef __BORLANDC__
#pragma hdrstop
#endif


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// #include "wb_timer.h"


#include <stdlib.h>
#include <string.h>
#include <sstream>


#include "labelManager.hh"


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif

#include "raslib/rmdebug.hh"
#define DEBUG_MAIN
#include "debug/debug.hh"

#include "rviewUtils.hh"
#include "rview.hh"
#include "rviewQuery.hh"




const int rviewMainFrame::main_width = 300;
const int rviewMainFrame::main_height = 400 + 2*rview_window_extra_height;
const int rviewMainFrame::main_border = 8;
const int rviewMainFrame::main_theight = 50;
const int rviewMainFrame::main_bwidth = 60;
const int rviewMainFrame::main_bheight = 30;




/*
 *  Global data
 */


// Start the app.
static rView rview;




// For some compilers...
IMPLEMENT_WXWIN_MAIN




#if 0
#include "rviewTypeMan.hh"
static void testTypeManager(void)
{
    r_Type *newType = r_Type::get_any_type("struct {char red, struct {short s1, long l1}, struct {short s2, long l2}, char green, char blue, struct {float u, float v, float w}}");
    new rviewTypeMan(NULL, newType);
    delete newType;
}
#endif






/*
 *  Labels all the widgets and menus in the main frame using the label manager
 *  to lookup the values associated with the symbolic names.
 */

void rviewMainFrame::label(void)
{
    mBar->SetLabel(MENU_MAIN_FILE_QUERY, lman->lookup("menMainFileQuery"));
    mBar->SetLabel(MENU_MAIN_FILE_PREFS, lman->lookup("menMainFilePrefs"));
    mBar->SetLabel(MENU_MAIN_FILE_EXIT, lman->lookup("menMainFileExit"));
    mBar->SetHelpString(MENU_MAIN_FILE_QUERY, lman->lookup("helpMainFileQuery"));
    mBar->SetHelpString(MENU_MAIN_FILE_PREFS, lman->lookup("helpMainFilePrefs"));
    mBar->SetHelpString(MENU_MAIN_FILE_EXIT, lman->lookup("helpMainFileExit"));

    mBar->SetLabel(MENU_MAIN_VIEW_OPEN, lman->lookup("menMainViewOpen"));
    mBar->SetLabel(MENU_MAIN_VIEW_CLOSE, lman->lookup("menMainViewClose"));
    mBar->SetHelpString(MENU_MAIN_VIEW_OPEN, lman->lookup("helpMainViewOpen"));
    mBar->SetHelpString(MENU_MAIN_VIEW_CLOSE, lman->lookup("helpMainViewClose"));

    mBar->SetLabel(MENU_MAIN_COLL_LOOK, lman->lookup("menMainCollLook"));
    mBar->SetLabel(MENU_MAIN_COLL_LKSCL, lman->lookup("menMainCollLkScale"));
    mBar->SetLabel(MENU_MAIN_COLL_LKORTH, lman->lookup("menMainCollLkOrtho"));
    //FIXME create collection not implemented yet
    //  mBar->SetLabel(MENU_MAIN_COLL_CREATE, lman->lookup("menMainCollCrt"));
    mBar->SetLabel(MENU_MAIN_COLL_DELETE, lman->lookup("menMainCollDel"));
    mBar->SetHelpString(MENU_MAIN_COLL_LOOK, lman->lookup("helpMainCollLook"));
    mBar->SetHelpString(MENU_MAIN_COLL_LKSCL, lman->lookup("helpMainCollLkScale"));
    mBar->SetHelpString(MENU_MAIN_COLL_LKORTH, lman->lookup("helpMainCollLkOrtho"));
    //mBar->SetHelpString(MENU_MAIN_COLL_CREATE, lman->lookup("helpMainCollCrt"));
    mBar->SetHelpString(MENU_MAIN_COLL_DELETE, lman->lookup("helpMainCollDel"));

    mBar->SetLabel(MENU_MAIN_HELP_ABOUT, lman->lookup("menMainHelpAbt"));
    mBar->SetHelpString(MENU_MAIN_HELP_ABOUT, lman->lookup("helpMainHelpAbt"));

    mBar->SetLabelTop(0, lman->lookup("menMainFile"));
    mBar->SetLabelTop(1, lman->lookup("menMainView"));
    mBar->SetLabelTop(2, lman->lookup("menMainColl"));
    mBar->SetLabelTop(3, lman->lookup("menMainHelp"));

    server->SetLabel(lman->lookup("serverName"));
    port->SetLabel(lman->lookup("serverPort"));
    database->SetLabel(lman->lookup("dbName"));
    username->SetLabel(lman->lookup("userName"));
    userpassword->SetLabel(lman->lookup("userPassword"));

    openBut->SetLabel(lman->lookup( (dbOpen) ? "textClose" : "textOpen"));
    openBut->SetSize(-1, -1, 80, 30);
}




/*
 *  This is called via the frame manager. Events are broadcast to all the
 *  existing frames. If a frame recognises the object as one of its
 *  children it must return a value != 0.
 */

int rviewMainFrame::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();

    //cout << "rviewMainFrame: Received event" << endl;

    if ((((&obj == (wxObject*)server) || (&obj == (wxObject*)database) ||
            (&obj == (wxObject*)username)|| (&obj == (wxObject*)userpassword) ||
            (&obj == (wxObject*)port))
            && (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND))
            || ((&obj == (wxObject*)openBut) && (type == wxEVENT_TYPE_BUTTON_COMMAND)))
    {
        rView *app = (rView*)rmanClientApp::theApp();
        app->OpenCloseServer();
        return 1;
    }
    return 0;
}





/*
 *  Initialises the main window's frame, containing the main menus and the
 *  widgets for server and database.
 */

rviewMainFrame::rviewMainFrame(wxFrame *frame, char *title, int x, int y, int w, int h): rviewFrame(frame, title, x, y, w, h)
{
    wxMenu *mbarMenus[4];
    char buffer[STRINGSIZE];

    aboutWindow = NULL;

    CreateStatusLine(1);
    dbOpen = FALSE;

    // Create the menus and the menu bar. All labelling is done in the label() member function.
    mbarMenus[0] = new wxMenu;
    mbarMenus[0]->Append(MENU_MAIN_FILE_QUERY, "");
    mbarMenus[0]->Append(MENU_MAIN_FILE_PREFS, "");
    mbarMenus[0]->Append(MENU_MAIN_FILE_EXIT, "");

    mbarMenus[1] = new wxMenu;
    mbarMenus[1]->Append(MENU_MAIN_VIEW_OPEN, "");
    mbarMenus[1]->Append(MENU_MAIN_VIEW_CLOSE, "");

    mbarMenus[2] = new wxMenu;
    mbarMenus[2]->Append(MENU_MAIN_COLL_LOOK, "");
    mbarMenus[2]->Append(MENU_MAIN_COLL_LKSCL, "");
    mbarMenus[2]->Append(MENU_MAIN_COLL_LKORTH, "");
    //FIXME create collection not implemented yet
    //mbarMenus[2]->Append(MENU_MAIN_COLL_CREATE, "");
    mbarMenus[2]->Append(MENU_MAIN_COLL_DELETE, "");

    mbarMenus[3] = new wxMenu;
    mbarMenus[3]->Append(MENU_MAIN_HELP_ABOUT, "");

    // Note: the titles will be overwritten later on by label() but we need some text
    // here for keyboard-shortcuts and correct alignment (help flush right).
    // See wxWindows documentation: the submenus mustn't be used any more after they
    // were appended to the MenuBar. They are still accessible via the MenuBar through
    // their id, however.
    mBar = new wxMenuBar;
    sprintf(buffer, "&%s", lman->lookup("menMainFile"));
    mBar->Append(mbarMenus[0], buffer);
    sprintf(buffer, "&%s", lman->lookup("menMainView"));
    mBar->Append(mbarMenus[1], buffer);
    sprintf(buffer, "&%s", lman->lookup("menMainColl"));
    mBar->Append(mbarMenus[2], buffer);
    sprintf(buffer, "&%s", lman->lookup("menMainHelp"));
    mBar->Append(mbarMenus[3], buffer);

    // create panel
    panel = new wxPanel((wxWindow*)this, main_border, main_border, 256, 100, wxBORDER);
    panel->SetLabelPosition(wxVERTICAL);

    // Create the text widgets
    server = new rviewText(panel);
    panel->NewLine();
    port = new rviewText(panel);
    panel->NewLine();
    database = new rviewText(panel);
    panel->NewLine();
    username = new rviewText(panel);
    panel->NewLine();
    userpassword = new rviewText(wxTE_PASSWORD, panel);
    panel->NewLine();
    // ... and the button
    openBut = new rviewButton(panel);

    SetMenuBar(mBar);

    newDBState(dbOpen);

    label();

    // Resize everything correctly
    frameWidth = -1;
    frameHeight = -1;    // force resize

    OnSize(w, h);
    OnSize(w, h); //twice is done correct

}


const char *rviewMainFrame::getFrameName(void) const
{
    return "rviewMainFrame";
}

rviewFrameType rviewMainFrame::getFrameType(void) const
{
    return rviewFrameTypeMain;
}



void rviewMainFrame::newDBState(bool newState)
{
    openBut->SetLabel(lman->lookup( (newState) ? "textClose" : "textOpen"));
    openBut->SetSize(-1, -1, main_bwidth, main_bheight);
#ifndef DUMMY_MDD_OBJECT
    mBar->Enable(MENU_MAIN_COLL_LOOK, newState);
#endif
    mBar->Enable(MENU_MAIN_COLL_LKSCL, newState);
    mBar->Enable(MENU_MAIN_COLL_LKORTH, newState);
    //FIXME create collection not implemented yet
    //mBar->Enable(MENU_MAIN_COLL_CREATE, newState);
    mBar->Enable(MENU_MAIN_COLL_DELETE, newState);

    //set editable mode foe dbOpen
    server->SetEditable(!newState);
    port->SetEditable(!newState);
    database->SetEditable(!newState);
    username->SetEditable(!newState);
    userpassword->SetEditable(!newState);
    //reset to default when database is closed
    if(!newState)
        userpassword->SetValue("");

    dbOpen = newState;
}


int rviewMainFrame::userEvent(const user_event &ue)
{
    if ((ue.type == usr_db_opened) || (ue.type == usr_db_closed))
    {
        newDBState((ue.type == usr_db_opened));
        return 1;
    }
    return 0;
}




/*
 *  Dummy desctructor.
 */
rviewMainFrame::~rviewMainFrame(void)
{
}



/*
 *  Called from wxWindows when a menu item in the main window is selected.
 */

void rviewMainFrame::OnMenuCommand(int id)
{
    switch (id)
    {
    case MENU_MAIN_FILE_EXIT:
    {
        if (frameManager != NULL)
        {
            if (frameManager->broadcastQuit(0) == 0)
            {
                cerr << "Some windows refuse to die." << endl;
                return;
            }
            frameManager->broadcastQuit(1);
        }
        rmanClientApp::theApp()->SavePreferences();
        this->Close(TRUE);
    }
    break;
    case MENU_MAIN_FILE_PREFS:
        prefs->edit();
        break;
    case MENU_MAIN_FILE_QUERY:
    {
        rView *app = (rView*)rmanClientApp::theApp();
        app->OpenQueryWindow();
    }
    break;
    case MENU_MAIN_VIEW_OPEN:
        rmanClientApp::theApp()->OpenFile(0, NULL, TRUE);
        break;
    case MENU_MAIN_VIEW_CLOSE:
        if (frameManager != NULL)
        {
            user_event usr;

            usr.type = usr_close_viewers;
            frameManager->broadcastUserEvent(usr);
        }
        break;
    case MENU_MAIN_COLL_LOOK:
        rmanClientApp::theApp()->LookupCollection();
        break;
    case MENU_MAIN_COLL_LKSCL:
        rmanClientApp::theApp()->LookupScaledCollection();
        break;
    case MENU_MAIN_COLL_LKORTH:
        rmanClientApp::theApp()->LookupOrthosection();
        break;
    case MENU_MAIN_COLL_CREATE:
        rmanClientApp::theApp()->CreateCollection();
        break;
    case MENU_MAIN_COLL_DELETE:
        rmanClientApp::theApp()->DeleteCollection();
        break;
    case MENU_MAIN_HELP_ABOUT:
        if (aboutWindow == NULL)
        {
            aboutWindow = new rviewAbout();
        }
        aboutWindow->Show(TRUE);
        break;
    default:
        break;
    }
}

/*
 * Only one exit
 */
bool
rviewMainFrame::OnClose()
{
    return FALSE;
}


/*
 *  Called from wxWindows when the main window is resized.
 */

void rviewMainFrame::OnSize(int w, int h)
{
    int x, y, d;

    GetClientSize(&x, &y);

    //need to resize?
    if (( main_width != x) || ( main_height != y))
    {
        frameWidth =  main_width;
        frameHeight =  main_height;
        x =  main_width;
        y =  main_height;
        SetClientSize(x, y);
        return;
    }

    x -= 2*main_border;
    y -= 2*main_border;
    panel->SetSize(main_border, main_border, x, y);

    x -= 2*main_border;
    y -= 2*main_border;
    d = (y - (5*main_theight + main_bheight)) / 6;

    server->SetSize(main_border, main_border + d/2, x, main_theight);
    port->SetSize(main_border, main_border + main_theight + (3*d)/2, x, main_theight);
    database->SetSize(main_border, main_border + 2*main_theight + (5*d)/2, x, main_theight);
    username->SetSize(main_border, main_border + 3*main_theight + (7*d)/2, x, main_theight);
    userpassword->SetSize(main_border, main_border + 4*main_theight + (9*d)/2, x, main_theight);

    openBut->SetSize((x - main_bwidth)/2, main_border + 5*main_theight + (11*d)/2, main_bwidth, main_bheight);
}



void rviewMainFrame::SetDatabaseInfo(const char *srvName, int srvPort, const char *dbName, const char *usrName)
{
    server->SetValue(srvName);
    port->SetValue(srvPort);
    database->SetValue(dbName);
    username->SetValue(usrName);
    userpassword->SetValue(""); // empty password string
}



void rviewMainFrame::GetDatabaseInfo(DynamicString &srvName, int& srvPort, DynamicString &dbName,
                                     DynamicString &usrName, DynamicString &usrPassword) const
{
    server->GetValue(srvName);
    port->GetValue(srvPort);
    database->GetValue(dbName);
    username->GetValue(usrName);
    userpassword->GetValue(usrPassword);
}




/*
 *  rView specifics; not too much code, really ;-)
 */

const char rView::labels_filename[] = "labels.txt";
const char rView::prefs_filename[] =  ".rviewrc";
const double rView::version = RVIEW_VERSION;


rView::rView(void) : rmanClientApp("RVIEWHOME", prefs_filename, labels_filename)
{
    mainFrame = NULL;
}


rView::~rView(void)
{
}


/*
 *  Initialises the GUI of the program. Most of the real stuff is done in
 *  the rviewMainFrame constructor.
 */

wxFrame *rView::OnInit(void)
{
    char buffer[STRINGSIZE];

    sprintf(buffer, "%s %.1f", lman->lookup("titleRview"), version);
    mainFrame = new rviewMainFrame(NULL, buffer, 0, 0, rviewMainFrame::main_width, rviewMainFrame::main_height);

    mainFrame->SetDatabaseInfo(prefs->serverName, prefs->serverPort, prefs->databaseName, prefs->userName);

    mainFrame->Show(TRUE);

    return (wxFrame*)mainFrame;
}


/*
 *  This function is called from the global callback function mainFrameOpenServer
 *  upon receiving a <RETURN> in one of the text widgets or a click on the
 *  Open button.
 */

void rView::OpenCloseServer(void)
{
    if (database.isOpen())
    {
        CloseServer();
    }
    else
    {
        DynamicString userPassword;
        mainFrame->GetDatabaseInfo(prefs->serverName, prefs->serverPort, prefs->databaseName,
                                   prefs->userName, userPassword);
        prefs->markModified();
        if (OpenServer(prefs->serverName, prefs->serverPort, prefs->databaseName, prefs->userName, userPassword))
            mainFrame->newDBState(FALSE);
    }
}


void rView::OpenQueryWindow(void)
{
    rviewQuery *newQuery = new rviewQuery(&database);
}
