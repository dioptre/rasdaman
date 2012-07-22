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
*/

using namespace std;

#include <unistd.h>
#include <netdb.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <sstream>

#include "debug.hh" // ENTER, LEAVE, TALK
#include "globals.hh"   // DEFAULT_PORT, LOGDIR, LOG_SUFFIX
#include "rasserver_config.hh"

#include "storagemgr/sstoragelayout.hh"
#include "servercomm/httpserver.hh"

Configuration configuration;

Configuration::Configuration()
{
    logToStdOut = true;
    logFileName = 0;
}


bool Configuration::parseCommandLine(int argc, char** argv)
{
    CommandLineParser &cmlInter  = CommandLineParser::getInstance();
    initParameters();

    try
    {
        myExecutable = argv[0];
        cmlInter.processCommandLine(argc, argv);

        if(cmlHelp->isPresent())
        {
            printHelp();
            exit( -2 ); // Unix code for 'help' -- PB 2005-sep-18
        }
        checkParameters();
    }

    catch(CmlException &ex)
    {
        cout<<"Error: " << ex.what()<<endl;

        if(!logToStdOut)
        {
            RMInit::logOut<<"Error: " << ex.what()<<endl;
        }
        return false;
    }
    return true;
}

// just for shorter lines...
#define NSN CommandLineParser::noShortName

void Configuration::initParameters()
{
    CommandLineParser &cmlInter  = CommandLineParser::getInstance();

    cmlHelp     = &cmlInter.addFlagParameter('h', "help", "print this help");

    //connection
    cmlRsn      = &cmlInter.addStringParameter(NSN, "rsn", "<srv-name> rasserver instance name");
    cmlPort     = &cmlInter.addStringParameter(NSN, "lport", "<nnnn> rasserver listen port (RPC or HTTP)");
    cmlMgr      = &cmlInter.addStringParameter(NSN, "mgr", "<mgr-host> name of RasMGR host", DEFAULT_HOSTNAME);
    cmlMgrPort  = &cmlInter.addLongParameter(NSN, "mgrport", "<nnnn> rasmgr port", DEFAULT_PORT );
    cmlMgrSync  = &cmlInter.addStringParameter(NSN, "sync", ""); // deprecated

    cmlTransBuffer = &cmlInter.addLongParameter(NSN, "transbuffer", "<nnnn> maximal size of the transfer buffer in bytes", MAX_BUFFER_SIZE);
    cmlTimeOut     = &cmlInter.addLongParameter(NSN, "timeout", "<nnn> client time out in seconds.\n\t\tif it is set to 0 server doesn't check for client timeouts", CLIENT_TIMEOUT);
    cmlMgmntInt    = &cmlInter.addStringParameter(NSN, "mgmntint", ""); // deprecated
    cmlHttp        = &cmlInter.addFlagParameter(NSN, "http", "start HTTP version of rasserver");
    cmlRnp         = &cmlInter.addFlagParameter(NSN, "rnp", "start RNP version of rasserver");

    cmlOptLevel    = &cmlInter.addLongParameter(NSN, "opt", "<nn> optimization level (0-3)\n\t\t 0 = no / 3 = maximum optimization (see manual)", 3L);
    cmlConnectStr  = &cmlInter.addStringParameter(NSN, "connect", "<connect-str> connect string for underlying database(e.g. test/test@julep)", "/");
    cmlUserStr  = &cmlInter.addStringParameter('u', "user", "<username> database connection user (empty by default)", "");
    cmlPasswdStr  = &cmlInter.addStringParameter('p', "passwd", "<password> database connection password (empty by default)", "");
    cmlLog     = &cmlInter.addStringParameter('l', "log", "<log-file> log is printed to <log-file>\n\t\tif <log-file> is stdout , log output is printed to standard out", "$RMANHOME/log/<srv-name>.<pid>.log");

    cmlTileSize = &cmlInter.addLongParameter(NSN, "tilesize", "<nnnn> specifies maximal size of tiles in bytes\n\t\t-regular tiles with equal edge lengthes",  2097152);
    cmlPctMin   = &cmlInter.addLongParameter(NSN, "pctmin", "<nnnn> specifies minimal size of blobtiles in bytes",  2048);
    cmlPctMax   = &cmlInter.addLongParameter(NSN, "pctmax", "<nnnn> specifies maximal size of inlinetiles in bytes",  4096);
    cmlUseTC    = &cmlInter.addFlagParameter(NSN, "usetc", "use TileContainerIndex");
    cmlTileConf = &cmlInter.addStringParameter(NSN, "tileconf", "<dom> default tile configuration (e.g. [0:1,0:2])", "[0:511,0:511]");

    string tilingDesc = string("<tiling-name> retiling strategy, specified as:") + CommandLineParameter::descLineSep +
                        "  " + tiling_name_notiling          + "," + CommandLineParameter::descLineSep +
                        "  " + tiling_name_regulartiling;
    cmlTiling   = &cmlInter.addStringParameter(NSN, "tiling", tilingDesc.c_str(), tiling_name_notiling);

    string indexDesc  = string("<index-name> index for created objects, specified as:") + CommandLineParameter::descLineSep +
                        "  " + index_name_auto              + "," + CommandLineParameter::descLineSep +
                        "  " + index_name_directory         + "," + CommandLineParameter::descLineSep +
                        "  " + index_name_regdirectory      + "," + CommandLineParameter::descLineSep +
                        "  " + index_name_rplustree         + "," + CommandLineParameter::descLineSep +
                        "  " + index_name_regrplustree      + "," + CommandLineParameter::descLineSep +
                        "  " + index_name_tilecontainer     + "," + CommandLineParameter::descLineSep +
                        "  " + index_name_regcomputed;
    cmlIndex    = &cmlInter.addStringParameter(NSN, "index", indexDesc.c_str(), index_name_rplustree);

    // for systemtest use e.g.3 together with tileSize 12
    string indexsizeDesc = string("<nnnn> make the index use n nodes");
    cmlIndexSize = &cmlInter.addLongParameter(NSN, "indexsize", indexsizeDesc.c_str(),0L);
#ifdef RMANDEBUG
    cmlDbg   = &cmlInter.addStringParameter('d', "debug", "<dgb-file> debug output is printed to <dbg-file>; if <dbg-file> is stdout, debug output is printed to standard out","<srv-name>.log");
    cmlDbgLevel  = &cmlInter.addLongParameter(NSN, "dl", "<nn> debug level (0-4; 0 = no / 4 = maximal debug information)", 0L);
#endif // RMANDEBUG

}
#undef NSN

