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
 *  RasDaMan + wxWindows application class used as base of rView etc.
 *  The main application includes the database object which has to be accessed
 *  through rmanClientApp-member functions.
 *
 *  COMMENTS:
 *      none
 */

/**
*   @file rviewApp.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_APP_H_
#define _RVIEW_APP_H_

#ifdef __GNUG__
#pragma interface
#endif

// RasDaMan includes
#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"

#include "rviewUtils.hh"
#include "rviewDb.hh"
#include "rviewDModes.hh"

/*
 *  A generic RasDaMan client app using wxWindows.
 */
class rmanClientApp : public wxApp
{
public:

    rmanClientApp(const char *homevar, const char *prefsname, const char *labelname);
    virtual ~rmanClientApp(void);

    int  OpenServer(const char *srvname, int srvport, const char *dbname,
                    const char *usrname, const char *usrpassword);
    int  CloseServer(void);
    bool ReadDBState(void);
    int  LookupCollection(void);
    int  LookupScaledCollection(void);
    int  LookupScaledCollection(const char *name, double scale);
    int  LookupOrthosection(void);
    int  LookupOrthosection(const char *name, const double *loid);
    int  CreateCollection(void);
    int  DeleteCollection(void);
    rviewFrame *OpenFile(unsigned int flags=0, r_Ref<r_GMarray> *newMddObj=NULL, bool resultwin=FALSE);
    virtual void Shutdown(void);
    int insertMDD(r_Ref<r_GMarray> srcMdd, char *collName=NULL, r_Minterval *domain=NULL);
    int executeQuery(char *query, r_Ref<r_GMarray> *updateMdd=NULL);
    collection_desc *executeQuerySync(char *query, r_Ref<r_GMarray> *updateMdd=NULL, bool showProgress=TRUE);
    int getMinterval(r_Minterval &dom, const char *collname, const double *loid=NULL);

    int  SavePreferences(void) const;
    bool findPreferencesOnPath(char *path);

    // use this call to contact the application instance
    static rmanClientApp *theApp(void);

protected:

    rviewDatabase database;
    char prefsFile[STRINGSIZE];
    char prefsSaveFile[STRINGSIZE];
    char prefsFileLeafname[STRINGSIZE];
    char homeDir[STRINGSIZE];

private:

    static rmanClientApp *theclientapp;
    static const char *vffFileName;
};

#endif
