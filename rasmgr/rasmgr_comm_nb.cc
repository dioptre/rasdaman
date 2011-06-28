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
 * SOURCE: rasmgr_comm_nb.cc
 *
 * MODULE: rasmgr
 * CLASS:  IOSelector, NbJob, NbServerComm
 *
 * PURPOSE:
 * Performs reliable, non-blocking HTTP communication. used by the master rasmgr
 * Hierarchy: NbServerComm uses NbJob uses IOSelector
 * IOSelector maintains a set of read and write file descriptors (ie, sockets)
 * plus a timeout value common to all of them.
 * NbJob maintains message streams allowing partial (piecewise) read/write.
 * NbServerComm bundles all this, abstracting from the particular socket used.
 *
 * COMMENTS:
 *   none
 *
*/

#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "rasmgr_comm_nb.hh"

#include "debug-srv.hh"
#include "raslib/rminit.hh"

/*************************************************************************
 *
 * CLASS:  IOSelector
 *
 * PURPOSE:
 * IOSelector maintains a set of read and write file descriptors to be watched
 * plus a timeout value common to all of them.
 * ATTENTION: This class does not really care about socket status!
 * Exceptions:
 * - waitForRequest() performs blocking wait (w timeout) for incoming request on the sets.
 * - closeForcedAllSockets() closes all open sockets in the read and write set (should go to NbJob where bind() etc is performed)
 *
 * CHANGE HISTORY (append further entries): [see module header]
 *
 * COMMENTS:
 * - this class is (almost) internal to NbJob
 * - when adding sockets there is no check for valid socket, duplicate insertion, etc
 * - when clearing a socket from a set, there is no check whether it was really in there
 *
 ***********************************************************************/

IOSelector::IOSelector()
{
	ENTER( "IOSelector::IOSelector: enter." );
	FD_ZERO(& watchReadFdSet);
	FD_ZERO(& watchWriteFdSet);
	FD_ZERO(& watchExceptFdSet);
	tvptr = NULL;
	LEAVE( "IOSelector::IOSelector: leave." );
}

void IOSelector::setTimeout(int sec,int millisec)
{
	ENTER( "IOSelector::setTimeout: enter. timeout=" << sec << "." << millisec << " secs." );
	tvinit.tv_sec=sec;
	tvinit.tv_usec=millisec * 1000;
	tvptr=&tv; // yes, yes, &tv
	LEAVE( "IOSelector::setTimeout: leave." );
}

void IOSelector::disableTimeout()
{
	ENTER( "IOSelector::disableTimeout: enter." );
	tvptr=NULL;
	LEAVE( "IOSelector::disableTimeout: leave." );
}

// add socket to the socket set watched for incoming requests
void IOSelector::setReadSocket(int socket)
{
	ENTER( "IOSelector::setReadSocket: enter. add to watchReadSet: " << socket );
	FD_SET(socket,&watchReadFdSet);
	LEAVE( "IOSelector::setReadSocket: leave." );
}

// remove socket from read socket set
void IOSelector::clearReadSocket(int socket)
{
	ENTER( "IOSelector::clearReadSocket: enter. remove from watchReadSet: " << socket );
	FD_CLR(socket,&watchReadFdSet);
	LEAVE( "IOSelector::clearReadSocket: leave." );
}

// add socket to the socket set watched for outgoing requests
void IOSelector::setWriteSocket(int socket)
{
	ENTER( "IOSelector::setWriteSocket: enter. add to watchWriteSet: " << socket );
	FD_SET(socket,&watchWriteFdSet);
	LEAVE( "IOSelector::setWriteSocket: leave." );
}

// remove socket from write socket set
void IOSelector::clearWriteSocket(int socket)
{
	ENTER( "IOSelector::clearWriteSocket: enter. remove from watchWriteSet: " << socket );
	FD_CLR(socket,&watchWriteFdSet);
	LEAVE( "IOSelector::clearWriteSocket: leave." );
}

// result = outcome of select() request:
//	> 0: number of active sockets
//	==0: timeout
//	< 0: error
// preconditions:
// - read set, write set contains valid sockets
// - bind() on each socket in the read and write set

