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
 * SOURCE: rasmgr_rascontrol.cc
 *
 * MODULE: rasmgr
 * CLASS:  RasControl
 *
 * PURPOSE:
 *   Decodes, verifies and executes the commands
 *
 * COMMENTS:
 *          None
 *
*/

using namespace std;

#include "globals.hh"           // DEFAULT_PORT
#include "rasmgr_rascontrol.hh"
#include "rasmgr_config.hh"
#include "rasmgr_master.hh"
#include "rasmgr_srv.hh"
#include "rasmgr_users.hh"
#include "rasmgr_error.hh"
#include "raslib/rminit.hh"

#ifndef RMANVERSION
#error "Please specify RAMNVERSION variable!"
#endif

#ifndef COMPDATE
#error "Please specify the COMPDATE variable!"
/*
COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`
and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
*/
#endif

//#include "rasmgr_rascontrol_help.cc"

#include "debug-srv.hh"

extern bool hostCmp( const char *h1, const char *h2);

// function to migrate -xp parameters, only for v5.1, remove after
void migrateExtraParams(const char *orig, char *migrated);

int RasControl::processRequest(char* reqMessage, char *answMessage)
{
    ENTER( "RasControl::processRequest: enter. rascontrol msg: " << reqMessage );
    splitRequest(reqMessage);

    const char *command=argc ? token[0].take() : "#";

    try
    {
        if(command)
        {
            TALK( "RasControl::processRequest: command=" << command );

            if     (isCommand(RASMGRCMD_HELLO))  helloCommand();
            else if(isCommand(RASMGRCMD_HELP))   helpCommand();
            else if(isCommand(RASMGRCMD_LIST))   listCommand();
            else if(isCommand(RASMGRCMD_DEFINE)) defineCommand();
            else if(isCommand(RASMGRCMD_REMOVE)) removeCommand();
            else if(isCommand(RASMGRCMD_CHECK))  checkCommand();
            else if(isCommand(RASMGRCMD_UP))     upCommand();
            else if(isCommand(RASMGRCMD_DOWN))   downCommand();
            else if(isCommand(RASMGRCMD_CHANGE)) changeCommand();
            else if(isCommand(RASMGRCMD_SAVE))   saveCommand();
            else if(isCommand(RASMGRCMD_EXIT))   exitCommand();
            else if(isCommand(RASMGRCMD_RESET))  resetCommand();

#ifdef INCLUDE_HIDDEN_COMMANDS
            // both are unofficial, PB doesn't like them, but I do
            else if(isCommand(RASMGRCMD_GRANT))  grantCommand();
            else if(isCommand(RASMGRCMD_REVOKE)) revokeCommand();
            //################
#endif

            else if(isCommand("#"))  sprintf(answBuffer," "); // comment
            else
            {
                errorInCommand("Invalid command; try HELP." );
                RMInit::logOut << "Invalid command word: " << command << endl;
            }
        }
        else
        {
            RMInit::logOut << "Error in request: " << reqMessage << endl;
            errorInCommand("Error in request." );
        }
    }
    catch(RCError& e)
    {
        strcpy(answBuffer,"Error: ");
        e.getString(answBuffer + strlen(answBuffer));
        RMInit::logOut << answBuffer << endl;
    }

    LEAVE( "RasControl::processRequest: leave. answerBuffer: " << answBuffer );
    return prepareAnswer(answMessage);
}

// set dirty flags
// used to differentiate between config file read and real changes thru rascontrol
void RasControl::setConfigDirty( bool isDirty )
{
    configDirty = isDirty;
}

void RasControl::setAuthDirty( bool isDirty )
{
    authDirty = isDirty;
}

int RasControl::prepareAnswer(char *answMessage)
{
    sprintf(answMessage,"HTTP/1.1 200 OK\r\nContent-type: text/plain\r\nContent-length: %d\r\n\r\n%s",strlen(answBuffer)+1,answBuffer);
    return strlen(answMessage)+1;
}

//*************************************************
void RasControl::helloCommand()
{
    sprintf(answBuffer,"Hello %s, you are connected to %s",authorization.getUserName(),config.getHostName());
}
//*************************************************

void RasControl::exitCommand()
{
    bool configResult = false;
    bool authResult = false;

    ENTER( "RasControl::exitCommand: enter" );

    (void) strcpy( answBuffer, "Exiting rascontrol session." );

    if (configDirty)
        configResult = config.saveAltConfigFile();

    if (authDirty)
        authResult = authorization.saveAltAuthFile();

    sprintf(answBuffer, "Exiting rascontrol session.%s\n%s%s%s%s%s%s%s%s%s%s",
            ((argc <= 1)  ? "" : " Ignoring extra parameters."),

            (!configDirty ? "" : "Configuration file was changed but not saved, storing rescue copy to " ),
            (!configDirty ? "" : config.getAltConfigFileName() ),
            (!configDirty ? "" : "..." ),
            (!configDirty ? "" : (configResult ? "ok" : "failed") ),
            (!configDirty ? "" : "\n" ),

            (!authDirty   ? "" : "Authorisation file was changed but not saved, storing rescue copy to " ),
            (!authDirty   ? "" : authorization.getAltAuthFileName() ),
            (!authDirty   ? "" : "..." ),
            (!authDirty   ? "" : (authResult ? "ok" : "failed") ),
            (!authDirty   ? "" : "\n" ) );

    // (!configDirty ? "" : "Configuration was changed but not saved, storing rescue copy to " << config.getAltConfigFileName() << "..." << (configResult ? "ok" : "failed") << "." << endl),
    // (!authDirty   ? "" : "Authorisation was changed but not saved, storing rescue copy to " << authorization.getAltAuthFileName()   << "..." << (authResult ? "ok" : "failed") << "." << endl) );

    LEAVE( "RasControl::exitCommand: leave. answBuffer=" << answBuffer );
}

void RasControl::listCommand()
{
    const char *listwhat = argc==1 ? "xxx":token[1].take();

    if     (strcasecmp(listwhat,RASMGRCMD_SRV)==0)      listRasServers();
    else if(strcasecmp(listwhat,"version")==0)  listVersion();
    else if(strcasecmp(listwhat,"modus"  )==0)  listModus();
    else if(strcasecmp(listwhat,RASMGRCMD_USER   )==0)  listUsers();
    else if(strcasecmp(listwhat,RASMGRCMD_HOST)==0)     listRasHosts();
    else if(strcasecmp(listwhat,"dbh")==0)      listDBHosts();
    else if(strcasecmp(listwhat,"db")==0)       listDatabases();

#ifdef INCLUDE_HIDDEN_COMMANDS
    else if(strcasecmp(listwhat,"rights")==0) listRights();// unofficial
#endif

    else if(strcasecmp(listwhat,RASMGRCMD_HELP)==0) listHelp();


    else errorInCommand("Illegal LIST modifier. Try HELP LIST." );
}

void RasControl::listModus()
{
    checkPermission(admR_info);

    checkUnexpectedTokens();

    const char *modus = config.isTestModus() ? "test"  :"normal";

    sprintf(answBuffer,"rasmgr running in %s modus",modus);
}
void RasControl::listVersion()
{
    checkPermission(admR_info);

    checkUnexpectedTokens();

    // Version 1.1 is 1.0 with changes P.B. wanted, 06.03.2001
    // Version 1.2 is 1.1 with changes P.B. wanted, 17.04.2001
    // Version 1.3 with "list srv -x" and "-hostname" parameter
    // Version 1.4 with migration of command line options from v5.0 to v5.1
    // Version 1.5 with new cmds, bug fixes in socket communication
    sprintf(answBuffer,"rasdaman v%1f (rasmgr v1.5, compiled on %s)", RMANVERSION/1000, COMPDATE);


#ifdef INCLUDE_HIDDEN_COMMANDS
    strcat(answBuffer," ('inside only'-version)");
#endif
}

void RasControl::listUsers()
{
    checkPermission(admR_info);

    bool isRights = isFlag("-rights");

    checkUnexpectedTokens();

    sprintf(answBuffer,"List of defined users:");

    for(int i=0; i<userManager.countUsers(); i++)
    {
        User &user=userManager[i];
        //sprintf(answBuffer+strlen(answBuffer),"\r\n%d. %s (%ld)",i,u.getName(),u.getUserID());
        sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %-20s ",i+1,user.getName());

        if(isRights)
        {
            sprintf(answBuffer+strlen(answBuffer)," [%s]",authorization.convertAdminRights(user.getAdminRights()));
            sprintf(answBuffer+strlen(answBuffer)," -[%s]",authorization.convertDatabRights(user.getDefaultDBRights()));
        }
    }
}

