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
 * SOURCE: rasmgr_host.hh
 *
 * MODULE: rasmgr
 * CLASS:  ServerHost, HostManager
 *
 * COMMENTS:
 *   none
 *
*/

#ifndef RASMGR_HOST_HH
#define RASMGR_HOST_HH

#include "rasmgr.hh"
#include "rasmgr_config.hh"

//In normal release there can be just one RasMGR on a host
//But for testing we provide the possibility to put more than one on a host
//Just for fun, we call the unique RasMGR on a host the HIGHLANDER (thanks to Peter Zoller)
//So, for normal release, just...
//#define HIGHLANDER

class ServerHost 
  {
    public:
      ServerHost();
      ~ServerHost();
      void  init(const char* hostName,const char *netwName,int listenport,bool isInternal);      
      const char *getName();
      const char *getNetworkName();
      long        getListenPort();
      bool  isInternal();
      bool  checkStatus();

      int   getStartedServers();
      int   getLicensedServers();

      bool  isUp();
      bool  downHost();
      
      static char* getDescriptionHeader(char *destBuffer);
      char* getDescription(char *destBuffer);
      
      int   countDefinedServers();
      int   getConnectionSocket();
      void  regStartServer();
      void  regDownServer();
      void  setIsUp(bool);
      
      void  useLocalHost(bool);
      bool  useLocalHost();
      
      void  changeName(const char*);
      void  changeNetName(const char*);
      void  changeListenPort(int);

      bool  isValid();
      bool  reset();
    private:
      void setNotUp();
      char hostName[100];
      char netwName[100];
      int  listenPort;
      int  startedServers;
      bool isup;
      bool isinternal;
      bool isuseLocalHost; // if internal, use localhost instead of network name, default on!
            
      bool valid;
   };

class HostManager
  { 
    public:
      HostManager();
      ~HostManager();
      bool insertInternalHost();
      bool checkAcceptAnotherHost();
      bool insertNewHost(const char* hostName,const char *netwName,int listenport);
      bool removeHost(const char *hostName);
      int  countHosts();
      int countUpHosts();
      ServerHost& operator[](int);
      ServerHost& operator[](const char* hostName);
      
      int postSlaveMGR(char *body,char *outBuffer); 
      bool reset(); 
      bool acceptChangeName(const char *oldName,const char *newName);
    private:
      bool testUniqueness(const char* srvName);
      list<ServerHost> hostList;      
      ServerHost       protElem;
   };
   
extern HostManager hostmanager;


#endif