int IOSelector::waitForRequest()
{
	int result;

	ENTER( "IOSelector::waitForRequest: enter." );

	resultReadFdSet =watchReadFdSet;
	resultWriteFdSet=watchWriteFdSet;
	// error unused
	// tv has to be reloaded every time; if tvptr is NULL it doesn't matter
	tv.tv_sec  = tvinit.tv_sec;
	tv.tv_usec = tvinit.tv_usec;    
	TALK( "IOSelector::waitForRequest: timeout=" << tv.tv_sec << "sec " << tv.tv_usec << "microsec." );

	result = select(FD_SETSIZE,&resultReadFdSet,&resultWriteFdSet,NULL,tvptr);
	if (result < 0)
	{
		TALK( "IOSelector::waitForRequest: select error: " << strerror(errno) );
	}
	else // if ( result == 0)
	{
		TALK( "IOSelector::waitForRequest: select() successful, result=" << result );
	}

#if 0  // unsuccessful try
	else // (result > 0)
	{
		TALK( "IOSelector::waitForRequest: select() successful, returned " << result );
		int isPending = 0;
		for (int i=0; i<FD_SETSIZE && isPending==0; i++)
		{  // unfortunately, FD_ISSET() cannot be deployed within an if()
			isPending = (int) FD_ISSET( i, &resultReadFdSet );
			if ( isPending )
			{
				TALK( "IOSelector::waitForRequest: input pending on socket " << i );
				result = i;				// report this socket
			}
			isPending = (int) FD_ISSET( i, &resultWriteFdSet );
			if ( isPending )
			{
				TALK( "IOSelector::waitForRequest: output pending on socket " << i );
				result = i;				// report this socket
			}
		}
	}
#endif // 0

	LEAVE( "IOSelector::waitForRequest: leave. result=" << result );
	return result;    
}

// look into sockets if there is something pending
// precondition: a select() call has been made soon before (ie, socket state hasn't changed inbetween)
// returns:
//	pending socket if found
//	0 otherwise
// cycles through all FDs, read and write interleaved
int IOSelector::someWaitingSocket()
{
	static int lastVisited = 0;			// last inspecrted socket from last call, allows to run round robin strategy
	int waitingSocket = 0;			// some socket waiting to be treated
	bool found = false;				// result of FD_ISSET() call

	ENTER( "IOSelector::someWaitingSocket: enter. lastVisited=" << lastVisited );

	for ( int i=0; i<FD_SETSIZE && waitingSocket==0; i++ )		// walk through max all possible FDs
	{  
		lastVisited = (lastVisited == FD_SETSIZE) ? 0 : lastVisited+1;
		found = ( (int) FD_ISSET( lastVisited, &resultReadFdSet  )
			+ (int) FD_ISSET( lastVisited, &resultWriteFdSet ) ) > 0 ? true : false;
		if ( found )				// cannot use FD_ISSET in if()
			waitingSocket = lastVisited;
	}

	LEAVE( "IOSelector::someWaitingSocket: leave. waitingSocket=" << waitingSocket );
}

bool IOSelector::isReadSocket(int socket)
{
	bool result = (socket < 0) ? false: true;
	if (result == true)
			result = FD_ISSET(socket,&resultReadFdSet);
	return result;
}

bool IOSelector::isWriteSocket(int socket)
{ 
	bool result = (socket < 0) ? false: true;
	if (result == true)
			result = FD_ISSET(socket,&resultWriteFdSet);
	return result;
}
	
void IOSelector::closeForcedAllSockets()
{
	ENTER( "IOSelector::closeForcedAllSockets: enter." );
	for(int i=0;i<FD_SETSIZE;i++)
	{
		if(FD_ISSET(i,&watchReadFdSet) || FD_ISSET(i,&watchWriteFdSet))
		{
			TALK( "IOSelector::closeForcedAllSockets: closing " << i );
			int result = close(i);
			if (result < 0)
			{
				TALK( "IOSelector::closeListenSocket: error closing socket: " << strerror(errno) );
			}
		}
	}
	LEAVE( "IOSelector::closeForcedAllSockets: leave." );
}


// end of class IOSelector ###############################################

