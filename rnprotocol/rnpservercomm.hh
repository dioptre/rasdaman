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
/*************************************************************
 *
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef RNPSERVERCOMM_HH
#define RNPSERVERCOMM_HH

//#include <rnprotocol.hh>
//#include <rnpembedded.hh>
#include "rnpcommunication.hh"
#include "raslib/error.hh"

using namespace rnp;

class ClientTimer
  {
    public:
      ClientTimer();
      void setTimeoutInterval(int seconds);
      void markAction();
      bool checkForTimeout();
    private:
      time_t interval;
      time_t lastAction;
      bool   enabled;
   };
   
class RnpRasserverJob : public RnpServerJob
  {
    public:
      RnpRasserverJob() throw();
      
    private:
      bool validateMessage() throw();
      void executeOnAccept() throw();
      void executeOnWriteReady() throw();
      void specificCleanUpOnTimeout() throw();
      void executeOnReadError() throw();
      void executeOnWriteError() throw();
   };
   
class RnpRasDaManComm : public RnpBaseServerComm
  {
    public:
      RnpRasDaManComm() throw();
      
      ~RnpRasDaManComm() throw();
      
      void processRequest(CommBuffer *receiverBuffer, CommBuffer *transmiterBuffer, RnpTransport::CarrierProtocol, RnpServerJob *callingJob) throw();

      void setTimeoutInterval(int seconds);
      void checkForTimeout();
      
    private: // inherited from RnpBaseServerComm
      RnpServerJob* createJobs(int howMany);
      
      void decodeFragment() throw( r_Error );
      
      ClientTimer  clientTimer;
    private: // the execution functions:
      void executeConnect();
      void executeDisconnect();
      void executeOpenDB();
      void executeCloseDB();
      void executeBeginTA();
      void executeCommitTA();
      void executeAbortTA();
      void executeIsTAOpen();
      void executeQueryHttp();
      void executeGetNewOId();
      void executeQueryRpc();
      void executeGetNextElement();
      void executeEndTransfer();
      void executeGetNextMDD();
      void executeGetNextTile();
      
      void executeUpdateQuery();
      void executeStartInsertTransMDD();
      void executeInsertTile();
      void executeEndInsertMDD();
      void executeInitUpdate();
      void executeGetTypeStructure();
      void executeStartInsertPersMDD();
      void executeInsertMDD();
      void executeInsertCollection();
      void executeRemoveObjFromColl();
      void executeDeleteObjByOId();
      void executeDeleteCollByName();
      void executeGetCollection();
      void executeGetCollectionOIds();
      void executeGetObjectType();
      void executeSetFormat();
      
      
      void executeCreateCollection();
      void executeCreateMDD();
      void executeExtendMDD();
      void executeGetTileDomains();
      
      void answerr_Error(r_Error&); 
    private: // helper functions  
      void connectClient();
      // reset connection, without reporting availability to rasmgr
      void disconnectInternally();
      // reset connection, with reporting availability to rasmgr
      void disconnectClient();
      void verifyClientID( RnpQuark command ) throw (r_Error);
      int  makeNewClientID();
      
      int  clientID;         // un timestamp, de fapt!
      int  requestCounter;   // numara pachetele trimise de un client
      int  fragmentCounter;  // numara fragmentele trimise de un client
      
      static const int NoClient;
    };

class RasserverCommunicator : public NbCommunicator
  {
    public:
       RasserverCommunicator(RnpRasDaManComm*) throw();
       
    protected:
       bool executeOnTimeout() throw();
       
       RnpRasDaManComm *commPtr;
   };

#endif // RNPSERVERCOMM_HH

