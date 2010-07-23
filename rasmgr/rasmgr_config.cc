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
 * SOURCE: rasmgr_config.cc
 *
 * MODULE: rasmgr
 * CLASS:  Configuration, RasmgrLicense
 *
 * PURPOSE:
 *   Config info from commandline, environment and license
 *    
 * COMMENTS:
 *   none
 *
*/

using namespace std;

#include "globals.hh"		// DEFAULT_HOSTNAME, DEFAULT_PORT, RASMGR_CONF_FILE

#include "rasmgr_config.hh"
#include "rasmgr.hh"
#include "rasmgr_host.hh"
#include "rasmgr_dbm.hh"
#include "rasmgr_srv.hh"
#include "ras_crypto.hh"
#include "rasmgr_users.hh"
#include "rasmgr_comm.hh"
#include "rasmgr_rascontrol.hh"

#include <stdlib.h>		// mkstemp()
#include <iomanip>
#include <time.h>

#include "debug.hh"

extern bool hostCmp( const char *h1, const char *h2);


Configuration::Configuration():
	cmlInter      (CommandLineParser::getInstance()),
	cmlName       (cmlInter.addStringParameter(CommandLineParser::noShortName, "name", "<name> symbolic name of this rasmgr (slave only, default: the host name)")),
	cmlHostName   (cmlInter.addStringParameter(CommandLineParser::noShortName, "hostname", "<name> the advertized host name (master only, default: same as UNIX command 'hostname')")),
	cmlPort       (cmlInter.addLongParameter(CommandLineParser::noShortName, "port", "<port> listen port number", DEFAULT_PORT)),
	cmlPollFrequ  (cmlInter.addLongParameter(CommandLineParser::noShortName, "poll", "<poll> polling timeout (in seconds) for rasmgr listen port", DEFAULT_POLLING_FREQUENCY )),
	cmlMaster     (cmlInter.addStringParameter(CommandLineParser::noShortName, "master", "<name> host of rasmgr master (slave only)")), 
	cmlMasterPort (cmlInter.addLongParameter(CommandLineParser::noShortName, "mport", "<port> listen port number of rasmgr master (slave only)", DEFAULT_PORT)),
	cmlQuiet      (cmlInter.addFlagParameter( 'q', CommandLineParser::noLongName, "quiet: don't log requests (default: log requests to stdout)")),
#ifdef RMANDEBUG	// was: NO_OFFICIAL_RELEASE
	cmlTest       (cmlInter.addFlagParameter(CommandLineParser::noShortName, "test", "test mode")), 
	cmlDSup       (cmlInter.addFlagParameter(CommandLineParser::noShortName, "dsup", "debug mode")), 
	cmlRandTest   (cmlInter.addFlagParameter(CommandLineParser::noShortName, "rgt",  "random generator test")), 
	cmlRth        (cmlInter.addFlagParameter(CommandLineParser::noShortName, "rth", "disable rthl test")),
	cmlMultiWT    (cmlInter.addFlagParameter(CommandLineParser::noShortName, "amw", "allow multiple write transactions")),
#endif	      	// RMANDEBUG
	cmlHelp       (cmlInter.addFlagParameter('h', RASMGRCMD_HELP, "print this help"))
{
	ENTER( "Configuration::Configuration: enter." );

	int ghnResult = gethostname(hostName, sizeof(hostName) );
	if (ghnResult != 0)	// cannot get hostname?
	{
		int ghnErrno = errno;
		std::cout << "Error: cannot get hostname of my machine: error " << ghnErrno << "; will use '" << DEFAULT_HOSTNAME << "' as heuristic." << endl;
		strcpy( hostName, DEFAULT_HOSTNAME );
	}
	strcpy(slaveName,hostName);
	strcpy(publicHostName,hostName);
	listenPort=DEFAULT_PORT; 
	masterPort=DEFAULT_PORT;
	masterName[0]=0;

	pollFrequency = DEFAULT_POLLING_FREQUENCY;

	configFileName[0]=0;
	slave = false;

	if (sizeof(configFileName) < strlen(CONFDIR) + strlen(RASMGR_CONF_FILE) + 2)
	{
		std::cout << "Error: configuration path length exceeds system limits: '" << CONFDIR << "/" << RASMGR_CONF_FILE << "'" << endl;
		LEAVE( "Configuration::Configuration: leave." );
		return;
	}
	sprintf( configFileName, "%s/%s", CONFDIR, RASMGR_CONF_FILE );
	altConfigFileName[0] = '\0';
 
	testModus    = false;
	debugSupport = false;
	rtHlTest     = true;  // by default RasMgr tests at runtime if it's the only one 
	allowMultiWT = false; // rasmgr doesn't allow multiple write transactions for a db

	LEAVE( "Configuration::Configuration: leave." );
}
   
