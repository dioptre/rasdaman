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
/**
 * SOURCE: rascontrol.cc
 *
 * MODULE: rascontrol
 * CLASS:  
 *
 * PURPOSE:
 *  RasControl is the remote control utility for the rasmgr
 *    
 * COMMENTS:
 * - should normalize cmd interpretation with rasmgr: strcmp vs strncmp
*/

#define DEBUG_MAIN		// to allocate static vars for trace macros
#include "debug-clt.hh"

#include "rascontrol.hh"
#include <fstream>
#include <signal.h>

#ifndef RMANVERSION
#error "Please specify RMANVERSION variable!"
#endif

#ifndef COMPDATE
#error "Please specify the COMPDATE variable!"
/*
COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`

and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
*/
#endif

UserLogin        userLogin;
EditLine         editLine;
RasMgrClientComm httpClient;
RascontrolConfig config;

bool redirStdin  = false;
bool redirStdout = false;

const int NOERROR      = 0;
const int CANTCONNECT  = 1;
const int LOGINERROR   = 2;
const int ACCESSDENIED = 3;

// flag to indicate whether conf or auth file have been modified.
// if true files must be saved before terminating.
static bool confFileDirty = false;
static bool authFileDirty = false;

int  interactiveWork();
int  loginOnly();
int  batchMode();
int testLogin();
     
int main(int argc, char **argv)
{ 
  if(config.interpretArguments(argc,argv)==false) return 1;
  
  // /*for debug only: */ config.printDebugInfo(); return 0; 
  if(config.beQuiet()==false)
    {
      std::cout << "rascontrol: rasdaman server remote control utility. rasdaman v" << RMANVERSION / 1000. << " -- generated on " << COMPDATE << "." << std::endl;
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
     }
  if(config.showHelp())
  {
    config.printHelp();
    return 0;
  }
     
  if(testIsMessageDigestAvailable("MD5")==false)
    {
      std::cout<<"Error: Message Digest MD5 not available."<<std::endl;
      return 2;
    } 
     
  if(!isatty(0) )
      redirStdin =true;
  if(!isatty(1))
      redirStdout=true;

  httpClient.setRasMgrHost(config.getRasMgrHost(), config.getRasMgrPort());  
  
  int loginOK = (config.getLoginModus()==LGIINTERACTIV)  ? userLogin.interactiveLogin() : userLogin.environmentLogin();
    
  if(loginOK<0)
  {
    std::cout<<"Login error."<<std::endl;
    return LOGINERROR;
  }
  
  httpClient.setUserIdentification(userLogin.getUserName(),userLogin.getEncrPass());

  int answer=NOERROR;
  switch(config.getWorkModus())
    { 
      case WKMINTERACTIV: answer=interactiveWork(); break;
      case WKMLOGIN     : answer=loginOnly(); break;
      case WKMBATCH     : answer=batchMode(); break;
      case WKMTESTLOGIN : answer=testLogin(); break;
     }

  if(config.beQuiet()==false)
    std::cout<< "rascontrol terminated." <<std::endl;
  
  return answer;   
  } // main()
  
// check whether command fits with given keyword (case insensitive), maybe after stripping leading whitespace
bool isCommand( const char *command, const char *key)
  {
    bool result;

    if (command == NULL || key == NULL)
        result = false;
    else
      {
          while (*command == ' ' || *command == '\t')	// skip white space; newline cannot occur
              command++;
          result = strcasecmp( command, key );
      }

    return result;
  }

