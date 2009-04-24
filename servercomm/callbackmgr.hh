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


#ifndef _CALLBACK_MANAGER_
#define _CALLBACK_MANAGER_


//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The class CallBackManager stores function- and context pointers
  that are executed in the order they were registered in when
  calling executePending(). This system replaces the alarm handler
  approach formerly used which suffered from a restrictive
  environment where mallocs and printfs were not legal. Since
  neither call must happen when a callback function is registered
  the maximum number of callbacks must be fixed in the constructor.
*/

class CallBackManager
{
  public:
  /// constructor
  CallBackManager(unsigned int size=1024);
  /**
    Constructor; size is the maximum number of callback slots to
    reserve.
  */

  /// destructor
  ~CallBackManager(void);

  /// Resizes the number of callback slots at run-time
  void setMaximumSize(unsigned int size);

  /// callback function type
  typedef void (*callback_f)(void*);
  /**
    The type of a callback function is void f(void *context). The
    value of context is the one specified when registering. It's
    up to the function to cast the value.
  */

  /// register new callback
  int registerCallback(callback_f function, void *context);
  /**
    Register a new callback function. Returns 0 for OK, -1 if
    the callback table had an overflow. Since it must be possible
    to call this function from an alarm handler there may not be
    any mallocs or prints here.
  */

  /// register new callback, ensuring uniqueness
  int registerUniqueCallback(callback_f function, void *context);
  /**
    Same as registerCallback, but makes sure this callback isn't
    pending already in which case it does nothing.
  */

  /// remove callback
  int removeCallback(callback_f function, void *context);
  /**
    Deregister a callback function. Returns 0 for OK, -1 for
    not found.
  */

  /// Get the number of callback functions registered.
  unsigned int getNumCallbacks(void) const;

  /// execute pending callbacks
  int executePending(void);
  /**
    Execute all pending callback functions and clear the list
    afterwards. Returns the number of callback functions executed.
  */


  private:

  /// find a matching function/context pair
  int findCallback(callback_f function, void *context) const;
  /**
    Searches the pending callbacks for the one specified by the
    function parameters and returns its index if found, -1 otherwise.
  */

  //@{ Internal storage structure for callbacks.
  typedef struct callback_desc_s {
    callback_f function;
    void *context;
  } callback_desc_t;
  //@}

  callback_desc_t *callbacks;

  /// Maximum size of callback list
  unsigned int maxCallbacks;

  /// Currently occupied callback slots
  unsigned int numPending;

  /// Flag that's set to 1 if the callback table overflowed.
  int overflowDetected;
};

#endif