bool Configuration::readConfigFile()
{
	//insert internal host, it's not in config file
	hostmanager.insertInternalHost();

	char inBuffer[MAXMSG];
	char outBuffer[MAXMSG];
	bool result = true;
	bool fileIsOpen = false;
	
	ENTER( "Configuration::readConfigFile: enter. Looking for config file " << configFileName );

	VLOG << "Inspecting config file " << configFileName << "...";

	std::ifstream ifs(configFileName);		// open config file
	
	if(ifs)
		fileIsOpen = true;
	else
	{
		TALK( "Configuration::readConfigFile: cannot open config file." );
		std::cout << "Warning: cannot open config file " << configFileName << endl;
		fileIsOpen = false;
	}
	result = true;				// was: false, but I want to allow a missing file

	if (fileIsOpen)
	{
		authorization.startConfigFile();
	
		while( ! ifs.eof() )		// was: while(1), I simplified this
						// processRequest() will get an additional empty line at eof, but this is harmless
		{
			ifs.getline(inBuffer,MAXMSG);
			// if(!strlen(inBuffer) && ifs.eof())	// FIXME: what happens if last line in file is empty?
			// {
			// TALK( "Configuration::readConfigFile: strlen(inBuffer)=" << strlen(inBuffer) << ", eof=" << ifs.eof());
			// break;
			// }
	
			TALK( "Configuration::readConfigFile: processing line: " << inBuffer );
		 	rascontrol.processRequest(inBuffer,outBuffer);
		}
	
		authorization.endConfigFile();

		ifs.close();				// close config file handle
	}

	if (result == true && fileIsOpen)
		VLOG << "ok" << endl;

	LEAVE( "Configuration::readConfigFile: leave. result=" << result );
	return true;		
}

// return name of alternate config file;
// takes value from preceding saveAltConfigFile() call.
const char *Configuration::getAltConfigFileName()
{
	return altConfigFileName;
}

// in future this is not used directly, but through saveOrigConfigFile() and saveAltConfigFile() wrappers below
bool Configuration::saveConfigFile()
{
	ENTER( "Configuration::saveConfigFile: enter." );

	std::ofstream ofs(configFileName);
	if(!ofs)
	{
		LEAVE( "Configuration::saveConfigFile: leave. cannot open config file " << configFileName << " for writing." );
		return false;
	}

	ofs << "# rasmgr config file (v1.1)" << std::endl;
	ofs << "# warning: do not edit this file, it may be overwritten by rasmgr!" << std::endl;
	ofs << "#" << std::endl;
	
	int i;
	//serverhosts
	for(i=0;i<hostmanager.countHosts();i++)
	{
		ServerHost &xx=hostmanager[i];
	
		if(i > 0)
			ofs<<"define host "<<xx.getName()<<" -net "<<xx.getNetworkName()<<" -port "<<xx.getListenPort()<<std::endl;
		else
		{
			//by default the master RasMgr is init with the hostname as name => if we have to we change it here
			if( ! hostCmp(xx.getName(),config.getHostName()) )
				ofs << "change host "<<config.getHostName()<<" -name "<<xx.getName()<<std::endl;	

			if(xx.useLocalHost()==false)
				ofs<<"change host "<<xx.getName()<<" -uselocalhost off"<<std::endl;
		}
	}
	//databaseHosts
	for(i=0;i<dbHostManager.countHosts();i++)
	{
		DatabaseHost &xx=dbHostManager[i];
		ofs<<"define dbh "<<xx.getName()<<" -connect "<<xx.getConnectionString()<<std::endl;
	}
	//rasservers
	for(i=0;i<rasManager.countServers();i++)
	{
		RasServer &xx=rasManager[i];
	
		ofs<<"define srv "<<xx.getName()<<" -host "<<xx.getHostName()<<" -type "<<xx.getType();
		if(xx.getType()==SERVERTYPE_FLAG_HTTP)
			ofs<<" -port "<<xx.getPort();
		else
			ofs<<" -port 0x"<<std::hex<<xx.getPort()<<std::dec;
		if(xx.isConnectedToDBHost())
			ofs<<" -dbh "<<xx.getDBHostName();
		ofs<<std::endl;
	
		ofs<<"change srv "<<xx.getName()<<" -countdown "<<xx.getCountDown();
	
		if(strcmp(xx.getExecutableName(),RASEXECUTABLE))
			ofs<<" -exec "<<xx.getExecutableName();
		  
		ofs<<" -autorestart "<<(xx.isAutoRestart() ? "on":"off")<<" -xp "<<xx.getExtraParams()<<std::endl;      
	}
		 
	//databases
	for(i=0;i<dbManager.countDatabases();i++)
	{
		Database &xx=dbManager[i];
	
		for(int j=0;j<xx.countConnectionsToDBHosts();j++)
		{
			ofs<<"define db "<<xx.getName()<<" -dbh "<<xx.getDBHostName(j)<<std::endl;
		} 
	}

	ofs.close();		// this was missing, therefore sometimes the config file was cleared -- PB 2003-jun-06
	LEAVE( "Configuration::saveConfigFile: leave." );
	return true;
} // saveConfigFile()

