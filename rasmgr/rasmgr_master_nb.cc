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
 * SOURCE: rasmgr_master_nb.cc
 *
 * MODULE: rasmgr
 * CLASS:  MasterComm
 *
 * PURPOSE:
 *   Main loop of master rasmgr
 *    
 * COMMENTS:
 * - MasterComm::processRequest() is the central dispatcher for rasmgr requests, recognising and executing them.
 *
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "rasmgr_master.hh"
#include "rasmgr_config.hh"
#include "rasmgr_rascontrol.hh"
#include "rasmgr_users.hh"
#include "rasmgr_host.hh"
#include "rasmgr_localsrv.hh"
#include "rasmgr_srv.hh"

using namespace std;

#include "debug-srv.hh"
#include "raslib/rmdebug.hh"


// from rasmgr_localsrv.cc; should go to a central location -- PB 2003-nov-25
extern char *now();

// rasserver error codes (see errtxts)
// FIXME: should go into a central include file / class -- PB 2003-nov-20
#define MSG_OK			200
#define MSG_OK_STR			"Ok"
#define MSG_UNKNOWNSERVERTYPE	1001
#define MSG_UNKNOWNSERVERTYPE_STR	"Unknown server type"
#define MSG_UNKNOWNACCESSTYPE	1002
#define MSG_UNKNOWNACCESSTYPE_STR	"Unknown access type"
#define MSG_DATABASENOTFOUND	807
#define MSG_DATABASENOTFOUND_STR	"Database not found"
#define MSG_WRITETRANSACTION	806
#define MSG_WRITETRANSACTION_STR	"Write transaction in progress"
#define MSG_NOSUITABLESERVER	805
#define MSG_NOSUITABLESERVER_STR	"No suitable servers started"
#define MSG_SYSTEMOVERLOADED	801
#define MSG_SYSTEMOVERLOADED_STR	"System overloaded"
// the following code means: I got an error code which I don't know (FIXME: reconsider nr!)
#define MSG_ILLEGAL		999
#define MSG_ILLEGAL_STR			"Internal error: Illegal response code."

// here I start collecting rasmgr protocol tokens to eventually gather them all in one include file
#define RASMGRPROT_EOL		"\r\n"
#define RASMGRPROT_DOUBLE_EOL	"\r\n\r\n"

// time [secs] until a client can be freed from the pending request list after a fake request
#define WAITTIME_AFTER_FAKE	2
// time increment [secs] of iterated timeout (see updateTime())
#define WAITTIME_REPEATED	3


MasterComm::MasterComm()
  { commit=false;
    allowMultipleWriteTransactions = false;
   }
   
MasterComm::~MasterComm()
  { 
   }

void MasterComm::Run()
  {
    ENTER("MasterComm::Run: enter." );
    RMTIMER( "MasterComm", "Run" );			// benchmark this routine, if enabled

    initListenSocket(config.getListenPort());		// connect/bind the central listen socket
							// using IOSelector level here!

    initJobs(MAXJOBSMASTER);				// init jobs structure (nothing with sockets here)
    
    allowMultipleWriteTransactions = config.allowMultipleWriteTransactions();
    
    selector.setTimeout( config.getPollFrequency() , 0 );
    
    VLOG << "Entering server mode, prepared to receive requests." << endl << endl;

    while(mayExit()==false)
    {         
        TALK("MasterComm::Run: new request cycle, status before processing is:" );
        printStatus(); 

        if(commit)
            doCommit();
	   
	int answerLen=0;

        TALK("MasterComm::Run: (c) Waiting...");

        // wait for incoming requests, using select()
        int r=selector.waitForRequest();		// 0 is timeout, <0 error, >0 success
							// again, IOSelector level here!

	TALK("MasterComm::Run: (d) It's ringing..." << r);

	localServerManager.cleanChild(); // sa fie

	if(r<0) 					// nothing to read
	  {
            TALK("MasterComm::Run: (f1) it's a signal (or a socket failure)...");
	    continue;
          }
        if(r==0)					// timeout, nothing to read
	  {
            TALK("MasterComm::Run: (f2) nothing, look for timeouts...");
            lookForTimeout();
	    continue;
	  }
	if(r>0)						// something is pending
	  { 
	  
	    TALK("MasterComm::Run: (e) Got request, r=" << r << "...");

            // iterate over all jobs to see what we can read / reconnect (?) / write
	    dispatchWriteRequest(); // first this, to increase the chance to free a client
            connectNewClients();			// wait for requests, using accept()
            dispatchReadRequest();			// now read in new requests

	    for(int i=0;i<maxJobs;i++)
              {
                TALK( "- request processing: " << i );	// fake similar entry to benchmark logger
		// creates too large log files, so omit in production:
                // BenchmarkTimer *bPtr = new BenchmarkTimer("request processing");// print job start time to log

                processJob(job[i]);			// this can involve closing the connection!

		// creates too large log files, so omit in production:
                // bPtr->result();				// print stop time to log
              }
          }
      }
    LEAVE("MasterComm::Run: leave." );
   } // Run()

