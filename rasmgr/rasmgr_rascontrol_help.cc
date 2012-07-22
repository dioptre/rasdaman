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
 * SOURCE: rasmgr_rascontrol_help.hh
 *
 * MODULE: rasmgr
 * CLASS:  RasControl
 *
 * PURPOSE:
 *   Decodes, verifies and executes the help commands
 *
 * COMMENTS:
 *  Logicaly it's part of rasmgr_rascontrol.cc, but compilation unit was split
 *
*/

// this file is a temporary file, used to keep rasmgr_rascontrol things as long as we check each one
// here are the unchecked things!!

#include "debug-srv.hh"
#include "raslib/rminit.hh"
#include "rasmgr_rascontrol.hh"


void RasControl::helpCommand()
{
    const char *what = argc==1 ? RASMGRCMD_HELP:token[1].take();

    if     (strcasecmp(what,RASMGRCMD_LIST)==0)       listHelp();
    else if(strcasecmp(what,RASMGRCMD_DEFINE   )==0)  defineHelp();
    else if(strcasecmp(what,RASMGRCMD_REMOVE)==0)     removeHelp();
    else if(strcasecmp(what,RASMGRCMD_CHANGE)==0)     changeHelp();
    else if(strcasecmp(what,RASMGRCMD_UP)==0)         upHelp();
    else if(strcasecmp(what,RASMGRCMD_DOWN)==0)       downHelp();
    else if(strcasecmp(what,RASMGRCMD_SAVE)==0)       saveHelp();
    else if(strcasecmp(what,RASMGRCMD_CHECK)==0)      checkHelp();
    else if(strcasecmp(what,RASMGRCMD_EXIT)==0)       exitHelp();
    else if(strcasecmp(what,RASMGRCMD_QUIT)==0)       exitHelp();
    else if(strcasecmp(what,RASMGRCMD_BYE)==0)        exitHelp();
    else helpHelp();

    // no help for version and licence - they have no further params
}


void RasControl::helpHelp()
{
    sprintf(answBuffer,"Help for rascontrol command language\r\n"
            "rasdaman uses the following terms:\r\n"
            "  host (server host)    - a computer running a RasManager (rasmgr), with or without currently active servers\r\n"
            "  srv  (server)         - the rasdaman server (rasserver)\r\n"
            "  dbh  (data base host) - a computer running the database software\r\n"
            "  db   (database)       - the rasdaman database, hosted by the underlying database instance\r\n"
            "  user                  - a person registered by rasdaman through user name and password\r\n"
            "\r\nThe rascontrol utility allows to configure and do run-time administration work for the rasdaman system\r\n"
            "Commands:\r\n"
            "   >help       ...this help\r\n"
            "   >exit       ...exit rascontrol\r\n"
            "   >list       ...list information about the current status of the system\r\n"
            "   >up         ...start servers\r\n"
            "   >down       ...stop servers and rasmanagers\r\n"
            "   >define     ...define a new object\r\n"
            "   >remove     ...remove an object\r\n"
            "   >change     ...change parameters of objects\r\n"
            "   >save       ...make changes permanent\r\n"
            "   >check      ...checks the current status of a slave rasmgr\r\n"
            "Type 'help command' to get specific information about command\r\n"

           );
}

void RasControl::listHelp()
{
    checkPermission(admR_info);

    sprintf(answBuffer,"   The list command:\r\n"
            "list srv [ s | -host h | -all ] [-p] \r\n"
            "       - list information about 'server s' or 'all servers on host h' or 'all defined servers' (default)\r\n"
            "         '-p' prints configuration information; default: runtime status information\r\n"
            "list host\r\n"
            "       - list information about server hosts\r\n"
            "list dbh\r\n"
            "       - list information about database hosts\r\n"
            "list db [ d | -dbh h | -all ] \r\n"
            "       - list information about 'database s' or all 'databases on database host h' or 'all defined databases'\r\n"
            "list user [ -rights]\r\n"
            "       - list the defined users\r\n"
            "         '-rights' additionally lists each user's rights\r\n"
            "list [ license | licence ]\r\n"
            "       - lists licence information\r\n"
            "list version\r\n"
            "       - list version information"
           );

}

