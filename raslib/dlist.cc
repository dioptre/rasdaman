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

#include <iostream>
#include <vector>
#include "raslib/dlist.hh"
#include "raslib/minterval.hh"

template <class T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& list)
{
    os << "[ ";
    for (typename std::vector<T>::const_iterator it = list.begin(); it != list.end(); it++)
        os << (*it) << " ";
    os << "]";
    return os;
}

template <class T>
std::ostream& operator<< (const std::vector<T>& list, std::ostream& os)
{
    os << "[ ";
    for (typename std::vector<T>::const_iterator it = list.begin(); it != list.end(); it++)
        os << (*it) << " ";
    os << "]";
    return os;
}

#if defined(SOLARIS) && ! defined(EARLY_TEMPLATE)
//this is here to get around a template instantiation problem on sun
template <>
std::ostream& operator<< (std::ostream& os, const std::vector<r_Minterval>& list)
{
    os << "[ ";
    for (typename std::vector<r_Minterval>::const_iterator it = list.begin(); it != list.end(); it++)
        os << (*it) << " ";
    os << "]";
    return os;
}

template <>
std::ostream& operator<< (const std::vector<r_Minterval>& list, std::ostream& os)
{
    os << "[ ";
    for (typename std::vector<r_Minterval>::const_iterator it = list.begin(); it != list.end(); it++)
        os << (*it) << " ";
    os << "]";
    return os;
}

#endif
