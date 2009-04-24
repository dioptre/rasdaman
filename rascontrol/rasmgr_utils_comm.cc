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
 * SOURCE: rasmgr_utils_comm.cc
 *
 * MODULE: rascontrol
 * CLASS:  RasMgrClientComm, UserLogin
 *
 * PURPOSE:
 *   rasmgr-Client communication and login classes
 *    
 * COMMENTS:
 * 
 *
*/

// trace macros
#include "debug-clt.hh"

#include "globals.hh"

#include "rasmgr_utils_comm.hh"
#include "rascontrol.hh"


RasMgrClientComm::RasMgrClientComm()
  {
    rasmgrSocket=-1;
    userName[0]= EOS_CHAR;
    encrPass[0]= EOS_CHAR;
    rasmgrHost[0]= EOS_CHAR;
    answerBody=answerMessage;
    answerMessage[0]= EOS_CHAR;
   }

RasMgrClientComm::~RasMgrClientComm()
  {
   }
void RasMgrClientComm::setRasMgrHost(const char *rasmgrHost, int rasmgrPort)
  {
    strcpy(this->rasmgrHost,rasmgrHost);
    this->rasmgrPort=rasmgrPort;
   }

const char* RasMgrClientComm::getRasMgrHost()
  { return rasmgrHost;
   }
   
void RasMgrClientComm::setUserIdentification(const char *userName, const char *encrPass)
  {
    strcpy(this->userName,userName);
    strcpy(this->encrPass,encrPass);
   }
   
int RasMgrClientComm::openSocket()
  {
    ENTER("RasMgrClientComm::openSocket: enter." );

    // if open already, close beforehand
    if(rasmgrSocket!=-1)
      {
        TALK ("RasMgrClientComm::openSocket: socket was open, closing it." );
        closeSocket();
      }
 
    struct protoent *getprotoptr = getprotoptr=getprotobyname("tcp");	// FIXME: what is this???
    struct hostent *hostinfo = gethostbyname(rasmgrHost);
    
    if(hostinfo==NULL)
      {
        TALK ("RasMgrClientComm::openSocket: leave. unknown host " << rasmgrHost );
        return -1;
       }
    
    sockaddr_in internetAddress;
    internetAddress.sin_family=AF_INET;
    internetAddress.sin_port=htons(rasmgrPort);
    internetAddress.sin_addr=*(struct in_addr*)hostinfo->h_addr;    

    rasmgrSocket=socket(PF_INET,SOCK_STREAM,getprotoptr->p_proto);
  
    if(rasmgrSocket<0)
      {
        int tempErrno = errno;
        TALK ("RasMgrClientComm::openSocket: leave. error opening socket: " << strerror(tempErrno ) );
        return -1;
      }
  
    if(0>connect(rasmgrSocket,(struct sockaddr*)&internetAddress,sizeof(internetAddress)))
      {
        int tempErrno = errno;
        TALK ("RasMgrClientComm::openSocket: leave. error connecting socket: " << strerror(tempErrno ) );
        return -1;
      }

    LEAVE( "RasMgrClientComm::openSocket: leave. ok." );
    return 0;
   }
     
void RasMgrClientComm::closeSocket()
  {
    ENTER( "RasMgrClientComm::closeSocket. enter." );
    if(rasmgrSocket >0) 
      {
        TALK( "RasMgrClientComm::closeSocket. closing, socket=" << rasmgrSocket );
        close(rasmgrSocket);
        rasmgrSocket=-1;
      }	
    LEAVE( "RasMgrClientComm::closeSocket. leave." );
   }
     
int RasMgrClientComm::sendMessage(const char *message)
  {
    char request[MAXMSG];
    int result = COMM_CONT;			// was: 0, but this is same value

    ENTER( "RasMgrClientComm::sendMessage: enter. message=" << message );

    sprintf(request,                "POST rascontrol HTTP/1.1\r\nAccept: text/plain\r\nUserAgent: rascontrol/2.0");
    sprintf(request+strlen(request),"\r\nAuthorization: ras %s:%s",userName,encrPass);//"rasadmin","d293a15562d3e70b6fdc5ee452eaed40");    
    sprintf(request+strlen(request),"\r\nContent length: %d\r\n\r\n%s ",strlen(message)+1,message);        
    
    int reqLen=strlen(request)+1; 		// including final '\0'!!
    if(writeWholeMessage(rasmgrSocket,request,reqLen)<reqLen)
      { closeSocket();				// redundant, but ok as safety measure^
        TALK( "RasMgrClientComm::sendMessage: cannot write, socket closed." ); 
	result = COMM_ERR;
       }
    
    LEAVE( "RasMgrClientComm::sendMessage: leave. result=" << result );
    return result;			
   } 

const char* RasMgrClientComm::readMessage()
  {
    ENTER( "RasMgrClientComm::readMessage: enter." );

    if(readWholeMessage(rasmgrSocket,answerMessage,MAXMSGRASCONTROL)<0)
      { closeSocket();				// redundant, but ok as safety measure^
        TALK( "RasMgrClientComm::readMessage: cannot read message from rasmgr." );
	answerBody=answerMessage;
	answerMessage[0] = EOS_CHAR;
	return NULL;
       }

    answerMessage[MAXMSGRASCONTROL-1]='\0';
    
    answerBody=strstr(answerMessage,"\r\n\r\n");
    if(answerBody)
      { *answerBody=0;
        answerBody+=4;
       }
    else
        answerBody=answerMessage+strlen(answerMessage);   

    LEAVE( "RasMgrClientComm::readMessage: leave. answerBody=" << answerBody );
    return answerBody;
   }

