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
 * SOURCE: akgnet_nbcomm.cc
 *
 * MODULE: akg network
 * CLASS:  NbJob, NbServerJob, NbClientJob, NbCommunicator
 *
 * COMMENTS:
 *
*/

#include <akgnet_nbcomm.hh>
#include <assert.h>

//### NBJob - static members #########################
time_t akg::NbJob::timeOutInterv = 30;
time_t akg::NbJob::currentTime = 0;

void akg::NbJob::setCurrentTime() throw()
  {
    currentTime = time(NULL);
   }
   
void akg::NbJob::setTimeoutInterval(time_t x) throw()
  {
    timeOutInterv = x;
   }
   
time_t akg::NbJob::getTimeoutInterval() throw()
  {
    return timeOutInterv;
   }
   
//####################################################   
akg::NbJob::NbJob(FileDescriptor &fd) throw()
:fdRef(fd)
  {
    status           = wks_notdefined;
    selectorPtr      = NULL;
    currentBufferPtr = NULL;
    lastActionTime   = 0;
   }

akg::NbJob::~NbJob() throw()
  {
   }

akg::NbJob::workingStatus akg::NbJob::getStatus()  throw()
  { 
    return status;
   }

bool akg::NbJob::isOperationPending() throw()
  { 
    return (status != wks_notdefined && 
            status != wks_accepting) ? true:false;
   }
   
bool akg::NbJob::isAccepting() throw()
  { 
    return status == wks_accepting ? true:false;
   }
bool akg::NbJob::isReading() throw()
  { 
    return status == wks_reading ? true:false;
   }
bool akg::NbJob::isWriting() throw()
  { 
    return status == wks_writing ? true:false;
   }
bool akg::NbJob::isProcessing() throw()
  { 
    return status == wks_processing ? true:false;
   }

bool akg::NbJob::readPartialMessage() throw()
  { 
    assert(currentBufferPtr != NULL);
      
    action();
    
    int nbytes = currentBufferPtr->read(fdRef);
    
    if(nbytes>0)
      { 
        DBTALK("..read socket("<<fdRef()<<") "<<nbytes);
	return validateMessage();
       }	
   
    else
      { int saveerrno = fdRef.getErrno();
        switch(saveerrno)
          {
	    case EINTR:  //DBTALK("EINTR, retry please");
	                 break;
			 
	    case EAGAIN: //DBTALK("EAGAIN, retry please");
	                 break;
			 	
	    //case 0:      DBTALK("Premature End-of-file");
	    		 // executeOnReadError() ???
	//		 break;
			 
	    default:     DBTALK("Read error "<<saveerrno);
	                 executeOnReadError();
	                 break;		
	   }
	}        
    return false;
   }

bool akg::NbJob::writePartialMessage() throw()
  {
    assert(currentBufferPtr != NULL);
    
    action();
    int nbytes = currentBufferPtr->write(fdRef);
    
    if(nbytes>0)
      { 
        DBTALK("..write socket("<<fdRef()<<") "<<nbytes);
        
        if(currentBufferPtr->getNotSendedSize()==0)
	  { 
	    DBTALK("Write ready");
	    executeOnWriteReady();
	    return true;
	   }
        }	
     else
       {int saveerrno = fdRef.getErrno();
        switch(saveerrno)
          {
	    case EINTR:  //DBTALK("EINTR, retry please");
	                 break;
			 
	    case EAGAIN: //DBTALK("EAGAIN, retry please");
	                 break;
			 	
	    //case 0:      DBTALK("Premature partner hang up"); //?? valabil la write
	    //		 break;
			 
	    default:     DBTALK("Write error "<<saveerrno);
	                 executeOnWriteError();
	                 break;		
	   }
        }  
    return  false;    
   }
   
bool akg::NbJob::cleanUpIfTimeout() throw()
  {
    if(fdRef.isOpen() == false ) return false;
    if(lastActionTime + timeOutInterv > currentTime) return false;
        
    DBTALK("Client socket "<<fdRef()<<" timeout"); 
    clearConnection();
    
    //********************
    specificCleanUpOnTimeout();
    //********************
    return true;	
   }

void akg::NbJob::clearConnection() throw()
  {
    if(fdRef.isOpen() && selectorPtr)
      { selectorPtr->clearRead(fdRef()); 
	selectorPtr->clearWrite(fdRef()); 
	fdRef.close();
      } 	
   }
      
void akg::NbJob::action()  throw()
  { lastActionTime = currentTime;
   }
   
int  akg::NbJob::getSocket() throw()
  { return fdRef();
   }

void akg::NbJob::executeOnAccept() throw()
  {
   }
bool akg::NbJob::setReading() throw()
  {
    if(selectorPtr == NULL) return false;
    selectorPtr->setRead(fdRef());    
    status = wks_reading;
    return true;
   }

