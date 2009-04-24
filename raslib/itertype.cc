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

#include "raslib/itertype.hh"
#include <stdlib.h>

template<class T>
r_IterType<T>::r_IterType() : lastElem(NULL), myElems(NULL), currPos(0)
{
}

template<class T>
r_IterType<T>::r_IterType(T* newLastElem, T* newElems)
  : lastElem(newLastElem), myElems(newElems), currPos(newElems)
{
}

template<class T>
r_IterType<T>::r_IterType(T* newLastElem, T* newElems, T* newCurrPos)
  : lastElem(newLastElem), myElems(newElems), currPos(newCurrPos)
{
}

template<class T>
r_IterType<T>::r_IterType( const r_IterType<T>& iter )
  : lastElem(iter.lastElem), myElems(iter.myElems), currPos(iter.currPos)
{
}

template<class T>
r_IterType<T>::~r_IterType()
{
  // nothing to do, memory management is done by r_Attribute.
}

template<class T> r_IterType<T>& 
r_IterType<T>::operator=( const r_IterType<T>& iter )
{
  lastElem = iter.lastElem;
  myElems = iter.myElems;
  currPos = iter.currPos;
  return *this;
}

template<class T> bool 
r_IterType<T>::operator==( const r_IterType<T>& otherIter )
{
  return currPos == otherIter.currPos;
}

template<class T> bool
r_IterType<T>::operator!=( const r_IterType<T>& otherIter )
{
  return currPos != otherIter.currPos;
}

template<class T> r_IterType<T>& 
r_IterType<T>::operator++()
{
  currPos++;
  return *this;
}

template<class T> r_IterType<T>
r_IterType<T>::operator++( int )
{
  r_IterType<T> result( *this );
  operator++();
  return result;
}

template<class T> T 
r_IterType<T>::operator*()
{
  return *currPos;
}

