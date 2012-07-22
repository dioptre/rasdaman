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
#include "wx/wx.h"
#endif



#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <signal.h>
#include <ctype.h>


#include "labelManager.hh"


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif


#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"

#include "rviewApp.hh"
#include "rviewDb.hh"
#include "rviewPrefs.hh"
//#include "rviewQuery.hh"
#include "rviewIO.hh"
#include "rviewDModes.hh"
#include "rviewOSection.hh"





/*
 *  Class for the lookup of scaled images, used internally only
 */

class rviewLookScaleDialog: public rviewFrame
{
public:

    rviewLookScaleDialog(rmanClientApp *parentApp, const char *name, double scale);
    ~rviewLookScaleDialog(void);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);
    void OnSize(int w, int h);

    // constants
    static const int lkscale_border;
    static const int lkscale_theight;
    static const int lkscale_bwidth;
    static const int lkscale_bheight;
    static const int lkscale_width;
    static const int lkscale_height;


protected:

    wxPanel *panel;
    rviewText *collName;
    rviewText *scaleString;
    rviewButton *okBut;
    rviewButton *cancelBut;

    rmanClientApp *parent;
};


const int rviewLookScaleDialog::lkscale_border = 8;
const int rviewLookScaleDialog::lkscale_theight = 50;
const int rviewLookScaleDialog::lkscale_bwidth = 50;
const int rviewLookScaleDialog::lkscale_bheight = 30;
const int rviewLookScaleDialog::lkscale_width = 300;
const int rviewLookScaleDialog::lkscale_height = 2*rviewLookScaleDialog::lkscale_border + 2*rviewLookScaleDialog::lkscale_theight + rview_window_extra_height;


rviewLookScaleDialog::rviewLookScaleDialog(rmanClientApp *parentApp, const char *name, double scale) : rviewFrame(NULL, lman->lookup("titleLookScaleColl"), -1, -1, lkscale_width, lkscale_height)
{
    panel = new wxPanel(this, 0, 0, lkscale_width, lkscale_height);
    panel->SetLabelPosition(wxVERTICAL);
    collName = new rviewText(panel, name);
    scaleString = new rviewText(panel, scale);
    okBut = new rviewButton(panel);
    cancelBut = new rviewButton(panel);
    parent = parentApp;
    label();

    frameWidth=-1;
    frameHeight=-1;

    OnSize(lkscale_width, lkscale_height);
    OnSize(lkscale_width, lkscale_height);

    Show(TRUE);
}

rviewLookScaleDialog::~rviewLookScaleDialog(void)
{
}

void rviewLookScaleDialog::OnSize(int w, int h)
{
    int x, y, aux, boff;

    GetClientSize(&x, &y);
    panel->SetSize(0, 0, x, y);
    aux = x - 3*lkscale_border - lkscale_bwidth;
    collName->SetSize(lkscale_border, lkscale_border, aux, lkscale_theight);
    scaleString->SetSize(lkscale_border, lkscale_border + lkscale_theight, aux, lkscale_theight);
    aux += 2*lkscale_border;
    boff = lkscale_border + lkscale_theight - lkscale_bheight;
    okBut->SetSize(aux, boff, lkscale_bwidth, lkscale_bheight);
    cancelBut->SetSize(aux, boff + lkscale_theight, lkscale_bwidth, lkscale_bheight);
}

void rviewLookScaleDialog::label(void)
{
    SetTitle(lman->lookup("titleLookScaleColl"));
    collName->SetLabel(lman->lookup("lkScaleName"));
    scaleString->SetLabel(lman->lookup("lkScaleScale"));
    okBut->SetLabel(lman->lookup("textOK"));
    cancelBut->SetLabel(lman->lookup("textCancel"));
}

int rviewLookScaleDialog::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();
    int readAndClose=0;

    if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
        readAndClose = 1;
    else if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)okBut)
            readAndClose = 1;
        else if (&obj == (wxObject*)cancelBut)
        {
            parent->LookupScaledCollection(NULL, 0.0);
            Close(TRUE);
            return 1;
        }
    }
    if (readAndClose != 0)
    {
        double scale;
        char *b, *rest;

        b = scaleString->GetValue();
        scale = strtod(b, &rest);
        if (b == rest)
        {
            cerr << "Bad scaling parameter..." << endl;
        }
        else
        {
            parent->LookupScaledCollection(collName->GetValue(), scale);
            Close(TRUE);
        }
        return -1;
    }
    return 0;
}





//#define DUMMY_MDD_OBJECT  r_UShort