bool akg::NbJob::setWriting() throw()
  {
    if(selectorPtr == NULL) return false;
    selectorPtr->setWrite(fdRef());    
    status = wks_writing;
    return true;
   }

int akg::NbJob::getErrno() throw()
  {
    return fdRef.getErrno();
   }

//##################################################################   
akg::NbServerJob::NbServerJob()  throw()
:NbJob(serverSocket)
  {
   }

void akg::NbServerJob::initOnAttach(Selector *pSelector) throw()
  {
    selectorPtr = pSelector;
   }
  

akg::NbJob::acceptStatus akg::NbServerJob::acceptConnection(ListenSocket& listenSocket) throw()
  {    
    DBTALK("Am intrat in accepting");
    assert(currentBufferPtr != NULL);
    
    if(status != wks_accepting) return acs_Iambusy;
    action();
    
    if(serverSocket.acceptFrom(listenSocket) == false) 
      { 
        int saveerrno = serverSocket.getErrno();
	if(saveerrno==EAGAIN) DBTALK("No pending connections");
	else                  DBTALK("Accept error "<<saveerrno);
	return acs_nopending;
       }        
    
    serverSocket.setNonBlocking(true);      
       	
    setReading();
    
    executeOnAccept();
    DBTALK("Accept: Socket="<<fdRef());        
    return acs_accepted;       
   }


akg::SocketAddress akg::NbServerJob::getClientSocketAddress() throw()
  {
    return serverSocket.getPeerAddress();
   }
   
akg::HostAddress akg::NbServerJob::getClientHostAddress() throw()
  { 
    return serverSocket.getPeerAddress().getHostAddress();
   }

void akg::NbServerJob::readyToWriteAnswer() throw()
  { 
    currentBufferPtr->clearToWrite();
     
    selectorPtr->clearRead(serverSocket());
    selectorPtr->setWrite(serverSocket());
    action();
    
    status = wks_writing;
   }
//##################################################################

akg::NbClientJob::NbClientJob() throw()
:NbJob(clientSocket)
  {
   }

bool akg::NbClientJob::connectToServer(const char* serverHost,int serverPort) throw()
  {
    if(clientSocket.open(serverHost,serverPort))
      {
	clientSocket.setNonBlocking(true);
	selectorPtr->setWrite(clientSocket());
	status = wks_writing;
	action();
	return true;
       }
    return false;     
   }
      
void akg::NbClientJob::initOnAttach(Selector *pselector) throw()
  {
    selectorPtr = pselector;
    
    if(status == wks_writing)
      { selectorPtr->setWrite(clientSocket());
       }
   }

akg::NbJob::acceptStatus 
akg::NbClientJob::acceptConnection(ListenSocket&) throw()
  {
    // we don't accept connections
    return acs_Iambusy;
   }

void akg::NbClientJob::readyToReadAnswer() throw()
  {
    currentBufferPtr->clearToRead();
    
    selectorPtr->clearWrite(clientSocket());
    selectorPtr->setRead(clientSocket());
    action();
    
    status = wks_reading;
   }

//##################################################################
akg::NbCommunicator::NbCommunicator() throw()
  {
    maxJobs = 0;
    jobPtr = NULL;
   }
akg::NbCommunicator::NbCommunicator(int newMaxJobs)
  {
    jobPtr = NULL;
    initJobs(newMaxJobs);
   }

bool akg::NbCommunicator::initJobs(int newMaxJobs)
  { 
    if(jobPtr != NULL) return false;
    maxJobs = newMaxJobs;   
    jobPtr  = new JobPtr[maxJobs];

    for(int i=0;i<maxJobs;i++)
      { jobPtr[i]=0;
       }
    return true;
   }

akg::NbCommunicator::~NbCommunicator() throw()
  {
     if(jobPtr != NULL) delete[] jobPtr;
   }

bool akg::NbCommunicator::attachJob(NbJob &newJob) throw()
  {
     int freeSlot  = -1;
     for(int i=0;i<maxJobs;i++)
       {
          if(jobPtr[i]== &newJob) return false; // job e in lista
	  if(jobPtr[i]== NULL && freeSlot ==-1 ) freeSlot = i;
        }
     if(freeSlot ==-1 ) return false;
     
     jobPtr[freeSlot]= &newJob;
     newJob.initOnAttach(&selector);
     return true;
   }

bool akg::NbCommunicator::deattachJob(NbJob &oldJob) throw()
  {
     for(int i=0;i<maxJobs;i++)
       {
          if(jobPtr[i]== &oldJob) 
	    { 
	      jobPtr[i] = NULL; 
	      oldJob.clearConnection();
	      oldJob.initOnAttach(NULL);
	      return true;
	     }
        }
     return false;
   }   
   
