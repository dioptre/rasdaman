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
 * INCLUDE: itertype.hh
 *
 * MODULE:  raslib
 * CLASS:   r_IterType
 *
 * COMMENTS:
 *
*/

#ifndef _D_ITER_TYPE_
#define _D_ITER_TYPE_

//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class realizes the iterator used to access
  single elements of \Ref{r_Structure_Type}.
*/

template <class T>
class r_IterType
{
public:
    /// default constructor
    r_IterType();
    /// constructor used in r_Structure_Type
    r_IterType( T* newLastElem, T* newElems );
    /// constructor used in r_Structure_Type
    r_IterType( T* newLastElem, T* newElems, T* newCurrPos );
    /// copy constructor
    r_IterType( const r_IterType<T>& iter );
    /// destructor
    ~r_IterType();
    /// assignment operator
    r_IterType<T>& operator=( const r_IterType<T>& iter );
    /// equal comparison: equal if they point to the same element.
    bool operator==( const r_IterType<T>& otherIter );
    /// no equal comparison: not equal if they point to different elements
    bool operator!=( const r_IterType<T>& otherIter );
    /// prefix incrementor
    r_IterType<T>& operator++();
    /// postfix incrementor
    r_IterType<T> operator++( int );
    /// the dereference operator gets the actual element
    T operator*();

protected:
    T* lastElem;
    T* myElems;
    T* currPos;
};

#ifdef EARLY_TEMPLATE
#ifdef __EXECUTABLE__
#ifdef __VISUALC__
#include "itertype.cpp"
#else
#include "itertype.cc"
#endif
#endif
#endif

#endif
