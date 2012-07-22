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
 *          none
 */

/**
*   @file rview.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_H_
#define _RVIEW_H_


#ifdef __GNUG__
#pragma interface
#endif





// RasDaMan includes
#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"


#include "rviewApp.hh"
#include "rviewUtils.hh"
#include "rviewPrefs.hh"



/*
 *  rView's main frame
 */
class rviewMainFrame: public rviewFrame
{
public:

    rviewMainFrame(wxFrame *frame, char *title, int x, int y, int w, int h);
    ~rviewMainFrame(void);
    void OnMenuCommand(int id);
    void OnSize(int w, int h);
    void SetDatabaseInfo(const char *srvName, int srvPort,
                         const char *dbName, const char*usrName);
    void GetDatabaseInfo(DynamicString &srvName, int& srvPort, DynamicString &dbName,
                         DynamicString &usrName, DynamicString &usrPassword) const;
    void newDBState(bool newState);
    bool OnClose();

    // Implementations of the rviewFrame virtual functions
    void label(void);
    int process(wxObject &obj, wxEvent &evt);

    int userEvent(const user_event &ue);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // constants
    // Width and height of main window
    static const int main_width;
    static const int main_height;
    // Border around panel in main frame
    static const int main_border;
    // Height of server / database text widgets
    static const int main_theight;
    // Dimensions of button(s)
    static const int main_bwidth;
    static const int main_bheight;


protected:

    rviewText *server;
    rviewText *port;
    rviewText *database;
    rviewText *username;
    rviewText *userpassword;
    rviewButton *openBut;
    wxPanel *panel;
    wxMenuBar *mBar;
    bool dbOpen;
    rviewAbout *aboutWindow;
};



/*
 *  rView application specifics
 */
class rView: public rmanClientApp
{
public:

    rView(void);
    virtual ~rView(void);
    wxFrame *OnInit(void);
    void OpenCloseServer(void);
    void OpenQueryWindow(void);


private:

    rviewMainFrame *mainFrame;

    static const char labels_filename[];
    static const char prefs_filename[];
    static const double version;
};

#endif
