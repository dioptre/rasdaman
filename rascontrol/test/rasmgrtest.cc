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
 * SOURCE: rasmgrtest.cc
 *
 * MODULE: rascontrol
 * CLASS:
 *
 * PURPOSE:
 *  Utility to test the rasmgr command language.
 *
 * COMMENTS:
 *
 *
 */

#include <fstream>
#include <iomanip>

#define DEBUG_MAIN
#include "debug-clt.hh"

#include "rascontrol.hh"
#include "rasmgr_tester.hh"
#include "rasmgrtester_conf.hh"

TesterConf config;

RasMgrTester rasmgrTester;

#define TESTERCOMMENT '§'

void testCommandLanguage();
void createTestFile();

int main(int argc, char **argv)
{

    if(config.interpretArguments(argc,argv) == false) return 0;

    if(config.getWorkModus() == WKMUNKNOWN )
    {
        cout << "And what should I do?"<<endl;
        return 1;
    }

    if(testIsMessageDigestAvailable("MD5")==false)
    {
        cout<<"No MD5-Algorythm!"<<endl;
        return 2;
    }

    rasmgrTester.setRasMgrHost(config.getRasMgrHost(), config.getRasMgrPort());

    // tre sa testezi modul de lucru al rasmgr, sa nu faci test cind e activ!!
    if(rasmgrTester.mayWeDoTest()==false)
    {
        cout<<"Can't do test while RasMgr is not running in test modus"<<endl;
        return 3;
    }

    if(config.getWorkModus() == WKMCREATE) createTestFile();

    if(config.getWorkModus() == WKMTSLANG) testCommandLanguage();

    return 0;
}

void createTestFile()
{
    // preia history file si face din el testfile-ul
    cout<<"Creating test file "<<config.getTestFile()<<" from command list file "<<config.getCommandListFile()<<endl;
    std::ifstream history(config.getCommandListFile());
    if(!history)
    {
        cout<<"Can't open command list file "<<config.getCommandListFile()<<endl;
        return;
    }

    std::ofstream testofs(config.getTestFile(),ios::out|ios::trunc);
    if(!testofs)
    {
        cout<<"Can't open test file "<<config.getTestFile()<<endl;
        return;
    }

    while(rasmgrTester.loadCommand(history))
    {
        if(rasmgrTester.sendCommandGetAnswer()==false) break;

        rasmgrTester.saveCommand(testofs);
        rasmgrTester.saveAnswer(testofs);
        testofs<<"§§§- new command -§§§"<<endl;
    }

    cout<<"OK"<<endl;
}

void removeComments(std::ifstream &ifs)
{
    char *line = new char[MAXCOMMAND];

    for(;;)
    {
        std::streampos poz=ifs.tellg();
        ifs.getline(line,MAXCOMMAND);
        if(!ifs) break;
        if(line[0]!=TESTERCOMMENT)
        {
            ifs.seekg(poz, std::ios::beg);
            break;
        }
    }
    delete[] line;
}
void testCommandLanguage()
{
    bool quiet = config.beQuiet();

    cout<<"Testing the command language"<<endl;
    std::ifstream iftestfile(config.getTestFile());
    if(!iftestfile)
    {
        cout<<"Can't find testfile: "<<config.getTestFile()<<endl;
        return;
    }
    std::ofstream logfile(config.getLogFile(),ios::out|ios::trunc);
    if(!logfile)
    {
        cout<<"Can't open logfile: "<<config.getLogFile()<<endl;
        return;
    }

    int countOk     = 0;
    int countFailed = 0;
    int countTotal  = 0;

    for(int i=0;; i++)
    {
        removeComments(iftestfile);
        if(rasmgrTester.loadCommand(iftestfile) == false) break;
        rasmgrTester.loadExpected(iftestfile,TESTERCOMMENT);
        rasmgrTester.sendCommandGetAnswer();

        if(!quiet) cout   <<std::setw(3)<<i<<". "<<rasmgrTester.getCommand()<<" ... ";
        logfile<<std::setw(3)<<i<<". "<<rasmgrTester.getCommand()<<" ... ";

        if(rasmgrTester.isAnswerOK())
        {
            if(!quiet) cout   <<"OK"<<endl;
            logfile<<"OK"<<endl;
            countOk++;
        }
        else
        {
            if(!quiet) cout   <<"FAILED"<<endl;
            logfile<<"FAILED"<<endl;
            logfile<<"§expected:"<<endl<<rasmgrTester.getExpected()<<'*'<<endl;
            logfile<<"$got:"<<endl<<rasmgrTester.getAnswer()<<endl;
            countFailed++;
        }

        countTotal++;
    }

    iftestfile.close();
    logfile.close();

    cout<<"Total: "<<countTotal<<" Passed="<<countOk<<" Failed="<<countFailed<<endl;
}

