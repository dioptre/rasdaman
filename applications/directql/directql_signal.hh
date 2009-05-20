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

/* *
 * COMMENTS:
 *
 *		No comments
*/

#ifndef _RASQL_SIGNAL_HH_
#define _RASQL_SIGNAL_HH_

#include <signal.h>

//signalCleanup function is called when a signal is received by the program. 
//You should write your function in order to have signal management
void signalCleanup();

//signalHandler function is called when a signal occurs
void
signalHandler(int sig);

//installSignalHandlers function should be called first in main function
//in order to receive signal in your program
void
installSignalHandlers();

#endif _RASQL_SIGNAL_HH_

