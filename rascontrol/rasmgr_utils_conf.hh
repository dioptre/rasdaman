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
 *
*/
#ifndef RASMGR_UTILS_CONF_HH
#define RASMGR_UTILS_CONF_HH

// This is the base class for every programs configuration class
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

class ConfigurationBase
{
public:
    ConfigurationBase();

    // false means program shouldn't continue
    virtual bool interpretArguments(int argc, char **argv);

protected:

    int checkArguments( int argc, char** argv, const char* searchText, int& optionValueIndex );

    virtual void printHelp();

};

#endif
