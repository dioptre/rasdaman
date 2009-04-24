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
 * SOURCE: rascontrol.hh
 *
 * MODULE: rascontrol
 * CLASS:  
 *
 * PURPOSE:
 *  RasControl is the remote control utility for the rasmgr
 *    
 * COMMENTS:
 *
*/

#ifndef RASCONTROL_H
#define RASCONTROL_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <string>

#include "commline/cmlparser.hh"

#include "../rasmgr/ras_crypto.hh"
#include "rasmgr_utils_comm.hh"
#include "rasmgr_utils_conf.hh"

const int  WKMUNKNOWN       = 0;
const int  WKMINTERACTIV    = 1;
const int  WKMBATCH         = 2;
const int  WKMLOGIN         = 3;
const int  WKMTESTLOGIN     = 4;
const int  WKMCREATE        = 5;
const int  WKMTSLANG        = 6;

const int  LGIUNKNOWN       = 0;
const int  LGIINTERACTIV    = 1;
const int  LGIENVIRONM      = 2;

const int  PROMPTSING       = 0;
const int  PROMPTRASC       = 1;
const int  PROMPTFULL       = 2;


// ----------------
// global constants
// ----------------

#define EOS_CHAR '\0'

// rasmgr command words
#define RASMGRCMD_EXIT       "exit"
#define RASMGRCMD_HELLO      "Hello"
#define RASMGRCMD_LIST_MODUS "list modus"


// interim debug solution, should be replaced by std rasdaman mechanism -- now with ../rasmgr/debug.hh
// #define TALK(a) cout << a << endl;
//#define TALK(a) { /* TALK(a) */ }

void printCommandLineHelp();
int  getCommand(); 
     
int  sendCommandandPrintAnswer(const char*);
bool exitbyerror(char*);
int  createSocket();

class EditLine
  {
    public:
      EditLine();
      ~EditLine();
     
     const char *interactiveCommand(const char *prompt);
     const char *fromStdinCommand(const char *prompt);  
    
    private:
      char *rl_gets(const char *prompt);
      char line[MAXMSG];
        
   };

class RascontrolConfig:public ConfigurationBase
  {
    public:
      RascontrolConfig();
      
      // false means program shouldn't continue
      bool interpretArguments(int argc, char **argv);
      
      int         getWorkModus();
      int         getLoginModus();
      const char* getRasMgrHost();
      int         getRasMgrPort();
      bool        histRequest();
      const char* getHistFileName();
      const char* getPrompt();
      const char* getCommand();
      bool        beQuiet();
      bool        showHelp();      
      
      void printDebugInfo();
      
      void printHelp();
    private:

      bool paramError();
      
      int workModus;
      int loginModus;
      
      char rasmgrHost[100];
      int  rasmgrPort;
      bool reqHist;
      char histFileName[100];
      
      int promptType;
      char prompt[100];
      char command[100];
      bool quiet;

      bool isCommand( const char *command, const char *key );

      //-- parameters of this program
      CommandLineParser    &cmlInter;
      CommandLineParameter &cmlHelp, &cmlHost, &cmlPort, &cmlLogin;
#ifdef NO_OFFICIAL_RELEASE
      CommandLineParameter &cmlHist;
#endif      
      CommandLineParameter &cmlPrompt, &cmlTestLogin;
      CommandLineParameter &cmlInteractive, &cmlQuiet, &cmlExecute;

   };

#endif