//FILE *logfile;        // WIN32 debug!!!


// Emergency exit signal handler
void rmanAbort(int code)
{
    char *sig;

    // First things first
    rmanClientApp::theApp()->Shutdown();

    switch (code)
    {
    case SIGINT:
        sig = "SIGINT";
        break;
    case SIGABRT:
        sig = "SIGABRT";
        break;
    case SIGSEGV:
        sig = "SIGSEGV";
        break;
    case SIGFPE:
        sig = "SIGFPE";
        break;
#ifndef __VISUALC__
    case SIGBUS:
        sig = "SIGBUS";
        break;
#endif
    default:
        sig = "SIG?";
        break;
    }

    cerr << "Aborted " << code << " (" << sig << " )" << endl;
    //fprintf(logfile, "Aborted (%s)\n", sig);

    exit(-1);
}


/*
 *  A RasDaMan client application using wxWindows. This can be used as base class
 *  to other RasDaMan apps as well.
 */

rmanClientApp *rmanClientApp::theclientapp = NULL;

const char *rmanClientApp::vffFileName = "vff-file";

rmanClientApp::rmanClientApp(const char *homevar, const char *prefsname, const char *labelname)
{
    char *rvh;
    char *b;
#ifdef __VISUALC__
    char *d;
#endif
    char labels[STRINGSIZE];

    theclientapp = this;

    //logfile = fopen("rview.log", "w");

    rvh = getenv(homevar);

    // labels.txt is looked for in $RVIEWHOME or failing that .
    if (rvh == NULL)
        sprintf(homeDir, ".");
    else
        sprintf(homeDir, "%s", rvh);

    sprintf(labels, "%s"DIR_SEPARATOR"%s", homeDir, labelname);

    lman = new labelManager(labels);

    frameManager = new rviewFrameMgr();

    rviewInitCharacterTables();

    strcpy(prefsFileLeafname, prefsname);

    // Preferences are looked for in $HOME, ., $RVIEWHOME
#ifdef __VISUALC__
    if (((b = getenv("HOMEDRIVE")) != NULL) && ((d = getenv("HOMEPATH")) != NULL))
        sprintf(prefsFile, "%s%s", b, d);
#else
    if ((b = getenv("HOME")) != NULL)
        sprintf(prefsFile, "%s", b);
#endif
    else
        strcpy(prefsFile, ".");

    // always save prefs to $HOME/.rviewrc
    sprintf(prefsSaveFile, "%s"DIR_SEPARATOR"%s", prefsFile, prefsname);

    if (!findPreferencesOnPath(prefsFile))
    {
        strcpy(prefsFile, ".");
        if (!findPreferencesOnPath(prefsFile) && (rvh != NULL))
        {
            strcpy(prefsFile, homeDir);
            findPreferencesOnPath(prefsFile);
        }
    }

    if (prefsFile[0] == 0)
    {
        cerr << "Couldn't find any preferences files." << endl;
        cout << "New prefs will be written to " << prefsSaveFile << endl;
    }

    prefs = new rviewPrefs(prefsFile);

    // Install signal handlers (open databases are ANNOYING!)
    signal(SIGILL, (void(*)(int))rmanAbort);
    signal(SIGINT, (void(*)(int))rmanAbort);
#if !defined(RMANDEBUG)
    signal(SIGABRT, (void(*)(int))rmanAbort);
    signal(SIGSEGV, (void(*)(int))rmanAbort);
#endif
    signal(SIGFPE, (void(*)(int))rmanAbort);
#ifndef __VISUALC__
    signal(SIGBUS, (void(*)(int))rmanAbort);
#endif
}


rmanClientApp::~rmanClientApp(void)
{
    delete lman;
    lman = NULL;
    delete frameManager;
    frameManager = NULL;
    theclientapp = NULL;
    rviewIO::terminate();
    //prefs->save(prefsSaveFile);
    //fclose(logfile);
}


bool rmanClientApp::findPreferencesOnPath(char *path)
{
    sprintf(path + strlen(path), DIR_SEPARATOR"%s", prefsFileLeafname);
    if (::wxFileExists(path)) return TRUE;
    path[0] = 0;
    return FALSE;
}


int rmanClientApp::SavePreferences(void) const
{
    return prefs->save(prefsSaveFile);
}


rmanClientApp *rmanClientApp::theApp(void)
{
    return theclientapp;
}


bool rmanClientApp::ReadDBState(void)
{
    return database.isOpen();
}




/*
 *  Called when something crashes.
 */