void RasControl::listRasHosts()
{
    checkPermission(admR_info);
    checkUnexpectedTokens();

    sprintf(answBuffer,"List of server hosts:\r\n");
    ServerHost::getDescriptionHeader(answBuffer+strlen(answBuffer));
    for(int i=0; i<hostmanager.countHosts(); i++)
    {
        sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. ",i+1);
        hostmanager[i].getDescription(answBuffer+strlen(answBuffer));

    }
}

void RasControl::listDBHosts()
{
    checkPermission(admR_info);
    checkUnexpectedTokens();

    sprintf(answBuffer,"List of database hosts:\r\n");
    sprintf(answBuffer+strlen(answBuffer),"    Database Host     Connection String            Databases");

    for(int i=0; i<dbHostManager.countHosts(); i++)
    {
        DatabaseHost &dbh = dbHostManager[i];
        sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %-15s %-30s",(i+1),dbh.getName(),dbh.getConnectionString());

        for(int j=0; j<dbManager.countDatabases(); j++)
        {
            if(dbManager[j].isConnectedToDBHost(dbh.getName()))
                sprintf(answBuffer+strlen(answBuffer)," %s",dbManager[j].getName());
        }
    }
}


void RasControl::listRasServers()
{
    checkPermission(admR_info);
    int answLen=0;
    int maxAnswLen=MAXMSGOUTBUFF-100; // if there are many servers, we could get an overflow

    bool fports= isFlag("-p");
    bool fexec = isFlag("-x");

    if(fports && fexec)
    {
        // error, not both of them together!
    }


    const char *srvName  = getValueOf(RASMGRCMD_SRV);
    if(srvName)
    {
        checkUnexpectedTokens();
        RasServer &serv=getServer(srvName);

        sprintf(answBuffer,"Status of server %s\r\n",srvName);
        if(fports)     RasServer::getDescriptionPortHeader(answBuffer+strlen(answBuffer));
        else if(fexec) RasServer::getDescriptionExecHeader(answBuffer+strlen(answBuffer));
        else           RasServer::getDescriptionHeader(answBuffer+strlen(answBuffer));
        strcat(answBuffer,"\r\n    ");
        if(fports)     serv.getDescriptionPort(answBuffer+strlen(answBuffer));
        else if(fexec) serv.getDescriptionExec(answBuffer+strlen(answBuffer));
        else           serv.getDescription(answBuffer+strlen(answBuffer));

        return;
    }

    bool isHost = isFlag("-host");
    const char *hostName = getValueOf("-host");
    bool listAll = false;
    if (isHost)
    {
        checkUnexpectedTokens();
        checkNotNull(hostName,"host name");
        getServerHost(hostName);

    }
    else
    {

        // no -host hostName, so check for -all
        listAll = isFlag("-all");
        checkUnexpectedTokens();
        // normally we should generate an error if -all is not, but for compatibility...
        // put the error here
    }

    if(hostName == NULL) sprintf(answBuffer,"List of servers:\r\n");
    else                 sprintf(answBuffer,"List of servers on host %s:\r\n",hostName);

    if(fports)     RasServer::getDescriptionPortHeader(answBuffer+strlen(answBuffer));
    else if(fexec) RasServer::getDescriptionExecHeader(answBuffer+strlen(answBuffer));
    else           RasServer::getDescriptionHeader(answBuffer+strlen(answBuffer));
    int crnt=1;
    for(int i=0; i<rasManager.countServers(); i++)
    {
        answLen=strlen(answBuffer);

        if(answLen >=maxAnswLen)
        {
            sprintf(answBuffer+answLen,"\r\n(Answer too long, overflow!)");
            break;
        }

        if(hostName)
            if(!hostCmp(hostName,rasManager[i].getHostName()))
                continue;

        sprintf(answBuffer+answLen,"\r\n%2d. ",crnt++);

        if(fports)     rasManager[i].getDescriptionPort(answBuffer+strlen(answBuffer));
        else if(fexec) rasManager[i].getDescriptionExec(answBuffer+strlen(answBuffer));
        else           rasManager[i].getDescription(answBuffer+strlen(answBuffer));
    }

}

void RasControl::listDatabases()
{
    checkPermission(admR_info);

    const char* dbName = getValueOf("db");

    if(dbName)
    {
        checkUnexpectedTokens();
        Database &db = getDatabase(dbName);

        sprintf(answBuffer,"List database: %s\r\n",dbName);
        Database::getDescriptionHeader(answBuffer+strlen(answBuffer));
        strcat(answBuffer,"\r\n    ");
        db.getDescription(answBuffer+strlen(answBuffer));
        return;
    }

    bool flagDBH = isFlag("-dbh");
    const char* dbhName = getValueOf("-dbh");

    if(flagDBH)
    {
        checkUnexpectedTokens();
        checkNotNull(dbhName,"database host name");
        getDatabaseHost(dbhName);
    }

    bool flagAll = isFlag("-all");
    // if flagALL is not, we should generate an error message, but we list all silently
    checkUnexpectedTokens();

    if(dbhName)
        sprintf(answBuffer,"List of databases on host: %s\r\n",dbhName);
    else sprintf(answBuffer,"List of databases:\r\n");// blanks necesary for nice output
    Database::getDescriptionHeader(answBuffer+strlen(answBuffer));

    int crnt=1;
    for(int i=0; i<dbManager.countDatabases(); i++)
    {
        Database &db=dbManager[i];

        if(dbhName)
        {
            bool found=false;
            for(int j=0; j < db.countConnectionsToDBHosts(); j++)
            {
                if(hostCmp(db.getDBHostName(j),dbhName))
                {
                    found=true;
                    break;
                }
            }
            if(found == false)
                continue;
        }
        sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. ",crnt++);
        db.getDescription(answBuffer+strlen(answBuffer));
    }

}
//******************************************************
void RasControl::defineCommand()
{
    const char *what = argc==1 ? "xxx":token[1].take();

    if     (strcasecmp(what,RASMGRCMD_SRV)==0)
    {
        defineRasServers();
        configDirty = true;
    }
    else if(strcasecmp(what,RASMGRCMD_HOST)==0)
    {
        defineRasHosts();
        configDirty = true;
    }
    else if(strcasecmp(what,"dbh")==0)
    {
        defineDBHosts();
        configDirty = true;
    }
    else if(strcasecmp(what,"db")==0)
    {
        defineDatabases();
        configDirty = true;
    }
    else if(strcasecmp(what,RASMGRCMD_USER)==0)
    {
        defineUsers();
        authDirty = true;
    }
    else if(strcasecmp(what,RASMGRCMD_HELP)==0)
        defineHelp();
    else
        errorInCommand("Wrong DEFINE command");
}

void RasControl::defineUsers()
{
    checkPermission(admR_acctrl);

    const char *userName  = getValueOf(RASMGRCMD_USER);
    const char *plainPass = getValueIfFlag("-passwd");
    const char *rString   = getValueIfFlag("-rights",true);
    int admRights =0;
    int dtbRights =0;
    checkUnexpectedTokens();
    checkNotNull(userName,"user name");

    for(int i=0; userName[i]; i++)
        if(userName[i]==':')
        {
            errorInCommand("Invalid character (':') in user name.");
            return;
        }

    if(rString)
    {
        admRights=authorization.convertAdminRights(rString);
        dtbRights=authorization.convertDatabRights(rString);
        if(admRights==-1 || dtbRights==-1)
        {
            errorInCommand("Unknown right in command");
            return;
        }
    }

    if(userManager.insertNewUser(userName)==false)
    {
        errorInCommand("User name should be unique.");
        return;
    }

    User &user = userManager[userName];

    if(plainPass)  user.changePTPassword(plainPass);
    if(rString)
    {
        user.setAdminRights(admRights);
        user.setDefaultDBRights(dtbRights);
    }

    sprintf(answBuffer,"Defining user %s",userName);
}

