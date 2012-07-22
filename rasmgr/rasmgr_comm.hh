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
 * SOURCE: rasmgr_comm.hh
 *
 * MODULE: rasmgr
 * CLASS:  HTTPComm
 *
 * PURPOSE:
 *   Performs reliable, but blocking HTTP communication. used by the slave rasmgr
 *
 * COMMENTS:
 *   Will be removed, the plan is to have only non-blocking communication
 *
*/

#ifndef RASMGR_COMM_HH
#define RASMGR_COMM_HH

#include "rasmgr.hh"

#define MAXMSG 1024
#define MAXMSGOUTBUFF 20000

/**
  * \defgroup Rasmgrs Rasmgr Classes
  */

/**
  * \ingroup Rasmgrs
  */
class HTTPComm
{
public:
    HTTPComm();
    ~HTTPComm();
    void closeListenSocket();
    void shouldExit();
    char *decodeFlag(int statusFlag);
protected:
    int initListenSocket(int port);
    int makeSocket(int port);
    int sendAnswer(int socket,int len);
    int getMessage();

protected:
    bool isMessage(const char *messageStart);
    // pid_t parentPID;
    int listen_socket;
    fd_set active_fd_set,read_fd_set;

    struct timeval tv;
    timeval *tvptr;
    char *header;
    char *body;
    char inBuffer[MAXMSG];
    char outBuffer[MAXMSGOUTBUFF];

    bool exitRequest;
private:
    int realGetMessage();
};

// status flags that rasmgr understands
// - these come from rasserver
#define SERVER_DOWN         0
#define SERVER_AVAILABLE    1
#define SERVER_REGULARSIG   3
// - this comes from slave rasmgr and LSM
#define SERVER_CRASHED      2

// textual representation of the above status flags -- PB 2004-jul-16
#define SERVER_DOWN_TXT       "server down"
#define SERVER_AVAILABLE_TXT  "server available"
#define SERVER_REGULARSIG_TXT "server alive"
#define SERVER_CRASHED_TXT    "server aborted"


/* This two functions where written late in the night, when we came back from BLVA
   (Bayerisches Landesvermessungsamt), where we had problems on DEC+CompaqTrue64
    this &@$! operating system wasn't able to send a message in one piece which was written in one piece
*/
int readWholeMessage(int socket,char *destBuffer,int buffSize);
int writeWholeMessage(int socket,char *destBuffer,int buffSize);

#endif