// save config file at original place, i.e., under the name of configFile
bool Configuration::saveOrigConfigFile()
{
	ENTER( "Configuration::saveOrigConfigFile: enter." );

	bool result = saveConfigFile();

	LEAVE( "Configuration::saveOrigAltConfigFile: leave. result=" << result );
	return result;
}

// save configuration file in another file, same dir as config file
bool Configuration::saveAltConfigFile()
{
	ENTER( "Configuration::saveAltConfigFile()" );

	bool result = true;
	char origFileName[ sizeof(configFileName) ];        // temp copy of origFileName

	// save original file name
	(void) strcpy( origFileName, configFileName );

	// build temp file by appending a unique string
	(void) strcpy( altConfigFileName, configFileName );
	(void) strcat( altConfigFileName, ".XXXXXX" );       // 6 * 'X', see man mkstemp()

	int altFile = mkstemp( altConfigFileName );         // replaces the Xs by unique string
	if (altFile < 0)                                    // error in creating file name
	{
		int tempError = errno;
		TALK( "Configuration::saveAltConfigFile: error creating alternate file name: " << strerror(tempError) );
		result = false;
	}

	if (result == true)
	{
		// now we have a valid + open file, but we can't use it like that, because we open down below.
		// so close it again, being happy that we have a valid file name. bad hack, though.
		int closeResult = close( altFile );
		if (closeResult != 0)
			TALK( "Configuration::saveAltConfigFile: error in temporary closing file, ignoring that." );
	}

	if (result == true)
	{
		(void) strcpy( configFileName, altConfigFileName );	// set file to be written to alternate name
		result = saveConfigFile();                      // save file, name has been substituted successfully
		(void) strcpy( configFileName, origFileName );	// restore original config file name
	}

	LEAVE( "Configuration::saveAltConfigFile: leave. result=" << result );
	return result;
}

bool Configuration::interpretArguments(int argc, char **argv, char **envp)
{
	ENTER( "Configuration::interpretArguments: enter." );

	bool result = true;
	//errorCode=0;
	
	//process command line
	try
	{
		cmlInter.processCommandLine(argc, argv);
	}
	catch(CmlException& err)
	{
		std::cout << "Error parsing command line: " << err.what() << std::endl;
		printHelp();
		result = false;
	}
   
	SET_OUTPUT( true );			// by default, enable trace (debug) output
	verbose = true;			// by default, be verbose
	if( (result==true) && cmlQuiet.isPresent() )
	{
		SET_OUTPUT( false );			// disable trace (debug) output
		// debugOutput = true;			// done via the above macro
		verbose = false;			// only minimum messages
		result = true;
	} 
	
	if( (result==true) && cmlHelp.isPresent() )
	{
		printHelp();
		result = false;
	} 
	
	if( (result==true) && cmlHostName.isPresent() )
	{
		if (sizeof(hostName) > strlen(cmlHostName.getValueAsString()))
			strcpy(publicHostName,cmlHostName.getValueAsString());
		else
		{
			VLOG << "Error: host name exceeds length limit of " << sizeof(hostName) << " characters." << std::endl;
			result = false;
		}
	}
		 
	if( (result==true) && cmlPort.isPresent() )
	{
		try 
		{
			listenPort = cmlPort.getValueAsLong();
		}
		catch(CmlException& err) 
		{
			VLOG << "Error converting port parameter " << cmlPort.getLongName() << " to integer: " << err.what() << std::endl;
			result = false;
		}           
	}
		 
	if( (result==true) && cmlMaster.isPresent() )
	{
	        slave = true;
		strcpy(masterName,cmlMaster.getValueAsString());
	}
	
	if( (result==true) && cmlMasterPort.isPresent() )
	{
		try 
		{
			masterPort = cmlMasterPort.getValueAsLong();
		}
		catch(CmlException& err) 
		{
			VLOG << "Error converting " << cmlMasterPort.getLongName() << " to integer: " << err.what() << std::endl;
			result = false;
		}           
	}

	if( (result==true) && cmlPollFrequ.isPresent() )
	{
		try
		{
			pollFrequency = cmlPollFrequ.getValueAsLong();
		}
		catch(CmlException& err)
		{
			VLOG << "Error converting " << cmlPollFrequ.getLongName() << " to integer: " << err.what() << std::endl;
			result = false;
		}
		if (result == true && pollFrequency <= 0)
		{
			VLOG << "Error: poll frequency must be a positive integer." << std::endl;
			result = false;
		}
	}

	if( (result==true) && cmlName.isPresent() )
	{
		if (sizeof(slaveName) > strlen(cmlName.getValueAsString()))
			strcpy(slaveName,cmlName.getValueAsString());
		else
		{
		      VLOG << "Error: slave name exceeds length limit of " << sizeof(slaveName) << " characters." << std::endl;
		      result = false;
		}
	}       
		 
#ifdef RMANDEBUG	// was: NO_OFFICIAL_RELEASE
	testModus = cmlTest.isPresent();
	debugSupport=cmlDSup.isPresent();
	rtHlTest=cmlRth.isPresent();

	if( (result==true) && cmlRandTest.isPresent() )
	{
		std::cout<<"Random generator test..."<<(randomGenerator.insideTest() ? "PASSED":"FAILED" )<<std::endl;
		result = false;
	}
	
	allowMultiWT = cmlMultiWT.isPresent();

#endif       // RMANDEBUG

	LEAVE( "Configuration::interpretArguments: leave. result=" << result );
	return result;   
}
	