// keep connection open after processing request?
// ...according to request type and comm success, shall we keep socket open?
// Note: this is a bad hack, but I don't want to change the "answer" ret code of processRequest() unless I fully understand it -- PB 2003-jun-10
static bool keepConnection;

void MasterComm::processJob(NbJob &currentJob)
  {
    ENTER( "MasterComm::processJob: enter." );

    if(currentJob.isOperationPending()==false )
      {
        LEAVE( "MasterComm::processJob: leave. isOperationPending=false" );
        return;
      }

    if(currentJob.wasError())				// low-level comm error
      {
        TALK( "MasterComm::processJob: closing connection." );
        currentJob.closeConnection();
        LEAVE( "MasterComm::processJob: leave." );
        return;
      }
		
    if(currentJob.isMessageOK() == false)
      {
        LEAVE( "MasterComm::processJob: leave. isMessageOK=false" );
        return;
      }
    
    if(fillInBuffer(currentJob.getMessage())==false)	// fill msg into answer buffer header + body
      {
        TALK( "MasterComm::processJob: closing connection." );
        currentJob.closeConnection();
        LEAVE( "MasterComm::processJob: leave. fillInBuffer=false" );
        return;
      }

    // now we have the message in inBuffer, with header and body set correctly		

    int answer = processRequest( currentJob );	

    int outLen = strlen(outBuffer);
    
    if(outLen && answer != 2) // sending the answer
					// FIXME: what is answer==2 ? never happens! -- PB 2003-jun-10
      { 
        TALK( "MasterComm::processJob: init sending answer for outBuffer, set socket to write mode." );
        currentJob.initSendAnswer(outBuffer);
      }
      
    if(outLen == 0) // no answer to send
      { 
        TALK( "MasterComm::processJob: no answer to send, closing connection." );
        currentJob.closeConnection();
      }  

     if( answer == 1 ) // means "delayedOperation"
					// FIXME: according to processRequest, delOp is 0 !!! -- PB 2003-jun-10
					// ...and 1 comes back for POST rasservernewstatus
       { // the only known until now
        TALK( "MasterComm::processJob: delayedOp, therefore changeServerStatus()." );
         rasManager.changeServerStatus(body);
       }	  
     /* Two words about this delayedOperation. If a remote server crashes, the remote rasmgr
        sends a message and does not wait for answer. But if the master rasmgr restarts 
	immediately the crashed server, it could come to a deadlock. Maybe not, but we
	wish to avoid any possibility, so we close first the connection and then attempt
	to restart the server.
      */	

    // EXPERIMENTAL: close sockets as soon and as always as possible
    // if (! keepConnection)		// singleton request or comm error
      // {
        // TALK( "MasterComm::processJob: singleton request, closing connection." );
        // currentJob.closeConnection();
      // }

    LEAVE( "MasterComm::processJob: leave." );
   }  // processJob() 


// printClientAddr(): aux fct to print client address to designated stream
const char *getClientAddr( int mySocket )
{
	const char *result = NULL;
	struct sockaddr_in s;
	socklen_t sockaddrSize = (socklen_t) sizeof(s);
	if ( getpeername( mySocket, (struct sockaddr*)&s, &sockaddrSize ) != 0)
		result = strerror(errno);
	else
		result = inet_ntoa(s.sin_addr);
	return result;
}