/*************************************************************************
 *
 * CLASS:  NbJob
 *
 * PURPOSE:
 * NbJob maintains a message stream (socket) allowing partial (piecewise) read/write.
 * Keeps an IOSelector instance for watching.
 * Outbound messages need a NULL terminator; existence can be checked with isMessageOK().
 *
 * CHANGE HISTORY (append further entries): [see module header]
 *
 * COMMENTS:
 * - sockets must be managed elsewhere: open/select/bind/listen/close
 *
 ***********************************************************************/

// now declared private in .hh
time_t NbJob::timeOutInterv = 30;
time_t NbJob::currentTime = 0;

NbJob::NbJob()
{
	ENTER( "NbJob::NbJob: enter." );
	socket = -1; 
	lastActionTime=0;
	outputBuffer = 0;
	inputBuffer  = 0;
	bigError = false;
	LEAVE( "NbJob::NbJob: leave." );
}

void NbJob::init(IOSelector *pselector,int maxInputBuffer)
{
	ENTER( "NbJob::init: enter. maxInputBuffer=" << maxInputBuffer );
	this->pselector = pselector;
	this->maxInputLength = maxInputBuffer;
	messageTerminator = '\0';
	LEAVE( "NbJob::init: leave." );
}

// dropped (being redundant) in favour of closeConnection() -- PB 2003-jun-04
#ifdef NEVER_AGAIN
void NbJob::reset()
{
	ENTER( "NbJob::reset: enter." );
	clearConnection();
	clearInputBuffer();
	clearOutputBuffer();
	LEAVE( "NbJob::reset: leave." );
}
#endif

void NbJob::clearInputBuffer()
{
	ENTER( "NbJob::clearInputBuffer: enter." );
	if(inputBuffer)
		delete[] inputBuffer;
	inputBuffer = 0;
	nextReadPos = 0;
	LEAVE( "NbJob::clearInputBuffer: leave." );
}

void NbJob::clearOutputBuffer()
{
	ENTER( "NbJob::clearOutputBuffer: enter." );
	if(outputBuffer)
		delete[] outputBuffer;
	outputBuffer = 0;
	nextWritePos = 0;
	LEAVE( "NbJob::clearOutputBuffer: leave." );
}

// clear connection completely, close socket
void NbJob::clearConnection()
{
	ENTER( "NbJob::clearConnection: enter." );
	if(socket > 0)
	{ 
		pselector->clearReadSocket(socket); 
		pselector->clearWriteSocket(socket);
		int result = close(socket);
		int tempErrno = errno;
		TALK( "NbJob::clearConnection: close() on socket " << socket << " returned " << result );
		if (result != 0)
		{
			TALK( "NbJob::clearConnection: error closing socket: " << strerror(tempErrno) );
		}
		socket = -1;
		bigError = false;
	} 	
	LEAVE( "NbJob::clearConnection: leave." );
}
			
// returns true if the current job is too old
bool NbJob::processJobTimeout()
{
	bool result = (messageReadyTime + timeOutInterv > currentTime) ?  false:true;  
	TALK( "NbJob::processJobTimeout: result=" << result );
	return result;  
}      

// on timeout, reset all buffers but don't close socket
bool NbJob::cleanUpIfTimeout()   
{ 
	ENTER( "NbJob::cleanUpIfTimeout: enter." );

	bool result = (socket < 0 ) || (lastActionTime + timeOutInterv > currentTime) ? false : true;

	if (result==true)
	{
		TALK("NbJob::cleanUpIfTimeout: client timeout on socket " << socket); 
		closeConnection();
	}
	
	LEAVE( "NbJob::cleanUpIfTimeout: leave. result=" << result );
	return result;	
}

// reset all that's necessary, then do an accept() to wait [timeout] for incoming calls
// upon success, set socket to the descriptor returned by accept()
// preconditions:
// - socket initialised with bind() etc.
// Note: accept() works like an "open" in that it creates a separate socket which must be closed explicitly!

