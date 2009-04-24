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
 * SOURCE: rasmgr_main.cc
 *
 * MODULE: rasmgr
 * CLASS:  
 *
 * PURPOSE:
 *   management of rasserver executables
 *    
 * COMMENTS:
 * - FIXME: looks like a rasmgr slave still uses old comm scheme -- compatible?
 *
*/

#include <iostream>
 
#include "rasmgr.hh"
#include "rasmgr_config.hh"
#include "rasmgr_host.hh"
#include "rasmgr_dbm.hh"
#include "rasmgr_srv.hh"
#include "rasmgr_master.hh" 
#include "rasmgr_rascontrol.hh"
#include "rasmgr_users.hh"
#include "ras_crypto.hh"
#include "rasmgr_localsrv.hh"
#include "rasmgr_error.hh"

#ifndef COMPDATE
#error "Please specify the COMPDATE variable!"
/*
COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`

and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
*/
#endif

#define DEBUG_MAIN
#undef DEBUG_HH
#include "debug.hh"


Configuration config;

HostManager         hostmanager;
DatabaseHostManager dbHostManager;
DatabaseManager     dbManager;
RasServerManager    rasManager;
MasterComm          masterCommunicator;
RasControl          rascontrol;
UserManager         userManager;
Authorization       authorization;
LocalServerManager  localServerManager;
RandomGenerator     randomGenerator;

void installSignalHandlers();

