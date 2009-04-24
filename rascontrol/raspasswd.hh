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
 * SOURCE: raspasswd.hh
 *
 * MODULE: rascontrol
 * CLASS:  
 *
 * PURPOSE:
 *   RasDaMan password utility
 *    
 * COMMENTS:
 * 
*/

#ifndef RASPASSWD_HH
#define RASPASSWD_HH

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
#include <string>

#include "commline/cmlparser.hh"

#include "../rasmgr/ras_crypto.hh"
#include "rasmgr_utils_conf.hh"

using namespace std;

void printCommandLineHelp();
     
bool exitbyerror(char*);

const char* getPasswdKeyboard(const char*text,char*dest);
bool  execute(const char *message);

class RasPasswdConfig:public ConfigurationBase
  {
    public:
      RasPasswdConfig();
      
      // false means program shouldn't continue
      bool interpretArguments(int argc, char **argv);
      
      const char* getRasMgrHost();
      int         getRasMgrPort();
      // void printDebugInfo();
    private:

      void printHelp();
      //bool paramError();
      
      char rasmgrHost[100];
      int  rasmgrPort;
      
      //-- parameters of this program
      CommandLineParser    &cmlInter;
      CommandLineParameter &cmlHost, &cmlPort, &cmlHelp;
   };

#endif
