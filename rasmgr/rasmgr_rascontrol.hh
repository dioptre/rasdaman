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
 * SOURCE: rasmgr_rascontrol.hh
 *
 * MODULE: rasmgr
 * CLASS:  RasControl
 *
 * PURPOSE:
 *   Decodes, verifies and xecutes the commands
 *
 * COMMENTS:
 *          None
 *
*/
#ifndef RASMGR_RASCONTROL_HH
#define RASMGR_RASCONTROL_HH

#include "rasmgr.hh"
#include "rasmgr_comm.hh"
#include "rasmgr_users.hh"
#include "rasmgr_host.hh"


/**
  * \ingroup Rasmgrs
  */
class RasControl
{
public:
    int processRequest(char* reqMessage, char *answMessage);
    void setConfigDirty( bool isDirty );
    void setAuthDirty( bool isDirty );

private:
    void helloCommand();
    void exitCommand();
    void helpCommand();
    void helpHelp();
    void exitHelp();
    void listCommand();
    void listRasServers();
    void listRasHosts();
    void listDBHosts();
    void listDatabases();
    void listUsers();
    void listModus();
    void listVersion();
    void listConnections();
    void listRights();
    void listHelp();
    void defineCommand();
    void defineRasServers();
    void defineRasHosts();
    void defineDBHosts();
    void defineDatabases();
    void defineUsers();
    void defineHelp();
    void removeCommand();
    void removeRasServers();
    void removeRasHosts();
    void removeDBHosts();
    void removeDatabases();
    void removeUsers();
    void removeHelp();
    void checkCommand();
    void checkRasHosts();
    void checkHelp();
    void upCommand();
    void upRasServers();
    int upAllServersOnHost(const char*hostName);
    void upHelp();
    void downCommand();
    void downRasServers();
    int downAllServersOnHost(const char *hostName);
    void downRasHosts();
    int downRasHost(const char *hostName);
    void downHelp();
    void changeCommand();
    void changeHost();
    void changeUser();
    void changeRasServer();
    void changeRasServer(const char *serverName, const char *dbhName, const char *countString, const char *extraString, const char *autoRestart, const char* execName);
    void changeDBHost();
    void changeDB();
    void changeHelp();
    void saveCommand();
    void saveHelp();
    // void stopCommand();
    void resetCommand();

    void grantCommand();
    void revokeCommand();

    // flag whether conf or auth file must be written before exit
    bool configDirty;
    bool authDirty;

    void errorInCommand(const char*);
    int  prepareAnswer(char *answMessage);
    char answBuffer[MAXMSGOUTBUFF+20];
    bool isCommand(const char *key);
    char commandBuffer[MAXMSG+20]; //for bug search blva

    void splitRequest(const char* reqMessage);
    bool         isFlag(const char*,int pos=-1);
    const char * getValueOf(const char*,bool acceptMinus=false); //'-' alone, only void right string
    const char * getValueIfFlag(const char*,bool acceptMinus=false);
    void checkUnexpectedTokens();
    void checkPermission(int reqRights);
    void checkNotNull(const char *ptr, const char *what);
    unsigned long convertToULong(const char *stringValue,const char *what);

    RasServer&    getServer(const char*); //later, just use rasmanager[serverName]
    Database&     getDatabase(const char *name);
    DatabaseHost& getDatabaseHost(const char *name);
    ServerHost&   getServerHost(const char *name);
    User&         getUser(const char *name);

    struct Token
    {
        char *argv;
        bool  used;
        void set(char*);
        const char *take();
    };
    int  argc;
    Token token[30];

};
extern RasControl rascontrol;

/* obsolete, but in the future...
      void grantCommand();
      void revokeCommand();

      void connectCommand();
         void connectRasServerToDBH();
         void connectDatabaseToDBH();
         void connectHelp();
      void disconnectCommand();
         void disconnectRasServerFromDBH();
         void disconnectDatabaseFromDBH();
         void disconnectHelp();
       void listRasServersOnDBH(const char*);
       void listRasServersDatabase(const char*);

*/
#endif