NbJob::acceptStatus NbJob::acceptConnection(int listenSocket)
{
	ENTER( "NbJob::acceptConnection: enter. listenSocket=" << listenSocket );

	if(socket>0)
	{
		bool result = cleanUpIfTimeout();
		if(result == false)
		{
			LEAVE("NbJob::acceptConnection: leave. cleanUpIfIimeout() returned: no timeout yet.");
			return acs_Iambusy;
			// free again
		}
	}
 
	markAction();

	clearInputBuffer();
	inputBuffer = new char[maxInputLength];
	if(inputBuffer == NULL) 
	{ 
		LEAVE("NbJob::acceptConnection: leave. out of memory.");
		return acs_outofmem;
	}

	struct sockaddr_in internetAddress;
	r_Socklen_t size=sizeof(sockaddr_in);

	// extract the first pending request from the socket queue    
	// NB: accept() clones the socket.
	errno = 0;

// replace accept() code with a simple FD_ISSET() search
#ifndef NEVER_AGAIN
	// accept() here serves to clone the socket.
	// there shouldn't be any wait because of the select() call just passed via waitForRequest()
	socket=accept(listenSocket,(struct sockaddr*)&internetAddress,&size);
	int saveerrno=errno;
	TALK("NbJob::acceptConnection: accept() with socket " << listenSocket << " returned " << socket << ", sin_port=" << htons(internetAddress.sin_port) << ", requestor=" << inet_ntoa(internetAddress.sin_addr) );
	if(socket<0) 
	{ 
		if(saveerrno==EAGAIN)
		{
			LEAVE("NbJob::acceptConnection: leave. no pending connections"); 
		}
		else
		{
			LEAVE("NbJob::acceptConnection: leave. accept error " << strerror(saveerrno) );
		}
		return acs_nopending;
	}        

	// several flags, such as non-blocking, are not inherited accross accept() - see manual
	int val = fcntl(socket,F_GETFL,0);
	val |= O_NONBLOCK;
	fcntl(socket,F_SETFL,val);

	pselector->setReadSocket(socket);

#else // !NEVER_AGAIN -- but currently inactive.

	// int activeSocket = pselector->someWaitingSocket();
	// basically we should always have at least one pending request because a select() came before
	if (activeSocket == 0)
	{
		TALK( "NbJob::acceptConnection: found NO active socket." );
	}
	else
	{
		TALK( "NbJob::acceptConnection: found active socket " << activeSocket );
	}

# endif // NEVER_AGAIN

	LEAVE( "NbJob::acceptConnection: leave. acs_accepted, cloned socket is " << socket);
	return acs_accepted;       
} // acceptConnection()

// this method got the body of reset() which (being redundant) has been dropped -- PB 2003-jun-04
void NbJob::closeConnection()
{
	ENTER( "NbJob::closeConnection: enter." );
	clearConnection();
	clearInputBuffer();
	clearOutputBuffer();
	LEAVE( "NbJob::closeConnection: leave." );
}

// PB 2003-may-29: to fix bug that connections cloned by accept() remain open, so the number grows infinitely   
// ...but this doesn't work yet, don't use it...
void NbJob::closeSocket()
{
	ENTER( "NbJob::closeSocket: enter. socket=" << socket );
	int result = close(socket);
	if (result != 0)
	{
		TALK( "NbJob::closeSocket: error closing socket: " << strerror(errno) );
	}
	socket = -1;
	LEAVE( "NbJob::closeSocket: leave." );
}

void NbJob::markAction()
{
	lastActionTime=currentTime;
}
	
int  NbJob::getSocket()
{
	return socket;
}

bool NbJob::isMessageOK()
{
	return  (nextReadPos > 1 && inputBuffer[nextReadPos - 1] == messageTerminator) ? true:false;
}
	
bool NbJob::wasError()
{
	return bigError;
}