void RasControl::defineDBHosts()
{
    checkPermission(admR_config);

    const char *dbhName=getValueOf("dbh");
    const char *connStr=getValueOf("-connect", true);
    const char *userStr=getValueOf("-user", true);
    const char *passwdStr=getValueOf("-passwd", true);

    checkUnexpectedTokens();
    checkNotNull(dbhName,"database host name");
    checkNotNull(connStr,"connection string");

    if(dbHostManager.insertNewHost(dbhName,connStr,userStr,passwdStr))
    {
        sprintf(answBuffer,"Defining database host %s with connection string: %s",dbhName,connStr);
    }
    else
    {
        sprintf(answBuffer,"Database host %s already defined.",dbhName);
    }
}

void RasControl::defineDatabases()
{
    checkPermission(admR_config);

    const char *dbName  = getValueOf("db");
    const char *dbhName = getValueOf("-dbh");
    checkUnexpectedTokens();
    checkNotNull(dbName,"database name");
    // we don't accept hostless databases any more, define goes together with connect from now on
    checkNotNull(dbhName,"database host name");
    getDatabaseHost(dbhName);// just check if valid

    bool defDB   = dbManager.insertNewDatabase(dbName);
    Database &db = dbManager[dbName];

    bool connDB =  db.connectToDBHost(dbhName);

    if(defDB)
    {
        sprintf(answBuffer,"Defining database %s on database host %s",dbName,dbhName);
    }
    else
    {
        //database was already defined
        if(connDB)    sprintf(answBuffer,"Defining a mirrored instance of database %s on database host %s.",dbName,dbhName);
        else          sprintf(answBuffer,"Database %s already defined on database host %s.",dbName,dbhName);
    }

}

void RasControl::defineRasHosts()
{
    ENTER( "RasControl::defineRasHosts()" );

    checkPermission(admR_config);

    const char *hostName=getValueOf(RASMGRCMD_HOST);
    const char *netName =getValueOf("-net");
    const char *portStr =getValueOf("-port");
    checkUnexpectedTokens();

    checkNotNull(hostName,"host name");

    checkNotNull(netName,"-net parameter");

    long listenPort;
    if(portStr) listenPort=convertToULong(portStr,"port");
    else        listenPort=DEFAULT_PORT;

    if(hostmanager.insertNewHost(hostName,netName,listenPort)==false)
    {
        sprintf(answBuffer,"Error: Host %s already defined.",hostName);
        LEAVE( "RasControl::defineRasHosts()" );
        return;
    }
    else
    {
        sprintf(answBuffer,"Defining server host %s port=%d",hostName,listenPort);
    }

    LEAVE( "RasControl::defineRasHosts()" );
}

void RasControl::defineRasServers()
{
    ENTER( "RasControl::defineRasServers()" );

    checkPermission(admR_config);
    bool inConfigFile = authorization.isInConfigFile();

    const char *serverName=getValueOf(RASMGRCMD_SRV);
    const char *hostName  =getValueOf("-host");
    const char *portStr   =getValueOf("-port");
    const char *sTypeStr  =getValueOf("-type");
    const char *dbhName   =getValueOf("-dbh");

    const char *autoRestart    = getValueIfFlag("-autorestart");
    const char *countString    = getValueIfFlag("-countdown");
    const char *execString     = getValueIfFlag("-exec");
    bool isExtra  = isFlag("-xp");
    // make the -xp string first
    char extraString[300];
    extraString[0]=0;

    if(isExtra)
    {
        bool found=false;
        for(int i=0; i<argc; i++)
        {
            if(!found)
            {
                if(strcasecmp(token[i].argv,"-xp")==0) found=true;
            }
            else
            {
                strcat(extraString,token[i].take());
                strcat(extraString," ");
            }
        }
    }

    checkUnexpectedTokens();

    checkNotNull(serverName,"server name");

    if(!sTypeStr)
    {
        errorInCommand("Missing server type, specify one of r (RPC), h (HTTP) or n (RNP).");
        LEAVE( "RasControl::defineRasServers()" );
        return;
    }

    char serverType=0;
    if(strcasecmp(sTypeStr,"r")==0)
    {
        serverType=SERVERTYPE_FLAG_RPC;
    }
    if(strcasecmp(sTypeStr,"h")==0)
    {
        serverType=SERVERTYPE_FLAG_HTTP;
    }
    if(strcasecmp(sTypeStr,"n")==0)
    {
        serverType=SERVERTYPE_FLAG_RNP;
    }
    if(!serverType)
    {
        errorInCommand("Illegal server type, use one of [r|h|n].");
        LEAVE( "RasControl::defineRasServers()" );
        return;
    }

    checkNotNull(hostName,"host name");
    checkNotNull(portStr,"port number");

    if(inConfigFile == false)
    {
        // we accept connectionless servers in config file, because we do not remove
        // the connected servers when we remove a database host
        checkNotNull(dbhName,"database host name");
        getDatabaseHost(dbhName);// just check if valid

    }

    long listenPort=convertToULong(portStr,"port");

    if(autoRestart)
    {
        if(strcasecmp(autoRestart,"on")==0)
            ;
        else if(strcasecmp(autoRestart,"off")==0)
            ;
        else
        {
            errorInCommand("Incorrect autorestart option, use one of [on|off].");
            LEAVE( "RasControl::defineRasServers()" );
            return;
        }
    }

    getServerHost(hostName);// just check
    if(rasManager.insertNewServer(serverName,hostName,serverType,listenPort)==false)
    {
        sprintf(answBuffer,"Error: server name already existing.");
        LEAVE( "RasControl::defineRasServers()" );
        return;
    }

    if(inConfigFile && !dbhName)
    {
        sprintf(answBuffer,"Error: no database host specified.");
        LEAVE( "RasControl::defineRasServers()" );
        return;
    }

    RasServer &srv = rasManager[serverName];
    srv.connectToDBHost(dbhName);
    // if ok, write what you defined

    // we put this options in the config file with the change srv command, so it's ok that it could return before
    if(countString) srv.changeCountDown(convertToULong(countString,"countdown"));

    if(extraString) srv.changeExtraParam(extraString);

    if(autoRestart) srv.changeAutoRestart( strcasecmp(autoRestart,"on")==0 ? true:false);

    if(execString)  srv.changeExecutableName(execString);

    if(serverType==SERVERTYPE_FLAG_RPC)
        sprintf(answBuffer,"Defining deprecated server %s of type RPC on host %s port=%#x",serverName,hostName,listenPort);

    if(serverType==SERVERTYPE_FLAG_HTTP)
        sprintf(answBuffer,"Defining deprecated server %s of type HTTP on host %s port=%d",serverName,hostName,listenPort);

    if(serverType==SERVERTYPE_FLAG_RNP)
        sprintf(answBuffer,"Defining server %s of type RNP on host %s port=%d",serverName,hostName,listenPort);

    LEAVE( "RasControl::defineRasServers()" );
}

//----------------------------------------
void RasControl::removeCommand()
{
    const char *what = argc==1 ? "xxx":token[1].take();

    if     (strcasecmp(what,RASMGRCMD_SRV)==0)  removeRasServers();

    else if(strcasecmp(what,RASMGRCMD_HOST)==0) removeRasHosts();

    else if(strcasecmp(what,"dbh")==0)  removeDBHosts();

    else if(strcasecmp(what,"db")==0)   removeDatabases();

    else if(strcasecmp(what,RASMGRCMD_USER)==0) removeUsers();

    else if(strcasecmp(what,RASMGRCMD_HELP)==0) removeHelp();

    else errorInCommand("Error in REMOVE command");
}

void RasControl::removeUsers()
{
    checkPermission(admR_acctrl);

    const char *userName=getValueOf(RASMGRCMD_USER);
    checkUnexpectedTokens();

    checkNotNull(userName,"user name");

    User &user=getUser(userName);

    if(user.getUserID()==0)
    {
        errorInCommand("You cannot remove the rasadmin.");
        return;
    }

    if(userManager.removeUser(userName)==false)
    {
        errorInCommand("Cannot remove user, don't know why");
        return;
    }
    sprintf(answBuffer,"User %s removed",userName);
}

