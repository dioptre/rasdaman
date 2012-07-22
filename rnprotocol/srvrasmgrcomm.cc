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
 * - why sometimes exit() and sometimes return on error?
 *   should be return or exception, always.
 *
 ************************************************************/

#include "srvrasmgrcomm.hh"
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<iostream>
#include<string.h>
#include "raslib/rminit.hh"


#include "debug.hh"


// exit code of the server when communication is impossible
// PB: Why 10 ? Anyway, should not use exit in a server, how often did I tell you, Walter Schatz!!!
const unsigned int EXIT_CODE = 10;

// max number of retries to connect to rasmgr in informRasMgr()
const unsigned int SRV_MAX_RETRY = 10000000;

// how many retries are attempted before a message is issued informRasMgr()
const unsigned int SRV_TALK_INTERVAL = 100000;


SrvRasmgrComm rasmgrComm;

SrvRasmgrComm::SrvRasmgrComm()
{
    timeout    = 0;
    serverName = 0;
    rasmgrHost = 0;
    rasmgrPort = -1;
}

// note: should make use of timeout as defined in cmd line,
// but that's a major undertaking -- PB 2005-sep-02

void SrvRasmgrComm::init(unsigned int timeOut, const char* instanceName, const char* nRasmgrHost, int nRasmgrPort)
{
    timeout    = timeOut;
    serverName = instanceName;
    rasmgrHost = nRasmgrHost;
    rasmgrPort = nRasmgrPort;
}

unsigned int SrvRasmgrComm::getTimeout()
{
    return timeout;
}

void SrvRasmgrComm::informRasmgrServerAvailable()
{
    informRasMGR(SERVER_AVAILABLE);
}

void SrvRasmgrComm::informRasmgrServerDown()
{
    informRasMGR(SERVER_DOWN);
}

void SrvRasmgrComm::informRasmgrServerStillAvailable()
{
    // too verbose, blows up log file
    // RMInit::logOut << "informing rasmgr: server still available." << endl;
    informRasMGR(SERVER_REGULARSIG);
}

void SrvRasmgrComm::informRasMGR(int what)
{
    //what: 0 - going down
    //      1 - available
    //      2 - regular signal

//    cout<<"servername ="<<serverName<<" rasmgrhost="<<rasmgrHost<<"  port="<<rasmgrPort<<endl;

//    if(what == SERVER_AVAILABLE) accessControl.resetForNewClient();

    struct protoent* getprotoptr = getprotobyname("tcp");

    struct hostent *hostinfo = gethostbyname(rasmgrHost);
    if(hostinfo==NULL)
    {
        RMInit::logOut << "Error: cannot locate rasmgr host '" << rasmgrHost << "': " << strerror(errno) << std::endl;
        return;
    }

    sockaddr_in internetSocketAddress;
    internetSocketAddress.sin_family = AF_INET;
    internetSocketAddress.sin_port=htons(rasmgrPort);
    internetSocketAddress.sin_addr=*(struct in_addr*)hostinfo->h_addr;

    int sock;

    bool ok=false;
    long talkInterval=SRV_TALK_INTERVAL;
    long maxRetry=SRV_MAX_RETRY;
    long retry =0;
    // creating socket
    for(retry=0; retry<maxRetry; retry++)
    {
        sock=socket(PF_INET,SOCK_STREAM,getprotoptr->p_proto);
        //std::cout<<"Socket="<<sock<<" protocol(tcp)="<<getprotoptr->p_proto<<std::endl;

        if(sock<0)
        {
            if( (retry%talkInterval) == 0)
            {
                std::cerr<< "Error: server '" << serverName << " cannot open socket to rasmgr (" << retry << " attempts, still retrying): " << strerror(errno) << std::endl;
                RMInit::logOut << "Error: server '" << serverName << " cannot open socket to rasmgr (" << retry << " attempts, still retrying): " << strerror(errno) << std::endl;
            }
            continue;
        }

        if(connect(sock,(struct sockaddr*)&internetSocketAddress,sizeof(internetSocketAddress)) < 0)
        {
            if( (retry%talkInterval) == 0)
            {
                std::cerr << "Error: server '" << serverName << " cannot connect to rasmgr (" << retry << " attempts, still retrying): " << strerror(errno) << std::endl;
                RMInit::logOut << "Error: server '" << serverName << " cannot connect to rasmgr (" << retry << " attempts, still retrying): " << strerror(errno) << std::endl;
            }
            close(sock); //yes, some SO requieres this, like DEC from BLVA
            continue;
        }
        ok = true;
        break;
    }

    if( !ok )
    {
        std::cerr << "Error: unable to contact rasmgr, server '" << serverName << "' herewith giving up." <<std::endl;
        RMInit::logOut << "Error: unable to contact rasmgr, server '" << serverName << "' herewith giving up." <<std::endl;
        if(sock)
            close(sock);
        exit( EXIT_CODE );
    }

    // creating the HTTP message
    char message[200];
    sprintf(message,"%s%d\r\n\r\n%s %d %ld ","POST rasservernewstatus HTTP/1.1\r\nUserAgent: RasServer/1.0\r\nContent-length: ",strlen(serverName)+3,serverName,what,0);

    // writing message;
    if(writeWholeMessage(sock,message,strlen(message)+1)<0)
    {
        std::cerr << "Error: cannot send message to rasmgr: " << strerror(errno) << std::endl;
        RMInit::logOut << "Error: cannot send message to rasmgr: " << strerror(errno) << std::endl;
        close(sock);
        exit( EXIT_CODE );
    }
    close(sock);
}


int SrvRasmgrComm::writeWholeMessage(int socket,char *destBuffer,int buffSize)
{
    // we write the whole message, including the ending '\0', which is already in
    // the buffSize provided by the caller
    int totalLength=0;
    int writeNow;
    while(1)
    {
        writeNow = write(socket,destBuffer+totalLength,buffSize-totalLength);
        if(writeNow == -1)
        {
            if(errno == EINTR)
                continue; // read was interrupted by signal (on bad SO's)
            return -1; // another error
        }
        totalLength+=writeNow;

        if( totalLength==buffSize )
            break; // THE END
    }

    return totalLength;
}

