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
 * SOURCE: raspasswd.cc
 *
 * MODULE: rascontrol
 * CLASS:  
 *
 * PURPOSE:
 *   RasDaMan password utility
 *    
 * COMMENTS:
 * 
 *
*/

// for trace macros
#define DEBUG_MAIN
#include "debug-clt.hh"

#include "globals.hh"

#include "rascontrol.hh"
#include "raspasswd.hh"
#include "rasmgr_utils_comm.hh"

#ifndef RMANVERSION
#error "Please specify the RMANVERSION variable!"
#endif

#ifndef COMPDATE
#error "Please specify the COMPDATE variable!"
/*
COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`

and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
*/
#endif

RasPasswdConfig  config;
UserLogin        userLogin;
RasMgrClientComm rasmgrComm;

char message[100];
char encrNewPass1[35];
char encrNewPass2[35];


int main(int argc, char **argv)
{ 
  cout << "raspasswd: rasdaman password utility. rasdaman v" << RMANVERSION / 1000. << " -- generated on " << COMPDATE << "." <<endl;
 std::cout << " Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann rasdaman GmbH." << std::endl
          << "Rasdaman community is free software: you can redistribute it and/or modify "
             "it under the terms of the GNU General Public License as published by "
             "the Free Software Foundation, either version 3 of the License, or "
             "(at your option) any later version. \n"
             "Rasdaman community is distributed in the hope that it will be useful, "
             "but WITHOUT ANY WARRANTY; without even the implied warranty of "
             "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
             "GNU General Public License for more details. \n\n";

  cout << "This software contains software which is in the public domain:" << endl;
  cout << "- openssl 0.96c (C) 1998-2002 The OpenSSL Project, (C) 1995-1998 Eric A. Young, Tim J. Hudson" << endl;

  if( config.interpretArguments(argc, argv) == false) return 0;
  
  if(testIsMessageDigestAvailable("MD5")==false)
    { cout<<"No MD5-Algorithm."<<endl;
      return 2;
     } 
  
  if(userLogin.interactiveLogin()<0) return 1;
  
  rasmgrComm.setRasMgrHost(config.getRasMgrHost(),config.getRasMgrPort());
  rasmgrComm.setUserIdentification(userLogin.getUserName(),userLogin.getEncrPass());
  
  if(execute( RASMGRCMD_HELLO ) == false) return 3;
  
  getPasswdKeyboard(" Type your new password: ",encrNewPass1);
  getPasswdKeyboard(" Retype the new password: ",encrNewPass2);
  
  if(strcmp(encrNewPass1,encrNewPass2)!=0)
    { cout<<"Passwords don't match."<<endl;
      return 2;
     }
  
 // cout<<"name="<<username<<" pass="<<encrPass<<endl;
  
  sprintf(message,"change user %s -encrPasswd %s ",userLogin.getUserName(),encrNewPass1);
  
  if(execute(message) == false) return 3;
    
  return 0;
}

bool execute(const char *message)
  {
    bool result = true;				// function result
    int comm = 0; 				// COMM_* values
    const char *answer = NULL;			// rasmgr response

    comm = rasmgrComm.sendMessageGetAnswer(message, &answer);
  
    switch(comm)
      { 
        case COMM_ERR:
            cout << "Cannot connect to rasmgr host "<<config.getRasMgrHost() <<"." << endl;
            result = false;
            break; 
        case COMM_ACDN:
            cout << "Access denied." << endl;
            result = false;
            break; 
        default:				// then it's ok
            break;
     }
    cout << rasmgrComm.getBody() << endl;
  
    return result;
  } 

const char* getPasswdKeyboard(const char*text, char*dest)
  {
    char *plainPass=getpass(text);
    messageDigest(plainPass,dest,"MD5");
    for(int i=0;i<strlen(plainPass);i++) plainPass[i]=0;
    //cout<<endl;
    return dest;
   }


bool exitbyerror(char* text)
{ perror(text);return false;
}

//################ Config ##################################
RasPasswdConfig::RasPasswdConfig() :
 cmlInter        (CommandLineParser::getInstance()),
 cmlHost         (cmlInter.addStringParameter(CommandLineParser::noShortName,"host", "<name> name of host where master rasmgr runs", DEFAULT_HOSTNAME)),
 cmlPort         (cmlInter.addLongParameter(CommandLineParser::noShortName,"port", "<nnnn> the rasmgr port", DEFAULT_PORT )),
 cmlHelp         (cmlInter.addFlagParameter('h',"help","this help"))
  {
// done by default value of commandlineparser
//    strcpy(rasmgrHost,RASMGRHOST.c_str());
//    rasmgrPort = RASMGRPORT;
   }
   
bool RasPasswdConfig::interpretArguments(int argc, char **argv)
  {
    try {
      cmlInter.processCommandLine(argc, argv);
    }
    catch(CmlException& err) {
      cerr << "Command Line Parsing Error:" << endl << err.what() << endl;
      return false;
    }
    
    if( cmlHelp.isPresent() ) { 
       printHelp();
       return false;
    } 
   
    try { 
      rasmgrPort = cmlPort.getValueAsLong();
    }
    catch(CmlException& err) {
      cerr << "Command Line Parsing Error:" << endl << err.what() << endl;
      return false;
    }
       
    strcpy(rasmgrHost, cmlHost.getValueAsString());
       
    return true;  
   }
   
void RasPasswdConfig::printHelp()
  {
    cout << "Usage: raspasswd [options]"<<endl;
    cout << "Options:" << endl;
    cmlInter.printHelp();
    cout<<endl;
   }

const char* RasPasswdConfig::getRasMgrHost()
  { return rasmgrHost;
   }
int RasPasswdConfig::getRasMgrPort()
  { return rasmgrPort;
   }

/*
void RasPasswdConfig::printDebugInfo()
  {
          cout<<"Working modus:.";
    switch(workModus)
      {
        case WKMUNKNOWN     : cout<<"unknown";break;
        case WKMINTERACTIV  : cout<<"interactiv";break;
        case WKMBATCH       : cout<<"batch";break;
        case WKMLOGIN       : cout<<"login";break;
       }
       
    cout<<endl<<"Login modus:...";
    switch(loginModus)
      { 
        case LGIUNKNOWN     : cout<<"unknown";break;
        case LGIINTERACTIV  : cout<<"interactiv";break;
        case LGIENVIRONM    : cout<<"environm";break;
       }
      
    cout<<endl<<"Rasmgr:........"<<rasmgrHost<<":"<<rasmgrPort;
    
    cout<<endl<<"History:......."<<(reqHist ? histFileName : "not requested");
    
    cout<<endl<<"Prompt:........"<<getPrompt();
    
    cout<<endl<<"Command:......."<<command;
    
    cout<<endl;    
   }
*/   
