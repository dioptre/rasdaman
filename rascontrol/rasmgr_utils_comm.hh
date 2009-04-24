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
 * SOURCE: rasmgr_utils_comm.hh
 *
 * MODULE: rascontrol
 * CLASS:  RasMgrClientComm, UserLogin
 *
 * PURPOSE:
 *   rasmgr-Client communication and login classes
 *    
 * COMMENTS:
 * - RASMGRPORT should have central definition outside
 *
*/
#ifndef RASMGR_UTILS_COMM_HH 
#define RASMGR_UTILS_COMM_HH 


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>

#include "../rasmgr/ras_crypto.hh"

// now defined vid -D as compile flag -- PB 2003-sep-03
// const int RMANVERSION   = 5100;

const int RASMGRPORT    = 7001;
extern const char* RASMGRPORTS;
extern const char* RASMGRHOST;


const int MAXMSG           = 2048;
const int MAXCOMMAND       =  100;
const int MAXMSGRASCONTROL = 20000;

const int MAXUSERNAME = 100;
const int MAXENCRPASS = 35;
const int MAXHOSTNAME = 100;

const int COMM_CONT = 0;
const int COMM_SEND = 1;
const int COMM_EXIT = 2;
const int COMM_ERR  = 3;
const int COMM_ACDN = 4;

class RasMgrClientComm
  {
   public:
     RasMgrClientComm();
     ~RasMgrClientComm();
     
     void setRasMgrHost(const char *rasmgrHost, int rasmgrPort);
     const char* getRasMgrHost();
     void setUserIdentification(const char *userName, const char *encrPass);
     
     int openSocket();
     void closeSocket();
     
     int sendMessage(const char *message);
     int sendMessageGetAnswer(const char *message, const char **responsePtr);
     const char* readMessage();
     const char* getHeader();
     const char* getBody();
     
   private:
     int writeWholeMessage(int socket,char *destBuffer,int buffSize);
     int readWholeMessage(int socket,char *destBuffer,int buffSize);
     const char *stripBlanks(const char*);
     
     char answerMessage[MAXMSGRASCONTROL];
     char *answerBody;
	       
     char userName[MAXUSERNAME];
     char encrPass[MAXENCRPASS];
     
     int  rasmgrSocket;
     char rasmgrHost[MAXHOSTNAME];
     int  rasmgrPort;
   };
   
//#####################################################################
class UserLogin
  {
    public:
      UserLogin();
      ~UserLogin();
      int interactiveLogin();
      int environmentLogin();
      int quickLogin();
      const char *getUserName();
      const char *getEncrPass();
    private:  
      
      char userName[MAXUSERNAME];
      char encrPass[MAXENCRPASS];
   };      

#endif