// read from socket into input buffer as much as fits in or until NULL terminator
// returns true iff some bytes could be written
bool NbJob::readPartialMessage()
{ 
	bool messOK;

	ENTER("NbJob::readPartialMessage: enter." );
	markAction();
	errno = 0;
	int nbytes = read(socket,inputBuffer + nextReadPos,maxInputLength - nextReadPos);
	TALK("NbJob::readPartialMessage: read() with socket=" << socket << " returned " << nbytes );

	if(nbytes)	// wrote some bytes
	{ 
		TALK( "NbJob::readPartialMessage: read socket("<<socket<<") "<<nbytes<<" bytes to pos="<<nextReadPos);
		nextReadPos += nbytes;
		inputBuffer[nextReadPos] = 0;
		messOK = isMessageOK();
	
		if(messOK) 
		{
			TALK("NbJob::readPartialMessage: socket read completed on " << socket );
			messageReadyTime = currentTime;
		} 
	}	
	else	// nothing written = error
	{
		int saveerrno=errno;
		switch(saveerrno)
		{
			case EINTR:
				TALK("NbJob::readPartialMessage: read: EINTR, retry please");
				break;
			
			case EAGAIN:
				TALK("NbJob::readPartialMessage: read: EAGAIN, retry please");
				break;
				
			case 0:
				TALK("NbJob::readPartialMessage: read: Premature End-of-file");
				bigError=true;
				break;
			
			default:
				TALK("NbJob::readPartialMessage: read: error " << saveerrno );
				bigError = true;
				break;		
		}
		messOK = false;
	}        

	LEAVE("NbJob::readPartialMessage: leave. read completed=" << messOK );
	return messOK;
} // readPartialMessage()

const char* NbJob::getMessage()
{
	return inputBuffer;
}

// initialise answer sending, copy complete message in local buffer
// set socket to write mode
bool NbJob::initSendAnswer(const char *message)
{
	ENTER("NbJob::initSendAnswer: enter. message=" << message );
	bool result = true;

	markAction();
	clearInputBuffer();
	answerLength = strlen(message)+1;
	outputBuffer = new char[answerLength];
	if (outputBuffer == NULL)
	{
		TALK( "NbJob::initSendAnswer: error: out of memory." );
		result = false;
		// FIXME: close socket?
	}

	if (result == true)
	{
		strcpy(outputBuffer,message);
		nextWritePos = 0;

		pselector->setWriteSocket(socket);
		pselector->clearReadSocket(socket); // sa fie
	}

	LEAVE("NbJob::initSendAnswer: leave. result=" << result );
	return result;    
}

// write current contents of output buffer to socket
// result == true iff writing went fine
bool NbJob::writePartialMessage()
{
	bool result = false;

	ENTER("NbJob::writePartialMessage: enter." );

	markAction();
	errno = 0;
	int nbytes = write(socket,outputBuffer + nextWritePos,answerLength - nextWritePos);
	TALK("NbJob::writePartialMessage: write() with socket=" << socket << " returned " << nbytes );
		
	if(nbytes)
	{ 
		TALK("NbJob::writePartialMessage: write to socket=" << socket << ", " << nbytes << " bytes to pos=" << nextWritePos << ", answerLength=" << answerLength );
		nextWritePos += nbytes;
				
		if(nextWritePos == answerLength) // everything written?
		{ 
			TALK("NbJob::writePartialMessage: write completed.");
			// closeConnection(); // was here, now shifted up the hierarchy -- PB 2003-jun-10
			result =  true;
		}
	}	
	else
	{
		int saveerrno=errno;
		switch(saveerrno)
		{
			case EINTR:
				TALK("NbJob::writePartialMessage: EINTR, retry please");
				break;

			case EAGAIN:
				TALK("NbJob::writePartialMessage: EAGAIN, retry please");
				break;

			case 0:
				TALK("NbJob::writePartialMessage: premature client hang up."); 
				bigError=true;
				break;
			
			default:
				TALK("NbJob::writePartialMessage: error "<< strerror(saveerrno) );
				bigError = true;
				break;		
		}
	}
	LEAVE("NbJob::writePartialMessage: leave. result=" << result );
	return result;    
}

// is socket still open? (actually; wrong name)
bool NbJob::isOperationPending()
{
	bool result = socket > 0 ? true:false;
	TALK("NbJob::isOperationPending (i.e.: socket open) -> " << result );
	return result;
}

void NbJob::printStatus()
{
	TALK( "NbJob::printStatus: socket=" << socket << ", isRead=" << (int) pselector->isReadSocket(socket) << ", isWrite=" << (int) pselector->isWriteSocket(socket) );
}
				
//################################################################################################

NbServerComm::NbServerComm()
{ 
	listenSocket = -1;
	maxJobs = 0;   
	job = 0;
	mypid = getpid();
}