void RasControl::removeRasHosts()
{
    checkPermission(admR_config);

    const char *hostName=getValueOf(RASMGRCMD_HOST);
    checkUnexpectedTokens();

    checkNotNull(hostName,"host name");

    ServerHost &host = getServerHost(hostName);

    if(host.isInternal()==true)
    {
        errorInCommand("You cannot remove the master rasmgr host.");
        return;
    }

    if(hostmanager.removeHost(hostName)==false)
    {
        errorInCommand("Cannot remove host, it still has defined servers.");
        return;
    }
    sprintf(answBuffer,"Host %s removed",hostName);
}

void RasControl::removeRasServers()
{
    checkPermission(admR_config);

    const char *srvName=getValueOf(RASMGRCMD_SRV);
    checkUnexpectedTokens();

    checkNotNull(srvName,"server name");

    RasServer &srv = getServer(srvName);

    if(srv.isUp())
    {
        errorInCommand("Cannot remove the server, it's still up.");
        return;
    }

    if(rasManager.removeServer(srvName)==false)
    {
        errorInCommand("Cannot remove the server, is probably still up.");
        return;
    }
    sprintf(answBuffer,"Server %s removed",srvName);
}

void RasControl::removeDBHosts()
{
    checkPermission(admR_config);

    const char *dbhName=getValueOf("dbh");
    checkUnexpectedTokens();

    checkNotNull(dbhName,"database host name");

    getDatabaseHost(dbhName);

    if(dbHostManager.removeHost(dbhName)==false)
    {
        errorInCommand("Cannot remove the database host, is probably still busy.");
        return;
    }
    sprintf(answBuffer,"Database host %s removed",dbhName);

    // now remove all databases not connected to any database host
    for(int i=0; i<dbManager.countDatabases(); i++)
    {
        if(dbManager[i].countConnectionsToDBHosts()==0)
        {
            dbManager.removeDatabase(dbManager[i].getName());
            i--; // you understand why
        }
    }
}
void RasControl::removeDatabases()
{
    checkPermission(admR_config);

    const char *dbName  = getValueOf("db");
    const char *dbhName = getValueOf("-dbh");
    checkUnexpectedTokens();

    checkNotNull(dbName,"database name");

    Database &db= getDatabase(dbName);

    checkNotNull(dbhName,"database host name");

    getDatabaseHost(dbhName);

    if(db.isBusy())
    {
        errorInCommand("Database is busy.");
        return;
    }

    if(db.disconnectFromDBHost(dbhName) == false)
    {
        sprintf(answBuffer,"No database %s on database host %s.",dbName,dbhName);
        return;
    }

    if(db.countConnectionsToDBHosts() !=0 )
    {
        sprintf(answBuffer,"Database %s removed from database host %s",dbName,dbhName);
        // this means disconnected
    }
    else if(dbManager.removeDatabase(dbName))
    {
        sprintf(answBuffer,"Database %s removed from database host %s",dbName,dbhName);
        // this time removed completely
    }
    else
    {
        errorInCommand("Cannot remove the database, but why? (There shouldn't be any reason for that)");
    }
}

//--------------------------------------------
void RasControl::changeCommand()
{
    const char *what = argc==1 ? "xxx":token[1].take();

    if     (strcasecmp(what,RASMGRCMD_USER)==0)
    {
        changeUser();
        authDirty = true;
    }
    else if(strcasecmp(what,RASMGRCMD_SRV)==0)
    {
        changeRasServer();
        configDirty = true;
    }
    else if(strcasecmp(what,"dbh")==0)
    {
        changeDBHost();
        configDirty = true;
    }
    else if(strcasecmp(what,"db")==0)
    {
        changeDB();
        configDirty = true;
    }
    else if(strcasecmp(what,RASMGRCMD_HOST)==0)
    {
        changeHost();
        configDirty = true;
    }
    else if(strcasecmp(what,RASMGRCMD_HELP)==0)
        changeHelp();
    else
        errorInCommand("Error in CHANGE command");

}
void RasControl::changeHost()
{
    checkPermission(admR_config);
    // we accept all this parameters, but we make all changes or none
    const char *hostName=getValueOf(RASMGRCMD_HOST);

    const char *uselocal = getValueIfFlag("-uselocalhost");
    const char *newName  = getValueIfFlag("-name");
    const char *newNet   = getValueIfFlag("-net");
    const char *newPort  = getValueIfFlag("-port");

    checkUnexpectedTokens();

    // This means changes were requested
    bool chUseLoc = false;
    bool chName   = false;
    bool chNet    = false;
    bool chPort   = false;

    checkNotNull(hostName,"server host name");

    ServerHost &sh= getServerHost(hostName);

    answBuffer[0]=0;
    int changes = 0;
    int port = 0;

    if(uselocal)
    {
        chUseLoc = true;
        if(sh.isInternal()==false)
        {
            errorInCommand("Option '-uselocalhost' is meaningfull only on master rasmgr host");
            return;
        }
        else
        {
            if     (strcasecmp(uselocal,"on" )==0); // ok
            else if(strcasecmp(uselocal,"off")==0); // ok
            else
            {
                errorInCommand("Option '-uselocalhost' - wrong parameter");
                return;
            }
        }
    }

    if(newName)
    {
        if(hostmanager.acceptChangeName(hostName,newName)==false)
        {
            errorInCommand("The new name is not unique.");
            return;
        }

        chName = true;
    }

    if(newNet )
    {
        chNet = true;
        if(sh.isUp())
        {
            errorInCommand("You cannot change the network address of a RasMgr while it is up.");
            return;
        }
    }

    if(newPort)
    {
        chPort = true;
        if(sh.isUp())
        {
            errorInCommand("You cannot change the listen port of a RasMgr while it is up.");
            return;
        }

        port = convertToULong(newPort,"port");
    }



    if(chUseLoc)
    {
        changes++;
        sh.useLocalHost( strcasecmp(uselocal,"on") ==0 ? true : false);
    }

    if(chName)
    {
        changes++;
        sh.changeName(newName);
    }

    if(chNet)
    {
        changes++;
        sh.changeNetName(newNet);
    }

    if(chPort)
    {
        changes++;
        sh.changeListenPort(port);
    }

    if(changes) sprintf(answBuffer,"Ready");
    else errorInCommand("Change what?");

}

void RasControl::changeDBHost()
{
    checkPermission(admR_config);
    const char *dbhName    = getValueOf("dbh");
    const char *newName    = getValueIfFlag("-name");
    const char *connString = getValueIfFlag("-connect");
    checkUnexpectedTokens();

    checkNotNull(dbhName,"database host name");

    DatabaseHost &dbh= getDatabaseHost(dbhName);

    if(newName)
    {
        if(dbHostManager.acceptChangeName(dbhName,newName)==false)
        {
            errorInCommand("The new name is not unique.");
            return;
        }

        dbh.changeName(newName);
    }
    else if(connString) dbh.changeConnectionString(connString);

    else
    {
        errorInCommand("Change what?");
        return;
    }

    sprintf(answBuffer,"Ready");

}

void RasControl::changeDB()
{
    checkPermission(admR_config);
    const char *dbName    = getValueOf("db");
    const char *dbNewName = getValueIfFlag("-name");

    checkUnexpectedTokens();

    checkNotNull(dbName,"database name");

    Database &db= getDatabase(dbName);

    if(dbNewName)
    {
        if(dbManager.acceptChangeName(dbName,dbNewName)==false)
        {
            errorInCommand("The new name is not unique.");
            return;
        }
        db.changeName(dbNewName);
    }
    else
    {
        errorInCommand("Change what?");
        return;
    }

    sprintf(answBuffer,"Ready");

}

