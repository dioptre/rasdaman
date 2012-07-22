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
 * INCLUDE: akgnet_selector.hh
 *
 * MODULE:  akg network
 * CLASS:   Selector
 *
 * COMMENTS:
 * Namespace akg
 *
*/

#ifndef AKGNET_SELECTOR
#define AKGNET_SELECTOR

#include "akgnet_common.hh"

namespace akg
{

/** This class envelops the 'select' function of the C-library
*/

/**
  * \ingroup Networks
  */

class Selector
{
public:
    /// Default constructor
    Selector() throw();

    /// Sets the timeout interval
    void setTimeout(int sec,int milisec) throw();

    /// Disables the timeout
    void disableTimeout() throw();

    /// Registers the file descriptor for reading
    void setRead(int fdescr) throw();
    /// Unregisters the file descriptor from reading
    void clearRead(int fdescr) throw();

    /// Registers the file descriptor for writing
    void setWrite(int fdescr) throw();
    /// Unregisters the file descriptor from writing
    void clearWrite(int fdescr) throw();

    /// The real 'select' operation. The return value is the one of 'select'
    int operator()() throw();

    /// Returns true if the file descriptor is registered for read
    bool isRead(int fdescr) throw();
    /// Returns true if the file descriptor is registered for write
    bool isWrite(int fdescr) throw();

    /** Closes all file descriptors. Usefull when forking so
        child processes don't inherit this file descriptors
    */
    void closeForcedAllFileDescriptors() throw();
private:
    fd_set watchReadFdSet;
    fd_set watchWriteFdSet;
    fd_set watchExceptFdSet; // unused but ...

    fd_set resultReadFdSet;
    fd_set resultWriteFdSet;
    fd_set resultExceptFdSet; // unused but ...

    struct timeval tvinit;
    struct timeval tv;
    timeval *tvptr;

};

} // namespace
#endif