bool akg::NbCommunicator::mayExit() throw()
  {
    if(exitRequest == false) return false;
    
    closeSocket(listenSocket); // we don't accept requests any more
    
    for(int i=0;i<maxJobs;i++)
      {
        if(jobPtr[i] == NULL) continue;
	if(jobPtr[i]->isOperationPending()) return false; // no, we have pending 
       }
    return true; // ok, we may exit   
   }

bool akg::NbCommunicator::runServer() throw()
  {
    if(listenPort == 0) return false;
    
    if(initListenSocket(listenPort,true) == false) return false;
   
    return mainLoop();
   }  

bool akg::NbCommunicator::runClient() throw()
  {
    return mainLoop();
   }  

bool akg::NbCommunicator::mainLoop() throw()
  {  
    exitRequest=false;
    
    while( mayExit() == false)
      {
        DBTALK("Waiting for calls");
        
	if(executeBeforeSelect() == false) return false;
	
	int rasp = selector();
	
	akg::NbJob::setCurrentTime();
	
	if(executeAfterSelect() == false) return false;
	
	if(rasp > 0)
	  {  
	    DBTALK("Ringing");
	    // first this, to increase the chance to free a client
            dispatchWriteRequest(); 
            connectNewClients(); 
            dispatchReadRequest();
	    processJobs();
    	    lookForTimeout(); // important!
	   }
	if(rasp == 0)
	  {
	    DBTALK("Timeout");
	    lookForTimeout();
	    if(executeOnTimeout() == false) return false;
	   } 
	if(rasp < 0)
	  {
	    DBTALK("select error: "<<strerror(errno));
	   }     
      }
    return true;
  } 

void akg::NbCommunicator::processJobs() throw()
  {
    DBTALK("process Jobs - entering");

    for(int i=0;i<maxJobs;i++)
      {
        if(jobPtr[i]==NULL) continue;
	
        JobPtr& currentJob = jobPtr[i];
	
	if(currentJob->isProcessing())
	  {
	    DBTALK("job "<<i<<" is processing");
	    
	    currentJob->processRequest();
           }	   
      }  		      
   } 
  
void akg::NbCommunicator::lookForTimeout() throw()
  { 
    DBTALK("Looking for timeout");
    
    for(int i=0;i<maxJobs;i++)
      { 
        if(jobPtr[i]==NULL) continue;
	
        jobPtr[i]->cleanUpIfTimeout();
       }
   }

void akg::NbCommunicator::dispatchWriteRequest() throw()
  {
    DBTALK("Dispatch writing");
    int i;
    for(i=0;i<maxJobs;i++)
      {
        if(jobPtr[i]==NULL) continue;
	
        JobPtr& currentJob = jobPtr[i];
	
	if(currentJob->isWriting())
	  {
    	    DBTALK("job "<<i<<' '<<currentJob->getSocket()<<" is active");
	    if(selector.isWrite(currentJob->getSocket()))
	     {
	       DBTALK("...and may write ");
	       currentJob->writePartialMessage();
	      }
	   }   
       }
   }

void akg::NbCommunicator::dispatchReadRequest() throw()
  {
    DBTALK("Dispatch reading");
    int i;
    for(i=0;i<maxJobs;i++)
      {
        if(jobPtr[i]==NULL) continue;
        
	JobPtr& currentJob = jobPtr[i];
	
	if(currentJob->isReading())
	  {
	    DBTALK("job "<<i<<' '<<currentJob->getSocket()<<" is active");
	    if(selector.isRead(currentJob->getSocket()))
	     {
	       DBTALK("... and has message");
	       currentJob->readPartialMessage();
	      }
	   }   
       }
   }
    
void akg::NbCommunicator::connectNewClients() throw()
  {  
    DBTALK("connect listenSocket="<<listenSocket());
        
    if(selector.isRead(listenSocket()) == false) return;
    
    DBTALK("Client is calling");
    
    akg::NbJob::acceptStatus status;

    for(int i=0;i<maxJobs;i++)
      { 
        if(jobPtr[i] == NULL) continue;
	   
	JobPtr& currentJob = jobPtr[i];
	   
	if(currentJob->isAccepting())
	  {
            // we try to connect as much pending connections as possible
            status = currentJob->acceptConnection(listenSocket);
	
            if(status == akg::NbJob::acs_nopending  ) break;
	    // there is no pending request, 
	    DBTALK("Connected client "<<i<<" on socket "<<currentJob->getSocket());
         }
      }	 
    } 
    
bool akg::NbCommunicator::executeBeforeSelect() throw()
  {
    // false means server exit immediately
    return true;
   }

bool akg::NbCommunicator::executeAfterSelect() throw()
  {
    // false means server exit immediately
    return true;
   }

bool akg::NbCommunicator::executeOnTimeout() throw()
  {
    // false means server exit immediately
    return true;
   }

