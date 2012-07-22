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
 * SOURCE: rasmgr_tester.hh
 *
 * MODULE: rascontrol
 * CLASS:  RasMgrTester
 *
 * COMMENTS:
 *
 *
*/

#ifndef RASMGR_TESTER_HH
#define RASMGR_TESTER_HH

#include "rasmgr_utils_comm.hh"
#include <fstream>

class RasMgrTester
{
public:
    RasMgrTester();
    ~RasMgrTester();
    void setRasMgrHost(const char *rasmgrHost, int port);
    bool mayWeDoTest();

    // load command from string
    bool loadCommand(const char*);

    // load command from file, one line only
    bool loadCommand(std::ifstream&);

    // load expected from string
    bool loadExpected(const char*);

    // load expected from file, until delim, without it
    // if delim==0, until EOF
    bool loadExpected(std::ifstream&,char delim);

    bool sendCommandGetAnswer();

    bool isAnswerOK();

    const char* getCommand();
    const char* getExpected();
    const char* getAnswer();

    bool saveCommand(std::ofstream&);
    bool saveExpected(std::ofstream&);
    bool saveAnswer(std::ofstream&);

private:
    // STL ifstream::getline() drops the '\n', but let's a '\r' live
    // so this function clears this stupid '\r'
    void clearCR(char *line);
    void clearFinalCRLF(char *string);
    UserLogin  userLogin;
    RasMgrClientComm rasmgrClient;

    char *command;
    char *expected;

};
#endif
