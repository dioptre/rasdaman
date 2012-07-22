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

#include "akgnet_selector.hh"


akg::Selector::Selector() throw()
{
    FD_ZERO(& watchReadFdSet);
    FD_ZERO(& watchWriteFdSet);
    FD_ZERO(& watchExceptFdSet);
    tvptr = NULL;
}
void akg::Selector::setTimeout(int sec,int milisec) throw()
{
    tvinit.tv_sec=sec;
    tvinit.tv_usec=milisec;
    tvptr=&tv; // yes, yes, &tv
}
void akg::Selector::disableTimeout() throw()
{
    tvptr=NULL;
}
void akg::Selector::setRead(int fdescr) throw()
{
    if(fdescr < 0) return;
    FD_SET(fdescr,&watchReadFdSet);
}
void akg::Selector::clearRead(int fdescr) throw()
{
    if(fdescr < 0) return;
    FD_CLR(fdescr,&watchReadFdSet);
}
void akg::Selector::setWrite(int fdescr) throw()
{
    if(fdescr < 0) return;
    FD_SET(fdescr,&watchWriteFdSet);
}
void akg::Selector::clearWrite(int fdescr) throw()
{
    if(fdescr < 0) return;
    FD_CLR(fdescr,&watchWriteFdSet);
}

int akg::Selector::operator()() throw()
{
    resultReadFdSet =watchReadFdSet;
    resultWriteFdSet=watchWriteFdSet;
    // error unused
    // tv has to be reloaded every time; if tvptr is NULL it doesn't matter
    tv.tv_sec  = tvinit.tv_sec;
    tv.tv_usec = tvinit.tv_usec;

    return select(FD_SETSIZE,&resultReadFdSet,&resultWriteFdSet,NULL,tvptr);
}

bool akg::Selector::isRead(int fdescr) throw()
{
    if(fdescr < 0) return false;
    return FD_ISSET(fdescr,&resultReadFdSet);
}
bool akg::Selector::isWrite(int fdescr) throw()
{
    if(fdescr < 0) return false;
    return FD_ISSET(fdescr,&resultWriteFdSet);
}

void akg::Selector::closeForcedAllFileDescriptors() throw()
{
    for(int i=0; i<FD_SETSIZE; i++)
    {
        if(FD_ISSET(i,&watchReadFdSet) || FD_ISSET(i,&watchWriteFdSet))
        {
            close(i);
        }
    }
}