// process request which has been prepared in inBuffer
// if 'verbose' is enabled in configuration then requests will be logged.
// returns 
//	0 normally (?)
//	1 for POST rasservernewstatus
// NB: keepConnection is static above -- bad hack, see there
int MasterComm::processRequest( NbJob &currentJob )
{ 
	ENTER( "MasterComm::processRequest: enter. inBuffer=" << inBuffer );

	// inBuffer: header + body Ok, output in outBuffer, which is not initialized here
	outBuffer[0]=0;		// mark outBuffer as empty
	int answer = 0;	
	// delayedOperation = 0;

	bool fake = false;	// getfreeserver request really wants to allocate a new server?
   
	// --- this is the central dispatcher for rasmgr requests, recognising and executing them.

	if(isMessage("POST rasmgrslave")) 
	{
		VLOG << now() << " slave rasmgr request from "
		     << getClientAddr( currentJob.getSocket() )
		     << ": '" << body << "'..." << flush;

		hostmanager.postSlaveMGR(body,outBuffer);	// prepare outBuffer from body for send to slave
		keepConnection = false;				// master mgr passes thru, it's singleton commo, so don't keep conn
								// FIXME: is this really correct?? -- PB 2003-jun-10
		VLOG << "ok" << endl;
	}
	else if(isMessage("POST rasservernewstatus"))
	{
		// extract server status from msg body
		char serverName[50];
		int  newstatus = 0;
		long dummy = 0;
		serverName[0] = '\0';	// initialize in case sscanf() fails

		int result = sscanf( body, "%s %d %ld", serverName, &newstatus, &dummy);
		if (result == 3)				// we simply ignore malformed requests, reason see below
		{
			const char *statusText = NULL;
			switch (newstatus)
			{
				case SERVER_DOWN:
					statusText = SERVER_DOWN_TXT;
					break;
				case SERVER_AVAILABLE:
					statusText = SERVER_AVAILABLE_TXT;
					break;
				case SERVER_REGULARSIG:
					statusText = SERVER_REGULARSIG_TXT;
					break;
				case SERVER_CRASHED:
					statusText = SERVER_CRASHED_TXT;
					break;
				default:
					statusText = "(unknown message flag)";
					break;
			}
			if (newstatus != SERVER_REGULARSIG && newstatus != SERVER_AVAILABLE)	// don't blow up the log file with "still alive" signals
			{
				VLOG << now() << " status info from server " << serverName
				     << " @ " << getClientAddr( currentJob.getSocket() )
				     << ": '" << statusText << "'...ok" << endl;
			}

			keepConnection = false;			// singleton msg slave -> master
			answer = 1; 
		}
		else // malformed request
		{
			VLOG << now() << " Error: malformed request (ignoring it) from "
			     << getClientAddr( currentJob.getSocket() )
			     << ": '" << body << "'" << endl;
		}
	}
	else if( (fake = isMessage("POST getfreeserver2")) || isMessage("POST getfreeserver")) 
	{ 
		VLOG << now() << " client request from "
		     << getClientAddr( currentJob.getSocket() )
		     << ": " << "'get server'..." << flush;

		if(authorization.acceptEntry(header))
		{
			int rc = getFreeServer(fake);	// returns std rasdaman errors -- FIXME: error ignored!
			keepConnection = (rc == MSG_OK) ? true : false;	// 200 is "ok"
			VLOG << "ok" << endl;
	  	}
		else
		{
			answerAccessDeniedCode();
			keepConnection = false;		// this is a final answer, don't keep conn open afterwards
			VLOG << "denied." << endl;
		}   

	}	  
	else if(isMessage("POST rascontrol")) 
	{ 
		VLOG << now() << " rascontrol request from "
		     << getClientAddr( currentJob.getSocket() )
		     << ": '" << body << "'..." << flush;

		if(authorization.acceptEntry(header))
		{
			rascontrol.processRequest(body,outBuffer);	        
			keepConnection = true;		// rascontrol connection accepted, so keep it
			VLOG << "ok" << endl;
		}
		else
		{
			answerAccessDenied();
			keepConnection = false;		// this is a final answer, don't keep conn open afterwards
			VLOG << "denied." << endl;
		}
	}	     
	
	LEAVE( "MasterComm::processRequest: leave. answer=" << answer << ", keepConnection=" << keepConnection );
	return answer;
} // processRequest()

