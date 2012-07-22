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
 * INCLUDE: akgnet_commbuffer.hh
 *
 * MODULE:  akg network
 * CLASS:   CommBuffer
 *
 * COMMENTS:
 * Namespace akg
 *
*/

#ifndef AKGNET_BUFFER_HH
#define AKGNET_BUFFER_HH

#include "akgnet_fdescr.hh"

namespace akg
{

/** This class is a specialized buffer used for communication by the elements
    of this library.
    Important:
        - if the internal buffer overflows, it is not reallocated, even there is a resize() function. This is not an error!
      Please think twice before you change this
        - 'new' is supposed to throw
*/
/**
  * \defgroup Networks Network Classes
  */

/**
  * \ingroup Networks
  */

class CommBuffer
{
public:
    /// Default constructor, no data is allocated
    CommBuffer() throw();

    /** Constructor allocating a 'size' bytes buffer
        Assert: size > 0
    */
    CommBuffer(int size)  throw();

    /** Constructor taking over an existing buffer
        totalSize - the total size of the buffer
    dataSize  - the size of the usefull data
    */
    CommBuffer(void*,int totalSize,int dataSize=0) throw();

    /// Destructor. If the internal buffer is allocated, it will be deallocated
    ~CommBuffer() throw();

    /** Allocates a new buffer. The old one, if allocated, will be deallocated
        Assert: size > 0
    */
    bool  allocate(int size) throw();

    /// Frees the internal buffer, if allocated.
    void  freeBuffer() throw();

    /** Takes over an external buffer
        totalSize - the total size of the buffer
    dataSize  - the size of the usefull data
    Assert: externalBuffer != 0, totalSize > 0, dataSize>=0, totalSize>=dataSize
    */
    void takeOver(void *externalBuffer,int totalSize,int dataSize=0) throw();

    /** Resizes the internal buffer, by allocating and copying the data.
        Returns false if the new buffer is smaller than the actual data
    Assert: there is a buffer, so 'data != 0'*/
    bool resize(int newSize) throw();

    /// Returns a pointer to the internal buffer. You are on your own!
    void* getData() throw();

    /// Returns the size of the data stored in the buffer
    int   getDataSize() throw();

    /// Returns the total capacity of the buffer
    int   getBufferSize() throw();

    /// Returns the size already written
    int   getSendedSize() throw();

    /// Returns the size not filled yet
    int   getNotFilledSize() throw();

    /// Returns the size not written yet
    int   getNotSendedSize() throw();

    /// Returns true if the internal buffer is allocated
    bool  isAllocated() throw();

    /** Reads as much as possible from the specified FileDescriptor
        It stops if the buffer is full or there are are no more bytes to read
    Returns the number of bytes red
    */
    int   read(FileDescriptor&) throw();

    /** Reads at most 'size' bytes from the specified memory address
        It stops if the buffer is full or there are are no more bytes to read
    Returns the number of bytes red
    Assert: externalBuffer != 0, size >=0
    */
    int   read(const void *externalBuffer,int size) throw();

    /** Fake read, used to reserve space for future direct write
        Returns the number of bytes reserverd, which can be less
    than 'size' if there is not enough space
    Assert: size >=0
    */
    int   reserve(int size) throw();


    /** Write as much as possible to the specified FileDescriptor
        It stops if there are no more bytes to write or the FileDescriptor
    can't accept more bytes
    Returns the number of bytes written
    */
    int   write(FileDescriptor&) throw();

    /** Writes at most 'size' bytes to the specified memory address
        It stops if the there are no more bytes to write
    Returns the number of bytes written
    Assert: externalBuffer != 0, size >=0
    */
    int   write(void *externalBuffer,int size) throw();

    /// Resets the buffer for reading. The data inside is discarded
    void  clearToRead() throw();

    /** Resets the buffer for writing. The data inside is not touched,
        just the writing counters are reset
    */
    void  clearToWrite() throw();

private:
    /// the internal buffer
    char *data;
    /// the size of the internal buffer
    int  buffSize;
    /// the filled size
    int  fillSize;
    /// the send size.
    int  sendSize;
    /// is the buffer allocated?
    bool allocated;
};

} //namespace
#endif

