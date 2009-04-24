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
 * SOURCE: rasmgr_utils_conf.hh
 *
 * MODULE: rascontrol
 * CLASS:  ConfigurationBase
 *
 * PURPOSE:
 *   Base class for other configuration classes, to interpret commandline arguments and environment settings
 *    
 * COMMENTS:
 *
*/

#include "debug-clt.hh"

#include "rasmgr_utils_conf.hh"

ConfigurationBase::ConfigurationBase()
  {
   }
      
bool ConfigurationBase::interpretArguments(int argc, char **argv)
  {
    return false;
   }
      
    
int ConfigurationBase::checkArguments( int argc, char** argv, const char* searchText, int& optionValueIndex )
{
  int found = 0;
  int i=1;

  while( !found && i<argc )
    found = !strcmp( searchText, argv[i++] );

  if( found && i<argc && !strchr(argv[i],'-') )
    optionValueIndex = i;
  else
    optionValueIndex = 0;

  return found;
}
  

void ConfigurationBase::printHelp()
  {
    std::cout<<"Help not available..."<< std::endl;
   }