// fillInBuffer: fill parameter string passed into message header and body (both global)
// separator is a double newline
// FIXME: unstable and weird programming, improve! -- PB 2003-jun-10
// input:
//	s	message input string
// returns:
//	true	filled buffer properly
//	false	NULL body string
//	inBuffer	(global buffer) set to s; header string part properly NULL terminated in inBuffer
//	body	(global ptr) set to beginning of message body in inBuffer
//	header	(global ptr) set to beginning of inBuffer
bool MasterComm::fillInBuffer(const char *s)
{  
	strcpy(inBuffer,s);
	header=inBuffer;				// set header to begining of msg buffer
	body=strstr(inBuffer, RASMGRPROT_DOUBLE_EOL );	// find double EOL, this is where body starts
	if(body == NULL)				// not found? this means a protocol syntax error
	{
		TALK( "MasterComm::fillInBuffer: Error in rasmgr protocol encountered (2xEOL missing). msg=" << inBuffer );
		return false; // only if client is stupid 
	}

	*body=0;					// terminate header (!) string
	body+= strlen( RASMGRPROT_DOUBLE_EOL );		// let body start after this double newline

	return true;
}

// save config and auth file; deprecated
void MasterComm::doCommit()
{ 
   if(config.isTestModus()==false)
     { 
      TALK( "MasterComm::doCommit: deprecated, should not be called any longer." );
#if 0 // now done by saveCommand() directly
      if(commitAuthOnly==false)
        { VLOG << "Save configuration file...";
          if(config.saveConfigFile()) VLOG << "OK" << std::endl;
          else                        VLOG << "Failed" << std::endl;
	 } 
	    
       VLOG << "Save authorization file...";
       if(authorization.saveAuthFile()) VLOG << "OK" << std::endl;
       else                             VLOG << "Failed" << std::endl;
#endif
      }
   else
     { RMInit::logOut<<"Save requested, but not permitted during test modus!"<<std::endl;
      }   	     
   commit=false;
  }
         
void MasterComm::commitChanges()
  { commit=true;
    commitAuthOnly=false;
   }
void MasterComm::commitAuthFile()
  { commit=true;
    commitAuthOnly=true;
   }
   
int MasterComm::answerAccessDenied()
  { // send to rascontrol when wrong login
    sprintf(outBuffer,"HTTP/1.1 400 Error\r\nContent-type: text/plain\r\nContent-length: %d\r\n\r\nAccess denied",strlen("Access denied")+1);
    return strlen(outBuffer)+1;
   }

int MasterComm::answerAccessDeniedCode()
  { // send to clients requesting free server when wrong login
    sprintf(outBuffer,"HTTP/1.1 400 Error\r\nContent-type: text/plain\r\nContent-length: %d\r\n\r\n802 Access denied",strlen("802 Access denied")+1);
    return strlen(outBuffer)+1;
   }