bool Configuration::allowMultipleWriteTransactions()
{
	return allowMultiWT;
}

bool Configuration::isDebugSupport()
{ 
	return debugSupport;
}
	
bool Configuration::isVerbose()
{
	return verbose;
}

bool Configuration::isTestModus()
{
	return testModus;
}
   
const char* Configuration::getHostName()
{
	return hostName;
}

const char* Configuration::getPublicHostName()
{
	return publicHostName;
}

int Configuration::getListenPort()
{
	return listenPort;
}

const char* Configuration::getMasterName()
{
	return masterName;
}
int Configuration::getMasterPort()
{
	return masterPort; 
}

int Configuration::getPollFrequency()
{
	return pollFrequency;
}

const char* Configuration::getSlaveName()
{
	return slaveName[0] ? slaveName:hostName;
}

   
void Configuration::printHelp()
{
	std::cout << "Usage: rasmgr [options]" << std::endl;
	std::cout << "Options:" << std::endl;
	cmlInter.printHelp();
	std::cout << std::endl;
	return;
}

void Configuration::printStatus()
{
	std::cout << "rasmgr configuration parameter settings:" << std::endl;
	std::cout << "   symb name   = " << publicHostName << std::endl;
	std::cout << "   hostname    = " << publicHostName << std::endl;
	std::cout << "   port        = " << listenPort     << std::endl;
	std::cout << "   poll        = " << pollFrequency  << std::endl;
	std::cout << "   quiet     = "   << (!verbose) << std::endl;
	return;
}

//****************************************************************************************************************

// FIXME: this needs refinement -- PB 2003-jun-09
BenchmarkTimer::BenchmarkTimer(const char *text)
{
	this->text = (char*)text;
	time_t now = time(NULL);
	char *n = ctime(&now);
	n[strlen(n)-1] = '\0';		// delete trailing newline char
	// cout << "--- rasmgr timer start for \'" << text << "\' at " << n << "." << endl;    
	gettimeofday(&start,NULL);
}
   
BenchmarkTimer::~BenchmarkTimer()
{
}
   
void BenchmarkTimer::result()
{
	timeval result;
	gettimeofday(&end,NULL);
	int r=timeval_subtract(&result,&end,&start);
	time_t now = time(NULL);
	char *n = ctime(&now);
	n[strlen(n)-1] = '\0';		// delete trailing newline char
	VLOG << "rasmgr timer stop for \'" << text << "\' at " << n << ": " << result.tv_sec << '.' << std::setw(3) << std::setfill('0') << result.tv_usec << " seconds elapsed." << endl;    
}

int BenchmarkTimer::timeval_subtract(timeval *result,timeval *x,timeval *y)
{
#define ONE_MILLION 1000000
	/* Perform the carry for the later subtraction by updating Y. */
	if (x->tv_usec < y->tv_usec)
	{
		int nsec = (y->tv_usec - x->tv_usec) / ONE_MILLION + 1;
		y->tv_usec -= ONE_MILLION * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > ONE_MILLION)
	{
		int nsec = (x->tv_usec - y->tv_usec) / ONE_MILLION;
		y->tv_usec += ONE_MILLION * nsec;
		y->tv_sec -= nsec;
	}
	 
	/* Compute the time remaining to wait.
	   `tv_usec' is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;
	 
	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