void rmanClientApp::Shutdown(void)
{
    if (database.isOpen())
    {
        database.close();
    }
}




int rmanClientApp::OpenServer(const char *srvname, int srvport, const char *dbname,
                              const char *usrname, const char *usrpassword)
{
    if (database.isOpen())
        return -1;

    RMDBGONCE(3, RMDebug::module_applications, "rviewApp", "OpenServer( " << srvname << ", " << dbname << "," << usrname << "," << usrpassword << " )" );

    if (database.open(srvname, srvport, dbname, usrname, usrpassword) != 0)
    {
        user_event ue;

        ue.type = usr_db_opened;
        if (frameManager != NULL)
            frameManager->broadcastUserEvent(ue);

        return 0;
    }
    return -1;
}


int rmanClientApp::CloseServer(void)
{
    if (!database.isOpen())
        return -1;

    user_event ue;

    database.close();
    ue.type = usr_db_closed;

    if (frameManager != NULL)
        frameManager->broadcastUserEvent(ue);

    RMDBGONCE(3, RMDebug::module_applications, "rviewApp", "CloseServer() Database closed." );

    return 0;
}



rviewFrame *rmanClientApp::OpenFile(unsigned int flags, r_Ref<r_GMarray> *newMddObj, bool resultwin)
{
    char *s;
    char *prefPath = (char*)(prefs->filePath.ptr());

    s = wxFileSelector(lman->lookup("loadFile"), (::wxDirExists(prefPath)) ? prefPath : NULL , NULL, NULL, "*");

    if (s)
    {
        int status;
        r_Ref<r_GMarray> mddPtr;

        RMDBGONCE(3, RMDebug::module_applications, "rviewApp", "OpenFile(...) " << s );

        prefs->filePath = ::wxPathOnly(s);
        prefs->markModified();

        // load TIFF
        if (rviewIO::isTIFF(s))
        {
            status = rviewIO::loadTIFF(s, mddPtr, prefs->vffParams.ptr());
            if (status == RVIEW_IO_OK)
            {
                rviewDisplay *newImage;
                mdd_frame mddObj;
                // receiver makes a copy.
                mddObj.mdd = mddPtr;
                mddObj.flags = 0;
                newImage = new rviewFlatImage(&mddObj, flags | rviewDisplay::display_flag_standalone);
                if (newMddObj != NULL) *newMddObj = mddPtr;
                if (newImage->openViewer() != 0)
                {
                    delete newImage;
                    newImage = NULL;
                }
                return newImage;
            }
        }
        // load VFF -- whether this is supported is sorted out in rviewIO.cpp
        else if (rviewIO::isVFF(s))
        {
            status = rviewIO::loadVFF(s, mddPtr, prefs->vffParams.ptr());
            if (status == RVIEW_IO_OK)
            {
                if (resultwin)
                {
                    // load in results window
                    rviewResult *result;
                    const char *file = ::wxFileNameFromPath(s);

                    collection_desc *cdesc = new collection_desc;
                    cdesc->collName = new char[strlen(file)+1];
                    strcpy(cdesc->collName, file);
                    cdesc->collType = new char[strlen(mddPtr->get_type_name())+1];
                    strcpy(cdesc->collType, mddPtr->get_type_name());
                    cdesc->collInfo = new char[strlen(vffFileName)+1];
                    strcpy(cdesc->collInfo, vffFileName);
                    cdesc->number = 1;
                    cdesc->mddObjs = new mdd_frame[1];
                    cdesc->strObjs = NULL;
                    cdesc->mddObjs[0].mdd = mddPtr;
                    cdesc->mddObjs[0].flags = 0;

                    result = new rviewResult(cdesc);
                    return result;
                }
                else
                {
                    // load in default viewer
                    rviewDisplay *newImage;
                    mdd_frame mddObj;
                    mddObj.mdd = mddPtr;
                    mddObj.flags = 0;
                    newImage = new rviewVolumeImage(&mddObj, flags | rviewDisplay::display_flag_standalone);
                    if (newMddObj != NULL) *newMddObj = mddPtr;
                    if (newImage->openViewer() != 0)
                    {
                        delete newImage;
                        newImage = NULL;
                    }
                    return newImage;
                }
            }
        }
    }
    return (rviewFrame*)NULL;
}