/** the version supporting only one server - the documented one
void RasControl::changeRasServer()
  {
    checkPermission(admR_config);

    const char *serverName  = getValueOf(RASMGRCMD_SRV);
    const char *newServerName  = getValueIfFlag("-name");
    const char *portString     = getValueIfFlag("-port");
    const char *autoRestart    = getValueIfFlag("-autorestart");
    const char *countString    = getValueIfFlag("-countdown");
    const char *dbhName        = getValueIfFlag("-dbh");
    bool isExtra  = isFlag("-xp");
    bool autorestartValue;
    // make the -xp string first
    char extraString[300]; extraString[0]=0;
    if(isExtra)
       {
         bool found=false;
         for(int i=0;i<argc;i++)
       {
         if(!found)
           { if(strcasecmp(token[i].argv,"-xp")==0) found=true;
            }
         else
           { strcat(extraString,token[i].take());
             strcat(extraString," ");
            }
        }
        }
    // so we touch all tokens
    checkUnexpectedTokens();

    checkNotNull(serverName,"server name");

    RasServer &r= getServer(serverName);

    if(dbhName) getDatabaseHost(dbhName);

    if(autoRestart)
       { if(strcasecmp(autoRestart,"on")==0)       autorestartValue=true;
         else if(strcasecmp(autoRestart,"off")==0) autorestartValue=false;
     else { errorInCommand("Incorect autorestart option");
            return;
           }
    }

    if(newServerName && r.isUp()==false) )
        { errorInCommand("Cannot change the name of a running server.");
      return;
     }

    // from here, it cannot fail any more
    if(autoRestart) r.changeAutoRestart(autorestartValue);

    if(dbhName)
      { r.disconnectFromDBHost();
        r.connectToDBHost(dbhName);
       }
    if(newServerName) r.changeName(newServerName);

    if(portString)  r.changePort(strtoul(portString,(char**)NULL,0));

    if(countString) r.changeCountDown(strtoul(countString,(char**)NULL,0));

    if(isExtra)     r.changeExtraParam(extraString);

    sprintf(answBuffer,"Ready");

   }
*/

// The version supporting change srv [s| -hos h | -all ] { rest of params }
void RasControl::changeRasServer()
{
    checkPermission(admR_config);

// first, see if params are ok

    // only if "srv name": const char *newServerName  = getValueIfFlag("-name");
    // only if "srv name": const char *portString     = getValueIfFlag("-port");
    const char *autoRestart    = getValueIfFlag("-autorestart");
    const char *countString    = getValueIfFlag("-countdown");
    const char *execString     = getValueIfFlag("-exec");
    const char *dbhName        = getValueIfFlag("-dbh");
    bool isExtra  = isFlag("-xp");
    int countChanges = 0;
    // make the -xp string first
    char extraString[300];
    extraString[0]=0;

    if(isExtra)
    {
        bool found=false;
        for(int i=0; i<argc; i++)
        {
            if(!found)
            {
                if(strcasecmp(token[i].argv,"-xp")==0) found=true;
            }
            else
            {
                strcat(extraString,token[i].take());
                strcat(extraString," ");
            }
        }
        countChanges++;
    }
    // so we touch all tokens


    if(dbhName)
    {
        getDatabaseHost(dbhName);
        countChanges++;
    }

    if(autoRestart)
    {
        if(strcasecmp(autoRestart,"on")==0)       ;
        else if(strcasecmp(autoRestart,"off")==0) ;
        else
        {
            errorInCommand("Incorect autorestart option");
            return;
        }
        countChanges++;
    }
    if(countString) countChanges++;
    if(execString)  countChanges++;

    // ok, the existing params are ok

    const char *serverName  = getValueOf(RASMGRCMD_SRV);
    if(serverName)
    {
        // permitted only here
        const char *newType        = getValueIfFlag("-type");
        const char *newServerName  = getValueIfFlag("-name");
        const char *portString     = getValueIfFlag("-port");

        checkUnexpectedTokens();
        RasServer &r = getServer(serverName);
        char serverType=0;
        if(newType)
        {
            if(strcasecmp(newType,"r")==0)
            {
                serverType=SERVERTYPE_FLAG_RPC;
            }
            if(strcasecmp(newType,"h")==0)
            {
                serverType=SERVERTYPE_FLAG_HTTP;
            }
            if(strcasecmp(newType,"n")==0)
            {
                serverType=SERVERTYPE_FLAG_RNP;
            }
            if(!serverType)
            {
                errorInCommand("Unknown server type.");
                return;
            }
            else countChanges++;
        }
        if(newServerName)
        {
            if(r.isUp())
            {
                errorInCommand("Cannot change the name of a running server.");
                return;
            }
            if(rasManager.acceptChangeName(serverName,newServerName)==false)
            {
                errorInCommand("The new name is not unique.");
                return;
            }
            countChanges++;
        }
        if(portString) countChanges++;
        //------------
        if(countChanges==0)
        {
            errorInCommand("Change what?");
            return;
        }

        if(newType)       r.changeType(serverType);
        if(portString)    r.changePort(convertToULong(portString,"port"));

        changeRasServer(serverName,dbhName,countString,(isExtra ? extraString : NULL), autoRestart, execString);

        if(newServerName) r.changeName(newServerName);

        sprintf(answBuffer,"Ready");
        return;
    }

    const char *hostName = getValueOf("-host");
    if(hostName)
    {
        ServerHost &host=getServerHost(hostName);
        checkUnexpectedTokens();
    }

    bool flagAll = isFlag("-all");
    checkUnexpectedTokens();

    if(hostName == NULL && flagAll == false)
    {
        errorInCommand("No server specified");
        return;
    }

    if(countChanges==0)
    {
        errorInCommand("Change what?");
        return;
    }

    for(int i=0; i<rasManager.countServers(); i++)
    {
        serverName = rasManager[i].getName();
        if(hostName)
            if(!hostCmp(hostName,rasManager[i].getHostName()))
                continue;
        changeRasServer(serverName,dbhName,countString,(isExtra ? extraString : NULL), autoRestart, execString);
    }

    sprintf(answBuffer,"Ready");

}

void RasControl::changeRasServer(const char *serverName, const char *dbhName, const char *countString, const char *extraString, const char *autoRestart, const char* execName)
{
    // called only by by changedServer, after verification of parameters
    RasServer &r = getServer(serverName);

    if(dbhName)
    {
        r.disconnectFromDBHost();
        r.connectToDBHost(dbhName);
    }

    if(countString) r.changeCountDown(convertToULong(countString,"countdown"));

    if(extraString) r.changeExtraParam(extraString);

    if(autoRestart) r.changeAutoRestart( strcasecmp(autoRestart,"on")==0 ? true:false);

    if(execName)    r.changeExecutableName(execName);
}

void RasControl::changeUser()
{
    const char *userName  = getValueOf(RASMGRCMD_USER);
    const char *newName   = getValueIfFlag("-name");
    const char *plainPass = getValueIfFlag("-passwd");
    const char *encrPass  = getValueIfFlag("-encrPasswd");
    const char *rString   = getValueIfFlag("-rights",true);
    checkUnexpectedTokens();

    // encr passwd has priority
    const char *newPass   = encrPass ? encrPass:plainPass;
    bool  takeEncrPass    = encrPass ? true:false;

    bool okChName  =false;
    bool okChPasswd=false;
    bool okChRights=false;
    int admRights = 0;
    int dtbRights = 0;

    //for name change you need acces Control (you cannot change rasadmin)
    //for passwd change you need also acces Control, or, without, only yourself.(only rasadmin can change his passwd)
    if(userName==0)
    {
        errorInCommand("You should provide a valid user name.");
        return;
    }
    User &u=getUser(userName);

    if(newName!=0)
    {
        checkPermission(admR_acctrl);
        if(strcmp(userName,"rasadmin")==0)
        {
            errorInCommand("You cannot change rasadmin's name.");
            return;
        }
        if(userManager.acceptChangeName(userName,newName)==false)
        {
            errorInCommand("The new name is not unique.");
            return;
        }

        okChName=true;
    }

    if(newPass!=0)
    {
        if     (strcmp(userName  ,authorization.getUserName())==0) okChPasswd=true; // may change your own passwd

        else if(strcmp("rasadmin",authorization.getUserName())==0) okChPasswd=true; // rasadmin may change all passwd

        else if(authorization.hasAdminRights(admR_acctrl))
        {
            if (strcmp("rasadmin",userName)!=0) okChPasswd=true; // may change all passwd, except rasadmin's one
            else
            {
                errorInCommand("You don't have permission to change rasadmin's password.");
                return;
            }
        }
        else
        {
            errorInCommand("You don't have permission for this operation.");
            return;
        }

    }
    if(rString!=0)
    {
        checkPermission(admR_acctrl);
        admRights=authorization.convertAdminRights(rString);
        dtbRights=authorization.convertDatabRights(rString);

        if(admRights==-1 || dtbRights==-1)
        {
            errorInCommand("Unknown right in command");
            return;
        }

        okChRights = true;
    }

    // we don't talk so much, just say 'Ready'  answBuffer[0]=0;
    int changes =0;

    if(okChName)
    {
        u.changeName(newName);
        //sprintf(answBuffer,"User name changed: %s is now %s\r\n",userName,newName);
        changes++;
    }
    if(okChPasswd)
    {
        if(answBuffer[0]!=0) strcat(answBuffer,"\r\n");

        if(takeEncrPass) u.changePassword(newPass);
        else             u.changePTPassword(newPass);
        //sprintf(answBuffer+strlen(answBuffer),"Password for user %s was changed\r\n",userName);
        changes++;
    }
    if(okChRights)
    {
        User &user = u; // this is v1.1, is will not live long...
        const char *warning = user.getUserID()!=0 ? "": (admRights=admR_full,"You cannot change rasadmin's system rights\r\n");
        //^sorry, trick
        user.setAdminRights(admRights);
        user.setDefaultDBRights(dtbRights);
        //sprintf(answBuffer+strlen(answBuffer),"%sRights of user %s set to [%s]-[%s]\r\n", warning,userName,authorization.convertAdminRights(admRights),authorization.convertDatabRights(dtbRights));
        changes++;
    }

    if(okChRights || okChPasswd) masterCommunicator.commitAuthFile();

    if(changes==0) errorInCommand("Change what?");
    else           strcpy(answBuffer,"Ready");

}
//---------------------------------------------
void RasControl::upCommand()
{
    const char *what = argc==1 ? "xxx":token[1].take();

    if(strcasecmp(what,RASMGRCMD_SRV)==0)   upRasServers();

    else if(strcasecmp(what,RASMGRCMD_HELP)==0)  upHelp();

    else errorInCommand("Error in UP command");

}

