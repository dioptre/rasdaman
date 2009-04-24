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
 * - called from rasserver_main.cc
 * - startRnpServer() is twin to startRpcServer and startHttpServer()
 *
 ************************************************************/

#include <iostream>
#include <signal.h>
#include "rnpserver.hh"
#include "srvrasmgrcomm.hh"
#include "server/rasserver_config.hh"
#include "rnpservercomm.hh"

#include "raslib/rminit.hh"
#include "debug-srv.hh"

// only for access control
#include "servercomm/servercomm.hh" 

#include "server/rasserver_entry.hh"

RnpRasDaManComm rnpServerComm;
RasserverCommunicator communicator(&rnpServerComm); 

extern "C"
{
void rnpSignalHandler(int sig);
}

void startRnpServer()
{
	ENTER( "startRnpServer" );

	signal (SIGTERM, rnpSignalHandler);

	RMInit::logOut << "Initializing control connections..." << flush;
	rasmgrComm.init(configuration.getTimeout(), configuration.getServerName(), configuration.getRasmgrHost(), configuration.getRasmgrPort());
	
	accessControl.setServerName(configuration.getServerName());
	
	RMInit::logOut << "informing rasmgr: server available..." << flush;
	rasmgrComm.informRasmgrServerAvailable();
	RMInit::logOut << "ok" << endl;

	//##################
	
	RMInit::logOut << "Initializing job control..." << flush;
	communicator.initJobs(1);
	communicator.setTimeout(RNP_TIMEOUT_LISTEN,0); // the select loop!
	
	communicator.setListenPort(configuration.getListenPort());
	
	rnpServerComm.setServerJobs(1);
	rnpServerComm.connectToCommunicator(communicator);
	rnpServerComm.setTransmitterBufferSize(configuration.getMaxTransferBufferSize());

	RMInit::logOut<<"setting timeout to "<<configuration.getTimeout()<< " secs..." << flush;

	rnpServerComm.setTimeoutInterval(configuration.getTimeout());
	NbJob::setTimeoutInterval(configuration.getTimeout());

	RMInit::logOut << "connecting to base DBMS..." << flush;
	RasServerEntry &rasserver = RasServerEntry::getInstance();
	rasserver.compat_connectToDBMS();
	
	RMInit::logOut<<"ok, waiting for clients."<<endl<<endl;
	communicator.runServer();
	    
	RMInit::logOut<<"RNP server shutdown in progress..."<<flush;
	rnpServerComm.disconnectFromCommunicator();

	//##################

	RMInit::logOut<<"informing rasmgr..."<<flush;
	rasmgrComm.informRasmgrServerDown();
	
	RMInit::logOut<<"server stopped."<<endl;
	LEAVE( "startRnpServer" );
}

void stopRnpServer()
{
	ENTER( "stopRnpServer" );

	communicator.shouldExit();

	LEAVE( "stopRnpServer" );
}


void rnpSignalHandler(int sig)
{
	static int in_progress=0;	// sema for signal-in-signal
	
	if (in_progress)		// routine already active?
		return;			// ...then don't interfere
	
	in_progress = 1;		// block further signals
	
	for(long j=0;j<1000000;j++)  	// make sure server notices shutdown
		;			// NB: why this large number? doesn't seem to be thought over carefully -- PB 2003-nov-23

	stopRnpServer();		// send shutdown request
}