int rmanClientApp::LookupCollection(void)
{
    collection_desc *desc;
    char *name;

    name = ::wxGetTextFromUser(lman->lookup("promptEnterColl"), lman->lookup("titleCollLook"), (char*)(prefs->lastColl.ptr()));

    if (name == NULL) return 0;

    if ((desc = new collection_desc) == NULL)
    {
        cerr << lman->lookup("errorMemory")  << endl;
        return 0;
    }
    memset(desc, 0, sizeof(collection_desc));

    if ((desc->collName = new char [strlen(name) + 1]) == NULL)
    {
        cerr << lman->lookup("errorMemory") << endl;
        delete desc;
        return 0;
    }

    strcpy(desc->collName, name);
    prefs->lastColl = name;
    prefs->markModified();

    RMDBGENTER(3, RMDebug::module_applications, "rviewApp", "LookupCollection() " << desc->collName );

#ifdef DUMMY_MDD_OBJECT
    desc->number = 1;
    r_Minterval iv(3);
    iv << r_Sinterval(r_Range(0),r_Range(300)) << r_Sinterval(r_Range(200),r_Range(400)) << r_Sinterval(r_Range(10), r_Range(12));
    desc->mddObjs = new mdd_frame[1];
    desc->mddObjs[0].mdd = (r_Ref <r_GMarray>) new r_Marray<DUMMY_MDD_OBJECT>(iv);
    desc->collType = new char[16];
    strcpy(desc->collType, "r_UShort");
    r_Ref < r_Marray < DUMMY_MDD_OBJECT > > mddPtr = (r_Ref < r_Marray < DUMMY_MDD_OBJECT > >) desc->mddObjs[0].mdd;
    mddPtr->set_type_by_name("UShortImage");
    r_Point point(3);
    cout << mddPtr->spatial_domain() << endl;
    for (point[0]=0; point[0]<=300; point[0]++)
        for (point[1]=200; point[1]<=400; point[1]++)
            for (point[2]=10; point[2]<=12; point[2]++)
                (*mddPtr)[point] = ((DUMMY_MDD_OBJECT)(point[0] + point[1] + point[2])) << (8*(sizeof(DUMMY_MDD_OBJECT) - 1));

    if (1)
#else
    if (database.lookupCollection(desc) != 0)
#endif
    {

        RMDBGEXIT(3, RMDebug::module_applications, "rviewApp", "LookupCollection() OK." );

        /*for (int i=0; i<desc->number; i++)
        {
          cout << "Object #" << i << ": " << desc->mddArrays[i]->spatial_domain() << endl;
        }*/

        rviewResult *result = new rviewResult(desc);

        return 1;
    }
    else
    {
        rviewDeleteCollection(desc);
    }

    RMDBGEXIT(3, RMDebug::module_applications, "rviewApp", "LookupCollection() FAILED." );
    return 0;
}


int rmanClientApp::LookupScaledCollection(void)
{
    rviewLookScaleDialog *lsd = new rviewLookScaleDialog(this, (char*)(prefs->lastScColl.ptr()), prefs->imgScale);

    return 0;
}

int rmanClientApp::LookupScaledCollection(const char *name, double scale)
{
    if (name != NULL)
    {
        collection_desc *desc;

        RMDBGENTER(3, RMDebug::module_applications, "rviewApp", "LookupScaledCollection( " << name << " at scale " << scale << " )");

        prefs->imgScale = scale;

        if ((desc = new collection_desc) == NULL)
        {
            cerr << lman->lookup("errorMemory")  << endl;
            return 0;
        }
        memset(desc, 0, sizeof(collection_desc));

        if ((desc->collName = new char [strlen(name) + 1]) == NULL)
        {
            cerr << lman->lookup("errorMemory") << endl;
            delete desc;
            return 0;
        }

        strcpy(desc->collName, name);
        prefs->lastScColl = name;
        prefs->markModified();
        r_Fast_Base_Scale *scaler;

        if ((scaler = database.lookupScaledObject(desc, scale)) != NULL)
        {
            // don't call in standalone mode, viewer sorts it out itself.
            rviewScaledImage *image = new rviewScaledImage(desc, scaler);
            if (image->openViewer() != 0)
            {
                image->Close(TRUE);
                RMDBGEXIT(3, RMDebug::module_applications, "rviewApp", "LookupScalledCollection() FAILED" );
                return 0;
            }
            RMDBGEXIT(3, RMDebug::module_applications, "rviewApp", "LookupScalledCollection()  OK" );
            return 1;
        }
        else
        {
            rviewDeleteCollection(desc);
        }
    }

    RMDBGEXIT(3, RMDebug::module_applications, "rviewApp", "LookupScalledCollection() FAILED" );
    return 0;
}