// input:
//	fake	true if only testing, false if server is to be allocated
//	body	(global var) holding string encoding of request parameters
//		syntax: <dbname> [RPC|HTTP|RNP] [rw|ro] <previousID>
//			where the flags are NOT case sensitive
// returns: standard rasdasman error codes
//	200 (ok), 801, 805, 999, ...
int MasterComm::getFreeServer(bool fake)
{ 
	// creates too large log files, so omit in production:
	// BenchmarkTimer *freeServerTimePtr = new BenchmarkTimer("Get free server");
	
	char databaseName[100];
	char serverType[10]; 
	char serverName[100]; 		// name of rasserver found, if any
	char accessType[5];
	char prevID[200]="(none)";

	ENTER("MasterComm::getFreeServer: enter. fake=" << fake << ", body="<<body<<'*');

	// initialize server name
	strcpy( serverName, "(none)" );

	// extract components from body string
	int count = sscanf(body,"%s %s %s %s",databaseName,serverType,accessType, prevID);
	if (count != 4 && count != 3)
	{
		RMInit::logOut << "Error (internal): Cannot parse msg body received from client." << endl;
		LEAVE("MasterComm::getFreeServer: leave. Fatal error: cannot parse msg body string '" << body << "'" );
		return MSG_ILLEGAL;
	}

	ClientID clientID;
	// if we got a previous ID then take this one
	if(count >3)
		clientID.init(prevID); 
	
	TALK("GetFreeServer: db = " << databaseName << ", requested server type = " << serverType << ", access type = " << accessType << ", clientID="<<clientID<<" prevID="<< prevID );
	
	char sType=0;					// type of server requested, values SERVERTYPE_*
	bool writeTransaction;				// true <=> write transaction requested
	
	int  answCode=MSG_OK;				// request answer code
	const char *answText=MSG_OK_STR;		// string representation of above answer code

	char answerString[200]="";			// response string sent back to caller

	// this loop is executed at most once, it servers only to have
	// a well-defined point of continuation upon evaluation errors
	do	
	{
		// --- evaluate message body ------------------------

		// determine server type requested
		if(strcasecmp(serverType,"HTTP")==0)
			sType=SERVERTYPE_FLAG_HTTP;
		if(strcasecmp(serverType,"RPC")==0)
			sType=SERVERTYPE_FLAG_RPC;
		if(strcasecmp(serverType,"RNP")==0)
			sType=SERVERTYPE_FLAG_RNP;
		if(sType==0)
		{
			RMInit::logOut << "Error: unknown server type: " << serverType << endl;
			answCode=MSG_UNKNOWNSERVERTYPE;
			break;
		}

		// determine transaction mode
		if (strcasecmp(accessType,"ro")==0)
			writeTransaction=false;
		else if (strcasecmp(accessType,"rw")==0)
			writeTransaction=true;
		else
		{
			RMInit::logOut << "Error: unknown transaction type: " << accessType << endl;
			answCode=MSG_UNKNOWNACCESSTYPE;
			break;
		}

		TALK("accessType="<<accessType<<" writeTransaction="<<writeTransaction);       
		
		// --- check against database state ------------------------

		// does requested database exist? (i.e., is it known?)
		Database &db=dbManager[databaseName];
		if(db.isValid()==false)
		{
			RMInit::logOut << "Error: database not found: " << databaseName << endl;
			answCode=MSG_DATABASENOTFOUND;
			break;
		}

		// if r/w TA requested: is this compatible with the database's transaction state?
		if(writeTransaction==true && db.getWriteTransactionCount() && allowMultipleWriteTransactions == false)
		{
			RMInit::logOut << "Error: write transaction in progress, conflicts with request." << endl;
			answCode=MSG_WRITETRANSACTION;
			break;
		}
		
		// --- all fine, try to find a free server ------------------------

		// iterate over registered servers, try to find a free one
		// FIXME: should be "round robin" strategy wrt server hosts;
		// take last used per server host is fine to reduce swapping
		int countSuitableServers=0;		// number of servers we can choose from
		TALK( "starting to search for server of type " << sType << "..." );
		for(int i=0; i<db.countConnectionsToRasServers(); i++)
		{
			// inspect next server
			RasServer &r=rasManager[db.getRasServerName(i)];
			TALK( "  srv #" << i << ": name=" << r.getName() << ", type=" << r.getType() << ", isUp=" << r.isUp() << ", isAvailable=" << r.isAvailable() );
			if(sType == r.getType()) 	// type matches request?
			{ 
				if(r.isUp())		// server is up?
					countSuitableServers++;
			
				if(r.isAvailable())	// server is free?
				{	// part A: we have what you want
					int cbs = clientQueue.canBeServed(clientID, (const char*)databaseName, sType, fake); 
					// returns: 0=OK, otherwise rasdaman errors 801, 805 -- PB 2003-nov-20
					if(cbs != 0)
					{ 
						TALK("MasterComm::getFreeServer: clientQueue.canBeServed(" << clientID << "," << databaseName << "," << sType << "," << fake << ") -> " << cbs );
						RMInit::logOut << "Error: no server available, error code: " << cbs << endl;
						answCode = cbs;
						break;
					}
					if( fake == false)	// server to be allocated?
					{
						// mark server found as unavailable to others
						r.setNotAvailable();
						// set transaction mode requested
						if(writeTransaction==true)
							r.startWriteTransaction(db); // nothing real happens, no error can occur
						else
							r.startReadTransaction(db); // nothing real happens, no error can occur
						TALK("MasterComm::getFreeServer: You have the server.");
						// answCode is same as initialised if we come here
					}
					sprintf(answerString,"%s %ld %s ",r.getHostNetwName(),r.getPort(),authorization.getCapability(r.getName(),databaseName,!writeTransaction) );
					// remember server name
					strncpy( serverName, r.getName(), sizeof(serverName) );
;
					TALK( "answerString=" << answerString );
					break;
				}
			}	 
		} // for

		// any free server found?
		if(countSuitableServers == 0)
		{
			RMInit::logOut << "Error: no suitable free server available." << endl;
			answCode = MSG_NOSUITABLESERVER;
			break;
		}

		// no answer string provided -> no server available
		// oops?? why not uniformly check against answCode? -- PB 2003-nov-20
		if(answerString[0]==0)
		{
			RMInit::logOut << "Error: cannot find any free server; answer code: " << answCode << " -> ";
			answCode = MSG_SYSTEMOVERLOADED;
			RMInit::logOut << answCode << endl;
			break;
		}  
	
	} while(0); // see comment at start of "loop"
		
	answText = convertAnswerCode(answCode);
	 
	if(answCode == MSG_OK)
		sprintf(outBuffer,"HTTP/1.1 %d %s\r\nContent-type: text/plain\r\nContent-length: %d\r\n\r\n%s",answCode,answText,strlen(answerString)+1,answerString);
	else
	{
		sprintf(outBuffer,"HTTP/1.1 %d %s\r\nContent-type: text/plain\r\nContent-length: %d\r\n\r\n%d %s",400,"Error",strlen(answText)+1,answCode,answText);
		clientQueue.put(clientID, (const char*)databaseName, sType, answCode); 
	}
		
	// creates too large log files, so omit in production:
	// freeServerTimePtr->result();		// print time elapsed

	LEAVE("MasterComm::getFreeServer: leave. answCode=" << answCode << ", server=" << serverName << ", outBuffer=" << outBuffer );
	return answCode; //strlen(outBuffer)+1;    	 
} // getFreeServer()