void Configuration::checkParameters()
{
    ENTER( "Configuration::checkParameters()" );

    serverName = cmlRsn->getValueAsString();

    initLogFiles();

    listenPort = cmlPort->getValueAsLong();

    rasmgrHost = cmlMgr->getValueAsString();
    rasmgrPort = cmlMgrPort->getValueAsLong();
    TALK( "rasmgrHost = " << rasmgrHost << ", rasmgrPort = " << rasmgrPort );

    deprecated(cmlMgrSync);

    maxTransferBufferSize = cmlTransBuffer->getValueAsLong();
    timeout               = cmlTimeOut->getValueAsLong();
    deprecated(cmlMgmntInt);
    httpServ              = cmlHttp->isPresent();
    rnpServ               = cmlRnp->isPresent();

    deprecated(cmlOptLevel);
    dbConnection = cmlConnectStr->getValueAsString();
    dbUser       = cmlUserStr->getValueAsString();
    dbPasswd     = cmlPasswdStr->getValueAsString();

    tileSize   = cmlTileSize->getValueAsLong();
    pctMin     = cmlPctMin->getValueAsLong();
    pctMax     = cmlPctMax->getValueAsLong();
    useTC      = cmlUseTC->isPresent();


    tileConf   = cmlTileConf->getValueAsString();//(r_Minterval..)
    tilingName = cmlTiling->getValueAsString();
    indexType  = cmlIndex->getValueAsString();
    indexSize  = cmlIndexSize->getValueAsLong();

#ifdef RMANDEBUG
    //  deprecated(cmlDbg);     // will certainly not remove this... -- PB 2007-may-07
    // SET_OUTPUT( cmlDbg->isPresent() );   // enable trace macros depending on --debug parameter
    SET_OUTPUT( true );
    // dbgLevel   = cmlDbgLevel->getValueAsLong();
    dbgLevel   = 4;
#endif

    LEAVE( "Configuration::checkParameters()" );
}

