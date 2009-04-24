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
 * SOURCE: akgnet_server.cc
 *
 * MODULE: akg network
 * CLASS:  GenericServer, BlockingServer
 *
 * COMMENTS:
 *
 */

#include "debug.hh"

#include <akgnet_server.hh>

//#include<iostream>


akg::GenericServer::GenericServer() throw()
  { 
    listenPort = 0;
    exitRequest = false;
   }
akg::GenericServer::~GenericServer() throw()
  {
   }
      
void akg::GenericServer::setListenPort(int x) throw()
  {
    listenPort = x;
   }   
int akg::GenericServer::getListenPort() throw()
  {
    return listenPort;
   }   

void akg::GenericServer::setTimeout(int sec,int milisec) throw()
  { selector.setTimeout(sec,milisec);
   }
void akg::GenericServer::disableTimeout() throw()
  { selector.disableTimeout();
   }

void akg::GenericServer::shouldExit() throw()
  {
    exitRequest = true;
   }
      
    
bool akg::GenericServer::initListenSocket(int port, bool nonblocking) throw()
  {
    if(listenSocket.open(port) == false) return false;
    
    if(nonblocking) listenSocket.setNonBlocking(true);
    
    selector.setRead(listenSocket());
    
    DBTALK("Listen socket="<<listenSocket());
    return true;
   }

bool akg::GenericServer::connectNewClient(ServerSocket &s) throw()
  {
    if(s.acceptFrom(listenSocket)==false) return false;
    
    selector.setRead(s());
    
    return true;
   }
void akg::GenericServer::closeSocket(Socket &s) throw()
  {
    if(s.isOpen())
     { selector.clearRead(s());
       selector.clearWrite(s());
       s.close();
      }  
   }


//###########################

akg::BlockingServer::BlockingServer() throw()
  { 
   }
akg::BlockingServer::~BlockingServer() throw()
  {
   }
   
bool akg::BlockingServer::runServer() throw()
{
	ENTER( "akg::BlockingServer::runServer" );

	if(listenPort == 0)
	{
		LEAVE( "akg::BlockingServer::runServer, listenPort=0, result=false" );
		return false;
	}

	if(initListenSocket(listenPort,false)==false)
	{
		LEAVE( "akg::BlockingServer::runServer, Error: init socket failed for port " << listenPort << ", result=false" );
		return false;
	}

	while(exitRequest == false)
	{ 
		int rasp = selector();
		TALK( "rasp=" << rasp );	
		if(rasp>0)
		{
			if(serverSocket.isOpen())
	     		{
				TALK( "socket is open." );
				if(selector.isRead(serverSocket()))
				{
					TALK( "socket is readable, executing request." );
					executeRequest(serverSocket);
					TALK( "closing socket." );
					closeSocket(serverSocket);
					TALK( "socket closed." );
				}	
	      		}
			else if(selector.isRead(listenSocket())) 
			{
				TALK( "socket not open, but readable (???). connecting new client." );
				connectNewClient(serverSocket);
				TALK( "after client connect." );
				// we don't care why it could fail
			}
			else
			{
				TALK( "no read socket - should never have reached this point." );
			}
		}
	   
		if(rasp == 0)      
		{
			TALK( "exec timeout" );
			executeTimeout();
		}

		if(rasp<0)
		{
			TALK( "Internal connect error: bad selector." );
		}
	}
	
	LEAVE( "akg::BlockingServer::runServer, result=true" );
	return true;     
}
       