// convertAnswerCode: convert numeric answer code to error text for selected errors + OK
// input:
//	code	numeric error code, cf. errtxts
// returns:
//	answer	ptr to static error text
const char* MasterComm::convertAnswerCode(int code)
{
	const char *answer = MSG_ILLEGAL_STR;	// return value, initialized to "illegal"
	switch(code)
	{
        	case MSG_OK:
			answer = MSG_OK_STR;
			break;
        	case MSG_UNKNOWNSERVERTYPE:
			answer = MSG_UNKNOWNSERVERTYPE_STR;
			break;
        	case MSG_UNKNOWNACCESSTYPE:
			answer = MSG_UNKNOWNACCESSTYPE_STR;
			break;
        	case MSG_DATABASENOTFOUND:
			answer = MSG_DATABASENOTFOUND_STR;
			break;
        	case MSG_WRITETRANSACTION:
			answer = MSG_WRITETRANSACTION_STR;
			break;
        	case MSG_NOSUITABLESERVER:
			answer = MSG_NOSUITABLESERVER_STR;
			break;
        	case MSG_SYSTEMOVERLOADED:
			answer = MSG_SYSTEMOVERLOADED_STR;
			break;
        	default: 
			// RMInit::logOut<<"Default value not allowed ="<<code<<endl; assert( 0 != 0); break;
			// no program aborts deeply inside!!! -- PB 2003-jun-25
                   	answer = MSG_ILLEGAL_STR;
			break;
	}

	TALK("MasterComm::convertAnswerCode: code=" << code << ", answer=" << answer );
	return answer;
}
	
// isMessage: determine whether header conforms with a given prefix; case insensitive
// input:
//	messageStart	prefix to compare with
//	header		(global) message header to be inspected
// returns:
//	true		on match
//	false		otherwise
bool MasterComm::isMessage(const char *messageStart)
{
	ENTER( "MasterComm::isMessage, messageStart=" << messageStart );

	bool rasp= (strncasecmp(header,messageStart,strlen(messageStart))==0) ? true:false;
	if(rasp)
		TALK("(b) Message="<<messageStart);

	LEAVE( "MasterComm::isMessage, result=" << rasp );
	return rasp;
}


//********************************************************************

ClientID::ClientID()
{
	valid = false;
}

   
void ClientID::init(const char *stringrep)
{
	idstring = stringrep;
	valid = true;
}

string ClientID::getID() const
{
	return idstring;
}

bool   ClientID::isValid() const
{
	return valid;
}
            
bool ClientID::operator==(const ClientID& cl)
{
	return (idstring == cl.idstring && valid) ? true : false;
}
   
bool ClientID::operator!=(const ClientID& cl)
{
	return (idstring == cl.idstring && valid) ? false : true;
}    
        
