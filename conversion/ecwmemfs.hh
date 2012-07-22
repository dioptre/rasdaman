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

#ifndef _ECWMEMFS_HH_
#define _ECWMEMFS_HH_

#include "raslib/mddtypes.hh"

class MemoryFileSystem
{
public:
    enum m_Error
    {
        No_Error = 0,
        Error = 1
    };

    MemoryFileSystem();

    ///open file an read into memory - completely
    m_Error open(const char* fileName);

    ///use this memory chunk with mSize bytes length
    m_Error open(const char* memorySource, r_Bytes mSize);

    ///close and deinitialise (delete only my own data)
    m_Error close();

    ///call close
    ~MemoryFileSystem();

    ///read bSize bytes into buffer, only if bSize bytes are available
    m_Error read(void* buffer, r_Bytes bSize);

    ///get current position
    unsigned long long tell();

    ///go to offset bytes from begining of memory
    m_Error seek(unsigned long long offset);

    ///if you use open(const* char) and the pointer is memorySrcName then no file will be read but this pointer
    static const char* memorySrc;

    ///file names which point to this will not read from file system but from memorySrc
    static const char* memorySrcName;

    ///the length of the memorySrc chunk must be specified in memoryLength
    static r_Bytes memorySrcLength;

private:

    ///my memory block
    char* source;
    ///my current position
    char* current;
    ///am i closed
    bool closed;
    ///do i own my memory data
    bool owner;
    ///how long is my memory data
    r_Bytes length;
};

#endif