void RasControl::upRasServers()
{
    ENTER( "RasControl::upRasServers()" );

    checkPermission(admR_sysup);

    const char *srvName =getValueOf(RASMGRCMD_SRV);

    if(srvName) // up a specified server
    {
        bool flagForce = config.isDebugSupport() ? isFlag("-force") : false;
        bool flagDebug = config.isDebugSupport() ? isFlag("-debug") : false;

        checkUnexpectedTokens();

        TALK( "server name: " << srvName );
        RasServer &r=getServer(srvName);

        // just debug
        if(flagForce && r.isUp())
        {
            r.forceAvailable();
            sprintf(answBuffer,"Server forced to be available again");
            LEAVE( "RasControl::upRasServers() -- " << answBuffer );
            return;
        }

        if(r.isUp())
        {
            errorInCommand("Server is already up.");
            LEAVE( "RasControl::upRasServers() -- " << answBuffer );
            return;
        }
        int rasp = flagDebug ? r.startServerInDebugger(answBuffer):r.startServer();

        switch(rasp)
        {
        case  0:
            if(flagDebug==false) sprintf(answBuffer,"Server started");
            break;
        case -1:
            errorInCommand("Server is not connected to a database host.");
            break;
        case -2:
            errorInCommand("License violation.");
            break;
        case -3:
        case -4:
            errorInCommand("Cannot contact the slave rasmgr.");
            break;
        case -5:
            errorInCommand("Server host is down.");
            break;
        }
        LEAVE( "RasControl::upRasServers()" );
        return;
    }

    const char *hostName=getValueOf("-host");
    if(hostName)
    {
        checkUnexpectedTokens();

        int rasp = upAllServersOnHost(hostName);
        switch(rasp)
        {
            //case -1: errorInCommand("Wrong server host name."); break;
        case -2:
            errorInCommand("Server host is down.");
            break;

        default:
            sprintf(answBuffer,"Started %d servers on host %s",rasp,hostName);
            break;

        }
        LEAVE( "RasControl::upRasServers()" );
        return;
    }

    bool flagAll = isFlag("-all");

    if(flagAll)
    {
        checkUnexpectedTokens();

        int countUpSrv   = 0;
        int countUpHosts = 0;
        for(int i=0; i<hostmanager.countHosts(); i++)
        {
            int rasp = upAllServersOnHost(hostmanager[i].getName());

            if(rasp>=0)
            {
                countUpSrv+=rasp;
                countUpHosts++;
            }

        }
        sprintf(answBuffer,"Started %d servers on %d hosts",countUpSrv,countUpHosts);
        LEAVE( "RasControl::upRasServers() -- "  << answBuffer );
        return;
    }

    errorInCommand("Up what?");

    LEAVE( "RasControl::upRasServers()" );
}

int RasControl::upAllServersOnHost(const char*hostName)
{
    //return value is negativ =>error
    //return value is positiv =>nr of started servers

    ServerHost &sh= getServerHost(hostName);

    if(sh.checkStatus() == false)
    {
        //errorInCommand("Server host is down.");
        return -2;
    }

    int countStart=0;
    int alreadyUp       = sh.getStartedServers();

    for(int i=0; i<rasManager.countServers(); i++)
    {
        RasServer &r=rasManager[i];
        if(r.isUp()) continue;
        if(!hostCmp(hostName,r.getHostName())) continue;

        if(r.startServer()==0) countStart++;
    }
    // sprintf(answBuffer,"Started %d servers on host %s",countStart,hostName);
    return countStart;
}
//--------------------------------------------------------
void RasControl::downCommand()
{
    const char *what = argc==1 ? "xxx":token[1].take();

    if     (strcasecmp(what,RASMGRCMD_HOST)==0)  downRasHosts();

    else if(strcasecmp(what,RASMGRCMD_SRV)==0)   downRasServers();

    else if(strcasecmp(what,RASMGRCMD_HELP)==0)  downHelp();

    else errorInCommand("Error in DOWN command");

}

void RasControl::downRasServers()
{
    checkPermission(admR_sysup);

    const char *srvName=getValueOf(RASMGRCMD_SRV);

    if(srvName)
    {
        bool killFlag=isFlag("-kill");
        bool forceFlag=isFlag("-force");
        checkUnexpectedTokens();

        RasServer &r= getServer(srvName);
        if(r.isUp()==false && r.isStarting()==false)
        {
            errorInCommand("Server is already down.");
            return;
        }

        int res=killFlag ? r.killServer():r.downServer(forceFlag);

        if(res<0)
        {
            errorInCommand("Cannot contact the slave rasmgr.");
            return;
        }

        if(killFlag) sprintf(answBuffer,"Server %s was killed",srvName);
        else         sprintf(answBuffer,"Server %s is going down",srvName);

        return;
    }

    const char *hostName=getValueOf("-host");

    if(hostName)
    {
        checkUnexpectedTokens();

        int rasp=downAllServersOnHost(hostName);
        switch(rasp)
        {
//      case -1 : errorInCommand("Error in server host name.");
//                break;
        case -2 :
            errorInCommand("Server host is down.");
            break;
        case 0  :
            sprintf(answBuffer,"On host %s all servers are already down",hostName);
            break;
        default : // means >0
            sprintf(answBuffer,"%d servers on host %s are going down",rasp,hostName);
            break;
        }
        return;
    }

    bool flagAll = isFlag("-all");
    if(flagAll)
    {
        checkUnexpectedTokens();

        int countDownSrv   = 0;
        int countDownHosts = 0;
        for(int i=0; i<hostmanager.countHosts(); i++)
        {
            int rasp = downAllServersOnHost(hostmanager[i].getName());

            if(rasp>=0)
            {
                countDownSrv+=rasp;
                countDownHosts++;
            }

        }
        sprintf(answBuffer,"%d servers on %d hosts are going down",countDownSrv,countDownHosts);
        return;
    }

    errorInCommand("Down what?");
}

int RasControl::downAllServersOnHost(const char *hostName)
{
    ServerHost &sh= getServerHost(hostName);

    if(sh.checkStatus()==false)
    {
        //errorInCommand("Server host is down.");
        return -2;
    }

    int countDownServers=0;
    for(int i=0; i<rasManager.countServers(); i++)
    {
        RasServer &r=rasManager[i];
        if(r.isUp()==false && r.isStarting()==false) continue;

        if(!hostCmp(hostName,r.getHostName()))
            continue;
        r.downServer(false);
        countDownServers++;
    }
    return countDownServers;
}