std::ostream& operator<<(std::ostream &os, const ClientID &cl)
{
	os<<cl.getID();
	return os;
}

// ----------------------------------------

/*
list of pending client requests.
requests are entered if for some reason server allocation failed, or if a "fake" request was sent.
*/
// member attributes are defined in rasmgr_master.hh

ClientQueue::ClientQueue()
{
	// do nothing
}

ClientQueue::~ClientQueue()
{
	// do nothing
}

// put: put client request into (static) queue; do nothing if request is malformed
// well formed if:
//	- valid client ID in clientID
//	- server assignment error in errorCode
void ClientQueue::put(ClientID &clientID, const char *dbName, char serverType, int errorCode)
{
	ENTER("ClientQueue::put: start, clientID=" << clientID << ", db=" << dbName << ", serverType=" << serverType << ", errorCode=" << errorCode );

	// --- input parameter check ---------------

	if(clientID.isValid() == false)		// invalid clientID's are not put in queue
		return; 

	if (errorCode != MSG_SYSTEMOVERLOADED
	 && errorCode != MSG_NOSUITABLESERVER
	 && errorCode != MSG_WRITETRANSACTION)	// only these codes are put in queue
        	return;    

	// --- walk through client list to find a matching entry ---------------

	ClientEntry *client = 0;		// ptr to a list entry

	// iterate thru list of client requests
	TALK( "iterating through list, client table size=" << clients.size() );
	for(int i=0;i<clients.size(); i++)
	{
		ClientEntry& curClient = clients[i];	// list entry to be inspected

		// on the fly, remove first list entry if outdated or inactive
		// FIXME: what an ugly code -- PB 2003-nov-20
		if(curClient.activ == false || curClient.isTimeout()) 
		{
			if(i==0) 			// do only for 1st element
			{
				TALK("ClientQueue::put: cleaned up client "<<curClient.clientID );
				clients.pop_front();	// remove this first element
				i--; // set back loop ctr
			}
			continue;
		}

		// have an entry with matching client ID?
		if(curClient.clientID == clientID)
 		{
			client = &clients[i];		// remember this entry
			break;
		}
	} // for

	if(client == 0) 				// no matching entry found
	{
		ClientEntry newClient(clientID, dbName, serverType, errorCode);
		newClient.activ = true;
		newClient.updateTime();
		clients.push_back(newClient);
		TALK("ClientQueue::put, new client first time, id="<<clientID );	
	}
	else						// matching entry found
	{ 
		// Attention: we compare ptrs, not string contents!! -- PB 2003-nov-20
		if(client->dbName == dbName && client->serverType == serverType)
		{	// wants the same thing
			client->errorCode  = errorCode;
			client->updateTime();
			TALK("ClientQueue::put, id=" << clientID << ", db=" << dbName << ", serverType=" << serverType << ": updated" );
		}
		else
		{	// same client, wants something different, is a new client  
			client->activ = false;
			ClientEntry newClient(clientID, dbName, serverType, errorCode);
			newClient.activ = true;
			newClient.updateTime();
			clients.push_back(newClient);
			TALK("ClientQueue::put, known client db=" << dbName << ", serverType=" << serverType << ", but different request: id="<<clientID );	
		} 
	}    	

	LEAVE("ClientQueue::put: done." );	
} // ClientQueue::put()