void RasControl::defineHelp()
{
    sprintf(answBuffer,"  The define command:\r\n"
            "define dbh 'dbhname' -connect 'connectstring'\r\n"
            "       - define database host with symbolic name 'dbhname'\r\n"
            "         'connectstring' is the string used to connect a client to the underlying database instance\r\n"
            "         (example: user/passwd@hostaddress)\r\n"
            "define db 'dbname' -dbh 'dbhname'\r\n"
            "       - define database 'dbname' on database host 'dbhname'\r\n"
            "         ('dbname' is not a symbolic name, it is the real name of the rasdaman database)\r\n"
            "define host 'hostname' -net 'netaddress' [-port 'portnumber']\r\n"
            "       - define server host with symbolic name 'hostname', located at address 'netaddress:portnumber'\r\n"
            "         ('portnumber' defaults to 7001)\r\n"
            "define srv 'srvname' -host 'hostname' -dbh 'dbhname' -type 'servertype' -port 'portnumber' \r\n"
            "                                [-autorestart on|off] [-countdown 'number'] [-xp 'options']\r\n"
            "       - define server with symbolic name 'srvname' on server host 'hostname' connected to database host 'dbhname'\r\n"
            "         'servertype' can be 'r' (RPC) or 'h' (HTTP) or 'n' (RNP)\r\n"
            "         'portnumber' is the IP port number for HTTP servers / the 'prognum' for RPC/RNP servers\r\n"
            "         -autorestart (default: on): the server will autorestart after an unexpected termination\r\n"
            "         -countdown 'number' (default: 1000): the server will be restarted after 'number' transactions\r\n"
            "         -xp 'options': extra parameter string 'options' that will be passed to the server at startup \r\n"
            "          (default: \"\", see documentation for valid 'options')\r\n"
            "          this option has to be the last, because anything after it and until end of line is considered to be 'options'\r\n"
            "define user 'username' [-passwd 'password'] [-rights 'rightsstring']\r\n"
            "       - define user account with symbolic name 'username'\r\n"
            "         'password' defaults to 'username' (use the raspasswd utility to change)\r\n"
            "         -rights 'rightsstring': the rights granted to the user (default: none; see documentation for valid rights)\r\n"
           );
}
void RasControl::removeHelp()
{
    sprintf(answBuffer,"   The remove command:\r\n"
            "remove dbh 'dbhname'\r\n"
            "       - remove database host 'dbhname'\r\n"
            "remove db 'dbname' -dbh 'dbhname'\r\n"
            "       - remove database 'dbname' from database host 'dbhname'\r\n"
            "         (the database itself is not deleted, only the name is removed from the config tables)\r\n"
            "remove host 'hostname' \r\n"
            "       - remove server host 'hostname'\r\n"
            "remove srv 'srvname'\r\n"
            "       - remove server 'srvname'\r\n"
            "remove user 'username'\r\n"
            "       - remove the user 'username'\r\n"
           );
}

void RasControl::changeHelp()
{
    sprintf(answBuffer,"   The change command:\r\n"
            "change dbh 'dbhname' [-name 'newname'] [-connect 'newconnectstring']\r\n"
            "change db 'dbname' [-name 'newname']\r\n"
            "change host 'hostname' [-name 'newname'] [-net 'newnetaddress'] [-port 'newportnumber']\r\n"
            "change srv 'servername' [-name 'newname'][-dbh 'newdbhname'] [-type 'newservertype'] [-port 'newportnumber'] [-autorestart on|off] [-countdown 'newnumber'] [-xp 'newoptions']\r\n"
            "change user 'username' [-name 'newname'] [-passwd 'newpasswd] [-rights 'rightsstring']\r\n"
            "       - see the help for the define command for option description\r\n"

           );
}

void RasControl::upHelp()
{
    sprintf(answBuffer,"   The up command:\r\n"
            "up srv [ s | -host h | -all]\r\n"
            "       - start 'server s' or 'all servers on host h' or 'all defined servers'\r\n"
           );
}


void RasControl::downHelp()
{
    sprintf(answBuffer,"   The down command:\r\n"
            "down srv [ s | -host h | -all] [ -force] [-kill]\r\n"
            "       - stops 'server s' or 'all started servers on host h' or 'all started servers'\r\n"
            "         -force: stops the 'server s' without waiting to complete the current transaction (using SIGTERM)\r\n"
            "         -kill:  instantly stops the 'server s' (using SIGKILL)\r\n"
            "          (without -force or -kill the server completes the current transaction and exits)\r\n"
            "down host [ h | -all]\r\n"
            "       - stops the rasmgr on 'host h' or all started rasmgr\r\n"
           );
}

