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
 * COMMENTS:
 *      None
 */

/**
*   @file rviewQuery.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_QUERY_H_
#define _RVIEW_QUERY_H_


#ifdef __GNUG__
#pragma interface
#endif



#include "rviewUtils.hh"
#include "rviewDb.hh"





class rviewQuery: public rviewFrame
{
public:

    rviewQuery(rviewDatabase *db, char *query=NULL);
    ~rviewQuery(void);

    void OnSize(int w, int h);
    void OnMenuCommand(int id);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);

    int userEvent(const user_event &ue);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;

    // Returns the query window ID
    int getIdentifier(void) const;
    int getQueryCounter(void) const;


protected:

    void buildMenubar(void);
    bool loadQuery(char *file);
    bool saveQuery(char *file);
    void newDBState(bool newState);
    void updateTitle(void);

    wxTextWindow *twin;
    wxMenuBar *mbar;
    wxMenu *mbarMenus[3];
    wxPanel *panel;
    rviewButton *butClear, *butExec, *butUpdt;
    wxFont *font;
    int hotNumber;
    DynamicString hotPath;
    DynamicString lastHotPath;
    rviewDatabase *queryDb;
    // For update queries
    rviewDisplay *updateDisplay;
    r_Ref<r_GMarray> updateMddObj;
    int qwindowID;
    int updateID;

    static const char query_extension[];
    static const char query_firstline[];

    static const keyword_to_ident_c fontNameTab[];
    static const keyword_to_ident_c fontStyleTab[];
    static const keyword_to_ident_c fontWeightTab[];

    // constants
    // Width and height of query window
    static const int query_width;
    static const int query_height;
    // Borders in query window
    static const int query_border;
    // Height of control area at the bottom of the query
    static const int query_bottom;
    // Button dimensions
    static const int query_bwidth;
    static const int query_bheight;


private:

    // Query window counter, realises a unique integer ID for each
    // query window.
    static int queryCounter;
};

#endif
