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
 * SOURCE: rasmgr_master.hh
 *
 * MODULE: rasmgr
 * CLASS:  MasterComm
 *
 * PURPOSE:
 *   Main loop of master rasmgr
 *    
 * COMMENTS:
 *   		None
 *
*/

#ifndef RASMGR_MASTER_HH
#define RASMGR_MASTER_HH

#include "rasmgr_comm_nb.hh"
#include <deque>
#include<string>

class ClientID
  {
    public:
      ClientID();
      void init(const char*);
      
      bool operator==(const ClientID&);
      bool operator!=(const ClientID&);
      
      std::string  getID() const;
      bool    isValid() const;
    private:
      std::string idstring;
      bool valid;
      friend std::ostream& operator<<(std::ostream&, const ClientID&);
   };

      
class ClientQueue
  {
    public:
      ClientQueue();
      ~ClientQueue();
      
      void put(ClientID&, const char *dbName, char serverType, int errorCode);
      
      // the answer is 0 or the errorcode 
      int  canBeServed(ClientID&, const char *dbName, char serverType, bool fake);
      
    private:
      struct ClientEntry
        {
	  bool      activ;
	  ClientID  clientID;
	  std::string    dbName;
	  char      serverType;
	  int       errorCode;
	  time_t    lastAction;
	  time_t    timeLimit;
	  
	  bool      wasfake;
	  
	  ClientEntry();
	  ClientEntry(ClientID &client, const char *dbName, char serverType, int errorCode);
	  
	  bool shouldWeCleanup(bool fake);
	  void updateTime();
	  
	  bool isTimeout();
	 }; 
     
     std::deque<ClientEntry> clients;
      
   };
   
class MasterComm:public NbServerComm
  {
    public:
      MasterComm();
      ~MasterComm();
      void Run();
      void commitChanges();
      void commitAuthFile();
    private:
      bool isMessage(const char *messageStart);
      int  getFreeServer(bool fake);
      	const char* convertAnswerCode(int code);
	
//      int  getFakeFreeServer();
      int  answerAccessDenied();
      int  answerAccessDeniedCode(); 
      void doCommit();
      
      bool commit;
      bool commitAuthOnly;
      
      void processJob( NbJob &currentJob );
      int  processRequest( NbJob &currentJob );

      bool fillInBuffer(const char*);
      char *header;
      char *body;
      char inBuffer[MAXMSG];
      char outBuffer[MAXMSGOUTBUFF];
      
      bool allowMultipleWriteTransactions;
      
      ClientQueue clientQueue;
   };		  	       
   
extern MasterComm masterCommunicator;

#endif