void RasControl::checkHelp()
{
    sprintf(answBuffer,"   The check command\r\n"
            "check host 'hostname'\r\n"
            "        - checks the status of the slave rasmgr located on server host 'hostname'\r\n"
            "        (use this command if the master rasmgr started after the slave rasmgr for synchronising them)\r\n"
           );
}

void RasControl::saveHelp()
{
    sprintf(answBuffer,"   The save command\r\n"
            "save\r\n"
            "    - saves the current configuration information\r\n"
            "      (upon changes the files will be saved automatically to rescue files next to the config files when exiting rasmgr)\r\n"
           );
}

void RasControl::exitHelp()
{
    sprintf(answBuffer,"   The exit command\r\n"
            "exit | quit | bye\r\n"
            "    - finish this rascontrol session\r\n"
           );
}



//###       ###########    ############# ################ ##############


/* obsolete in v1.1, but who knows, in future
void RasControl::disconnectCommand()
  {
    const char *what = argc==1 ? RASMGRCMD_HELP:token[1].take();

    if     (strcasecmp(what,"srv")==0)  disconnectRasServerFromDBH();

    else if(strcasecmp(what,"db")==0)   disconnectDatabaseFromDBH();

    else if(strcasecmp(what,RASMGRCMD_HELP)==0) disconnectHelp();

    else errorInCommand("Wrong DISCONNECT command");

   }
void RasControl::disconnectRasServerFromDBH()
  {
    CHECK(admR_config);

    const char *serverName=getValueOf("srv");

    if(!serverName)
      { errorInCommand("Server name missing.");
    return;
       }
    RasServer    &r=rasManager[serverName];

    if(!r.isValid())
      { errorInCommand("Invalid server name.");
        return;
       }
    if(r.isUp())
      { errorInCommand("Cannot disconnet a running server.");
        return;
       }

    const char *dbHost=r.getDBHostName();

    if(strcasecmp(dbHost,"noDBHost!")!=0)
      { sprintf(answBuffer,"Server %s isn't connected to any database host",serverName);
        return;
       }
    if(r.disconnectFromDBHost())
      { sprintf(answBuffer,"Disconnecting server %s from database host %s",serverName,dbHost);
       }
    errorInCommand("Internal error during disconnect.");
   }
void RasControl::disconnectDatabaseFromDBH()
  {
    CHECK(admR_config);

    const char *dbName=getValueOf("db");

    if(!dbName)
      { errorInCommand("Database name missing.");
    return;
       }
    const char *dbHost=getValueOf("-dbh");

    if(!dbHost)
      { errorInCommand("Database host name missing.");
    return;
       }

    Database     &r=dbManager[dbName];
    DatabaseHost &d=dbHostManager[dbHost];

    if(!r.isValid())
      { errorInCommand("Invalid database name.");
        return;
       }
    if(!d.isValid())
      { errorInCommand("Invalid database host name.");
        return;
       }

    if(r.disconnectFromDBHost(dbHost))
      { sprintf(answBuffer,"Disconnecting database %s from database host %s",dbName,dbHost);
       }
     else
      { sprintf(answBuffer,"Database %s is not connected to database host %s",dbName,dbHost);
       }
   }
void RasControl::disconnectHelp()
  {
    sprintf(answBuffer,"Disconnect usage: -sorry, help not available yet");
   }

//--------------------------------------------------------------

void RasControl::connectCommand()
  {
    const char *what = argc==1 ? RASMGRCMD_HELP:token[1].take();

    if     (strcasecmp(what,"srv")==0)  connectRasServerToDBH();

    else if(strcasecmp(what,"db")==0)   connectDatabaseToDBH();

    else if(strcasecmp(what,RASMGRCMD_HELP)==0) connectHelp();

    else errorInCommand("Wrong CONNECT command");

   }
void RasControl::connectRasServerToDBH()
  {
    CHECK(admR_config);

    const char *serverName=getValueOf("srv");

    if(!serverName)
      { errorInCommand("Server name missing.");
    return;
       }
    const char *dbHost=getValueOf("-dbh");

    if(!dbHost)
      { errorInCommand("Database host name missing.");
    return;
       }
    RasServer    &r=rasManager[serverName];
    DatabaseHost &d=dbHostManager[dbHost];

    if(!r.isValid())
      { errorInCommand("Invalid server name.");
        return;
       }
    if(!d.isValid())
      { errorInCommand("Invalid database host name.");
        return;
       }

   // const char *connStr=getValueOf("-conn");
   // if(!connStr) connStr="\\";

    if(r.connectToDBHost(dbHost))//,connStr))
      { sprintf(answBuffer,"Connecting server %s to database host %s",serverName,dbHost);
       }
     else
      { sprintf(answBuffer,"Server %s already connected to database host %s",serverName,dbHost);
       }

   }
void RasControl::connectDatabaseToDBH()
  {
    CHECK(admR_config);

    const char *dbName=getValueOf("db");

    if(!dbName)
      { errorInCommand("Database name missing.");
    return;
       }
    const char *dbHost=getValueOf("-dbh");

    if(!dbHost)
      { errorInCommand("Database host name missing.");
    return;
       }

    Database     &r=dbManager[dbName];
    DatabaseHost &d=dbHostManager[dbHost];

    if(!r.isValid())
      { errorInCommand("Invalid database name.");
       }
    if(!d.isValid())
      { errorInCommand("Invalid database host name.");
       }

    if(r.connectToDBHost(dbHost))
      { sprintf(answBuffer,"Connecting database %s to database host %s",dbName,dbHost);
       }
     else
      { sprintf(answBuffer,"Database %s already connected to database host %s",dbName,dbHost);
       }
   }
void RasControl::connectHelp()
  {
    sprintf(answBuffer,"Connect usage: -sorry, help not available yet");
   }

void RasControl::listRasServersDatabase(const char *dbName)
  {
    CHECK(admR_info);

    if(dbName)
      { sprintf(answBuffer,"List of servers connected to database %s:",dbName);

    Database &db = dbManager[dbName];
    if(db.isValid()==false)
      { sprintf(answBuffer+strlen(answBuffer),"\r\nNo such database");
        return;
       }
    int crnt=0;
    for(int i=0;i<db.countConnectionsToRasServers();i++)
          {
        sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %s",crnt++,db.getRasServerName(i));
       }
       }
    else
      {
         errorInCommand("Database name missing.");
       }
   }

void RasControl::listRasServersOnDBH(const char *dbhName)
  {
    CHECK(admR_info);

    if(dbhName)
      { sprintf(answBuffer,"List of servers connected to database host %s:",dbhName);

    int crnt=0;
    for(int i=0;i<rasManager.countServers();i++)
          {
        RasServer &r=rasManager[i];
        if(strcmp(dbhName,r.getDBHostName())!=0) continue;

            sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %s (%s)",crnt++,r.getName(),r.getHostName());
       }
       }
     else
       { sprintf(answBuffer,"List of servers with connected database host:");

         int crnt=0;
     for(int i=0;i<rasManager.countServers();i++)
           {
         RasServer &r=rasManager[i];
         sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %-20s %s",crnt++,r.getName(),r.getDBHostName());
        }
    }
   }

*/



