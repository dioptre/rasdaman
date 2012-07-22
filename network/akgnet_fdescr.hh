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
 * INCLUDE: akgnet_fdescr.hh
 *
 * MODULE:  akg network
 * CLASS:   FileDescriptor
 *
 * COMMENTS:
 * Namespace akg
 *
*/

#ifndef AKGNET_FDESCR_HH
#define AKGNET_FDESCR_HH

#include "akgnet_common.hh"

namespace akg
{

/**
  This class the base class for a hierarchie, which are
  envelopes for the usual OS file descriptors. They offer
  only that much functionallity as needed for our library
  The objects of this hierarchie can't be copied!
*/

/**
  * \ingroup Networks
  */

class FileDescriptor
{
public:
    /// Destructor, if open, closes the file descriptor
    ~FileDescriptor() throw();

    /// Returns the OS file descriptor
    int  operator()() throw ();

    /** Returns true if the descriptor is open.
        Be aware that closing the file descriptor by
    using SO specific functions instead of the
    methods of this class can lead to incorrect results
    */
    bool isOpen() throw();

    /// Closes the descriptor
    void close()  throw();

    /// Returns the error number of the last operation
    int  getErrno() throw();

    /** Writes the specified number of bytes from the
        specified buffer.
    Returns the number of bytes actually written
    */
    int  write(const void *buffer, int count) throw();

    /** Reads the specified number of bytes into the
        specified buffer.
    Returns the number of bytes actually read
    */
    int  read (void *buffer, int count) throw();

    /** Sets the non-blocking mode on or off
        Returns true o succes
    */
    bool setNonBlocking(bool nonBlocking) throw();

    ///  Returns true if the descriptors is in non-blocking mode
    bool isNonBlocking() throw();
protected:
    /// Protected constructor
    FileDescriptor() throw();

    /// Saves the errno
    void saveErrno() throw();

    int  fileDescriptor;
    int  savedErrno;
private:
    /// unimplemented, objects can't be copied
    FileDescriptor(const FileDescriptor&);
    /// unimplemented, objects can't be copied
    FileDescriptor& operator=(const FileDescriptor&);
};

} // namespace
#endif