int main(int argc, char** argv, char** envp)
{
	SET_OUTPUT( true );	// enable debugging trace, if compiled so

	ENTER( "main." );

	std::cout<< "rasmgr: rasdaman server manager tool. rasdaman v" << RMANVERSION / 1000. << " -- generated on " << COMPDATE << "." << std::endl;
	
// just to see the difference between an official release and the inside development version
#ifdef NO_OFFICIAL_RELEASE    
	std::cout<<"This is not the official release version, it supports test modus and rasserver in debugger"<<std::endl;
#else
          std::cout << " Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann rasdaman GmbH." << std::endl
          << "Rasdaman community is free software: you can redistribute it and/or modify "
             "it under the terms of the GNU General Public License as published by "
             "the Free Software Foundation, either version 3 of the License, or "
             "(at your option) any later version. \n"
             "Rasdaman community is distributed in the hope that it will be useful, "
             "but WITHOUT ANY WARRANTY; without even the implied warranty of "
             "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
             "GNU General Public License for more details. \n\n";

	std::cout << "This software contains software which is in the public domain:" << std::endl;
	std::cout << "- openssl 0.96c (C) 1998-2002 The OpenSSL Project, (C) 1995-1998 Eric A. Young, Tim J. Hudson" << std::endl;
#endif

#ifdef INCLUDE_HIDDEN_COMMANDS
	std::cout << "This version is 'inside only'" <<std::endl;
#endif

	if(testIsMessageDigestAvailable("MD5")==false) 
	{
		std::cout<<"Error: Message Digest MD5 not available."<<std::endl;
		return RASMGR_RESULT_NO_MD5;
	}
	
	installSignalHandlers();

	bool result  = config.interpretArguments(argc,argv,envp);
	if (result == false)
		 return RASMGR_RESULT_ILL_ARGS;

	if(config.isTestModus())
	{
	  std::cout<<"rasmgr running in test modus ";
	  VLOG <<", listening on port=" << config.getListenPort() << std::endl;
	}
	else
	{   
		 VLOG << " rasmgr running on " << config.getHostName() << ", listening on port " << config.getListenPort();
		 VLOG << " with poll timeout " << config.getPollFrequency() << " seconds. ";
	}	
	
	if(config.isTestModus()==false)
	{
		TALK( "hostname=" << config.getHostName() << ", publicHostname=" << config.getPublicHostName() );
		if(strcmp(config.getHostName(),config.getPublicHostName()) != 0)
		{
			VLOG <<"Advertised host name is "<<config.getPublicHostName()<<std::endl;
		}
		
		 bool resultConfig = config.readConfigFile();
		 TALK( "rasmgr::main: resultConfig=" << resultConfig );
		 rascontrol.setConfigDirty( false );		// all changes to config up to now come from config file, do not require save

		int resultAuth = authorization.readAuthFile();
		TALK( "rasmgr::main: resultAuth=" << resultAuth );
		switch( resultAuth )
		{
			case RC_OK:
				TALK( "rasmgr::main: auth file ok, set state to not dirty." );
				rascontrol.setAuthDirty( false );	// auth file ok, so clean init state
				break;
			case ERRAUTHFNOTF:
				TALK( "rasmgr::main: auth file not found, loading defaults." );
				userManager.loadDefaults();
				// disabled because otherwise tons of new auth files are generated -- PB 2005-jul-02
				// rascontrol.setAuthDirty( true );	// auth file not present, write default
				break;
			case ERRAUTHFCORR:
				LEAVE( "rasmgr::main: auth file corrupt." );
				return RASMGR_RESULT_AUTH_CORRUPT;
				break;
			case ERRAUTHFWRHOST:
				LEAVE( "rasmgr::main: auth file for another host." );
				return RASMGR_RESULT_AUTH_OTHERHOST;
				break;
			case ERRAUTHFVERS:
				LEAVE( "rasmgr::main: auth file version mismatch." );
				return RASMGR_RESULT_AUTH_INCOMPAT;
				break;
			default:				// should not occur, internal enum mismatch
				LEAVE( "rasmgr::main: illegal auth file result code " << resultAuth << ", internal error." );
				return RASMGR_RESULT_INTERNAL;
				break;
			}

		try
		{
			BenchmarkTimer *totalTimePtr = new BenchmarkTimer("Total master time");

			TALK( "launching masterCommunicator.Run()..." );
  			masterCommunicator.Run();		// central request handling loop
			TALK( "masterCommunicator.Run() done." );

			totalTimePtr->result();			// print total time elapsed
		}
		catch(RCError& e)
		{
			char *errorMsg;
			e.getString(errorMsg);
			std::cout<<"Error: "<<errorMsg<<std::endl;
		}                   

// write the config file only on explicit rascontrol request "save"
// (and at that moment), or at rascontrol "exit" to a rescue file -- PB 2003-jun-06
#ifdef NEVER_AGAIN
		if(!config.saveConfigFile())
		{
			std::cout<<"Error saving configuration file."<<std::endl;
		}

		if(!authorization.saveAuthFile())
		{
			std::cout<<"Error saving user authorization file."<<std::endl;
		}
#endif
	}
		
	else if(config.isTestModus())
	{
		hostmanager.insertInternalHost();
		userManager.loadDefaults();
		masterCommunicator.Run();
	}
	
	cout <<"rasmgr terminated."<<std::endl;

	int retval = RASMGR_RESULT_OK;
	LEAVE( "main: leave. retval=" << retval );
	return retval;
} // main()

// danger: cout in interrupt???   
// handler for SIGINT and SIGTERM = call for exit
void SigIntHandler(int sig)
{
	std::cout<<"rasmgr received terminate signal...";
	masterCommunicator.shouldExit();
}
   
void installSignalHandlers()   
{
   signal (SIGINT, SigIntHandler);
   signal (SIGTERM, SigIntHandler);
   signal (SIGHUP, SIG_IGN);
   signal (SIGPIPE,SIG_IGN);
   signal (SIGTTOU,SIG_IGN); // no console, ei si?
}
   
// should be replaced by something cleaner, eventually 
void exitbyerror(const char* text)
{
	perror(text);
	exit( RASMGR_EXIT_FAILURE );
}

char *strtolwr(char *string)// should be somewhere in the C-library, but can't find it
{
	char *t=string;
	for(;*t;t++)
	{
		 if(*t>='A' && *t<='Z') *t|='a'-'A';
	}
	return string;  
}

