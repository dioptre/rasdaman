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
 * SOURCE: test_servercomm.cc
 *
 * MODULE: servercomm
 *
 * COMMENTS:
 *          None
*/



#include <iostream.h>

#define __EXECUTABLE__
#define EARLY_TEMPLATE
#define DEBUG_MAIN
#define DEBUG
#include "debug.hh"
#include "template_inst.hh"

#include "raslib/rmdebug.hh"
#include "qlparser/qtscalardata.hh"

#include "servercomm/servercomm.hh"


extern char* myExecArgv0 = "";
extern int   tiling = 1;
extern unsigned long maxTransferBufferSize = 4000000;
extern int           globalOptimizationLevel = 4;
extern char*         dbSchema = 0;
extern int           noTimeOut = 0;
char         globalConnectId[255] = {0};
char         globalDbUser[255] = {0};
char         globalDbPasswd[255] = {0};
bool udfEnabled = true;

RMINITGLOBALS('C');

#include <signal.h>


int checkArguments( int argc, char** argv, const char* searchText, int& optionValueIndex )
{
    int found = 0;
    int i=1;

    while( !found && i<argc )
        found = !strcmp( searchText, argv[i++] );

    if( found && i<argc && !strchr(argv[i],'-') )
        optionValueIndex = i;
    else
        optionValueIndex = 0;

    return found;
}


int main( int argc, char** argv )
{
    strcpy(globalConnectId, "tcp:postgresql://localhost:5432/RASBASE");

    ServerComm server(300, 120, 7013, "rasmgr", 7001, "N1");
    ExecuteQueryRes result;

    DatabaseIf database;
    TransactionIf ta;
    AdminIf* myAdmin = AdminIf::instance();
    database.open( "RASSERVICE");
    ta.begin( &database );

    ServerComm::ClientTblElt *r = new ServerComm::ClientTblElt("testclient", 2);

    server.addClientTblEntry (r);

    accessControl.setServerName("NT1");
    accessControl.crunchCapability("$I1$ER.$BRASBASE$T1:3:2008:23:39:24$NNT1$D983893f406445a922cba0301bc5a85ec$K");
    server.openDB(2, "RASBASE", "costea");
    SET_OUTPUT(TRUE);

    char *buff = new char[1000];
    unsigned int size;

    QtScalarData* t;

    try
    {
        server.executeQuery(2, "SELECT a from RAS_COLLECTIONNAMES as a", result );
        vector<QtData*>::iterator i;
        /*    for (i=r->transferData->begin(); i!=r->transferData->end(); ++i) {
          // t = (QtScalarData*)(*i);
          // t->printStatus();
          }*/
    }
    catch ( r_Error& errorObj )
    {
        cerr << errorObj.what() << endl;
        return -1;
    }
    catch ( ... )
    {
        cerr << "Unknown exception caught in main." << endl;
        return -1;
    }

    return 0;
}
