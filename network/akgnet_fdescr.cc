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
 * SOURCE: akgnet_fdescr.cc
 *
 * MODULE: akg network
 * CLASS: FileDescriptor
 *
 * COMMENTS:
 *
 */

#include <akgnet_fdescr.hh>

akg::FileDescriptor::FileDescriptor() throw()
{
    fileDescriptor = -1;
    savedErrno = 0;
}

akg::FileDescriptor::~FileDescriptor() throw()
{
    close();
}

int akg::FileDescriptor::operator()() throw()
{
    return fileDescriptor;
}

bool akg::FileDescriptor::isOpen() throw()
{
    return fileDescriptor == -1 ? false:true;
}

void akg::FileDescriptor::close() throw()
{
    if(isOpen())
    {
        ::close(fileDescriptor);
        saveErrno();
    }
    fileDescriptor = -1;
}

int akg::FileDescriptor::write(const void *buffer, int count) throw()
{
    savedErrno = 0;
    DBTALK("FileDescriptor write: "<<buffer<<" count="<<count);
    int nbytes = ::write(fileDescriptor,buffer,count);
    if(nbytes < 0) saveErrno();
    return nbytes;
}

int akg::FileDescriptor::read (void *buffer, int count) throw()
{
    savedErrno = 0;
    DBTALK("FileDescriptor read: "<<buffer<<" count="<<count);
    int nbytes = ::read(fileDescriptor,buffer,count);
    if(nbytes < 0) saveErrno();
    return nbytes;
}

bool akg::FileDescriptor::setNonBlocking(bool  nonBlocking) throw()
{
    if(isOpen())
    {
        int val  = fcntl(fileDescriptor,F_GETFL,0);

        if( nonBlocking) val |= O_NONBLOCK;
        else             val &=~O_NONBLOCK;

        fcntl(fileDescriptor,F_SETFL,val);
        return true;
    }

    return false;
}
bool akg::FileDescriptor::isNonBlocking() throw()
{
    if(isOpen())
    {
        int val = fcntl(fileDescriptor,F_GETFL,0);
        return (val & O_NONBLOCK) ? true:false;
    }
    return false;
}

int akg::FileDescriptor::getErrno() throw()
{
    return savedErrno;
}
void akg::FileDescriptor::saveErrno() throw()
{
    savedErrno=errno;
}


