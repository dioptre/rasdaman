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
 * SOURCE: callbackmgr.hh
 *
 * MODULE: servercomm
 * CLASS:  CallBackManager
 *
 * COMMENTS:
 *		No Comments
*/


#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "raslib/rmdebug.hh"
#include "debug.hh"

#include "servercomm/callbackmgr.hh"


CallBackManager::CallBackManager(unsigned int size)
{
  RMDBGONCE(1, RMDebug::module_servercomm, "CallBackManager", "CallBackManager(" << size << ")" )

  callbacks = new callback_desc_t[size];
  maxCallbacks = size;
  numPending = 0;
  overflowDetected = 0;
}


CallBackManager::~CallBackManager(void)
{
  RMDBGONCE(1, RMDebug::module_servercomm, "CallBackManager", "~CallBackManager()" )

  delete [] callbacks;
}


void CallBackManager::setMaximumSize(unsigned int size)
{
  RMDBGONCE(1, RMDebug::module_servercomm, "CallBackManager", "setMaximumSize(" << size << ")" )

  callback_desc_t *newcb = new callback_desc_t[size];

  if (callbacks != NULL)
  {
    if (numPending != 0)
    {
      unsigned int copysize = (numPending > size) ? size : numPending;
      memcpy(newcb, callbacks, copysize * sizeof(callback_desc_t));
      if (copysize < numPending)
	overflowDetected = 1;
    }
    delete [] callbacks;
  }
  callbacks = newcb;
  maxCallbacks = size;
}


int CallBackManager::findCallback(callback_f function, void *context) const
{
  unsigned int i;

  // No system calls from this function!
  for (i=0; i<numPending; i++)
  {
    if ((callbacks[i].function == function) && (callbacks[i].context == context))
      return (int)i;
  }
  return -1;
}


int CallBackManager::registerCallback(callback_f function, void *context)
{
  // No mallocs, debug output, system calls, ... in this function!!!
  if (numPending >= maxCallbacks)
  {
    overflowDetected = 1;
    return -1;
  }

  callbacks[numPending].function = function;
  callbacks[numPending].context = context;

  numPending++;

  return 0;
}


int CallBackManager::registerUniqueCallback(callback_f function, void *context)
{
  if (findCallback(function, context) == -1)
    return registerCallback(function, context);
  else
    return -1;
}


int CallBackManager::removeCallback(callback_f function, void *context)
{
  int i;

  // restrictive environment here too, no system calls!
  i = findCallback(function, context);
  if (i != -1)
  {
    if (i < (int)numPending-1)
      memmove(callbacks+i, callbacks+(i+1), (numPending-i-1)*sizeof(callback_desc_t));
    numPending--;
    return 0;
  }
  return -1;
}


unsigned int CallBackManager::getNumCallbacks(void) const
{
  return numPending;
}


int CallBackManager::executePending(void)
{
  unsigned int i;

  RMDBGENTER(2, RMDebug::module_servercomm, "CallBackManager", "executePending()" )

  for (i=0; i<numPending; i++)
  {
    RMDBGMIDDLE(3, RMDebug::module_servercomm, "CallBackManager", "callback function " << i << "..." );
    callbacks[i].function(callbacks[i].context);
  }

  if (overflowDetected)
  {
    TALK( "CallBackManager::executePending(): overflow detected, number of pending calls: " << numPending );
    RMInit::logOut << "Internal error: callback overflow." << endl;
    overflowDetected = 0;
  }

  i = numPending;
  numPending = 0;

  return i;
}
