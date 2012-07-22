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
 * SOURCE: rasmgr_tester.cc
 *
 * MODULE: rascontrol
 * CLASS:  RasMgrTester
 *
 * PURPOSE:
 *
 * COMMENTS:
 *
*/

#include "debug-clt.hh"
#include "rascontrol.hh"
#include "rasmgr_tester.hh"


RasMgrTester::RasMgrTester()
{

    command = new char[MAXCOMMAND];
    expected= new char[MAXMSGRASCONTROL];

}
RasMgrTester::~RasMgrTester()
{
    delete[] command;
    delete[] expected;
}

void RasMgrTester::setRasMgrHost(const char *rasmgrHost, int rasmgrPort)
{
    rasmgrClient.setRasMgrHost(rasmgrHost, rasmgrPort); // The first argument is the RasMgr host
    userLogin.quickLogin();
    rasmgrClient.setUserIdentification(userLogin.getUserName(),userLogin.getEncrPass());
}

bool RasMgrTester::mayWeDoTest()
{
    int result = 0;                             // COMM_* values
    const char *modus = NULL;

    result = rasmgrClient.sendMessageGetAnswer( RASMGRCMD_LIST_MODUS, &modus );
    const char *expect = "RasMGR running as master in test modus";
    if(modus == NULL) return false; //if connection refused, no rasmgr running
    //cout<<"A:"<<modus<<endl;
    //cout<<"E:"<<expect <<endl;
    if(strcasecmp(modus,expect)!=0)
    {
        //cout<<"RasMgr is not running in test modus, so we can't do the test!"<<endl;
        return false;
    }
    return true;
}

bool RasMgrTester::loadCommand(const char *x)
{
    strncpy(command,x,100);
    return true;
}
bool RasMgrTester::loadCommand(std::ifstream &ifs)
{
    command[0] =  EOS_CHAR;
    ifs.getline(command,MAXCOMMAND);
    clearCR(command);
    return command[0] ? true:false;
}

bool RasMgrTester::loadExpected(const char *x)
{
    strncpy(expected,x,MAXMSGRASCONTROL);
    clearFinalCRLF(expected);
    return true;
}
bool RasMgrTester::loadExpected(std::ifstream &ifs,char delim)
{
    int i;
    const std::streamoff off=-1;
    for(i=0; i<MAXMSGRASCONTROL; i++)
    {
        char c;
        ifs.read(&c,1);
        if(!ifs) break;

        if(c==delim)
        {
            ifs.seekg(off, std::ios::cur);
            break;
        }
        expected[i]=c;
    }
    expected[i]=0;

    clearFinalCRLF(expected);

    return strlen(expected) ? true:false;
}

bool RasMgrTester::sendCommandGetAnswer()
{
    int result = 0;                             // COMM_* values
    const char *r = NULL;

    result = rasmgrClient.sendMessageGetAnswer(command, &r );
    if(r == NULL)
        return false;

    return true;
}

bool RasMgrTester::isAnswerOK()
{
    const char *r=rasmgrClient.getBody();

    return  strcasecmp(r,expected)==0 ? true:false;
}
const char* RasMgrTester::getCommand()
{
    return command;
}

const char* RasMgrTester::getExpected()
{
    return expected;
}

const char* RasMgrTester::getAnswer()
{
    return rasmgrClient.getBody();
}

bool RasMgrTester::saveCommand(std::ofstream &ofs)
{
    ofs<<command<<endl;
    return true;
}
bool RasMgrTester::saveExpected(std::ofstream &ofs)
{
    ofs<<expected<<endl;
    return true;
}
bool RasMgrTester::saveAnswer(std::ofstream &ofs)
{
    ofs<<rasmgrClient.getBody()<<endl;
    return true;
}

void  RasMgrTester::clearCR(char *line)
{
    // This func clears the CR from the end of the line read by getline
    int len = strlen(line);
    if(len==0) return;
    if(line[len-1]=='\r') line[len-1]=0;
}
void  RasMgrTester::clearFinalCRLF(char *string)
{
    // reading the expected answer from a file can bring and ending CRLF, which has to be removed
    int len = strlen(string);
    if(len<2) return;
    if(string[len-1]=='\r' || string[len-1]=='\n') string[len-1]=0;
    if(string[len-2]=='\r' || string[len-2]=='\n') string[len-2]=0;
}

