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
 * SOURCE: akgnet_commbuffer.cc
 *
 * MODULE: akg network
 * CLASS:  CommBuffer
 *
 * COMMENTS:
 *
 */

#include <akgnet_commbuffer.hh>
#include <string>
#include <assert.h>


akg::CommBuffer::CommBuffer() throw()
{
    data     = NULL;
    buffSize = 0;
    fillSize = 0;
    sendSize = 0;
    allocated= false;
}

akg::CommBuffer::CommBuffer(int size) throw()
{
    assert(size > 0);
    allocate(size);
}

akg::CommBuffer::CommBuffer(void *externalBuffer,int totalSize, int dataSize) throw()
{
    data = NULL;
    takeOver(externalBuffer,totalSize,dataSize);
}

akg::CommBuffer::~CommBuffer() throw()
{
    freeBuffer();
}

bool  akg::CommBuffer::allocate(int size) throw()
{
    assert(size > 0);

    freeBuffer();
    data = new char[size];
    // new throws?
    buffSize=size;
    allocated=true;
    return true;
}

void akg::CommBuffer::freeBuffer() throw()
{
    if(allocated == true && data != NULL) delete[] data;
    data     = NULL;
    buffSize = 0;
    fillSize = 0;
    sendSize = 0;
    allocated= false;
}

void akg::CommBuffer::takeOver(void *externalBuffer,int totalSize, int dataSize) throw()
{
    assert(externalBuffer != 0);
    assert(totalSize > 0);
    assert(dataSize >= 0);
    assert(totalSize >= dataSize);

    freeBuffer();
    data     = (char*)externalBuffer;
    buffSize = totalSize;
    fillSize = dataSize;
}

bool akg::CommBuffer::resize(int newSize) throw()
{
    assert(data != 0);

    // we can't make the buffer smaller by truncating inside data!
    if(newSize < fillSize) return false;

    char *newData = new char[newSize];
    memcpy(newData, data, fillSize);
    if(allocated == true ) delete[] data;

    data      = newData;
    buffSize  = newSize;
    allocated = true;
    return true;
}

void* akg::CommBuffer::getData()          throw()
{
    return data;
}
int   akg::CommBuffer::getDataSize()      throw()
{
    return fillSize;
}
int   akg::CommBuffer::getBufferSize()    throw()
{
    return buffSize;
}
int   akg::CommBuffer::getSendedSize()    throw()
{
    return sendSize;
}
int   akg::CommBuffer::getNotFilledSize() throw()
{
    return buffSize-fillSize;
}
int   akg::CommBuffer::getNotSendedSize() throw()
{
    return fillSize-sendSize;
}
bool  akg::CommBuffer::isAllocated()      throw()
{
    return allocated;
}

int akg::CommBuffer::read(FileDescriptor &socket) throw()
{
    int rasp = socket.read(data+fillSize,buffSize-fillSize);

    if(rasp>=0) fillSize += rasp;

    return rasp;
}

int akg::CommBuffer::read(const void *externalBuffer,int size) throw()
{
    assert(externalBuffer != 0);
    assert(size >= 0);

    int cpSize = size<(buffSize-fillSize) ? size:(buffSize-fillSize);

    memcpy(data+fillSize,externalBuffer,cpSize);
    fillSize += cpSize;

    return cpSize;
}

int akg::CommBuffer::reserve(int size) throw()
{
    assert(size >= 0);

    int cpSize = size<(buffSize-fillSize) ? size:(buffSize-fillSize);

    fillSize += cpSize;

    return cpSize;
}

int akg::CommBuffer::write(FileDescriptor &socket) throw()
{
    DBTALK("CommBuffer write fillSize="<<fillSize<<" sendSize="<<sendSize);
    int rasp = socket.write(data+sendSize,fillSize-sendSize);

    if(rasp>=0) sendSize+=rasp;

    return rasp;
}

int akg::CommBuffer::write(void *externalBuffer,int size) throw()
{
    assert(externalBuffer != 0);
    assert(size >= 0);

    int cpSize = size<(fillSize-sendSize) ? size:(fillSize-sendSize);

    memcpy(externalBuffer,data+sendSize,cpSize);
    sendSize+=cpSize;

    return cpSize;
}

void akg::CommBuffer::clearToRead() throw()
{
    DBTALK("CommBuffer clearToRead");
    fillSize=0;
    sendSize=0;
}
void akg::CommBuffer::clearToWrite() throw()
{
    DBTALK("CommBuffer clearToWrite");
    sendSize=0;
}

