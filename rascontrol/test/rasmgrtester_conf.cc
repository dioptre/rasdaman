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
 * SOURCE: rasmgrtester_conf.cc
 *
 * MODULE: rascontrol
 * CLASS:  TesterConf
 *
 * PURPOSE:
 *  Configuration from commandline and environment for rasmgrtest
 *
 * COMMENTS:
 *
*/


#include "rasmgrtester_conf.hh"
#include "rascontrol.hh"


TesterConf::TesterConf()
{
    strcpy(rasmgrHost,"localhost");
    rasmgrPort = RASMGRPORT;

    commandListFile[0] =  EOS_CHAR;
    testFile[0] =  EOS_CHAR;

    strcpy(logFile,"rasmgrtest.log");

    workModus=WKMUNKNOWN;
    quiet = false;
}

bool TesterConf::interpretArguments(int argc, char **argv)
{
    if(argc < 2)
    {
        printHelp();
        return false;
    }

    if(argv[1][0] != '-') strcpy(rasmgrHost,argv[1]);

    int optionValueIndex;

    if( checkArguments( argc, argv, "-h", optionValueIndex ) )
    {
        printHelp();
        return false;
    }

    if(checkArguments( argc, argv, "-port", optionValueIndex ) )
    {
        if(optionValueIndex)
            rasmgrPort = strtoul(argv[optionValueIndex],(char**)NULL,10);
    }
    if(checkArguments( argc, argv, "-cf", optionValueIndex ) )
    {
        if(optionValueIndex)
            strcpy(commandListFile,argv[optionValueIndex]);
    }

    if(checkArguments( argc, argv, "-tf", optionValueIndex ) )
    {
        if(optionValueIndex)
            strcpy(testFile,argv[optionValueIndex]);
    }

    if(checkArguments( argc, argv, "-lf", optionValueIndex ) )
    {
        if(optionValueIndex)
            strcpy(logFile,argv[optionValueIndex]);
    }

    if(checkArguments( argc, argv, "-quiet", optionValueIndex ) )
    {
        quiet = true;
    }

    if( commandListFile[0] && testFile[0]) workModus = WKMCREATE;

    else if(testFile[0]) workModus = WKMTSLANG;

    return true;
}
int TesterConf::getWorkModus()
{
    return workModus;
}
const char* TesterConf::getRasMgrHost()
{
    return rasmgrHost;
}

int TesterConf::getRasMgrPort()
{
    return rasmgrPort;
}

const char* TesterConf::getCommandListFile()
{
    return commandListFile;
}
const char* TesterConf::getTestFile()
{
    return testFile;
}
const char* TesterConf::getLogFile()
{
    return logFile;
}

bool TesterConf::beQuiet()
{
    return quiet;
}

void TesterConf::printHelp()
{
    cout<<"Usage: rasmgrtest mainhost [options]"<<endl;
    cout<<"\tmainhost       ... the hostname  where the master RasMgr runs"<<endl;
    cout<<"\t-port          ... the listen port of the master RasMgr (default: "<<RASMGRPORT<<") "<<endl;
    cout<<"\t-cf <file>     ... command list file, from which the test file should be created"<<endl;
    cout<<"                     (if none the test file is used to test the RasMgr)"<<endl;
    cout<<"\t-tf <file>     ... test file"<<endl;
    cout<<"\t-lf <file>     ... log file (default rasmgrtest.log)"<<endl;
    cout<<endl;
}