void NbServerComm::initJobs(int maxJobs)
{
	ENTER( "NbServerComm::initJobs: enter. maxJobs=" << maxJobs );

	this->maxJobs = maxJobs;   
	job = new NbJob[maxJobs];

	for(int i=0;i<maxJobs;i++)
	{
		job[i].init(&selector,MAXMSG);
	}

	LEAVE( "NbServerComm::initJobs: leave." );
}

NbServerComm::~NbServerComm()
{
	ENTER( "NbServerComm::~NbServerComm: enter." );
	closeListenSocket();
	LEAVE( "NbServerComm::~NbServerComm: leave." );
}
			
// opens the central listen socket
bool NbServerComm::initListenSocket(int listenPort)
{
	ENTER( "NbServerComm::initListenSocket: enter. listenPort=" << listenPort );

	struct protoent *getprotoptr = getprotobyname("tcp");
	
	struct sockaddr_in name;
	name.sin_family=AF_INET;
	name.sin_port=htons(listenPort);		// translate listen port#
	name.sin_addr.s_addr=htonl(INADDR_ANY);

	listenSocket=socket(PF_INET,SOCK_STREAM,getprotoptr->p_proto);
	if(listenSocket < 0)
	{
		TALK( "NbServerComm::initListenSocket: socket error: " << strerror(errno) );
		exitbyerror("socket");
	}

	// make the socket nonblocking
	int val =fcntl(listenSocket,F_GETFL,0);
	val|=O_NONBLOCK;
	fcntl(listenSocket,F_SETFL,val);

	val =fcntl(listenSocket,F_GETFL,0); 
	if(val & O_NONBLOCK)
		TALK("NbServerComm::initListenSocket: socket " << listenSocket << " is nonblocking" );

#ifdef SO_REUSEADDR
	val = 1;
	int len = sizeof( val );
	if(setsockopt( listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&val, len ))
	{
		TALK( "NbServerComm::initListenSocket: cannot set address reusable using setsockopt: " << strerror(errno) );
	}
#endif    
			
	int sockResult = bind(listenSocket,(sockaddr*)&name,sizeof(name));
	TALK( "NbServerComm::initListenSocket: bind() with socket=" << listenSocket << ", name.port=" << name.sin_port << " returned " << sockResult );
	if (sockResult < 0)
	{
		TALK( "NbServerComm::initListenSocket: cannot set address reusable using bind: " << strerror(errno) );
		exitbyerror("bind");
	}
		
	int queuesize=SOMAXCONN; // the maximum number allowed by SO!!
	sockResult = listen(listenSocket,queuesize);
	TALK("NbServerComm::initListenSocket: listen() with socket=" << listenSocket << ", queuesize=" << queuesize << " returned " << sockResult );
	if(sockResult < 0)
	{
		TALK( "NbServerComm::initListenSocket: listen error: " << strerror(errno) );
		exitbyerror("listen");
	}
		
	selector.setReadSocket(listenSocket);		// add this socket to the read watch list

	LEAVE( "NbServerComm::initListenSocket: leave." );
	return true;
}

void NbServerComm::closeListenSocket()
{
	ENTER( "NbServerComm::closeListenSocket: enter." );
	if(listenSocket>0) 
	{ 
		selector.clearReadSocket(listenSocket);
		TALK( "NbServerComm::closeListenSocket: closing socket " << listenSocket );
		int result = close(listenSocket);
		if (result < 0)
		{
			TALK( "NbServerComm::closeListenSocket: error closing socket: " << strerror(errno) );
		}
		listenSocket = -1;
	} 
	LEAVE( "NbServerComm::closeListenSocket: leave." );
}
			
void NbServerComm::shouldExit()
{
	ENTER( "NbServerComm::shouldExit: enter." );
	exitRequest=true;
	LEAVE( "NbServerComm::shouldExit: leave." );
}
	
