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
#include "dbobjectiditerator.hh"
#include "objectbroker.hh"
#include "dbref.hh"
#include "raslib/rmdebug.hh"

template<class T>
DBObjectIdIterator<T>::DBObjectIdIterator(const DBObjectIdIterator<T>& oidlist)
    :   mySet(NULL),
        counter(0)
{
    RMDBGONCE(7, RMDebug::module_adminif, "DBObjectIdIterator", "DBObjectIdIterator(const DBObjectIdIterator<T>&)");
    mySet = oidlist.mySet;
    myIter = mySet->begin();
}

template<class T>
DBObjectIdIterator<T>::DBObjectIdIterator(const std::set<DBRef<T>, std::less<DBRef<T> > >& oidlist)
    :   mySet(NULL),
        counter(0)
{
    RMDBGONCE(7, RMDebug::module_adminif, "DBObjectIdIterator", "DBObjectIdIterator(OIdSet)");
    mySet = (std::set<DBRef<T>, std::less<DBRef<T> > >*)&oidlist;
    myIter = mySet->begin();
}

template<class T> void
DBObjectIdIterator<T>::reset()
{
    RMDBGONCE(7, RMDebug::module_adminif, "DBObjectIdIterator","reset()");
    myIter = mySet->begin();
    counter = 0;
}

template<class T> bool
DBObjectIdIterator<T>::not_done() const
{
    RMDBGENTER(7, RMDebug::module_adminif, "DBObjectIdIterator","not_done()");
    bool retval = false;
    if (myIter == mySet->end())
    {
        retval = false;
    }
    else
    {
        if (mySet->empty())
        {
            retval = false;
        }
        else
        {
            if (counter == mySet->size())
                retval = false;
            else
                retval = true;
        }
    }
    RMDBGEXIT(7, RMDebug::module_adminif, "DBObjectIdIterator","not_done() " << retval);
    return retval;
}

template<class T> void
DBObjectIdIterator<T>::advance()
{
    RMDBGONCE(7, RMDebug::module_adminif, "DBObjectIdIterator","advance() " << counter);
    myIter++;
    counter++;
}

template<class T> DBRef<T>
DBObjectIdIterator<T>::get_element() const
{
    RMDBGONCE(7, RMDebug::module_adminif, "DBObjectIdIterator", "get_element() " << (*myIter).getOId());
    return (*myIter);
}

template<class T>
DBObjectIdIterator<T>::~DBObjectIdIterator()
{
    RMDBGONCE(7, RMDebug::module_adminif, "DBObjectIdIterator", "~DBObjectIdIterator()");
    mySet = NULL;
    counter = 0;
}