// canBeServed: determine whether given request can be served 
// by looking into client list; return code indicates yes/no/why not
// returns:
//	0	ok, can be served
//	else	rasdaman error code
int ClientQueue::canBeServed(ClientID &clientID, const char *dbName, char serverType, bool fake)
{ // the answer is the errorcode, why it can't be served
    
	if(clients.size() == 0)
		return 0;

	for(int i=0;i<clients.size();i++)
	{
		ClientEntry& client = clients[i];

		// on the fly, clean first element if necessary
		// FIXME: this is not just as ugly as above, it also duplicates code! -- PB 2003-nov-20
		if(client.activ == false || client.isTimeout()) 
		{
			if(i==0) 
			{ 
				TALK("ClientQueue::canBeServed  id="<<client.clientID<<" cleaned up");
				clients.pop_front();
				i--; 
			}
			continue;
		}
		
		if(client.dbName == dbName && client.serverType == serverType)
		{ // wants the same thing
			if(client.clientID == clientID)
			{	// it's the same client
				// first fake request is not cleaned up.
				// Chances are 99.999% that the same client comes back very quickly with a true request
				if(client.shouldWeCleanup(fake))
				{
					client.activ = false;
					if(i==0) 
					{
						TALK("ClientQueue::canBeServed  id="<<client.clientID<<" cleaned up, you get a server");
						clients.pop_front();
						i--; 
					}
				}
				LEAVE("ClientQueue::canBeServed  id="<<clientID<<" yes (1)");
				return 0; // OK, it can be served
			}
			else // it's another client
			{ 
				if(client.errorCode == MSG_SYSTEMOVERLOADED || client.errorCode == MSG_NOSUITABLESERVER) 
				{	// yes, only these two, 806 (Write trans in progr) is not inherited!
					// If there would be a client waiting because of 806 then:
					// - either we want to write and have also 806, and wouldn't be here at all,
					// - or we want to read and we don't care for that 806
					LEAVE("ClientQueue::canBeServed  id="<<clientID<<" no:"<<client.errorCode);
					return client.errorCode;
				}
				LEAVE("ClientQueue::canBeServed  id="<<clientID<<" yes (r/w)");		   
				return 0; // OK, can be served
			}  
		}
	}

	// if we are here, it can be served. There are clients, but not for the same reason
	LEAVE("ClientQueue::canBeServed id="<<clientID<<" yes (2)");
	return 0;
}

// -----------------------------------------------

/*
 client entry in client request list.
Requests are put into the list only if a server allocation error
has happened before or if a fake request has been made, so that
a retry is needed.
Member attributes:
    activ      this entry active? (manipulated by several list functions external to this class!!)
    serverType type of rasdaman server requested
    errorCode  error code of last call
    timeLimit  time when request is timed out
    lastAction used by updateTime to determine timeout increment
    wasfake    last request was fake
*/

ClientQueue::ClientEntry::ClientEntry()
  {
    activ      = false;
    serverType = 'x';
    errorCode  = 0;
    lastAction = 0;
    timeLimit  = 0;
    wasfake    = false;
   }
   
ClientQueue::ClientEntry::ClientEntry(ClientID &_clientID, const char *_dbName, char _serverType, int _errorCode)
{
	activ      = false;
	clientID   = _clientID;
	dbName     = _dbName;
	serverType = _serverType;
	errorCode  = _errorCode;
	lastAction = 0;
	timeLimit  = 0;
	wasfake    = false;
}

// shouldWeCleanup: does current client need to be removed from pending request list?
// true iff fake || wasfake
// "we admit a first fake request without cleaning up, so the client gots a chance to come back
// with a true request. It's important to do this only for the first time, since a client which 
// loops with openDB could lock the system!!!"
// side effects:
//	if (fake && !wasfake): sets wasfake flag, increments update time
// input:
//	fake	true iff fake request
// returns:
//	true	cleanup recommended
//	false	not recommended
// major changes:
//	single exit logic -- PB 2003-nov-20
bool ClientQueue::ClientEntry::shouldWeCleanup(bool fake)
{
	bool result = false;	// in dubio don't remove

	if(fake == false)
		result = true; // yes, clean up
	else
	{    
		if(wasfake == true)
			result = true; // clean up, there was a fake request already
		else
		{
			wasfake = true;	// why?? -- PB 2003-nov-20

			// update time, but short time limit, client should come quickly
			time_t now = time(NULL);
			timeLimit  = now + WAITTIME_AFTER_FAKE; 

			result = false;
		}
	}

	return result;
}

// updateTime: set new timeout interval
// "we will use an adaptative method. first the client will have 3 sec before timimg out
// than we compute deltaT and give him deltaT + 3. So clients can start by being unpatience and
// than reduce their frequency. This will reduce CPU usage and remain flexible"
void ClientQueue::ClientEntry::updateTime()
{

	time_t now = time(NULL);

	if(lastAction == 0)
	{ // first use
		lastAction = now;
		timeLimit  = now + WAITTIME_REPEATED; // client has WAITTIME_REPEATED seconds time to ask again
	}
	else
	{
		time_t deltaT = now - lastAction;
		lastAction    = now;
		timeLimit     = now + deltaT + WAITTIME_REPEATED;
	}   
}

// isTimeout: has client request reached timeout limit?
bool ClientQueue::ClientEntry::isTimeout()
{
	return timeLimit < time(NULL);
}