bool NbServerComm::mayExit()
{
	ENTER( "NbServerComm::mayExit: enter." );
	bool result = true;

	if(exitRequest==false)
		result = false;
	else
	{
		closeListenSocket(); // we don't accept requests any more

		for(int i=0;i<maxJobs && (result==true);i++)
		{
			if(job[i].isOperationPending())
			result = false; // no, we have pending operations, don't close
		}
	}

	LEAVE( "NbServerComm::mayExit: leave. result=" << result );
	return result;
}

void NbServerComm::lookForTimeout()
{
	ENTER( "NbServerComm::lookForTimeout: enter." );
	for(int i=0;i<maxJobs;i++)
	{
		job[i].cleanUpIfTimeout();
	}
	LEAVE( "NbServerComm::lookForTimeout: leave." );
}

// look through all write jobs and write out pending message
void NbServerComm::dispatchWriteRequest()
{
	ENTER( "NbServerComm::dispatchWriteRequest: enter." );

	int i;
	for(i=0;i<maxJobs;i++)
	{
		int socket=job[i].getSocket();
		if(socket>0)	// active job pending?
		{ 
			if(selector.isWriteSocket(socket))
			{
				TALK( "flushing write job " << i << ", socket " << socket );
				bool result = job[i].writePartialMessage();
				if (result == true)
				{
					TALK( "job done, closing connection " << i << ", socket " << socket );
					job[i].closeConnection(); // was in writePartialMessage() -- PB 2003-jun-10
				}
				else
				{
					TALK( "connection " << i << "write error, socket " << socket );
				}
			}
			else
			{
				TALK( "job " << i << ": socket not writable, nothing to do." );
			}
		}
	} // for

	LEAVE( "NbServerComm::dispatchWriteRequest: leave." );
}

// look through all read jobs and load msg buffers
// NB: as opposed to write, here is no closeSocket! why??
void NbServerComm::dispatchReadRequest()
{
	ENTER( "NbServerComm::dispatchReadRequest: enter." );

	int i;
	for(i=0;i<maxJobs;i++)
	{
		int socket=job[i].getSocket();
		if(socket>0)
		{ 
			if(selector.isReadSocket(socket))
			{
				TALK( "NbServerComm::dispatchReadRequest: flush reading job " << i << ", socket " << socket << " -- NO CLOSE!?!" );
				// result code was not queried, added it -- PB 2004-jul-16
				bool allOk = job[i].readPartialMessage();
				if (allOk)
				{
					TALK( "connection " << i << " done reading, socket " << socket );
					// no close here, connection used for writing afterwards
				}
				else	// could not read
				{
					TALK( "connection " << i << " read error, socket " << socket );
				}
			}
		}
	} // for

	LEAVE( "NbServerComm::dispatchReadRequest: leave." );
}
		
void NbServerComm::connectNewClients()
{
	ENTER( "NbServerComm::connectNewClients: enter." );

	// why only for read sockets? because we process _incoming_ requests
	if(selector.isReadSocket(listenSocket))
	{
		for(int i=0;i<maxJobs;i++)
		{ 
			// we try to connect as many pending connections as possible

			TALK( "NbServerComm::connectNewClients: trying to open #" << i << " of " << maxJobs << " sockets (initially: read)." );
			// the accept() call inside this below will clone the socket, so we get many read sockets
			NbJob::acceptStatus status = job[i].acceptConnection(listenSocket);

			if(status == NbJob::acs_nopending || status == NbJob::acs_outofmem)
			{
				TALK( "NbServerComm::connectNewClients: aborting, bad status:" << status );
				break;
				// first, because there is no pending request, second, because out of mem is not solved by retry 
			}
		} // for
	}
	else
	{
		TALK( "NbServerComm::connectNewClients: master socket is no read socket: " << listenSocket );
	}

	LEAVE( "NbServerComm::connectNewClients: leave." );
}
	
void NbServerComm::closeForcedAllSockets()
{
	ENTER( "NbServerComm::closeForcedAllSockets: enter." );
	if(mypid != getpid())
		selector.closeForcedAllSockets();
	LEAVE( "NbServerComm::closeForcedAllSockets: leave." );
}

void NbServerComm::printStatus()
{
	int i;
	for(i=0;i<maxJobs;i++)
	{
		TALK( "NbServerComm::printStatus: connection #" << i << ":" );
		job[i].printStatus();
	}
	return;
}

//#######################################################