int rmanClientApp::LookupOrthosection(void)
{
    char *name;

    name = ::wxGetTextFromUser(lman->lookup("promptEnterOrtho"), lman->lookup("titleOrthoLook"), (char*)(prefs->lastOrthoColl.ptr()));

    if ((name != NULL) && (strlen(name) != 0))
    {
        char buffer[STRINGSIZE];
        const char *d = name;
        char *b = buffer;
        const double *lp = NULL;
        double loid;

        // search the collection name for a trailing ,<local oid>
        while ((*d != '\0') && (*d != ','))
        {
            *b++ = *d++;
        }
        while ((b > buffer) && (isspace((unsigned int)(b[-1])))) b--;
        *b++ = '\0';
        if (*d == ',')
        {
            loid = atof(d+1);
            lp = &loid;
        }

        return LookupOrthosection(buffer, lp);
    }
    return -1;
}


int rmanClientApp::LookupOrthosection(const char *collname, const double *loid)
{
    rviewOSectionPartImage *newImage = rviewOSectionPartImage::createViewer(collname, loid);

    if (newImage == NULL)
    {
        rviewErrorbox::reportError(lman->lookup("errorOrthoViewer"), "rmanClientApp", "LookupOrthosection");
        return -1;
    }

    prefs->lastOrthoColl = collname;

    return 0;
}


int rmanClientApp::CreateCollection(void)
{
    char *name;

    name = ::wxGetTextFromUser(lman->lookup("promptEnterColl"), lman->lookup("titleCollCrt"), (char*)(prefs->lastOrthoColl.ptr()));

    if (name != NULL)
    {
        char buffer[STRINGSIZE];

        strncpy(buffer, name, STRINGSIZE);
        return database.createCollection(buffer, rbt_none);
    }
    return 0;
}



int rmanClientApp::DeleteCollection(void)
{
    char *name;
    char buffer[STRINGSIZE];

    name = ::wxGetTextFromUser(lman->lookup("promptEnterColl"), lman->lookup("titleCollDel"), (char*)(prefs->lastColl.ptr()));

    if (name != NULL)
    {
        strncpy(buffer, name, STRINGSIZE);
        return database.deleteCollection(buffer);
    }
    return 0;
}



int rmanClientApp::insertMDD(r_Ref<r_GMarray> srcMdd, char *collName, r_Minterval *domain)
{
    char useName[STRINGSIZE];
    char *title;
    int status;

    if (domain == NULL)
        title = lman->lookup("titleInsertMdd");
    else
        title = lman->lookup("titleInsertMddPro");

    if (collName == NULL)
    {
        title = ::wxGetTextFromUser(lman->lookup("promptEnterColl"), title, (char*)(prefs->lastColl.ptr()));
        if (title == NULL) return 0;
        strncpy(useName, title, STRINGSIZE);
    }
    else
        strncpy(useName, collName, STRINGSIZE);

    if ((status = database.insertObject(useName, srcMdd, domain)) != 0)
    {
        prefs->lastColl = useName;
        prefs->markModified();
    }
    return status;
}



collection_desc *rmanClientApp::executeQuerySync(char *query, r_Ref<r_GMarray> *updateMdd, bool showProgress)
{
    collection_desc *desc;

    if ((desc = new collection_desc) == NULL)
    {
        cerr << lman->lookup("errorMemory") << endl;
        return NULL;
    }

    desc->number = 0;
    if (database.executeQuery(desc, query, updateMdd, showProgress) == 0)
    {
        delete desc;
        return NULL;
    }

    // no result?
    if (desc->number > 0)
    {
        char qcollName[] = "<query>";

        desc->collName = new char[strlen(qcollName)+1];
        strcpy(desc->collName, qcollName);
    }
    else
    {
        delete desc;
        return NULL;
    }

    return desc;
}


int rmanClientApp::executeQuery(char *query, r_Ref<r_GMarray> *updateMdd)
{
    collection_desc *desc;

    if ((desc = executeQuerySync(query, updateMdd)) != NULL)
    {
        if (desc->mddObjs != NULL)
        {
            rviewResult *qresult = new rviewResult(desc);
        }
        else
        {
            rviewStringSet *qresult = new rviewStringSet(desc);
            // rviewStringSet doesn't need the descriptor data after the
            // constructor, so we can just delete it right away
            rviewDeleteCollection(desc);
        }
        return 1;
    }
    return 0;
}


int rmanClientApp::getMinterval(r_Minterval &dom, const char *collname, const double *loid)
{
    return database.getMinterval(dom, collname, loid);
}