void Configuration::printHelp()
{
    CommandLineParser &cmlInter  = CommandLineParser::getInstance();

    cout << "Usage:   rasserver [options]" << endl;
    cout << "Options:" << endl;
    cmlInter.printHelp();

    cout << endl;

}

void
Configuration::initLogFiles()
{
    if( cmlLog->isPresent())
    {
        if( strcasecmp(cmlLog->getValueAsString(), "stdout") != 0)
        {
            logFileName = cmlLog->getValueAsString();
            logToStdOut = false;
        }
        else
        {
            logFileName = "stdout";
            logToStdOut = true;
        }
    }
    else
    {
        // default
        logFileName = makeLogFileName( serverName, LOG_SUFFIX );
        logToStdOut = false;
    }

    if( logToStdOut == true)
    {
        RMInit::logOut.rdbuf(cout.rdbuf());
        RMInit::dbgOut.rdbuf(cout.rdbuf());
        RMInit::bmOut.rdbuf(cout.rdbuf());
    }
    else
    {
        if (RMInit::logFileOut.is_open())
            RMInit::logFileOut.close();

        RMInit::logFileOut.open(logFileName, ios::out | ios::ate);
        RMInit::logOut.rdbuf(RMInit::logFileOut.rdbuf());
        RMInit::dbgOut.rdbuf(RMInit::logFileOut.rdbuf());
        RMInit::bmOut.rdbuf(RMInit::logFileOut.rdbuf());
    }
}

const char *
Configuration::makeLogFileName(const char *srvName,const char *desExt)
{
    static char logfilePath[ FILENAME_MAX ];
    int pid =getpid();
    mkdir( LOGDIR, S_IRWXU + S_IRGRP+S_IXGRP + S_IROTH+S_IXOTH ); // create if not exist, rwxr-xr-x
    int pathLen = snprintf( logfilePath, FILENAME_MAX, "%s/%s.%06d.%s", LOGDIR, srvName, pid, desExt );
    if (pathLen >= FILENAME_MAX)
    {
        logfilePath[FILENAME_MAX-1] = '\0'; // force-terminate string before printing
        cerr << "Warning: path name longer than allowed by OS, likely log file cannot be written: " << logfilePath << endl;
    }
    return logfilePath;
}

void
Configuration::deprecated(CommandLineParameter *cml)
{
    if(cml->isPresent())
    {
        cout<<"WARNING: parameter '"<<cml->calledName()<<"' is deprecated, will be removed in next version!"<<endl;
    }
}

const char* Configuration::getServerName()
{
    return serverName;
}
int         Configuration::getListenPort()
{
    return listenPort;
}
bool        Configuration::isHttpServer()
{
    return httpServ;
}
bool        Configuration::isRnpServer()
{
    return rnpServ;
}

const char* Configuration::getRasmgrHost()
{
    return rasmgrHost;
}
int         Configuration::getRasmgrPort()
{
    return rasmgrPort;
}

bool        Configuration::isLogToStdOut()
{
    return logToStdOut;
}

int         Configuration::getMaxTransferBufferSize()
{
    return maxTransferBufferSize;
}
int         Configuration::getTimeout()
{
    return timeout;
}
const char* Configuration::getDbConnectionID()
{
    return dbConnection;
}
const char* Configuration::getDbUser()
{
    return dbUser;
}
const char* Configuration::getDbPasswd()
{
    return dbPasswd;
}

int         Configuration::getDefaultTileSize()
{
    return tileSize;
}
int         Configuration::getDefaultPCTMin()
{
    return pctMin;
}
int         Configuration::getDefaultPCTMax()
{
    return pctMax;
}

int         Configuration::getDefaultIndexSize()
{
    return indexSize;
}

#ifdef RMANDEBUG
int         Configuration::getDebugLevel()
{
    return dbgLevel;
}
#endif

const char* Configuration::getDefaultTileConfig()
{
    return tileConf;
}
const char* Configuration::getTilingScheme()
{
    return tilingName;
}
const char* Configuration::getIndexType()
{
    return indexType;
}
bool        Configuration::useTileContainer()
{
    return useTC;
}


