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
 * SOURCE: rasmgr_comm_nb.hh
 *
 * MODULE: rasmgr
 * CLASS:  <many>
 *
 * PURPOSE:
 *   Performs reliable, non-blocking HTTP communication. used by the master rasmgr
 *    
 * COMMENTS:
 *   none
 *
*/
#ifndef RASMGR_COMM_NB_HH
#define RASMGR_COMM_NB_HH

#include "rasmgr_comm.hh"

// this is the NON-BLOCKING version, which will replace the other one

// maximum number of slaves a master mgr can handle
// #define MAXJOBSMASTER 50
#define MAXJOBSMASTER 1


/**
  * \ingroup Rasmgrs
  */
class IOSelector
  {
    public:
      IOSelector();
      void setTimeout(int sec,int milisec);
      void disableTimeout();
      void setReadSocket(int socket);
      void clearReadSocket(int socket);
      void setWriteSocket(int socket);
      void clearWriteSocket(int socket);
    
      int waitForRequest();
      int someWaitingSocket();

      bool isReadSocket(int socket);
      bool isWriteSocket(int socket);
      
      void closeForcedAllSockets(); // useful for childs which don't have to inherit this sockets
    private:
      fd_set watchReadFdSet;
      fd_set watchWriteFdSet;  
      fd_set watchExceptFdSet; // unused but ...
    
      fd_set resultReadFdSet;
      fd_set resultWriteFdSet; 
      fd_set resultExceptFdSet; // unused but ...
    
      struct timeval tvinit;
      struct timeval tv;
      timeval *tvptr;		// set to &tv... for timeout, NULL for no timeout
      
   };

/**
  * \ingroup Rasmgrs
  */
class NbJob
  {
    public:
      NbJob();
      void init(IOSelector *pselector,int maxInputBuffer);
      
      enum acceptStatus
        { acs_nopending = 0,
	  acs_Iambusy   = 1,
	  acs_accepted  = 2,
	  acs_outofmem  = 3,
	  acs_invalidsocket = 4
	 };
      acceptStatus acceptConnection(int listenSocket);

      bool readPartialMessage();
      bool isMessageOK();
      const char *getMessage();
      
      bool initSendAnswer(const char*);
      bool writePartialMessage();
      bool isOperationPending();
      
      int  getSocket();
      const char *getRequestor();	// added -- PB 2004-jul-16

      bool wasError();
      void closeConnection();
      void closeSocket();
      bool cleanUpIfTimeout();
      bool processJobTimeout();
      void printStatus();
      // void reset(); replaced by closeConnection() -- PB 2003-jun-04
      void clearConnection();
    private:
      void clearInputBuffer();
      void clearOutputBuffer();
      int socket;
      IOSelector *pselector;
      
      // reading
      char *inputBuffer;
      int   nextReadPos;
      int   maxInputLength;
      char  messageTerminator;
      // writing
      char *outputBuffer;
      int  answerLength;
      int  nextWritePos;
      // errors
      bool bigError;

      // timing
      time_t lastActionTime;
      time_t messageReadyTime;
      void   markAction();
    // public:  
      static time_t timeOutInterv;
      static time_t currentTime;
      
   };
   
//###################   

/**
  * \ingroup Rasmgrs
  */         
class NbServerComm
  {
    public:
      NbServerComm();
      ~NbServerComm();
      
     // void work();
      void shouldExit();
      void closeForcedAllSockets(); // useful for children which don't have to inherit these sockets
      void printStatus(); 
    protected:
       void initJobs(int maxJobs);
       bool initListenSocket(int port);
       void closeListenSocket();
       
       // void itsRinging();		doesn't exit -- PB 2003-may-04
       void dispatchReadRequest();
       void dispatchWriteRequest();
      
       void connectNewClients();
       
       void lookForTimeout();
       int listenSocket;
      
       NbJob *job;
       int   maxJobs;
      
       volatile bool exitRequest;
       bool mayExit();
      
       IOSelector selector;
       pid_t mypid;
       
   };

#ifdef X86
 #define r_Socklen_t socklen_t
#endif

#ifdef AIX
 #define r_Socklen_t socklen_t
#endif

#ifdef SOLARIS
 #define r_Socklen_t socklen_t
#endif

#ifdef DECALPHA 
 #define r_Socklen_t int
#endif 

#endif   
