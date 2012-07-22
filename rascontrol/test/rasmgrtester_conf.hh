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
 * SOURCE: rasmgrtester_conf.hh
 *
 * MODULE: rascontrol
 * CLASS:  TesterConf
 *
 * PURPOSE:
 *  Configuration from commandline and environment for rasmgrtest
 *
 * COMMENTS:
 *
 *
*/

#ifndef RASMGRTESTER_CONF_HH
#define RASMGRTESTER_CONF_HH

#include "rasmgr_utils_conf.hh"
#include "rasmgr_utils_comm.hh"

#define MAXFILENAME 200

// This is the base class for every programs configuration class
class TesterConf:public ConfigurationBase
{
public:
    TesterConf();

    bool interpretArguments(int argc, char **argv);

    const char* getRasMgrHost();
    int         getRasMgrPort();

    int         getWorkModus();
    const char* getCommandListFile();
    const char* getTestFile();
    const char* getLogFile();
    bool        beQuiet();
private:

    void printHelp();

    char rasmgrHost[MAXHOSTNAME];
    int  rasmgrPort;

    char commandListFile[MAXFILENAME];
    char testFile[MAXFILENAME];
    char logFile[MAXFILENAME];

    int workModus;
    bool quiet;

};

extern TesterConf config;

#endif