/* no, out
void RasControl::stopCommand()
  {
    CHECK(admR_sysup);

   // sprintf(answBuffer,RASMGRCMD_EXIT);
   // masterCommunicator.shouldExit();
    if(rasManager.countUpServers()==0)
      {
        sprintf(answBuffer,RASMGRCMD_EXIT);
    masterCommunicator.shouldExit();
       }
    else
      {
        sprintf(answBuffer,"Please shut down all servers first.");
       }
   }


void RasControl::listConnections()
  {
    CHECK(admR_info);
    bool full = isFlag("-full");

    checkUnexpectedTokens();

    int answLen=0;
    int maxAnswLen=MAXMSGOUTBUFF-100; // if there are many servers, we could get an overflow

    bool overflow=false;
    sprintf(answBuffer,"List connections\r\n  Database      Database host         Server");
    for(int i=0;i<dbManager.countDatabases();i++)
      {
        Database&db = dbManager[i];
        const char *dbName = db.getName();

    for(int j=0;j<db.countConnectionsToDBHosts();j++)
      {
        const char *dbhName = db.getDBHostName(j);
        const char* writtenDbhName = dbhName;

        for(int k=0;k < rasManager.countServers();k++)
          {
            RasServer &srv = rasManager[k];

            if(strcmp(srv.getDBHostName(),dbhName) == 0)
          {
            int len = strlen(answBuffer);

            if(len < maxAnswLen)
              { sprintf(answBuffer+len,"\r\n  %-15s %-20s %-20s",dbName,writtenDbhName,srv.getName());
                if(full==false) dbName = writtenDbhName ="  -\"-";
               }
            else overflow=true;
           }
           }
       }
        }
     if(overflow) sprintf(answBuffer+strlen(answBuffer),"\r\n(Answer too long, overflow!)");
   }

*/