const char* RasMgrClientComm::getHeader()
  {
    return answerMessage;
   }
   
const char* RasMgrClientComm::getBody()
  {
    return answerBody;
   }

// bugfix: socket was not closed in case of send or receive error (was done in subroutines, dispersed)
int RasMgrClientComm::sendMessageGetAnswer(const char *message, const char **responsePtr)
  { 
    int result = COMM_CONT;			// actually, in the end COMM_* from rasmgr_utils_comm.hh; COMM_CONT means OK
    const char *rcvMsg  = NULL;			// message ptr delivered by readMessage()

    ENTER( "RasMgrClientComm::sendMessageGetAnswer: enter. message=" << message );

    TALK( "RasMgrClientComm::sendMessageGetAnswer: opening socket." );
    result = openSocket();			// open socket to rasmgr
    if ( result < 0 )				// open went wrong?
      {
        TALK( "RasMgrClientComm::sendMessageGetAnswer: cannot open socket." );
        result = COMM_ERR;
      } 
    else					// we have a good socket, proceed
      {
        result = sendMessage(message);		// send message to rasmgr, get a COMM_* answer
        if ( result == COMM_CONT )		// "continue, no error"
            rcvMsg = readMessage();		// receive result from rasmgr
        result = ( result == COMM_CONT && rcvMsg != NULL) ? COMM_CONT : COMM_ERR;
						// FIXME: should be refined
        TALK( "RasMgrClientComm::sendMessageGetAnswer: closing socket." );
        closeSocket();				// close socket again, in any case
						// (due to current implementation, may have been closed before, no problem)
      }

    if (result == COMM_CONT && rcvMsg != NULL)
      {
        rcvMsg = stripBlanks( rcvMsg );
        *responsePtr = rcvMsg;
      }

    LEAVE( "RasMgrClientComm::sendMessageGetAnswer: leave. result=" << result );
    return result;
   }

// strip leading blanks from message string
const char* RasMgrClientComm::stripBlanks(const char *r)
  {
    if (r==NULL)
        return NULL;
    
    const char *s = r;
    while ( *s == ' ' || *s == '\t' )
        s++;

    return s;
   }
   
int RasMgrClientComm::writeWholeMessage(int socket,char *destBuffer,int buffSize)
  {
    ENTER( "RasMgrClientComm::writeWholeMessage: enter. socket=" << socket );

    // we write the whole message, including the ending '\0', which is already in
    // the buffSize provided by the caller
    int totalLength=0;
    int writeNow;
    while(1)
      {
        writeNow = write(socket,destBuffer+totalLength,buffSize-totalLength);
	if(writeNow == -1)
	  { if(errno == EINTR) continue; // read was interrupted by signal
	    
            LEAVE( "RasMgrClientComm::writeWholeMessage: leave. EINTR." );
	    return -1; // another error
	   }
        totalLength+=writeNow;
	
	if( totalLength==buffSize ) break; // THE END	    
       }

    LEAVE( "RasMgrClientComm::writeWholeMessage: leave. totalLength=" << totalLength );
    return totalLength;
   }

int RasMgrClientComm::readWholeMessage(int socket,char *destBuffer,int buffSize)
  {
    ENTER( "RasMgrClientComm::readWholeMessage: enter. socket=" << socket );

    // we read what is comming in until we encounter a '\0'
    // this is our end-sign. 
    int totalLength=0;
    int redNow;
    while(1)
      {
        redNow = read(socket,destBuffer+totalLength,buffSize-totalLength);
	if(redNow == -1)
	  { if(errno == EINTR) continue; // read was interrupted by signal
	    
            LEAVE( "RasMgrClientComm::readWholeMessage: leave. EINTR." );
	    return -1; // another error
	   }
        totalLength+=redNow;
	
	if(destBuffer[totalLength-1]==0) break; // THE END	    
       }

    LEAVE( "RasMgrClientComm::readWholeMessage: leave. totalLength=" << totalLength );
    return totalLength;
   }
   


//################################################################################
UserLogin::UserLogin()
  { 
    userName[0]= EOS_CHAR;
    encrPass[0]= EOS_CHAR;
   }
UserLogin::~UserLogin()
  {
   }
int UserLogin::interactiveLogin()
  {
    std::cerr<<"Login name: ";
    char *rasp=fgets(userName,99,stdin);
    if(!rasp) return -1;
    //strtok(userName,"\r\n"); //removes the ending \r\n
    
    for(int i=0;userName[i];i++)
      {
        if(userName[i]==' ' || userName[i]=='\t' || userName[i]=='\r' || userName[i]=='\n')
	  { userName[i]=0;break;
	   }
       }

    char *plainPass=getpass("  Password: ");
    messageDigest(plainPass,encrPass,"MD5");
    for(int i=0;i<strlen(plainPass);i++) plainPass[i]=0;
    std::cerr<<std::endl;
  
 // cout<<"name="<<username<<" pass="<<encrPass<<endl;
    return 0;
   }
int UserLogin::environmentLogin()
  {
    char *s=getenv("RASLOGIN");
    if(s==NULL) return -1;
    int i;
    
    for(i=0;i<99 && *s!=':' && *s ;i++,s++)
      { 
        userName[i]=*s;
       }
    userName[i]=0;
    
    if(*s != ':') return -1;
    
    s++;
    strcpy(encrPass,s);
    return 0;
   }
   
int UserLogin::quickLogin()
  {
    strcpy(userName,"rasadmin");
    messageDigest("rasadmin",encrPass,"MD5");
    return 0;
   }
   
const char *UserLogin::getUserName()
  { 
    return userName;
   }
const char *UserLogin::getEncrPass()
  { 
    return encrPass; 
   }
   