void RasControl::downRasHosts()
{
    // this is the new stop command, but this version is incipient
    checkPermission(admR_sysup);

    const char *hostName=getValueOf(RASMGRCMD_HOST);

    if(hostName)
    {
        checkUnexpectedTokens();
        int rasp = downRasHost(hostName);
        switch(rasp)
        {
        case  0:
            sprintf(answBuffer,"Server host %s is going down", hostName);
            break;
//            case -1: errorInCommand("Wrong server host name.");
//               break;
        case -2:
            errorInCommand("Sorry, you should down all servers on this host first.");
            break;
        case -3:
            errorInCommand("Sorry, you should down all slave hosts first, the master should be the last one.");
            break;
        }
        return;
    }

    bool flagAll = isFlag("-all");

    if(flagAll)
    {
        checkUnexpectedTokens();
        if(rasManager.countUpServers()!=0)
        {
            errorInCommand("Sorry, you should down all servers first.");
            return;
        }

        for(int i=1; i<hostmanager.countHosts(); i++)
        {
            //yes, from 1, master the last!
            downRasHost(hostmanager[i].getName());
        }
        //now the master
        masterCommunicator.shouldExit();
        sprintf(answBuffer,"All hosts are down, bye");
        return;
    }

    errorInCommand("Down what?");
    return;
}

int RasControl::downRasHost(const char *hostName)
{
    ServerHost &sh= getServerHost(hostName);

    if(sh.getStartedServers())
    {
        //errorInCommand("Sorry, you should down all servers on this host first.");
        return -2;
    }

    if(sh.isInternal() && hostmanager.countUpHosts()>1)
    {
        // errorInCommand("Sorry, you should down all slave hosts first, the master should be the last one.");
        return -3;
    }

    if(sh.isInternal()) masterCommunicator.shouldExit();
    else     // later, sorry
    {
        sh.downHost();
    }
    return 0;
}

//----------------------------------------------------
void RasControl::saveCommand()
{
    checkPermission(admR_config);
    checkUnexpectedTokens();

    bool resultConf = config.saveOrigConfigFile();
    bool resultAuth = authorization.saveOrigAuthFile();

    // this has been done by the lines above: -- PB 2003-jun-08
    // masterCommunicator.commitChanges();

    sprintf(answBuffer,"Saving configuration file...%s. Saving authorization file...%s.",
            (resultConf==true ? "ok" : "failed"),
            (resultAuth==true ? "ok" : "failed") );
}

//----------------------------------------------------
void RasControl::resetCommand()
{
    checkPermission(admR_config);
    checkUnexpectedTokens();

    if(config.isTestModus()==false)
    {
        errorInCommand("This operation is possible only in test modus.");
        return;
    }

    if(rasManager.reset()==false)
    {
        errorInCommand("Resetting not possible, there are active servers.");
        return;
    }

    dbHostManager.reset();
    dbManager.reset();

    hostmanager.reset();
    hostmanager.insertInternalHost();

    userManager.reset();
    userManager.loadDefaults();

    VLOG <<"rasmgr was succesfully reset."<<std::endl;
    sprintf(answBuffer,"rasmgr was succesfully reset.");
}

//-----------------------------------------

void RasControl::checkCommand()
{
    const char *what = argc==1 ? "xxx":token[1].take();

    if     (strcasecmp(what,RASMGRCMD_HOST)==0)  checkRasHosts();

    else if(strcasecmp(what,RASMGRCMD_HELP)==0)  checkHelp();

    else errorInCommand("Error in CHECK command");

}

void RasControl::checkRasHosts()
{
    checkPermission(admR_sysup);

    const char *hostName=getValueOf(RASMGRCMD_HOST);

    if(hostName)
    {
        checkUnexpectedTokens();

        ServerHost &sh=getServerHost(hostName);

        sh.checkStatus();
        sprintf(answBuffer,"The host %s is %s",sh.getName(),(sh.isUp() ? "up":"down"));
        return;
    }

    bool flagAll = isFlag("-all");

    if(flagAll)
    {
        checkUnexpectedTokens();
        for(int i=0; i<hostmanager.countHosts(); i++)
        {
            hostmanager[i].checkStatus();
        }
        sprintf(answBuffer,"Checking status of all hosts ... done");
        return;
    }
    errorInCommand("Check what?");
}




//###---###---###---###---###---###---###---###---###---###---###
void RasControl::errorInCommand(const char *errText)
{
    sprintf(answBuffer,"Error: %s",errText);
}

bool RasControl::isCommand( const char *key )
{
    if(argc)
        return (strcasecmp(token[0].argv,key)==0) ? true:false;
    else
        return (strcmp("#",key)==0) ? true:false;
}

void RasControl::splitRequest(const char* reqMessage)
{
    argc=0;
    commandBuffer[0]=' ';
    strncpy(commandBuffer+1,reqMessage,MAXMSG-1);
    commandBuffer[MAXMSG]=0;

    TALK("RasControl::splitRequest: (a) Com="<<commandBuffer);

    char *temp = commandBuffer;

    for(argc=0; argc<30; argc++)
    {
        token[argc].set(strtok(temp," \r\n\t\0"));
        temp=NULL;

        if(token[argc].argv == NULL) break;
        if(token[argc].argv[0] == '#')
        {
            token[argc].argv=NULL; // from here, comment
            break;
        }
    }

}

bool RasControl::isFlag(const char *flag, int pos)
{
    if(pos<0) // doesn't matter
    {
        for(int i=1; i<argc; i++) // flags are from 1->, 0 is the command itself
        {
            if(strcasecmp(flag,token[i].argv)==0)
            {
                token[i].used=true;
                return true;
            }
        }
        return false;
    }
    if(pos>1 && pos<argc)
    {
        if(strcasecmp(flag,token[pos].argv)==0)
        {
            token[pos].used=true;
            return true;
        }
    }
    return false;
}

const char * RasControl::getValueOf(const char *flag, bool acceptMinus)
{
    //no,not always if(flag[0]!='-') return NULL; // all flags start with '-'

    for(int i=1; i<argc-1; i++)
    {
        if(strcasecmp(flag,token[i].argv)==0)
        {
            token[i].used=true;

            if(acceptMinus)
            {
                if(token[i+1].argv[0]=='-' && token[i+1].argv[1]!=0) return NULL; // values don't start with '-' (we don't have minus-signs)
            }                                                              // except in void right string
            else
            {
                if(token[i+1].argv[0]=='-') return NULL;
            }
            token[i+1].used=true;
            return token[i+1].argv;
        }
    }

    if(strcasecmp(flag,token[argc-1].argv)==0) token[argc-1].used=true;

    return NULL; // not found;
}
const char * RasControl::getValueIfFlag(const char *flag,bool acceptMinus)
{
    if(isFlag(flag)==false) return NULL;

    const char *r = getValueOf(flag,acceptMinus);

    if(r==NULL)
    {
        static char temp[30];
        sprintf(temp,"'%s' value",flag);
        throw RCErrorMissingParam(temp);
    }
    return r;
}

void RasControl::checkUnexpectedTokens()
{
    for(int i=2; i<argc; i++)
    {
        if(token[i].used == false) throw RCErrorUnexpToken(token[i].argv);
    }
}
void RasControl::checkPermission(int reqRights)
{
    if(authorization.hasAdminRights(reqRights)==false)
        throw RCErrorNoPermission();
}

void RasControl::checkNotNull(const char *ptr, const char *what)
{
    if(ptr==NULL) throw RCErrorMissingParam(what);
}

RasServer& RasControl::getServer(const char *name)
{
    RasServer &srv = rasManager[name];
    if(srv.isValid()==false) throw RCErrorInvalidName("server");
    return srv;
}
Database& RasControl::getDatabase(const char *name)
{
    Database &db = dbManager[name];
    if(db.isValid()==false) throw RCErrorInvalidName(RASMGRCMD_DATABASE);
    return db;
}
DatabaseHost& RasControl::getDatabaseHost(const char *name)
{
    DatabaseHost &dbh = dbHostManager[name];
    if(dbh.isValid()==false) throw RCErrorInvalidName("database host");
    return dbh;
}
ServerHost& RasControl::getServerHost(const char *name)
{
    TALK( "RasControl::getServerHost( " << name << " )" );
    ServerHost &host = hostmanager[name];
    if(host.isValid()==false) throw RCErrorInvalidName("server host");
    return host;
}
User& RasControl::getUser(const char *name)
{
    User &user = userManager[name];
    if(user.isValid()==false) throw RCErrorInvalidName(RASMGRCMD_USER);
    return user;
}