int interactiveWork()
  {
    int result = 0;
    const char *answer = NULL;
    int comm = COMM_CONT;			// COMM_* values ; should be enum

    std::ofstream history;
    bool historyFile=config.histRequest();

    comm = httpClient.sendMessageGetAnswer( RASMGRCMD_HELLO, &answer );
    
    if(answer)
        std::cout<<"    "<<answer<<std::endl;
    
    if(comm==COMM_ERR)
     { std::cout<<"Cannot connect to main host "<<httpClient.getRasMgrHost()<<std::endl;
       return CANTCONNECT;
      }
    if(comm==COMM_ACDN)
     { //std::cout<<"Access denied."<<std::endl;
       return ACCESSDENIED;
      }

    // shifted here, after return, so that it always will be closed
    if(historyFile)
     {
        TALK( "RasControl::interactiveWork: using history file " << config.getHistFileName() );
        history.open(config.getHistFileName(),std::ios::out|std::ios::trunc);
     }

    do{
        TALK( "RasControl::interactiveWork: entering new request cycle." );

        const char *command=editLine.interactiveCommand(config.getPrompt());
        if(command == NULL || strlen(command)==0)	// empty cmd line
            continue;

	if(historyFile)
            history<<command<<std::endl;

        TALK( "RasControl::interactiveWork: cmd=" << command );

        // send message to rasmgr & receive answer
        const char *answer = NULL;
        if ( (isCommand(command,"exit") == 0)
          || (isCommand(command,"quit") == 0)
          || (isCommand(command,"bye" ) == 0) )
          {
            // normalize "quit", "bye" to "exit"; do so after history to maintain original cmd there
            comm = httpClient.sendMessageGetAnswer( RASMGRCMD_EXIT, &answer );
            comm = COMM_EXIT;			// ignore any result, we have to terminate anyway
          }
        else
          {
            comm = httpClient.sendMessageGetAnswer( command, &answer );
          }

        if(answer!=0 && strlen(answer)>0)
	    std::cout<<"    "<<answer<<std::endl;
        else
            answer = "Internal error: cannot decode rasmgr result.";

	// EXPERIMENTAL: close socket as early and as always as possible
        //TALK( "RasControl::interactiveWork: closing socket." );
        //httpClient.closeSocket();
      
        TALK( "RasControl::interactiveWork: comm=" << comm );

    }while( comm != COMM_EXIT && comm != COMM_ACDN);	// COMM_ERR is no reason to terminate interactive session
  
    if(historyFile)
        history.close();

    switch(comm)
    {
      case COMM_ERR:  result = CANTCONNECT;  break;
      case COMM_ACDN: result = ACCESSDENIED; break;
      default:        result = NOERROR;      break;
    }
    return result;
  }
   			     		 	
int loginOnly()
  {
    std::cout<<userLogin.getUserName()<<':'<<userLogin.getEncrPass()<<std::endl;
    // Thats all
    return NOERROR;
   }
int batchMode()
  {
    int result = 0;                             // COMM_* values

    bool fromCommandLine = strlen(config.getCommand()) ? true: false;
    const char *prompt   = !redirStdin ? config.getPrompt():"";
    
    //std::cout<<"batch mode "<<fromCommandLine<<" comm="<<config.getCommand()<<std::endl;
    
    int comm=0;
    do{
      const char *command= fromCommandLine ? config.getCommand():editLine.fromStdinCommand(prompt);  
      
      TALK( "batch: Command=" << command );
    
      if(command == NULL ) return CANTCONNECT;
      
      if(strlen(command)==0 ) 
          if( fromCommandLine)
              return CANTCONNECT;
	  else
              continue; 
    
      bool printCommand=false;	    
    
      if( redirStdout ) printCommand =  true;
      else 
        { if(redirStdin) printCommand =  true;
         }
      
      if(printCommand == true)
          std::cout<<config.getPrompt()<<command<<std::endl;		
    
      // normalize "quit", "bye" to "exit"
      const char *answer = NULL;
      if ( (isCommand(command,"exit") == 0)
        || (isCommand(command,"quit") == 0)
        || (isCommand(command,"bye" ) == 0) )
        {
          // normalize "quit", "bye" to "exit"; do so after history to maintain original cmd there
          comm = httpClient.sendMessageGetAnswer( RASMGRCMD_EXIT, &answer );
          comm = COMM_EXIT;                   // ignore any result, we have to terminate anyway
        }
      else
        {
          comm = httpClient.sendMessageGetAnswer( command, &answer );
        }

      if(answer==0 || strlen(answer)==0)
          answer = "Error: cannot decode rasmgr result.";
      else
        {
          if(comm==COMM_ERR)
              std::cout<<"    "<<"Cannot connect to main host."<<std::endl;
          else if(comm==COMM_ACDN)
              std::cout<<"    "<<"Access denied by rasmgr."<<std::endl;
          else
              std::cout<<"    "<<answer<<std::endl;
        }

    }while(fromCommandLine==false && redirStdin
           && comm != COMM_ERR
           && comm != COMM_ACDN
           && comm != COMM_EXIT); //process the whole  "< file"

    switch(comm)
    {
      case COMM_ERR:  result = CANTCONNECT;  break;
      case COMM_ACDN: result = ACCESSDENIED; break;
      default:        result = NOERROR;      break;
    }
    return result; 
   }

int testLogin()
  {
    int comm = 0;                             // COMM_* values
    const char *answer = NULL;

    comm = httpClient.sendMessageGetAnswer( RASMGRCMD_HELLO, &answer );
    
    if(comm==COMM_ERR)
       { //std::cout<<"Cannot connect to main host "<<httpClient.getRasMgrHost()<<std::endl;
         return CANTCONNECT;
        }
    if(comm==COMM_ACDN)
       { //std::cout<<"Acces denied."<<std::endl;
        return ACCESSDENIED;
        }
   
    std::cout<<"Login OK."<<std::endl;
    return NOERROR;
   }