unsigned long RasControl::convertToULong(const char *stringValue,const char *what)
{
    char *end;
    unsigned long rasp = strtoul(stringValue,&end,0);

    if(strlen(end)!=0) throw RCErrorIncorNumberValue(what);

    return rasp;
}

void RasControl::Token::set(char *p)
{
    argv = p;
    used = false;
}
const char* RasControl::Token::take()
{
    used=true;
    return argv;
}

//######### OBSOLETE in v1.1, but keep them, because could be usefull one day ################################

#ifdef INCLUDE_HIDDEN_COMMANDS
// removed in version 1.1, reimplemented in v1.3, but kept unofficial

void RasControl::listRights()
{
    const char *userName = getValueOf("-user");

    if(userName==NULL)
    {
        errorInCommand("You have to provide an user name");
        //checkPermission(admR_info);
        // list global Initial Rights
        // gi rights hidden in version 1.1 sprintf(answBuffer,"Initial global rights are set to: [%s]-[%s]:",authorization.convertGlobalInitAdminRights(),
        //                           authorization.convertGlobalInitDatabRights());
        return;
    }
    User &user=userManager[userName];

    if(strcmp(userName,authorization.getUserName())!=0)
    {
        checkPermission(admR_info);
    }
    if(user.isValid()==false)
    {
        errorInCommand("Unknown user");
        //sprintf(answBuffer,"Unknown user %s",userName);
        return;
    }
    //RMInit::logOut<<"list rights of user "<<userName<<"  "<<user.getAdminRights()<<"  "<<user.getDefaultDBRights()<<std::endl;
    sprintf(answBuffer,"List of rights defined for user %s",userName);
    sprintf(answBuffer+strlen(answBuffer),"\r\n administrative rights: [%s]",authorization.convertAdminRights(user.getAdminRights()));
    sprintf(answBuffer+strlen(answBuffer),"\r\n default database rights: [%s]",authorization.convertDatabRights(user.getDefaultDBRights()));

    //sprintf(answBuffer+strlen(answBuffer),"\r\n List of defined rights on databases:");


    for(int i=0; i<dbManager.countDatabases(); i++)
    {
        const char *dbName=dbManager[i].getName();

        if(user.isTrusteeOn(dbName))
        {
            int rights=user.getEffectiveDatabaseRights(dbName);
            sprintf(answBuffer+strlen(answBuffer),"\r\n database %-20s [%s]",dbName,authorization.convertDatabRights(rights));
        }
    }

}



void RasControl::grantCommand()
{
    if(argc==1 || strcasecmp(token[1].argv,RASMGRCMD_HELP)==0)
    {
        //grantHelp();
        return;
    }
    checkPermission(admR_acctrl);

    const char *rString =  token[1].take();

    const char *userName= getValueOf("-user");

    const char *dbName  = getValueOf("-db");

    bool gi=isFlag("-gi");

    int admRights=authorization.convertAdminRights(rString);
    int dtbRights=authorization.convertDatabRights(rString);

    if(admRights==-1 || dtbRights==-1)
    {
        errorInCommand("Unknown right in command");
        return;
    }

    if(userName!=NULL)
    {
        User &user=userManager[userName];
        if(user.isValid()==false)
        {
            errorInCommand("Unknown user");
            return;
        }

        if(dbName!=NULL)
        {
            if(user.setDatabaseRights(dbName,dtbRights)==false)
                errorInCommand("Unknown database name");

            else sprintf(answBuffer,"Granted %s to user %s for database %s",rString,userName,dbName);
        }
        else
        {
            const char *warning = user.getUserID()!=0 ? "": (admRights=admR_full,"You cannot change rasadmin's system rights\r\n");
            //^sorry, trick
            user.setAdminRights(admRights);
            user.setDefaultDBRights(dtbRights);
            sprintf(answBuffer,"%sGranted [%s]-[%s] to user %s",warning,authorization.convertAdminRights(admRights),authorization.convertDatabRights(dtbRights),userName);
        }
    }
    else
    {
        if(gi)
        {
            authorization.setGlobalInitAdminRights(admRights);
            authorization.setGlobalInitDatabRights(dtbRights);
            sprintf(answBuffer,"Set global initial rights to [%s]-[%s]",authorization.convertAdminRights(admRights),authorization.convertDatabRights(dtbRights));
        }
        else
        {
            errorInCommand("Error in GRANT command");
        }
    }
}

// removed in version 1.1
void RasControl::revokeCommand()
{
    if(argc==1 || strcasecmp(token[1].argv,RASMGRCMD_HELP)==0)
    {
        //revokeHelp();
        return;
    }
    checkPermission(admR_acctrl);

    const char *userName= token[1].take();

    const char *dbName  = getValueOf("-db");

    User &user=userManager[userName];
    if(user.isValid()==false)
    {
        errorInCommand("Unknown user");
        return;
    }

    if(dbName!=NULL)
    {
        if(user.removeDatabaseRights(dbName)==false)
            errorInCommand("Unknown database name");

        else sprintf(answBuffer,"Rights of user %s for database %s revoked",userName,dbName);
    }
    else
    {
        errorInCommand("Error in REVOKE command");
    }
}

#endif

/*
--------
from list servers

     bool fdbh = isFlag("-dbh",2);
     const char *dbhName  = getValueOf("-dbh");
          if(fdbh) listRasServersOnDBH(dbhName);

     else if(fdb)  listRasServersDatabase(dbName);

     else if(hostName)
       {
         sprintf(answBuffer,"List of servers on host %s:",hostName);

     int crnt=0;
     for(int i=0;i<rasManager.countServers();i++)
           {
         answLen=strlen(answBuffer);

         if(answLen >=maxAnswLen)
           { sprintf(answBuffer+answLen,"\r\n(Answer too long, overflow danger!)");
                 break;
            }
         if(strcmp(hostName,rasManager[i].getHostName())!=0) continue;

             sprintf(answBuffer+answLen,"\r\n%2d. ",crnt++);

         if(fports) rasManager[i].getDescriptionPort(answBuffer+strlen(answBuffer));
             else       rasManager[i].getDescription(answBuffer+strlen(answBuffer));
        }
        }
     else


---------
from list db
     if(isFlag("-dbh",2))
       {
         const char *dbhName=getValueOf("-dbh");

     if(dbhName)
          sprintf(answBuffer,"List of databases on host: %s",dbhName);
         else sprintf(answBuffer,"List of databases:");

     int crnt=0;
         for(int i=0;i<dbManager.countDatabases();i++)
        {
          Database &refTemp=dbManager[i];

              if( dbhName && refTemp.isConnectedToDBHost(dbhName) )
             { sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %s ",crnt++,refTemp.getName());
              }

          if( !dbhName )
             { sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %s",crnt++,refTemp.getName());

           for(int j=0;j<refTemp.countConnectionsToDBHosts();j++)
                 sprintf(answBuffer+strlen(answBuffer),"\r\n         %s ",refTemp.getDBHostName(j));
          }
         }
    }
     else if(isFlag("-srv",2))
       {
         const char *srvName=getValueOf("-srv");

     if(srvName)
          sprintf(answBuffer,"List of databases accesible by server: %s",srvName);
         else sprintf(answBuffer,"List of databases:");

     int crnt=0;
         for(int i=0;i<dbManager.countDatabases();i++)
        {
          Database &refTemp=dbManager[i];

              if( srvName && refTemp.isConnectedToRasServer(srvName) )
             { sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %s ",crnt++,refTemp.getName());
              }

          if( !srvName )
             { sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %s",crnt++,refTemp.getName());

           for(int j=0;j<refTemp.countConnectionsToRasServers();j++)
                  sprintf(answBuffer+strlen(answBuffer),"\r\n         %s ",refTemp.getRasServerName(j));

              }
         }
        }

      else
        { sprintf(answBuffer,"List of databases:");
      int crnt=0;
          for(int i=0;i<dbManager.countDatabases();i++)
        {
          Database &refTemp=dbManager[i];

          sprintf(answBuffer+strlen(answBuffer),"\r\n%2d. %s ",crnt++,refTemp.getName());//,refTemp.isValid());

          sprintf(answBuffer+strlen(answBuffer),"(%dw + %dr) ",refTemp.getWriteTransactionCount(),refTemp.getReadTransactionCount());

         }
      }

---------

*/

